#ifndef TRIBHASHA_PARSER_H
#define TRIBHASHA_PARSER_H

#include "Token.h"
#include "AST.h"
#include <vector>
#include <memory>
#include <stdexcept>

namespace tribhasha {

class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& message) : std::runtime_error(message) {}
};

class Parser {
private:
    std::vector<Token> tokens;
    int current = 0;
    
    // Helper methods
    bool isAtEnd() const;
    Token peek() const;
    Token previous() const;
    Token advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    bool matchAny(const std::vector<TokenType>& types);
    Token consume(TokenType type, const std::string& message);
    ParseError error(Token token, const std::string& message);
    void synchronize();
    
    // Grammar rules
    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> assignment();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> call();
    std::shared_ptr<Expr> primary();
    std::shared_ptr<Expr> finishCall(std::shared_ptr<Expr> callee);
    
    std::shared_ptr<Stmt> statement();
    std::shared_ptr<Stmt> declaration();
    std::shared_ptr<Stmt> varDeclaration();
    std::shared_ptr<Stmt> expressionStatement();
    std::shared_ptr<Stmt> blockStatement();
    std::shared_ptr<Stmt> ifStatement();
    std::shared_ptr<Stmt> whileStatement();
    std::shared_ptr<Stmt> forStatement();
    std::shared_ptr<Stmt> functionDeclaration(const std::string& kind);
    std::shared_ptr<Stmt> returnStatement();
    
    // Language-specific helpers
    TokenType getVarTokenType() const;
    TokenType getFunctionTokenType() const;
    TokenType getIfTokenType() const;
    TokenType getElseTokenType() const;
    TokenType getWhileTokenType() const;
    TokenType getForTokenType() const;
    TokenType getReturnTokenType() const;
    TokenType getTrueTokenType() const;
    TokenType getFalseTokenType() const;
    
public:
    explicit Parser(std::vector<Token> tokens);
    
    std::vector<std::shared_ptr<Stmt>> parse();
};

} // namespace tribhasha

#endif // TRIBHASHA_PARSER_H