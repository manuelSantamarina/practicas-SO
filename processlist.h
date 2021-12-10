#include <stdbool.h>
#include <stddef.h>
#include <bits/types/time_t.h>

typedef struct tNodeP* pPosL;
typedef  int id;                                                        //Id of process
typedef char owner[512];                                                //Owner process
typedef char command[512];                                              //INI process
typedef int priorityP;                                                  //Priority
enum ptype {RUNNING,STOPPED,TERMINATEDNOR,TERMINATEDBYSIGNAL};          //State of process
typedef int signalP;

typedef struct pItemL{
    id id;
    time_t time;
    owner ownerP;
    command commandP;
    priorityP priorityP;
    enum ptype ptype;
    signalP signalP;                                                     //Only for process with TERMINATEDBYSIGNAL
}pItemL;

struct tNodeP{
    struct pItemL dataP;
    pPosL nextP;
};

typedef pPosL pList;


void createEmptyListP(pList*);
bool isEmptyListP(pList);
pItemL getItemP(pPosL , pList);
void updateItemP(pItemL, pPosL , pList*);
pPosL firstP(pList);
pPosL lastP(pList);
pPosL nextP(pPosL , pList*);
void deleteListP(pList*);
void deleteAtPositionP(pPosL , pList*);
bool insertItemP(pItemL,pPosL , pList*);