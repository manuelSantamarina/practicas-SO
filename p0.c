#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/utsname.h>
#include <unistd.h>

#include "dynamic_list.c"
//Autores
/*  Manuel Santamariña Ruiz de León (manuel.santamarina)
    Mateo Rivela Santos (mateo.rivela)*/
#define AUTOR_1_N "Manuel Santamariña Ruiz de León"
#define AUTOR_1_L "manuel.santamarina"
#define AUTOR_2_N "Mateo Rivela Santos"
#define AUTOR_2_L "mateo.rivela"


#define MAX_LINE 1024
#define MAX_TOKENS 100
//prototypes:

int ayuda(char *tokens[], int ntokens);
int hist(char *tokens[],int ntokens, tList *L);
int comando(char *tokens[],int ntokens,tList *L);

void debug(char string[]){
    printf("%s\n",string);
    fflush(stdout);
}
void imprimirPrompt(){
    char name[32];
    char cwd[100] = "current working directory";
    getcwd(cwd, 100);

    gethostname(name,32);
    printf("%s@%s:%s$ ",getlogin(),name,cwd);
}
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

    if(ntokens==1){
        printf("Autores:\n");
        printf("%s (%s)\n",AUTOR_1_N,AUTOR_1_L);
        printf("%s (%s)\n",AUTOR_2_N,AUTOR_2_L);

    }else if (!strcmp(tokens[1],"-l")){
        printf("Autores:\n");
        printf("%s\n",AUTOR_1_L);
        printf("%s\n",AUTOR_2_L);
    }else if (!strcmp(tokens[1],"-n")){
        printf("Autores:\n");
        printf("%s\n",AUTOR_1_N);
        printf("%s\n",AUTOR_2_N);
    }else{
        printf("comando no reconocido\n");
    }
        printf("\n");
    return 0;
}   

int carpeta(char *tokens[], int ntokens){
    //sacamos argumento de los tokens
    char cwd[100] = "current working directory";
    getcwd(cwd, 100);

    if(ntokens==2){
        chdir(tokens[1]);
    }else{
        printf("%s\n", cwd);
    }
    return 0;
}


int fecha(char *tokens[], int ntokens){
     time_t t = time(NULL);
    struct tm tm = *localtime(&t);



    if(tokens[1]!=NULL){
        if(strcmp(tokens[1],"-d")==0){
            //imprime la fecha en formato DD/MM/AAAA
            printf("%02d/%02d/%d\n", tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900);
        }else if(strcmp(tokens[1], "-h")==0){
            //imprime la hora en formato hh:mm:ss
            printf("%02d:%02d:%02d\n",tm.tm_hour, tm.tm_min, tm.tm_sec);
            }
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
    exit(1);
    return 1;
}

struct cmd {
    char *cmd_name;
    //int (*cmd_fun)(char *tokens[], int ntokens);
    int (*cmd_fun)();
    char *help_text;
};

struct cmd cmds[] = {
    //help format: "Usage: <command usage>\n<cmd-description>\n\nOptions:\n<option>\t<noption-text>\n\n"
    {"autores", autores, "Usage: autores [-l|-n]\nPrints the names and logins of the program authors.\n\nOptions:\n-l\tPrint only the logins\n-n\tPrint only the names"},
    {"pid", pid,"Usage: pid [-p]\nPrints the pid of the process executing the shell.\n\nOptions:\n-p\tPrints the pid of the shell's parent process"},
    {"carpeta", carpeta,"Usage: carpeta [direct]\nChanges the current working directory of the shell to \"direct\" (using the \"chdir\" system call). When invoked without arguments it prints the current working directory (using the \"getcwd\" system call)."},
    {"fecha", fecha,"Usage: fecha [-d|-h]\nWithout arguments it prints both the current date and the current time.\n\nOptions:\n-d\tPrint the current date in the format DD/MM/YYYY.\n-h\tPrint the current time in the format hh:mm:ss"},
    {"infosis", infosis,"Usage: infosis\nPrints information on the machine running the shell (as obtained via the \"uname\" system call/library function)."},
    {"fin", quit,"Usage: fin\nEnds the shell"},
    {"salir",quit,"Usage: salir\nEnds the shell"},
    {"bye",quit,"Usage: bye\nEnds the shell"},
    {"ayuda", ayuda,"Usage: ayuda [cmd]\n\"ayuda\" displays a list of available commands. \"ayuda cmd\" gives a brief help on the usage of command \"cmd\""},
    {"hist", hist, "Usage: hist [-c | N]\nShows/clears the \"historic\" of commands executed by this shell. In order to do this, a list to store all the commands input to the shell must be implemented.\n\nOptions:\n-c\t Clear the historic\n-N\tPrints the first N commands"},
    //{"comando", comando, "Usage: comando N\n Repeats command number N (from historic list).\n"},
    {NULL, NULL}
};

int ayuda(char *tokens[], int ntokens){
    int i = 0;
    while(cmds[i].cmd_name != NULL){
        if(tokens[1] != NULL){
            if(strcmp(tokens[1],cmds[i].cmd_name) == 0){

                if(cmds[i].help_text != NULL){
                printf("%s\n\n",cmds[i].help_text);
                }else{
                    printf("No help found for command %s\n\n",tokens[0]);
                }
                return 0;
            
            }
        }
        i++;
    }
    if(tokens[1] != NULL){
    printf("No help found for input %s\n\n",tokens[1]);
    }else if(ntokens == 1){
        printf("Usage: ayuda [cmd]\n\"ayuda\" displays a list of available commands. \"ayuda cmd\" gives a brief help on the usage of command \"cmd\"\n\n");
    }
    return 0;
}
int hist(char *tokens[],int ntokens,tList *L){
    int i = 0;
    if(ntokens==1){
        if(!isEmptyList(*L)){
            tPosL c = first(*L);

            while (c!=last(*L)){
                printf("%i > %s %s\n",i,getItem(c,*L).command,getItem(c,*L).parameters);

                c=next(c,*L);
                i++;
            }
            printf("%i > %s %s\n",i,getItem(c,*L).command,getItem(c,*L).parameters);

        }else{
            printf("void2\n");
        }
    }else if(strcmp(tokens[1],"-c")==0){
        if(!isEmptyList(*L)){
            tPosL c = first(*L);
            tPosL c2;
        while (c!=last(*L)){
            c2 = next(c,*L);
            deleteAtPosition(c,L);
            c = c2;
        }
    }else if(strcmp(tokens[1],"-N")==0){
        if(!isEmptyList(*L)){
            tPosL c = first(*L);
            printf("%s %s\n",getItem(c,*L).command,getItem(c,*L).parameters);
        }else{
            printf("void1\n");
            }
        }
    }
    return i;
}




//si quiero insertar al final inserto en 0
int processCmd(char *tokens[], int ntokens, tList *L) {
    int i;
    tItemL item;
    
    strcpy(item.command,tokens[0]);
    strcpy(item.parameters,"");
    if(tokens[1]!=NULL&&ntokens==2){
        strcpy(item.parameters,tokens[1]);
    }


    
    for(i=0; cmds[i].cmd_name != NULL; i++) {

        if(strcmp(tokens[0], "hist")==0){
            hist(tokens,ntokens,L);
            return 0;
        }
        if(strcmp(tokens[0], "comando")==0){
            if(ntokens==2){
                comando(tokens,ntokens,L);
            }else{
                printf("comando no reconocido\n");
            }

            return 0;
        }
        if(strcmp(tokens[0], cmds[i].cmd_name) ==0){

            insertItem(item, NULL,L);

            return cmds[i].cmd_fun(tokens, ntokens);
        }
    }
    printf("Comando no reconocido\n");
    return 0;
}

int comando(char *tokens[],int ntokens,tList *L){
    int n;
    char *tr[MAX_TOKENS];
    tItemL item;
    tPosL p;
    char* comd;
    p=first(*L);
    //El error probablemente esté aquí
    for(int i = 0;i!= atoi(tokens[1]);i++){
        p=next(p,*L);
    }
    item=getItem(p,*L);
    comd=item.command;
    if(strcmp(item.parameters,"")!=0){
        strcat(comd," ");
        comd=item.parameters;
        n= parseString(comd,tr);
        processCmd( tr, n, L);

        return 1;
    }
    n= parseString(comd,tr);
    processCmd(tr, n, L);
    return 1;

}

int main(){
    char line[MAX_LINE];
    char *tokens[MAX_TOKENS];
    int ntokens;
    int end = 0;

    tList L;
    createEmptyList(&L);

    while(!end) {
        imprimirPrompt();
        fgets(line,MAX_LINE,stdin);
        ntokens = parseString(line, tokens);
        end = processCmd(tokens, ntokens, &L);

    }
    return 0;
}