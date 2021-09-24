#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <readline/readline.h>
#include <sys/utsname.h>
#include <unistd.h>

#include "dynamic_list.h"
//#include "dynamic_list.c"
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
    printf("Autores:\n");
    if(ntokens==1){
        printf("%s (%s)\n",AUTOR_1_N,AUTOR_1_L);
        printf("%s (%s)\n",AUTOR_2_N,AUTOR_2_L);

    }else if (ntokens==2 && !strcmp(tokens[1],"-l")){
        printf("%s\n",AUTOR_1_L);
        printf("%s\n",AUTOR_2_L);
    }else if (ntokens==2 && !strcmp(tokens[1],"-n")){
        printf("%s\n",AUTOR_1_N);
        printf("%s\n",AUTOR_2_N);
    }
        printf("\n");
    return 0;
}   

int carpeta(char *tokens[], int ntokens){
    //sacamos argumento de los tokens
    char cwd[100] = "current working directory";
    getcwd(cwd, 100);
    if(ntokens == 2 && strcmp(tokens[1],"-l")){
    chdir(tokens[1]);
    }else{
    printf("%s\n", cwd);
    }

    return 0;
}

int fecha(char *tokens[], int ntokens){
     time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    if(ntokens == 2 && strcmp(tokens[1],"-d")){
        //imprime la fecha en formato DD/MM/AAAA
        printf("%02d/%02d/%d\n", tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900);
        
        
    }else if(ntokens == 2 && strcmp(tokens[1], "-h")){
        //imprime la hora en formato hh:mm:ss
        printf("%02d:%02d:%02d\n",tm.tm_hour, tm.tm_min, tm.tm_sec);
    }else{
    	printf("%02d/%02d/%d\n", tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900);
    	printf("%02d:%02d:%02d\n",tm.tm_hour, tm.tm_min, tm.tm_sec);
    }
    
    return 0;
}

int infosis(char *tokens[], int ntokens){
     struct utsname sysinfo;

    if( uname( &sysinfo ) == -1 ) {
       perror( "uname" );
       return EXIT_FAILURE;
    }
    printf( "Nombre de sistema  : %s\n", sysinfo.sysname );
    printf( "Nombre de equipo: %s\n", sysinfo.nodename );
    printf( "Versión de kernel: %s\n", sysinfo.release );
    printf( "version name : %s\n", sysinfo.version );
    return EXIT_SUCCESS;
}
int pid(char *tokens[], int ntokens){
    if(ntokens == 1){
        printf("%d\n",getpid());
    }else if(ntokens == 2 && !strcmp(tokens[1], "-p")){
        printf("%d\n",getppid());
    }
    return 0;
}
int quit(char *tokens[], int ntokens) { 
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
    {"fecha", fecha},
    {"infosis", infosis},
    {"fin", quit},
    {"salir",quit},
    {"bye",quit},
    {"hist",hist},
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

int hist(tList L,char *tokens[],int ntokens){
    if(ntokens==1){
        if(!isEmptyList(L)){
            tPosL c = first(L);
            while (c!=last(L)){   
                printf("%c\n",getItem(c,L));

                c=next(c,L);
            }
        }else{
        printf("void\n");
        }
    }else if( ntokens == 2 && strcmp(tokens[1],"-c")){
        if(!isEmptyList(L)){
        tPosL c = first(L);
        tPosL c2;
        while (c!=last(L)){   
            c2 = next(c,L);
            deleteAtPosition(c,&L);
            c = c2;
        }
    }else if( ntokens == 2 && strcmp(tokens[1],"-N")){
        if(!isEmptyList(L)){
            tPosL c = first(L);
            printf("%c\n",getItem(c,L));
        }else{
            printf("void\n");
        }
    }
}

int main() {
    char *line;
    char *tokens[MAX_TOKENS];
    int ntokens;
    int end = 0;

    tList L;
    createEmptyList(&L);

    while(!end) {
        line = readline("> ");
        
        if(line!="hist" && line!="hist -c" && line!="hist -N"){
            insertItem(*line,&L);
        }
        
        ntokens = parseString(line, tokens);
        end = processCmd(tokens, ntokens);
        free(line);
    }

    return 0;
}
