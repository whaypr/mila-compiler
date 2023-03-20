#include "descent.hpp"
#include <iostream>
#include <ostream>

#define DECOMP_INFO 0


void Descent::match( Token tok ) {
    if (lookahead == tok)
        lookahead = lexer.nextToken();
    else {
        throw ParserError("Parser error in MATCH");
    }
}


std::string Descent::ParserError( std::string msg ) {
    return msg;
}


// ----------------------------------------------------------------
void Descent::PROGRAM()
{
    switch(lookahead) {
    case tok_program:
        if (DECOMP_INFO) std::cout << "rule 1: PROGRAM ⟶ program ident semi BLOCK dot" << std::endl;
        match(tok_program);
        match(tok_identifier);
        match(tok_semicolon);
        BLOCK();
        match(tok_dot);
        break;
    default:
        throw ParserError("Parser error in PROGRAM");
    }
}

void Descent::BLOCK()
{
    switch(lookahead) {
    case tok_const:
    case tok_var:
    case tok_procedure:
    case tok_function:
    case tok_begin:
        if (DECOMP_INFO) std::cout << "rule 2: BLOCK ⟶ BLOCK_I COMPOUND_STATEMENT" << std::endl;
        BLOCK_I();
        COMPOUND_STATEMENT();
        break;
    default:
        throw ParserError("Parser error in BLOCK");
    }
}

void Descent::BLOCK_I()
{
    switch(lookahead) {
    case tok_const:
        if (DECOMP_INFO) std::cout << "rule 3: BLOCK_I ⟶ CONSTANT_DEFINITION BLOCK_I" << std::endl;
        CONSTANT_DEFINITION();
        BLOCK_I();
        break;
    case tok_var:
        if (DECOMP_INFO) std::cout << "rule 4: BLOCK_I ⟶ VARIABLE_DECLARATION_PART BLOCK_I" << std::endl;
        VARIABLE_DECLARATION_PART();
        BLOCK_I();
        break;
    case tok_procedure:
    case tok_function:
        if (DECOMP_INFO) std::cout << "rule 5: BLOCK_I ⟶ PROCEDURE_AND_FUNCTION_DECLARATION_PART BLOCK_I" << std::endl;
        PROCEDURE_AND_FUNCTION_DECLARATION_PART();
        BLOCK_I();
        break;
    case tok_begin:
        if (DECOMP_INFO) std::cout << "rule 6: BLOCK_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in BLOCK_I");
    }
}

void Descent::CONSTANT_DEFINITION()
{
    switch(lookahead) {
    case tok_const:
        if (DECOMP_INFO) std::cout << "rule 7: CONSTANT_DEFINITION ⟶ const ident equal CONSTANT semi CONSTANT_DEFINITION_I" << std::endl;
        match(tok_const);
        match(tok_identifier);
        match(tok_equal);
        CONSTANT();
        match(tok_semicolon);
        CONSTANT_DEFINITION_I();
        break;
    default:
        throw ParserError("Parser error in CONSTANT_DEFINITION");
    }
}

void Descent::CONSTANT_DEFINITION_I()
{
    switch(lookahead) {
    case tok_identifier:
        if (DECOMP_INFO) std::cout << "rule 8: CONSTANT_DEFINITION_I ⟶ ident equal CONSTANT semi CONSTANT_DEFINITION_I" << std::endl;
        match(tok_identifier);
        match(tok_equal);
        CONSTANT();
        match(tok_semicolon);
        CONSTANT_DEFINITION_I();
        break;
    case tok_const:
    case tok_var:
    case tok_procedure:
    case tok_function:
    case tok_begin:
        if (DECOMP_INFO) std::cout << "rule 9: CONSTANT_DEFINITION_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in CONSTANT_DEFINITION_I");
    }
}

void Descent::CONSTANT()
{
    switch(lookahead) {
    case tok_intLiteral:
    case tok_realLiteral:
        if (DECOMP_INFO) std::cout << "rule 10: CONSTANT ⟶ UNSIGNED_NUMBER" << std::endl;
        UNSIGNED_NUMBER();
        break;
    case tok_plus:
        if (DECOMP_INFO) std::cout << "rule 11: CONSTANT ⟶ plus CONSTANT_L" << std::endl;
        match(tok_plus);
        CONSTANT_L();
        break;
    case tok_identifier:
        if (DECOMP_INFO) std::cout << "rule 12: CONSTANT ⟶ ident" << std::endl;
        match(tok_identifier);
        break;
    case tok_minus:
        if (DECOMP_INFO) std::cout << "rule 13: CONSTANT ⟶ minus CONSTANT_L" << std::endl;
        match(tok_minus);
        CONSTANT_L();
        break;
    case tok_stringLiteral:
        if (DECOMP_INFO) std::cout << "rule 14: CONSTANT ⟶ string_literal" << std::endl;
        match(tok_stringLiteral);
        break;
    default:
        throw ParserError("Parser error in CONSTANT");
    }
}

void Descent::CONSTANT_L()
{
    switch(lookahead) {
    case tok_intLiteral:
    case tok_realLiteral:
        if (DECOMP_INFO) std::cout << "rule 15: CONSTANT_L ⟶ UNSIGNED_NUMBER" << std::endl;
        UNSIGNED_NUMBER();
        break;
    case tok_identifier:
        if (DECOMP_INFO) std::cout << "rule 16: CONSTANT_L ⟶ ident" << std::endl;
        match(tok_identifier);
        break;
    default:
        throw ParserError("Parser error in CONSTANT_L");
    }
}

void Descent::UNSIGNED_NUMBER()
{
    switch(lookahead) {
    case tok_intLiteral:
        if (DECOMP_INFO) std::cout << "rule 17: UNSIGNED_NUMBER -> num_int" << std::endl;
        match(tok_intLiteral);
        break;
    case tok_realLiteral:
        if (DECOMP_INFO) std::cout << "rule 18: UNSIGNED_NUMBER -> num_real" << std::endl;
        match(tok_realLiteral);
        break;
    default:
        throw ParserError("Parser error in UNSIGNED_NUMBER");
    }
}

void Descent::VARIABLE_DECLARATION_PART()
{
    switch(lookahead) {
    case tok_var:
        if (DECOMP_INFO) std::cout << "rule 19: VARIABLE_DECLARATION_PART ⟶ var VARIABLE_DECLARATION VARIABLE_DECLARATION_PART_I_semi" << std::endl;
        match(tok_var);
        VARIABLE_DECLARATION();
        VARIABLE_DECLARATION_PART_I_semi();
        break;
    default:
        throw ParserError("Parser error in VARIABLE_DECLARATION_PART");
    }
}

void Descent::VARIABLE_DECLARATION_PART_I_semi()
{
    switch(lookahead) {
    case tok_semicolon:
        if (DECOMP_INFO) std::cout << "rule 20: VARIABLE_DECLARATION_PART_I_semi ⟶ semi VARIABLE_DECLARATION_PART_I_semi_L" << std::endl;
        match(tok_semicolon);
        VARIABLE_DECLARATION_PART_I_semi_L();
        break;
    default:
        throw ParserError("Parser error in VARIABLE_DECLARATION_PART_I_semi");
    }
}

void Descent::VARIABLE_DECLARATION_PART_I_semi_L()
{
    switch(lookahead) {
    case tok_identifier:
        if (DECOMP_INFO) std::cout << "rule 21: VARIABLE_DECLARATION_PART_I_semi_L ⟶ VARIABLE_DECLARATION VARIABLE_DECLARATION_PART_I_semi" << std::endl;
        VARIABLE_DECLARATION();
        VARIABLE_DECLARATION_PART_I_semi();
        break;
    case tok_const:
    case tok_var:
    case tok_procedure:
    case tok_function:
    case tok_begin:
        if (DECOMP_INFO) std::cout << "rule 22: VARIABLE_DECLARATION_PART_I_semi_L ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in VARIABLE_DECLARATION_PART_I_semi_L");
    }
}

void Descent::VARIABLE_DECLARATION()
{
    switch(lookahead) {
    case tok_identifier:
        if (DECOMP_INFO) std::cout << "rule 23: VARIABLE_DECLARATION ⟶ IDENTIFIER_LIST colon TYPE_IDENTIFIER" << std::endl;
        IDENTIFIER_LIST();
        match(tok_colon);
        TYPE_IDENTIFIER();
        break;
    default:
        throw ParserError("Parser error in VARIABLE_DECLARATION");
    }
}

void Descent::IDENTIFIER_LIST()
{
    switch(lookahead) {
    case tok_identifier:
        if (DECOMP_INFO) std::cout << "rule 24: IDENTIFIER_LIST ⟶ ident IDENTIFIER_LIST_I" << std::endl;
        match(tok_identifier);
        IDENTIFIER_LIST_I();
        break;
    default:
        throw ParserError("Parser error in IDENTIFIER_LIST");
    }
}

void Descent::IDENTIFIER_LIST_I()
{
    switch(lookahead) {
    case tok_comma:
        if (DECOMP_INFO) std::cout << "rule 25: IDENTIFIER_LIST_I ⟶ comma ident" << std::endl;
        match(tok_comma);
        match(tok_identifier);
        break;
    case tok_colon:
        if (DECOMP_INFO) std::cout << "rule 26: IDENTIFIER_LIST_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in IDENTIFIER_LIST_I");
    }
}

void Descent::TYPE_IDENTIFIER()
{
    switch(lookahead) {
    case tok_identifier:
        if (DECOMP_INFO) std::cout << "rule 27: TYPE_IDENTIFIER ⟶ ident" << std::endl;
        match(tok_identifier);
        break;
    case tok_integer:
        if (DECOMP_INFO) std::cout << "rule 28: TYPE_IDENTIFIER ⟶ integer" << std::endl;
        match(tok_integer);
        break;
    case tok_real:
        if (DECOMP_INFO) std::cout << "rule 29: TYPE_IDENTIFIER ⟶ real" << std::endl;
        match(tok_real);
        break;
    case tok_string:
        if (DECOMP_INFO) std::cout << "rule 30: TYPE_IDENTIFIER ⟶ string" << std::endl;
        match(tok_string);
        break;
    default:
        throw ParserError("Parser error in TYPE_IDENTIFIER");
    }
}

void Descent::PROCEDURE_AND_FUNCTION_DECLARATION_PART()
{
    switch(lookahead) {
    case tok_procedure:
    case tok_function:
        if (DECOMP_INFO) std::cout << "rule 31: PROCEDURE_AND_FUNCTION_DECLARATION_PART ⟶ PROCEDURE_OR_FUNCTION_DECLARATION semi" << std::endl;
        PROCEDURE_OR_FUNCTION_DECLARATION();
        match(tok_semicolon);
        break;
    default:
        throw ParserError("Parser error in PROCEDURE_AND_FUNCTION_DECLARATION_PART");
    }
}

void Descent::PROCEDURE_OR_FUNCTION_DECLARATION()
{
    switch(lookahead) {
    case tok_procedure:
        if (DECOMP_INFO) std::cout << "rule 32: PROCEDURE_OR_FUNCTION_DECLARATION ⟶ PROCEDURE_DECLARATION" << std::endl;
        PROCEDURE_DECLARATION();
        break;
    case tok_function:
        if (DECOMP_INFO) std::cout << "rule 33: PROCEDURE_OR_FUNCTION_DECLARATION ⟶ FUNCTION_DECLARATION" << std::endl;
        FUNCTION_DECLARATION();
        break;
    default:
        throw ParserError("Parser error in PROCEDURE_OR_FUNCTION_DECLARATION");
    }
}

void Descent::PROCEDURE_DECLARATION()
{
    switch(lookahead) {
    case tok_procedure:
        if (DECOMP_INFO) std::cout << "rule 34: PROCEDURE_DECLARATION ⟶ procedure ident FORMAL_PARAMETER_LIST_OPT semi FORWARD_OR_BLOCK" << std::endl;
        match(tok_procedure);
        match(tok_identifier);
        FORMAL_PARAMETER_LIST_OPT();
        match(tok_semicolon);
        FORWARD_OR_BLOCK();
        break;
    default:
        throw ParserError("Parser error in PROCEDURE_DECLARATION");
    }
}

void Descent::FORMAL_PARAMETER_LIST_OPT()
{
    switch(lookahead) {
    case tok_lparen:
        if (DECOMP_INFO) std::cout << "rule 35: FORMAL_PARAMETER_LIST_OPT ⟶ FORMAL_PARAMETER_LIST" << std::endl;
        FORMAL_PARAMETER_LIST();
        break;
    case tok_semicolon:
    case tok_colon:
        if (DECOMP_INFO) std::cout << "rule 36: FORMAL_PARAMETER_LIST_OPT ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in FORMAL_PARAMETER_LIST_OPT");
    }
}

void Descent::FORMAL_PARAMETER_LIST()
{
    switch(lookahead) {
    case tok_lparen:
        if (DECOMP_INFO) std::cout << "rule 37: FORMAL_PARAMETER_LIST ⟶ lparen PARAMETER_GROUP FORMAL_PARAMETER_LIST_I rparen" << std::endl;
        match(tok_lparen);
        PARAMETER_GROUP();
        FORMAL_PARAMETER_LIST_I();
        match(tok_rparen);
        break;
    default:
        throw ParserError("Parser error in FORMAL_PARAMETER_LIST");
    }
}

void Descent::FORMAL_PARAMETER_LIST_I()
{
    switch(lookahead) {
    case tok_semicolon:
        if (DECOMP_INFO) std::cout << "rule 38: FORMAL_PARAMETER_LIST_I ⟶ semi PARAMETER_GROUP FORMAL_PARAMETER_LIST_I" << std::endl;
        match(tok_semicolon);
        PARAMETER_GROUP();
        FORMAL_PARAMETER_LIST_I();
        break;
    case tok_rparen:
        if (DECOMP_INFO) std::cout << "rule 39: FORMAL_PARAMETER_LIST_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in FORMAL_PARAMETER_LIST_I");
    }
}

void Descent::PARAMETER_GROUP()
{
    switch(lookahead) {
    case tok_identifier:
        if (DECOMP_INFO) std::cout << "rule 40: PARAMETER_GROUP ⟶ IDENTIFIER_LIST colon TYPE_IDENTIFIER" << std::endl;
        IDENTIFIER_LIST();
        match(tok_colon);
        TYPE_IDENTIFIER();
        break;
    default:
        throw ParserError("Parser error in PARAMETER_GROUP");
    }
}

void Descent::FORWARD_OR_BLOCK()
{
    switch(lookahead) {
    case tok_forward:
        if (DECOMP_INFO) std::cout << "rule 41: FORWARD_OR_BLOCK ⟶ forward" << std::endl;
        match(tok_forward);
        break;
    case tok_begin:
    case tok_const:
    case tok_function:
    case tok_procedure:
    case tok_var:
        if (DECOMP_INFO) std::cout << "rule 42: FORWARD_OR_BLOCK ⟶ BLOCK" << std::endl;
        BLOCK();
        break;
    default:
        throw ParserError("Parser error in FORWARD");
    }
}

void Descent::FUNCTION_DECLARATION()
{
    switch(lookahead) {
    case tok_function:
        if (DECOMP_INFO) std::cout << "rule 43: FUNCTION_DECLARATION ⟶ function ident FORMAL_PARAMETER_LIST_OPT colon TYPE_IDENTIFIER semi FORWARD_OR_BLOCK" << std::endl;
        match(tok_function);
        match(tok_identifier);
        FORMAL_PARAMETER_LIST_OPT();
        match(tok_colon);
        TYPE_IDENTIFIER();
        match(tok_semicolon);
        FORWARD_OR_BLOCK();
        break;
    default:
        throw ParserError("Parser error in FUNCTION_DECLARATION");
    }
}

void Descent::STATEMENT()
{
    switch(lookahead) {
    case tok_else:
    case tok_end:
    case tok_identifier:
    case tok_semicolon:
    case tok_exit:
        if (DECOMP_INFO) std::cout << "rule 42: STATEMENT ⟶ SIMPLE_STATEMENT" << std::endl;
        SIMPLE_STATEMENT();
        break;
    case tok_begin:
    case tok_for:
    case tok_if:
    case tok_while:
        if (DECOMP_INFO) std::cout << "rule 43: STATEMENT ⟶ STRUCTURED_STATEMENT" << std::endl;
        STRUCTURED_STATEMENT();
        break;
    default:
        throw ParserError("Parser error in STATEMENT");
    }
}

void Descent::SIMPLE_STATEMENT()
{
    switch(lookahead) {
    case tok_else:
    case tok_end:
    case tok_semicolon:
        if (DECOMP_INFO) std::cout << "rule 44: SIMPLE_STATEMENT ⟶ EMPTY_STATEMENT" << std::endl;
        EMPTY_STATEMENT();
        break;
    case tok_exit:
        if (DECOMP_INFO) std::cout << "rule 45: SIMPLE_STATEMENT ⟶ exit" << std::endl;
        match(tok_exit);
        break;
    case tok_identifier:
        if (DECOMP_INFO) std::cout << "rule 46: SIMPLE_STATEMENT ⟶ ident SIMPLE_STATEMENT_L" << std::endl;
        match(tok_identifier);
        SIMPLE_STATEMENT_L();
        break;
    default:
        throw ParserError("Parser error in SIMPLE_STATEMENT");
    }
}

void Descent::SIMPLE_STATEMENT_L()
{
    switch(lookahead) {
    case tok_assign:
        if (DECOMP_INFO) std::cout << "rule 47: SIMPLE_STATEMENT_L ⟶ assign EXPRESSION" << std::endl;
        match(tok_assign);
        EXPRESSION();
        break;
    case tok_lparen:
    case tok_semicolon:
    case tok_else:
    case tok_end:
        if (DECOMP_INFO) std::cout << "rule 48: SIMPLE_STATEMENT_L ⟶ PROCEDURE_STATEMENT_I" << std::endl;
        PROCEDURE_STATEMENT_I();
        break;
    default:
        throw ParserError("Parser error in SIMPLE_STATEMENT_L");
    }
}

void Descent::PROCEDURE_STATEMENT_I()
{
    switch(lookahead) {
    case tok_lparen:
        if (DECOMP_INFO) std::cout << "rule 49: PROCEDURE_STATEMENT_I ⟶ lparen PARAMETER_LIST rparen" << std::endl;
        match(tok_lparen);
        PARAMETER_LIST();
        match(tok_rparen);
        break;
    case tok_semicolon:
    case tok_end:
    case tok_else:
        if (DECOMP_INFO) std::cout << "rule 50: PROCEDURE_STATEMENT_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in PROCEDURE_STATEMENT_I");
    }
}

void Descent::STRUCTURED_STATEMENT()
{
    switch(lookahead) {
    case tok_begin:
        if (DECOMP_INFO) std::cout << "rule 51: STRUCTURED_STATEMENT ⟶ COMPOUND_STATEMENT" << std::endl;
        COMPOUND_STATEMENT();
        break;
    case tok_if:
        if (DECOMP_INFO) std::cout << "rule 52: STRUCTURED_STATEMENT ⟶ IF_STATEMENT" << std::endl;
        IF_STATEMENT();
        break;
    case tok_for:
    case tok_while:
        if (DECOMP_INFO) std::cout << "rule 53: STRUCTURED_STATEMENT ⟶ REPETETIVE_STATEMENT" << std::endl;
        REPETETIVE_STATEMENT();
        break;
    default:
        throw ParserError("Parser error in STRUCTURED_STATEMENT");
    }
}

void Descent::COMPOUND_STATEMENT()
{
    switch(lookahead) {
    case tok_begin:
        if (DECOMP_INFO) std::cout << "rule 54: COMPOUND_STATEMENT ⟶ begin STATEMENT COMPOUND_STATEMENT_I end" << std::endl;
        match(tok_begin);
        STATEMENT();
        COMPOUND_STATEMENT_I();
        match(tok_end);
        break;
    default:
        throw ParserError("Parser error in COMPOUND_STATEMENT");
    }
}

void Descent::COMPOUND_STATEMENT_I()
{
    switch(lookahead) {
    case tok_semicolon:
        if (DECOMP_INFO) std::cout << "rule 55: COMPOUND_STATEMENT_I ⟶ semi STATEMENT COMPOUND_STATEMENT_I" << std::endl;
        match(tok_semicolon);
        STATEMENT();
        COMPOUND_STATEMENT_I();
        break;
    case tok_end:
        if (DECOMP_INFO) std::cout << "rule 56: COMPOUND_STATEMENT_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in COMPOUND_STATEMENT_I");
    }
}

void Descent::IF_STATEMENT()
{
    switch(lookahead) {
    case tok_if:
        if (DECOMP_INFO) std::cout << "rule 57: IF_STATEMENT ⟶ if EXPRESSION then STATEMENT IF_STATEMENT_I" << std::endl;
        match(tok_if);
        EXPRESSION();
        match(tok_then);
        STATEMENT();
        IF_STATEMENT_I();
        break;
    default:
        throw ParserError("Parser error in IF_STATEMENT");
    }
}

void Descent::IF_STATEMENT_I()
{
    switch(lookahead) {
    case tok_else:
        if (DECOMP_INFO) std::cout << "rule 58: IF_STATEMENT_I ⟶ else STATEMENT" << std::endl; // could be rule 58 as well
        match(tok_else);
        STATEMENT();
        break;
    case tok_end:
    case tok_semicolon:
        if (DECOMP_INFO) std::cout << "rule 59: IF_STATEMENT_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in IF_STATEMENT_I");
    }
}

void Descent::REPETETIVE_STATEMENT()
{
    switch(lookahead) {
    case tok_while:
        if (DECOMP_INFO) std::cout << "rule 60: REPETETIVE_STATEMENT ⟶ WHILE_STATEMENT" << std::endl;
        WHILE_STATEMENT();
        break;
    case tok_for:
        if (DECOMP_INFO) std::cout << "rule 61: REPETETIVE_STATEMENT ⟶ FOR_STATEMENT" << std::endl;
        FOR_STATEMENT();
        break;
    default:
        throw ParserError("Parser error in REPETETIVE_STATEMENT");
    }
}

void Descent::WHILE_STATEMENT()
{
    switch(lookahead) {
    case tok_while:
        if (DECOMP_INFO) std::cout << "rule 62: WHILE_STATEMENT ⟶ while EXPRESSION do STATEMENT" << std::endl;
        match(tok_while);
        EXPRESSION();
        match(tok_do);
        STATEMENT();
        break;
    default:
        throw ParserError("Parser error in WHILE_STATEMENT");
    }
}

void Descent::FOR_STATEMENT()
{
    switch(lookahead) {
    case tok_for:
        if (DECOMP_INFO) std::cout << "rule 63: FOR_STATEMENT ⟶ for ident assign EXPRESSION to EXPRESSION do STATEMENT" << std::endl;
        match(tok_for);
        match(tok_identifier);
        match(tok_assign);
        EXPRESSION();
        match(tok_to);
        EXPRESSION();
        match(tok_do);
        STATEMENT();
        break;
    default:
        throw ParserError("Parser error in FOR_STATEMENT");
    }
}

void Descent::EMPTY_STATEMENT()
{
    switch(lookahead) {
    case tok_else:
    case tok_end:
    case tok_semicolon:
        if (DECOMP_INFO) std::cout << "rule 64: EMPTY_STATEMENT ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in EMPTY_STATEMENT");
    }
}

void Descent::EXPRESSION()
{
    switch(lookahead) {
    case tok_plus:
    case tok_minus:
    case tok_identifier:
    case tok_lparen:
    case tok_not:
    case tok_stringLiteral:
    case tok_intLiteral:
    case tok_realLiteral:
        if (DECOMP_INFO) std::cout << "rule 65: EXPRESSION ⟶ SIMPLE_EXPRESSION EXPRESSION_I" << std::endl;
        SIMPLE_EXPRESSION();
        EXPRESSION_I();
        break;
    default:
        throw ParserError("Parser error in EXPRESSION");
    }
}

void Descent::EXPRESSION_I()
{
    switch(lookahead) {
    case tok_equal:
    case tok_notequal:
    case tok_lt:
    case tok_le:
    case tok_ge:
    case tok_gt:
        if (DECOMP_INFO) std::cout << "rule 66: EXPRESSION_I ⟶ RELATIONAL_OPERATOR EXPRESSION" << std::endl;
        RELATIONAL_OPERATOR();
        EXPRESSION();
        break;
    case tok_then:
    case tok_do:
    case tok_to:
    case tok_rparen:
    case tok_comma:
    case tok_else:
    case tok_end:
    case tok_semicolon:
        if (DECOMP_INFO) std::cout << "rule 67: EXPRESSION_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in EXPRESSION_I");
    }
}

void Descent::RELATIONAL_OPERATOR()
{
    switch(lookahead) {
    case tok_equal:
        if (DECOMP_INFO) std::cout << "rule 68: RELATIONAL_OPERATOR ⟶ equal" << std::endl;
        match(tok_equal);
        break;
    case tok_notequal:
        if (DECOMP_INFO) std::cout << "rule 69: RELATIONAL_OPERATOR ⟶ not_equal" << std::endl;
        match(tok_notequal);
        break;
    case tok_lt:
        if (DECOMP_INFO) std::cout << "rule 70: RELATIONAL_OPERATOR ⟶ lt" << std::endl;
        match(tok_lt);
        break;
    case tok_le:
        if (DECOMP_INFO) std::cout << "rule 71: RELATIONAL_OPERATOR ⟶ le" << std::endl;
        match(tok_le);
        break;
    case tok_ge:
        if (DECOMP_INFO) std::cout << "rule 72: RELATIONAL_OPERATOR ⟶ ge" << std::endl;
        match(tok_ge);
        break;
    case tok_gt:
        if (DECOMP_INFO) std::cout << "rule 73: RELATIONAL_OPERATOR ⟶ gt" << std::endl;
        match(tok_gt);
        break;
    default:
        throw ParserError("Parser error in RELATIONAL_OPERATOR");
    }
}

void Descent::SIMPLE_EXPRESSION()
{
    switch(lookahead) {
    case tok_plus:
    case tok_minus:
    case tok_identifier:
    case tok_lparen:
    case tok_not:
    case tok_intLiteral:
    case tok_realLiteral:
    case tok_stringLiteral:
        if (DECOMP_INFO) std::cout << "rule 74: SIMPLE_EXPRESSION ⟶ TERM SIMPLE_EXPRESSION_I" << std::endl;
        TERM();
        SIMPLE_EXPRESSION_I();
        break;
    default:
        throw ParserError("Parser error in SIMPLE_EXPRESSION");
    }
}

void Descent::SIMPLE_EXPRESSION_I()
{
    switch(lookahead) {
    case tok_plus:
    case tok_minus:
    case tok_or:
        if (DECOMP_INFO) std::cout << "rule 75: SIMPLE_EXPRESSION_I ⟶ ADDITIVE_OPERATOR SIMPLE_EXPRESSION" << std::endl;
        ADDITIVE_OPERATOR();
        SIMPLE_EXPRESSION();
        break;
    case tok_equal:
    case tok_notequal:
    case tok_lt:
    case tok_le:
    case tok_ge:
    case tok_gt:
    case tok_then:
    case tok_do:
    case tok_to:
    case tok_rparen:
    case tok_comma:
    case tok_else:
    case tok_end:
    case tok_semicolon:
        if (DECOMP_INFO) std::cout << "rule 76: SIMPLE_EXPRESSION_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in SIMPLE_EXPRESSION_I");
    }
}

void Descent::ADDITIVE_OPERATOR()
{
    switch(lookahead) {
    case tok_plus:
        if (DECOMP_INFO) std::cout << "rule 77: ADDITIVE_OPERATOR ⟶ plus" << std::endl;
        match(tok_plus);
        break;
    case tok_minus:
        if (DECOMP_INFO) std::cout << "rule 78: ADDITIVE_OPERATOR ⟶ minus" << std::endl;
        match(tok_minus);
        break;
    case tok_or:
        if (DECOMP_INFO) std::cout << "rule 79: ADDITIVE_OPERATOR ⟶ or" << std::endl;
        match(tok_or);
        break;
    default:
        throw ParserError("Parser error in ADDITIVE_OPERATOR");
    }
}

void Descent::TERM()
{
    switch(lookahead) {
    case tok_plus:
    case tok_minus:
    case tok_identifier:
    case tok_lparen:
    case tok_not:
    case tok_intLiteral:
    case tok_realLiteral:
    case tok_stringLiteral:
        if (DECOMP_INFO) std::cout << "rule 80: TERM ⟶ SIGNED_FACTOR TERM_I" << std::endl;
        SIGNED_FACTOR();
        TERM_I();
        break;
    default:
        throw ParserError("Parser error in TERM");
    }
}

void Descent::TERM_I()
{
    switch(lookahead) {
    case tok_star:
    case tok_div:
    case tok_mod:
    case tok_and:
        if (DECOMP_INFO) std::cout << "rule 81: TERM_I ⟶ MULTIPLICATIVE_OPERATOR TERM" << std::endl;
        MULTIPLICATIVE_OPERATOR();
        TERM();
        break;
    case tok_plus:
    case tok_minus:
    case tok_or:
    case tok_else:
    case tok_end:
    case tok_equal:
    case tok_notequal:
    case tok_lt:
    case tok_le:
    case tok_ge:
    case tok_gt:
    case tok_then:
    case tok_do:
    case tok_to:
    case tok_rparen:
    case tok_semicolon:
    case tok_comma:
        if (DECOMP_INFO) std::cout << "rule 82: TERM_I ⟶ ε " << std::endl;
        break;
    default:
        throw ParserError("Parser error in TERM_I");
    }
}

void Descent::MULTIPLICATIVE_OPERATOR()
{
    switch(lookahead) {
    case tok_star:
        if (DECOMP_INFO) std::cout << "rule 83: MULTIPLICATIVE_OPERATOR ⟶ star" << std::endl;
        match(tok_star);
        break;
    case tok_div:
        if (DECOMP_INFO) std::cout << "rule 84: MULTIPLICATIVE_OPERATOR ⟶ div" << std::endl;
        match(tok_div);
        break;
    case tok_mod:
        if (DECOMP_INFO) std::cout << "rule 85: MULTIPLICATIVE_OPERATOR ⟶ mod" << std::endl;
        match(tok_mod);
        break;
    case tok_and:
        if (DECOMP_INFO) std::cout << "rule 86: MULTIPLICATIVE_OPERATOR ⟶ and" << std::endl;
        match(tok_and);
        break;
    default:
        throw ParserError("Parser error in MULTIPLICATIVE_OPERATOR");
    }
}

void Descent::SIGNED_FACTOR()
{
    switch(lookahead) {
    case tok_plus:
    case tok_minus:
    case tok_identifier:
    case tok_lparen:
    case tok_not:
    case tok_intLiteral:
    case tok_realLiteral:
    case tok_stringLiteral:
        if (DECOMP_INFO) std::cout << "rule 87: SIGNED_FACTOR ⟶ SIGNED_FACTOR_I FACTOR" << std::endl;
        SIGNED_FACTOR_I();
        FACTOR();
        break;
    default:
        throw ParserError("Parser error in SIGNED_FACTOR");
    }
}

void Descent::SIGNED_FACTOR_I()
{
    switch(lookahead) {
    case tok_plus:
        if (DECOMP_INFO) std::cout << "rule 88: SIGNED_FACTOR_I ⟶ plus" << std::endl;
        match(tok_plus);
        break;
    case tok_minus:
        if (DECOMP_INFO) std::cout << "rule 89: SIGNED_FACTOR_I ⟶ minus" << std::endl;
        match(tok_minus);
        break;
    case tok_identifier:
    case tok_lparen:
    case tok_not:
    case tok_intLiteral:
    case tok_realLiteral:
    case tok_stringLiteral:
        if (DECOMP_INFO) std::cout << "rule 90: SIGNED_FACTOR_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in SIGNED_FACTOR_I");
    }
}

void Descent::FACTOR()
{
    switch(lookahead) {
    case tok_identifier:
        if (DECOMP_INFO) std::cout << "rule 91: FACTOR ⟶ ident FACTOR_I" << std::endl;
        match(tok_identifier);
        FACTOR_I();
        break;
    case tok_lparen:
        if (DECOMP_INFO) std::cout << "rule 92: FACTOR ⟶ lparen EXPRESSION rparen" << std::endl;
        match(tok_lparen);
        EXPRESSION();
        match(tok_rparen);
        break;
    case tok_intLiteral:
    case tok_realLiteral:
    case tok_stringLiteral:
        if (DECOMP_INFO) std::cout << "rule 93: FACTOR ⟶ UNSIGNED_CONSTANT" << std::endl;
        UNSIGNED_CONSTANT();
        break;
    case tok_not:
        if (DECOMP_INFO) std::cout << "rule 94: FACTOR ⟶ not FACTOR" << std::endl;
        match(tok_not);
        FACTOR();
        break;
    default:
        throw ParserError("Parser error in FACTOR");
    }
}

void Descent::FACTOR_I()
{
    switch(lookahead) {
    case tok_lparen:
        if (DECOMP_INFO) std::cout << "rule 95: FACTOR_I ⟶ lparen PARAMETER_LIST rparen" << std::endl;
        match(tok_lparen);
        PARAMETER_LIST();
        match(tok_rparen);
        break;
    case tok_and:
    case tok_comma:
    case tok_div:
    case tok_do:
    case tok_else:
    case tok_end:
    case tok_equal:
    case tok_ge:
    case tok_gt:
    case tok_le:
    case tok_lt:
    case tok_minus:
    case tok_mod:
    case tok_notequal:
    case tok_or:
    case tok_plus:
    case tok_rparen:
    case tok_semicolon:
    case tok_star:
    case tok_then:
    case tok_to:
        if (DECOMP_INFO) std::cout << "rule 96: FACTOR_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in FACTOR_I");
    }
}

void Descent::UNSIGNED_CONSTANT()
{
    switch(lookahead) {
    case tok_intLiteral:
    case tok_realLiteral:
        if (DECOMP_INFO) std::cout << "rule 97: UNSIGNED_CONSTANT ⟶ UNSIGNED_NUMBER" << std::endl;
        UNSIGNED_NUMBER();
        break;
    case tok_stringLiteral:
        if (DECOMP_INFO) std::cout << "rule 98: UNSIGNED_CONSTANT ⟶ string_literal" << std::endl;
        match(tok_stringLiteral);
        break;
    default:
        throw ParserError("Parser error in UNSIGNED_CONSTANT");
    }
}

void Descent::PARAMETER_LIST()
{
    switch(lookahead) {
    case tok_plus:
    case tok_minus:
    case tok_identifier:
    case tok_lparen:
    case tok_not:
    case tok_intLiteral:
    case tok_realLiteral:
    case tok_stringLiteral:
        if (DECOMP_INFO) std::cout << "rule 99: PARAMETER_LIST ⟶ EXPRESSION PARAMETER_LIST_I" << std::endl;
        EXPRESSION();
        PARAMETER_LIST_I();
        break;
    default:
        throw ParserError("Parser error in PARAMETER_LIST");
    }
}

void Descent::PARAMETER_LIST_I()
{
    switch(lookahead) {
    case tok_comma:
        if (DECOMP_INFO) std::cout << "rule 100: PARAMETER_LIST_I ⟶ comma EXPRESSION PARAMETER_LIST_I" << std::endl;
        match(tok_comma);
        EXPRESSION();
        PARAMETER_LIST_I();
        break;
    case tok_rparen:
        if (DECOMP_INFO) std::cout << "rule 101: PARAMETER_LIST_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in PARAMETER_LIST_I");
    }
}
