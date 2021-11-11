#include <stdbool.h>
#include <stddef.h>

typedef struct tNodeM* tPosLM;
typedef char addresM[1024];
typedef size_t sizeM[1024];
typedef char timeM[1024];
enum typeM {MALLOC,MMAP,SHARED};
typedef struct tItemLM{
    addresM maddres;
    sizeM size;
    timeM time;
    enum typeM type;
}tItemLM;
struct tNodeM{
    struct tItemLM dataM;
    tPosLM nextM;
};
typedef tPosLM tListM;


void createEmptyListM(tListM*);
bool isEmptyListM(tListM);
tItemLM getItemM(tPosLM, tListM);
void updateItemM(tItemLM, tPosLM, tListM*);
tPosLM firstM(tListM);
tPosLM lastM(tListM);
tPosLM nextM(tPosLM, tListM);
void deleteListM(tListM*);
void deleteAtPositionM(tPosLM, tListM*);
bool insertItemM(tItemLM,tPosLM, tListM*);