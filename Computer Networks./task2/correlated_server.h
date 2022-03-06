#ifndef TASK1_CORRELATED_SERVER_H
#define TASK1_CORRELATED_SERVER_H

#include <string>


class CorrelatedServers {
public:
    std::string serverFilePath;

    explicit CorrelatedServers(const std::string &serversFilePath);

    // Return new address of file on correlated server or "" if
    // there is not such file.
    std::string findFile(const std::string &absoluteFilePath) const;
};

#endif //TASK1_CORRELATED_SERVER_H
