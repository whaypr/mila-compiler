#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include <iostream>
#include <string>


int main (int argc, char *argv[]) {
    Lexer lexer;
    Parser parser(lexer, argc == 1 ? false : true);

    try {
            std::cout << "------------ LEFT DECOMPOSITION ------------" << std::endl;
            std::unique_ptr<ProgramASTNode> ast = parser.parse();
    } catch (std::string msg) {
        std::cout << msg << std::endl;
    }

    return 0;
}
