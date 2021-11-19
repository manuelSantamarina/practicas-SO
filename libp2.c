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

int val1;
int val2;
int val3;
static int val4;
static int val5;
static int val6;


int mallocimpl(char *tokens[],int ntokens, mList *LM){
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
            printf("%d",i);
            
            tam = atoi(tokens[i]);
            printf("Se ha recibido un tamaño de %lu\n", tam );
            _contains_tam = true;
        }
        
    }
    if(!_contains_free && _contains_tam ){
        printf("Allocated %lu bytes on address %p\n",tam, addr);
        time_t t;
        /*mItemL listItem = {
            .maddress = addr,
            .size = tam,
            .time = localtime(&t),
            .type = MALLOC
        };*/
    
        /*TODO: include in list*/
        return 0;
    }else if (_contains_free && _contains_tam){
        free(addr);
        printf("Freed %lu bytes on address %p\n",tam,addr);
        /*TODO: remove from list*/
        return 0;
    }else if(!_contains_free && !_contains_tam){
        /*TODO: Print currently allocated segments*/
        printf("Currently allocated segments: \n");
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

int deallocimpl(char* tokens[], int ntokens){

    
    bool _contains_malloc = false;
    bool _contains_shared = false;
    bool _contains_mmap = false;
    bool _contains_dir = false;
    /*dealloc addr*/
    if(!_contains_malloc && !_contains_shared &&!_contains_mmap &&!_contains_dir){
        /*TODO: get allocation mode from list iterating by address parameter*/
        
        /**/
    }
    else if(ntokens == 1){
        /*Print list of currently allocated blocks.*/
    }
}

void SharedDelkey (char *args[]) /*arg[0] points to a str containing the key*/
{
    key_t clave;
    int id;
    char *key=args[0];

    if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf (" shared -delkey clave_valida\n");
        return;
    }
    if ((id=shmget(clave,0,0666))==-1){
        perror ("shmget: imposible obtener memoria compartida");
        return;
    }
    if (shmctl(id,IPC_RMID,NULL)==-1)
        perror ("shmctl: imposible eliminar memoria compartida\n");
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

int shared(char *tokens[], int ntokens, mList *LM){

    if(ntokens==1){
        printf("******Lista de bloques asignados shared para el proceso %d\n",getpid());
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
            printf("Imposible asignar memoria compartida clave %s",tokens[1]);
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

void recursiva (int n){
    char automatico[4086];
    static char estatico[4096];
    printf ("parametro n:%d en %p\n",n,&n);

    printf ("array estatico en:%p \n",estatico);
    printf ("array automatico en %p\n",automatico);
    n--;
    if (n>0)
        recursiva(n);
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
            if(x<32||x>126){
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
            if(x<32|x>126){
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
    void *p;
    int protection=0;
    if (arg[1]==NULL)
    {
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
    if ((p=MmapFichero(arg[0],protection))==NULL)
        perror("Imposible mapear fichero");
    else
        printf("fichero %s mapeado en %p\n", arg[0], p);
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
    Mmap(tokens, LM);
    return 0; 
}