#pragma once

#include <string>
#include <vector>
#include "../lexer/lexer.hpp"
#include "../ast/ast.hpp"

#include <memory>


class Descent {
public:
    Descent (Lexer &l, Token &t, bool DECOMP_INFO): lexer(l), lookahead(t), DECOMP_INFO(DECOMP_INFO) {}

    Token match(Token tok);

    std::unique_ptr<ProgramASTNode> PROGRAM();
    std::unique_ptr<BlockASTNode> BLOCK();
    void BLOCK_I( std::vector<std::unique_ptr<StatementASTNode>> &statements );
    void CONSTANT_DEFINITION( std::vector<std::unique_ptr<StatementASTNode>> &statements );
    void CONSTANT_DEFINITION_I( std::vector<std::unique_ptr<StatementASTNode>> &statements );
    std::unique_ptr<LiteralASTNode>CONSTANT();
    std::unique_ptr<LiteralASTNode> CONSTANT_L();
    std::unique_ptr<LiteralASTNode> UNSIGNED_NUMBER();
    void VARIABLE_DECLARATION_PART( std::vector<std::unique_ptr<StatementASTNode>> &statements );
    void VARIABLE_DECLARATION_PART_I_semi( std::vector<std::unique_ptr<StatementASTNode>> &statements );
    void VARIABLE_DECLARATION_PART_I_semi_L( std::vector<std::unique_ptr<StatementASTNode>> &statements );
    void VARIABLE_DECLARATION( std::vector<std::unique_ptr<StatementASTNode>> &statements );
    std::vector<std::string> IDENTIFIER_LIST();
    void IDENTIFIER_LIST_I( std::vector<std::string> &identificators );
    TypeASTNode::Type TYPE_IDENTIFIER();
    void PROCEDURE_AND_FUNCTION_DECLARATION_PART( std::vector<std::unique_ptr<StatementASTNode>> &statements );
    void PROCEDURE_OR_FUNCTION_DECLARATION( std::vector<std::unique_ptr<StatementASTNode>> &statements );
    void PROCEDURE_DECLARATION( std::vector<std::unique_ptr<StatementASTNode>> &statements );
    std::vector<std::unique_ptr<VarDeclASTNode>> FORMAL_PARAMETER_LIST_OPT();
    std::vector<std::unique_ptr<VarDeclASTNode>> FORMAL_PARAMETER_LIST();
    void FORMAL_PARAMETER_LIST_I( std::vector<std::unique_ptr<VarDeclASTNode>> &parameters );
    void PARAMETER_GROUP( std::vector<std::unique_ptr<VarDeclASTNode>> &parameters );
    std::unique_ptr<BlockASTNode> FORWARD_OR_BLOCK();
    void FUNCTION_DECLARATION( std::vector<std::unique_ptr<StatementASTNode>> &statements );
    std::unique_ptr<StatementASTNode> STATEMENT();
    std::unique_ptr<StatementASTNode> SIMPLE_STATEMENT();
    std::unique_ptr<StatementASTNode> SIMPLE_STATEMENT_L( std::string ident );
    std::vector<std::unique_ptr<ExprASTNode>> PROCEDURE_STATEMENT_I();
    std::unique_ptr<StatementASTNode> STRUCTURED_STATEMENT();
    std::unique_ptr<CompoundStmtASTNode> COMPOUND_STATEMENT();
    void COMPOUND_STATEMENT_I( std::vector<std::unique_ptr<StatementASTNode>> &statements );
    std::unique_ptr<StatementASTNode> IF_STATEMENT();
    std::unique_ptr<StatementASTNode> IF_STATEMENT_I();
    std::unique_ptr<StatementASTNode> REPETETIVE_STATEMENT();
    std::unique_ptr<WhileASTNode> WHILE_STATEMENT();
    std::unique_ptr<ForASTNode> FOR_STATEMENT();
    std::unique_ptr<StatementASTNode> EMPTY_STATEMENT();
    std::unique_ptr<ExprASTNode> EXPRESSION();
    std::unique_ptr<ExprASTNode> EXPRESSION_I( std::unique_ptr<ExprASTNode> lhs );
    Token RELATIONAL_OPERATOR();
    std::unique_ptr<ExprASTNode> SIMPLE_EXPRESSION();
    std::unique_ptr<ExprASTNode> SIMPLE_EXPRESSION_I( std::unique_ptr<ExprASTNode> lhs );
    Token ADDITIVE_OPERATOR();
    std::unique_ptr<ExprASTNode> TERM();
    std::unique_ptr<ExprASTNode> TERM_I( std::unique_ptr<ExprASTNode> lhs );
    Token MULTIPLICATIVE_OPERATOR();
    std::unique_ptr<ExprASTNode> SIGNED_FACTOR();
    Token SIGNED_FACTOR_I();
    std::unique_ptr<ExprASTNode> FACTOR();
    std::unique_ptr<ExprASTNode> FACTOR_I( std::string ident );
    std::unique_ptr<LiteralASTNode> UNSIGNED_CONSTANT();
    std::vector<std::unique_ptr<ExprASTNode>> PARAMETER_LIST();
    void PARAMETER_LIST_I( std::vector<std::unique_ptr<ExprASTNode>> &params );

private:
    Lexer &lexer;
    Token &lookahead;
    bool DECOMP_INFO;
    
    std::string ParserError( std::string msg="PARSE ERROR!" );
};
