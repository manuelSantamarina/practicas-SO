#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "memorylist.c"
#include "common_utils.c"
#include <errno.h>
#include <stdint.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int priority(char tokens[], int ntokens);
int rederr(char tokens[], int ntokens);
int entorno(char tokens[], int ntokens);
int mostrarvar(char tokens[], int ntokens);
int cambiarvar(char tokens[], int ntokens);
int uid(char tokens[], int ntokens);


int _fork(char tokens[], int ntokens){
    //llamada a fork() y wait
    pid_t mypid;
    
    if ((mypid = fork())==-1){
        perror("Error: An error has occurred while calling fork()");
        return -1;
    }
    if(mypid != 0){
        waitpid(mypid,NULL,0);
        return 0;
    }else{
        return 0;
    }
    int status = 0;
}
//ejecutar sin crear proceso
//
int ejec(char tokens[], int ntokens){

    

    /*ejec prog arg1 arg2:
    Ejecuta sin crear un proceso nuevo... 
    ntokens: 1...n
    
    No hay suficientes tokens? */
    
    
    if (ntokens <= 1){
        //Solamente contiene la instrucción del programa.
        perror("A program name must be specified.");
        //we return an int value of -1 to signify that it failed
        return -1;
    }else{
        if(startsWith('@',tokens[1])){
            //Contiene un argumento de prioridad, lo cual quiere decir
            //que hay que cambiar la prioridad antes de ejecutar
            priority()
        }
        
        //Más de 1 token
        //usamos chpri para establecer que es una tarea en primer plano
        //execvp ejecuta
        
    }
}
    
int ejecpri(char tokens[], int ntokens){
}

//Ejecuta en primer plano
//usamos fork 
int fg(char tokens[], int ntokens){
    pid_t pid = getpid();

    //Ejecutamos fork sin parámetros extra
    if(_fork(NULL,0) == -1) return -1;
    if (pid == getpid()) return 0;

    return ejec(tokens,ntokens);
}

int fgpri(char tokens[], int ntokens);
int back(char tokens[], int ntokens);
int backpri(char tokens[], int ntokens);
int ejecas(char tokens[], int ntokens);
int fgas(char tokens[], int ntokens);
int bgas(char tokens[], int ntokens);


int listjobs(char tokens[], int ntokens);
int job(char tokens[], int ntokens);
int borrarjobs(char tokens[], int ntokens);

priority(char tokens[], int ntokens){

}