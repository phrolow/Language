#include "compiler.h"

#define NODE_KEYW(NODE, KEYW) (NODE->val->type == KEYWORD_TYPE && NODE->val->value.keyword == KEYW)

#define KEYW(NODE) ((NODE->val->type == KEYWORD_TYPE) ? NODE->val->value.keyword : 0)

#define ABORT(MSG) {\
    printf(MSG);    \
    assert(0);      \
}

void CreateKeyword(token_t **token, KEYW keyword) {
    assert(token && *token);

    (*token)->type = KEYWORD_TYPE;
    (*token)->value.keyword = keyword;
}

void PushInNametable(struct Node *node, struct List *NT) {
    assert(node && NT);

    name_t name  = {};

    memcpy(name.name, node->val->value.name, sizeof(node->val->value.name));

    ListTailInsert(NT, name);
}

int IndexNametable(struct Node *node, struct List *NT) {
    assert(node && NT);

    if (node->val->type != VAR_TYPE)
    {
        printf("ERROR: got non-name type\n");
        assert(0);
    }

    size_t number = ListIndexFirst(NT, node->val->value.name);

    if (number == 0) ABORT("ERROR: couldn't find var");

    return number;
}

int SearchInNametable(struct Node *node, struct List *NT) {
    assert(node && NT);

    size_t index = ListIndexFirst(NT, node->val->value.name);

    if (index != -1) {
        return 1;
    }

    return 0;
}

static FILE   *out       = nullptr;
static struct Node *node_main = nullptr;

void GenerateScan(struct Node *node, struct List *NT, struct List *GlobalNT) {
    assert(node && NT && GlobalNT);

    if (!node->children[LEFT]) ABORT("ERROR: no arg\n");

    PushInNametable(node->children[LEFT], NT);

    size_t index = IndexNametable(node->children[LEFT], NT);

    fprintf(out, "IN\n");
    fprintf(out, "POP [rbx+%lu]\n", index - 1);
}

void GeneratePrint(struct Node *node, struct List *NT, struct List *GlobalNT) {
    assert(node && NT && GlobalNT);

    if (!node->children[RIGHT]) ABORT("ERROR: no arg\n");

    GenerateExpr(node->children[RIGHT], NT, GlobalNT);

    fprintf(out, "OUT\n");
    fprintf(out, "POP rdx\n");
}

void GenerateAssign(struct Node *node, struct List *NT, struct List *GlobalNT) {
    assert(node && GlobalNT);

    if (!NT) {
        InitGlobVar(node, GlobalNT);
        
    } else {
        InitVar(node, NT, GlobalNT);
    }
}

void GenerateStmt  (struct Node *node, struct List *NT, struct List *GlobalNT) {
    assert(node && GlobalNT);

    switch (KEYW(node))
    {
        case KEYW_ASSIGN:
            GenerateAssign(node, NT, GlobalNT);
            break;
        case KEYW_IF:
            GenerateIf    (node, NT, GlobalNT);
            break;
        case KEYW_WHILE:
            GenerateWhile (node, NT, GlobalNT);
            break;
        case KEYW_CALL:
            GenerateCall  (node, NT, GlobalNT);
            break;
        case KEYW_RETURN:
            GenerateReturn(node, NT, GlobalNT);
            break;
        case KEYW_SCAN:
            GenerateScan  (node, NT, GlobalNT);
            break;
        case KEYW_PRINT:
            GeneratePrint (node, NT, GlobalNT);
            break;
        case KEYW_ADD:
        case KEYW_SUB:
        case KEYW_MUL:
        case KEYW_DIV:
        case KEYW_POW:
            GenerateExpr  (node, NT, GlobalNT);
            break;
        default:
            ABORT("ERROR: invalid statement\n");
            break;
    }
}

void GenerateStmts (struct Node *node, struct List *NT, struct List *GlobalNT) {
    if (!NODE_KEYW(node, KEYW_STMT))
        ABORT("ERROR: invalid statement");

    while (node->children[RIGHT])
        node = node->children[RIGHT];

    while (NODE_KEYW(node, KEYW_STMT))
    {
        GenerateStmt(node->children[LEFT], NT, GlobalNT);
        node = node->parent;
    }
}

int IsNum(struct Node *node) {
    assert(node);

    return (node->val->type == NUM_TYPE);
}

int IsVar(struct Node *node) {
    assert(node);

    return (node->val->type == VAR_TYPE && node->parent && KEYW(node->parent) != KEYW_DEFINE);
}

int IsMathOper(struct Node *node) {
    assert(node);

    return ((KEYW(node) != 0) && (KEYW(node) >= KEYW_ADD) && (KEYW(node) <= KEYW_POW));
}

int IsLogOper(struct Node *node) {
    assert(node);

    return ((KEYW(node) != 0) && (KEYW(node) >= KEYW_EQUAL) && (KEYW(node) <= KEYW_OR));
}

void GenerateMathOper(struct Node *node)
{
    assert(node);

    int is_math_oper = IsMathOper(node);

    switch (KEYW(node))
    {
        case KEYW_ADD:
            fprintf(out, "ADD\n");
            break;
        case KEYW_SUB:
            fprintf(out, "SUB\n");
            break;
        case KEYW_MUL:
            fprintf(out, "MUL\n");
            break;
        case KEYW_DIV:
            fprintf(out, "DIV\n");
            break;
        case KEYW_POW:
            fprintf(out, "POW\n");
            break;

        default:
            ABORT("ERROR: unexpected oper\n");
            
            break;
    }
}

void GenerateNum(struct Node *node)
{
    assert(node);

    fprintf(out, "PUSH %lg\n", node->val->value.num);
}

void InitVar(struct Node *node, struct List *NT, struct List *GlobalNT) {
    if(SearchInNametable(node->children[LEFT], GlobalNT)) ABORT("ERROR: repeating var\n");

    if (SearchInNametable(node->children[LEFT], NT))
    {
        GenerateExpr(node->children[RIGHT], NT, GlobalNT);

        size_t index = IndexNametable(node->children[LEFT], NT);

        assert(index >= 1);

        fprintf(out, "POP [rbx+%lu]\n", index - 1);

        return;
    }

    PushInNametable(node->children[LEFT], NT);

    size_t index = IndexNametable(node->children[LEFT], NT);

    assert(index >= 1);

    GenerateExpr(node->children[RIGHT], NT, GlobalNT);

    fprintf(out, "POP [rbx+%lu]\n", index - 1);
}

void GenerateVar(struct Node *node, struct List *NT, struct List *GlobalNT) {
    if (SearchInNametable(node, GlobalNT)) {
        GenerateGlobVar(node, GlobalNT);
        return;
    }

    if (SearchInNametable(node, NT))
    {
        size_t index = IndexNametable(node, NT);

        fprintf(out, "PUSH [rbx+%lu]\n", index - 1);

        return;
    }

    ABORT("ERROR: variable not found");
}

void GenerateExpr  (struct Node *node, struct List *NT, struct List *GlobalNT)
{
    if (KEYW(node) == KEYW_CALL)
    {
        GenerateCall(node, NT, GlobalNT);
        
        return;
    }

    if (node->children[LEFT])
    {
        GenerateExpr(node->children[LEFT],  NT, GlobalNT);
        
    }
    if (node->children[RIGHT])
    {
        GenerateExpr(node->children[RIGHT], NT, GlobalNT);
    }

    if (IsMathOper(node))
    {
        GenerateMathOper(node);

        return;
    }

    if (IsNum(node))
    {
        GenerateNum(node);

        return;
    }

    if (IsVar(node))
    {
        GenerateVar(node, NT, GlobalNT);

        return;
    }

    ABORT("ERROR: undefined operator\n");
}

void InitCallParams(struct Node *node, struct List *NT, struct List *GlobalNT, size_t *num_of_params) {
    assert(num_of_params);

    if (!NODE_KEYW(node, KEYW_PARAM))
    {
        ABORT("ERROR: they're not params...\n");
    }

    assert(*num_of_params == 0);

    (*num_of_params)++;

    GenerateExpr(node->children[RIGHT], NT, GlobalNT);

    fprintf(out, "POP [rbx+%d]\n", *num_of_params);
}

void GenerateCall(struct Node *node, struct List *NT, struct List *GlobalNT) {
    struct Node *params = node->children[RIGHT];

    assert(params);

    size_t num_of_params = 0;

    InitCallParams(params, NT, GlobalNT, &num_of_params);

    assert(num_of_params == 1);

    struct Node *name = node->children[LEFT];

    fprintf(out, "\nCALL %s:\n", name->val->value.name);

    fprintf(out, "PUSH rcx\n");
}

void GenerateJump(struct Node *node, struct List *NT, struct List *GlobalNT, const char *mark, const int num) {
    assert(mark);

    if (!IsLogOper(node)) ABORT("IS NOT LOG OP\n");

    switch (KEYW(node))
    {
        case KEYW_LESS:
            fprintf(out, "JAE");
            break;
        case KEYW_LESSOREQ:
            fprintf(out, "JA" );
            break;
        case KEYW_NOTEQUAL:
            fprintf(out, "JE" );
            break;
        case KEYW_EQUAL:
            fprintf(out, "JNE");
            break;
        case KEYW_GREATOREQ:
            fprintf(out, "JB" );
            break;
        case KEYW_GREAT:
            fprintf(out, "JBE");
            break;
        default: ABORT("UNDEF OP\n");
    }

    fprintf(out, " %s_%d:\n", mark, num);
}

void GenerateCond(struct Node *node, struct List *NT, struct List *GlobalNT, const char *mark, const int num) {
    GenerateExpr(node->children[LEFT],  NT, GlobalNT);
    GenerateExpr(node->children[RIGHT], NT, GlobalNT);
    GenerateJump(node, NT, GlobalNT, mark, num);
}

// TODO: place this counter into struct Backend
static size_t __IF_COUNTER__    = 0;
static size_t __WHILE_COUNTER__ = 0;

void GenerateIf(struct Node *node, struct List *NT, struct List *GlobalNT) {
    struct Node *else_node   = node->children[RIGHT];

    struct Node *condition  = node->children[LEFT];

    struct Node *if_stmts = NULL,
                *else_stmts = NULL;

    if(!NODE_KEYW(else_node, KEYW_ELSE)) {
        if_stmts = else_node;
    }
    else {
        if_stmts = else_node->children[LEFT];
        else_stmts = else_node->children[RIGHT];
    }

    int counter = __IF_COUNTER__++;

    if (else_stmts)
    {
        GenerateCond(condition, NT, GlobalNT, "ELSE", counter);
        GenerateStmts(if_stmts, NT, GlobalNT);

        fprintf(out, "JMP :END_IF_%d\n", counter);
        fprintf(out, ":ELSE_%d\n", counter);

        GenerateStmts(else_stmts, NT, GlobalNT);
    }
    else
    {
        GenerateCond(condition, NT, GlobalNT, "END_IF", counter);
        GenerateStmts(if_stmts, NT, GlobalNT);
    }

    fprintf(out, ":END_IF_%d\n", counter);
}

void GenerateReturn(struct Node *node, struct List *NT, struct List *GlobalNT)
{
    if (!NODE_KEYW(node, KEYW_RETURN)) {
        ABORT("THERE IS NO RETURN");
    }

    if(node->children[RIGHT]) {
        GenerateExpr(node->children[RIGHT], NT, GlobalNT);

        fprintf(out, "POP rcx\n");
    }

    fprintf(out, "RET\n");
}

void GenerateWhile(struct Node *node, struct List *NT, struct List *GlobalNT)
{
    struct Node *condition = node->children[LEFT];
    struct Node *while_stmts = node->children[RIGHT];

    int counter = __WHILE_COUNTER__++;

    fprintf(out, "\nWHILE_%d:\n", counter);

    GenerateCond(condition, NT, GlobalNT, "END_WHILE", counter);
    GenerateStmts(while_stmts, NT, GlobalNT);

    fprintf(out, "JMP WHILE_%d:\n", counter);
    fprintf(out, "END_WHILE_%d:\n", counter);
}

void GenerateDefParams(struct Node *node, struct List *NT, struct List *GlobalNT, size_t *free_memory_index) {
    assert(free_memory_index);

    if (node->children[RIGHT]) {
        return;
    }
    else {
        *free_memory_index = 1;
        if (SearchInNametable(node->children[LEFT], GlobalNT)) ABORT("VAR IS ALREASY DEFINED");
        PushInNametable(node->children[LEFT], NT);
    }
}

void IncreaseRBX(const size_t number) {
    fprintf(out, "PUSH rbx ; incrrbx\n");
    fprintf(out, "PUSH %lu\n", number);
    fprintf(out, "ADD\n");
    fprintf(out, "POP rbx\n");
}

void DecreaseRBX(const size_t number) {
    fprintf(out, "PUSH rbx ; decrrbx\n");
    fprintf(out, "PUSH %lu\n", number);
    fprintf(out, "SUB\n");
    fprintf(out, "POP rbx\n");
}

void GenerateMark(struct Node *mark) {
    if (mark->val->type == VAR_TYPE || NODE_KEYW(mark, KEYW_MAIN)) {
        if (mark->val->type == VAR_TYPE) {
            fprintf(out, "\n:%s\n", mark->val->value.name);
            return;
        }
        else
        if (KEYW(mark) == KEYW_MAIN) {
            fprintf(out, "\n:main\n");
            return;
        }
    }

    ABORT("NON-NAME TYPE OF NODE");
}

void GenerateFuncDef(struct Node *node, struct List *NT, struct List *GlobalNT)
{
//    if (NT->size != 1) {
//        ABORT("NT ISNT EMPTY\n");
//    }

    struct Node *func   = node->children[LEFT];
    struct Node *mark   = func->children[LEFT];

    if (KEYW(mark) == KEYW_MAIN) {
        if (!node_main) {
            node_main = node;
            return;
        }
        else {
            ABORT("REPEATING MAIN\n");
        }
    }

    struct Node *params = func->children[RIGHT];
    size_t free_memory_index = 0;
    GenerateMark(mark);

    if (params) {
        GenerateDefParams(params, NT, GlobalNT, &free_memory_index);
    }

    IncreaseRBX(free_memory_index);

    struct Node *stmts  = node->children[RIGHT];

    GenerateStmts(stmts, NT, GlobalNT);

    DecreaseRBX(free_memory_index);

    ListInit(NT);
}

void GenerateMain(struct Node *node, struct List *NT, struct List *GlobalNT) {
    if (!node_main) ABORT("NO_MAIN")

    struct Node *func = node->children[LEFT];
    struct Node *main = func->children[LEFT];

    IncreaseRBX(GlobalNT->size);
    GenerateMark(main);

    assert(node);

    struct Node *stmts  = node->children[RIGHT];

    GenerateStmts(stmts, NT, GlobalNT);
}

void GenerateGlobVar(struct Node *node, struct List *GlobalNT) {
    if (SearchInNametable(node, GlobalNT)) {
        size_t index = IndexNametable(node, GlobalNT);

        fprintf(out, "PUSH [%lu]\n", index - 1);
    }

    ABORT("VAR NOT FOUND\n");
}

void GenerateGlobExpr(struct Node *node, struct List *GlobalNT) {
    if (KEYW(node) == KEYW_CALL) {
        ABORT("WTF\n");
    }
    if (node->children[LEFT]) {
        GenerateGlobExpr(node->children[LEFT], GlobalNT);
    }
    if (node->children[RIGHT]) {
        GenerateGlobExpr(node->children[RIGHT], GlobalNT);
    }
    
    if (IsMathOper(node)) {
        GenerateMathOper(node);

        return;
    }

    if (IsNum(node)) {
        GenerateNum(node);

        return;
    }

    if (IsVar(node)) {
        GenerateGlobVar(node, GlobalNT);

        return;
    }

    ABORT("UNDEF OP\n");
}

void InitGlobVar(struct Node *node, struct List *GlobalNT) {
    if (SearchInNametable(node->children[LEFT], GlobalNT)) ABORT("REPEATING DEFINITION\n");

    PushInNametable(node->children[LEFT], GlobalNT);

    size_t index = IndexNametable(node->children[LEFT], GlobalNT);

    assert(index >= 1);

    GenerateGlobExpr(node->children[RIGHT], GlobalNT);

    fprintf(out, "POP [rbx+%lu]\n", index - 1);
}

void GenerateGS(struct Node *node, struct List *GlobalNT) {
    if (!NODE_KEYW(node, KEYW_STMT)) ABORT("NOT GLOBAL STATEMENT\n");

    fprintf(out, "CALL :main\n");
    fprintf(out, "HLT\n\n");

    while (node->children[RIGHT])
        node = node->children[RIGHT];

    struct Node *bottom = node;

    while (node) {
        if (KEYW(node->children[LEFT]) == KEYW_ASSIGN)
        {
            GenerateAssign(node, NULL, GlobalNT);
        }

        node = node->parent;
    }

    struct List *NT = newList();

    node = bottom;

    while (node) {
        if (NODE_KEYW(node->children[LEFT], KEYW_DEFINE)) {
            GenerateFuncDef(node->children[LEFT], NT, GlobalNT);
        } else if (NODE_KEYW(node->children[LEFT], KEYW_ASSIGN)) {
            node = node->parent;
            continue;
        } else {
            ABORT("INVALID STATEMENT");
        }
        node = node->parent;
    }

    GenerateMain(node_main, NT, GlobalNT);
    

    ListDtor(NT);

    free(NT);
}

void GenerateASM(const char *filename, tree *tree) {
    assert(filename);

    FILE *stream = fopen(filename, "w");

    out = stream;

    struct List *GlobalNT = newList();

    GenerateGS(tree->root, GlobalNT);
    
    ListDtor(GlobalNT);

    fclose(out);
    out = NULL;

    free(GlobalNT);
}