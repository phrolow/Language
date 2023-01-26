#ifndef LANGUAGE_COMPILER_H
#define LANGUAGE_COMPILER_H

#include "../Tree/tree.h"
#include "../Reader/reader.h"
#include "../Onegin/onegin.h"
#include "../Debug/debug.h"

const size_t MAX_ASM_LINE = 0x40;

void *PrintAsm(tree *code, const char *path);

const char *GetOpFormat(enum OP op);

char *ToLine(token_t *token, char *line);

#endif //LANGUAGE_COMPILER_H
