#include "tribhasha/Lexer.h"
#include "tribhasha/Parser.h"
#include "tribhasha/CodeGen.h"
#include "tribhasha/JIT.h"
#include "tribhasha/REPL.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>

using namespace tribhasha;

void printUsage(const std::string& programName) {
    std::cout << "Usage: " << programName << " [options] [file]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h, --help          Print this help message" << std::endl;
    std::cout << "  -v, --version       Print version information" << std::endl;
    std::cout << "  -t, --tokens        Print tokens (requires file)" << std::endl;
    std::cout << "  -a, --ast           Print AST (requires file)" << std::endl;
    std::cout << "  -e, --execute       Execute the file (default)" << std::endl;
    std::cout << "If no file is provided, the REPL will start." << std::endl;
}

void printVersion() {
    std::cout << "त्रिभाषा (Tribhasha) Programming Language" << std::endl;
    std::cout << "Version 0.1.0" << std::endl;
    std::cout << "Copyright (c) 2025 रायन तामुली (Raayan Tamuly)" << std::endl;
}

bool executeFile(const std::string& filename) {
    // Read the file
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    
    try {
        // Tokenize
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.scanTokens();
        
        // Parse
        Parser parser(tokens);
        std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
        
        // Generate code
        CodeGen codegen;
        codegen.generate(statements);
        
        // Get the module
        auto module = codegen.getModule();
        
        // Create JIT
        auto jitResult = TribhashaJIT::create();
        if (!jitResult) {
            std::cerr << "Error creating JIT" << std::endl;
            return false;
        }
        
        auto jit = std::move(*jitResult);
        
        // Add the module to the JIT
        auto err = jit->addModule(std::move(module));
        if (err) {
            std::cerr << "Error adding module to JIT" << std::endl;
            return false;
        }
        
        // Execute the main function
        err = jit->executeMain();
        if (err) {
            std::cerr << "Error executing code" << std::endl;
            return false;
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

void printTokens(const std::string& filename) {
    // Read the file
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    
    // Tokenize
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    
    // Print tokens
    for (const auto& token : tokens) {
        std::cout << token.toString() << std::endl;
    }
}

void printAST(const std::string& filename) {
    // Read the file
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    
    try {
        // Tokenize
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.scanTokens();
        
        // Parse
        Parser parser(tokens);
        std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
        
        // Print AST
        std::cout << "AST with " << statements.size() << " statements" << std::endl;
        // TODO: Implement a proper AST printer
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // Initialize keyword maps
    Keywords::initialize();
    
    // If no arguments, start REPL
    if (argc == 1) {
        REPL repl;
        repl.run();
        return 0;
    }
    
    // Parse arguments
    std::string filename;
    bool printTokensFlag = false;
    bool printASTFlag = false;
    bool executeFlag = true;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            printVersion();
            return 0;
        } else if (arg == "-t" || arg == "--tokens") {
            printTokensFlag = true;
            executeFlag = false;
        } else if (arg == "-a" || arg == "--ast") {
            printASTFlag = true;
            executeFlag = false;
        } else if (arg == "-e" || arg == "--execute") {
            executeFlag = true;
        } else if (arg[0] == '-') {
            std::cerr << "Unknown option: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        } else {
            filename = arg;
        }
    }
    
    // Check if a filename was provided for token/AST printing
    if ((printTokensFlag || printASTFlag) && filename.empty()) {
        std::cerr << "Error: File required for --tokens or --ast options" << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    // Execute the requested actions
    if (printTokensFlag) {
        printTokens(filename);
    }
    
    if (printASTFlag) {
        printAST(filename);
    }
    
    if (executeFlag && !filename.empty()) {
        if (!executeFile(filename)) {
            return 1;
        }
    }
    
    return 0;
}