cmake_minimum_required(VERSION 3.23)
project(Language)

set(CMAKE_CXX_STANDARD 14)

add_executable(Language main.cpp Tree/tree.cpp Reader/reader.cpp Debug/debug.cpp Compiler/compiler_main.cpp Tokens/tokens.cpp Onegin/onegin.cpp Compiler/asm_generate.cpp config.h TextToTokens/analyzer.cpp TextToTokens/analyzer.h Tokens/token_stk.cpp List/Functions/insertion.cpp List/Functions/indexes.cpp List/Functions/pointers.cpp List/Functions/otheropers.cpp List/Struct/list.cpp Compiler/compiler.cpp)
