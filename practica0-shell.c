#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

void imprimirPrompt(){
    char name[32];
    gethostname(name,32);
    printf("%s@%s>",getlogin(),name);
}
void leerEntrada(){
    char inputBuff[256];
    printf("%s",fgets(inputBuff, 256, stdin));
    
}
void procesarEntrada();

int main(){
    bool terminado = false;
    
    while(!terminado){
        imprimirPrompt();
        leerEntrada();
    }
    return 0;  
}