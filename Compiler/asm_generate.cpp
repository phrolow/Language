#include "compiler.h"

#define DEF_CMD(name, num, sign, ...)   \
    case name:                          \
        format = #name;                 \
        break;

//const char *GetOpFormat(enum KEYW keyword) {
//    const char *format = NULL;
//
//    switch (keyword) {
//        #include "../codegen.h"
//        #undef DEF_CMD
//        default:
//            break;
//    }
//
//    return format;
//}
//
//char *ToLine(token_t *token, char *line, Compiler *compiler) {
//    switch (token->type) {
//        case NUM_TYPE:
//            sprintf(line, "PUSH %lg\n", token->value.num);
//            break;
//        case KEYW:
//            sprintf(line, GetOpFormat(token->value.keyword));
//            line = strcat(line, "\n");
//            break;
//        case VAR_TYPE:
//            sprintf(line, "PUSH [%d]\n", VarIndex(token->value.name, compiler));
//            break;
//        default:
//            sprintf(line, "Я ПИДОРАС");
//            break;
//    }
//
//    return line;
//}
//
//static void addline(node *nod, FILE *fp, Compiler *compiler) {
//    char *line = (char*) calloc(MAX_ASM_LINE, sizeof(char*));
//
//    if(nod->children[LEFT]) {
//        addline(nod->children[LEFT], fp, compiler);
//    }
//
//    if(nod->children[RIGHT]) {
//        addline(nod->children[RIGHT], fp, compiler);
//    }
//
//    fprintf(fp, ToLine((nod->val), line, compiler));
//
//    free(line);
//}
//
//void PrintAsm(tree *code, const char *path, Compiler *compiler) {
//    FILE *fp = fopen(path, "w");
//
//    addline(code->root, fp, compiler);
//
//    fprintf(fp, "OUT\nHLT\n");
//
//    fclose(fp);
//}
//
//struct Compiler *NewCompiler(Tree *code) {
//    Compiler *compiler = (Compiler*) malloc(sizeof(Compiler));
//
//    compiler->code = code;
//
//    for(int i = 0; i < NAME_MAX_LEN; i++)
//        for(int j = 0; j < MAX_VARS; j++) {
//            compiler->var_table[i][j] = '\0';
//        }
//
//
//    return compiler;
//}
//
//size_t VarIndex(char *var, Compiler *compiler) { //cringe
//    size_t index = 0;
//
//    for(index = 0; compiler->var_table[index][0] != '\0'; index++) {
//        if(!strcmp(var, compiler->var_table[index]))
//            return index;
//    }
//
//    strcpy(compiler->var_table[index], var);
//
//    return index;
//}