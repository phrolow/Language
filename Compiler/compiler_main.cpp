#include "compiler.h"
#include "../config.h"

int language_compile(const char *in, const char *out) {
    tree *expression;

    text txt = textFromFile(in);

    FILE *fp = fopen(out, "w");

    Compiler *compiler = newCompiler(fp);

    expression = ReadExpression(txt.content);

    TreeDump(expression);

    GenerateASM(ASM_PATH, expression, compiler);

    TreeDtor(expression);
    CompilerDtor(compiler);

    fclose(fp);

    return 0;
}