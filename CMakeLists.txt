cmake_minimum_required(VERSION 3.23)
project(Language)

set(CMAKE_CXX_STANDARD 14)

add_executable(Language Tree/tree.cpp Reader/reader.cpp Debug/debug.cpp Compiler/compiler_main.cpp Tokens/tokens.cpp Onegin/onegin.cpp Compiler/asm_generate.cpp SoftCPU/asm/asm.cpp SoftCPU/asm/asm_main.cpp SoftCPU/cpu/cpu.cpp SoftCPU/cpu/cpu_main.cpp SoftCPU/cpu/stack/hash.cpp SoftCPU/cpu/stack/log.cpp SoftCPU/cpu/stack/other.cpp SoftCPU/cpu/stack/stack.cpp  main.cpp config.h TextToTokens/analyzer.cpp TextToTokens/analyzer.h Tokens/token_stk.cpp)
