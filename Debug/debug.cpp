#include "debug.h"

#include "../Tree/tree.h"

#define CHECK(cond, err, msg)   \
    if(!(cond)) {                  \
        printf(msg);            \
        return err;             \
    }


//int NodeVerify(node *node) {
//    return TREE_OK;
//}

static void PrintNode(const node *node, const size_t *nNode, const char color[COLORLEN], FILE *fp) {
    assert(node && nNode && color && fp);

    fprintf(fp, "\tnode%lu [fillcolor=\"%s\", ", *nNode, color);

    switch (node->val->type) {
        case NUM_TYPE:
            fprintf(fp, "label=\"%lg\"", node->val->value);
            break;
        case VAR_TYPE:
            fprintf(fp, "label=\"%s\"", node->val->value.name);
            break;
        case KEYWORD_TYPE:
            #define DEF_KEYW(name, num, sign)                                       \
                    case KEYW_##name:                                               \
                        fprintf(fp, "label=\"%s\"", #name);     \
                        break;

            #define DEF_OPER(name, num, sign)                                       \
                    case KEYW_##name:                                               \
                        fprintf(fp, "label=\"%s\"", #name);     \
                        break;

            #define DEF_HELP(name, num, sign)

            switch (node->val->value.keyword) {
                #include "../keywords.h"

                #include "../operators.h"

                #undef DEF_KEYW
                #undef DEF_OPER
                #undef DEF_HELP
                default:
                    fprintf(fp, "label=\"UNDEF KEYW\"");
                    break;
            }
            break;
        default:
            fprintf(fp, "label=\"%d %d\"", node->val->type, node->val->value);
            break;
    }

    fprintf(fp, "];\n");

    return;
}

static void PrintEdge(const size_t from, const size_t to, const char color[COLORLEN], FILE *fp) {
    assert(color && fp);

    fprintf(fp, "\tedge [color=\"%s\"]; node%lu -> node%lu;\n", color, from, to);

    return;
}

static void nodedump(const node *node, size_t *nNode, FILE *fp) {
    assert(fp && nNode);

    switch (node->val->type) {
        case KEYWORD_TYPE:
            PrintNode(node, nNode, OP_COLOR, fp);
            break;
        case NUM_TYPE:
            PrintNode(node, nNode, CONST_COLOR, fp);
            break;
        case VAR_TYPE:
            PrintNode(node, nNode, VAR_COLOR, fp);
            break;
        default:
            PrintNode(node, nNode, ERR_COLOR, fp);
            break;
    }

    size_t current_number_of_node = *nNode;

    if (node->children[LEFT]) {
        PrintEdge(current_number_of_node, ++(*nNode), LEFT_COLOR, fp);
        nodedump(node->children[LEFT], nNode, fp);
    }

    if (node->children[RIGHT]) {
        PrintEdge(current_number_of_node, ++(*nNode), RIGHT_COLOR, fp);
        nodedump(node->children[RIGHT], nNode, fp);
    }
}

void TreeDump(tree *tree) {
    size_t nNode = 0;
    FILE *fp = fopen(DOTPATH, "w");

    fputs("digraph structs {\n", fp);
    fputs("\tnode [color=black, shape=box, style=\"filled\"];\n", fp);

    nodedump(tree->root, &nNode, fp);

    fputs("}\n", fp);

    fclose(fp);

    system("dot tree -T png -o tree.png");
}
//
//static TREE_ERROR nodeverify(node *nod) {
//    CHECK(nod, NULL_NODE, "null node")
//
//    CHECK(nod->parent == NULL || !(nod->parent) || nod == nod->parent->children[0] || nod == nod->parent->children[1], INVALID_LINK, "Invalid link")
//
//    CHECK(nod->type != NOT_DEFINED, UNDEFINED_TYPE, "undefined type")
//
//    CHECK(nod->type != KEYW || nod->value.keyword != WTF, UNDEFINED_OP, "not defined keyword")
//
//    CHECK(nod->type != VAR_TYPE || nod->value.name, NULL_NAME, "null name")
//
//    if(nod->children[0]) {
//        TREE_ERROR le = nodeverify(nod->children[LEFT]);
//        TREE_ERROR re = nodeverify(nod->children[RIGHT]);
//
//        if(le)
//            return le;
//
//        if(re)
//            return re;
//    }
//
//    return TREE_OK;
//}
//
//TREE_ERROR TreeVerify(tree *tree) {
//#ifdef FATAL_VERIFY
//    if(nodeverify(tree->root))
//        abort();
//#endif
//
//    //return nodeverify(tree->root);
//
//    return TREE_OK;
//}