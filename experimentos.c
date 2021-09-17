#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
void test(){
     time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  printf("%02d/%02d/%d\n", tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900);
  printf("%02d:%02d:%02d\n",tm.tm_hour, tm.tm_min, tm.tm_sec);

}

void experimentoString(){
    char inputString[32] = "autores -a";
    const char delim[2] = " ";
    char *token;

    token = strtok(inputString, delim);

    while(token != NULL){
        printf(" %s\n", token);
        token = strtok(NULL,delim);
    }
}

int main(){
    test();
}
