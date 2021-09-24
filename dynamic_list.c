#include "dynamic_list.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void createEmptyList(tList * L){
    *L=NULL;
}

bool isEmptyList(tList L){
    return L==NULL;
}

tItemL getItem(tPosL p,tList L){
    return p->data;
}

void updateItem(tItemL d, tPosL p, tList *L){
    p->data=d;
}

tPosL next(tPosL p, tList L){
    return p->next;
}

tPosL first(tList L){
    return L;
}

tPosL previous(tPosL p, tList L){
    tPosL q;
    if((p=NULL))
        q=NULL;
    else
        for(q=L;(q->next=!p);q=q->next);
    return q;
}

tPosL last(tList L){
    tPosL p;
    for(p=L;p->next!=NULL;p=p->next);
    return p;
}
void deleteList(tList * L){
    tPosL p;
    while(*L!=NULL){
        p=*L;
        *L=(*L)->next;
        free(p);
    }
}

void deleteAtPosition(tPosL p, tList *L){
    tPosL q;
    if(p==*L)
        *L=(*L)->next;
    else if(p->next==NULL){
        for(q=*L;q->next!=p;q=q->next);
        q->next=NULL;
    }
    else{
        q=p->next;
        p->data=q->data;
        p->next=q->next;
        p=q;
    }
    free(p);
}

bool createNode(tPosL *p){
    *p=malloc(sizeof(struct tNode));
    return *p!=NULL;
}

bool insertItem(tItemL d, tPosL p, tList *L){
    tPosL q,r;
    if(!createNode(&q))
        return false;
    else{
        q->data=d;
        q->next=NULL;
        if(*L==NULL)
            *L=q;
        else{ if(p==NULL){
            for(r=*L;r->next!=NULL;r=r->next);
            r->next=q;
            }
            else{
                q->next=p->next;
                p->next=q;
                q->data=p->data;
                p->data=d;
            }
        }
        return true;
    }
}