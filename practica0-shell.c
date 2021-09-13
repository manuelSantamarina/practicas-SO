#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

void imprimirPrompt(){
    char name[32];
    gethostname(name,32);
    printf("%s@%s>",getlogin(),name);
}
void leerEntrada(){
    
}
void procesarEntrada();
int main(){
    bool terminado = false;
    
    while(!terminado){
        imprimirPrompt();
    }
    return 0;  
}