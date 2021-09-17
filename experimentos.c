#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(){
    char inputString[32] = "autores -a";
    const char delim[2] = " ";
    char *token;

    token = strtok(inputString, delim);

    while(token != NULL){
        printf(" %s\n", token);

        token = strtok(NULL,delim);
    }
    return 0;
}