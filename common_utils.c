//
// Created by groug on 9/12/21.
//
#include <stdio.h>
int isOption(char *token){
    if(token[0] == '-'){
        return 1;
    }else{
        return 0;
    }
}

int startsWith(char *argument, char token){
    if(argument[0] == token){
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