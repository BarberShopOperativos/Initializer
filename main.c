#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include "BarberShopStructures.h"

#define SHMSZ     50

int main()
{
    int chairsShmID, barbersShmID, cashiershmID;
    key_t _ChairsKey, _BarbersKey, _ChashierKey;
    _ChairsKey = 5677;
    _BarbersKey = 5678;
    _ChashierKey = 5679;
    int chairsQuantity, barbersQuantity;

    printf("Amount of chairs: ");
    scanf("%d", &chairsQuantity);
    Container *_ChairsQueue = createContainer();
    _ChairsQueue->maxLenght = chairsQuantity;

    printf("Amount of barbers: ");
    scanf("%d", &barbersQuantity);
    Container *_BarbersList = createContainer();
    _BarbersList->maxLenght = barbersQuantity;
    for(; barbersQuantity != 0; barbersQuantity--){
        addNodeToListContainer(_BarbersList,0);
    }

    Container *_ChashierQueue = createContainer();

    chairsShmID = createSegment(_ChairsKey);
    barbersShmID = createSegment(_BarbersKey);
    cashiershmID = createSegment(_ChashierKey);

    _ChairsQueue = attachSegment(chairsShmID,_ChairsQueue);
    _BarbersList = attachSegment(barbersShmID,_BarbersList);
    _ChashierQueue = attachSegment(cashiershmID,_ChashierQueue);

    exit(0);
    return 0;
}

int createSegment(key_t pKey){
    int shmID;
    if ((shmID = shmget(pKey, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    return shmID;
}
int attachSegment(int pShmID, Container * pContainer){
    if ((pContainer = shmat(pShmID, NULL, 0)) == (Container *) -1) {
        perror("shmat");
        exit(1);
    }
    return pContainer;
}


