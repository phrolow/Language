#include "analyzer.h"

int streq(const char *txt, const char *str) {
    int len = strlen(str);

    for(int i = 0; i < len; i++) {
        if(*(txt + i) != *(str + i)) {
            return 0;
        }
    }

    return 1;
}

void skip(char **ptr) {
    while(isspace(*ptr))
        (*ptr)++;
}

void getKeyword (char *word, int *keyword_code, size_t *length) {
    assert(keyword_code && word && length);

#define DEF_KEYW(DEF, CODE, WORD, FMT)          \
    if (streq(word, #WORD)) {                   \
        *keyword_code = CODE;                   \
        *length = strlen(#WORD);                \
    } else
#include "../codegen.h"
/*  else */ {
        *keyword_code = 0;
        *length = 0;
    }
#undef DEF_KEYW

    return;
}