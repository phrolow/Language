#include "compiler.h"

int language_compile(const char *in, const char *out) {
    assert(in && out);

    tree *expression;

    text txt = textFromFile(in);

    expression = ReadExpression(txt.content);

    TreeDump(expression);

    PrintAsm(expression, out);

    TreeDtor(expression);

    return 0;
}