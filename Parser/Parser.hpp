#ifndef PJPPROJECT_PARSER_HPP
#define PJPPROJECT_PARSER_HPP

#include "../Lexer/Lexer.hpp"
#include "Descent.hpp"

class Parser {
public:
    Parser();
    ~Parser() = default;

    bool Parse();                    // parse

private:
    int getNextToken();

    Descent descent;
    Lexer lexer;                     // lexer is used to read tokens
    Token lookahead;                 // to keep the current token
};

#endif //PJPPROJECT_PARSER_HPP
