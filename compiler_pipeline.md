# त्रिभाषा (Tribhasha) Compiler Pipeline

```
┌─────────────────┐     ┌─────────────────┐     ┌─────────────────┐
│   Source Code   │     │      Lexer      │     │     Parser      │
│  (Trilingual)   │ ──> │ (Tokenization)  │ ──> │  (Syntax Tree)  │
└─────────────────┘     └─────────────────┘     └─────────────────┘
        │                                                │
        │                                                │
        ▼                                                ▼
┌─────────────────┐     ┌─────────────────┐     ┌─────────────────┐
│     Code in     │     │  Abstract Syntax│     │    LLVM IR      │
│ English/Hindi/  │     │  Tree (AST)     │     │  Generation     │
│    Assamese     │     │                 │     │                 │
└─────────────────┘     └─────────────────┘     └─────────────────┘
                                                        │
                                                        │
                                                        ▼
┌─────────────────┐     ┌─────────────────┐     ┌─────────────────┐
│  Native Code    │     │  JIT Compiler   │     │  LLVM Optimizer │
│   Execution     │ <── │  (LLVM ORC)     │ <── │                 │
└─────────────────┘     └─────────────────┘     └─────────────────┘
```

## Example Flow

1. **Source Code** (Trilingual)
   ```
   function factorial(n) {    // English
       if (n <= 1) {
           return 1;
       }
       return n * factorial(n - 1);
   }
   
   फलन फैक्टोरियल(न) {       // Hindi
       अगर (न <= 1) {
           वापस 1;
       }
       वापस न * फैक्टोरियल(न - 1);
   }
   ```

2. **Lexer Output** (Tokenization)
   ```
   [FUNCTION_EN, "function"]
   [IDENTIFIER, "factorial"]
   [LEFT_PAREN, "("]
   [IDENTIFIER, "n"]
   [RIGHT_PAREN, ")"]
   [LEFT_BRACE, "{"]
   ...
   [FUNCTION_HI, "फलन"]
   [IDENTIFIER, "फैक्टोरियल"]
   ...
   ```

3. **Parser Output** (AST Structure)
   ```
   FunctionStmt {
     name: "factorial",
     params: ["n"],
     body: [
       IfStmt {
         condition: BinaryExpr {
           left: VariableExpr { name: "n" },
           operator: "<=",
           right: LiteralExpr { value: 1 }
         },
         thenBranch: ReturnStmt { value: LiteralExpr { value: 1 } },
         elseBranch: null
       },
       ReturnStmt {
         value: BinaryExpr {
           left: VariableExpr { name: "n" },
           operator: "*",
           right: CallExpr {
             callee: "factorial",
             arguments: [
               BinaryExpr {
                 left: VariableExpr { name: "n" },
                 operator: "-",
                 right: LiteralExpr { value: 1 }
               }
             ]
           }
         }
       }
     ]
   }
   ```

4. **LLVM IR Generation**
   ```llvm
   define double @factorial(double %n) {
   entry:
     %cmptmp = fcmp ole double %n, 1.000000e+00
     br i1 %cmptmp, label %then, label %else
     
   then:
     ret double 1.000000e+00
     
   else:
     %subtmp = fsub double %n, 1.000000e+00
     %calltmp = call double @factorial(double %subtmp)
     %multmp = fmul double %n, %calltmp
     ret double %multmp
   }
   ```

5. **JIT Compilation & Execution**
   - LLVM ORC JIT compiles IR to native machine code
   - Functions are directly callable from memory
   - No need for separate compilation step
   - Blazing-fast execution speed