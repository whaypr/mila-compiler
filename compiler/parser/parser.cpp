#include <iostream>

#include "parser.hpp"
#include "descent.hpp"


std::unique_ptr<ProgramASTNode> Parser::parse()
{
    lexer.nextSymbol();
    getNextToken();

    auto res = descent.PROGRAM();

    descent.match(tok_eof);
    return res;
}


/**
 * @brief Simple token buffer.
 *
 * lookahead is the current token the parser is looking at
 * getNextToken reads another token from the lexer and updates lookahead with ts result
 * Every function in the parser will assume that lookahead is the cureent token that needs to be parsed
 */
int Parser::getNextToken() {
    return lookahead = lexer.nextToken();
}
