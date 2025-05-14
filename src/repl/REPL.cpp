#include "tribhasha/REPL.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <cctype>
#include <llvm/Support/raw_ostream.h>

namespace tribhasha {

// ANSI color codes for syntax highlighting
const std::string REPL::RESET = "\033[0m";
const std::string REPL::RED = "\033[31m";
const std::string REPL::GREEN = "\033[32m";
const std::string REPL::YELLOW = "\033[33m";
const std::string REPL::BLUE = "\033[34m";
const std::string REPL::MAGENTA = "\033[35m";
const std::string REPL::CYAN = "\033[36m";
const std::string REPL::WHITE = "\033[37m";

REPL::REPL() {
    // Create JIT instance
    auto jitResult = TribhashaJIT::create();
    if (!jitResult) {
        std::cerr << "Error creating JIT: ";
        llvm::handleAllErrors(jitResult.takeError(), [](const llvm::ErrorInfoBase& error) {
            error.log(llvm::errs());
        });
        exit(1);
    }
    
    jit = std::move(*jitResult);
}

void REPL::run() {
    std::cout << "त्रिभाषा (Tribhasha) Programming Language" << std::endl;
    std::cout << "Version 0.1.0 - REPL Mode" << std::endl;
    std::cout << "Type 'exit' to quit, 'help' for assistance." << std::endl;
    
    std::string line;
    bool running = true;
    
    while (running) {
        line = readLine("tribhasha> ");
        
        if (line == "exit" || line == "quit") {
            running = false;
        } else if (line == "help") {
            std::cout << "Available commands:\n"
                      << "  help - Show this help message\n"
                      << "  exit/quit - Exit the REPL\n"
                      << "  clear - Clear the screen\n"
                      << "  history - Show command history\n"
                      << "  load <filename> - Load and execute a file\n"
                      << "  tokens <code> - Show tokens for code\n"
                      << "  ast <code> - Show AST for code\n"
                      << std::endl;
        } else if (line == "clear") {
            // Clear screen (works on most terminals)
            std::cout << "\033[2J\033[1;1H";
        } else if (line == "history") {
            for (size_t i = 0; i < history.size(); i++) {
                std::cout << i << ": " << history[i] << std::endl;
            }
        } else if (line.substr(0, 5) == "load ") {
            std::string filename = line.substr(5);
            executeFile(filename);
        } else if (line.substr(0, 7) == "tokens ") {
            std::string code = line.substr(7);
            Lexer lexer(code);
            std::vector<Token> tokens = lexer.scanTokens();
            printTokens(tokens);
        } else if (line.substr(0, 4) == "ast ") {
            std::string code = line.substr(4);
            Lexer lexer(code);
            std::vector<Token> tokens = lexer.scanTokens();
            Parser parser(tokens);
            try {
                std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
                printAST(statements);
            } catch (const ParseError& error) {
                std::cerr << "Parse error: " << error.what() << std::endl;
            }
        } else if (!line.empty()) {
            executeLine(line);
            history.push_back(line);
        }
    }
}

void REPL::executeLine(const std::string& line) {
    try {
        // Print the code with syntax highlighting
        std::cout << highlightSyntax(line) << std::endl;
        
        // Tokenize
        Lexer lexer(line);
        std::vector<Token> tokens = lexer.scanTokens();
        
        // Parse
        Parser parser(tokens);
        std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
        
        // Generate code
        CodeGen codegen;
        codegen.generate(statements);
        
        // Get the module
        auto module = codegen.getModule();
        
        // Add the module to the JIT
        auto err = jit->addModule(std::move(module));
        if (err) {
            std::cerr << "Error adding module to JIT: ";
            llvm::handleAllErrors(std::move(err), [](const llvm::ErrorInfoBase& error) {
                error.log(llvm::errs());
            });
            return;
        }
        
        // Execute the main function
        err = jit->executeMain();
        if (err) {
            std::cerr << "Error executing code: ";
            llvm::handleAllErrors(std::move(err), [](const llvm::ErrorInfoBase& error) {
                error.log(llvm::errs());
            });
            return;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void REPL::executeFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    
    executeLine(content);
}

std::string REPL::readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

void REPL::printTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        std::cout << token.toString() << std::endl;
    }
}

void REPL::printAST(const std::vector<std::shared_ptr<Stmt>>& statements) {
    // Simple representation of the AST
    std::cout << "AST with " << statements.size() << " statements" << std::endl;
    // TODO: Implement a proper AST printer
}

std::string REPL::highlightSyntax(const std::string& source) {
    // Tokenize the source
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    
    std::string result;
    int lastPos = 0;
    
    for (const auto& token : tokens) {
        // Add any whitespace before this token
        int tokenStart = source.find(token.lexeme, lastPos);
        if (tokenStart != std::string::npos && tokenStart > lastPos) {
            result += source.substr(lastPos, tokenStart - lastPos);
        }
        
        // Add the colored token
        result += getColorForToken(token) + token.lexeme + RESET;
        
        // Update the lastPos
        lastPos = tokenStart + token.lexeme.length();
    }
    
    // Add any remaining text
    if (lastPos < static_cast<int>(source.length())) {
        result += source.substr(lastPos);
    }
    
    return result;
}

std::string REPL::getColorForToken(const Token& token) {
    switch (token.type) {
        // Keywords - English
        case TokenType::VAR_EN:
        case TokenType::FUNCTION_EN:
        case TokenType::IF_EN:
        case TokenType::ELSE_EN:
        case TokenType::WHILE_EN:
        case TokenType::FOR_EN:
        case TokenType::RETURN_EN:
            return BLUE;
            
        // Keywords - Hindi
        case TokenType::VAR_HI:
        case TokenType::FUNCTION_HI:
        case TokenType::IF_HI:
        case TokenType::ELSE_HI:
        case TokenType::WHILE_HI:
        case TokenType::FOR_HI:
        case TokenType::RETURN_HI:
            return MAGENTA;
            
        // Keywords - Assamese
        case TokenType::VAR_AS:
        case TokenType::FUNCTION_AS:
        case TokenType::IF_AS:
        case TokenType::ELSE_AS:
        case TokenType::WHILE_AS:
        case TokenType::FOR_AS:
        case TokenType::RETURN_AS:
            return CYAN;
            
        // Boolean literals
        case TokenType::TRUE_EN:
        case TokenType::TRUE_HI:
        case TokenType::TRUE_AS:
        case TokenType::FALSE_EN:
        case TokenType::FALSE_HI:
        case TokenType::FALSE_AS:
            return YELLOW;
            
        // Other literals
        case TokenType::INT_LITERAL:
        case TokenType::FLOAT_LITERAL:
            return GREEN;
            
        case TokenType::STRING_LITERAL:
            return RED;
            
        // Identifiers
        case TokenType::IDENTIFIER:
            return WHITE;
            
        // Operators and delimiters
        default:
            return RESET;
    }
}

} // namespace tribhasha