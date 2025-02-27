#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

typedef struct {
    char result[8];
    char arg1[8];
    char op[8];
    char arg2[8];
} QUADRUPLE;

extern int quadIndex;

void emit(char* result, char* arg1, char* op, char* arg2);

void emitJump(char* result, char* arg1, char* arg2);

void emitnJump(char* arg2, int quadIndex);

char* newtemp();

void printQuadruples();

#endif