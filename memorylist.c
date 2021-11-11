#include "memorylist.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void createEmptyListM(tListM * L){
    *L=NULL;
}
bool isEmptyListM(tListM L){
    return L==NULL;
}
tItemLM getItemM(tPosLM p,tListM L){
    return p->dataM;
}
void updateItemM(tItemLM d, tPosLM p, tListM *L){
    p->dataM=d;
}
tPosLM nextM(tPosLM p, tListM L){
    return p->nextM;
}
tPosLM firstM(tListM L){
    return L;
}

tPosLM lastM(tListM L){
    tPosLM p;
    for(p=L;p->nextM!=NULL;p=p->nextM);
    return p;
}
void deleteListM(tListM * L){
    tPosLM p;
    while(*L!=NULL){
        p=*L;
        *L=(*L)->nextM;
        free(p);
    }
}

void deleteAtPositionM(tPosLM p, tListM *L){
    tPosLM q;
    if(p==*L)
        *L=(*L)->nextM;
    else if(p->nextM==NULL){
        for(q=*L;q->nextM!=p;q=q->nextM);
        q->nextM=NULL;
    }
    else{
        q=p->nextM;
        p->dataM=q->dataM;
        p->nextM=q->nextM;
        p=q;
    }
    free(p);
}
bool createNodeM(tPosLM *p){
    *p=malloc(sizeof(struct tNodeM));
    return *p!=NULL;
}
bool insertItemM(tItemLM d, tPosLM p, tListM *L){
    tPosLM q,r;
    if(!createNodeM(&q))
        return false;
    else{
        q->dataM=d;
        q->nextM=NULL;
        if(*L==NULL)
            *L=q;
        else{ if(p==NULL){
                for(r=*L;r->nextM!=NULL;r=r->nextM);
                r->nextM=q;
            }
            else{
                q->nextM=p->nextM;
                p->nextM=q;
                q->dataM=p->dataM;
                p->dataM=d;
            }
        }
        return true;
    }
}