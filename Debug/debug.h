#ifndef LANGUAGE_DEBUG_H
#define LANGUAGE_DEBUG_H

#include "../Tree/tree.h"

#define PRINT(n) printf("%d %d\n", (n)->type, (n)->value)

const int WRONG = 0xBAD;

const char* const    PNGPATH = "tree.png";
const char* const    HTMLPATH = "log.html";
const char* const    DOTPATH = "tree";

const size_t COLORLEN = 8;

const char  OP_COLOR[COLORLEN] = "#F4CF72",
        CONST_COLOR[COLORLEN] = "#71DCC1",
        VAR_COLOR[COLORLEN] = "#F49872",
        MCONST_COLOR[COLORLEN] = "#B0C4DE",
        ERR_COLOR[COLORLEN] = "#808080",
        RIGHT_COLOR[COLORLEN] = "#7173DC",
        LEFT_COLOR[COLORLEN] = "#CD5C5C";

int NodeVerify(node *node);

void TreeDump(tree *tree);

#endif //LANGUAGE_DEBUG_H
