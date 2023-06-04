#pragma once

#include <string>
#include <vector>
#include "../lexer/lexer.hpp"

#include <memory>


class Descent {
public:
    Descent (Lexer &l, Token &t, bool DECOMP_INFO): lexer(l), lookahead(t), DECOMP_INFO(DECOMP_INFO) {}

    Token match(Token tok);

    void PROGRAM();
    void BLOCK();
    void BLOCK_I();
    void CONSTANT_DEFINITION();
    void CONSTANT_DEFINITION_I();
    void CONSTANT();
    void CONSTANT_L();
    void UNSIGNED_NUMBER();
    void VARIABLE_DECLARATION_PART();
    void VARIABLE_DECLARATION_PART_I_semi();
    void VARIABLE_DECLARATION_PART_I_semi_L();
    void VARIABLE_DECLARATION();
    void IDENTIFIER_LIST();
    void IDENTIFIER_LIST_I();
    void TYPE_IDENTIFIER();
    void PROCEDURE_AND_FUNCTION_DECLARATION_PART();
    void PROCEDURE_OR_FUNCTION_DECLARATION();
    void PROCEDURE_DECLARATION();
    void FORMAL_PARAMETER_LIST_OPT();
    void FORMAL_PARAMETER_LIST();
    void FORMAL_PARAMETER_LIST_I();
    void PARAMETER_GROUP();
    void FORWARD_OR_BLOCK();
    void FUNCTION_DECLARATION();
    void STATEMENT();
    void SIMPLE_STATEMENT();
    void SIMPLE_STATEMENT_L();
    void PROCEDURE_STATEMENT_I();
    void STRUCTURED_STATEMENT();
    void COMPOUND_STATEMENT();
    void COMPOUND_STATEMENT_I();
    void IF_STATEMENT();
    void IF_STATEMENT_I();
    void REPETETIVE_STATEMENT();
    void WHILE_STATEMENT();
    void FOR_STATEMENT();
    void EMPTY_STATEMENT();
    void EXPRESSION();
    void EXPRESSION_I();
    void RELATIONAL_OPERATOR();
    void SIMPLE_EXPRESSION();
    void SIMPLE_EXPRESSION_I();
    void ADDITIVE_OPERATOR();
    void TERM();
    void TERM_I();
    void MULTIPLICATIVE_OPERATOR();
    void SIGNED_FACTOR();
    void SIGNED_FACTOR_I();
    void FACTOR();
    void FACTOR_I();
    void UNSIGNED_CONSTANT();
    void PARAMETER_LIST();
    void PARAMETER_LIST_I();

private:
    Lexer &lexer;
    Token &lookahead;
    bool DECOMP_INFO;
    
    std::string ParserError( std::string msg="PARSE ERROR!" );
};
