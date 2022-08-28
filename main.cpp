#include "FileWrapper.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Error input. Incorrect number of arguments.";
        return 1;
    }
    std::string path{argv[1]};
    std::string mask(argv[2]);
    FileWrapper fileWrapper(path);
    fileWrapper.OutputAllUsageOfString(mask, std::cout);
    return 0;
}
