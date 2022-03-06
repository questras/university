#ifndef TASK1_LOG_H
#define TASK1_LOG_H

#include <string>

// Print info to server log.
void server_info(const std::string &s);

// Print error info to server log and exit server
// with EXIT_FAILURE code.
void server_error(const std::string &s);

#endif //TASK1_LOG_H
