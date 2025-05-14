#include "tribhasha/CodeGen.h"
#include <iostream>
#include <llvm/IR/Constants.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

namespace tribhasha {

CodeGen::CodeGen() : builder(context) {
    initialize();
}

void CodeGen::initialize() {
    module = std::make_unique<llvm::Module>("tribhasha", context);
    
    // Create a printf function prototype (for print statements later)
    std::vector<llvm::Type*> printfArgs;
    printfArgs.push_back(llvm::Type::getInt8PtrTy(context));
    
    llvm::FunctionType* printfType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(context),
        printfArgs,
        true // varargs
    );
    
    llvm::Function::Create(
        printfType,
        llvm::Function::ExternalLinkage,
        "printf",
        module.get()
    );
}

std::unique_ptr<llvm::Module> CodeGen::getModule() {
    return std::move(module);
}

void CodeGen::generate(const std::vector<std::shared_ptr<Stmt>>& statements) {
    // Create a main function for the program
    llvm::FunctionType* mainType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(context),
        false
    );
    
    llvm::Function* main = llvm::Function::Create(
        mainType,
        llvm::Function::ExternalLinkage,
        "main",
        module.get()
    );
    
    // Create a basic block to start insertion into
    llvm::BasicBlock* block = llvm::BasicBlock::Create(context, "entry", main);
    builder.SetInsertPoint(block);
    
    // Set current function
    currentFunction = main;
    
    // Generate code for each statement
    for (const auto& stmt : statements) {
        stmt->accept(this);
    }
    
    // Return 0 from main
    builder.CreateRet(llvm::ConstantInt::get(context, llvm::APInt(32, 0)));
    
    // Verify the function
    llvm::verifyFunction(*main);
}

// Helper methods
llvm::AllocaInst* CodeGen::createEntryBlockAlloca(llvm::Function* function, const std::string& varName) {
    llvm::IRBuilder<> tempBuilder(&function->getEntryBlock(), function->getEntryBlock().begin());
    return tempBuilder.CreateAlloca(llvm::Type::getDoubleTy(context), 0, varName.c_str());
}

llvm::Value* CodeGen::logErrorV(const std::string& str) {
    std::cerr << "Error: " << str << std::endl;
    return nullptr;
}

llvm::Function* CodeGen::getFunction(const std::string& name) {
    // First, check if the function is in our module
    if (auto* F = module->getFunction(name)) {
        return F;
    }
    
    // If not, check if we can codegen the declaration now
    auto it = functions.find(name);
    if (it != functions.end()) {
        return it->second;
    }
    
    return nullptr;
}

// Expression visitors
void* CodeGen::visitBinaryExpr(BinaryExpr* expr) {
    llvm::Value* left = static_cast<llvm::Value*>(expr->left->accept(this));
    llvm::Value* right = static_cast<llvm::Value*>(expr->right->accept(this));
    
    if (!left || !right) {
        return logErrorV("Invalid binary operands");
    }
    
    switch (expr->op.type) {
        case TokenType::PLUS:
            return builder.CreateFAdd(left, right, "addtmp");
        case TokenType::MINUS:
            return builder.CreateFSub(left, right, "subtmp");
        case TokenType::STAR:
            return builder.CreateFMul(left, right, "multmp");
        case TokenType::SLASH:
            return builder.CreateFDiv(left, right, "divtmp");
        case TokenType::MODULO:
            return builder.CreateFRem(left, right, "modtmp");
        case TokenType::LESS:
            return builder.CreateFCmpULT(left, right, "cmptmp");
        case TokenType::LESS_EQUAL:
            return builder.CreateFCmpULE(left, right, "cmptmp");
        case TokenType::GREATER:
            return builder.CreateFCmpUGT(left, right, "cmptmp");
        case TokenType::GREATER_EQUAL:
            return builder.CreateFCmpUGE(left, right, "cmptmp");
        case TokenType::EQUAL:
            return builder.CreateFCmpUEQ(left, right, "cmptmp");
        case TokenType::NOT_EQUAL:
            return builder.CreateFCmpUNE(left, right, "cmptmp");
        default:
            return logErrorV("Unknown binary operator");
    }
}

void* CodeGen::visitGroupingExpr(GroupingExpr* expr) {
    // Simply visit the contained expression
    return expr->expression->accept(this);
}

void* CodeGen::visitLiteralExpr(LiteralExpr* expr) {
    switch (expr->type) {
        case TokenType::INT_LITERAL:
            return llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), std::stoi(expr->value));
        case TokenType::FLOAT_LITERAL:
            return llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), std::stod(expr->value));
        case TokenType::STRING_LITERAL: {
            // Create a global string constant
            llvm::Constant* stringConstant = llvm::ConstantDataArray::getString(context, expr->value);
            llvm::GlobalVariable* globalStr = new llvm::GlobalVariable(
                *module,
                stringConstant->getType(),
                true,
                llvm::GlobalValue::PrivateLinkage,
                stringConstant,
                ".str"
            );
            
            // Get pointer to the first character
            llvm::Constant* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
            llvm::Constant* indices[] = {zero, zero};
            return llvm::ConstantExpr::getGetElementPtr(
                globalStr->getValueType(),
                globalStr,
                indices
            );
        }
        case TokenType::TRUE_EN:
        case TokenType::TRUE_HI:
        case TokenType::TRUE_AS:
            return llvm::ConstantInt::getTrue(context);
        case TokenType::FALSE_EN:
        case TokenType::FALSE_HI:
        case TokenType::FALSE_AS:
            return llvm::ConstantInt::getFalse(context);
        default:
            return logErrorV("Unknown literal type");
    }
}

void* CodeGen::visitUnaryExpr(UnaryExpr* expr) {
    llvm::Value* operand = static_cast<llvm::Value*>(expr->right->accept(this));
    if (!operand) {
        return logErrorV("Invalid unary operand");
    }
    
    switch (expr->op.type) {
        case TokenType::MINUS:
            return builder.CreateFNeg(operand, "negtmp");
        case TokenType::NOT_EN:
        case TokenType::NOT_HI:
        case TokenType::NOT_AS:
            return builder.CreateNot(operand, "nottmp");
        default:
            return logErrorV("Unknown unary operator");
    }
}

void* CodeGen::visitVariableExpr(VariableExpr* expr) {
    // Look up the variable in the symbol table
    llvm::AllocaInst* alloca = namedValues[expr->name.lexeme];
    if (!alloca) {
        return logErrorV("Unknown variable name: " + expr->name.lexeme);
    }
    
    // Load the value
    return builder.CreateLoad(llvm::Type::getDoubleTy(context), alloca, expr->name.lexeme.c_str());
}

void* CodeGen::visitAssignExpr(AssignExpr* expr) {
    // Generate code for the right-hand side expression
    llvm::Value* value = static_cast<llvm::Value*>(expr->value->accept(this));
    if (!value) {
        return nullptr;
    }
    
    // Look up the variable in the symbol table
    llvm::AllocaInst* alloca = namedValues[expr->name.lexeme];
    if (!alloca) {
        return logErrorV("Unknown variable name: " + expr->name.lexeme);
    }
    
    // Store the value
    builder.CreateStore(value, alloca);
    return value;
}

void* CodeGen::visitCallExpr(CallExpr* expr) {
    // Get the function to call
    llvm::Function* callee = getFunction(
        static_cast<VariableExpr*>(expr->callee.get())->name.lexeme
    );
    
    if (!callee) {
        return logErrorV("Unknown function referenced");
    }
    
    // Check arity
    if (callee->arg_size() != expr->arguments.size()) {
        return logErrorV("Incorrect number of arguments passed");
    }
    
    // Generate code for arguments
    std::vector<llvm::Value*> argsV;
    for (const auto& arg : expr->arguments) {
        llvm::Value* argValue = static_cast<llvm::Value*>(arg->accept(this));
        if (!argValue) {
            return nullptr;
        }
        argsV.push_back(argValue);
    }
    
    // Create the call
    return builder.CreateCall(callee, argsV, "calltmp");
}

// Statement visitors
void* CodeGen::visitExpressionStmt(ExpressionStmt* stmt) {
    // Generate code for the expression
    return stmt->expression->accept(this);
}

void* CodeGen::visitVarStmt(VarStmt* stmt) {
    llvm::Value* initValue = nullptr;
    
    // Generate code for the initializer if it exists
    if (stmt->initializer) {
        initValue = static_cast<llvm::Value*>(stmt->initializer->accept(this));
        if (!initValue) {
            return nullptr;
        }
    } else {
        // Default initialization to 0
        initValue = llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), 0.0);
    }
    
    // Create a variable allocation in the current function
    llvm::AllocaInst* alloca = createEntryBlockAlloca(currentFunction, stmt->name.lexeme);
    
    // Store the initial value
    builder.CreateStore(initValue, alloca);
    
    // Add to symbol table
    namedValues[stmt->name.lexeme] = alloca;
    
    return nullptr;
}

void* CodeGen::visitBlockStmt(BlockStmt* stmt) {
    // Save the current symbol table
    std::unordered_map<std::string, llvm::AllocaInst*> oldNamedValues = namedValues;
    
    // Generate code for each statement in the block
    for (const auto& statement : stmt->statements) {
        statement->accept(this);
    }
    
    // Restore the original symbol table
    namedValues = oldNamedValues;
    
    return nullptr;
}

void* CodeGen::visitIfStmt(IfStmt* stmt) {
    // Generate condition
    llvm::Value* condV = static_cast<llvm::Value*>(stmt->condition->accept(this));
    if (!condV) {
        return nullptr;
    }
    
    // Convert condition to boolean (non-zero is true)
    condV = builder.CreateFCmpONE(
        condV,
        llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), 0.0),
        "ifcond"
    );
    
    // Create blocks for then, else, and merge
    llvm::Function* function = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(context, "then", function);
    llvm::BasicBlock* elseBB = nullptr;
    llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(context, "ifcont");
    
    if (stmt->elseBranch) {
        elseBB = llvm::BasicBlock::Create(context, "else");
        builder.CreateCondBr(condV, thenBB, elseBB);
    } else {
        builder.CreateCondBr(condV, thenBB, mergeBB);
    }
    
    // Generate then block
    builder.SetInsertPoint(thenBB);
    stmt->thenBranch->accept(this);
    builder.CreateBr(mergeBB);
    
    // Generate else block if it exists
    if (stmt->elseBranch) {
        function->getBasicBlockList().push_back(elseBB);
        builder.SetInsertPoint(elseBB);
        stmt->elseBranch->accept(this);
        builder.CreateBr(mergeBB);
    }
    
    // Generate merge block
    function->getBasicBlockList().push_back(mergeBB);
    builder.SetInsertPoint(mergeBB);
    
    return nullptr;
}

void* CodeGen::visitWhileStmt(WhileStmt* stmt) {
    // Create blocks for loop condition, body, and after
    llvm::Function* function = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* condBB = llvm::BasicBlock::Create(context, "loopcond", function);
    llvm::BasicBlock* bodyBB = llvm::BasicBlock::Create(context, "loopbody");
    llvm::BasicBlock* afterBB = llvm::BasicBlock::Create(context, "afterloop");
    
    // Branch to condition block
    builder.CreateBr(condBB);
    
    // Generate condition block
    builder.SetInsertPoint(condBB);
    llvm::Value* condV = static_cast<llvm::Value*>(stmt->condition->accept(this));
    if (!condV) {
        return nullptr;
    }
    
    // Convert condition to boolean
    condV = builder.CreateFCmpONE(
        condV,
        llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), 0.0),
        "loopcond"
    );
    
    builder.CreateCondBr(condV, bodyBB, afterBB);
    
    // Generate body block
    function->getBasicBlockList().push_back(bodyBB);
    builder.SetInsertPoint(bodyBB);
    stmt->body->accept(this);
    builder.CreateBr(condBB);
    
    // Generate after block
    function->getBasicBlockList().push_back(afterBB);
    builder.SetInsertPoint(afterBB);
    
    return nullptr;
}

void* CodeGen::visitFunctionStmt(FunctionStmt* stmt) {
    // Create a function type
    std::vector<llvm::Type*> argTypes(stmt->params.size(), llvm::Type::getDoubleTy(context));
    llvm::FunctionType* functionType = llvm::FunctionType::get(
        llvm::Type::getDoubleTy(context),
        argTypes,
        false
    );
    
    // Create the function
    llvm::Function* function = llvm::Function::Create(
        functionType,
        llvm::Function::ExternalLinkage,
        stmt->name.lexeme,
        module.get()
    );
    
    // Set names for all arguments
    unsigned i = 0;
    for (auto& arg : function->args()) {
        arg.setName(stmt->params[i++].lexeme);
    }
    
    // Create a new basic block to start insertion into
    llvm::BasicBlock* block = llvm::BasicBlock::Create(context, "entry", function);
    builder.SetInsertPoint(block);
    
    // Save the current function
    llvm::Function* oldFunction = currentFunction;
    currentFunction = function;
    
    // Save the current named values
    std::unordered_map<std::string, llvm::AllocaInst*> oldNamedValues = namedValues;
    namedValues.clear();
    
    // Create allocas for arguments
    for (auto& arg : function->args()) {
        llvm::AllocaInst* alloca = createEntryBlockAlloca(function, arg.getName().str());
        builder.CreateStore(&arg, alloca);
        namedValues[arg.getName().str()] = alloca;
    }
    
    // Generate code for function body
    for (const auto& statement : stmt->body) {
        statement->accept(this);
    }
    
    // Return a default value if control flow reaches the end of the function
    if (!block->getTerminator()) {
        builder.CreateRet(llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), 0.0));
    }
    
    // Verify the function
    llvm::verifyFunction(*function);
    
    // Add function to symbol table
    functions[stmt->name.lexeme] = function;
    
    // Restore the old function and named values
    currentFunction = oldFunction;
    namedValues = oldNamedValues;
    
    return nullptr;
}

void* CodeGen::visitReturnStmt(ReturnStmt* stmt) {
    llvm::Value* returnValue = nullptr;
    
    if (stmt->value) {
        returnValue = static_cast<llvm::Value*>(stmt->value->accept(this));
    } else {
        returnValue = llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), 0.0);
    }
    
    builder.CreateRet(returnValue);
    
    return nullptr;
}

} // namespace tribhasha