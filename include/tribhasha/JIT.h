#ifndef TRIBHASHA_JIT_H
#define TRIBHASHA_JIT_H

#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <llvm/Support/Error.h>
#include <memory>
#include <string>

namespace tribhasha {

class TribhashaJIT {
private:
    // The ORC JIT instance
    std::unique_ptr<llvm::orc::LLJIT> lljit;
    
    // Constructor is private - use create() instead
    TribhashaJIT(std::unique_ptr<llvm::orc::LLJIT> lljit);
    
public:
    // Create a new JIT instance
    static llvm::Expected<std::unique_ptr<TribhashaJIT>> create();
    
    // Add a module to the JIT
    llvm::Error addModule(std::unique_ptr<llvm::Module> module);
    
    // Look up a symbol in the JIT
    llvm::Expected<llvm::JITEvaluatedSymbol> lookup(const std::string& name);
    
    // Execute the main function
    llvm::Error executeMain();
    
    // Get the raw pointer to the LLJIT
    llvm::orc::LLJIT* getJIT() const;
};

} // namespace tribhasha

#endif // TRIBHASHA_JIT_H