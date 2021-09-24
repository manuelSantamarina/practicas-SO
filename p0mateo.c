#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <unistd.h>

//Autores
#define AUTOR_1_N "Manuel Santamariña Ruiz de León"
#define AUTOR_1_L "manuel.santamarina"
#define AUTOR_2_N "Mateo Rivela Santos"
#define AUTOR_2_L "mateo.rivela"


#define MAX_LINE 1024
#define MAX_TOKENS 100


int parseString(char * cadena, char * trozos[]) {
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;

    return i;
}

//Métodos
//Autores

int autores(char *tokens[], int ntokens) {
    //TODO
    printf("Autores:\n");
    if(ntokens==1){
        printf("%s %s\n",AUTOR_1_N,AUTOR_1_L);
        printf("%s %s\n",AUTOR_2_N,AUTOR_2_L);

    }else if (ntokens==2 && strcmp(tokens[1],"-l")){
        printf("%s\n",AUTOR_1_N);
        printf("%s\n",AUTOR_2_N);
    }else if (ntokens==2 && strcmp(tokens[1],"-n")){
        printf("%s\n",AUTOR_1_L);
        printf("%s\n",AUTOR_2_L);
    }
        printf("\n");
    return 0;
}   

//int pid(char *tokens[], int ntokens){
//    return getpid();
//    if( /*-p */){
//    return getppid();
//    }
//}

int fin(char *tokens[], int ntokens) {
    return 1;
}

struct cmd {
    char *cmd_name;
    int (*cmd_fun)(char *tokens[], int ntokens);
};

struct cmd cmds[] = {
    {"autores", autores},
    //{"pid", pid},
    {"fin", fin},
    {NULL, NULL}
};

int processCmd(char *tokens[], int ntokens) {
    int i;
    for(i=0; cmds[i].cmd_name != NULL; i++) {
        if(strcmp(tokens[0], cmds[i].cmd_name) == 0)
            return cmds[i].cmd_fun(tokens, ntokens);
    }

    printf("Comando no reconocido\n");
    return 0;
}

int main() {
    char *line;
    char *tokens[MAX_TOKENS];
    int ntokens;
    int end = 0;

    while(!end) {
        line = readline("> ");
        ntokens = parseString(line, tokens); 
        end = processCmd(tokens, ntokens);
        free(line);
    }
}
