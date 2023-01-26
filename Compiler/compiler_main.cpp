#include "compiler.h"

int main(int argc, char **argv) {
    if(!argc) {
        printf("Too few arguments!");
        exit(1);
    }

    if(!checkfile(argv[1])) {
        printf("Invalid file");
        exit(2);
    }

    tree *expression;

    text txt = textFromFile(argv[1]);

    expression = ReadExpression(txt.content);

    TreeDump(expression);

    TreeDtor(expression);

    return 0;
}