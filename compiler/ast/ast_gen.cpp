#include <cstddef>
#include <llvm/ADT/APFloat.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Value.h>
#include <ostream>

#include "ast.hpp"


GenContext::GenContext(const std::string& moduleName)
    : builder(ctx)
    , module(moduleName, ctx)
{
}

void ASTNode::gen() const
{
    GenContext gen("pjp-semestral-work");
    codegen(gen);
    gen.module.print(llvm::outs(), nullptr);
}

llvm::Value* TypeASTNode::codegen(GenContext& gen) const { return nullptr; }

llvm::Type* TypeASTNode::genType(GenContext& gen) const
{
    switch (m_type) {
    case Type::DOUBLE:
        return llvm::Type::getDoubleTy(gen.ctx);
    case Type::INT:
        return llvm::Type::getInt32Ty(gen.ctx);
    default:
        assert(false);
    }
}

llvm::Value* BinOpASTNode::codegen(GenContext& gen) const
{
    auto lhs = m_lhs->codegen(gen);
    auto rhs = m_rhs->codegen(gen);

    assert(lhs);
    assert(rhs);

    bool dblArith = lhs->getType()->isDoubleTy() || rhs->getType()->isDoubleTy();
    auto maybeSIToFP = [&gen](llvm::Value* val) {
        if (!val->getType()->isDoubleTy())
            return gen.builder.CreateSIToFP(val, llvm::Type::getDoubleTy(gen.ctx));
        return val;
    };

    switch (m_op) {
    case Token::tok_equal: // originally EQ
        if (dblArith)
            return gen.builder.CreateFCmpOEQ(maybeSIToFP(lhs), maybeSIToFP(rhs), "eq");
        else
            return gen.builder.CreateICmpEQ(lhs, rhs, "eq");
    case Token::tok_notequal:
        if (dblArith)
            return gen.builder.CreateFCmpONE(maybeSIToFP(lhs), maybeSIToFP(rhs), "neq");
        else
            return gen.builder.CreateICmpNE(lhs, rhs, "neq");
    case Token::tok_lt:
        if (dblArith)
            return gen.builder.CreateFCmpOLT(maybeSIToFP(lhs), maybeSIToFP(rhs), "lt");
        else
            return gen.builder.CreateICmpSLT(lhs, rhs, "lt");
    case Token::tok_gt:
        if (dblArith)
            return gen.builder.CreateFCmpOGT(maybeSIToFP(lhs), maybeSIToFP(rhs), "gt");
        else
            return gen.builder.CreateICmpSGT(lhs, rhs, "gt");
    case Token::tok_le:
        if (dblArith)
            return gen.builder.CreateFCmpOLE(maybeSIToFP(lhs), maybeSIToFP(rhs), "le");
        else
            return gen.builder.CreateICmpSLE(lhs, rhs, "le");
    case Token::tok_ge:
        if (dblArith)
            return gen.builder.CreateFCmpOGE(maybeSIToFP(lhs), maybeSIToFP(rhs), "ge");
        else
            return gen.builder.CreateICmpSGE(lhs, rhs, "ge");
    //-----
    case Token::tok_plus:
        if (dblArith)
            return gen.builder.CreateFAdd(maybeSIToFP(lhs), maybeSIToFP(rhs), "add");
        else
            return gen.builder.CreateAdd(lhs, rhs, "add");
    case Token::tok_minus:
        if (dblArith)
            return gen.builder.CreateFSub(maybeSIToFP(lhs), maybeSIToFP(rhs), "sub");
        else
            return gen.builder.CreateSub(lhs, rhs, "sub");
    case Token::tok_star:
        if (dblArith)
            return gen.builder.CreateFMul(maybeSIToFP(lhs), maybeSIToFP(rhs), "mul");
        else
            return gen.builder.CreateMul(lhs, rhs, "mul");
    case Token::tok_div: // originally SLASH
        if (dblArith)
            return gen.builder.CreateFDiv(maybeSIToFP(lhs), maybeSIToFP(rhs), "div");
        else
            return gen.builder.CreateSDiv(lhs, rhs, "div");
    case Token::tok_mod:
        if (dblArith)
            return gen.builder.CreateFRem(maybeSIToFP(lhs), maybeSIToFP(rhs), "mod");
        else
            return gen.builder.CreateSRem(lhs, rhs, "mod");
    //-----
    case Token::tok_or:
        if (dblArith)
            throw "unsupported";
        else
            return gen.builder.CreateLogicalOr(lhs, rhs, "or");
    case Token::tok_and:
        if (dblArith)
            throw "unsupported";
        else
            return gen.builder.CreateLogicalAnd(lhs, rhs, "and");
    case Token::tok_xor:
        if (dblArith)
            throw "unsupported";
        else
            return gen.builder.CreateXor(lhs, rhs, "xor");
    default:
        throw "unimplemented";
    }
}

llvm::Value* UnaryOpASTNode::codegen(GenContext& gen) const
{
    auto expr = m_expr->codegen(gen);

    assert(expr);

    switch (m_op) {
    case Token::tok_minus:
        if (expr->getType()->isDoubleTy())
            return gen.builder.CreateFSub(
                llvm::ConstantFP::get(llvm::Type::getDoubleTy(gen.ctx), llvm::APFloat(0.0)),
                expr,
                "unminus");
        return gen.builder.CreateSub(
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(gen.ctx), 0),
            expr,
            "unminus");
    default:
        throw "unimplemented";
    }
}

llvm::Value* AssignASTNode::codegen(GenContext& gen) const
{
    auto* store = m_var->getStore(gen);
    auto expr = m_expr->codegen(gen);

    if (store->getAllocatedType()->isDoubleTy())
        expr = gen.builder.CreateSIToFP(expr, llvm::Type::getDoubleTy(gen.ctx));

    gen.builder.CreateStore(expr, store);
    return nullptr;
}

llvm::AllocaInst* AssignASTNode::getLHSStore(GenContext& gen) const
{
    return m_var->getStore(gen);
}

llvm::Value* AssignASTNode::loadLHS(GenContext& gen) const
{
    return m_var->codegen(gen);
}

llvm::Value* LiteralASTNode::codegen(GenContext& gen) const
{
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(gen.ctx), m_value);
}

llvm::Value* DeclRefASTNode::codegen(GenContext& gen) const
{
    assert(gen.symbolTable.contains(m_var));
    const auto& symbol = gen.symbolTable[m_var];

    return gen.builder.CreateLoad(symbol.type->genType(gen), symbol.store, m_var);
}

llvm::AllocaInst* DeclRefASTNode::getStore(GenContext& gen) const
{
    assert(gen.symbolTable.contains(m_var));
    return gen.symbolTable[m_var].store;
}

llvm::Value* FunCallASTNode::codegen(GenContext& gen) const
{
    auto* func = gen.module.getFunction(m_func);
    assert(func);

    std::vector<llvm::Value*> args;
    for (const auto& arg : m_args)
        args.emplace_back(arg->codegen(gen));

    return gen.builder.CreateCall(func, args);
}

llvm::Value* CompoundStmtASTNode::codegen(GenContext& gen) const
{
    auto parent = gen.builder.GetInsertBlock()->getParent();
    assert(parent);
    
    llvm::BasicBlock* BB = llvm::BasicBlock::Create(gen.ctx, "statement", parent);
    llvm::BasicBlock* BBafter = llvm::BasicBlock::Create(gen.ctx, "after", parent);

    gen.builder.CreateBr(BB);
    gen.builder.SetInsertPoint(BB);

    for (const auto& s : m_statements) {
        s->codegen(gen);
    }
    gen.builder.CreateBr(BBafter);

    gen.builder.SetInsertPoint(BBafter);

    return nullptr;
}

llvm::Value* VarDeclASTNode::codegen(GenContext& gen) const
{
    assert(!gen.symbolTable.contains(m_var));

    auto* store = gen.builder.CreateAlloca(m_type->genType(gen), 0, m_var);
    gen.symbolTable[m_var] = {m_var, m_type.get(), store};

    // if (m_expr) {
    //     auto expr = m_expr->codegen(gen);
    //     assert(expr);
    //     if (store->getAllocatedType()->isDoubleTy())
    //         expr = gen.builder.CreateSIToFP(expr, llvm::Type::getDoubleTy(gen.ctx));
    //     gen.builder.CreateStore(expr, store);
    // }

    return nullptr;
}

llvm::Value* ConstDefASTNode::codegen(GenContext& gen) const
{
    assert(!gen.symbolTable.contains(m_const));

    auto* store = gen.builder.CreateAlloca(m_type->genType(gen), 0, m_const);
    gen.symbolTable[m_const] = {m_const, m_type.get(), store};

    if (m_val) {
        auto val = m_val->codegen(gen);
        assert(val);
        if (store->getAllocatedType()->isDoubleTy())
            val = gen.builder.CreateSIToFP(val, llvm::Type::getDoubleTy(gen.ctx));
        gen.builder.CreateStore(val, store);
    }

    return nullptr;
}

llvm::Value* IfASTNode::codegen(GenContext& gen) const
{
    auto parent = gen.builder.GetInsertBlock()->getParent();
    // assert(parent);

    llvm::BasicBlock* BBbody = llvm::BasicBlock::Create(gen.ctx, "body", parent);
    llvm::BasicBlock* BBelseBody = llvm::BasicBlock::Create(gen.ctx, "elseBody", parent);
    llvm::BasicBlock* BBafter = llvm::BasicBlock::Create(gen.ctx, "after", parent);

    auto cond = m_cond->codegen(gen);
    gen.builder.CreateCondBr(cond, BBbody, BBelseBody);

    gen.builder.SetInsertPoint(BBbody);
    m_body->codegen(gen);
    gen.builder.CreateBr(BBafter);

    gen.builder.SetInsertPoint(BBelseBody);
    if (m_elseBody) {
        m_elseBody->codegen(gen);
    }
    gen.builder.CreateBr(BBafter);

    gen.builder.SetInsertPoint(BBafter);

    return nullptr;
}

llvm::Value* WhileASTNode::codegen(GenContext& gen) const
{
    auto parent = gen.builder.GetInsertBlock()->getParent();
    // assert(parent);

    llvm::BasicBlock* BBcond = llvm::BasicBlock::Create(gen.ctx, "cond", parent);
    llvm::BasicBlock* BBbody = llvm::BasicBlock::Create(gen.ctx, "body", parent);
    llvm::BasicBlock* BBafter = llvm::BasicBlock::Create(gen.ctx, "after", parent);

    gen.builder.CreateBr(BBcond);

    gen.builder.SetInsertPoint(BBcond);
    auto cond = m_cond->codegen(gen);
    gen.builder.CreateCondBr(cond, BBbody, BBafter);

    gen.builder.SetInsertPoint(BBbody);
    m_body->codegen(gen);
    gen.builder.CreateBr(BBcond);

    gen.builder.SetInsertPoint(BBafter);

    return nullptr;
}

llvm::Value* ForASTNode::codegen(GenContext& gen) const
{
    auto parent = gen.builder.GetInsertBlock()->getParent();
    assert(parent);

    llvm::BasicBlock* BBcond = llvm::BasicBlock::Create(gen.ctx, "cond", parent);
    llvm::BasicBlock* BBbody = llvm::BasicBlock::Create(gen.ctx, "body", parent);
    llvm::BasicBlock* BBafter = llvm::BasicBlock::Create(gen.ctx, "after", parent);

    m_init->codegen(gen);
    gen.builder.CreateBr(BBcond);

    gen.builder.SetInsertPoint(BBcond);
    auto to = m_to->codegen(gen);
    llvm::Value* cond;
    if (isTo)
        cond = gen.builder.CreateICmpSLE(m_init->loadLHS(gen), to, "le");
    else
        cond = gen.builder.CreateICmpSGE(m_init->loadLHS(gen), to, "ge");
    gen.builder.CreateCondBr(cond, BBbody, BBafter);

    gen.builder.SetInsertPoint(BBbody);
    m_body->codegen(gen);
    int addVal = isTo ? 1 : -1;
    auto add = gen.builder.CreateAdd(m_init->loadLHS(gen), gen.builder.getInt32(addVal));
    gen.builder.CreateStore(add, m_init->getLHSStore(gen));
    gen.builder.CreateBr(BBcond);

    gen.builder.SetInsertPoint(BBafter);

    return nullptr;
}

llvm::Value* ExpressionStatementASTNode::codegen(GenContext& gen) const
{
    return m_expr->codegen(gen);
}

llvm::Value* ProcCallASTNode::codegen(GenContext& gen) const
{
    auto* proc = gen.module.getFunction(m_proc);
    assert(proc);

    std::vector<llvm::Value*> args;
    if (m_proc != "readln" && m_proc != "dec") {
        for (const auto& arg : m_args)
            args.emplace_back(arg->codegen(gen));
    } else {
        auto store = static_cast<DeclRefASTNode*>(m_args[0].get())->getStore(gen);
        args.emplace_back(store);
    }

    return gen.builder.CreateCall(proc, args);
}

llvm::Value* EmptyStmtASTNode::codegen(GenContext& gen) const
{
    return nullptr;
}

llvm::Value* ProgramASTNode::codegen(GenContext& gen) const
{
    llvm::FunctionType* ftMain = llvm::FunctionType::get(llvm::Type::getInt32Ty(gen.ctx), false);
    llvm::Function* fMain = llvm::Function::Create(ftMain, llvm::Function::ExternalLinkage, "main", gen.module);

    // create write function
    {
        llvm::FunctionType* ft = llvm::FunctionType::get(
            llvm::Type::getVoidTy(gen.ctx),
            {llvm::Type::getInt32Ty(gen.ctx)},
            false);
        llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "write", gen.module);
    }
    // create writeln function
    {
        llvm::FunctionType* ft = llvm::FunctionType::get(
            llvm::Type::getVoidTy(gen.ctx),
            {llvm::Type::getInt32Ty(gen.ctx)},
            false);
        llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "writeln", gen.module);
    }
    // create readln function
    {
        llvm::FunctionType* ft = llvm::FunctionType::get(
            llvm::Type::getVoidTy(gen.ctx),
            {llvm::Type::getInt32PtrTy(gen.ctx)},
            false);
        llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "readln", gen.module);
    }
    // create dec function
    {
        llvm::FunctionType* ft = llvm::FunctionType::get(
            llvm::Type::getVoidTy(gen.ctx),
            {llvm::Type::getInt32PtrTy(gen.ctx)},
            false);
        llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "dec", gen.module);
    }

    llvm::BasicBlock* BB = llvm::BasicBlock::Create(gen.ctx, "entry", fMain);
    gen.builder.SetInsertPoint(BB);

    for (const auto& s : m_statements) {
        s->codegen(gen);
    }

    gen.builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(gen.ctx), 42));

    return nullptr;
}
