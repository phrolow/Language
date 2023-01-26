#ifndef LANGUAGE_COMPILER_H
#define LANGUAGE_COMPILER_H

#include "../Tree/tree.h"
#include "../Reader/reader.h"
#include "../Onegin/onegin.h"
#include "../Debug/debug.h"

const char * const 

char *ToAsm(tree *code);

char *ToLine(token_t *token);

#endif //LANGUAGE_COMPILER_H
