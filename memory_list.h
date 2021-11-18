#include <stdbool.h>
#include <stddef.h>

typedef struct tNodeM* mPosL;

typedef  int *address;
typedef  size_t msize;
typedef int key;
enum mtype {MALLOC,MMAP,SHARED};

typedef struct mItemL{
    address maddress;
    size_t size;
    time_t time;
    enum mtype type;
    key key;
}mItemL;
struct tNodeM{
    struct mItemL dataM;
    mPosL nextM;
};
typedef mPosL mList;


void createEmptyListM(mList*);
bool isEmptyListM(mList);
mItemL getItemM(mPosL, mList);
void updateItemM(mItemL, mPosL, mList*);
mPosL firstM(mList);
mPosL lastM(mList);
mPosL nextM(mPosL, mList);
void deleteListM(mList*);
void deleteAtPositionM(mPosL, mList*);
bool insertItemM(mItemL,mPosL, mList*);