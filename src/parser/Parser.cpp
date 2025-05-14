#include "tribhasha/Parser.h"
#include <iostream>

namespace tribhasha {

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    
    while (!isAtEnd()) {
        try {
            statements.push_back(declaration());
        } catch (const ParseError& error) {
            synchronize();
        }
    }
    
    return statements;
}

// Helper methods
bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

Token Parser::peek() const {
    return tokens[current];
}

Token Parser::previous() const {
    return tokens[current - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::matchAny(const std::vector<TokenType>& types) {
    for (const auto& type : types) {
        if (match(type)) return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

ParseError Parser::error(Token token, const std::string& message) {
    std::cerr << "[line " << token.line << "] Error";
    
    if (token.type == TokenType::END_OF_FILE) {
        std::cerr << " at end";
    } else {
        std::cerr << " at '" << token.lexeme << "'";
    }
    
    std::cerr << ": " << message << std::endl;
    
    return ParseError(message);
}

void Parser::synchronize() {
    advance();
    
    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;
        
        switch (peek().type) {
            case TokenType::VAR_EN:
            case TokenType::VAR_HI:
            case TokenType::VAR_AS:
            case TokenType::FUNCTION_EN:
            case TokenType::FUNCTION_HI:
            case TokenType::FUNCTION_AS:
            case TokenType::IF_EN:
            case TokenType::IF_HI:
            case TokenType::IF_AS:
            case TokenType::WHILE_EN:
            case TokenType::WHILE_HI:
            case TokenType::WHILE_AS:
            case TokenType::FOR_EN:
            case TokenType::FOR_HI:
            case TokenType::FOR_AS:
            case TokenType::RETURN_EN:
            case TokenType::RETURN_HI:
            case TokenType::RETURN_AS:
                return;
            default:
                break;
        }
        
        advance();
    }
}

// Grammar rules
std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment() {
    auto expr = equality();
    
    if (match(TokenType::ASSIGN)) {
        Token equals = previous();
        auto value = assignment();
        
        if (auto* varExpr = dynamic_cast<VariableExpr*>(expr.get())) {
            Token name = varExpr->name;
            return std::make_shared<AssignExpr>(name, value);
        }
        
        error(equals, "Invalid assignment target.");
    }
    
    return expr;
}

std::shared_ptr<Expr> Parser::equality() {
    auto expr = comparison();
    
    while (matchAny({TokenType::EQUAL, TokenType::NOT_EQUAL})) {
        Token op = previous();
        auto right = comparison();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    
    return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
    auto expr = term();
    
    while (matchAny({TokenType::LESS, TokenType::LESS_EQUAL, TokenType::GREATER, TokenType::GREATER_EQUAL})) {
        Token op = previous();
        auto right = term();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    
    return expr;
}

std::shared_ptr<Expr> Parser::term() {
    auto expr = factor();
    
    while (matchAny({TokenType::PLUS, TokenType::MINUS})) {
        Token op = previous();
        auto right = factor();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    
    return expr;
}

std::shared_ptr<Expr> Parser::factor() {
    auto expr = unary();
    
    while (matchAny({TokenType::STAR, TokenType::SLASH, TokenType::MODULO})) {
        Token op = previous();
        auto right = unary();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    
    return expr;
}

std::shared_ptr<Expr> Parser::unary() {
    if (matchAny({TokenType::MINUS, TokenType::NOT_EN, TokenType::NOT_HI, TokenType::NOT_AS})) {
        Token op = previous();
        auto right = unary();
        return std::make_shared<UnaryExpr>(op, right);
    }
    
    return call();
}

std::shared_ptr<Expr> Parser::call() {
    auto expr = primary();
    
    while (true) {
        if (match(TokenType::LEFT_PAREN)) {
            expr = finishCall(expr);
        } else {
            break;
        }
    }
    
    return expr;
}

std::shared_ptr<Expr> Parser::finishCall(std::shared_ptr<Expr> callee) {
    std::vector<std::shared_ptr<Expr>> arguments;
    
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            arguments.push_back(expression());
        } while (match(TokenType::COMMA));
    }
    
    Token paren = consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments.");
    
    return std::make_shared<CallExpr>(callee, paren, arguments);
}

std::shared_ptr<Expr> Parser::primary() {
    // True literal (in any language)
    if (matchAny({TokenType::TRUE_EN, TokenType::TRUE_HI, TokenType::TRUE_AS})) {
        return std::make_shared<LiteralExpr>("true", TokenType::TRUE_EN);
    }
    
    // False literal (in any language)
    if (matchAny({TokenType::FALSE_EN, TokenType::FALSE_HI, TokenType::FALSE_AS})) {
        return std::make_shared<LiteralExpr>("false", TokenType::FALSE_EN);
    }
    
    // Number literals
    if (matchAny({TokenType::INT_LITERAL, TokenType::FLOAT_LITERAL})) {
        return std::make_shared<LiteralExpr>(previous().lexeme, previous().type);
    }
    
    // String literals
    if (match(TokenType::STRING_LITERAL)) {
        return std::make_shared<LiteralExpr>(previous().lexeme, TokenType::STRING_LITERAL);
    }
    
    // Grouping
    if (match(TokenType::LEFT_PAREN)) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
        return std::make_shared<GroupingExpr>(expr);
    }
    
    // Variable
    if (match(TokenType::IDENTIFIER)) {
        return std::make_shared<VariableExpr>(previous());
    }
    
    throw error(peek(), "Expected expression.");
}

std::shared_ptr<Stmt> Parser::statement() {
    // If statement (in any language)
    if (matchAny({TokenType::IF_EN, TokenType::IF_HI, TokenType::IF_AS})) {
        return ifStatement();
    }
    
    // While statement (in any language)
    if (matchAny({TokenType::WHILE_EN, TokenType::WHILE_HI, TokenType::WHILE_AS})) {
        return whileStatement();
    }
    
    // For statement (in any language)
    if (matchAny({TokenType::FOR_EN, TokenType::FOR_HI, TokenType::FOR_AS})) {
        return forStatement();
    }
    
    // Return statement (in any language)
    if (matchAny({TokenType::RETURN_EN, TokenType::RETURN_HI, TokenType::RETURN_AS})) {
        return returnStatement();
    }
    
    // Block statement
    if (match(TokenType::LEFT_BRACE)) {
        return blockStatement();
    }
    
    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::declaration() {
    // Variable declaration (in any language)
    if (matchAny({TokenType::VAR_EN, TokenType::VAR_HI, TokenType::VAR_AS})) {
        return varDeclaration();
    }
    
    // Function declaration (in any language)
    if (matchAny({TokenType::FUNCTION_EN, TokenType::FUNCTION_HI, TokenType::FUNCTION_AS})) {
        return functionDeclaration("function");
    }
    
    return statement();
}

std::shared_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name.");
    
    std::shared_ptr<Expr> initializer = nullptr;
    if (match(TokenType::ASSIGN)) {
        initializer = expression();
    }
    
    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration.");
    return std::make_shared<VarStmt>(name, initializer);
}

std::shared_ptr<Stmt> Parser::expressionStatement() {
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression.");
    return std::make_shared<ExpressionStmt>(expr);
}

std::shared_ptr<Stmt> Parser::blockStatement() {
    std::vector<std::shared_ptr<Stmt>> statements;
    
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after block.");
    return std::make_shared<BlockStmt>(statements);
}

std::shared_ptr<Stmt> Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'if'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after if condition.");
    
    auto thenBranch = statement();
    std::shared_ptr<Stmt> elseBranch = nullptr;
    
    if (matchAny({TokenType::ELSE_EN, TokenType::ELSE_HI, TokenType::ELSE_AS})) {
        elseBranch = statement();
    }
    
    return std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
}

std::shared_ptr<Stmt> Parser::whileStatement() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'while'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after while condition.");
    
    auto body = statement();
    
    return std::make_shared<WhileStmt>(condition, body);
}

std::shared_ptr<Stmt> Parser::forStatement() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'for'.");
    
    // Initializer
    std::shared_ptr<Stmt> initializer;
    if (match(TokenType::SEMICOLON)) {
        initializer = nullptr;
    } else if (matchAny({TokenType::VAR_EN, TokenType::VAR_HI, TokenType::VAR_AS})) {
        initializer = varDeclaration();
    } else {
        initializer = expressionStatement();
    }
    
    // Condition
    std::shared_ptr<Expr> condition = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expected ';' after loop condition.");
    
    // Increment
    std::shared_ptr<Expr> increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) {
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expected ')' after for clauses.");
    
    // Body
    auto body = statement();
    
    // Desugar 'for' loop into a 'while' loop
    if (increment != nullptr) {
        body = std::make_shared<BlockStmt>(std::vector<std::shared_ptr<Stmt>>{
            body,
            std::make_shared<ExpressionStmt>(increment)
        });
    }
    
    if (condition == nullptr) {
        condition = std::make_shared<LiteralExpr>("true", TokenType::TRUE_EN);
    }
    body = std::make_shared<WhileStmt>(condition, body);
    
    if (initializer != nullptr) {
        body = std::make_shared<BlockStmt>(std::vector<std::shared_ptr<Stmt>>{initializer, body});
    }
    
    return body;
}

std::shared_ptr<Stmt> Parser::functionDeclaration(const std::string& kind) {
    Token name = consume(TokenType::IDENTIFIER, "Expected " + kind + " name.");
    
    consume(TokenType::LEFT_PAREN, "Expected '(' after " + kind + " name.");
    std::vector<Token> parameters;
    
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            parameters.push_back(consume(TokenType::IDENTIFIER, "Expected parameter name."));
        } while (match(TokenType::COMMA));
    }
    
    consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters.");
    
    consume(TokenType::LEFT_BRACE, "Expected '{' before " + kind + " body.");
    auto body = std::dynamic_pointer_cast<BlockStmt>(blockStatement())->statements;
    
    return std::make_shared<FunctionStmt>(name, parameters, body);
}

std::shared_ptr<Stmt> Parser::returnStatement() {
    Token keyword = previous();
    std::shared_ptr<Expr> value = nullptr;
    
    if (!check(TokenType::SEMICOLON)) {
        value = expression();
    }
    
    consume(TokenType::SEMICOLON, "Expected ';' after return value.");
    return std::make_shared<ReturnStmt>(keyword, value);
}

// Language-specific helpers
TokenType Parser::getVarTokenType() const {
    if (check(TokenType::VAR_EN)) return TokenType::VAR_EN;
    if (check(TokenType::VAR_HI)) return TokenType::VAR_HI;
    if (check(TokenType::VAR_AS)) return TokenType::VAR_AS;
    return TokenType::VAR_EN; // Default to English
}

TokenType Parser::getFunctionTokenType() const {
    if (check(TokenType::FUNCTION_EN)) return TokenType::FUNCTION_EN;
    if (check(TokenType::FUNCTION_HI)) return TokenType::FUNCTION_HI;
    if (check(TokenType::FUNCTION_AS)) return TokenType::FUNCTION_AS;
    return TokenType::FUNCTION_EN; // Default to English
}

TokenType Parser::getIfTokenType() const {
    if (check(TokenType::IF_EN)) return TokenType::IF_EN;
    if (check(TokenType::IF_HI)) return TokenType::IF_HI;
    if (check(TokenType::IF_AS)) return TokenType::IF_AS;
    return TokenType::IF_EN; // Default to English
}

TokenType Parser::getElseTokenType() const {
    if (check(TokenType::ELSE_EN)) return TokenType::ELSE_EN;
    if (check(TokenType::ELSE_HI)) return TokenType::ELSE_HI;
    if (check(TokenType::ELSE_AS)) return TokenType::ELSE_AS;
    return TokenType::ELSE_EN; // Default to English
}

TokenType Parser::getWhileTokenType() const {
    if (check(TokenType::WHILE_EN)) return TokenType::WHILE_EN;
    if (check(TokenType::WHILE_HI)) return TokenType::WHILE_HI;
    if (check(TokenType::WHILE_AS)) return TokenType::WHILE_AS;
    return TokenType::WHILE_EN; // Default to English
}

TokenType Parser::getForTokenType() const {
    if (check(TokenType::FOR_EN)) return TokenType::FOR_EN;
    if (check(TokenType::FOR_HI)) return TokenType::FOR_HI;
    if (check(TokenType::FOR_AS)) return TokenType::FOR_AS;
    return TokenType::FOR_EN; // Default to English
}

TokenType Parser::getReturnTokenType() const {
    if (check(TokenType::RETURN_EN)) return TokenType::RETURN_EN;
    if (check(TokenType::RETURN_HI)) return TokenType::RETURN_HI;
    if (check(TokenType::RETURN_AS)) return TokenType::RETURN_AS;
    return TokenType::RETURN_EN; // Default to English
}

TokenType Parser::getTrueTokenType() const {
    if (check(TokenType::TRUE_EN)) return TokenType::TRUE_EN;
    if (check(TokenType::TRUE_HI)) return TokenType::TRUE_HI;
    if (check(TokenType::TRUE_AS)) return TokenType::TRUE_AS;
    return TokenType::TRUE_EN; // Default to English
}

TokenType Parser::getFalseTokenType() const {
    if (check(TokenType::FALSE_EN)) return TokenType::FALSE_EN;
    if (check(TokenType::FALSE_HI)) return TokenType::FALSE_HI;
    if (check(TokenType::FALSE_AS)) return TokenType::FALSE_AS;
    return TokenType::FALSE_EN; // Default to English
}

} // namespace tribhasha