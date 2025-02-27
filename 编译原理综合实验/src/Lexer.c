#include<stdio.h>
#include "shared.h"

#define KEYWORDS_COUNT 8
char* rwtab[] = {"main", "void", "int", "char", "if", "else", "for", "while"};

char m_getch(char* input, int* p_input, char* ch){
    *ch = input[*p_input];
    return input[(*p_input)++];
}

void getbc(char* input, int* p_input, int* line){
    while(input[*p_input] == ' ' || input[*p_input] == 10 || input[*p_input] == 13 || input[*p_input] == 9){
        if(input[*p_input] == 10) (*line)++;
        (*p_input)++;
    }
}

void concat(char* token, int *p_token, char* ch){
    token[*p_token] = *ch;
    token[++(*p_token)] = '\0';
}

int letter(char ch){
    if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) return 1;
    else return 0;
}

int digit(char ch){
    if(ch >= '0' && ch <= '9') return 1;
    else return 0;
}

int reserve(char* token){
    int i = 0;
    while(i < KEYWORDS_COUNT){
        if(strcmp(token, rwtab[i]) == 0) return i+1;
        i++;
    }
    return 10;
}

void retract(int* p_input){
    (*p_input)--;
}

WORD* scaner(char* input, int* p_input){
    WORD* word = (WORD*)malloc(sizeof(WORD)*(*p_input));
    word->typenum = 0;
    word->word = "";
    int p_token = 0;
    char token[255] = "";
    char ch;
    m_getch(input, p_input, &ch);
    if(letter(ch)){
        while (letter(ch) || digit(ch)){
            concat(token, &p_token, &ch);
            m_getch(input, p_input, &ch);
        }
        retract(p_input);
        word->typenum = reserve(token);
        strcpy(word->word,token);
        return word;
    }
    else if(digit(ch)){
        while(digit(ch)){
            concat(token, &p_token, &ch);
            m_getch(input, p_input, &ch);
        }
        retract(p_input);
        word->typenum = 20;
        strcpy(word->word,token);
        return word;
    }
    else switch (ch){
        case '=':
            m_getch(input, p_input, &ch);
            if(ch == '='){
                word->typenum = 42;
                word->word = "==";
                return word;
            }
            retract(p_input);
            word->typenum = 21;
            word->word = "=";
            return word;
        case '+':
            word->typenum = 22;
            word->word = "+";
            return word;
        case '-':
            word->typenum = 23;
            word->word = "-";
            return word;
        case '*':
            word->typenum = 24;
            word->word = "*";
            return word;
        case '/':
            word->typenum = 25;
            word->word = "/";
            return word;
        case '(':
            word->typenum = 26;
            word->word = "(";
            return word;
        case ')':
            word->typenum = 27;
            word->word = ")";
            return word;
        case '[':
            word->typenum = 28;
            word->word = "[";
            return word;
        case ']':
            word->typenum = 29;
            word->word = "]";
            return word;
        case '{':
            word->typenum = 30;
            word->word = "{";
            return word;
        case '}':
            word->typenum = 31;
            word->word = "}";
            return word;
        case ',':
            word->typenum = 32;
            word->word = ",";
            return word;
        case ';':
            word->typenum = 33;
            word->word = ";";
            return word;
        case ':':
            word->typenum = 34;
            word->word = ":";
            return word;
        case '<':
            m_getch(input, p_input, &ch);
            if(ch == '='){
                word->typenum = 41;
                word->word = "<=";
                return word;
            }
            retract(p_input);
            word->typenum = 35;
            word->word = "<";
            return word;
        case '>':
            m_getch(input, p_input, &ch);
            if(ch == '='){
                word->typenum = 40;
                word->word = ">=";
                return word;
            }
            retract(p_input);
            word->typenum = 36;
            word->word = ">";
            return word;
        case '!':
            m_getch(input, p_input, &ch);
            if(ch == '='){
                word->typenum = 43;
                word->word = "!=";
                return word;
            }
            retract(p_input);
            word->typenum = 37;
            word->word = "!";
            return word;
        case '#':
            word->typenum = 1000;
            word->word = "over";
            return word;
        case '\0':
            word->typenum = 1000;
            word->word = "OVER";
            return word;
        default: 
            word->typenum = -1;
            word->word = "ERROR";
            return word;
    }
    
}
