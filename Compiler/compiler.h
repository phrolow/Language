#ifndef LANGUAGE_COMPILER_H
#define LANGUAGE_COMPILER_H

#include "../Tree/tree.h"
#include "../Reader/reader.h"
#include "../Onegin/onegin.h"
#include "../Debug/debug.h"

const size_t MAX_VARS = 50;
const size_t MAX_ASM_LINE = 0x40;

struct Compiler {
    char var_table[NAME_MAX_LEN][MAX_VARS];
    Tree *code;
};

void PrintAsm(tree *code, const char *path, Compiler *compiler);

const char *GetOpFormat(enum OP op);

char *ToLine(token_t *token, char *line, Compiler *compiler);

int language_compile(const char *in, const char *out);

struct Compiler *NewCompiler(Tree *code);

size_t VarIndex(char *var, Compiler *compiler);

#endif //LANGUAGE_COMPILER_H
