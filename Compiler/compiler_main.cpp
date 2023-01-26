#include "compiler.h"

int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Too few arguments!");
        exit(1);
    }

    if(!checkfile(argv[1])) {
        printf("Invalid input file");
        exit(2);
    }

    if(!checkfile(argv[2])) {
        printf("Invalid output file");
        exit(2);
    }

    tree *expression;

    text txt = textFromFile(argv[1]);

    expression = ReadExpression(txt.content);

    TreeDump(expression);

    PrintAsm(expression, argv[2]);

    TreeDtor(expression);

    return 0;
}