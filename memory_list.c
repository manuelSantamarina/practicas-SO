#include "memory_list.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void createEmptyListM(mList * L){
    *L=NULL;
}
bool isEmptyListM(mList L){
    return L==NULL;
}

mItemL getItemM(mPosL p,mList L){
    return p->dataM;
}
void updateItemM(mItemL d, mPosL p, mList *L){
    p->dataM=d;
}
mPosL nextM(mPosL p, mList L){
    return p->nextM;
}
mPosL firstM(mList L){
    return L;
}

mPosL lastM(mList L){
    mPosL p;
    for(p=L;p->nextM!=NULL;p=p->nextM);
    return p;
}
void deleteListM(mList * L){
    mPosL p;
    while(*L!=NULL){
        p=*L;
        *L=(*L)->nextM;
        free(p);
    }
}

void deleteAtPositionM(mPosL p, mList *L){
    mPosL q;
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
bool createNodeM(mPosL *p){
    *p=malloc(sizeof(struct tNodeM));
    return *p!=NULL;
}
bool insertItemM(mItemL d, mPosL p, mList *L){
    mPosL q,r;

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