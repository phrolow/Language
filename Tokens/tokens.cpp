#include "tokens.h"

#define CHECK(cond, err) (cond) ? 0 : (err)

token_t *NewToken(TYPE type, Value value) {
    token_t *token = (token_t*) malloc(sizeof(token));

    token->type = type;
    token->value = value;

    return token;
}

void TokenDtor(token_t *token) {
    token->type = NOT_DEFINED;
    token->value.num = POISON;

    free(token);
}

int TokenCheck(token_t *token) {
    int res = 0;

    res |= CHECK(!token, NULLPTR);
    res |= CHECK(token->type == NOT_DEFINED, NO_TYPE);
    res |= CHECK(token->value.num == POISON, BAD_VAL);

    return res;
}