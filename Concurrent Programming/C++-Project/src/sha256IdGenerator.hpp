#ifndef SRC_SHA256IDGENERATOR_HPP_
#define SRC_SHA256IDGENERATOR_HPP_

#include <cstdlib>

#include "immutable/idGenerator.hpp"
#include "immutable/pageId.hpp"

class Sha256IdGenerator : public IdGenerator {
public:
    virtual PageId generateId(std::string const& content) const
    {
        std::string command = "echo -n \"" + content + "\" | sha256sum";
        char buffer[128];
        std::string result;

        FILE* fp;
        fp = popen(command.c_str(), "r");

        if (fp == NULL) {
            exit(1);
        }

        while (fgets(buffer, 128, fp) != NULL) {
            result += buffer;
        }

        pclose(fp);
        // Truncate result to not contain ending whitespaces, -, and new line sign.
        result = result.substr(0, result.size() - 4);

        return PageId(result);
    }
};

#endif /* SRC_SHA256IDGENERATOR_HPP_ */
