#pragma once

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include <memory>

#include "../lexer/lexer.hpp"
#include "descent.hpp"


class Parser {
public:
    Parser(Lexer &l, bool DECOMP_INFO)
        : lexer(l)
        , descent(lexer, lookahead, DECOMP_INFO) {}
    
    ~Parser() = default;

    std::unique_ptr<ProgramASTNode> parse();

private:
    int getNextToken();

    Lexer lexer;                     // lexer is used to read tokens
    Token lookahead;                 // to keep the current token
    Descent descent;
};
