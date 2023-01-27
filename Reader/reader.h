#ifndef LANGUAGE_READER_H
#define LANGUAGE_READER_H

#include "../Onegin/onegin.h"
#include "../Tree/tree.h"
#include "../Tokens/tokens.h"
#include <ctype.h>

node* getP(const char **ptr, side side);

node* getN(const char **ptr, side side);

node* getV(const char **ptr, side side);

node* getPow(const char **ptr, side side);

node* getT(const char **ptr, side side);

node* getE(const char **ptr, side side);

node* getG(const char *expression);

tree* ReadExpression(const char *txt);

#endif //LANGUAGE_READER_H
