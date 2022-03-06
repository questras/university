#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "message.h"
#include "constants.h"

using std::string;


// Transform given string to be lowercase.
void transform_to_lowercase(string &s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return tolower(c); });
}


Response::Response(const string &statusCode,
                   const string &reasonPhrase,
                   const std::vector<std::pair<string, string>> &headers,
                   const string &body,
                   bool closeConnection)
        : statusCode(statusCode),
          reasonPhrase(reasonPhrase),
          headers(headers),
          body(body),
          closeConnection(closeConnection) {}

string Response::toString() {
    string http_version = "HTTP/1.1";
    std::stringstream repr;

    repr << http_version << " " << this->statusCode << " " << this->reasonPhrase << CRLF;
    for (const auto &header : this->headers) {
        repr << header.first << ":" << header.second << CRLF;
    }
    repr << CRLF << this->body;

    return repr.str();
}

bool Request::isValid() const {
    return !this->method.empty();
}

bool Request::isMethodAvailable() const {
    return this->method == "GET" || this->method == "HEAD";
}

Request::Request(const string &method, const string &filepath, bool connectionClose)
        : method(method), filepath(filepath), connectionClose(connectionClose) {}

Request MessageFactory::createRequest(const string &method, const string &filepath,
                                      const std::vector<std::pair<string, string>> &headers) {
    string contentLengthRepr = "content-length";
    string connectionRepr = "connection";
    bool connectionClose = false;
    bool connectionHeaderPresent = false;
    bool contentLengthHeaderPresent = false;

    // Validate headers.
    for (auto &header : headers) {
        string headerName = header.first;
        string headerValue = header.second;
        if (headerName.length() == contentLengthRepr.length() ||
            headerName.length() == connectionRepr.length()) {

            transform_to_lowercase(headerName);
            if (headerName == contentLengthRepr) {
                if (contentLengthHeaderPresent) {
                    // Duplicated header.
                    return MessageFactory::createBadFormatRequest();
                }

                contentLengthHeaderPresent = true;
                if (headerValue != "0") {
                    // Only 0 is acceptable content-length.
                    return MessageFactory::createBadFormatRequest();
                }
            }
            else if (headerName == connectionRepr) {
                if (connectionHeaderPresent) {
                    // Duplicated header.
                    return MessageFactory::createBadFormatRequest();
                }

                connectionHeaderPresent = true;
                transform_to_lowercase(headerValue);
                if (headerValue == "close") {
                    connectionClose = true;
                }
            }
        }
    }

    return Request(method, filepath, connectionClose);
}

Request MessageFactory::createBadFormatRequest() {
    return Request("", "", true);
}

Response MessageFactory::createBadFormatResponse() {
    std::vector<std::pair<string, string>> headers;
    headers.emplace_back("Connection", "close");
    headers.emplace_back("Content-Length", "0");
    return Response("400", "Bad request", headers, "", true);
}

Response MessageFactory::createBadMethodResponse() {
    std::vector<std::pair<string, string>> headers;
    headers.emplace_back("Connection", "close");
    headers.emplace_back("Content-Length", "0");
    return Response("501", "Not implemented", headers, "", true);
}

Response
MessageFactory::createGoodResponse(const string &requestMethod, const string &body, bool connectionClose) {
    std::vector<std::pair<string, string>> headers;
    string responseBody = body;

    headers.emplace_back("Content-Length", std::to_string(body.length()));
    headers.emplace_back("Content-Type", "application/octet-stream");
    if (connectionClose) {
        headers.emplace_back("Connection", "close");
    }
    if (requestMethod == "HEAD") {
        responseBody = "";
    }

    return Response("200", "OK", headers, responseBody, connectionClose);
}

Response MessageFactory::createNotFoundResponse(bool connectionClose) {
    std::vector<std::pair<string, string>> headers;
    headers.emplace_back("Content-Length", "0");
    if (connectionClose) {
        headers.emplace_back("Connection", "close");
    }
    return Response("404", "Not found", headers, "", connectionClose);
}

Response MessageFactory::createRedirectResponse(string &newFileAddress, bool connectionClose) {
    std::vector<std::pair<string, string>> headers;
    headers.emplace_back("Content-Length", "0");
    headers.emplace_back("Location", newFileAddress);
    if (connectionClose) {
        headers.emplace_back("Connection", "close");
    }
    return Response("302", "Redirect", headers, "", connectionClose);
}
