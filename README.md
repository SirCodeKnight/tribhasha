# त्रिभाषा (Tribhasha) - A Trilingual Programming Language

[![GitHub stars](https://img.shields.io/github/stars/SirCodeKnight/tribhasha.svg)](https://github.com/SirCodeKnight/tribhasha/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/SirCodeKnight/tribhasha.svg)](https://github.com/SirCodeKnight/tribhasha/network)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

त्रिभाषा (Tribhasha) is a revolutionary programming language that supports natural code writing in Assamese, Hindi, and English. Built with a blazing-fast LLVM-based JIT compiler, it brings regional language support to modern programming.

## Features

- **Trilingual Support**: Write code seamlessly in Assamese, Hindi, and English
- **LLVM JIT Compilation**: Blazing-fast performance with LLVM's ORC JIT API
- **Elegant Syntax**: Clean, minimal syntax inspired by Python and JavaScript
- **Interactive REPL**: Syntax-highlighted command-line interface for interactive development
- **Full Language Features**: Variables, functions, loops, conditionals, and basic types in all three languages

## Example Code

```tribhasha
// English syntax
function factorial(n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

// Hindi syntax
फलन फैक्टोरियल(न) {
    अगर (न <= 1) {
        वापस 1;
    }
    वापस न * फैक्टोरियल(न - 1);
}

// Assamese syntax
কাৰ্য্য ফেক্টৰিয়েল(ন) {
    যদি (ন <= 1) {
        ঘূৰাই_দিয়ক 1;
    }
    ঘূৰাই_দিয়ক ন * ফেক্টৰিয়েল(ন - 1);
}
```

## Getting Started

### Prerequisites

- C++ compiler with C++17 support (GCC 7+ or Clang 5+)
- LLVM 11.0 or later
- CMake 3.13 or later

### Building from Source

```bash
git clone https://github.com/SirCodeKnight/tribhasha.git
cd tribhasha
mkdir build && cd build
cmake ..
make
```

## Usage

### Interactive REPL

```bash
./tribhasha
```

### Running a Script

```bash
./tribhasha path/to/script.tri
```

## Language Documentation

See the [documentation](./docs/LANGUAGE.md) for detailed information about the language syntax and features.

## Project Structure

```
tribhasha/
├── src/
│   ├── lexer/         # Tokenization and lexical analysis
│   ├── parser/        # Parsing and AST generation
│   ├── ast/           # Abstract Syntax Tree definitions
│   ├── codegen/       # LLVM IR code generation
│   ├── jit/           # JIT compilation using LLVM ORC
│   ├── repl/          # Interactive REPL implementation
│   └── main.cpp       # Entry point
├── include/           # Header files
├── docs/              # Documentation
├── tests/             # Test cases
└── examples/          # Example programs
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Author

- **रायन तामुली (Raayan Tamuly)** - [GitHub](https://github.com/SirCodeKnight)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- LLVM Project and community
- The multilingual programming community
- All regional language developers

---

Keywords: multilingual programming language, trilingual compiler, LLVM, JIT compiler, Assamese programming, Hindi programming, regional language coding, compiler design, language design, LLVM ORC