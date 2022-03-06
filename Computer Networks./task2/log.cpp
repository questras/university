#include <string>
#include <iostream>
#include <cstdlib>

#include "log.h"


void server_info(const std::string &s) {
    std::cout << "\033[94m" << "[INFO] " << s << "\033[0m\n";
}

void server_error(const std::string &s) {
    std::cout << "\033[91m" << "[ERROR] " << s << "\033[0m\n";
    exit(EXIT_FAILURE);
}
