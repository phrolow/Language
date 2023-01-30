#ifndef LANGUAGE_COMPILER_H
#define LANGUAGE_COMPILER_H

#include <math.h>

#include "../Tree/tree.h"
#include "../Reader/reader.h"
#include "../Onegin/onegin.h"
#include "../Debug/debug.h"
#include "../List/list.h"

struct Compiler {
    FILE *out;
    struct Node *node_main;
    size_t __IF_COUNTER__ ;
    size_t __WHILE_COUNTER__;
};

int language_compile(const char *in, const char *out);

struct Compiler *newCompiler(FILE *out);
void CompilerDtor(struct Compiler *compiler);

int IsNum(struct Node *node);
int IsVar(struct Node *node);
int IsMathOper(struct Node *node);
int IsLogOper(struct Node *node);

void IncreaseRBX        (const size_t number);
void DecreaseRBX        (const size_t number);

void GenerateMark      (struct Node *mark);
void GenerateMain      (struct Node *node, struct List *NT, struct List *GlobalNT);
void GenerateFuncDef   (struct Node *node, struct List *NT, struct List *GlobalNT);
void GenerateDefParams (struct Node *node, struct List *NT, struct List *GlobalNT, size_t *free_memory_index);

void InitVar           (struct Node *node, struct List *NT, struct List *GlobalNT);
void GenerateVar       (struct Node *node, struct List *NT, struct List *GlobalNT);
void GenerateExpr      (struct Node *node, struct List *NT, struct List *GlobalNT);
void InitGlobVar       (struct Node *node, struct List *GlobalNT);
void GenerateGlobExpr  (struct Node *node, struct List *GlobalNT);
void GenerateGlobVar   (struct Node *node, struct List *GlobalNT);

void GenerateMathOper  (struct Node *node);
void GenerateNum       (struct Node *node);
void InitCallParams    (struct Node *node, struct List *NT, struct List *GlobalNT, size_t *num_of_params);
void GenerateCall      (struct Node *node, struct List *NT, struct List *GlobalNT);
void GenerateJump      (struct Node *node, struct List *NT, struct List *GlobalNT, const char *mark, const int num);
void GenerateCond      (struct Node *node, struct List *NT, struct List *GlobalNT, const char *mark, const int num);
void GenerateIf        (struct Node *node, struct List *NT, struct List *GlobalNT);
void GenerateReturn    (struct Node *node, struct List *NT, struct List *GlobalNT);
void GenerateWhile     (struct Node *node, struct List *NT, struct List *GlobalNT);
void GenerateScan      (struct Node *node, struct List *NT, struct List *GlobalNT);
void GeneratePrint     (struct Node *node, struct List *NT, struct List *GlobalNT);
void GenerateAssign    (struct Node *node, struct List *NT, struct List *GlobalNT);

void GenerateStmt      (struct Node *node, struct List *NT, struct List *GlobalNT);
void GenerateStmts     (struct Node *node, struct List *NT, struct List *GlobalNT);
void GenerateGS        (struct Node *node, struct List *GlobalNT);
void GenerateASM       (const char *filename, tree *tree);

#endif //LANGUAGE_COMPILER_H
