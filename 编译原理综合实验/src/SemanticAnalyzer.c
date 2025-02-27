#include<stdio.h>
#include "shared.h"
#include "Lexer.h"
#include "SemanticAnalyzer.h"

#define MAX_QUADRUPLES 100

static int tempnum = 0;
int quadIndex = 0;
QUADRUPLE quadruples[MAX_QUADRUPLES];

char* newtemp() {
    static char temp[8];
    sprintf(temp, "T%d", tempnum++);
    return temp;
}

// 生成四元式并加入到四元式表中
void emit(char* result, char* arg1, char* op, char* arg2) {
    if (quadIndex >= MAX_QUADRUPLES) {
        printf("Error: Quadruple table overflow\n");
        return;
    }
    strcpy(quadruples[quadIndex].result, result);
    strcpy(quadruples[quadIndex].arg1, arg1);
    strcpy(quadruples[quadIndex].op, op);
    strcpy(quadruples[quadIndex].arg2, arg2);
    quadIndex++;
}

// 生成条件跳转四元式
void emitJump(char* reason, char* arg1, char* arg2) {
    emit(reason, arg1, "goto", arg2);
    quadIndex++;
}

// 生成无条件跳转四元式 回填
void emitnJump(char* arg2, int old_quadIndex) {
    strcpy(quadruples[old_quadIndex].result, "");
    strcpy(quadruples[old_quadIndex].arg1, "");
    strcpy(quadruples[old_quadIndex].op, "goto");
    strcpy(quadruples[old_quadIndex].arg2, arg2);
}

// 打印四元式表
void printQuadruples() {
    for (int i = 0; i < quadIndex; i++) {
        printf("(%d):(%s, %s, %s, %s)\n", i, quadruples[i].result, quadruples[i].arg1, quadruples[i].op, quadruples[i].arg2);
    }
}