#include "tribhasha/Lexer.h"
#include "tribhasha/Parser.h"
#include <iostream>
#include <functional>
#include <cassert>

using namespace tribhasha;

// Test if parsing succeeds without errors
bool testParsingSuccess(const std::string& source) {
    try {
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.scanTokens();
        
        Parser parser(tokens);
        auto statements = parser.parse();
        
        return !statements.empty();
    } catch (const std::exception& e) {
        std::cerr << "Parsing error: " << e.what() << std::endl;
        return false;
    }
}

// Basic English parsing test
bool testEnglishParsing() {
    std::string source = "var x = 10; function test() { return x + 5; }";
    return testParsingSuccess(source);
}

// Basic Hindi parsing test
bool testHindiParsing() {
    std::string source = "चर य = 10; फलन परीक्षण() { वापस य + 5; }";
    return testParsingSuccess(source);
}

// Basic Assamese parsing test
bool testAssameseParsing() {
    std::string source = "ভেৰিয়েবল জ = 10; কাৰ্য্য পৰীক্ষা() { ঘূৰাই_দিয়ক জ + 5; }";
    return testParsingSuccess(source);
}

// Mixed language parsing test
bool testMixedLanguageParsing() {
    std::string source = "function factorial(n) { अगर (n <= 1) { ঘূৰাই_দিয়ক 1; } return n * factorial(n - 1); }";
    return testParsingSuccess(source);
}

// Test if function parsing works correctly
bool testFunctionParsing() {
    std::string source = R"(
        function add(a, b) {
            return a + b;
        }
        
        फलन घटाव(a, b) {
            वापस a - b;
        }
        
        কাৰ্য্য গুণন(a, b) {
            ঘূৰাই_দিয়ক a * b;
        }
    )";
    
    return testParsingSuccess(source);
}

// Test if control flow parsing works correctly
bool testControlFlowParsing() {
    std::string source = R"(
        function test(n) {
            if (n > 10) {
                return "Greater than 10";
            } else {
                return "Less than or equal to 10";
            }
        }
        
        फलन परीक्षण(n) {
            जबतक (n > 0) {
                n = n - 1;
            }
            वापस n;
        }
        
        কাৰ্য্য লুপ_পৰীক্ষা(n) {
            ৰ_বাবে (চর i = 0; i < n; i = i + 1) {
                দেখুৱাওক(i);
            }
            ঘূৰাই_দিয়ক 0;
        }
    )";
    
    return testParsingSuccess(source);
}

// Register all parser tests
void registerParserTests() {
    // Initialize keyword maps
    Keywords::initialize();
    
    extern void registerTest(const std::string&, const std::string&, std::function<bool()>);
    
    registerTest("parser", "English Parsing", testEnglishParsing);
    registerTest("parser", "Hindi Parsing", testHindiParsing);
    registerTest("parser", "Assamese Parsing", testAssameseParsing);
    registerTest("parser", "Mixed Language Parsing", testMixedLanguageParsing);
    registerTest("parser", "Function Parsing", testFunctionParsing);
    registerTest("parser", "Control Flow Parsing", testControlFlowParsing);
}