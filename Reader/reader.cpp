#include "reader.h"

#define KILL {                                                      \
    printf("WRONG TOKEN No. %llu\n", *index);                       \
                                                                    \
    assert(0);                                                      \
}

#define NEWNODE(name, token, side)          \
    node *name = (node*) malloc(sizeof(node));  \
    NodeCtor(name, NULL, token, side);

#define DEF_KEYW(name, num, sign)          \
            case KEYW_##name:                  \
                printf(#sign);  \
                break;

#define DEF_OPER(name, num, sign)                   \
            case KEYW_##name:                      \
                printf(#sign);     \
                break;

#define DEF_HELP(name, num, sign)                   \
            case KEYW_##name:                      \
                printf("%c\n", sign);      \
                break;


token_t *require(token_stk_t *tokens, size_t *index, KEYW keyw) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    if(token->type != KEYWORD_TYPE || token->value.keyword != keyw) {
        printf("INVALID SYNTAX: at %llu required ");

        switch (keyw) {
            #include "../keywords.h"
            #include "../operators.h"
            default:
                printf("... what?)");
                break;
        }

        #undef DEF_KEYW
        #undef DEF_OPER
        #undef DEF_HELP

        assert(0);
    }

    (*index)++;

    return token;
}

node *getToken(token_stk_t *tokens, size_t *index, side side) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    NEWNODE(newnode, token, side);

    (*index)++;

    return newnode;
}

node* getN(token_stk_t *tokens, size_t *index, side side) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    if(token->type != NUM_TYPE) KILL

    return getToken(tokens, index, side);
}

node* getV(token_stk_t *tokens, size_t *index, side side) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    if (token->type == VAR_TYPE)
        return getToken(tokens, index, side);

    return getN(tokens, index, side);
}

node* getPow(token_stk_t *tokens, size_t *index, side side) {
    assert(tokens && tokens->tokens && index);

    node *left = getP(tokens, index, side);

    token_t *token = TokensElem(tokens, *index);

    while(token->value.keyword == KEYW_POW) {
        node *nod = getToken(tokens, index, side);

        left->side = LEFT;

        node *right = getP(tokens, index, RIGHT);

        NodeConnect(nod, left);
        NodeConnect(nod, right);

        left = nod;
        token = TokensElem(tokens, *index);
    }

    return left;
}

node* getT(token_stk_t *tokens, size_t *index, side side) {
    assert(tokens && tokens->tokens && index);

    node *left = getPow(tokens, index, side);

    token_t *token = TokensElem(tokens, *index);

    while(token->value.keyword == KEYW_MUL || token->value.keyword == KEYW_DIV) {
        node *nod = getToken(tokens, index, side);

        left->side = LEFT;

        node *right = getPow(tokens, index, RIGHT);

        NodeConnect(nod, left);
        NodeConnect(nod, right);

        left = nod;
        token = TokensElem(tokens, *index);
    }

    return left;
}

node* getE(token_stk_t *tokens, size_t *index, side side) {
    assert(tokens && tokens->tokens && index);

    node *left = getT(tokens, index, side);

    token_t *token = TokensElem(tokens, *index);

    while(token->value.keyword == KEYW_ADD || token->value.keyword == KEYW_SUB) {
        node *nod = getToken(tokens, index, side);

        left->side = LEFT;

        node *right = getT(tokens, index, RIGHT);

        NodeConnect(nod, left);
        NodeConnect(nod, right);

        left = nod;
        token = TokensElem(tokens, *index);
    }

    return left;
}

//node *getAssign(token_stk_t *tokens, size_t *index, side side) {
//    assert(tokens && tokens->tokens && index);
//
//    node *left = getV(ptr, LEFT);
//
//    SKIP_SPACES(*ptr);
//
//    if(**ptr != '=') KILL;
//
//    NEWNODE(assign, KEYWORD_TYPE, {.keyword = KEYW_ASSIGN}, side);
//
//    (*ptr)++;
//
//    SKIP_SPACES(*ptr);
//
//    node *right = getE(ptr, RIGHT);
//
//    NodeConnect(assign, left);
//    NodeConnect(assign, right);
//
//    SKIP_SPACES(*ptr);
//
//
//
//    return assign;
//}

node* getP(token_stk_t *tokens, size_t *index, side side) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    if(token->type != KEYWORD_TYPE || token->value.keyword != KEYW_OPRND)
        return getV(tokens, index, side);

    (*index)++;

    node *nod = getE(tokens, index, side);

    require(tokens, index, KEYW_CLRND);

    return nod;
}

//node *getStmts(token_stk_t *tokens, size_t *index, side side) {
//    assert(tokens && tokens->tokens && index);
//
//    node *left = getAssign(ptr, side);
//
//    SKIP_SPACES(*ptr);
//
//    while(**ptr == ';') {
//        left->side = LEFT;
//
//        (*ptr)++;
//
//        SKIP_SPACES(*ptr);
//
//        node *right = getAssign(ptr, RIGHT);
//
//        NEWNODE(nod, KEYWORD_TYPE, {.keyword = KEYW_STMT}, side);
//
//        NodeConnect(nod, left);
//        NodeConnect(nod, right);
//
//        left = nod;
//    }
//
//    SKIP_SPACES(*ptr);
//
//    return left;
//}

node* getG(token_stk_t *tokens) {
    assert(tokens);

    size_t index = 0;

    node *root = getE(tokens, &index, ROOT);
    assert(root);

    if(tokens->tokens[index].value.keyword != KEYW_EOF) {
        printf("WRONG TOKEN No. %llu", index);

        assert(0);
    }

    return root;
}

tree* ReadExpression(const char *txt) {
    tree *expression = (tree*) malloc(sizeof(tree));

    token_stk_t *tokens = NewTokenStk();



    TreeCtor(expression, getG(tokens));

    return expression;
}