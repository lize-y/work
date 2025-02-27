#include<stdio.h>
#include "shared.h"
#include "Lexer.h"
#include "SemanticAnalyzer.h"

WORD* Word;


int line = 0;

WORD* scaner_and_getbc(char* input, int* p_input){
    WORD* word=scaner(input, p_input);
    getbc(input, p_input, &line);
    return word;
};

void program(char* input);
void statement_block(char* input, int* p_input);
void statement_string(char* input, int* p_input);
void statement(char* input, int* p_input);
void assignment(char* input, int* p_input);
void conditional_statement(char* input, int* p_input);
void loop(char* input, int* p_input);
char* condition(char* input, int* p_input);
void relational(char* input, int* p_input);
char* expression(char* input, int* p_input);
char* term(char* input, int* p_input);
char* factor(char* input, int* p_input);


int SyntaxError = 0;
//<程序> = main( )<语句块>
void program(char* input){
    
    int p_input = 0;
    Word=scaner_and_getbc(input, &p_input);
    //"main"
    if (Word->typenum==1){
        Word=scaner_and_getbc(input, &p_input);
        //"("
        if (Word->typenum==26){
            Word=scaner_and_getbc(input, &p_input);
            //")"
            if (Word->typenum==27){
                Word=scaner_and_getbc(input, &p_input);
                //语句块
                statement_block(input, &p_input);
            }
            else{
                SyntaxError = 1;
                printf("Error in line %d: program should start with 'main()'\n", line);
            }
        }else{
            SyntaxError = 1;
            printf("Error in line %d: program should start with 'main()'\n", line);
        }
    }else{
        SyntaxError = 1;
        printf("Error in line %d: program should start with 'main()'\n", line);
    }
    if (SyntaxError==1){
        printf("Syntax error\n");
    }
    else{
        emit("end", "", "", "");
        printQuadruples();
    }
        
}

//<语句块> = {<语句串>}
void statement_block(char* input, int* p_input){
    //"{"
    if (Word->typenum==30){
        Word=scaner_and_getbc(input, p_input);
        //语句串
        statement_string(input, p_input);
        //"}"
        if (SyntaxError)
            return;
        if (Word->typenum==31){
            Word=scaner_and_getbc(input, p_input);
        }
        else{
            SyntaxError = 1;
            printf("Error in line %d: statement block should end with '}'\n", line);
        } 
    }
    else{
        SyntaxError = 1;
        printf("Error in line %d: statement block should start with '{'\n", line);
    }
    
}

//<语句串> = <语句>{;<语句>};
void statement_string(char* input, int* p_input){
    int end = 0;
    statement(input, p_input);
    if (SyntaxError)
        return;
    while (Word->typenum==33){
        Word=scaner_and_getbc(input, p_input);
        //语句的follow集合: {"}"}
        if (Word->typenum==31){
            end = 1;
            break;
        }
        statement(input, p_input);
    }
    if (end==0){
        SyntaxError = 1;
        printf("Error in line %d: statement string should end with ';'\n", line);
    }
}

//<语句> = <赋值语句>|<条件语句>|<循环语句>
//<赋值语句> = 标识符=表达式，first集合：{标识符}
//<条件语句> = if<条件><语句块>[else<语句块>]，first集合：{if}; 条件语句结尾没";"
//<循环语句> = while<条件><语句块>，first集合：{while}
void statement(char* input, int* p_input){
    switch (Word->typenum){
        case 10:
            assignment(input, p_input);
            break;
        case 5:
            conditional_statement(input, p_input);
            break;
        case 8:
            loop(input, p_input);
    }
}

//<赋值语句> = 标识符=表达式
void assignment(char* input, int* p_input){
    char result[8], arg1[8];
    strcpy(result, Word->word);
    Word=scaner_and_getbc(input, p_input);
    //"="
    if (Word->typenum==21){
        Word=scaner_and_getbc(input, p_input);
        strcpy(arg1,expression(input, p_input));
        emit(result, arg1, "", "");
    }
    else{
        SyntaxError = 1;
        printf("Error in line %d: assignment should have '='\n", line);
    }
}

//<条件语句> = if'('<条件>')'<语句块>[else<语句块>]
void conditional_statement(char* input, int* p_input){
    int l1 = quadIndex+1;
    int l2 = quadIndex+2;
    char lable1[8], lable2[8];
    sprintf(lable2, "(%d)", l2);
    char arg1[8], arg2[8];
    Word=scaner_and_getbc(input, p_input);
    //"("
    if (Word->typenum==26){
        Word=scaner_and_getbc(input, p_input);
        strcpy(arg1,condition(input, p_input));
        //")"
        if (SyntaxError)
            return;
        if (Word->typenum==27){
            emitJump("if", arg1, lable2);
            Word=scaner_and_getbc(input, p_input);
            statement_block(input, p_input);
            if (SyntaxError)
                return;
            sprintf(lable1, "(%d)",quadIndex);
            emitnJump(lable1,l1);
            //"else"
            if (Word->typenum==6){
                Word=scaner_and_getbc(input, p_input);
                statement_block(input, p_input);
            }
        }
        else{
            SyntaxError = 1;
            printf("Error in line %d: conditional statement should end with ')'\n", line);
        }
    }
    else{
        SyntaxError = 1;
        printf("Error in line %d: conditional statement should start with '('\n", line);
    }
}

//<循环语句> = while'('<条件>')'<语句块>
void loop(char* input, int* p_input){
    int l0 = quadIndex;
    int l1 = quadIndex+1;
    int l2 = quadIndex+2;
    char lable1[8], lable2[8];
    sprintf(lable2, "(%d)", l2);
    char arg1[8], arg2[8];
    Word=scaner_and_getbc(input, p_input);
    //"("
    if (Word->typenum==26){
        Word=scaner_and_getbc(input, p_input);
        strcpy(arg1,condition(input, p_input));
        if (SyntaxError)
            return;
        //")"
        if (Word->typenum==27){
            emitJump("if", arg1, lable2);
            Word=scaner_and_getbc(input, p_input);
            statement_block(input, p_input);
        }
        else{
            SyntaxError = 1;
            printf("Error in line %d: loop should end with ')'\n", line);
        }
        sprintf(lable1, "(%d)",quadIndex+1);
        sprintf(lable2, "(%d)",l0);
        emitnJump(lable2,quadIndex++);
        emitnJump(lable1,l1);
    }
    else{
        SyntaxError = 1;
        printf("Error in line %d: loop should start with '('\n", line);
    }
}

//<条件> = <表达式><关系运算符><表达式>
char* condition(char* input, int* p_input){
    char* result = (char*)malloc(8 * sizeof(char));
    strcpy(result,expression(input, p_input));
    if (SyntaxError){
        free(result);
        return NULL;
    }
    strcat(result, Word->word);
    relational(input, p_input);
    if (SyntaxError){
        free(result);
        return NULL;
    }
    strcat(result, expression(input, p_input));
    return result;
}

//<关系运算符> = < | <= | > | >= | == | !=
void relational(char* input, int* p_input){
    switch (Word->typenum){
        case 35:
        case 36:
        case 40:
        case 41:
        case 42:
        case 43:
            Word=scaner_and_getbc(input, p_input);
            break;
        default:
            SyntaxError = 1;
            printf("Error in line %d: relational operator should be <, <=, >, >=, == or !=\n", line);
    }
}

//<表达式> = <项>{<加法运算符><项>|<减法运算符><项>}
char* expression(char* input, int* p_input){
    char result[8], arg1[8], op[8], arg2[8];
    strcpy(arg1,term(input, p_input));
    if (SyntaxError)
        return NULL;
    while (Word->typenum==22||Word->typenum==23){
        strcpy(op,Word->word);
        Word=scaner_and_getbc(input, p_input);
        strcpy(arg2,term(input, p_input));
        strcpy(result,newtemp());
        emit(result, arg1, op, arg2);
        strcpy(arg1,result);
    }
    return arg1;
}

//<项> = <因子>{<乘法运算符><因子>|<除法运算符><因子>}
char* term(char* input, int* p_input){
    char result[8], arg1[8], op[8], arg2[8];
    strcpy(arg1,factor(input, p_input));
    if (SyntaxError){
        return NULL;
    }
    while (Word->typenum==24||Word->typenum==25){
        strcpy(op,Word->word);
        Word=scaner_and_getbc(input, p_input);
        strcpy(arg2,factor(input, p_input));
        strcpy(result,newtemp());
        emit(result, arg1, op, arg2);
        strcpy(arg1,result);
    }
    return arg1;
}

//<因子> = 标识符|整数|'('<表达式>')'
char* factor(char* input, int* p_input){
    char* result = (char*)malloc(8 * sizeof(char));
    switch (Word->typenum){
        case 10:
        case 20:
            strcpy(result,Word->word);
            Word=scaner_and_getbc(input, p_input);
            break;
        case 26:
            Word=scaner_and_getbc(input, p_input);
            strcpy(result,expression(input, p_input));
            if (SyntaxError){
                free(result);
                return NULL;
            }
            if (Word->typenum==27){
                Word=scaner_and_getbc(input, p_input);
                return result;
            }
            else{
                SyntaxError = 1;
                printf("Error in line %d: expression in factor should end with ')'\n", line);
                free(result);
                return NULL;
            }
        default:
            SyntaxError = 1;
            printf("Error in line %d: factor should be identifier, integer or expression\n", line);
            free(result);
            return NULL;
    }
    return result;
}
