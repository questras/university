#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <regex>
#include <fstream>
#include <vector>

#include "message.h"
#include "constants.h"
#include "correlated_server.h"
#include "parsing.h"
#include "log.h"

using std::stringstream;
using std::string;


// Read from `msg_sock` until CRLF characters are found.
// In `bufferString` there should be data that was left after reading during previous
// calls of this function.
// Data until CRLF is returned in `bufferStream`, data after CRLF is
// returned in `bufferString`.
// Return 0 if CRLF found, -1 otherwise.
int read_until_crlf(stringstream &bufferStream, string &bufferString, int msg_sock) {
    ssize_t len;
    char temp_buffer[BUFFER_SIZE];

    while (true) {
        bufferStream << bufferString;
        bufferString = bufferStream.str();
        auto pos = bufferString.find(CRLF);
        if (pos != string::npos) {
            bufferStream.str(bufferString.substr(0, pos + 2));
            bufferString = bufferString.substr(pos + 2);
            return 0;
        }

        // Read from socket.
        len = read(msg_sock, temp_buffer, sizeof(temp_buffer));
        if (len < 0) {
            server_error("Reading from client socket.");
        }
        else if (len == 0) {
            // Read length 0 before finding CRLF.
            return -1;
        }
        bufferString = string(temp_buffer, len);
    }
}


// Send response to msg_sock.
// Return true if response was sent successfully, false otherwise.
bool send_response(Response &response, int msg_sock) {
    string responseString = response.toString();

    ssize_t snd_len = write(msg_sock, responseString.c_str(), responseString.length());
    if (snd_len < 0 || (size_t)snd_len != responseString.length()) {
        server_info("Writing to client socket failed.");
        return false;
    }

    return true;
}


int main(int argc, char *argv[]) {
    // Command line arguments.
    string absoluteBaseDir, correlatedServersFile;
    int port;
    parse_programme_arguments(argc, argv, absoluteBaseDir, correlatedServersFile, port);
    CorrelatedServers correlatedServers(correlatedServersFile);

    // Create IPv4 TCP socket.
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        server_error("Socket could not be created.");
    }

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t client_address_len;

    server_address.sin_family = AF_INET;
    // Listen on all interfaces.
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    // Listen on given port.
    server_address.sin_port = htons(port);

    // Bind the socket to a concrete address.
    if (bind(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        server_error("Socket binding failed.");
    }

    // Start to listen.
    if (listen(sock, QUEUE_LENGTH) < 0) {
        server_error("Socket listening failed.");
    }

    server_info("Accepting client connections on port:");
    server_info(std::to_string(ntohs(server_address.sin_port)));

    int msg_sock;
    for (;;) {
        client_address_len = sizeof(client_address);
        // Accept client connection from the socket.
        msg_sock = accept(sock, (struct sockaddr *) &client_address, &client_address_len);
        if (msg_sock < 0) {
            server_info("Failed to accept client, waiting for another.");
            continue;
        }

        server_info("Read requests from connected client.");
        string bufferString;
        while (true) {
            Response responseMessage;
            stringstream bufferStream;

            // Read start-line.
            if (-1 == read_until_crlf(bufferStream, bufferString, msg_sock)) {
                if (bufferString.empty()) {
                    server_info("Read 0 bytes.");
                    break;
                }
                else {
                    server_info("Bad request: read incorrect start-line.");
                    responseMessage = MessageFactory::createBadFormatResponse();
                    send_response(responseMessage, msg_sock);
                    break;
                }
            }
            string start_line = bufferStream.str();
            bufferStream.str(string());

            // Parse start-line.
            string method, filepath;
            if (-1 == parse_start_line(start_line, method, filepath)) {
                server_info("Bad request: cannot parse start-line.");
                responseMessage = MessageFactory::createBadFormatResponse();
                send_response(responseMessage, msg_sock);
                break;
            }

            // Read headers.
            std::vector<std::pair<string, string>> headers;
            string header_line, header_name, header_value;
            bool is_bad_request = false;

            // Read one header at a time.
            while (true) {
                if (-1 == read_until_crlf(bufferStream, bufferString, msg_sock)) {
                    server_info("Bad request: read incorrect header.");
                    responseMessage = MessageFactory::createBadFormatResponse();
                    send_response(responseMessage, msg_sock);
                    is_bad_request = true;
                    break;
                }

                header_line = bufferStream.str();
                if (header_line == CRLF) {
                    // Read last CRLF after headers, end of headers.
                    if (bufferString.length() > 2) {
                        // Update buffer to not contain this CRLF.
                        bufferString = bufferString.substr(2);
                    }
                    else {
                        bufferString = "";
                    }
                    break;
                }

                bufferStream.str(string());
                if (-1 == parse_header_line(header_line, header_name, header_value)) {
                    server_info("Bad request: cannot parse a header.");
                    responseMessage = MessageFactory::createBadFormatResponse();
                    send_response(responseMessage, msg_sock);
                    is_bad_request = true;
                    break;
                }

                headers.emplace_back(header_name, header_value);
            }
            if (is_bad_request) {
                // End connection with this client.
                break;
            }

            Request request = MessageFactory::createRequest(method, filepath, headers);
            if (!request.isValid()) {
                server_info("Request could not be formatted correctly.");
                responseMessage = MessageFactory::createBadFormatResponse();
            }
            else if (!request.isMethodAvailable()) {
                server_info("Request method unavailable.");
                responseMessage = MessageFactory::createBadMethodResponse();
            }
            else {
                string file_location;
                bool in_base_dir_scope = parse_filepath(absoluteBaseDir,
                                                        request.filepath,
                                                        file_location);
                std::ifstream file;
                if (in_base_dir_scope) {
                    file = std::ifstream(file_location);
                }

                if (file.is_open()) {
                    string responseBody((std::istreambuf_iterator<char>(file)),
                                        (std::istreambuf_iterator<char>()));
                    file.close();

                    responseMessage = MessageFactory::createGoodResponse(request.method, responseBody,
                                                                         request.connectionClose);
                }
                else {
                    // Look for file in correlated servers.
                    string newAddress = correlatedServers.findFile(request.filepath);

                    if (newAddress.empty()) {
                        server_info("Resource not found.");
                        responseMessage = MessageFactory::createNotFoundResponse(request.connectionClose);
                    }
                    else {
                        server_info("Resource found in correlated server. Redirecting.");
                        responseMessage = MessageFactory::createRedirectResponse(newAddress, request.connectionClose);
                    }
                }
            }

            if (!send_response(responseMessage, msg_sock)) {
                break;
            }
            if (responseMessage.closeConnection) {
                break;
            }
        }

        server_info("Ending Connection.");
        if (close(msg_sock) < 0) {
            server_info("Could not close client socket properly.");
        }
    }

    if (close(sock) < 0) {
        server_error("Could not close server socket properly.");
    }
    return 0;
}
