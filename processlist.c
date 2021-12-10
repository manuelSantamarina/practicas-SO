#include "processlist.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void createEmptyListP(pList *L){
    *L=NULL;
}
bool isEmptyListP(pList L){
    return L==NULL;
}

pItemL getItemP(pPosL p,pList L){
    return p->dataP;
}
void updateItemP(pItemL d, pPosL p, pList *L){
    p->dataP=d;
}

pPosL nextP(pPosL p, pList *L){
    return p->nextP;
}
pPosL firstP(pList L){
    return L;
}

pPosL lastP(pList L){
    pPosL p;
    for(p=L;p->nextP!=NULL;p=p->nextP);
    return p;
}
void deleteListP(pList * L){
    pPosL p;
    while(*L!=NULL){
        p=*L;
        *L=(*L)->nextP;
        free(p);
    }
}

void deleteAtPositionP(pPosL p, pList *L){
    pPosL q;
    if(p==*L)
        *L=(*L)->nextP;
    else if(p->nextP==NULL){
        for(q=*L;q->nextP!=p;q=q->nextP);
        q->nextP=NULL;
    }
    else{
        q=p->nextP;
        p->dataP=q->dataP;
        p->nextP=q->nextP;
        p=q;
    }
    free(p);
}
bool createNodeP(pPosL *p){
    *p=malloc(sizeof(struct tNodeP));
    return *p!=NULL;
}
bool insertItemP(pItemL d, pPosL p, pList *L){
    pPosL q,r;

    if(!createNodeP(&q))
        return false;
    else{
        q->dataP=d;
        q->nextP=NULL;
        if(*L==NULL)
            *L=q;
        else{ if(p==NULL){
                for(r=*L;r->nextP!=NULL;r=r->nextP);
                r->nextP=q;
            }
            else{
                q->nextP=p->nextP;
                p->nextP=q;
                q->dataP=p->dataP;
                p->dataP=d;
            }
        }
        return true;
    }
}