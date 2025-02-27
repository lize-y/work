#include<stdio.h>
#include "shared.h"
#include "Lexer.h"
#include "Parser.h"
void main(){
    char input[1024];
    //读取父文件夹下的src.txt文件，将其内容存入input数组
    FILE *fp = fopen("src.txt", "r");
    if(fp == NULL){
        printf("File open error!\n");
        return;
    }
    int i = 0;
    while(!feof(fp)){
        input[i] = fgetc(fp);
        i++;
    }
    input[i-1] = '\0';
    fclose(fp);

    printf("Input:\n%s\n", input);
    
    program(input);
}