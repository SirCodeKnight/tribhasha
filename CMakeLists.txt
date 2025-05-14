cmake_minimum_required(VERSION 3.13)
project(tribhasha VERSION 0.1.0 LANGUAGES CXX)

# C++17 is required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

# Print LLVM information
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Include LLVM headers
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Include project headers
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/lexer/Lexer.cpp
    src/parser/Parser.cpp
    src/codegen/CodeGen.cpp
    src/jit/JIT.cpp
    src/repl/REPL.cpp
)

# Create the executable
add_executable(tribhasha ${SOURCES})

# Link against LLVM libraries
llvm_map_components_to_libnames(llvm_libs
    core
    orcjit
    native
    support
)

target_link_libraries(tribhasha ${llvm_libs})

# Installation
install(TARGETS tribhasha DESTINATION bin)
install(DIRECTORY include/ DESTINATION include/tribhasha FILES_MATCHING PATTERN "*.h")

# Examples directory
add_subdirectory(examples)

# Enable testing
enable_testing()
add_subdirectory(tests)