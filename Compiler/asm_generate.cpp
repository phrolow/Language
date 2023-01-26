#include "compiler.h"

#define DEF_CMD(name, num, sign, ...)   \
    case name:                          \
        format = #name;                 \
        break;

const char *GetOpFormat(enum OP op) {
    const char *format = NULL;

    switch (op) {
        #include "../codegen.h"
        #undef DEF_CMD
        default:
            break;
    }

    return format;
}

char *ToLine(token_t *token, char *line) {
    switch (token->type) {
        case NUMERAL:
            sprintf(line, "PUSH %lg\n", token->value.num);
            break;
        case OP:
            sprintf(line, GetOpFormat(token->value.op));
            line = strcat(line, "\n");

            break;
        default:
            sprintf(line, "Я ПИДОРАС");
            break;
    }

    return line;
}

static void addline(node *nod, FILE *fp) {
    char *line = (char*) calloc(MAX_ASM_LINE, sizeof(char*));

    if(nod->children[LEFT]) {
        addline(nod->children[LEFT], fp);
    }

    if(nod->children[RIGHT]) {
        addline(nod->children[RIGHT], fp);
    }

    fprintf(fp, ToLine((nod->val), line));

    free(line);
}

void PrintAsm(tree *code, const char *path) {
    FILE *fp = fopen(path, "w");

    addline(code->root, fp);

    fprintf(fp, "HLT\n");

    fclose(fp);
}