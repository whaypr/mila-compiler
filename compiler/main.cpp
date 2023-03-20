#include "parser/parser.hpp"
#include <iostream>
#include <string>


int main (int argc, char *argv[]) {
    Parser parser;

    try {
        parser.Parse();
        std::cout << "Parsed successfully" << std::endl;
    } catch (std::string msg) {
        std::cout << msg << std::endl;
    }

    return 0;
}
