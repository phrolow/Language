#ifndef LANGUAGE_TOKENS_H
#define LANGUAGE_TOKENS_H

#include <ctype.h>
#include <stdlib.h>

#define DEF_CMD(name, num, sign, ...) name = num,

const size_t NAME_MAX_LEN = 8;
const double TOKEN_POISON = 0xDEAD;

enum TYPE {
    NOT_DEFINED = 0,
    NUMERAL,
    VAR,
    OP,
};

enum OP {
    #include "../codegen.h"
    #undef DEF_CMD
};

enum TOKEN_ERROR {
    NULLPTR,
    NO_TYPE,
    BAD_VAL,
};

union Value {
    enum OP op;
    double num;
    char name[NAME_MAX_LEN];
};

struct token_t {
    enum TYPE type;

    Value value;
};

token_t *NewToken(TYPE type, Value value);

void TokenDtor(token_t *token);

int TokenCheck(token_t *token);

#endif //LANGUAGE_TOKENS_H
