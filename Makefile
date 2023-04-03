.PHONY: all
all: 
	g++ main.cpp Tree/tree.cpp Reader/reader.cpp Tree/debug.cpp Compiler/compiler_main.cpp Tokens/tokens.cpp Tokens/debug.cpp Onegin/onegin.cpp Compiler/asm_generate.cpp config.h TextToTokens/analyzer.cpp TextToTokens/analyzer.h Tokens/token_stk.cpp List/Functions/insertion.cpp List/Functions/indexes.cpp List/Functions/pointers.cpp List/Functions/otheropers.cpp List/Struct/list.cpp Compiler/compiler.cpp -fsanitize=address -o Language
