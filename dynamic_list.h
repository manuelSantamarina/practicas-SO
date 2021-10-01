#include <stdbool.h>
#include <stddef.h>

typedef struct tNode* tPosL;
typedef char tCommand[256];
typedef char tParameters[10];
typedef struct tItemL{
    tCommand command;
    tParameters parameters;
}tItemL;
struct tNode{
    tItemL data;
    tPosL next;
};
typedef tPosL tList;


void createEmptyList(tList*);
bool isEmptyList(tList);
tItemL getItem(tPosL, tList);
void updateItem(tItemL, tPosL, tList*);
tPosL first(tList);
tPosL last(tList);
tPosL next(tPosL, tList);
void deleteList(tList*);
void deleteAtPosition(tPosL, tList*);
bool insertItem(tItemL,tPosL, tList*);
