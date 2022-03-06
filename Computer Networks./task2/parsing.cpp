#include <regex>
#include <string>
#include <filesystem>

#include "parsing.h"
#include "constants.h"
#include "log.h"


std::regex get_start_line_regex() {
    static std::string METHOD_REGEX = R"([a-zA-Z]+)";
    static std::string REQUEST_TARGET_REGEX = R"(\/[a-zA-Z0-9\.\-\/]*)";
    static std::string HTTP_VERSION_REGEX = R"(\bHTTP\/1\.1\b)";
    static std::string START_LINE_REGEX =
            "(" + METHOD_REGEX + ")\\s" +
            "(" + REQUEST_TARGET_REGEX + ")\\s" +
            "(" + HTTP_VERSION_REGEX + ")" +
            R"(\r\n)";

    static std::regex re(START_LINE_REGEX);

    return re;
}

std::regex get_header_regex() {
    static std::string HEADER_NAME_REGEX = R"([a-zA-Z0-9\-\_]+)";
    static std::string HEADER_VALUE_REGEX = R"(.+\S)";
    static std::string HEADER_LINE_REGEX =
            "(" + HEADER_NAME_REGEX + ")" +
            R"(:\s*)" +
            "(" + HEADER_VALUE_REGEX + ")" +
            R"(\s*\r\n)";

    static std::regex re(HEADER_LINE_REGEX);

    return re;
}

int parse_start_line(const std::string &start_line,
                     std::string &method,
                     std::string &filepath) {
    std::regex re = get_start_line_regex();
    std::smatch matches;

    if (regex_match(start_line, matches, re)) {
        method = matches[1];
        filepath = matches[2];

        return 0;
    }
    else {
        // Invalid format.
        return -1;
    }
}

int parse_header_line(const std::string &header_line,
                      std::string &name,
                      std::string &value) {
    std::regex re = get_header_regex();
    std::smatch matches;

    if (regex_match(header_line, matches, re)) {
        name = matches[1];
        value = matches[2];

        return 0;
    }
    else {
        // Invalid format.
        return -1;
    }
}

std::string parse_base_directory_to_absolute(const std::string &base_directory) {
    char *parsed = canonicalize_file_name(base_directory.c_str());

    if (parsed == NULL || !std::filesystem::is_directory(parsed)) {
        return "";
    }

    return parsed;
}

bool parse_filepath(const std::string &base_directory,
                    const std::string &filepath,
                    std::string &file_location) {

    std::stringstream absolute_path;
    absolute_path << base_directory << filepath;
    char *parsed = canonicalize_file_name(absolute_path.str().c_str());

    if (parsed == NULL) {
        // Path could not be parsed.
        file_location = "";
        return false;
    }

    file_location = parsed;
    // Check whether base directory is a prefix of parsed filepath.
    bool is_prefix = std::mismatch(base_directory.begin(), base_directory.end(),
                                   file_location.begin(), file_location.end()).first == base_directory.end();

    if (!is_prefix || std::filesystem::is_directory(file_location)) {
        // Filepath is outside of the scope of base directory or is a directory.
        file_location = "";
        return false;
    }

    return true;
}


void parse_programme_arguments(int argc, char *argv[], std::string &absoluteBaseDir,
                               std::string &correlatedServersFile, int &port) {
    // Check if number of arguments is correct.
    if (argc < 3) {
        server_error("Usage: ./serwer <dir-with-files> <file-with-correlated-servers> [<server-port-number>]");
    }

    absoluteBaseDir = parse_base_directory_to_absolute(argv[1]);
    if (absoluteBaseDir.empty()) {
        server_error("Base directory is incorrect.");
    }

    correlatedServersFile = argv[2];
    if (!std::filesystem::exists(correlatedServersFile)) {
        server_error("Incorrect file with correlated servers.");
    }

    if (argc > 3) {
        port = std::stoi(argv[3]);
    }
    else {
        port = DEFAULT_PORT;
    }
}
