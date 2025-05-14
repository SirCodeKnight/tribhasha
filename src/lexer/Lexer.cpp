#include "tribhasha/Lexer.h"
#include <iostream>
#include <codecvt>
#include <locale>

namespace tribhasha {

// Initialize static keyword maps
std::unordered_map<std::string, TokenType> Keywords::englishKeywords = {
    {"var", TokenType::VAR_EN},
    {"function", TokenType::FUNCTION_EN},
    {"if", TokenType::IF_EN},
    {"else", TokenType::ELSE_EN},
    {"while", TokenType::WHILE_EN},
    {"for", TokenType::FOR_EN},
    {"return", TokenType::RETURN_EN},
    {"true", TokenType::TRUE_EN},
    {"false", TokenType::FALSE_EN},
    {"and", TokenType::AND_EN},
    {"or", TokenType::OR_EN},
    {"not", TokenType::NOT_EN}
};

std::unordered_map<std::string, TokenType> Keywords::hindiKeywords = {
    {"चर", TokenType::VAR_HI},
    {"फलन", TokenType::FUNCTION_HI},
    {"अगर", TokenType::IF_HI},
    {"अन्यथा", TokenType::ELSE_HI},
    {"जबतक", TokenType::WHILE_HI},
    {"के_लिए", TokenType::FOR_HI},
    {"वापस", TokenType::RETURN_HI},
    {"सही", TokenType::TRUE_HI},
    {"गलत", TokenType::FALSE_HI},
    {"और", TokenType::AND_HI},
    {"या", TokenType::OR_HI},
    {"नहीं", TokenType::NOT_HI}
};

std::unordered_map<std::string, TokenType> Keywords::assameseKeywords = {
    {"ভেৰিয়েবল", TokenType::VAR_AS},
    {"কাৰ্য্য", TokenType::FUNCTION_AS},
    {"যদি", TokenType::IF_AS},
    {"নহলে", TokenType::ELSE_AS},
    {"যতক্ষণ", TokenType::WHILE_AS},
    {"ৰ_বাবে", TokenType::FOR_AS},
    {"ঘূৰাই_দিয়ক", TokenType::RETURN_AS},
    {"সত্য", TokenType::TRUE_AS},
    {"মিছা", TokenType::FALSE_AS},
    {"আৰু", TokenType::AND_AS},
    {"বা", TokenType::OR_AS},
    {"নহয়", TokenType::NOT_AS}
};

// Implementation of Keywords class methods
TokenType Keywords::getKeywordType(const std::string& word) {
    // Check English keywords
    auto it = englishKeywords.find(word);
    if (it != englishKeywords.end()) {
        return it->second;
    }
    
    // Check Hindi keywords
    it = hindiKeywords.find(word);
    if (it != hindiKeywords.end()) {
        return it->second;
    }
    
    // Check Assamese keywords
    it = assameseKeywords.find(word);
    if (it != assameseKeywords.end()) {
        return it->second;
    }
    
    return TokenType::IDENTIFIER;
}

bool Keywords::isKeyword(const std::string& word) {
    return englishKeywords.find(word) != englishKeywords.end() ||
           hindiKeywords.find(word) != hindiKeywords.end() ||
           assameseKeywords.find(word) != assameseKeywords.end();
}

TokenType Keywords::normalizeKeywordType(TokenType type) {
    // Convert language-specific keywords to a canonical form
    switch (type) {
        // Variable declarations
        case TokenType::VAR_EN:
        case TokenType::VAR_HI:
        case TokenType::VAR_AS:
            return TokenType::VAR_EN;
            
        // Functions
        case TokenType::FUNCTION_EN:
        case TokenType::FUNCTION_HI:
        case TokenType::FUNCTION_AS:
            return TokenType::FUNCTION_EN;
            
        // Control flow
        case TokenType::IF_EN:
        case TokenType::IF_HI:
        case TokenType::IF_AS:
            return TokenType::IF_EN;
            
        case TokenType::ELSE_EN:
        case TokenType::ELSE_HI:
        case TokenType::ELSE_AS:
            return TokenType::ELSE_EN;
            
        case TokenType::WHILE_EN:
        case TokenType::WHILE_HI:
        case TokenType::WHILE_AS:
            return TokenType::WHILE_EN;
            
        case TokenType::FOR_EN:
        case TokenType::FOR_HI:
        case TokenType::FOR_AS:
            return TokenType::FOR_EN;
            
        case TokenType::RETURN_EN:
        case TokenType::RETURN_HI:
        case TokenType::RETURN_AS:
            return TokenType::RETURN_EN;
            
        // Boolean literals
        case TokenType::TRUE_EN:
        case TokenType::TRUE_HI:
        case TokenType::TRUE_AS:
            return TokenType::TRUE_EN;
            
        case TokenType::FALSE_EN:
        case TokenType::FALSE_HI:
        case TokenType::FALSE_AS:
            return TokenType::FALSE_EN;
            
        // Logical operators
        case TokenType::AND_EN:
        case TokenType::AND_HI:
        case TokenType::AND_AS:
            return TokenType::AND_EN;
            
        case TokenType::OR_EN:
        case TokenType::OR_HI:
        case TokenType::OR_AS:
            return TokenType::OR_EN;
            
        case TokenType::NOT_EN:
        case TokenType::NOT_HI:
        case TokenType::NOT_AS:
            return TokenType::NOT_EN;
            
        default:
            return type;
    }
}

void Keywords::initialize() {
    // Already initialized in the static initializers
}

// Token toString method
std::string Token::toString() const {
    std::string typeStr;
    
    // Convert token type to string
    switch (type) {
        case TokenType::END_OF_FILE: typeStr = "EOF"; break;
        case TokenType::ERROR: typeStr = "ERROR"; break;
        case TokenType::IDENTIFIER: typeStr = "IDENTIFIER"; break;
        case TokenType::INT_LITERAL: typeStr = "INT_LITERAL"; break;
        case TokenType::FLOAT_LITERAL: typeStr = "FLOAT_LITERAL"; break;
        case TokenType::STRING_LITERAL: typeStr = "STRING_LITERAL"; break;
        case TokenType::BOOL_LITERAL: typeStr = "BOOL_LITERAL"; break;
        // English keywords
        case TokenType::VAR_EN: typeStr = "VAR_EN"; break;
        case TokenType::FUNCTION_EN: typeStr = "FUNCTION_EN"; break;
        case TokenType::IF_EN: typeStr = "IF_EN"; break;
        case TokenType::ELSE_EN: typeStr = "ELSE_EN"; break;
        case TokenType::WHILE_EN: typeStr = "WHILE_EN"; break;
        case TokenType::FOR_EN: typeStr = "FOR_EN"; break;
        case TokenType::RETURN_EN: typeStr = "RETURN_EN"; break;
        case TokenType::TRUE_EN: typeStr = "TRUE_EN"; break;
        case TokenType::FALSE_EN: typeStr = "FALSE_EN"; break;
        case TokenType::AND_EN: typeStr = "AND_EN"; break;
        case TokenType::OR_EN: typeStr = "OR_EN"; break;
        case TokenType::NOT_EN: typeStr = "NOT_EN"; break;
        // Hindi keywords
        case TokenType::VAR_HI: typeStr = "VAR_HI"; break;
        case TokenType::FUNCTION_HI: typeStr = "FUNCTION_HI"; break;
        case TokenType::IF_HI: typeStr = "IF_HI"; break;
        case TokenType::ELSE_HI: typeStr = "ELSE_HI"; break;
        case TokenType::WHILE_HI: typeStr = "WHILE_HI"; break;
        case TokenType::FOR_HI: typeStr = "FOR_HI"; break;
        case TokenType::RETURN_HI: typeStr = "RETURN_HI"; break;
        case TokenType::TRUE_HI: typeStr = "TRUE_HI"; break;
        case TokenType::FALSE_HI: typeStr = "FALSE_HI"; break;
        case TokenType::AND_HI: typeStr = "AND_HI"; break;
        case TokenType::OR_HI: typeStr = "OR_HI"; break;
        case TokenType::NOT_HI: typeStr = "NOT_HI"; break;
        // Assamese keywords
        case TokenType::VAR_AS: typeStr = "VAR_AS"; break;
        case TokenType::FUNCTION_AS: typeStr = "FUNCTION_AS"; break;
        case TokenType::IF_AS: typeStr = "IF_AS"; break;
        case TokenType::ELSE_AS: typeStr = "ELSE_AS"; break;
        case TokenType::WHILE_AS: typeStr = "WHILE_AS"; break;
        case TokenType::FOR_AS: typeStr = "FOR_AS"; break;
        case TokenType::RETURN_AS: typeStr = "RETURN_AS"; break;
        case TokenType::TRUE_AS: typeStr = "TRUE_AS"; break;
        case TokenType::FALSE_AS: typeStr = "FALSE_AS"; break;
        case TokenType::AND_AS: typeStr = "AND_AS"; break;
        case TokenType::OR_AS: typeStr = "OR_AS"; break;
        case TokenType::NOT_AS: typeStr = "NOT_AS"; break;
        // Operators
        case TokenType::PLUS: typeStr = "PLUS"; break;
        case TokenType::MINUS: typeStr = "MINUS"; break;
        case TokenType::STAR: typeStr = "STAR"; break;
        case TokenType::SLASH: typeStr = "SLASH"; break;
        case TokenType::MODULO: typeStr = "MODULO"; break;
        // Comparison
        case TokenType::EQUAL: typeStr = "EQUAL"; break;
        case TokenType::NOT_EQUAL: typeStr = "NOT_EQUAL"; break;
        case TokenType::LESS: typeStr = "LESS"; break;
        case TokenType::LESS_EQUAL: typeStr = "LESS_EQUAL"; break;
        case TokenType::GREATER: typeStr = "GREATER"; break;
        case TokenType::GREATER_EQUAL: typeStr = "GREATER_EQUAL"; break;
        // Assignment
        case TokenType::ASSIGN: typeStr = "ASSIGN"; break;
        // Delimiters
        case TokenType::LEFT_PAREN: typeStr = "LEFT_PAREN"; break;
        case TokenType::RIGHT_PAREN: typeStr = "RIGHT_PAREN"; break;
        case TokenType::LEFT_BRACE: typeStr = "LEFT_BRACE"; break;
        case TokenType::RIGHT_BRACE: typeStr = "RIGHT_BRACE"; break;
        case TokenType::LEFT_BRACKET: typeStr = "LEFT_BRACKET"; break;
        case TokenType::RIGHT_BRACKET: typeStr = "RIGHT_BRACKET"; break;
        case TokenType::COMMA: typeStr = "COMMA"; break;
        case TokenType::DOT: typeStr = "DOT"; break;
        case TokenType::SEMICOLON: typeStr = "SEMICOLON"; break;
        case TokenType::COLON: typeStr = "COLON"; break;
        default: typeStr = "UNKNOWN"; break;
    }
    
    return typeStr + " " + lexeme + " (line " + std::to_string(line) + ")";
}

// Lexer implementation
Lexer::Lexer(std::string source) : source(std::move(source)) {}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        // We are at the beginning of the next lexeme
        start = current;
        scanToken();
    }
    
    tokens.emplace_back(TokenType::END_OF_FILE, "", line);
    return tokens;
}

void Lexer::scanToken() {
    char c = advance();
    
    switch (c) {
        // Single character tokens
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case '[': addToken(TokenType::LEFT_BRACKET); break;
        case ']': addToken(TokenType::RIGHT_BRACKET); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case ':': addToken(TokenType::COLON); break;
        case '*': addToken(TokenType::STAR); break;
        case '%': addToken(TokenType::MODULO); break;
        
        // One or two character tokens
        case '!': addToken(match('=') ? TokenType::NOT_EQUAL : TokenType::NOT_EN); break;
        case '=': addToken(match('=') ? TokenType::EQUAL : TokenType::ASSIGN); break;
        case '<': addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
        
        // Division or comment
        case '/':
            if (match('/')) {
                // A comment goes until the end of the line
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                addToken(TokenType::SLASH);
            }
            break;
            
        // Whitespace
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace
            break;
            
        case '\n':
            line++;
            break;
            
        // String literals
        case '"': scanString(); break;
        
        default:
            if (isDigit(c)) {
                scanNumber();
            } else if (isAlpha(c) || isUnicodeAlpha(source, current - 1)) {
                scanIdentifier();
            } else {
                error(line, "Unexpected character.");
            }
            break;
    }
}

bool Lexer::isAtEnd() const {
    return current >= static_cast<int>(source.length());
}

char Lexer::advance() {
    return source[current++];
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;
    
    current++;
    return true;
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Lexer::peekNext() const {
    if (current + 1 >= static_cast<int>(source.length())) return '\0';
    return source[current + 1];
}

void Lexer::scanString() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }
    
    if (isAtEnd()) {
        error(line, "Unterminated string.");
        return;
    }
    
    // The closing "
    advance();
    
    // Trim the surrounding quotes
    std::string value = source.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING_LITERAL, value);
}

void Lexer::scanNumber() {
    while (isDigit(peek())) advance();
    
    // Look for a fractional part
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the "."
        advance();
        
        while (isDigit(peek())) advance();
        
        addToken(TokenType::FLOAT_LITERAL, source.substr(start, current - start));
    } else {
        addToken(TokenType::INT_LITERAL, source.substr(start, current - start));
    }
}

void Lexer::scanIdentifier() {
    while (isAlphaNumeric(peek()) || isUnicodeAlphaNumeric(source, current)) advance();
    
    // See if the identifier is a reserved word
    std::string text = source.substr(start, current - start);
    TokenType type = Keywords::getKeywordType(text);
    
    addToken(type);
}

bool Lexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool Lexer::isUnicodeAlpha(const std::string& s, size_t pos) const {
    // Basic check for Devanagari (Hindi) and Bengali (Assamese) script characters
    // This is a simplification - a real implementation would use Unicode libraries
    unsigned char c = s[pos];
    
    // Check if it's a multi-byte UTF-8 character
    if ((c & 0xC0) == 0xC0) {
        // Devanagari range: U+0900 to U+097F (approx)
        // Bengali range: U+0980 to U+09FF (approx)
        return true;
    }
    
    return false;
}

bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

bool Lexer::isUnicodeAlphaNumeric(const std::string& s, size_t pos) const {
    return isUnicodeAlpha(s, pos);
}

void Lexer::addToken(TokenType type) {
    addToken(type, source.substr(start, current - start));
}

void Lexer::addToken(TokenType type, const std::string& literal) {
    tokens.emplace_back(type, literal, line);
}

void Lexer::error(int line, const std::string& message) {
    std::cerr << "[line " << line << "] Error: " << message << std::endl;
}

} // namespace tribhasha