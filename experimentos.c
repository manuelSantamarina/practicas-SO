#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/utsname.h>
int infosis();

void test(){
    
    infosis();
}

int infosis(){
     struct utsname sysinfo;

    if( uname( &sysinfo ) == -1 ) {
       perror( "uname" );
       return EXIT_FAILURE;
    }
    printf( "system name  : %s\n", sysinfo.sysname );
    printf( "node name    : %s\n", sysinfo.nodename );
    printf( "release name : %s\n", sysinfo.release );
    printf( "version name : %s\n", sysinfo.version );
    return EXIT_SUCCESS;
}
void experimentoTime(){

     time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  printf("%02d/%02d/%d\n", tm.tm_mday,tm.tm_mon+1,tm.tm_year+1900);
  printf("%02d:%02d:%02d\n",tm.tm_hour, tm.tm_min, tm.tm_sec);
}
void experimentoString(){
    char inputString[32] = "autores -a";
    const char delim[2] = " ";
    char *token;

    token = strtok(inputString, delim);

    while(token != NULL){
        printf(" %s\n", token);
        token = strtok(NULL,delim);
    }
}

int main(){
    test();
}
