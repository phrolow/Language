#ifndef LANGUAGE_ANALYZER_H
#define LANGUAGE_ANALYZER_H

#include "../Onegin/onegin.h"
#include "../Tokens/tokens.h"
#include <ctype.h>

const size_t WORD_MAX_LEN = 0x20;

void GetTokens(char *txt, token_stk_t *tokens);

#endif //LANGUAGE_ANALYZER_H
