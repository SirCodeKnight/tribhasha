# त्रिभाषा (Tribhasha) Installation Guide

This guide will help you install and build the त्रिभाषा programming language on your system.

## Prerequisites

To build त्रिभाषा, you'll need:

- C++ compiler with C++17 support (GCC 7+ or Clang 5+)
- LLVM 11.0 or later (with development headers)
- CMake 3.13 or later
- Git (for cloning the repository)

### Installing Prerequisites on Ubuntu/Debian

```bash
sudo apt update
sudo apt install build-essential cmake git
sudo apt install llvm-dev libllvm-11-ocaml-dev libllvm11 llvm-11 llvm-11-dev llvm-11-doc llvm-11-examples llvm-11-runtime
```

### Installing Prerequisites on macOS

```bash
brew install cmake llvm
```

### Installing Prerequisites on Windows

1. Install Visual Studio with C++ support
2. Install CMake: https://cmake.org/download/
3. Install LLVM: https://releases.llvm.org/download.html

## Building from Source

Clone the repository and build:

```bash
git clone https://github.com/SirCodeKnight/tribhasha.git
cd tribhasha
mkdir build && cd build
cmake ..
make -j$(nproc)  # On Linux/macOS
cmake --build .  # On Windows
```

## Installation

Install the built binary to your system:

```bash
sudo make install  # On Linux/macOS
cmake --build . --target INSTALL  # On Windows with admin privileges
```

This will install the `tribhasha` executable to your system.

## Verifying Installation

Test if the installation was successful:

```bash
tribhasha --version
```

You should see the version information for त्रिभाषा.

## Running the REPL

```bash
tribhasha
```

## Running a Script

```bash
tribhasha path/to/script.tri
```

## GitHub Upload Instructions

If you want to upload the source code to your own GitHub repository:

1. Create a new repository on GitHub: https://github.com/new
2. Initialize, add files, and push to your repository:

```bash
cd tribhasha  # Navigate to the source directory
git init
git add .
git commit -m "Initial commit of त्रिभाषा (Tribhasha) language"
git branch -M main
git remote add origin https://github.com/SirCodeKnight/tribhasha.git
git push -u origin main
```

## Troubleshooting

### LLVM Not Found

If CMake cannot find LLVM, you may need to specify the LLVM directory explicitly:

```bash
cmake -DLLVM_DIR=/path/to/llvm/lib/cmake/llvm ..
```

### Compilation Errors

If you encounter compilation errors, ensure you're using a compiler that supports C++17 and that you have the correct LLVM version installed.