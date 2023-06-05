#pragma once
#include <cstdint>
#include <map>
#include <memory>
#include <ostream>
#include <vector>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include "../lexer/lexer.hpp"


class TypeASTNode;

struct Symbol {
    std::string name;
    TypeASTNode* type;
    llvm::AllocaInst* store;
};

struct GenContext {
    GenContext(const std::string& moduleName);

    llvm::LLVMContext ctx;
    llvm::IRBuilder<> builder;
    llvm::Module module;

    std::map<std::string, Symbol> symbolTable;
};


//-----------------------------------------------------------------------
class ASTNode {
public:
    virtual ~ASTNode();
    virtual void print(std::ostream& os, unsigned indent = 0) const = 0;
    void gen() const;
    virtual llvm::Value* codegen(GenContext& gen) const = 0;
};

class TypeASTNode : public ASTNode {
public:
    enum class Type { INT,
                      DOUBLE,
    };

    TypeASTNode(Type type);
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
    llvm::Type* genType(GenContext& gen) const;

private:
    Type m_type;
};


//-----------------------------------------------------------------------
class ExprASTNode : public ASTNode {
public:
    virtual ~ExprASTNode();
};

// Plus / Minus / Multiply / Divide / And / Or / Xor
class BinOpASTNode : public ExprASTNode {
    Token m_op;
    std::unique_ptr<ExprASTNode> m_lhs;
    std::unique_ptr<ExprASTNode> m_rhs;

public:
    BinOpASTNode(Token op, std::unique_ptr<ExprASTNode> lhs, std::unique_ptr<ExprASTNode> rhs);
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};

// Minus / Not
class UnaryOpASTNode : public ExprASTNode {
    Token m_op;
    std::unique_ptr<ExprASTNode> m_expr;

public:
    UnaryOpASTNode(Token op, std::unique_ptr<ExprASTNode> expr);
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};

// literals - TODO add literals of more types
class LiteralASTNode : public ExprASTNode {
    int64_t m_value;

public:
    LiteralASTNode(int64_t value);
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value *codegen(GenContext &gen) const override;
    int64_t getVal() { return m_value; }
};

// declaration reference (variable/constant)
class DeclRefASTNode : public ExprASTNode {
    std::string m_var;

public:
    DeclRefASTNode(std::string var);
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
    llvm::AllocaInst* getStore(GenContext& gen) const;
};

// function call
class FunCallASTNode : public ExprASTNode {
    std::string m_func;
    std::vector<std::unique_ptr<ExprASTNode>> m_args;

public:
    FunCallASTNode(std::string func, std::vector<std::unique_ptr<ExprASTNode>> args);
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};


//-----------------------------------------------------------------------
class StatementASTNode : public ASTNode {
public:
    virtual ~StatementASTNode();
};

// compound statement
class CompoundStmtASTNode : public StatementASTNode {
    std::vector<std::unique_ptr<StatementASTNode>> m_statements;

public:
    CompoundStmtASTNode(std::vector<std::unique_ptr<StatementASTNode>> statements);
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};

// variable declaration
class VarDeclASTNode : public StatementASTNode {
    std::string m_var;
    std::unique_ptr<TypeASTNode> m_type;
    // std::unique_ptr<ExprASTNode> m_expr;

public:
    VarDeclASTNode(std::string var, std::unique_ptr<TypeASTNode> type);
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};

// constant definition
class ConstDefASTNode : public StatementASTNode {
    std::string m_const;
    std::unique_ptr<LiteralASTNode> m_val;
    std::unique_ptr<TypeASTNode> m_type = std::make_unique<TypeASTNode>(TypeASTNode::Type::INT); // zatím umí pouze int (TODO)

public:
    ConstDefASTNode(std::string _const, std::unique_ptr<LiteralASTNode> val);
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};

// assign
class AssignASTNode : public StatementASTNode {
    std::unique_ptr<DeclRefASTNode> m_var;
    std::unique_ptr<ExprASTNode> m_expr;

public:
    AssignASTNode(std::unique_ptr<DeclRefASTNode> var, std::unique_ptr<ExprASTNode> expr);
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value *codegen(GenContext &gen) const override;
    llvm::AllocaInst* getLHSStore(GenContext& gen) const;
    llvm::Value* loadLHS(GenContext& gen) const;
};

// if
class IfASTNode : public StatementASTNode {
    std::unique_ptr<ExprASTNode> m_cond;
    std::unique_ptr<StatementASTNode> m_body;
    std::unique_ptr<StatementASTNode> m_elseBody;

public:
    IfASTNode(std::unique_ptr<ExprASTNode> cond, std::unique_ptr<StatementASTNode> body, std::unique_ptr<StatementASTNode> elseBody);
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};

// while
class WhileASTNode : public StatementASTNode {
    std::unique_ptr<ExprASTNode> m_cond;
    std::unique_ptr<StatementASTNode> m_body;

public:
    WhileASTNode(std::unique_ptr<ExprASTNode> cond, std::unique_ptr<StatementASTNode> body);
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};

// for
class ForASTNode : public StatementASTNode {
    std::unique_ptr<AssignASTNode> m_init;
    std::unique_ptr<ExprASTNode> m_to;
    std::unique_ptr<StatementASTNode> m_body;
    bool isTo;

public:
    ForASTNode(std::unique_ptr<AssignASTNode> init, std::unique_ptr<ExprASTNode> to, std::unique_ptr<StatementASTNode> body, bool isTo);
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};

// expression statement
class ExpressionStatementASTNode : public StatementASTNode {
    std::unique_ptr<ExprASTNode> m_expr;

public:
    ExpressionStatementASTNode(std::unique_ptr<ExprASTNode> expr);
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};

// procedure call
class ProcCallASTNode : public StatementASTNode {
    std::string m_proc;
    std::vector<std::unique_ptr<ExprASTNode>> m_args;

public:
    ProcCallASTNode(std::string proc, std::vector<std::unique_ptr<ExprASTNode>> args);
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};

// empty statement
class EmptyStmtASTNode : public StatementASTNode {
  
public:
    EmptyStmtASTNode();
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};


//-----------------------------------------------------------------------
class ProgramASTNode : public ASTNode {
    std::vector<std::unique_ptr<StatementASTNode>> m_statements;

public:
    ProgramASTNode(std::vector<std::unique_ptr<StatementASTNode>> statements);
    void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};
