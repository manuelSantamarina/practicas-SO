#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

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
//autores
int autores(char *tokens[], int ntokens) {
    //TODO
    printf("Autores:\n")
    if(/* -l */){
        printf("%s\n",AUTOR_1_L,AUTOR_2_L);
    }else if (/* -n */){
        printf("%s\n",AUTOR_1_N,AUTOR_2_N);
    }
        printf("\n");
    return 0;
}
int pid(char *tokens[], int ntokens){
    return getpid()
    if(/*-p*/){
        return getppid()
    }
}

int carpeta(char *tokens[], int ntokens){
    //sacamos argumento de los tokens

    if(/*hay argumento*/){
    chdir(/* argumento */);
    }else{
        //devolvemos el directorio actual
    printf("%s\n", getcwd(s, 100));
    }
}


int fin(char *tokens[], int ntokens) {
    return 1;
}

struct cmd {
    char *cmd_name;
    int (*cmd_fun)(char *tokens[], int ntokens);
};

struct cmd cmds[] = {
    {"autores", autores},
    {"pid", pid},
    {"carpeta", carpeta},
    {"fin", fin},
    {NULL, NULL}
};

int processCmd(char *tokens[], int ntokens) {
    int i;
    for(i=0; cmds[i].cmd_name != NULL; i++) {
        if(strcmp(tokens[0], cmds[i].cmd_name) ==0)
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
