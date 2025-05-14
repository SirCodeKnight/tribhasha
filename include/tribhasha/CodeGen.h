#ifndef TRIBHASHA_CODEGEN_H
#define TRIBHASHA_CODEGEN_H

#include "AST.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <string>
#include <memory>
#include <unordered_map>

namespace tribhasha {

class CodeGen : public ExprVisitor, public StmtVisitor {
private:
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;
    
    // Symbol table for variables
    std::unordered_map<std::string, llvm::AllocaInst*> namedValues;
    
    // Symbol table for functions
    std::unordered_map<std::string, llvm::Function*> functions;
    
    // Current function being code generated
    llvm::Function* currentFunction = nullptr;
    
    // Helper methods
    llvm::AllocaInst* createEntryBlockAlloca(llvm::Function* function, const std::string& varName);
    llvm::Value* logErrorV(const std::string& str);
    llvm::Function* getFunction(const std::string& name);
    
public:
    CodeGen();
    
    // Initialize a fresh module
    void initialize();
    
    // Get the generated module
    std::unique_ptr<llvm::Module> getModule();
    
    // Generate code for a list of statements (the program)
    void generate(const std::vector<std::shared_ptr<Stmt>>& statements);
    
    // Visitor implementations for expressions
    void* visitBinaryExpr(BinaryExpr* expr) override;
    void* visitGroupingExpr(GroupingExpr* expr) override;
    void* visitLiteralExpr(LiteralExpr* expr) override;
    void* visitUnaryExpr(UnaryExpr* expr) override;
    void* visitVariableExpr(VariableExpr* expr) override;
    void* visitAssignExpr(AssignExpr* expr) override;
    void* visitCallExpr(CallExpr* expr) override;
    
    // Visitor implementations for statements
    void* visitExpressionStmt(ExpressionStmt* stmt) override;
    void* visitVarStmt(VarStmt* stmt) override;
    void* visitBlockStmt(BlockStmt* stmt) override;
    void* visitIfStmt(IfStmt* stmt) override;
    void* visitWhileStmt(WhileStmt* stmt) override;
    void* visitFunctionStmt(FunctionStmt* stmt) override;
    void* visitReturnStmt(ReturnStmt* stmt) override;
};

} // namespace tribhasha

#endif // TRIBHASHA_CODEGEN_H