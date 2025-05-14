#ifndef TRIBHASHA_REPL_H
#define TRIBHASHA_REPL_H

#include "Lexer.h"
#include "Parser.h"
#include "CodeGen.h"
#include "JIT.h"
#include <string>
#include <memory>
#include <vector>

namespace tribhasha {

class REPL {
private:
    // JIT compiler
    std::unique_ptr<TribhashaJIT> jit;
    
    // Keep track of history
    std::vector<std::string> history;
    
    // Helper methods
    std::string readLine(const std::string& prompt);
    void printTokens(const std::vector<Token>& tokens);
    void printAST(const std::vector<std::shared_ptr<Stmt>>& statements);
    
    // ANSI colors for syntax highlighting
    static const std::string RESET;
    static const std::string RED;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string BLUE;
    static const std::string MAGENTA;
    static const std::string CYAN;
    static const std::string WHITE;
    
    // Syntax highlighting helpers
    std::string highlightSyntax(const std::string& source);
    std::string getColorForToken(const Token& token);
    
public:
    REPL();
    
    // Run the REPL
    void run();
    
    // Execute a single line of code
    void executeLine(const std::string& line);
    
    // Execute a file
    void executeFile(const std::string& filename);
};

} // namespace tribhasha

#endif // TRIBHASHA_REPL_H