#ifndef TASK1_PARSING_H
#define TASK1_PARSING_H

#include <string>


//  Parse start line of a request, return 0 if parsed correctly,
//  -1 if invalid format. Parsed line is saved in `method` and `filepath` vars.
int parse_start_line(const std::string &start_line,
                     std::string &method,
                     std::string &filepath);

//  Parse single header line of a request, return 0 if parsed correctly,
//  -1 if invalid format. Parsed line is saved in `name` and `value` vars.
int parse_header_line(const std::string &header_line,
                      std::string &name,
                      std::string &value);

// Parse given base directory path to absolute, canonical path. If cannot be
// parsed, empty string is returned.
std::string parse_base_directory_to_absolute(const std::string &base_directory);

// Parse filepath to absolute path. Return true if parsed correctly
// and file is in scope of base_directory, false otherwise.
// Parsed filepath is saved to `file_location` variable.
bool parse_filepath(const std::string &base_directory,
                    const std::string &filepath,
                    std::string &file_location);

// Parse programme command line arguments, save directory with file
// to `absoluteBaseDir`, filename of file with correlated servers to
// `correlatedServersFile` and port to `port`. Error will be raised
// if any of programme arguments is incorrect.
void parse_programme_arguments(int argc, char *argv[], std::string &absoluteBaseDir,
                               std::string &correlatedServersFile, int &port);

#endif //TASK1_PARSING_H
