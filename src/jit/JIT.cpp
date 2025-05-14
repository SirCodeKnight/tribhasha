#include "tribhasha/JIT.h"
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/TargetSelect.h>
#include <iostream>

namespace tribhasha {

// Constructor
TribhashaJIT::TribhashaJIT(std::unique_ptr<llvm::orc::LLJIT> lljit)
    : lljit(std::move(lljit)) {}

// Create a new JIT instance
llvm::Expected<std::unique_ptr<TribhashaJIT>> TribhashaJIT::create() {
    // Initialize LLVM targets
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
    
    // Create an LLJIT instance
    auto lljit = llvm::orc::LLJITBuilder().create();
    if (!lljit) {
        return lljit.takeError();
    }
    
    return std::unique_ptr<TribhashaJIT>(new TribhashaJIT(std::move(*lljit)));
}

// Add a module to the JIT
llvm::Error TribhashaJIT::addModule(std::unique_ptr<llvm::Module> module) {
    // Create a ThreadSafeModule to add to the JIT
    auto threadSafeModule = llvm::orc::ThreadSafeModule(
        std::move(module),
        std::make_unique<llvm::LLVMContext>()
    );
    
    // Add the module to the JIT
    return lljit->addIRModule(std::move(threadSafeModule));
}

// Look up a symbol in the JIT
llvm::Expected<llvm::JITEvaluatedSymbol> TribhashaJIT::lookup(const std::string& name) {
    return lljit->lookup(name);
}

// Execute the main function
llvm::Error TribhashaJIT::executeMain() {
    // Look up the 'main' function
    auto mainSymbol = lookup("main");
    if (!mainSymbol) {
        return mainSymbol.takeError();
    }
    
    // Cast the symbol address to a function pointer
    auto* mainFunc = reinterpret_cast<int(*)()>(mainSymbol->getAddress());
    
    // Call the function
    int result = mainFunc();
    
    // Check for non-zero return code
    if (result != 0) {
        return llvm::createStringError(
            llvm::inconvertibleErrorCode(),
            "main function returned non-zero code: " + std::to_string(result)
        );
    }
    
    return llvm::Error::success();
}

// Get the raw pointer to the LLJIT
llvm::orc::LLJIT* TribhashaJIT::getJIT() const {
    return lljit.get();
}

} // namespace tribhasha