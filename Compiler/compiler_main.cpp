#include "compiler.h"
#include "../config.h"

int language_compile(const char *in, const char *out) {
    assert(in && out);

    tree *expression;

    text txt = textFromFile(in);

    //Compiler *compiler = NewCompiler(expression);

    expression = ReadExpression(txt.content);

    TreeDump(expression);

    GenerateASM(ASM_PATH, expression);

    TreeDtor(expression);
    //free(txt.content);

    return 0;
}