#include <stdlib.h>
#include <stdio.h>

void con_log(char string[]){
    printf("%s\n",string);
    fflush(stdout);
}
void con_log_int(int integer){
    printf("%d\n",integer);
    fflush(stdout);
}