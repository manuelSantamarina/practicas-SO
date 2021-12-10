//
// Created by groug on 9/12/21.
//
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int isOption(char *token){
    if(token[0] == '-'){
        return 1;
    }else{
        return 0;
    }
}

/*int startsWith(char token, char *argument){
    if(strcmp(token,argument[0])){
        return 1;
    }else{
        return 0;
    }
}*/
int isPriority(char* token){

    if(isdigit(token) &&
       atoi(token) >= -20 && atoi(token) <= 19){
        return 1;
    }else{
        return 0;
    }
}
void debug(char string[]){
    printf("%s\n",string);
    fflush(stdout);
}
void debug_int(int integer){
    printf("%d\n",integer);
    fflush(stdout);
}