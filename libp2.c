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

int val1;
int val2;
int val3;
static int val4;
static int val5;
static int val6;


void printMallocBlocks(mList const *LM) {
    printf("Currently allocated segments with malloc for this process:\n");
    mPosL p = firstM(*LM);
    while (p!=NULL){
        if(p->dataM.type==MALLOC){
            //Se cogen movidos
            addres mem = p->dataM.maddres;
            printf(" \t\t 0x%lx\t\t\t\t", (uintptr_t)&mem);
            printf("%ld",p->dataM.size);
            time_t t = p->dataM.time;
            struct tm tm = *localtime(&t);
            printf(" %d-%02d-%02d %02d:%02d:%02d\t\t", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            printf("malloc\n");
        }
        p=p->nextM;
    }
}
void printMmapBlocks(mList const *LM) {
    printf("Currently allocated segments with mmap for this process:\n");
    mPosL p = firstM(*LM);
    while (p!=NULL){
        if(p->dataM.type==MMAP){
            //Se cogen movidos
            addres mem = p->dataM.maddres;
            printf(" \t\t 0x%lx\t\t\t\t", (uintptr_t)&mem);
            printf("%ld",p->dataM.size);
            time_t t = p->dataM.time;
            struct tm tm = *localtime(&t);
            printf(" %d-%02d-%02d %02d:%02d:%02d\t\t", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            printf("malloc\n");
        }
        p=p->nextM;
    }
}
void printSharedBlocks(mList const *LM);
void SharedDelkey (char *args[]);
void unmapMmapByFich(mList *LM, char fich[]);
void unmapMmap(mList *LM, bool found);
void deleteOnListByAddress(mList *LM, size_t addr);
void removeMallocFromList(mList *LM, size_t tam) {
    bool found = false;
    mPosL p = firstM(*LM);
    while (p!=NULL) {
        if (p->dataM.type == MALLOC && p->dataM.size == tam) {
            free(p->dataM.maddres);
            printf("Freed %lu bytes on address %p\n", p->dataM.size, p->dataM.maddres);
            deleteAtPositionM(p,LM);
            found = true;
            break;
        }

        p = p->nextM;
    }
    if (!found) {
        printMallocBlocks(LM);
    }
}

int mallocimpl(char *tokens[], int ntokens, mList *LM){
    size_t tam = 0; 
    char* addr = malloc(512);
    bool _contains_free = false;
    bool _contains_tam = false;

    //current item declaration
    
    /*Parameter eval: */
    //TODO: check function
    for(int i = 0; i < ntokens; i++){
        if(!strcmp(tokens[i],"-free")){
            _contains_free = true;
        }
        if(atoi(tokens[i]) != 0){
            tam = atoi(tokens[i]);
            _contains_tam = true;
        }
    }

    if(!_contains_free && _contains_tam ){
        printf("Allocated %lu bytes on address %p\n",tam, addr);

        mItemL item;
        time_t t = time(NULL);
        item.time= t;
        item.size= tam;
        item.type= MALLOC;
        //CORREGIR PORQUE SE EJECUTA SIEMPRE
        if(addr==NULL){
            printf("Imposible asignar memoria");
            perror(":No such file or directory");
        }else{
            item.maddres= malloc(tam);
            insertItemM(item,NULL,LM);
            //TODO: Da segfault
        }
        /*TODO: include in list*/
        return 0;
    }else if (_contains_free && _contains_tam){
        removeMallocFromList(LM, tam);
        return 0;
    }else if(!_contains_free && !_contains_tam){
        /*TODO: Print currently allocated segments*/
        printMallocBlocks(LM);
        return 0;
    }else {
        perror("The arguments introduced are incorrect. Correct usage is malloc [-free] [tam]\n");
        return 0;
    }
    /*Types of input: 
        malloc  !_contains_free && !_contains_tam X
        malloc 512 !_contains free _contains_tam X
        malloc -free 512 !contains free && _contains tam X
        Corregir: se repite varias veces la misma dirección.
        */
}

int deallocimpl(char* tokens[], int ntokens, mList *LM){

    
    bool _contains_malloc = false;
    bool _contains_shared = false;
    bool _contains_mmap = false;
    bool _contains_num = false;
    bool _contains_arg = false;
    int* addr = 0;
    size_t num;
    for(int i = 0; i < ntokens; i++){
        if(!strcmp(tokens[i],"-malloc")){
            _contains_malloc = true;
        }
        if(!strcmp(tokens[i],"-shared")){
            _contains_shared = true;
        }
        if(!strcmp(tokens[i],"-mmap")){
            _contains_mmap = true;
        }
        if(atoi(tokens[i]) != 0){
            num = atoi(tokens[i]);
            *addr = atoi(tokens[i]);
            printf("Se ha recibido un número %lu\n", num );
            _contains_num = true;
        }
    }


    /*dealloc addr*/
    if(_contains_malloc && _contains_num) {
        removeMallocFromList(LM, atoi(tokens[ntokens - 1]));
    }else if(_contains_shared && _contains_num) {
        deleteOnListByAddress(LM,*addr);
    }else if (_contains_mmap) {
        unmapMmapByFich(LM,tokens[ntokens-1]);
    }else if(_contains_num){
        deleteOnListByAddress(LM,*addr);
    }if(!_contains_malloc && !_contains_shared &&!_contains_mmap &&!_contains_arg){
        /*TODO: get allocation mode from list iterating by address parameter*/
        printMallocBlocks(LM);
        printSharedBlocks(LM);
        printMmapBlocks(LM);
    }

    return 0;
}



void * ObtenerMemoriaShmget (key_t clave, size_t tam)
{ /*Obtienen un puntero a una zaona de memoria compartida*/
/*si tam >0 intenta crearla y si tam==0 asume que existe*/
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;
    if (tam) /*si tam no es 0 la crea en modo exclusivo
esta funcion vale para shared y shared -create*/
        flags=flags | IPC_CREAT | IPC_EXCL;
/*si tam es 0 intenta acceder a una ya creada*/
    if (clave==IPC_PRIVATE) /*no nos vale*/
    {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
    return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno; /*si se ha creado y no se puede mapear*/
        if (tam) /*se borra */
            shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s);
/* Guardar En Direcciones de Memoria Shared (p, s.shm_segsz, clave.....);*/
    return (p);
}



void printSharedBlocks(const mList *LM) {
    printf("******Currently allocated segments with shared for this process: %d\n", getpid());
    mPosL p = firstM(*LM);
    while (p!=NULL){
        if(p->dataM.type==SHARED){
            key_t key1= p->dataM.key;
            int *mem=ObtenerMemoriaShmget(key1,p->dataM.size);
            printf(" \t\t 0x%lx\t\t\t\t", (uintptr_t)&mem);
            printf("%ld",p->dataM.size);
            time_t t = p->dataM.time;
            struct tm tm = *localtime(&t);
            printf(" %d-%02d-%02d %02d:%02d:%02d\t\t", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            printf("shared (key %d)\n",p->dataM.key);
        }
        p=p->nextM;
    }
}

void SharedDelkey(char **args) /*arg[0] points to a str containing the key*/
{
    key_t clave;
    int id;
    char *key=args[0];

    if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf (" shared -delkey clave_valida\n");
        return;
    }
    if ((id=shmget(clave,0,0666))==-1){
        perror ("shmget: imposible obtener memoria compartida\n");
        return;
    }
    if (shmctl(id,IPC_RMID,NULL)==-1)
        perror ("shmctl: imposible eliminar memoria compartida\n");
}

int shared(char *tokens[], int ntokens, mList *LM){

    if(ntokens==1){
        printSharedBlocks(LM);

    } else if(strcmp(tokens[1],"-create")==0){

        mItemL item;
        time_t t = time(NULL);
        item.time=  t;
        item.key= atoi(tokens[2]);
        key_t key1;
        key1= (item.key);
        item.size= atoi(tokens[3]);
        item.type=SHARED;

        int *mem=ObtenerMemoriaShmget(key1,item.size);
        if(mem==NULL){
            printf("Imposible asignar memoria compartida clave %s\n",tokens[1]);
            perror(":No such file or directory");
        }else{
            item.maddres=mem;
            insertItemM(item,NULL,LM);
        }
    } else if(strcmp(tokens[1],"-free")==0){
        int cont2=0;
        mPosL p = firstM(*LM);
        mItemL itemaux;
        while (p!=NULL){
            itemaux= getItemM(p,*LM);
            if(p->dataM.key==atoi(tokens[2])){
                cont2++;
                break;
            }
            p=p->nextM;
            cont2++;
        }
        if(cont2!=0){
            char *arg[10];
            arg[0]=tokens[2];
            if(shmdt(p->dataM.maddres) == -1){
                perror ("shmdt: imposible eliminar memoria compartida\n");
            }else{
                deleteAtPositionM(p,LM);
            }
        }
    }else if(strcmp(tokens[1],"-delkey")==0){
        int cont2=0;
        mPosL p = firstM(*LM);
        mItemL itemaux;

        while (p!=NULL){
            itemaux= getItemM(p,*LM);
            if(p->dataM.key==atoi(tokens[2])){
                cont2++;
                break;
            }
            p=p->nextM;
            cont2++;
        }
        if(cont2!=0){
            char *arg[10];
            arg[0]=tokens[2];
            SharedDelkey(arg);
            shmctl(shmget(p->dataM.key ,0, 0666),SHM_LOCK,(struct shmid_ds *) NULL);
        }
    } else{
        int cont2=0;
        mPosL p = firstM(*LM);
        mItemL itemaux;

        while (p!=NULL){
            itemaux= getItemM(p,*LM);
            if(p->dataM.key==atoi(tokens[1])){
                cont2++;
                break;
            }
            p=p->nextM;
            cont2++;
        }
        if(cont2==0){
            printf("Imposible asignar memoria compartida clave %s",tokens[1]);
            perror(":No such file or directory");
        }else{

            time_t t = time(NULL);
            itemaux.time=  t;
            key_t key1;
            key1= itemaux.key;

            int *mem=ObtenerMemoriaShmget(key1,0);
            if(mem==NULL){
                printf("Imposible asignar memoria compartida clave %s",tokens[1]);
                perror(":No such file or directory");
            }else{
                itemaux.maddres=mem;
                insertItemM(itemaux,NULL,LM);
            }
        }
    }
    return 0;
}

void dopmap (void) /*no arguments necessary*/
{ pid_t pid; /*ejecuta el comando externo pmap para */
    char elpid[32]; /*pasandole el pid del proceso actual */
    char *argv[3]={"pmap",elpid,NULL};
    sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror ("Imposible crear proceso");
        return;
    }
    if (pid==0){
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap");
        exit(1);
    }
    waitpid (pid,NULL,0);
}

int memoria(char *tokens[], int ntokens, mList *LM){

    //printf("%p", memoria);
    int val7;
    int val8;
    int val9;

    bool blocks=false,vars=false,funcs=false,pmap=false;

    for(int i=1;i<ntokens;i++){
        if(strcmp(tokens[i],"-blocks")==0){
            blocks=true;
        } else if(strcmp(tokens[i],"-vars")==0){
            vars=true;
        } else if(strcmp(tokens[i],"-funcs")==0){
            funcs=true;
        }else if(strcmp(tokens[i],"-all")==0){
            funcs=true;
            vars=true;
            blocks=true;
        }else if(strcmp(tokens[i],"-pmap")==0){
            pmap=true;
        }
    }

    if(ntokens==1){
        tokens[1]="-all";
        memoria(tokens,2,LM);
    } else{
        printf("******Lista de bloques asignados para el proceso %d\n",getpid());
        if(blocks){
            mPosL p = firstM(*LM);
            while (p!=NULL){
                if(p->dataM.type==SHARED){
                    key_t key1= p->dataM.key;
                    int *mem=ObtenerMemoriaShmget(key1,p->dataM.size);
                    printf(" \t\t 0x%lx\t\t\t\t", (uintptr_t)&mem);
                    printf("%ld",p->dataM.size);
                    time_t t = p->dataM.time;
                    struct tm tm = *localtime(&t);
                    printf(" %d-%02d-%02d %02d:%02d:%02d\t\t", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                    printf("shared (key %d)\n",p->dataM.key);
                } else if(p->dataM.type==MALLOC){
                    key_t key1= p->dataM.key;
                    int *mem=ObtenerMemoriaShmget(key1,p->dataM.size);
                    printf(" \t\t 0x%lx\t\t\t\t", (uintptr_t)&mem);
                    printf("%ld",p->dataM.size);
                    time_t t = p->dataM.time;
                    struct tm tm = *localtime(&t);
                    printf(" %d-%02d-%02d %02d:%02d:%02d\t\t", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                }else if(p->dataM.type==MMAP){
                    key_t key1= p->dataM.key;
                    int *mem=ObtenerMemoriaShmget(key1,p->dataM.size);
                    printf(" \t\t 0x%lx\t\t\t\t", (uintptr_t)&mem);
                    printf("%ld",p->dataM.size);
                    time_t t = p->dataM.time;
                    struct tm tm = *localtime(&t);
                    printf(" %d-%02d-%02d %02d:%02d:%02d\t\t", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                }
                p=p->nextM;
            }
        }
        if(vars){
            printf("Variables locales        %p,    %p,    %p\n",&val9,&val8,&val7);
            printf("Variables globales       %p,    %p,    %p\n",&val1,&val2,&val3);
            printf("Variables estaticas      %p,    %p,    %p\n",&val4,&val5,&val6);
        }
        if(funcs){
            printf("Variables programa      %p,    %p,    %p\n",memoria,shared, SharedDelkey);
            printf("Variables libreria      %p,    %p,    %p\n", strcmp, strcat, strcpy);
        }
        if(pmap){
            dopmap();
            printf("\n");
        }
    }
    return 0;
}

int volcarmem(char *tokens[], int ntokens){
    if(ntokens==2){
        unsigned char* addres;
        addres = (unsigned char *) strtol(tokens[1],0,16);
        for(int long long i=0;i<25;i++){
            printf("%3x\t", *addres);
            addres = addres +1;
        }
        printf("\n");
        addres = (unsigned char *) strtol(tokens[1],0,16);
        for(int i=0;i<25;i++){
            unsigned char st=*addres;
            int x= (int)st;
            if((x<32)||(x>126)){
                printf("%3c\t", 32);
            } else{
                printf("%3c\t", x);
            }
            addres = addres +1;
        }
        printf("\n");
    } else{
        unsigned char* addres;
        addres = (unsigned char *) strtol(tokens[1],0,16);
        for(int long long i=0;i< atoi(tokens[2]);i++){
            if(i%30==1){
                printf("\n");
            }
            printf("%3x\t", *addres);
            addres = addres +1;
        }
        printf("\n");
        addres = (unsigned char *) strtol(tokens[1],0,16);
        for(int long long i=0;i<atoi(tokens[2]);i++){
            unsigned char st=*addres;
            int x= (int)st;
            if(i%30==1){
                printf("\n");
            }
            if((x<32)|(x>126)){
                printf("%3c\t", 32);
            } else{
                printf("%3c\t", x);
            }
            addres = addres +1;
        }
        printf("\n");
    }
    return 0;
}

void *MmapFichero (char * fichero, int protection)
{
int df, map=MAP_PRIVATE,modo=O_RDONLY;
struct stat s;
void *p;
if (protection&PROT_WRITE) modo=O_RDWR;
if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
return NULL;
if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
return NULL;
/*Guardar Direccion de Mmap (p, s.st_size,fichero,df......);*/
return p;
}


void Mmap (char *arg[], mList *LM){
    char *perm;
    int protection=0;
    if (arg[1]==NULL){
         printf("******Lista de bloques asignados con mmap para el proceso %d\n",getpid());
        mPosL p = firstM(*LM);
        while (p!=NULL){
            if(p->dataM.type==SHARED){
                key_t key1= p->dataM.key;
                int *mem=ObtenerMemoriaShmget(key1,p->dataM.size);
                printf(" \t\t 0x%lx\t\t\t\t", (uintptr_t)&mem);
                printf("%ld",p->dataM.size);
                time_t t = p->dataM.time;
                struct tm tm = *localtime(&t);
                printf(" %d-%02d-%02d %02d:%02d:%02d\t\t", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                printf("shared (key %d)\n",p->dataM.key);
            }
            p=p->nextM;
        }
    if ((perm=arg[2])!=NULL && strlen(perm)<4) {
    if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
    if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
    if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
    }
    if ((p=MmapFichero(arg[1],protection))==NULL)
        perror("Imposible mapear fichero");
    else
        printf("fichero %s mapeado en %p\n", arg[1], p);
    }
}

#define LEERCOMPLETO ((ssize_t)-1)

ssize_t LeerFichero (char *fich, void *p, ssize_t n){
    ssize_t nleidos,tam=n; /*si n==-1 lee el fichero completo*/
    int df, aux;
    struct stat s;

    if (stat (fich,&s)==-1 || (df=open(fich,O_RDONLY))==-1)
        return ((ssize_t)-1);
    if (n==LEERCOMPLETO)
        tam=(ssize_t) s.st_size;
    if ((nleidos=read(df,p, tam))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return ((ssize_t)-1);
        }
    close (df);

    return (nleidos);
}

int mmapimpl(char* tokens[], int ntokens, mList *LM){
    if(!strcmp(tokens[1],"-free")){
        //mmap -free fich
        bool found = false;
        unmapMmap(LM, found);
    }else{
        //mmap fich with or without perms

        if(!access(tokens[1],F_OK)) {
            Mmap(tokens, LM);
        }else{
            perror(strcat("can't find file ",tokens[1]));
        }
    }

    return 0; 
}

void unmapMmap(mList *LM, bool found) {
    mPosL p = firstM(*LM);
    while (p!=NULL){
        if(p->dataM.type==MMAP){
            munmap(p->dataM.maddres,p->dataM.size);
            found = true;
        };

        p=p->nextM;
    }
    if(!found){
        printMmapBlocks(LM);
    }
}

void unmapMmapByFich(mList *LM, char *fich) {
    bool found = false;
    mPosL p = firstM(*LM);
    while (p!=NULL){
        if(strcmp(p->dataM.filename,fich)){
            munmap(p->dataM.maddres,p->dataM.size);
            found = true;
        };
        p=p->nextM;
    }
    if(!found){
        perror("List item not found!");
    }

}



void deleteOnListByAddress(mList *LM, size_t addr) {
    bool found = false;
    mPosL p = firstM(*LM);
    while(p!=NULL){
        if(p->dataM.maddres == addr){
            found = true;
            switch (p->dataM.type) {
                case MALLOC:
                    free(p->dataM.maddres);
                    break;
                case MMAP:
                    munmap(p->dataM.maddres,p->dataM.size);
                    break;
                case SHARED:
                    SharedDelkey(p->dataM.key);
                    break;
            }
        }
    }
    if(!found){
        perror("List item not found");
    }
}

void deleteOnListByAddress(mList *LM, size_t addr);

int llenarmem(char *tokens[], int ntokens){
    size_t cont=128;
    int byte = 0x42;
    if(ntokens==2){
        memset(tokens[1],byte,cont);
    } else{
        if(ntokens==3){
            cont=atoi(tokens[2]);
            memset(tokens[1],byte,cont);
        } else if(ntokens==4){
            cont=atoi(tokens[2]);
            byte= atoi(tokens[3]);
            memset(tokens[1],byte,cont);
        }
    }
    return 0;
}

int recursivar (int n){
    char automatico[4086];
    static char estatico[4096];
    printf ("parametro n:%d en %p\n",n,&n);
    printf ("array estatico en:%p \n",estatico);
    printf ("array automatico en %p\n",automatico);
    n--;
    if (n>0)
        recursivar(n);
    return 0;
}
int recursiva(char *tokens[], int ntokens){
    recursivar(atoi(tokens[1]));
    return 0;
}
int e_s(char *tokens[], int ntokens){
    ssize_t cont;
    if(ntokens>2){
        if(strcmp(tokens[1],"read")==0){
            if(ntokens<4){
                cont=-1;
            }else{
                cont=atoi(tokens[4]);
            }
            printf("leidos %zd bytes de %s\n",LeerFichero(tokens[2],tokens[3],cont),tokens[2]);
        }else{

            if(strcmp(tokens[1],"write")==0) {
                if (strcmp(tokens[2], "-o") == 0) {
                    cont = atoi(tokens[5]);
                    int fichero;
                    fichero = open(tokens[3], O_WRONLY);
                    if(fichero==-1){
                        fichero = open(tokens[3],O_CREAT);
                    }
                    printf("escritos %zd bytes en %s desde %s\n", write(fichero, tokens[4], cont), tokens[3],tokens[4]);
                } else {
                    cont = atoi(tokens[4]);
                    int fichero;
                    fichero = open(tokens[2], O_WRONLY);
                    if(fichero == -1){
                        fichero = open(tokens[3],O_CREAT);
                        printf("escritos %zd bytes en %s desde %s\n", write(fichero, tokens[3], cont), tokens[2],tokens[3]);
                    }else{
                        printf("Imposible escribir fichero:File exists\n");
                    }
                }
            }
        }
    }
    return 0;
}
