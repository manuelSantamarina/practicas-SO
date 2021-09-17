#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define AUTOR_1
void imprimirPrompt(){
    char name[32];
    gethostname(name,32);
    printf("%s@%s:",getlogin(),name);
}
void leerEntrada(char buffer[]){
    fgets(buffer, 256, stdin);
    //buffer output test
    //printf("%s",fgets(inputBuff, 256, stdin));
}
void procesarEntrada(char buffer[]){
   const char delim[2] = " ";
   char *token;

    char operacion[15];

    int trozos
    /*token = strtok(buffer, delim);

    strcpy(operacion,token);

    printf("TEST: El string operacion vale %s\n",operacion);
    int contador = 0;

     token = strtok(NULL,delim);

    printf("TEST: Parámetros:\n");
   while (token != NULL){
       
       printf(" %s\n", token);
       token = strtok(NULL, delim);
   }*/

   switch(op){
   case 0: //autores
        if (/* condition */)
        {
            
        }
                
       break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break; 
   default:
       break;
   }
}
//utilities
bool isArgument(char string[]){
    bool isArgument = false;
}  

int main(){
    bool terminado = false;
    static char entrada[256] = "";
    while(!terminado){
        imprimirPrompt();
        leerEntrada(entrada);
        procesarEntrada(entrada);
        terminado = true;
    }
    return 0;  
}