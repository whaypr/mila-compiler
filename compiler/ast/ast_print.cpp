#include <sstream>

#include "ast.hpp"


#define MKINDENT os << mkIndent(indent);

std::ostream& operator<<(std::ostream& os, TypeASTNode::Type type)
{
    switch (type) {
    case TypeASTNode::Type::DOUBLE:
        return os << "double";
    case TypeASTNode::Type::INT:
        return os << "int";
    default:
        throw std::logic_error("Invalid type");
    }
}

std::string mkIndent(unsigned cnt)
{
    std::ostringstream oss;
    while (cnt--) {
        oss << ' ' << ' ';
    }
    return oss.str();
}

void TypeASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "Type " << m_type << std::endl;
}

void BinOpASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "BinOp " << m_op << std::endl;
    m_lhs->print(os, indent + 1);
    m_rhs->print(os, indent + 1);
}

void UnaryOpASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "UnaryOp " << m_op << std::endl;
    m_expr->print(os, indent + 1);
}

void LiteralASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "Literal " << m_value << std::endl;
}

void DeclRefASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "DeclRef " << m_var << std::endl;
}

void FunCallASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "FunCall " << m_func << std::endl;

    for (const auto& arg : m_args)
        arg->print(os, indent + 1);
}

void CompoundStmtASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "CompoundStmt" << std::endl;
    for (const auto& s : m_statements)
        s->print(os, indent + 1);
}

void VarDeclASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "VarDecl " << m_var << std::endl;
    m_type->print(os, indent + 1);
    // if (m_expr) {
    //     m_expr->print(os, indent + 1);
    // }
}

void ConstDefASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "ConstDef " << m_const << std::endl;
    m_type->print(os, indent + 1);
}

void AssignASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "Assign" << std::endl;
    m_var->print(os, indent + 1);
    m_expr->print(os, indent + 1);
}

void IfASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "If" << std::endl;
    m_cond->print(os, indent + 1);
    m_body->print(os, indent + 1);

    if (m_elseBody) {
        m_elseBody->print(os, indent + 1);
    } else {
        MKINDENT;
        os << "  <no-else>" << std::endl;
    }
}

void WhileASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "While" << std::endl;
    m_cond->print(os, indent + 1);
    m_body->print(os, indent + 1);
}

void ForASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "For" << std::endl;
    m_init->print(os, indent + 1);
    m_to->print(os, indent + 1);
    m_body->print(os, indent + 1);
}

void ExpressionStatementASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "ExprStmt" << std::endl;
    m_expr->print(os, indent + 1);
}

void ProcCallASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "ProcCall " << m_proc << std::endl;

    for (const auto& arg : m_args)
        arg->print(os, indent + 1);
}

void EmptyStmtASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "EmptyStmt " << std::endl;
}

void ProgramASTNode::print(std::ostream& os, unsigned indent) const
{
    MKINDENT;
    os << "Program" << std::endl;
    for (const auto& s : m_statements)
        s->print(os, indent + 1);
}
