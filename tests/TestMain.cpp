#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

// Test function type
using TestFunction = std::function<bool()>;

// Test registry
std::unordered_map<std::string, std::vector<std::pair<std::string, TestFunction>>> tests;

// Register a test
void registerTest(const std::string& suite, const std::string& name, TestFunction test) {
    tests[suite].push_back({name, test});
}

// Forward declarations for test suites
void registerLexerTests();
void registerParserTests();

int main(int argc, char* argv[]) {
    // Register all test suites
    registerLexerTests();
    registerParserTests();
    
    // If a specific suite is requested, only run that suite
    if (argc > 1) {
        std::string requestedSuite = argv[1];
        
        auto it = tests.find(requestedSuite);
        if (it == tests.end()) {
            std::cerr << "Test suite not found: " << requestedSuite << std::endl;
            return 1;
        }
        
        std::cout << "Running test suite: " << requestedSuite << std::endl;
        
        int passed = 0;
        int failed = 0;
        
        for (const auto& test : it->second) {
            std::cout << "  " << test.first << ": ";
            bool result = test.second();
            if (result) {
                std::cout << "PASS" << std::endl;
                passed++;
            } else {
                std::cout << "FAIL" << std::endl;
                failed++;
            }
        }
        
        std::cout << std::endl;
        std::cout << "Tests: " << (passed + failed) << ", Passed: " << passed << ", Failed: " << failed << std::endl;
        
        return failed > 0 ? 1 : 0;
    }
    
    // Otherwise, run all test suites
    int totalPassed = 0;
    int totalFailed = 0;
    
    for (const auto& suite : tests) {
        std::cout << "Running test suite: " << suite.first << std::endl;
        
        int passed = 0;
        int failed = 0;
        
        for (const auto& test : suite.second) {
            std::cout << "  " << test.first << ": ";
            bool result = test.second();
            if (result) {
                std::cout << "PASS" << std::endl;
                passed++;
            } else {
                std::cout << "FAIL" << std::endl;
                failed++;
            }
        }
        
        std::cout << "  Suite results: " << (passed + failed) << ", Passed: " << passed << ", Failed: " << failed << std::endl;
        std::cout << std::endl;
        
        totalPassed += passed;
        totalFailed += failed;
    }
    
    std::cout << "Total tests: " << (totalPassed + totalFailed) << ", Passed: " << totalPassed << ", Failed: " << totalFailed << std::endl;
    
    return totalFailed > 0 ? 1 : 0;
}