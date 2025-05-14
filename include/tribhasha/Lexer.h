#ifndef TRIBHASHA_LEXER_H
#define TRIBHASHA_LEXER_H

#include "Token.h"
#include <string>
#include <vector>

namespace tribhasha {

class Lexer {
private:
    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;
    
    // Helper methods
    bool isAtEnd() const;
    char advance();
    bool match(char expected);
    char peek() const;
    char peekNext() const;
    
    // Lexing methods
    void scanToken();
    void addToken(TokenType type);
    void addToken(TokenType type, const std::string& literal);
    
    // Specific token scanners
    void scanString();
    void scanNumber();
    void scanIdentifier();
    
    // Helper for Unicode support
    bool isAlpha(char c) const;
    bool isUnicodeAlpha(const std::string& s, size_t pos) const;
    bool isDigit(char c) const;
    bool isAlphaNumeric(char c) const;
    bool isUnicodeAlphaNumeric(const std::string& s, size_t pos) const;
    
    // Error handling
    void error(int line, const std::string& message);
    
public:
    explicit Lexer(std::string source);
    
    std::vector<Token> scanTokens();
};

} // namespace tribhasha

#endif // TRIBHASHA_LEXER_H