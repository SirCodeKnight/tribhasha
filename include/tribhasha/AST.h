#ifndef TRIBHASHA_AST_H
#define TRIBHASHA_AST_H

#include "Token.h"
#include <memory>
#include <vector>
#include <string>
#include <utility>

namespace tribhasha {

// Forward declarations
class Expr;
class Stmt;

// Visitor pattern interfaces
class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    
    virtual void* visitBinaryExpr(class BinaryExpr* expr) = 0;
    virtual void* visitGroupingExpr(class GroupingExpr* expr) = 0;
    virtual void* visitLiteralExpr(class LiteralExpr* expr) = 0;
    virtual void* visitUnaryExpr(class UnaryExpr* expr) = 0;
    virtual void* visitVariableExpr(class VariableExpr* expr) = 0;
    virtual void* visitAssignExpr(class AssignExpr* expr) = 0;
    virtual void* visitCallExpr(class CallExpr* expr) = 0;
};

class StmtVisitor {
public:
    virtual ~StmtVisitor() = default;
    
    virtual void* visitExpressionStmt(class ExpressionStmt* stmt) = 0;
    virtual void* visitVarStmt(class VarStmt* stmt) = 0;
    virtual void* visitBlockStmt(class BlockStmt* stmt) = 0;
    virtual void* visitIfStmt(class IfStmt* stmt) = 0;
    virtual void* visitWhileStmt(class WhileStmt* stmt) = 0;
    virtual void* visitFunctionStmt(class FunctionStmt* stmt) = 0;
    virtual void* visitReturnStmt(class ReturnStmt* stmt) = 0;
};

// Base classes
class Expr {
public:
    virtual ~Expr() = default;
    virtual void* accept(ExprVisitor* visitor) = 0;
};

class Stmt {
public:
    virtual ~Stmt() = default;
    virtual void* accept(StmtVisitor* visitor) = 0;
};

// Expression classes
class BinaryExpr : public Expr {
public:
    BinaryExpr(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}
    
    void* accept(ExprVisitor* visitor) override {
        return visitor->visitBinaryExpr(this);
    }
    
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;
};

class GroupingExpr : public Expr {
public:
    explicit GroupingExpr(std::shared_ptr<Expr> expression)
        : expression(std::move(expression)) {}
    
    void* accept(ExprVisitor* visitor) override {
        return visitor->visitGroupingExpr(this);
    }
    
    std::shared_ptr<Expr> expression;
};

class LiteralExpr : public Expr {
public:
    explicit LiteralExpr(std::string value, TokenType type)
        : value(std::move(value)), type(type) {}
    
    void* accept(ExprVisitor* visitor) override {
        return visitor->visitLiteralExpr(this);
    }
    
    std::string value;
    TokenType type;
};

class UnaryExpr : public Expr {
public:
    UnaryExpr(Token op, std::shared_ptr<Expr> right)
        : op(std::move(op)), right(std::move(right)) {}
    
    void* accept(ExprVisitor* visitor) override {
        return visitor->visitUnaryExpr(this);
    }
    
    Token op;
    std::shared_ptr<Expr> right;
};

class VariableExpr : public Expr {
public:
    explicit VariableExpr(Token name)
        : name(std::move(name)) {}
    
    void* accept(ExprVisitor* visitor) override {
        return visitor->visitVariableExpr(this);
    }
    
    Token name;
};

class AssignExpr : public Expr {
public:
    AssignExpr(Token name, std::shared_ptr<Expr> value)
        : name(std::move(name)), value(std::move(value)) {}
    
    void* accept(ExprVisitor* visitor) override {
        return visitor->visitAssignExpr(this);
    }
    
    Token name;
    std::shared_ptr<Expr> value;
};

class CallExpr : public Expr {
public:
    CallExpr(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments)
        : callee(std::move(callee)), paren(std::move(paren)), arguments(std::move(arguments)) {}
    
    void* accept(ExprVisitor* visitor) override {
        return visitor->visitCallExpr(this);
    }
    
    std::shared_ptr<Expr> callee;
    Token paren;
    std::vector<std::shared_ptr<Expr>> arguments;
};

// Statement classes
class ExpressionStmt : public Stmt {
public:
    explicit ExpressionStmt(std::shared_ptr<Expr> expression)
        : expression(std::move(expression)) {}
    
    void* accept(StmtVisitor* visitor) override {
        return visitor->visitExpressionStmt(this);
    }
    
    std::shared_ptr<Expr> expression;
};

class VarStmt : public Stmt {
public:
    VarStmt(Token name, std::shared_ptr<Expr> initializer)
        : name(std::move(name)), initializer(std::move(initializer)) {}
    
    void* accept(StmtVisitor* visitor) override {
        return visitor->visitVarStmt(this);
    }
    
    Token name;
    std::shared_ptr<Expr> initializer;
};

class BlockStmt : public Stmt {
public:
    explicit BlockStmt(std::vector<std::shared_ptr<Stmt>> statements)
        : statements(std::move(statements)) {}
    
    void* accept(StmtVisitor* visitor) override {
        return visitor->visitBlockStmt(this);
    }
    
    std::vector<std::shared_ptr<Stmt>> statements;
};

class IfStmt : public Stmt {
public:
    IfStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}
    
    void* accept(StmtVisitor* visitor) override {
        return visitor->visitIfStmt(this);
    }
    
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> thenBranch;
    std::shared_ptr<Stmt> elseBranch;
};

class WhileStmt : public Stmt {
public:
    WhileStmt(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)) {}
    
    void* accept(StmtVisitor* visitor) override {
        return visitor->visitWhileStmt(this);
    }
    
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> body;
};

class FunctionStmt : public Stmt {
public:
    FunctionStmt(Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body)
        : name(std::move(name)), params(std::move(params)), body(std::move(body)) {}
    
    void* accept(StmtVisitor* visitor) override {
        return visitor->visitFunctionStmt(this);
    }
    
    Token name;
    std::vector<Token> params;
    std::vector<std::shared_ptr<Stmt>> body;
};

class ReturnStmt : public Stmt {
public:
    ReturnStmt(Token keyword, std::shared_ptr<Expr> value)
        : keyword(std::move(keyword)), value(std::move(value)) {}
    
    void* accept(StmtVisitor* visitor) override {
        return visitor->visitReturnStmt(this);
    }
    
    Token keyword;
    std::shared_ptr<Expr> value;
};

} // namespace tribhasha

#endif // TRIBHASHA_AST_H