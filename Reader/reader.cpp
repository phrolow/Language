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

node *KeywordNode(KEYW keyw, SIDE side) {
    node *keywordnode = (node *) malloc(sizeof(node));

    token_t *token = NewToken(KEYWORD_TYPE, {.keyword = keyw}); //TODO:

    NodeCtor(keywordnode, NULL, token, side);

    return keywordnode;
}

token_t *require(token_stk_t *tokens, size_t *index, KEYW keyw) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    if(token->type != KEYWORD_TYPE || token->value.keyword != keyw) {
        printf("INVALID SYNTAX: at %llu required ", *index);

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

node *getStmt(token_stk_t *tokens, size_t *index, side side) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    node *stmt_child = NULL;

    if(token->type == VAR_TYPE) {
        token_t *first = token;
        (*index)++;
        token = TokensElem(tokens, *index);

        if(token->type == KEYWORD_TYPE && token->value.keyword == KEYW_ASSIGN) {
            (*index)--;

            node *var = getV(tokens, index, LEFT);
            node *assign = getToken(tokens, index, side);
            node *expression = getE(tokens, index, RIGHT);

            require(tokens, index, KEYW_DOTPOT);

            NodeConnect(assign,var);
            NodeConnect(assign,expression);

            stmt_child = assign;

            return stmt_child;
        }
        else {
            (*index)--;

            token - first;
        }
    }
    else if(token->type == KEYWORD_TYPE) {
        switch (token->value.keyword) {
            case KEYW_IF:
                return nullptr;
            default:
                printf("Чё бля...");
                assert(0);
        }
    }

    stmt_child = getE(tokens, index, side);

    require(tokens, index, KEYW_DOTPOT);

    return stmt_child;
}

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

node* getG(token_stk_t *tokens) {
    assert(tokens);

    size_t index = 0;

    node *root = getStmt(tokens, &index, ROOT);
    assert(root);

    if(tokens->tokens[index].value.keyword != KEYW_EOF) {
        printf("WRONG TOKEN No. %llu", index);

        assert(0);
    }

    return root;
}

tree* ReadExpression(char *txt) {
    tree *expression = (tree*) malloc(sizeof(tree));

    token_stk_t *tokens = NewTokenStk();

    GetTokens(txt, tokens);

    TreeCtor(expression, getG(tokens));

    return expression;
}