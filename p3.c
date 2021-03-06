#include "common_utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "dynamic_list.c"
#include "processlist.h"
#include <pwd.h>
#include <grp.h>
#include "libp2.c"
#include "libp3.c"

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
int hist(char *tokens[],int ntokens, tList *L,mList *LM);
int comando(char *tokens[],int ntokens,tList *L,mList *LM, char *argv[], char * envp[],pList *LP);
int crear(char *tokens[], int ntokens);
int borrar(char *tokens[], int ntokens);
int borrarrec(char *tokens[], int ntokens);
int listfich(char *tokens[], int ntokens);
int listdir(char *tokens[], int ntokens);
bool escarpeta(char *name);

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
void printCurrentDirectory(){
    char cwd[100] = "current working directory";
    getcwd(cwd, 100);
    printf("%s\n",cwd);
}
int carpeta(char *tokens[], int ntokens){
    if(ntokens==2){
        chdir(tokens[1]);
    }else{
        printCurrentDirectory();
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
        //help format: "Usage: <command usage>\n<cmd-description>\n\nOptions:\n<option>\t<noption-text>"
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
        //P2
        {"crear",crear,"Usage: crear [-f] name\nC Creates a file or directory in the file system. name is the name of the file (or directory) to be created. If no option is specified, a directory will be created. If name is not given, the name of the current working directory will be printed.\n\nOptions:\n-f\tIf specified, an empty directory is to be created."},
        {"borrar",borrar,"Usage: borrar name1 name2 ...\nDeletes files and/or empty directories. If no name is given, the current working directory will be printed."},
        {"borrarrec",borrarrec,"Usage: borrarrec name1 name2 ...\nDeletes files and/or non empty directories with all of their content. If no name is given, the current working directory will be printed"},
        {"listfich",listfich,"Usage: listfich [-long] [-link] [-acc] name1 name2 name3 ...\nGives info on files (or directories, or devices ... ) name1, name2 ... in one line per file. If no options are given, it prints the size and the name of each file. If no name is given, the name current working directory will be printed (as with the carpeta command)\n\nOptions:\n-long\tLong listing.it will print out the date of last modification (in format YYYY/MM/DD-HH:mm), number of links, owner, group, mode (drwx format), size and name of the file. If any of the names is a directory, information on the directory file itsself will be printed. The format to be used is: date number of links (inode number) owner group mode size name\n\n-link\t-link is only meaningful for long listings: if the file is a symbolic link the name of the file it points to is also printed. Format: date number of links (inode number) owner group mode size name->file the link points to.\n\n-acc\tLast access time will be used instead of last modification time."},
        {"listdir",listdir,"Usage: listdir [-reca] [-recb] [-hid] [-long] [-link] [-acc] name1 name2 ...\nLists the contents of directories with names name1, name2 ...  If any of name1, name2 . . . is not a directory, info on it will be printed EXACTLY as with command listfich. If no name is given, the name of the current working directory will be printed (as with the carpeta command).\n\nOptions:\n-long\tLong listing.it will print out the date of last modification (in format YYYY/MM/DD-HH:mm), number of links, owner, group, mode (drwx format), size and name of the file. If any of the names is a directory, information on the directory file itsself will be printed. The format to be used is: date number of links (inode number) owner group mode size name\n\n-link\t-link is only meaningful for long listings: if the file is a symbolic link the name of the file it points to is also printed. Format: date number of links (inode number) owner group mode size name->file the link points to.\n\n-acc\tLast access time will be used instead of last modification time.\n-long\tLong listing.it will print out the date of last modification (in format YYYY/MM/DD-HH:mm), number of links, owner, group, mode (drwx format), size and name of the file. If any of the names is a directory, information on the directory file itsself will be printed. The format to be used is: date number of links (inode number) owner group mode size name\n\n-link\t-link is only meaningful for long listings: if the file is a symbolic link the name of the file it points to is also printed. Format: date number of links (inode number) owner group mode size name->file the link points to.\n\n-acc\tLast access time will be used instead of last modification time.\n-reca\tWhen listing a directory's contents, subdirectories will be listed recursively BEFORE all the files in the directory. (if the -hid option is also given, hidden subdirectories will also get listed, except . and .. to avoid infinite recursion"},
        {"comando", comando, "Usage: comando N\n Repeats command number N (from historic list).\n"},
        {"malloc",mallocimpl,"Usage: malloc [-free] [tam]\n The shell allocates tam bytes using malloc and shows the memory address returned by malloc. This address, together with tam and the time of the allocation, must be kept in the aforementioned list. If tam is not specified the command will show the list of addresses allocated with the malloc command. malloc() requires an argument of size_t.\n\nOptions: -free: \tDeallocates one of the blocks of size tam that has been allocated with the command malloc. If no such block exists or if tam is not specified, the command will show the list of addresses allocated with the malloc command. Should there be more than one block of size tam it deallocates ONLY one of them(any)."},
        {"shared",shared,"shared: ...\n"},
        {"memoria",memoria,"memoria: ...\n"},
        {"volcarmem",volcarmem,"volcarmem: ...\n"},
        {"llenarmem",llenarmem,"llenarmem: ...\n"},
        {"recursiva",recursiva,"recursiva: ...\n"},
        {"e-s",e_s,"e-s: ...\n"},
        //P3
        {"priority",priority,"priority: ...\n"},
        {"rederr",rederr,"rederr: ...\n"},
        {"entorno",entorno,"entorno: ...\n"},
        {"mostrarvar",mostrarvar,"mostrarvar: ...\n"},
        {"cambiarvar",cambiarvar,"cambiarvar: ...\n"},
        {"uid",uid,"uid: ...\n"},
        {"listjobs",listjobs,"listjobs: ...\n"},
        {"job",job,"job: ...\n"},
        {"borrarjobs",borrarjobs,"borrarjobs: ...\n"},
        {"ejec",ejec,"exjec: ...\n"},
        {"ejecpri",ejecpri,"ejecpri: ...\n"},
        {"fork",_fork,"ejecpri: ...\n"}, //XX
        {"fg",fg,"ejecpri: ...\n"},
        {"fgpri",fgpri,"ejecpri: ...\n"},
        {"back",back,"ejecpri: ...\n"},
        {"backpri",backpri,"ejecpri: ...\n"},
        {"fgas",fgas,"ejecpri: ...\n"},
        {"bgas",bgas,"ejecpri: ...\n"},
        {"ejecas",ejecas,"ejecpri: ...\n"},
        {NULL, NULL}
};
char* getLastSegmentFromPath(char path[]){
    char* token;
    token = strtok(path,"/");
    char* previousFilename;
    while(token != NULL){
        previousFilename = token;
        token = strtok(NULL,"/");
    }
    return previousFilename;
}
char LetraTF (mode_t m){
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
    }
}

char * ConvierteModo2 (mode_t m){
    static char permisos[12];
    strcpy (permisos,"---------- ");
    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r'; /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r'; /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r'; /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s'; /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    return (permisos);
}

int listfich(char *tokens[], int ntokens){
    //boolean parameters
    bool _long = false;
    bool _link = false;
    bool _acc = false;

    for(int i = 0; i < ntokens; i++){
        if(!strcmp(tokens[i],"-long")){
            _long = true;
        }
        if(!strcmp(tokens[i],"-link")){
            _link = true;
        }
        if(!strcmp(tokens[i],"-acc")){
            _acc = true;
        }

        if(!isOption(tokens[i]) && i > 0){

            struct stat filestat;
            struct passwd *ownr;
            struct group *group;

            char path[128];
            strcpy(path,tokens[i]);
            stat(path, &filestat);

            char filename[16];
            strcpy(filename, getLastSegmentFromPath(path));
            if(!access(path, F_OK )){
                ownr = getpwuid(filestat.st_uid);
                group = getgrgid(filestat.st_gid);

                char symlink_s[PATH_MAX + 1];
                //char *rpath = realpath(path, symlink_s);
                //int is_link = S_ISLNK(filestat.st_mode);

                if(ntokens == 1){
                    printCurrentDirectory();
                }else{
                    //output long
                    if(_long && !_link){
                        if(_acc){
                            printf("%s %ld %ld %s %s %s %lld %s\n", ctime(&filestat.st_atime),(long)filestat.st_nlink, (long)filestat.st_ino,ownr->pw_name,group->gr_name,ConvierteModo2(filestat.st_mode),(long long)filestat.st_size, filename);
                        }else{
                            printf("%s %ld %ld %s %s %s %lld %s\n", ctime(&filestat.st_mtime),(long)filestat.st_nlink, (long)filestat.st_ino,ownr->pw_name,group->gr_name,ConvierteModo2(filestat.st_mode),(long long)filestat.st_size, filename);
                        }
                    }else if(_long && _link){
                        if(_acc){
                            printf("%s %ld %ld %s %s %s %lld %s %s -> %s\n", ctime(&filestat.st_atime),(long)filestat.st_nlink, (long)filestat.st_ino,ownr->pw_name,group->gr_name,ConvierteModo2(filestat.st_mode),(long long)filestat.st_size, filename, path, symlink_s);
                        }else{
                            printf("%s %ld %ld %s %s %s %lld %s %s -> %s\n", ctime(&filestat.st_mtime),(long)filestat.st_nlink, (long)filestat.st_ino,ownr->pw_name,group->gr_name,ConvierteModo2(filestat.st_mode),(long long)filestat.st_size, filename, path, symlink_s);
                        }
                    }else{
                        printf("%lld %s\n",(long long)filestat.st_size, filename);
                    }
                }
                //it exists! printf("%ld %s \n",filestat.st_size, filename);
            }else{
                debug(path);
                perror("");
            }

        }
    }

    //three types of output line
    //short list will give:
    //gives info on files in one line per file
    //print output
    return 0;
}
int ayuda(char *tokens[], int ntokens){
    int i = 0;
    while(cmds[i].cmd_name != NULL){
        if(tokens[1] != NULL){
            if(strcmp(tokens[1],cmds[i].cmd_name) == 0){

                if(cmds[i].help_text != NULL){
                    printf("%s\n\n",cmds[i].help_text);
                }else{
                    perror("No help found for that command");
                }
                return 0;

            }
        }
        i++;
    }
    if(tokens[1] != NULL){
        perror("No help found for that command");
    }else if(ntokens == 1){
        printf("Usage: ayuda [cmd]\n\"ayuda\" displays a list of available commands. \"ayuda cmd\" gives a brief help on the usage of command \"cmd\"\n\n");
    }
    return 0;
}
int hist(char *tokens[],int ntokens,tList *L,mList *LM){
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


int crear(char *tokens[], int ntokens){
    char cwd[100];
    getcwd(cwd, 100);
    if(ntokens == 1){
        printf("%s\n",cwd);
        mkdir(cwd,S_IRWXU);
    }else if(ntokens == 3 && !strcmp(tokens[1], "-f")){
        FILE* fichero; fichero = fopen(tokens[2], "wt");
        fclose(fichero);
    }else if(ntokens == 2){
        mkdir(tokens[1],S_IRWXU);
    }
    return 0;
}

int borrar(char *tokens[], int ntokens){
    if(ntokens == 1){
        printCurrentDirectory();
        return 0;
    }
    for(int i=1;i<ntokens;i++){
        remove(tokens[i]);
    }
    return 0;
}

void borrec(char *name){
    if(escarpeta(name)){
        DIR *d;
        struct dirent *dir;
        d = opendir(name);
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){
                    chdir(name);
                    if(escarpeta(dir->d_name)){

                        borrec(dir->d_name);
                        rmdir(dir->d_name);
                    } else{
                        remove(dir->d_name);
                    }
                    chdir("..");
                }
            }
            closedir(d);
            rmdir(name);
        }
    }
}

int borrarrec(char *tokens[], int ntokens){

    //1ºQue es lo que tiene q borrar -> carpeta entonces entre archivo enotnces borra
    //2ºSi es carpeta hace el primer paso con cada uno de los archivos y al terminar borra las carpetas en las que entro
    if(ntokens == 1){
        printCurrentDirectory();
        return 0;
    }
    for(int i=1;i<ntokens;i++){
        borrec(tokens[i]);
    }
    return 0;
}


bool escarpeta(char *name){

    struct stat st;
    lstat(name,&st);
    return S_ISDIR(st.st_mode);

}
void listfich_rebuild(char *name, bool lon, bool acc, bool link){
    if(lon){
        char *tokens[] = {"listfich", "-long", name};
        char ntokens=3;
        listfich(tokens,ntokens);
    }
    if (lon && acc){
        char *tokens[] = {"listfich",name, "-long", "-acc"};
        char ntokens =4;
        listfich(tokens,ntokens);
    }
    if (lon && acc && link){
        char *tokens[] = {"listfich",name, "-long", "-acc", "-link"};
        char ntokens =5;
        listfich(tokens,ntokens);
    }
    if(acc){
        char *tokens[] = {"listfich",name, "-acc"};
        char ntokens =3;
        listfich(tokens,ntokens);
    }
    if(acc && link){
        char *tokens[] = {"listfich",name, "-acc","-link"};
        char ntokens =4;
        listfich(tokens,ntokens);
    }

}
void listdirrec1(char *name,bool hid,bool lon,bool acc,bool link,char *tokens[],int ntokens){
    int i=0;
    if(escarpeta(name)){
        DIR *d;
        struct dirent *dir;
        char *carp[100];

        d = opendir(name);
        printf("************%s\n",name);
        if (d) {
            while ((dir = readdir(d)) != NULL) {

                struct stat st;
                lstat(dir->d_name, &st);

                chdir(name);
                if(escarpeta(dir->d_name)){
                    if(hid){

                        carp[i]=dir->d_name;
                        i++;

                        //listfich_rebuild(name, lon, acc, link);
                        printf("\t %6lu  %s \n", st.st_size,dir->d_name);
                    }else{
                        if(dir->d_name[0]!='.'){

                            carp[i]=dir->d_name;
                            i++;
                            //listfich_rebuild(name, lon, acc, link);
                            printf("\t %6lu  %s \n", st.st_size,dir->d_name);
                        }
                    }
                } else{
                    if(hid){
                        //listfich_rebuild(name, lon, acc, link);
                        printf("\t %6lu  %s \n", st.st_size,dir->d_name);
                    }else{
                        if(dir->d_name[0]!='.'){
                            // listfich_rebuild(name, lon, acc, link);
                            printf("\t %6lu  %s \n", st.st_size,dir->d_name);
                        }
                    }
                }
            }




            for(int j=0;j<i;j++) {
                char *nam="";

                if (strcmp(carp[j], "..") != 0 && strcmp(carp[j], ".") != 0) {
                    nam=carp[j];
                    listdirrec1(nam, hid, lon, acc, link,tokens,ntokens);
                }

            }
            chdir("..");
            closedir(d);
        }
    }
}


void listdirrec2(char *name,bool hid,bool lon,bool acc,bool link,char *tokens[],int ntokens){
    int i=0;
    if(escarpeta(name)){
        DIR *d;
        struct dirent *dir;
        char *carp[100];

        d = opendir(name);
        if (d) {
            while ((dir = readdir(d)) != NULL) {

                struct stat st;
                lstat(dir->d_name, &st);
                chdir(name);
                if(escarpeta(dir->d_name)){
                    if(hid){
                        carp[i]=dir->d_name;
                        i++;
                    }else{
                        if(dir->d_name[0]!='.'){
                            carp[i]=dir->d_name;
                            i++;
                        }
                    }
                }
            }

            for(int j=0;j<i;j++) {
                char *nam="";

                if (strcmp(carp[j], "..") != 0 && strcmp(carp[j], ".") != 0) {
                    nam=carp[j];
                    listdirrec1(nam, hid, lon, acc, link,tokens,ntokens);
                }
            }

            chdir("..");
            closedir(d);
            d = opendir(name);
            printf("************%s\n",name);
            while ((dir = readdir(d)) != NULL) {

                struct stat st;
                lstat(dir->d_name, &st);

                chdir(name);
                if(escarpeta(dir->d_name)){
                    if(hid){
                        printf("\t %6lu  %s \n", st.st_size,dir->d_name);
                    }else{
                        if(dir->d_name[0]!='.'){
                            printf("\t %6lu  %s \n", st.st_size,dir->d_name);
                        }
                    }
                } else{
                    if(hid){
                        printf("\t %6lu  %s \n", st.st_size,dir->d_name);
                    }else{
                        if(dir->d_name[0]!='.'){
                            printf("\t %6lu  %s \n", st.st_size,dir->d_name);
                        }
                    }
                }
            }
            chdir("..");
            closedir(d);
        }
    }
}


int listdir(char *tokens[],int ntokens){

    bool reca=false,recb=false,hid=false,lon=false,acc=false,link=false;
    int n=0;
    if(ntokens==1){
        carpeta(&tokens[0],1);
    }else{
        for(int i=1;i<ntokens;i++){
            if(strcmp(tokens[i],"-reca")==0){
                reca=true;
                n++;
            }else if(strcmp(tokens[i],"-recb")==0){
                recb=true;
                n++;
            }else if(strcmp(tokens[i],"-hid")==0){
                hid=true;
                n++;
            }else if(strcmp(tokens[i],"-long")==0){
                lon=true;
                n++;
            }else if(strcmp(tokens[i],"-acc")==0){
                acc=true;
                n++;
            } else if(strcmp(tokens[i],"-link")==0){
                link=true;
                n++;
            }
        }

        for(int i=n+1;i<ntokens;i++){
            if(!escarpeta(tokens[i])){
                perror("carpeta no encontrada");
            }else if(reca){
                listdirrec1(tokens[i],hid,lon,acc,link, tokens, ntokens);
            }else if(recb){
                listdirrec2(tokens[i],hid,lon,acc,link, tokens, ntokens);
            } else{

                if(escarpeta(tokens[i])) {
                    DIR *d;
                    struct dirent *dir;

                    d = opendir(tokens[i]);
                    printf("************%s\n", tokens[i]);
                    if (d) {
                        while ((dir = readdir(d)) != NULL) {

                            struct stat st;
                            lstat(dir->d_name, &st);

                            if(hid){
                                printf("\t  %s \n" ,dir->d_name);
                            }else{
                                if(dir->d_name[0]!='.'){
                                    printf("\t  %s \n",dir->d_name);
                                }
                            }
                        }
                        closedir(d);
                    }
                }
            }
        }
    }
    return 0;
}

//si quiero insertar al final inserto en 0
int processCmd(char *tokens[], int ntokens, tList *L,mList *LM,char *arg[],char *env[],pList *LP) {
    int i;
    tItemL item;
    strcpy(item.command,tokens[0]);
    strcpy(item.parameters,"");
    if(tokens[1]!=NULL&&ntokens==2){
        strcpy(item.parameters,tokens[1]);
    }

    for(i=0; cmds[i].cmd_name != NULL; i++) {

        if(strcmp(tokens[0], "hist")==0){
            hist(tokens,ntokens,L,LM);
            return 0;
        }
        if(strcmp(tokens[0], "comando")==0){
            if(ntokens==2){
                comando(tokens,ntokens,L,LM,arg,env,LP);
            }else{
                printf("comando no reconocido\n");
            }
            return 0;
        }

        if(strcmp(tokens[0], cmds[i].cmd_name) ==0){
            insertItem(item, NULL,L);
            if(strcmp(tokens[0], "entorno")==0||strcmp(tokens[0], "mostrarvar")==0||strcmp(tokens[0], "cambiarvar")==0){
                return cmds[i].cmd_fun(tokens, ntokens,arg,env);
            }
            if(strcmp(tokens[0], "listjobs")==0||strcmp(tokens[0], "job")==0||strcmp(tokens[0], "borrarjobs")==0){
                return cmds[i].cmd_fun(tokens, ntokens,LP);
            }
            if(strcmp(tokens[0], "shared")==0||strcmp(tokens[0], "memoria")==0){
                return cmds[i].cmd_fun(tokens, ntokens,LM);
            }
            return cmds[i].cmd_fun(tokens, ntokens);
        }
    }
    perror("Comando no reconocido\n");
    return 0;
}
int comando(char *tokens[],int ntokens,tList *L,mList *LM,char *argv[], char * envp[],pList *LP){
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
    char *par;
    par = item.parameters;
    if(strcmp(item.parameters,"")!=0){
        strcat(comd," ");
        strcat(comd,par);
        n= parseString(comd,tr);
        processCmd( tr, n, L, LM,argv,envp,LP);

        return 1;
    }else{
        n=parseString(comd,tr);
        processCmd(tr, n, L, LM,argv,envp,LP);
        return 1;
    }
}

int main(int argc, char *argv[], char * envp[]){
    char line[MAX_LINE];
    char *tokens[MAX_TOKENS];
    int ntokens;
    int end = 0;

    tList L;
    createEmptyList(&L);
    mList LM;
    createEmptyListM(&LM);
    pList LP;
    createEmptyListP(&LP);

    while(!end) {
        imprimirPrompt();
        fgets(line,MAX_LINE,stdin);
        ntokens = parseString(line, tokens);
        end = processCmd(tokens, ntokens, &L,&LM,argv,envp,&LP);

    }
    return 0;
}

