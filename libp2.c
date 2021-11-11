#include <stdlib.h>
#include <stdbool.h>

int mallocimpl(char *tokens[],int ntokens){
    size_t tam = 512; 
    char* addr = malloc(512);
    bool _contains_free = false;
    bool _contains_tam = true;
    /*Parameter eval: */

    for(int i = 0; i < ntokens; i++){
        if(!strcmp(tokens[i],"-free")){
            _contains_free = true;
        }
        /*TODO: check if tam is a number and is present*/
    }
    if(!_contains_free && _contains_tam){
        printf("Allocated %lu bytes on address %p\n",tam, addr);
        /*TODO: include in list*/
    }else if (_contains_free && _contains_tam){
        free(addr);
        printf("Freed %lu bytes on address %p",tam,addr);
        /*TODO: remove from list*/
    }else if(!_contains_free && !_contains_tam){
        /*TODO: Print currently allocated segments*/
    }else{
        printf("The arguments introduced are incorrect. Correct usage is malloc [-free] [tam]\n");
    }
}

int mmapimpl(char tokens[], int ntokens){

    bool _contains_free = false;
    char* _dir = "";
    char* _perm = "";

    char* address = "";
   if(!_contains_free && /*contains a dir*/) {
   }else if(_contains_free){
       /*TODO*/
       
       char length = "";/*gets length from list or filesystem and _dir*/


       munmap(address, length);
        /*TODO: Removes it from the list.*/
    
    }if(!_contains_free && /*does not cointain a dir*/){
        /*TODO: prints blocks allocated by mmap*/
    }else{
        perror('Unrecognized argument format, please use mmap [-free] fich [perm]');
    }
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

    if(ntokens == 1){
        /*Print list of currently allocated blocks.*/
    }
}