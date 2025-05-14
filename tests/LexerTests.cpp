#include "tribhasha/Lexer.h"
#include <iostream>
#include <functional>
#include <cassert>

using namespace tribhasha;

// Test helper - find a token of specific type in tokens
bool hasToken(const std::vector<Token>& tokens, TokenType type, const std::string& lexeme = "") {
    for (const auto& token : tokens) {
        if (token.type == type && (lexeme.empty() || token.lexeme == lexeme)) {
            return true;
        }
    }
    return false;
}

// Basic English lexing test
bool testEnglishLexing() {
    std::string source = "var x = 10; function test() { return x + 5; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    
    return hasToken(tokens, TokenType::VAR_EN, "var") &&
           hasToken(tokens, TokenType::IDENTIFIER, "x") &&
           hasToken(tokens, TokenType::ASSIGN, "=") &&
           hasToken(tokens, TokenType::INT_LITERAL, "10") &&
           hasToken(tokens, TokenType::SEMICOLON, ";") &&
           hasToken(tokens, TokenType::FUNCTION_EN, "function") &&
           hasToken(tokens, TokenType::IDENTIFIER, "test") &&
           hasToken(tokens, TokenType::LEFT_PAREN, "(") &&
           hasToken(tokens, TokenType::RIGHT_PAREN, ")") &&
           hasToken(tokens, TokenType::LEFT_BRACE, "{") &&
           hasToken(tokens, TokenType::RETURN_EN, "return") &&
           hasToken(tokens, TokenType::IDENTIFIER, "x") &&
           hasToken(tokens, TokenType::PLUS, "+") &&
           hasToken(tokens, TokenType::INT_LITERAL, "5") &&
           hasToken(tokens, TokenType::SEMICOLON, ";") &&
           hasToken(tokens, TokenType::RIGHT_BRACE, "}") &&
           hasToken(tokens, TokenType::END_OF_FILE);
}

// Basic Hindi lexing test
bool testHindiLexing() {
    std::string source = "चर य = 10; फलन परीक्षण() { वापस य + 5; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    
    return hasToken(tokens, TokenType::VAR_HI, "चर") &&
           hasToken(tokens, TokenType::IDENTIFIER, "य") &&
           hasToken(tokens, TokenType::ASSIGN, "=") &&
           hasToken(tokens, TokenType::INT_LITERAL, "10") &&
           hasToken(tokens, TokenType::SEMICOLON, ";") &&
           hasToken(tokens, TokenType::FUNCTION_HI, "फलन") &&
           hasToken(tokens, TokenType::IDENTIFIER, "परीक्षण") &&
           hasToken(tokens, TokenType::LEFT_PAREN, "(") &&
           hasToken(tokens, TokenType::RIGHT_PAREN, ")") &&
           hasToken(tokens, TokenType::LEFT_BRACE, "{") &&
           hasToken(tokens, TokenType::RETURN_HI, "वापस") &&
           hasToken(tokens, TokenType::IDENTIFIER, "य") &&
           hasToken(tokens, TokenType::PLUS, "+") &&
           hasToken(tokens, TokenType::INT_LITERAL, "5") &&
           hasToken(tokens, TokenType::SEMICOLON, ";") &&
           hasToken(tokens, TokenType::RIGHT_BRACE, "}") &&
           hasToken(tokens, TokenType::END_OF_FILE);
}

// Basic Assamese lexing test
bool testAssameseLexing() {
    std::string source = "ভেৰিয়েবল জ = 10; কাৰ্য্য পৰীক্ষা() { ঘূৰাই_দিয়ক জ + 5; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    
    return hasToken(tokens, TokenType::VAR_AS, "ভেৰিয়েবল") &&
           hasToken(tokens, TokenType::IDENTIFIER, "জ") &&
           hasToken(tokens, TokenType::ASSIGN, "=") &&
           hasToken(tokens, TokenType::INT_LITERAL, "10") &&
           hasToken(tokens, TokenType::SEMICOLON, ";") &&
           hasToken(tokens, TokenType::FUNCTION_AS, "কাৰ্য্য") &&
           hasToken(tokens, TokenType::IDENTIFIER, "পৰীক্ষা") &&
           hasToken(tokens, TokenType::LEFT_PAREN, "(") &&
           hasToken(tokens, TokenType::RIGHT_PAREN, ")") &&
           hasToken(tokens, TokenType::LEFT_BRACE, "{") &&
           hasToken(tokens, TokenType::RETURN_AS, "ঘূৰাই_দিয়ক") &&
           hasToken(tokens, TokenType::IDENTIFIER, "জ") &&
           hasToken(tokens, TokenType::PLUS, "+") &&
           hasToken(tokens, TokenType::INT_LITERAL, "5") &&
           hasToken(tokens, TokenType::SEMICOLON, ";") &&
           hasToken(tokens, TokenType::RIGHT_BRACE, "}") &&
           hasToken(tokens, TokenType::END_OF_FILE);
}

// Mixed language lexing test
bool testMixedLanguageLexing() {
    std::string source = "function factorial(n) { अगर (n <= 1) { ঘূৰাই_দিয়ক 1; } return n * factorial(n - 1); }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    
    return hasToken(tokens, TokenType::FUNCTION_EN, "function") &&
           hasToken(tokens, TokenType::IDENTIFIER, "factorial") &&
           hasToken(tokens, TokenType::LEFT_PAREN, "(") &&
           hasToken(tokens, TokenType::IDENTIFIER, "n") &&
           hasToken(tokens, TokenType::RIGHT_PAREN, ")") &&
           hasToken(tokens, TokenType::LEFT_BRACE, "{") &&
           hasToken(tokens, TokenType::IF_HI, "अगर") &&
           hasToken(tokens, TokenType::LEFT_PAREN, "(") &&
           hasToken(tokens, TokenType::IDENTIFIER, "n") &&
           hasToken(tokens, TokenType::LESS_EQUAL, "<=") &&
           hasToken(tokens, TokenType::INT_LITERAL, "1") &&
           hasToken(tokens, TokenType::RIGHT_PAREN, ")") &&
           hasToken(tokens, TokenType::LEFT_BRACE, "{") &&
           hasToken(tokens, TokenType::RETURN_AS, "ঘূৰাই_দিয়ক") &&
           hasToken(tokens, TokenType::INT_LITERAL, "1") &&
           hasToken(tokens, TokenType::SEMICOLON, ";") &&
           hasToken(tokens, TokenType::RIGHT_BRACE, "}") &&
           hasToken(tokens, TokenType::RETURN_EN, "return") &&
           hasToken(tokens, TokenType::IDENTIFIER, "n") &&
           hasToken(tokens, TokenType::STAR, "*") &&
           hasToken(tokens, TokenType::IDENTIFIER, "factorial") &&
           hasToken(tokens, TokenType::LEFT_PAREN, "(") &&
           hasToken(tokens, TokenType::IDENTIFIER, "n") &&
           hasToken(tokens, TokenType::MINUS, "-") &&
           hasToken(tokens, TokenType::INT_LITERAL, "1") &&
           hasToken(tokens, TokenType::RIGHT_PAREN, ")") &&
           hasToken(tokens, TokenType::SEMICOLON, ";") &&
           hasToken(tokens, TokenType::RIGHT_BRACE, "}") &&
           hasToken(tokens, TokenType::END_OF_FILE);
}

// Register all lexer tests
void registerLexerTests() {
    // Initialize keyword maps
    Keywords::initialize();
    
    extern void registerTest(const std::string&, const std::string&, std::function<bool()>);
    
    registerTest("lexer", "English Lexing", testEnglishLexing);
    registerTest("lexer", "Hindi Lexing", testHindiLexing);
    registerTest("lexer", "Assamese Lexing", testAssameseLexing);
    registerTest("lexer", "Mixed Language Lexing", testMixedLanguageLexing);
}