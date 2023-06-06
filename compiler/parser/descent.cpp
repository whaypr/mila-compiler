#include "descent.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>


Token Descent::match( Token tok ) {
    if (lookahead != tok)
        throw ParserError("Parser error in MATCH");
    return lookahead = lexer.nextToken();
}


std::string Descent::ParserError( std::string msg ) {
    return msg;
}


// ----------------------------------------------------------------
std::unique_ptr<ProgramASTNode> Descent::PROGRAM()
{
    switch(lookahead) {
    case tok_program: {
        if (DECOMP_INFO) std::cout << "rule 1: PROGRAM ⟶ program ident semi BLOCK dot" << std::endl;
        match(tok_program);
        match(tok_identifier); // program name - useless
        match(tok_semicolon);
        auto block = BLOCK();
        match(tok_dot);

        return std::make_unique<ProgramASTNode>(std::move(block));
    }
    default:
        throw ParserError("Parser error in PROGRAM");
    }
}

std::unique_ptr<BlockASTNode> Descent::BLOCK()
{
    switch(lookahead) {
    case tok_const:
    case tok_var:
    case tok_procedure:
    case tok_function:
    case tok_begin: {
        if (DECOMP_INFO) std::cout << "rule 2: BLOCK ⟶ BLOCK_I COMPOUND_STATEMENT" << std::endl;
        std::vector<std::unique_ptr<StatementASTNode>> statements = std::vector<std::unique_ptr<StatementASTNode>>();
        BLOCK_I(statements);
        auto followingStatement = COMPOUND_STATEMENT();
        statements.push_back(std::unique_ptr<StatementASTNode>(std::move(followingStatement)));
        return std::make_unique<BlockASTNode>(std::move(statements));
    }
    default:
        throw ParserError("Parser error in BLOCK");
    }
}

void Descent::BLOCK_I( std::vector<std::unique_ptr<StatementASTNode>> &statements )
{
    switch(lookahead) {
    case tok_const:
        if (DECOMP_INFO) std::cout << "rule 3: BLOCK_I ⟶ CONSTANT_DEFINITION BLOCK_I" << std::endl;
        CONSTANT_DEFINITION(statements);
        BLOCK_I(statements);
        break;
    case tok_var:
        if (DECOMP_INFO) std::cout << "rule 4: BLOCK_I ⟶ VARIABLE_DECLARATION_PART BLOCK_I" << std::endl;
        VARIABLE_DECLARATION_PART(statements);
        BLOCK_I(statements);
        break;
    case tok_procedure:
    case tok_function:
        if (DECOMP_INFO) std::cout << "rule 5: BLOCK_I ⟶ PROCEDURE_AND_FUNCTION_DECLARATION_PART BLOCK_I" << std::endl;
        PROCEDURE_AND_FUNCTION_DECLARATION_PART(statements);
        BLOCK_I(statements);
        break;
    case tok_begin:
        if (DECOMP_INFO) std::cout << "rule 6: BLOCK_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in BLOCK_I");
    }
}

void Descent::CONSTANT_DEFINITION( std::vector<std::unique_ptr<StatementASTNode>> &statements )
{
    switch(lookahead) {
    case tok_const: {
        if (DECOMP_INFO) std::cout << "rule 7: CONSTANT_DEFINITION ⟶ const ident equal CONSTANT semi CONSTANT_DEFINITION_I" << std::endl;
        match(tok_const);
        std::string ident = lexer.identifierStr();
        match(tok_identifier);
        match(tok_equal);
        std::unique_ptr<LiteralASTNode> val = CONSTANT(); // TODO víc než jen int
        match(tok_semicolon);

        statements.push_back(std::make_unique<ConstDefASTNode>(std::move(ident), std::move(val)));
        CONSTANT_DEFINITION_I(statements);
        break;
    }
    default:
        throw ParserError("Parser error in CONSTANT_DEFINITION");
    }
}

void Descent::CONSTANT_DEFINITION_I( std::vector<std::unique_ptr<StatementASTNode>> &statements )
{
    switch(lookahead) {
    case tok_identifier: {
        if (DECOMP_INFO) std::cout << "rule 8: CONSTANT_DEFINITION_I ⟶ ident equal CONSTANT semi CONSTANT_DEFINITION_I" << std::endl;
        std::string ident = lexer.identifierStr();
        match(tok_identifier);
        match(tok_equal);
        std::unique_ptr<LiteralASTNode> val = CONSTANT(); // TODO víc než jen int
        match(tok_semicolon);

        statements.push_back(std::make_unique<ConstDefASTNode>(std::move(ident), std::move(val)));
        CONSTANT_DEFINITION_I(statements);
        break;
    }
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

std::unique_ptr<LiteralASTNode> Descent::CONSTANT()
{
    switch(lookahead) {
    case tok_intLiteral:
    case tok_realLiteral:
        if (DECOMP_INFO) std::cout << "rule 10: CONSTANT ⟶ UNSIGNED_NUMBER" << std::endl;
        return UNSIGNED_NUMBER(); // TODO víc než int
    case tok_plus:
        if (DECOMP_INFO) std::cout << "rule 11: CONSTANT ⟶ plus CONSTANT_L" << std::endl;
        match(tok_plus);
        return CONSTANT_L(); // TODO víc než int
    case tok_identifier:
        if (DECOMP_INFO) std::cout << "rule 12: CONSTANT ⟶ ident" << std::endl;
        match(tok_identifier);
        return std::make_unique<LiteralASTNode>(0); // TODO zde identifier (hodnota z tabulky symbolů asi)
    case tok_minus:
        if (DECOMP_INFO) std::cout << "rule 13: CONSTANT ⟶ minus CONSTANT_L" << std::endl;
        match(tok_minus);
        return std::make_unique<LiteralASTNode>(- CONSTANT_L()->getVal()); // TODO víc než int
    case tok_stringLiteral:
        if (DECOMP_INFO) std::cout << "rule 14: CONSTANT ⟶ string_literal" << std::endl;
        match(tok_stringLiteral);
        return std::make_unique<LiteralASTNode>(0); // TODO zde string ( lexer.stringVal() )
    default:
        throw ParserError("Parser error in CONSTANT");
    }
}

std::unique_ptr<LiteralASTNode> Descent::CONSTANT_L()
{
    switch(lookahead) {
    case tok_intLiteral:
    case tok_realLiteral:
        if (DECOMP_INFO) std::cout << "rule 15: CONSTANT_L ⟶ UNSIGNED_NUMBER" << std::endl;
        return UNSIGNED_NUMBER();
    case tok_identifier:
        if (DECOMP_INFO) std::cout << "rule 16: CONSTANT_L ⟶ ident" << std::endl;
        match(tok_identifier);
        return std::make_unique<LiteralASTNode>(0); // TODO zde hodnota z tabulky symbolů asi
    default:
        throw ParserError("Parser error in CONSTANT_L");
    }
}

std::unique_ptr<LiteralASTNode> Descent::UNSIGNED_NUMBER()
{
    switch(lookahead) {
    case tok_intLiteral: {
        if (DECOMP_INFO) std::cout << "rule 17: UNSIGNED_NUMBER -> num_int" << std::endl;
        auto val = lexer.intVal();
        match(tok_intLiteral);
        return std::make_unique<LiteralASTNode>(val);
    }
    case tok_realLiteral: {
        if (DECOMP_INFO) std::cout << "rule 18: UNSIGNED_NUMBER -> num_real" << std::endl;
        auto val = lexer.realVal();
        match(tok_realLiteral);
        return std::make_unique<LiteralASTNode>(val); // TODO bude castnuto na int
    }
    default:
        throw ParserError("Parser error in UNSIGNED_NUMBER");
    }
}

void Descent::VARIABLE_DECLARATION_PART( std::vector<std::unique_ptr<StatementASTNode>> &statements )
{
    switch(lookahead) {
    case tok_var:
        if (DECOMP_INFO) std::cout << "rule 19: VARIABLE_DECLARATION_PART ⟶ var VARIABLE_DECLARATION VARIABLE_DECLARATION_PART_I_semi" << std::endl;
        match(tok_var);
        VARIABLE_DECLARATION(statements);
        VARIABLE_DECLARATION_PART_I_semi(statements);
        break;
    default:
        throw ParserError("Parser error in VARIABLE_DECLARATION_PART");
    }
}

void Descent::VARIABLE_DECLARATION_PART_I_semi( std::vector<std::unique_ptr<StatementASTNode>> &statements )
{
    switch(lookahead) {
    case tok_semicolon:
        if (DECOMP_INFO) std::cout << "rule 20: VARIABLE_DECLARATION_PART_I_semi ⟶ semi VARIABLE_DECLARATION_PART_I_semi_L" << std::endl;
        match(tok_semicolon);
        VARIABLE_DECLARATION_PART_I_semi_L(statements);
        break;
    default:
        throw ParserError("Parser error in VARIABLE_DECLARATION_PART_I_semi");
    }
}

void Descent::VARIABLE_DECLARATION_PART_I_semi_L( std::vector<std::unique_ptr<StatementASTNode>> &statements )
{
    switch(lookahead) {
    case tok_identifier:
        if (DECOMP_INFO) std::cout << "rule 21: VARIABLE_DECLARATION_PART_I_semi_L ⟶ VARIABLE_DECLARATION VARIABLE_DECLARATION_PART_I_semi" << std::endl;
        VARIABLE_DECLARATION(statements);
        VARIABLE_DECLARATION_PART_I_semi(statements);
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

void Descent::VARIABLE_DECLARATION( std::vector<std::unique_ptr<StatementASTNode>> &statements )
{
    switch(lookahead) {
    case tok_identifier: {
        if (DECOMP_INFO) std::cout << "rule 23: VARIABLE_DECLARATION ⟶ IDENTIFIER_LIST colon TYPE_IDENTIFIER" << std::endl;
        auto identifiers = IDENTIFIER_LIST();
        match(tok_colon);
        auto type = TYPE_IDENTIFIER();

        for ( const auto & ident : identifiers ) {
            auto typeNode = std::make_unique<TypeASTNode>(type);
            statements.push_back( std::make_unique<VarDeclASTNode>(ident, std::move(typeNode)) );
        }
        break;
    }
    default:
        throw ParserError("Parser error in VARIABLE_DECLARATION");
    }
}

std::vector<std::string> Descent::IDENTIFIER_LIST()
{
    switch(lookahead) {
    case tok_identifier: {
        if (DECOMP_INFO) std::cout << "rule 24: IDENTIFIER_LIST ⟶ ident IDENTIFIER_LIST_I" << std::endl;
        std::vector<std::string> identificators = std::vector<std::string>();
        identificators.push_back( lexer.identifierStr() );
        match(tok_identifier);

        IDENTIFIER_LIST_I(identificators);
        return identificators;
    }
    default:
        throw ParserError("Parser error in IDENTIFIER_LIST");
    }
}

void Descent::IDENTIFIER_LIST_I( std::vector<std::string> &identificators )
{
    switch(lookahead) {
    case tok_comma: {
        if (DECOMP_INFO) std::cout << "rule 25: IDENTIFIER_LIST_I ⟶ comma ident IDENTIFIER_LIST_I" << std::endl;
        match(tok_comma);
        identificators.push_back( lexer.identifierStr() );
        match(tok_identifier);
        IDENTIFIER_LIST_I(identificators);
        break;
    }
    case tok_colon:
        if (DECOMP_INFO) std::cout << "rule 26: IDENTIFIER_LIST_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in IDENTIFIER_LIST_I");
    }
}

TypeASTNode::Type Descent::TYPE_IDENTIFIER()
{
    switch(lookahead) {
    case tok_identifier:
        if (DECOMP_INFO) std::cout << "rule 27: TYPE_IDENTIFIER ⟶ ident" << std::endl;
        match(tok_identifier);
        return TypeASTNode::Type::INT; // TODO
    case tok_integer:
        if (DECOMP_INFO) std::cout << "rule 28: TYPE_IDENTIFIER ⟶ integer" << std::endl;
        match(tok_integer);
        return TypeASTNode::Type::INT;
    case tok_real:
        if (DECOMP_INFO) std::cout << "rule 29: TYPE_IDENTIFIER ⟶ real" << std::endl;
        match(tok_real);
        return TypeASTNode::Type::DOUBLE;
    case tok_string:
        if (DECOMP_INFO) std::cout << "rule 30: TYPE_IDENTIFIER ⟶ string" << std::endl;
        match(tok_string);
        return TypeASTNode::Type::INT; // TODO
    default:
        throw ParserError("Parser error in TYPE_IDENTIFIER");
    }
}

void Descent::PROCEDURE_AND_FUNCTION_DECLARATION_PART( std::vector<std::unique_ptr<StatementASTNode>> &statements )
{
    switch(lookahead) {
    case tok_procedure:
    case tok_function:
        if (DECOMP_INFO) std::cout << "rule 31: PROCEDURE_AND_FUNCTION_DECLARATION_PART ⟶ PROCEDURE_OR_FUNCTION_DECLARATION semi" << std::endl;
        PROCEDURE_OR_FUNCTION_DECLARATION(statements);
        match(tok_semicolon);
        break;
    default:
        throw ParserError("Parser error in PROCEDURE_AND_FUNCTION_DECLARATION_PART");
    }
}

void Descent::PROCEDURE_OR_FUNCTION_DECLARATION( std::vector<std::unique_ptr<StatementASTNode>> &statements )
{
    switch(lookahead) {
    case tok_procedure:
        if (DECOMP_INFO) std::cout << "rule 32: PROCEDURE_OR_FUNCTION_DECLARATION ⟶ PROCEDURE_DECLARATION" << std::endl;
        PROCEDURE_DECLARATION(statements);
        break;
    case tok_function:
        if (DECOMP_INFO) std::cout << "rule 33: PROCEDURE_OR_FUNCTION_DECLARATION ⟶ FUNCTION_DECLARATION" << std::endl;
        FUNCTION_DECLARATION(statements);
        break;
    default:
        throw ParserError("Parser error in PROCEDURE_OR_FUNCTION_DECLARATION");
    }
}

void Descent::PROCEDURE_DECLARATION( std::vector<std::unique_ptr<StatementASTNode>> &statements )
{
    switch(lookahead) {
    case tok_procedure: {
        if (DECOMP_INFO) std::cout << "rule 34: PROCEDURE_DECLARATION ⟶ procedure ident FORMAL_PARAMETER_LIST_OPT semi FORWARD_OR_BLOCK" << std::endl;
        match(tok_procedure);
        std::string ident = lexer.identifierStr();
        match(tok_identifier);
        auto params = FORMAL_PARAMETER_LIST_OPT();
        match(tok_semicolon);
        auto block = FORWARD_OR_BLOCK();

        statements.push_back( std::make_unique<ProcDeclASTNode>(ident, std::move(params), std::move(block)) );
        break;
    }
    default:
        throw ParserError("Parser error in PROCEDURE_DECLARATION");
    }
}

std::vector<std::unique_ptr<VarDeclASTNode>> Descent::FORMAL_PARAMETER_LIST_OPT()
{
    switch(lookahead) {
    case tok_lparen:
        if (DECOMP_INFO) std::cout << "rule 35: FORMAL_PARAMETER_LIST_OPT ⟶ FORMAL_PARAMETER_LIST" << std::endl;
        return FORMAL_PARAMETER_LIST();
    case tok_semicolon:
    case tok_colon:
        if (DECOMP_INFO) std::cout << "rule 36: FORMAL_PARAMETER_LIST_OPT ⟶ ε" << std::endl;
        return std::vector<std::unique_ptr<VarDeclASTNode>>();
    default:
        throw ParserError("Parser error in FORMAL_PARAMETER_LIST_OPT");
    }
}

std::vector<std::unique_ptr<VarDeclASTNode>> Descent::FORMAL_PARAMETER_LIST()
{
    switch(lookahead) {
    case tok_lparen: {
        if (DECOMP_INFO) std::cout << "rule 37: FORMAL_PARAMETER_LIST ⟶ lparen PARAMETER_GROUP FORMAL_PARAMETER_LIST_I rparen" << std::endl;
        match(tok_lparen);

        std::vector<std::unique_ptr<VarDeclASTNode>> parameters;
        PARAMETER_GROUP(parameters);
        FORMAL_PARAMETER_LIST_I(parameters);
        match(tok_rparen);
        return parameters;
    }
    default:
        throw ParserError("Parser error in FORMAL_PARAMETER_LIST");
    }
}

void Descent::FORMAL_PARAMETER_LIST_I( std::vector<std::unique_ptr<VarDeclASTNode>> &parameters )
{
    switch(lookahead) {
    case tok_semicolon:
        if (DECOMP_INFO) std::cout << "rule 38: FORMAL_PARAMETER_LIST_I ⟶ semi PARAMETER_GROUP FORMAL_PARAMETER_LIST_I" << std::endl;
        match(tok_semicolon);
        PARAMETER_GROUP(parameters);
        FORMAL_PARAMETER_LIST_I(parameters);
        break;
    case tok_rparen:
        if (DECOMP_INFO) std::cout << "rule 39: FORMAL_PARAMETER_LIST_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in FORMAL_PARAMETER_LIST_I");
    }
}

void Descent::PARAMETER_GROUP( std::vector<std::unique_ptr<VarDeclASTNode>> &parameters )
{
    switch(lookahead) {
    case tok_identifier: {
        if (DECOMP_INFO) std::cout << "rule 40: PARAMETER_GROUP ⟶ IDENTIFIER_LIST colon TYPE_IDENTIFIER" << std::endl;
        auto identifiers = IDENTIFIER_LIST();
        match(tok_colon);
        TypeASTNode::Type type = TYPE_IDENTIFIER();

        for ( const auto & ident : identifiers ) {
            auto typeNode = std::make_unique<TypeASTNode>(type);
            parameters.push_back( std::make_unique<VarDeclASTNode>(ident, std::move(typeNode)) );
        }
        break;
    }
    default:
        throw ParserError("Parser error in PARAMETER_GROUP");
    }
}

std::unique_ptr<BlockASTNode> Descent::FORWARD_OR_BLOCK()
{
    switch(lookahead) {
    case tok_forward:
        if (DECOMP_INFO) std::cout << "rule 41: FORWARD_OR_BLOCK ⟶ forward" << std::endl;
        match(tok_forward);
        return nullptr; // TODO
    case tok_begin:
    case tok_const:
    case tok_function:
    case tok_procedure:
    case tok_var:
        if (DECOMP_INFO) std::cout << "rule 42: FORWARD_OR_BLOCK ⟶ BLOCK" << std::endl;
        return BLOCK();
    default:
        throw ParserError("Parser error in FORWARD");
    }
}

void Descent::FUNCTION_DECLARATION( std::vector<std::unique_ptr<StatementASTNode>> &statements )
{
    switch(lookahead) {
    case tok_function: {
        if (DECOMP_INFO) std::cout << "rule 43: FUNCTION_DECLARATION ⟶ function ident FORMAL_PARAMETER_LIST_OPT colon TYPE_IDENTIFIER semi FORWARD_OR_BLOCK" << std::endl;
        match(tok_function);
        std::string ident = lexer.identifierStr();
        match(tok_identifier);
        auto params = FORMAL_PARAMETER_LIST_OPT();
        match(tok_colon);
        auto type = TYPE_IDENTIFIER();
        match(tok_semicolon);
        auto block = FORWARD_OR_BLOCK();

        statements.push_back( std::make_unique<FunDeclASTNode>(ident, std::move(params), std::move(block), std::make_unique<TypeASTNode>(type)) );
        break;
    }
    default:
        throw ParserError("Parser error in FUNCTION_DECLARATION");
    }
}

std::unique_ptr<StatementASTNode> Descent::STATEMENT()
{
    switch(lookahead) {
    case tok_else:
    case tok_end:
    case tok_identifier:
    case tok_semicolon:
    case tok_exit:
        if (DECOMP_INFO) std::cout << "rule 42: STATEMENT ⟶ SIMPLE_STATEMENT" << std::endl;
        return SIMPLE_STATEMENT();
    case tok_begin:
    case tok_for:
    case tok_if:
    case tok_while:
        if (DECOMP_INFO) std::cout << "rule 43: STATEMENT ⟶ STRUCTURED_STATEMENT" << std::endl;
        return STRUCTURED_STATEMENT();
    default:
        throw ParserError("Parser error in STATEMENT");
    }
}

std::unique_ptr<StatementASTNode> Descent::SIMPLE_STATEMENT()
{
    switch(lookahead) {
    case tok_else:
    case tok_end:
    case tok_semicolon:
        if (DECOMP_INFO) std::cout << "rule 44: SIMPLE_STATEMENT ⟶ EMPTY_STATEMENT" << std::endl;
        return EMPTY_STATEMENT();
    case tok_exit:
        if (DECOMP_INFO) std::cout << "rule 45: SIMPLE_STATEMENT ⟶ exit" << std::endl;
        match(tok_exit);
        return std::make_unique<ProcCallASTNode>("exit", std::vector<std::unique_ptr<ExprASTNode>>());
    case tok_identifier: {
        if (DECOMP_INFO) std::cout << "rule 46: SIMPLE_STATEMENT ⟶ ident SIMPLE_STATEMENT_L" << std::endl;
        std::string ident = lexer.identifierStr();
        match(tok_identifier);
        return SIMPLE_STATEMENT_L(ident);
    }
    default:
        throw ParserError("Parser error in SIMPLE_STATEMENT");
    }
}

std::unique_ptr<StatementASTNode> Descent::SIMPLE_STATEMENT_L( std::string ident )
{
    switch(lookahead) {
    case tok_assign:
        if (DECOMP_INFO) std::cout << "rule 47: SIMPLE_STATEMENT_L ⟶ assign EXPRESSION" << std::endl;
        match(tok_assign);
        return std::make_unique<AssignASTNode>( std::make_unique<DeclRefASTNode>(ident), EXPRESSION() );
    case tok_lparen:
    case tok_semicolon:
    case tok_else:
    case tok_end:
        if (DECOMP_INFO) std::cout << "rule 48: SIMPLE_STATEMENT_L ⟶ PROCEDURE_STATEMENT_I" << std::endl;
        return std::make_unique<ProcCallASTNode>(ident, PROCEDURE_STATEMENT_I());
    default:
        throw ParserError("Parser error in SIMPLE_STATEMENT_L");
    }
}

std::vector<std::unique_ptr<ExprASTNode>> Descent::PROCEDURE_STATEMENT_I()
{
    switch(lookahead) {
    case tok_lparen: {
        if (DECOMP_INFO) std::cout << "rule 49: PROCEDURE_STATEMENT_I ⟶ lparen PARAMETER_LIST rparen" << std::endl;
        match(tok_lparen);
        std::vector<std::unique_ptr<ExprASTNode>> params = PARAMETER_LIST();
        match(tok_rparen);
        return params;
    }
    case tok_semicolon:
    case tok_end:
    case tok_else:
        if (DECOMP_INFO) std::cout << "rule 50: PROCEDURE_STATEMENT_I ⟶ ε" << std::endl;
        return std::vector<std::unique_ptr<ExprASTNode>>();
    default:
        throw ParserError("Parser error in PROCEDURE_STATEMENT_I");
    }
}

std::unique_ptr<StatementASTNode> Descent::STRUCTURED_STATEMENT()
{
    switch(lookahead) {
    case tok_begin:
        if (DECOMP_INFO) std::cout << "rule 51: STRUCTURED_STATEMENT ⟶ COMPOUND_STATEMENT" << std::endl;
        return COMPOUND_STATEMENT();;
    case tok_if:
        if (DECOMP_INFO) std::cout << "rule 52: STRUCTURED_STATEMENT ⟶ IF_STATEMENT" << std::endl;
        return IF_STATEMENT();
    case tok_for:
    case tok_while:
        if (DECOMP_INFO) std::cout << "rule 53: STRUCTURED_STATEMENT ⟶ REPETETIVE_STATEMENT" << std::endl;
        return REPETETIVE_STATEMENT();
    default:
        throw ParserError("Parser error in STRUCTURED_STATEMENT");
    }
}

std::unique_ptr<CompoundStmtASTNode> Descent::COMPOUND_STATEMENT()
{
    switch(lookahead) {
    case tok_begin: {
        if (DECOMP_INFO) std::cout << "rule 54: COMPOUND_STATEMENT ⟶ begin STATEMENT COMPOUND_STATEMENT_I end" << std::endl;
        std::vector<std::unique_ptr<StatementASTNode>> statements;
        
        match(tok_begin);
        statements.push_back( STATEMENT() );
        COMPOUND_STATEMENT_I(statements);
        match(tok_end);
        return std::make_unique<CompoundStmtASTNode>(std::move(statements));
    }
    default:
        throw ParserError("Parser error in COMPOUND_STATEMENT");
    }
}

void Descent::COMPOUND_STATEMENT_I( std::vector<std::unique_ptr<StatementASTNode>> &statements )
{
    switch(lookahead) {
    case tok_semicolon:
        if (DECOMP_INFO) std::cout << "rule 55: COMPOUND_STATEMENT_I ⟶ semi STATEMENT COMPOUND_STATEMENT_I" << std::endl;
        match(tok_semicolon);
        statements.push_back( STATEMENT() );
        COMPOUND_STATEMENT_I(statements);
        break;
    case tok_end:
        if (DECOMP_INFO) std::cout << "rule 56: COMPOUND_STATEMENT_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in COMPOUND_STATEMENT_I");
    }
}

std::unique_ptr<StatementASTNode> Descent::IF_STATEMENT()
{
    switch(lookahead) {
    case tok_if: {
        if (DECOMP_INFO) std::cout << "rule 57: IF_STATEMENT ⟶ if EXPRESSION then STATEMENT IF_STATEMENT_I" << std::endl;
        match(tok_if);
        std::unique_ptr<ExprASTNode> cond = EXPRESSION();

        match(tok_then);
        std::unique_ptr<StatementASTNode> body = STATEMENT();

        std::unique_ptr<StatementASTNode> elseBody = IF_STATEMENT_I();

        return std::make_unique<IfASTNode>(
            std::move(cond), std::move(body), std::move(elseBody));
    }
    default:
        throw ParserError("Parser error in IF_STATEMENT");
    }
}

std::unique_ptr<StatementASTNode> Descent::IF_STATEMENT_I()
{
    switch(lookahead) {
    case tok_else:
        if (DECOMP_INFO) std::cout << "rule 58: IF_STATEMENT_I ⟶ else STATEMENT" << std::endl; // could be rule 58 as well
        match(tok_else);
        return STATEMENT();
    case tok_end:
    case tok_semicolon:
        if (DECOMP_INFO) std::cout << "rule 59: IF_STATEMENT_I ⟶ ε" << std::endl;
        return nullptr;
    default:
        throw ParserError("Parser error in IF_STATEMENT_I");
    }
}

std::unique_ptr<StatementASTNode> Descent::REPETETIVE_STATEMENT()
{
    switch(lookahead) {
    case tok_while:
        if (DECOMP_INFO) std::cout << "rule 60: REPETETIVE_STATEMENT ⟶ WHILE_STATEMENT" << std::endl;
        return WHILE_STATEMENT();
    case tok_for:
        if (DECOMP_INFO) std::cout << "rule 61: REPETETIVE_STATEMENT ⟶ FOR_STATEMENT" << std::endl;
        return FOR_STATEMENT();
    default:
        throw ParserError("Parser error in REPETETIVE_STATEMENT");
    }
}

std::unique_ptr<WhileASTNode> Descent::WHILE_STATEMENT()
{
    switch(lookahead) {
    case tok_while: {
        if (DECOMP_INFO) std::cout << "rule 62: WHILE_STATEMENT ⟶ while EXPRESSION do STATEMENT" << std::endl;
        match(tok_while);
        auto cond = EXPRESSION();
        match(tok_do);
        auto body = STATEMENT();
        return std::make_unique<WhileASTNode>(std::move(cond), std::move(body));
    }
    default:
        throw ParserError("Parser error in WHILE_STATEMENT");
    }
}

std::unique_ptr<ForASTNode> Descent::FOR_STATEMENT()
{
    switch(lookahead) {
    case tok_for: {
        if (DECOMP_INFO) std::cout << "rule 63: FOR_STATEMENT ⟶ for ident assign EXPRESSION to EXPRESSION do STATEMENT" << std::endl;
        match(tok_for);
        std::string ident = lexer.identifierStr();
        match(tok_identifier);
        match(tok_assign);
        auto init = std::make_unique<AssignASTNode>(std::make_unique<DeclRefASTNode>(ident), EXPRESSION());

        bool isTo = true;
        if (lookahead == tok_to) { // hack to enable "downto" option without changing the grammar
            match(tok_to);
        } else {
            match(tok_downto);
            isTo = false;
        }

        auto to = EXPRESSION();
        match(tok_do);
        return std::make_unique<ForASTNode>(std::move(init), std::move(to), STATEMENT(), isTo );
    }
    default:
        throw ParserError("Parser error in FOR_STATEMENT");
    }
}

std::unique_ptr<StatementASTNode> Descent::EMPTY_STATEMENT()
{
    switch(lookahead) {
    case tok_else:
    case tok_end:
    case tok_semicolon:
        if (DECOMP_INFO) std::cout << "rule 64: EMPTY_STATEMENT ⟶ ε" << std::endl;
        return std::make_unique<EmptyStmtASTNode>();
    default:
        throw ParserError("Parser error in EMPTY_STATEMENT");
    }
}

std::unique_ptr<ExprASTNode> Descent::EXPRESSION()
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
        return EXPRESSION_I( SIMPLE_EXPRESSION() );
    default:
        throw ParserError("Parser error in EXPRESSION");
    }
}

std::unique_ptr<ExprASTNode> Descent::EXPRESSION_I( std::unique_ptr<ExprASTNode> lhs )
{
    switch(lookahead) {
    case tok_equal:
    case tok_notequal:
    case tok_lt:
    case tok_le:
    case tok_ge:
    case tok_gt: {
        if (DECOMP_INFO) std::cout << "rule 66: EXPRESSION_I ⟶ RELATIONAL_OPERATOR EXPRESSION" << std::endl;
        Token tok = RELATIONAL_OPERATOR();
        auto rhs = EXPRESSION();
        return std::make_unique<BinOpASTNode>(tok, std::move(lhs), std::move(rhs));
    }
    case tok_then:
    case tok_do:
    case tok_to:
    case tok_downto: // hack - not in the grammar
    case tok_rparen:
    case tok_comma:
    case tok_else:
    case tok_end:
    case tok_semicolon:
        if (DECOMP_INFO) std::cout << "rule 67: EXPRESSION_I ⟶ ε" << std::endl;
        return lhs;
    default:
        throw ParserError("Parser error in EXPRESSION_I");
    }
}

Token Descent::RELATIONAL_OPERATOR()
{
    switch(lookahead) {
    case tok_equal:
        if (DECOMP_INFO) std::cout << "rule 68: RELATIONAL_OPERATOR ⟶ equal" << std::endl;
        match(tok_equal);
        return tok_equal;
    case tok_notequal:
        if (DECOMP_INFO) std::cout << "rule 69: RELATIONAL_OPERATOR ⟶ not_equal" << std::endl;
        match(tok_notequal);
        return tok_notequal;
    case tok_lt:
        if (DECOMP_INFO) std::cout << "rule 70: RELATIONAL_OPERATOR ⟶ lt" << std::endl;
        match(tok_lt);
        return tok_lt;
    case tok_le:
        if (DECOMP_INFO) std::cout << "rule 71: RELATIONAL_OPERATOR ⟶ le" << std::endl;
        match(tok_le);
        return tok_le;
    case tok_ge:
        if (DECOMP_INFO) std::cout << "rule 72: RELATIONAL_OPERATOR ⟶ ge" << std::endl;
        match(tok_ge);
        return tok_ge;
    case tok_gt:
        if (DECOMP_INFO) std::cout << "rule 73: RELATIONAL_OPERATOR ⟶ gt" << std::endl;
        match(tok_gt);
        return tok_gt;
    default:
        throw ParserError("Parser error in RELATIONAL_OPERATOR");
    }
}

std::unique_ptr<ExprASTNode> Descent::SIMPLE_EXPRESSION()
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
        return SIMPLE_EXPRESSION_I( TERM() );
    default:
        throw ParserError("Parser error in SIMPLE_EXPRESSION");
    }
}

std::unique_ptr<ExprASTNode> Descent::SIMPLE_EXPRESSION_I( std::unique_ptr<ExprASTNode> lhs )
{
    switch(lookahead) {
    case tok_plus:
    case tok_minus:
    case tok_or: {
        if (DECOMP_INFO) std::cout << "rule 75: SIMPLE_EXPRESSION_I ⟶ ADDITIVE_OPERATOR SIMPLE_EXPRESSION" << std::endl;
        Token tok = ADDITIVE_OPERATOR();
        auto rhs = SIMPLE_EXPRESSION();
        return std::make_unique<BinOpASTNode>(tok, std::move(lhs), std::move(rhs));
    }
    case tok_equal:
    case tok_notequal:
    case tok_lt:
    case tok_le:
    case tok_ge:
    case tok_gt:
    case tok_then:
    case tok_do:
    case tok_to:
    case tok_downto: // hack - not in the grammar
    case tok_rparen:
    case tok_comma:
    case tok_else:
    case tok_end:
    case tok_semicolon:
        if (DECOMP_INFO) std::cout << "rule 76: SIMPLE_EXPRESSION_I ⟶ ε" << std::endl;
        return lhs;
    default:
        throw ParserError("Parser error in SIMPLE_EXPRESSION_I");
    }
}

Token Descent::ADDITIVE_OPERATOR()
{
    switch(lookahead) {
    case tok_plus:
        if (DECOMP_INFO) std::cout << "rule 77: ADDITIVE_OPERATOR ⟶ plus" << std::endl;
        match(tok_plus);
        return tok_plus;
    case tok_minus:
        if (DECOMP_INFO) std::cout << "rule 78: ADDITIVE_OPERATOR ⟶ minus" << std::endl;
        match(tok_minus);
        return tok_minus;
    case tok_or:
        if (DECOMP_INFO) std::cout << "rule 79: ADDITIVE_OPERATOR ⟶ or" << std::endl;
        match(tok_or);
        return tok_or;
    default:
        throw ParserError("Parser error in ADDITIVE_OPERATOR");
    }
}

std::unique_ptr<ExprASTNode> Descent::TERM()
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
        return TERM_I( SIGNED_FACTOR() );
    default:
        throw ParserError("Parser error in TERM");
    }
}

std::unique_ptr<ExprASTNode> Descent::TERM_I( std::unique_ptr<ExprASTNode> lhs )
{
    switch(lookahead) {
    case tok_star:
    case tok_div:
    case tok_mod:
    case tok_and: {
        if (DECOMP_INFO) std::cout << "rule 81: TERM_I ⟶ MULTIPLICATIVE_OPERATOR TERM" << std::endl;
        Token tok = MULTIPLICATIVE_OPERATOR();
        auto rhs = TERM();
        return std::make_unique<BinOpASTNode>(tok, std::move(lhs), std::move(rhs));
    }
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
    case tok_downto: // hack - not in the grammar
    case tok_rparen:
    case tok_semicolon:
    case tok_comma:
        if (DECOMP_INFO) std::cout << "rule 82: TERM_I ⟶ ε " << std::endl;
        return lhs;
    default:
        throw ParserError("Parser error in TERM_I");
    }
}

Token Descent::MULTIPLICATIVE_OPERATOR()
{
    switch(lookahead) {
    case tok_star:
        if (DECOMP_INFO) std::cout << "rule 83: MULTIPLICATIVE_OPERATOR ⟶ star" << std::endl;
        match(tok_star);
        return tok_star;
    case tok_div:
        if (DECOMP_INFO) std::cout << "rule 84: MULTIPLICATIVE_OPERATOR ⟶ div" << std::endl;
        match(tok_div);
        return tok_div;
    case tok_mod:
        if (DECOMP_INFO) std::cout << "rule 85: MULTIPLICATIVE_OPERATOR ⟶ mod" << std::endl;
        match(tok_mod);
        return tok_mod;
    case tok_and:
        if (DECOMP_INFO) std::cout << "rule 86: MULTIPLICATIVE_OPERATOR ⟶ and" << std::endl;
        match(tok_and);
        return tok_and;
    default:
        throw ParserError("Parser error in MULTIPLICATIVE_OPERATOR");
    }
}

std::unique_ptr<ExprASTNode> Descent::SIGNED_FACTOR()
{
    switch(lookahead) {
    case tok_plus:
    case tok_minus:
    case tok_identifier:
    case tok_lparen:
    case tok_not:
    case tok_intLiteral:
    case tok_realLiteral:
    case tok_stringLiteral: {
        if (DECOMP_INFO) std::cout << "rule 87: SIGNED_FACTOR ⟶ SIGNED_FACTOR_I FACTOR" << std::endl;

        Token tok = SIGNED_FACTOR_I();
        if (tok != tok_plus && tok != tok_minus)
            return FACTOR();
        return std::make_unique<UnaryOpASTNode>(tok, FACTOR());
    }
    default:
        throw ParserError("Parser error in SIGNED_FACTOR");
    }
}

Token Descent::SIGNED_FACTOR_I()
{
    switch(lookahead) {
    case tok_plus:
        if (DECOMP_INFO) std::cout << "rule 88: SIGNED_FACTOR_I ⟶ plus" << std::endl;
        match(tok_plus);
        return tok_plus;
    case tok_minus:
        if (DECOMP_INFO) std::cout << "rule 89: SIGNED_FACTOR_I ⟶ minus" << std::endl;
        match(tok_minus);
        return tok_minus;
    case tok_identifier:
    case tok_lparen:
    case tok_not:
    case tok_intLiteral:
    case tok_realLiteral:
    case tok_stringLiteral:
        if (DECOMP_INFO) std::cout << "rule 90: SIGNED_FACTOR_I ⟶ ε" << std::endl;
        return tok_eof; // dummy token
    default:
        throw ParserError("Parser error in SIGNED_FACTOR_I");
    }
}

std::unique_ptr<ExprASTNode> Descent::FACTOR()
{
    switch(lookahead) {
    case tok_identifier: {
        if (DECOMP_INFO) std::cout << "rule 91: FACTOR ⟶ ident FACTOR_I" << std::endl;

        std::string ident = lexer.identifierStr();
        match(tok_identifier);
        return FACTOR_I(ident);
    }
    case tok_lparen: {
        if (DECOMP_INFO) std::cout << "rule 92: FACTOR ⟶ lparen EXPRESSION rparen" << std::endl;
        match(tok_lparen);
        auto expr = EXPRESSION();
        match(tok_rparen);
        return expr;
    }
    case tok_intLiteral:
    case tok_realLiteral:
    case tok_stringLiteral:
        if (DECOMP_INFO) std::cout << "rule 93: FACTOR ⟶ UNSIGNED_CONSTANT" << std::endl;
        return UNSIGNED_CONSTANT();
    case tok_not:
        if (DECOMP_INFO) std::cout << "rule 94: FACTOR ⟶ not FACTOR" << std::endl;
        match(tok_not);
        return std::make_unique<UnaryOpASTNode>(tok_not, FACTOR());
    default:
        throw ParserError("Parser error in FACTOR");
    }
}

std::unique_ptr<ExprASTNode> Descent::FACTOR_I( std::string ident )
{
    switch(lookahead) {
    case tok_lparen: {
        if (DECOMP_INFO) std::cout << "rule 95: FACTOR_I ⟶ lparen PARAMETER_LIST rparen" << std::endl;
        match(tok_lparen);
        auto params = PARAMETER_LIST();
        match(tok_rparen);
        return std::make_unique<FunCallASTNode>(ident, std::move(params));
    }
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
    case tok_downto: // hack - not in the grammar
        if (DECOMP_INFO) std::cout << "rule 96: FACTOR_I ⟶ ε" << std::endl;
        return std::make_unique<DeclRefASTNode>(ident);
    default:
        throw ParserError("Parser error in FACTOR_I");
    }
}

std::unique_ptr<LiteralASTNode> Descent::UNSIGNED_CONSTANT()
{
    switch(lookahead) {
    case tok_intLiteral:
    case tok_realLiteral:
        if (DECOMP_INFO) std::cout << "rule 97: UNSIGNED_CONSTANT ⟶ UNSIGNED_NUMBER" << std::endl;
        return UNSIGNED_NUMBER();
    case tok_stringLiteral:
        if (DECOMP_INFO) std::cout << "rule 98: UNSIGNED_CONSTANT ⟶ string_literal" << std::endl;
        match(tok_stringLiteral);
        return std::make_unique<LiteralASTNode>(0); // TODO vracet string
    default:
        throw ParserError("Parser error in UNSIGNED_CONSTANT");
    }
}

std::vector<std::unique_ptr<ExprASTNode>> Descent::PARAMETER_LIST()
{
    switch(lookahead) {
    case tok_plus:
    case tok_minus:
    case tok_identifier:
    case tok_lparen:
    case tok_not:
    case tok_intLiteral:
    case tok_realLiteral:
    case tok_stringLiteral: {
        if (DECOMP_INFO) std::cout << "rule 99: PARAMETER_LIST ⟶ EXPRESSION PARAMETER_LIST_I" << std::endl;
        std::vector<std::unique_ptr<ExprASTNode>> params = std::vector<std::unique_ptr<ExprASTNode>>();
        params.push_back( EXPRESSION() );
        PARAMETER_LIST_I(params);
        return params;
    }
    default:
        throw ParserError("Parser error in PARAMETER_LIST");
    }
}

void Descent::PARAMETER_LIST_I( std::vector<std::unique_ptr<ExprASTNode>> &params )
{
    switch(lookahead) {
    case tok_comma:
        if (DECOMP_INFO) std::cout << "rule 100: PARAMETER_LIST_I ⟶ comma EXPRESSION PARAMETER_LIST_I" << std::endl;
        match(tok_comma);
        params.push_back( EXPRESSION() );
        PARAMETER_LIST_I(params);
        break;
    case tok_rparen:
        if (DECOMP_INFO) std::cout << "rule 101: PARAMETER_LIST_I ⟶ ε" << std::endl;
        break;
    default:
        throw ParserError("Parser error in PARAMETER_LIST_I");
    }
}
