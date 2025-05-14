cmake_minimum_required(VERSION 3.13)

# Add test executable
add_executable(tribhasha_tests
    LexerTests.cpp
    ParserTests.cpp
    TestMain.cpp
)

# Link against project code
target_link_libraries(tribhasha_tests ${llvm_libs})

# Add tests
add_test(NAME LexerTests COMMAND tribhasha_tests lexer)
add_test(NAME ParserTests COMMAND tribhasha_tests parser)