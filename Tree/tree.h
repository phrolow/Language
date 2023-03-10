#ifndef LANGUAGE_TREE_H
#define LANGUAGE_TREE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../Tokens/tokens.h"

typedef token_t* value_t;

typedef enum SIDE {
    ROOT = -1,
    LEFT = 0,
    RIGHT = 1,
} side;

typedef struct Node {
    struct Node *parent;
    struct Node *children[2];
    value_t val;
    side side;
} node;

typedef struct Tree {
    node *root;
} tree;

#endif //LANGUAGE_TREE_H

void TreeCtor(tree *tree, node *node);

node* NodeCopy(node *original);

void TreeDtor(tree *tree);

void SubTreeDtor(node *node);

int NodeCtor(node *new_node, node *parent, value_t val, side side);

void NodeDtor(node *nod);

void NodeConnect(node *parent, node *child);