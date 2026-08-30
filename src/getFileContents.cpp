#include "getFileContents.h"

#include <fstream>
#include <string>

std::string getFileContents(const char* filename) {
    std::ifstream in(filename, std::ios::binary);
    if (in) {
        std::string contents;

        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(contents.data(), contents.size());
        in.close();

        return contents;
    }
    throw std::runtime_error(std::string("Failed to open shader file: ") + filename);
}
