#include "ast.hpp"


ASTNode::~ASTNode() = default;

TypeASTNode::TypeASTNode(Type type)
    : m_type(type)
{
}

ExprASTNode::~ExprASTNode() = default;

BinOpASTNode::BinOpASTNode(Token op, std::unique_ptr<ExprASTNode> lhs, std::unique_ptr<ExprASTNode> rhs)
    : m_op(op)
    , m_lhs(std::move(lhs))
    , m_rhs(std::move(rhs))
{
}

UnaryOpASTNode::UnaryOpASTNode(Token op, std::unique_ptr<ExprASTNode> expr)
    : m_op(op)
    , m_expr(std::move(expr))
{
}

LiteralASTNode::LiteralASTNode(int64_t value)
    : m_value(value)
{
}

DeclRefASTNode::DeclRefASTNode(std::string var)
    : m_var(std::move(var))
{
}

FunCallASTNode::FunCallASTNode(std::string func, std::vector<std::unique_ptr<ExprASTNode>> args)
    : m_func(std::move(func))
    , m_args(std::move(args))
{
}

StatementASTNode::~StatementASTNode() = default;

BlockASTNode::BlockASTNode(std::vector<std::unique_ptr<StatementASTNode>> statements)
    : m_statements(std::move(statements))
{
}

CompoundStmtASTNode::CompoundStmtASTNode(std::vector<std::unique_ptr<StatementASTNode>> statements)
    : m_statements(std::move(statements))
{
}

VarDeclASTNode::VarDeclASTNode(std::string var, std::unique_ptr<TypeASTNode> type)
    : m_var(std::move(var))
    , m_type(std::move(type))
{
}

ConstDefASTNode::ConstDefASTNode(std::string _const, std::unique_ptr<LiteralASTNode> val)
    : m_const(std::move(_const))
    , m_val(std::move(val))
{
}

ProcDeclASTNode::ProcDeclASTNode(std::string name, std::vector<std::unique_ptr<VarDeclASTNode>> params, std::unique_ptr<BlockASTNode> block)
    : m_name(std::move(name))
    , m_params(std::move(params))
    , m_block(std::move(block))
{
}

FunDeclASTNode::FunDeclASTNode(std::string name, std::vector<std::unique_ptr<VarDeclASTNode>> params, std::unique_ptr<BlockASTNode> block, std::unique_ptr<TypeASTNode> retType)
    : m_name(std::move(name))
    , m_params(std::move(params))
    , m_block(std::move(block))
    , m_retType(std::move(retType))
{
}

AssignASTNode::AssignASTNode(std::unique_ptr<DeclRefASTNode> var, std::unique_ptr<ExprASTNode> expr)
    : m_var(std::move(var))
    , m_expr(std::move(expr))
{
}

IfASTNode::IfASTNode(std::unique_ptr<ExprASTNode> cond, std::unique_ptr<StatementASTNode> body, std::unique_ptr<StatementASTNode> elseBody)
    : m_cond(std::move(cond))
    , m_body(std::move(body))
    , m_elseBody(std::move(elseBody))
{
}

WhileASTNode::WhileASTNode(std::unique_ptr<ExprASTNode> cond, std::unique_ptr<StatementASTNode> body)
    : m_cond(std::move(cond))
    , m_body(std::move(body))
{
}

ForASTNode::ForASTNode(std::unique_ptr<AssignASTNode> init, std::unique_ptr<ExprASTNode> to, std::unique_ptr<StatementASTNode> body, bool isTo)
    : m_init(std::move(init))
    , m_to(std::move(to))
    , m_body(std::move(body))
    , isTo(isTo)
{
}

ExpressionStatementASTNode::ExpressionStatementASTNode(std::unique_ptr<ExprASTNode> expr)
    : m_expr(std::move(expr))
{
}

ProcCallASTNode::ProcCallASTNode(std::string proc, std::vector<std::unique_ptr<ExprASTNode>> args)
    : m_proc(std::move(proc))
    , m_args(std::move(args))
{
}

EmptyStmtASTNode::EmptyStmtASTNode()
{
}

ProgramASTNode::ProgramASTNode(std::unique_ptr<BlockASTNode> block)
    : m_block(std::move(block))
{
}
