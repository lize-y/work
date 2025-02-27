#ifndef LEXER_H
#define LEXER_H
#include "shared.h"

WORD* scaner(char* input, int* p_input);

void getbc(char* input, int* p_input, int* line);

#endif