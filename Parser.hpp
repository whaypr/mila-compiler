#ifndef PJPPROJECT_PARSER_HPP
#define PJPPROJECT_PARSER_HPP

#include "Lexer.hpp"

class Parser {
public:
    Parser();
    ~Parser() = default;

    bool Parse();                    // parse

private:
    int getNextToken();

    Lexer m_Lexer;                   // lexer is used to read tokens
    int CurTok;                      // to keep the current token
};

#endif //PJPPROJECT_PARSER_HPP
