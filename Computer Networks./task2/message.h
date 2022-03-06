#ifndef TASK1_MESSAGE_H
#define TASK1_MESSAGE_H

#include <string>
#include <vector>
#include <exception>


class Response {
public:
    std::string statusCode;
    std::string reasonPhrase;
    std::vector<std::pair<std::string, std::string>> headers;
    std::string body;
    bool closeConnection;

    Response() = default;

    Response(const std::string &statusCode,
             const std::string &reasonPhrase,
             const std::vector<std::pair<std::string, std::string>> &headers,
             const std::string &body,
             bool closeConnection);

    std::string toString();
};

class Request {
public:
    std::string method;
    std::string filepath;
    bool connectionClose;

    Request(const std::string &method, const std::string &filepath, bool connectionClose);


    // Return true, if request is a valid request, false otherwise.
    bool isValid() const;

    // Return true, if method of request is available, false otherwise.
    bool isMethodAvailable() const;
};

class MessageFactory {
public:
    static Request createRequest(const std::string &method,
                                 const std::string &filepath,
                                 const std::vector<std::pair<std::string, std::string>> &headers);

    static Request createBadFormatRequest();

    static Response createBadFormatResponse();

    static Response createBadMethodResponse();

    static Response createGoodResponse(const std::string &requestMethod,
                                       const std::string &body,
                                       bool connectionClose);

    static Response createNotFoundResponse(bool connectionClose);

    static Response createRedirectResponse(std::string &newFileAddress,
                                           bool connectionClose);
};


#endif //TASK1_MESSAGE_H
