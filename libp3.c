int priority(char tokens[], int ntokens);
int rederr(char tokens[], int ntokens);
int entorno(char tokens[], int ntokens);
int mostrarvar(char tokens[], int ntokens);
int cambiarvar(char tokens[], int ntokens);
int uid(char tokens[], int ntokens);
int fork(char tokens[], int ntokens);
int ejec(char tokens[], int ntokens);
int ejecpri(char tokens[], int ntokens);
int fg(char tokens[], int ntokens);
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
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "memorylist.c"
#include <errno.h>
#include <stdint.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
}