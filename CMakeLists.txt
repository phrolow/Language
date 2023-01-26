cmake_minimum_required(VERSION 3.23)
project(Language)

set(CMAKE_CXX_STANDARD 14)

add_executable(Language Tree/tree.cpp Reader/reader.cpp Debug/debug.cpp Compiler/compiler_main.cpp Tokens/tokens.cpp Onegin/onegin.cpp Compiler/asm_generate.cpp)
