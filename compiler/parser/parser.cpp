#include "parser.hpp"
#include "descent.hpp"

Parser::Parser()
    : descent(lexer, lookahead)
{
}

bool Parser::Parse()
{
    lexer.nextSymbol();
    getNextToken();
    descent.PROGRAM();

    if ( lookahead != tok_eof )
        return false;

    return true;
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
