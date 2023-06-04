#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include <iostream>
#include <string>


int main (int argc, char *argv[]) {
    Lexer lexer;
    Parser parser(lexer, argc == 1 ? false : true);

    try {
        if (argc == 1) {
            std::unique_ptr<ProgramASTNode> ast = parser.parse();
            ast->gen();
        } else {
            std::cout << "------------ LEFT DECOMPOSITION ------------" << std::endl;
            std::unique_ptr<ProgramASTNode> ast = parser.parse();

            std::cout << "------------ ABSTRACT SYNTAX TREE ------------" << std::endl;
            ast->print(std::cout);

            std::cout << "------------ INTERMEDIATE REPRESENTATION ------------" << std::endl;
            ast->gen();
        }
    } catch (std::string msg) {
        std::cout << msg << std::endl;
    }

    return 0;
}
