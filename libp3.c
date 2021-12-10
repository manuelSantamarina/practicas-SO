#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdint.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/resource.h>
#include <ctype.h>
#include "common_utils.h"
#define MAXVAR 1024

void MostrarEntorno (char **entorno, char * nombre_entorno){
    int i=0;
    while (entorno[i]!=NULL) {
        printf ("%p->%s[%d]=(%p) %s\n", &entorno[i],
                nombre_entorno, i,entorno[i],entorno[i]);
        i++;
    }
}

int priority(char *tokens[], int ntokens){

    if(ntokens==1){
        int p=getpid();
        printf("Prioridad del proceso %d es %d\n",p,getpriority(PRIO_PROCESS,p));
    }else if(ntokens==2){
        printf("Prioridad del proceso %s es %d\n",tokens[1],getpriority(PRIO_PROCESS, atoi(tokens[1])));
    }else if(ntokens==3){
        setpriority(PRIO_PROCESS,atoi(tokens[1]),atoi(tokens[2]));
    }else{
        perror("priority");
    }
    return 0;
}

int changePriority(char *priority){
    if(isPriority(priority)) return -1; //wrong argument
    int prio = atoi(&priority[1]);
    if(setpriority(PRIO_PROCESS,getpid(),prio) == -1){
        perror("Error when setting priority.");
    }
}

int i=-1,n,x;
char file[]="";

int rederr(char *tokens[], int ntokens){
    if(ntokens==1){
        if(strcmp(file,"")==0){
            printf("error estandar en fichero configuracion original\n");
        }else{
            printf("error estandar en fichero %s\n",file);
        }
    }else if(ntokens==2 && strcmp(tokens[1],"-reset")==0){

        if(i!=-1){
            if(dup2(x,2)==-1){
                perror("ERROR");
            } else if(close(i)==-1){
                perror("Can not close");
            }
            strcpy(file,"");
        }

    }else if(ntokens==2){
        i = open(tokens[1],O_RDWR|O_CREAT, 0777);
        x = dup(2);
        n=dup2(i,2);
        if(i==-1){
            perror("Can not open");
        }else if(n==-1){
            perror("ERROR");
        }
        strcpy(file,tokens[1]);
    }
    return 0;
}


extern char **environ;

int entorno(char *tokens[], int ntokens, char *argv[], char * envp[]){
    if(ntokens==1){
        MostrarEntorno(envp,"argv3");
    }else if(ntokens==2&&strcmp(tokens[1],"-environ")==0){
        MostrarEntorno(environ,"environ");
    }else if(ntokens==2&&strcmp(tokens[1],"-addr")==0){
        printf("environ:   %p (almacenado en %p)\n",*environ,&environ);
        printf("main arg3: %p (almacenado en %p)\n",*envp,&envp);
    }else{
        perror("entorno");
    }
    return 0;
}
int BuscarVariable (char * var, char *e[])
{
    int pos=0;
    char aux[MAXVAR];
    strcpy (aux,var);
    strcat (aux,"=");
    while (e[pos]!=NULL)
        if (!strncmp(e[pos],aux,strlen(aux)))
            return (pos);
        else
            pos++;
    errno=ENOENT;
    return(-1);
}
int CambiarVariable(char * var, char * valor, char *e[])
{
    int pos;
    char *aux;

    if ((pos=BuscarVariable(var,e))==-1)
        return(-1);
    if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL)
        return -1;
    strcpy(aux,var);
    strcat(aux,"=");
    strcat(aux,valor);
    e[pos]=aux;
    return (pos);
}




int mostrarvar(char *tokens[], int ntokens, char *argv[], char * envp[]){
    if(ntokens==2){
        int j= BuscarVariable(tokens[1],envp);
        if(j==-1){
            printf("No existe variable");
        } else{
            printf("Con arg3 main %s=(%p) @%p\n",envp[j],envp[j],&envp[j]);
            printf("Con environ %s=(%p) @%p\n",environ[j],environ[j],&environ[j]);
            printf("Con getenv %s(%p)\n", getenv(tokens[1]),getenv(tokens[1]));
        }
    }else{
        if(ntokens==1){
            MostrarEntorno(envp,"argv3");
        }else{
            perror("Argumentos no validos");
        }
    }
    return 0;
}




int cambiarvar(char *tokens[], int ntokens, char *argv[], char * envp[]){
    if(ntokens==4){
        if(strcmp(tokens[1],"-a")==0){
            CambiarVariable(tokens[2],tokens[3],envp);
        } else if(strcmp(tokens[1],"-e")==0){
            CambiarVariable(tokens[2],tokens[3],environ);
        } else if(strcmp(tokens[1],"-p")==0){
            int j;
            static char var[100]="";
            strcpy(var,tokens[2]);
            strcat(var,"=");
            strcat(var,tokens[3]);
            j=putenv(var);
            if(j!=0){
                perror("");
            }
        }
    }else{
        perror("Invalid arguments");
    }
    return 0;
}

char * NombreUsuario (uid_t uid)
{
    struct passwd *p;
    if ((p=getpwuid(uid))==NULL)
        return (" ??????");
    return p->pw_name;
}

uid_t UidUsuario (char * nombre)
{
    struct passwd *p;
    if ((p=getpwnam (nombre))==NULL)
        return (uid_t) -1;
    return p->pw_uid;
}

void MostrarUidsProceso (void)
{
    uid_t real=getuid(), efec=geteuid();
    printf ("Credencial real: %d, (%s)\n", real, NombreUsuario (real));
    printf ("Credencial efectiva: %d, (%s)\n", efec, NombreUsuario (efec));
}

void CambiarUidLogin (char * login)
{
    uid_t uid;
    if ((uid=UidUsuario(login))==(uid_t) -1){
        printf("login no valido: %s\n", login);
        return;
    }
    if (setuid(uid)==.1)
        printf ("Imposible cambiar credencial: %s\n", strerror(errno));
}

int uid(char *tokens[], int ntokens){
    if(ntokens==1){
        MostrarUidsProceso();
    } else if(ntokens==2){
        MostrarUidsProceso();
    }else{
        if(ntokens==3){
            if(strcmp(tokens[1],"-set")==0){
                char *name= NombreUsuario(atoi(tokens[2]));
                uid_t id= UidUsuario(name);
                if(setuid(id)==-1){
                    perror("");
                }
            }else{
                perror("");
            }
        }else{
            if(ntokens==4){
                if(strcmp(tokens[1],"-set")==0&&strcmp(tokens[2],"-l")==0){
                    CambiarUidLogin(tokens[3]);
                }else{
                    perror("");
                }
            }else{
                perror("");
            }
        }
    }
    return 0;
}

int updateListP(pList *PL){
    pPosL p= firstP(*PL);
    while (p!=NULL){
        if(p->dataP.ptype!=RUNNING){
            if(kill(p->dataP.id,0)){                            //No se si va
                p->dataP.ptype=RUNNING;
            }
        }
        p=p->nextP;
    }
}


int listjobs(char *tokens[], int ntokens,pList *PL){

    pItemL Item;
    Item.ptype=RUNNING;
    Item.id=getpid();
    strcpy(Item.commandP,"listjobs");
    Item.signalP=10;
    Item.time=time(NULL);
    strcpy(Item.ownerP,"mateo");
    Item.priorityP= getpriority(PRIO_PROCESS,Item.id);
    insertItemP(Item,NULL,PL);

    if(ntokens==1){
        pPosL p= firstP(*PL);
        while (p!=NULL){

            time_t t = p->dataP.time;
            struct tm tm = *localtime(&t);
            updateListP(PL);
            if(p->dataP.ptype==RUNNING){
                printf("Process: %d, priority:%d, owner:%s, command:%s, time:%d-%02d-%02d %02d:%02d:%02d, state:running,signal:%d \n",p->dataP.id,p->dataP.priorityP,p->dataP.ownerP,p->dataP.commandP,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,p->dataP.signalP);
            }else if(p->dataP.ptype==STOPPED){
                printf("Process: %d, priority:%d, owner:%s, command:%s, time:%d-%02d-%02d %02d:%02d:%02d, state:stopped,signal:%d \n",p->dataP.id,p->dataP.priorityP,p->dataP.ownerP,p->dataP.commandP,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,p->dataP.signalP);
            }else if(p->dataP.ptype==TERMINATEDNOR){
                printf("Process: %d, priority:%d, owner:%s, command:%s, time:%d-%02d-%02d %02d:%02d:%02d, state:terminated normally,signal:%d \n",p->dataP.id,p->dataP.priorityP,p->dataP.ownerP,p->dataP.commandP,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,p->dataP.signalP);
            }else if(p->dataP.ptype==TERMINATEDBYSIGNAL){
                printf("Process: %d, priority:%d, owner:%s, command:%s, time:%d-%02d-%02d %02d:%02d:%02d, state:terminated by signal ,signal:%d \n",p->dataP.id,p->dataP.priorityP,p->dataP.ownerP,p->dataP.commandP,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,p->dataP.signalP);
            }

            p=p->nextP;
        }
    }
    return 0;
}

int job(char *tokens[], int ntokens,pList *PL){
    updateListP(PL);
    if(ntokens==1){
        pPosL p= firstP(*PL);
        while (p!=NULL){
            time_t t = p->dataP.time;
            struct tm tm = *localtime(&t);
            if(p->dataP.ptype==RUNNING){
                printf("Process: %d, priority:%d, owner:%s, command:%s, time:%d-%02d-%02d %02d:%02d:%02d, state:running,signal:%d \n",p->dataP.id,p->dataP.priorityP,p->dataP.ownerP,p->dataP.commandP,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,p->dataP.signalP);
            }else if(p->dataP.ptype==STOPPED){
                printf("Process: %d, priority:%d, owner:%s, command:%s, time:%d-%02d-%02d %02d:%02d:%02d, state:stopped,signal:%d \n",p->dataP.id,p->dataP.priorityP,p->dataP.ownerP,p->dataP.commandP,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,p->dataP.signalP);
            }else if(p->dataP.ptype==TERMINATEDNOR){
                printf("Process: %d, priority:%d, owner:%s, command:%s, time:%d-%02d-%02d %02d:%02d:%02d, state:terminated normally,signal:%d \n",p->dataP.id,p->dataP.priorityP,p->dataP.ownerP,p->dataP.commandP,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,p->dataP.signalP);
            }else if(p->dataP.ptype==TERMINATEDBYSIGNAL){
                printf("Process: %d, priority:%d, owner:%s, command:%s, time:%d-%02d-%02d %02d:%02d:%02d, state:terminated by signal ,signal:%d \n",p->dataP.id,p->dataP.priorityP,p->dataP.ownerP,p->dataP.commandP,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,p->dataP.signalP);
            }
            p=p->nextP;
        }
    } else{
        if(ntokens==2){
            updateListP(PL);
            pPosL p= firstP(*PL);
            while (p!=NULL){
                if(atoi(tokens[1])==p->dataP.id){
                    time_t t = p->dataP.time;
                    struct tm tm = *localtime(&t);
                    if(p->dataP.ptype==RUNNING){
                        printf("Process: %d, priority:%d, owner:%s, command:%s, time:%d-%02d-%02d %02d:%02d:%02d, state:running,signal:%d \n",p->dataP.id,p->dataP.priorityP,p->dataP.ownerP,p->dataP.commandP,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,p->dataP.signalP);
                    }else if(p->dataP.ptype==STOPPED){
                        printf("Process: %d, priority:%d, owner:%s, command:%s, time:%d-%02d-%02d %02d:%02d:%02d, state:stopped,signal:%d \n",p->dataP.id,p->dataP.priorityP,p->dataP.ownerP,p->dataP.commandP,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,p->dataP.signalP);
                    }else if(p->dataP.ptype==TERMINATEDNOR){
                        printf("Process: %d, priority:%d, owner:%s, command:%s, time:%d-%02d-%02d %02d:%02d:%02d, state:terminated normally,signal:%d \n",p->dataP.id,p->dataP.priorityP,p->dataP.ownerP,p->dataP.commandP,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,p->dataP.signalP);
                    }else if(p->dataP.ptype==TERMINATEDBYSIGNAL){
                        printf("Process: %d, priority:%d, owner:%s, command:%s, time:%d-%02d-%02d %02d:%02d:%02d, state:terminated by signal ,signal:%d \n",p->dataP.id,p->dataP.priorityP,p->dataP.ownerP,p->dataP.commandP,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,p->dataP.signalP);
                    }
                }
                p=p->nextP;
            }
        } else if(ntokens==3){
            if(strcmp(tokens[1],"-fg")==0){

                pPosL p= firstP(*PL);
                while (p!=NULL){
                    if(atoi(tokens[2])==p->dataP.id){
                        if ((p->dataP.id=fork())==0){
                            exit(255);
                        }
                        waitpid (p->dataP.id,NULL,1);               //No va
                        printf("%d",n);
                        deleteAtPositionP(p,PL);
                        break;
                    }
                    p=p->nextP;
                }
            }else{
                perror("");
            }
        }else{
            pPosL p= firstP(*PL);
            while (p!=NULL){
                time_t t = p->dataP.time;
                struct tm tm = *localtime(&t);
                if(p->dataP.ptype==RUNNING){
                    printf("Process: %d, priority:%d, owner:%s, command:%s, time:%d-%02d-%02d %02d:%02d:%02d, state:running,signal:%d \n",p->dataP.id,p->dataP.priorityP,p->dataP.ownerP,p->dataP.commandP,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,p->dataP.signalP);
                }else if(p->dataP.ptype==STOPPED){
                    printf("Process: %d, priority:%d, owner:%s, command:%s, time:%d-%02d-%02d %02d:%02d:%02d, state:stopped,signal:%d \n",p->dataP.id,p->dataP.priorityP,p->dataP.ownerP,p->dataP.commandP,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,p->dataP.signalP);
                }else if(p->dataP.ptype==TERMINATEDNOR){
                    printf("Process: %d, priority:%d, owner:%s, command:%s, time:%d-%02d-%02d %02d:%02d:%02d, state:terminated normally,signal:%d \n",p->dataP.id,p->dataP.priorityP,p->dataP.ownerP,p->dataP.commandP,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,p->dataP.signalP);
                }else if(p->dataP.ptype==TERMINATEDBYSIGNAL){
                    printf("Process: %d, priority:%d, owner:%s, command:%s, time:%d-%02d-%02d %02d:%02d:%02d, state:terminated by signal ,signal:%d \n",p->dataP.id,p->dataP.priorityP,p->dataP.ownerP,p->dataP.commandP,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,p->dataP.signalP);
                }

                p=p->nextP;
            }
        }
    }
    return 0;
}

int borrarjobs(char *tokens[], int ntokens,pList *PL){

    if(ntokens==2){
        if(strcmp(tokens[1],"-term")){
            pPosL p= firstP(*PL);
            while (p!=NULL){
                if(p->dataP.ptype==TERMINATEDNOR){
                    deleteAtPositionP(p,PL);
                }
                p=p->nextP;
            }
        }
        if(strcmp(tokens[1],"-sig")){
            pPosL p= firstP(*PL);
            while (p!=NULL){
                if(p->dataP.ptype==TERMINATEDBYSIGNAL){
                    deleteAtPositionP(p,PL);
                }
                p=p->nextP;
            }
        }
        if(strcmp(tokens[1],"-all")){
            pPosL p= firstP(*PL);
            while (p!=NULL){
                if(p->dataP.ptype==TERMINATEDBYSIGNAL||p->dataP.ptype==TERMINATEDNOR){
                    deleteAtPositionP(p,PL);
                }
                p=p->nextP;
            }
        }
        if(strcmp(tokens[1],"-clear")){
            deleteListP(PL);                    //No estoy seguro de si es asi o borrar poco a poco con bucle
        }
    }
    return 0;
}
int _fork(char tokens[], int ntokens){
    //llamada a fork() y wait
    pid_t mypid;
    
    if ((mypid = fork())==-1){
        //Rama de error, fork nos devuelve -1 porque no se ha podido crear el proceso
        perror("Error: An error has occurred while calling fork()");
        return -1;
    }
    //difernenciamos el pid
    if(mypid != 0){
        //Si el pid es distinto de 0, espera el proceso padre a que acabe el hijo
        waitpid(mypid,NULL,0);
        return 0;
    }else{
        //Si el pid es igual a 0, devolvemos 0 porque estamos en el proceso hijo.
        return 0;
    }
    int status = 0;
}
//ejecutar sin crear proceso
//
int ejec(char *tokens[], int ntokens){
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
        if(isPriority(tokens[1])){
            //Contiene un argumento de prioridad, lo cual quiere decir
            //que hay que cambiar la prioridad antes de ejecutar
            priority((char**)tokens[1], 1);
            //ejecutamos execvp en el padre, execvp sobreescribe
            //la imagen de proceso actual con una nueva.
            //Llamamos a execvp, le pasamos file y argv con el primer
            //elemento que apunta al archivo asociado con el proceso
            //que estamos ejecutando.
            execvp(tokens[2],(char* *const) &tokens[2]);
            //PROBAR
            perror("Error: ejecución fallida");
            return -1; //devolvemos un -1 porque ha habido un fallo// en la ejecución
        }
        //Más de 1 token y no contiene prioridad
        execvp(tokens[1], (char* const* ) (&tokens[1]));
        //duda ??? \/\/\/\/
        perror("Ejecución fallida");
        ;
    }

}


int ejecpri(char* tokens[], int ntokens){
    //Ejecutamos ejec con los tokens de 1 a n
    //Creamos un bundle nuevo para llamar a ejec con los argumentos de ejecpri
    char *ejec_call_tokens[10];
    strcpy(ejec_call_tokens[0], "ejec");
    for(int i = 2; i < ntokens;i++){
        strcpy(ejec_call_tokens[i],tokens[i-1]);
    }
    if(isPriority(tokens[1])){
        changePriority(tokens[1]);
        ejec(ejec_call_tokens,ntokens);
        return 0;
    }else{
        return -1;
        perror("Error: does not contain a priority");
    }
}

//Ejecuta en primer plano
//usamos fork 
int fg(char *tokens[], int ntokens){
    pid_t pid = getpid();

    //Ejecutamos fork sin parámetros extra

    //Si el fork nos da -1 eso quiere decir que el fork ha fallado, devolvemos 0
    if(fork() == -1) return -1;

    if ((pid=fork())==0){
        if (ejec(tokens,ntokens))
            perror ("Cannot execute");
        exit(255); /*exec has failed for whatever reason*/
    }
    waitpid (pid,NULL,0);
}

//primer plano + cambiar prioridad con changePriority
int fgpri(char tokens[], int ntokens){

}
//segundo plano

int _fork_background(){
    pid_t child_pid;

    if((child_pid = fork())==-1){
        perror("Error: el fork no ha podido ejecutarse");
        return .1;
    }
    if (child_pid != 0){
        return child_pid; //
    }else{
        return 0; //si devolvemos 0 es porque estamos en el pid hijo
    }

}
int back(char *tokens[], int ntokens){
    int child_pid;

    child_pid = _fork_background();
    //Si el pid hijo da -1 eso quiere decir que no se ha podido completar
    if(child_pid == -1 ){
        return -1;
    }else if(child_pid == 0){
        return ejec(tokens,ntokens);
    }
    //TODO: metemos en la lista
    return -15;
};
//segundo plano + cambiar prioridad con changePriority
int backpri(char tokens[], int ntokens){

}
//ejecutar como login, la shell tiene que ser una setuid
//siendo ejecutada por otra que no sea el propietario

//ejecutar como login
int ejecas(char tokens[], int ntokens){

}
//ejecutar como x en primer plano
int fgas(char tokens[], int ntokens){

}
//ejecutar como x en segundo plano
int bgas(char tokens[], int ntokens){

}

