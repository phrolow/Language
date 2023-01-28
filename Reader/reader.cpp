#include "reader.h"

#define DEF_CMD(name, num, sign, ...)   \
    if(streq(*ptr, sign)) {             \
        op = name;                      \
        (*ptr) += strlen(sign);         \
    } else

#define KILL {                                                      \
    printf("INVALID EXPRESSION: %c must not be there\n", **ptr);    \
                                                                    \
    assert(0);                                                      \
}

#define SKIP_SPACES(ptr) \
    while(isspace(*ptr))  \
        (ptr)++

#define NEWNODE(name, type, val, side)          \
    node *name = (node*) malloc(sizeof(node));  \
    token_t *token = NewToken(type, val);       \
    NodeCtor(name, NULL, token, side);          \
    printf("%lg\n", token->value.num)



node* getN(const char **ptr, side side) {
    assert(ptr && *ptr);

    if(!isdigit(**ptr)) KILL

    double val = 0;

    while(isdigit(**ptr)) {
        val = val * 10 + **ptr - '0';

        (*ptr)++;
    }

    NEWNODE(newnode, NUMERAL, {.num = val}, side);

    SKIP_SPACES(*ptr);

    return newnode;
}

node* getV(const char **ptr, side side) {
    assert(ptr && *ptr);

    if(isalpha(**ptr)) {
        int i = 0;
        char name[NAME_MAX_LEN] = {};

        do {
            name[i++] = **ptr;

            (*ptr)++;
        } while(isalpha(**ptr));

        NEWNODE(newnode, VAR, {.name = {}}, side);

        strcpy(newnode->val->value.name, name);

        return newnode;
    }

    SKIP_SPACES(*ptr);

    return getN(ptr, side);
}

node* getPow(const char **ptr, side side) {
    assert(ptr && *ptr);

    node *left = getP(ptr, side);

    SKIP_SPACES(*ptr);

    while(**ptr == '^') {
        left->side = LEFT;

        (*ptr)++;

        SKIP_SPACES(*ptr);

        node *right = getP(ptr, RIGHT);

        NEWNODE(nod, OP, {.op = POW}, side);

        NodeConnect(nod, left);
        NodeConnect(nod, right);

        left = nod;
    }

    SKIP_SPACES(*ptr);

    return left;
}

node* getT(const char **ptr, side side) {
    assert(ptr && *ptr);

    node *left = getPow(ptr, side);

    SKIP_SPACES(*ptr);

    while(**ptr == '*' || **ptr == '/') {
        enum OP op;

        left->side = LEFT;

        if(**ptr == '*')
            op = MUL;
        else
            op = DIV;

        (*ptr)++;

        SKIP_SPACES(*ptr);

        node *right = getPow(ptr, RIGHT);

        NEWNODE(nod, OP, {.op = op}, side);

        NodeConnect(nod, left);
        NodeConnect(nod, right);

        left = nod;
    }

    SKIP_SPACES(*ptr);

    return left;
}

node* getE(const char **ptr, side side) {
    assert(ptr && *ptr);

    enum OP op;
    node *left = getT(ptr, side);

    SKIP_SPACES(*ptr);

    while(**ptr == '+' || **ptr == '-') {
        left->side = LEFT;

        if(**ptr == '+')
            op = ADD;
        else
            op = SUB;

        (*ptr)++;

        SKIP_SPACES(*ptr);

        node *right = getT(ptr, RIGHT);

        NEWNODE(nod, OP, {.op = op}, side);

        NodeConnect(nod, left);
        NodeConnect(nod, right);

        left = nod;
    }

    SKIP_SPACES(*ptr);

    return left;
}

node *getAssign(const char **ptr, side side) {
    assert(ptr && *ptr);

    node *left = getV(ptr, LEFT);

    SKIP_SPACES(*ptr);

    if(**ptr != '=') KILL;

    NEWNODE(assign, OP, {.op = ASSIGN}, side);

    (*ptr)++;

    SKIP_SPACES(*ptr);

    node *right = getE(ptr, RIGHT);

    NodeConnect(assign, left);
    NodeConnect(assign, right);

    SKIP_SPACES(*ptr);



    return assign;
}

node* getP(const char **ptr, side side) {
    assert(ptr && *ptr);

    SKIP_SPACES(*ptr);

    if(**ptr == '(') {
        (*ptr)++;

        SKIP_SPACES(*ptr);

        node *nod = getE(ptr, side);

        if(**ptr != ')') KILL

        (*ptr)++;

        SKIP_SPACES(*ptr);

        return nod;
    }
    else
        return getV(ptr, side);
}

node *getStmts(const char **ptr, side side) {
    assert(ptr && *ptr);

    node *left = getAssign(ptr, side);

    SKIP_SPACES(*ptr);

    while(**ptr == ';') {
        left->side = LEFT;

        (*ptr)++;

        SKIP_SPACES(*ptr);

        node *right = getAssign(ptr, RIGHT);

        NEWNODE(nod, OP, {.op = STMT}, side);

        NodeConnect(nod, left);
        NodeConnect(nod, right);

        left = nod;
    }

    SKIP_SPACES(*ptr);

    return left;
}

node* getG(const char *expression) {
    assert(expression);

    const char **ptr = &expression;

    SKIP_SPACES(*ptr);

    node *root = getStmts(ptr, ROOT);
    assert(root);

    if(*expression != '\0') KILL

    return root;
}

tree* ReadExpression(const char *txt) {
    tree *expression = (tree*) malloc(sizeof(tree));

    TreeCtor(expression, getG(txt));

    return expression;
}