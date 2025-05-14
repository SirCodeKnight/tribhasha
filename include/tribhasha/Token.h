#ifndef TRIBHASHA_TOKEN_H
#define TRIBHASHA_TOKEN_H

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

namespace tribhasha {

// Token types
enum class TokenType {
    // Special tokens
    END_OF_FILE,
    ERROR,
    
    // Literals
    IDENTIFIER,
    INT_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL,
    BOOL_LITERAL,
    
    // Keywords - English
    VAR_EN,        // var
    FUNCTION_EN,   // function
    IF_EN,         // if
    ELSE_EN,       // else
    WHILE_EN,      // while
    FOR_EN,        // for
    RETURN_EN,     // return
    TRUE_EN,       // true
    FALSE_EN,      // false
    AND_EN,        // and
    OR_EN,         // or
    NOT_EN,        // not
    
    // Keywords - Hindi
    VAR_HI,        // चर
    FUNCTION_HI,   // फलन
    IF_HI,         // अगर
    ELSE_HI,       // अन्यथा
    WHILE_HI,      // जबतक
    FOR_HI,        // के_लिए
    RETURN_HI,     // वापस
    TRUE_HI,       // सही
    FALSE_HI,      // गलत
    AND_HI,        // और
    OR_HI,         // या
    NOT_HI,        // नहीं
    
    // Keywords - Assamese
    VAR_AS,        // ভেৰিয়েবল
    FUNCTION_AS,   // কাৰ্য্য
    IF_AS,         // যদি
    ELSE_AS,       // নহলে
    WHILE_AS,      // যতক্ষণ
    FOR_AS,        // ৰ_বাবে
    RETURN_AS,     // ঘূৰাই_দিয়ক
    TRUE_AS,       // সত্য
    FALSE_AS,      // মিছা
    AND_AS,        // আৰু
    OR_AS,         // বা
    NOT_AS,        // নহয়
    
    // Operators
    PLUS,          // +
    MINUS,         // -
    STAR,          // *
    SLASH,         // /
    MODULO,        // %
    
    // Comparison
    EQUAL,         // ==
    NOT_EQUAL,     // !=
    LESS,          // <
    LESS_EQUAL,    // <=
    GREATER,       // >
    GREATER_EQUAL, // >=
    
    // Assignment
    ASSIGN,        // =
    
    // Delimiters
    LEFT_PAREN,    // (
    RIGHT_PAREN,   // )
    LEFT_BRACE,    // {
    RIGHT_BRACE,   // }
    LEFT_BRACKET,  // [
    RIGHT_BRACKET, // ]
    COMMA,         // ,
    DOT,           // .
    SEMICOLON,     // ;
    COLON,         // :
};

// Token structure
struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    
    Token(TokenType type, std::string lexeme, int line)
        : type(type), lexeme(std::move(lexeme)), line(line) {}
    
    std::string toString() const;
};

// Keyword lookup tables
class Keywords {
private:
    static std::unordered_map<std::string, TokenType> englishKeywords;
    static std::unordered_map<std::string, TokenType> hindiKeywords;
    static std::unordered_map<std::string, TokenType> assameseKeywords;
    
public:
    static TokenType getKeywordType(const std::string& word);
    static bool isKeyword(const std::string& word);
    
    // Helper to get the canonical keyword in any language
    static TokenType normalizeKeywordType(TokenType type);
    
    // Initialize the keyword maps
    static void initialize();
};

} // namespace tribhasha

#endif // TRIBHASHA_TOKEN_H