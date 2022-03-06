#include <iostream>
#include <fstream>
#include <sstream>

#include "correlated_server.h"
#include "log.h"

using std::string;


CorrelatedServers::CorrelatedServers(const std::string &serversFilePath)
        : serverFilePath(serversFilePath) {}

std::string CorrelatedServers::findFile(const std::string &absoluteFilePath) const {
    // Open file with correlated servers.
    std::ifstream file;
    file.open(this->serverFilePath);
    if (!file.is_open()) {
        server_error("Correlated servers file cannot be read.");
    }

    string line, resource, server, port;
    while (getline(file, line)) {
        auto pos = line.find('\t');
        resource = line.substr(0, pos);

        if (absoluteFilePath == resource) {
            auto pos2 = line.find('\t', pos + 1);
            server = line.substr(pos + 1, pos2 - pos - 1);
            port = line.substr(pos2 + 1);

            std::stringstream result;
            result << "http://" << server << ":" << port << resource;

            file.close();
            return result.str();
        }
    }

    file.close();
    return std::string(); // Not found, so return empty string.
}
