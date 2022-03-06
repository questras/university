#include <string>
#include <iostream>

#include "debug.h"


void error(const std::string &message) {
    std::cerr << "[ERROR] " << message << "\n";
    exit(1);
}

void warning(const std::string &message) {
    std::cout << "[WARNING] " << message << "\n";
}

void info(const std::string &message) {
    std::cout << "[INFO] " << message << "\n";
}