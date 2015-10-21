#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include "BarberShopStructures.h"

#define SHMSZ 50

int createSegment(key_t pKey);
Container *attachContainerSegment(int pShmID);
int *attachIntSegment(int pShmID);

int main()
{
     // Declare values to store data in the segments
    int chairsShmID, barbersShmID, cashiershmID, specialClientsCounterShmID, stopClientsShmID, stopSpecialClientesShmID;
    key_t chairsKey, barbersKey, cashierKey, specialClientsCounterKey, stopClientsKey, stopSpecialClientsKey;


    chairsKey = 5677;
    barbersKey = 5678;
    cashierKey = 5679;
    specialClientsCounterKey = 5680;
    stopClientsKey = 5681;
    stopSpecialClientsKey = 5682;

    int chairsQuantity, barbersQuantity;
    int *specialClientsCounterPtr, *stopClientesPtr, *stopSpecialClientsPtr;
    Container *chairsQueue , *barbersList, *cashierQueue;

    // Initialize structures data
    chairsQueue = createContainer();
    barbersList = createContainer();
    cashierQueue = createContainer();

    // Create the neccessary segments
    chairsShmID = createSegment(chairsKey);
    barbersShmID = createSegment(barbersKey);
    cashiershmID = createSegment(cashierKey);
    specialClientsCounterShmID = createSegment(specialClientsCounterKey);
    stopClientsShmID = createSegment(stopClientsKey);
    stopSpecialClientesShmID = createSegment(stopSpecialClientsKey);

    // Attach the segment structures
    chairsQueue = attachContainerSegment(chairsShmID);
    barbersList = attachContainerSegment(barbersShmID);
    cashierQueue = attachContainerSegment(cashiershmID);
    specialClientsCounterPtr = attachIntSegment(specialClientsCounterShmID);
    stopClientesPtr = attachIntSegment(stopClientsShmID);
    stopSpecialClientsPtr = attachIntSegment(stopSpecialClientesShmID);

    chairsQueue->firstNode = chairsQueue->lastNode = NULL;
    barbersList->firstNode = barbersList->lastNode = NULL;
    cashierQueue->firstNode = cashierQueue->lastNode = NULL;
    *specialClientsCounterPtr = 0;
    *stopClientesPtr = 1;
    *stopSpecialClientsPtr = 1;

    printf("Amount of chairs: ");
    scanf("%d", &chairsQuantity);
    chairsQueue->maxLenght = chairsQuantity;

    printf("Amount of barbers: ");
    scanf("%d", &barbersQuantity);
    barbersList->maxLenght = barbersQuantity;

    int nodeIndex;
    for(nodeIndex = 0; nodeIndex < barbersQuantity; nodeIndex++){
        addNodeToListContainer(barbersList,nodeIndex);
    }

    printf("Cantidad sillas: %d \n", chairsQueue->maxLenght);
    printf("Cantidad barberos: %d \n", barbersList->maxLenght);

    exit(0);
    return 0;
}

int createSegment(key_t pKey) {

    int shmID;
    if ((shmID = shmget(pKey, SHMSZ, IPC_CREAT | 0666)) < 0) {
        printf("Error creating segment with key: %d", pKey);
        exit(1);
    }
    printf("Segment created ... \n");

    return shmID;
}

Container *attachContainerSegment(int pShmID){

    Container *container;
    if ((container = shmat(pShmID, NULL, 0)) == (Container *) -1) {
        printf("Error attaching segment with key: %d",pShmID);
        exit(1);
    }
    printf("Segment attached ... \n");
    return container;
}

int *attachIntSegment(int pShmID){

    int *pointer;
    if ((pointer = shmat(pShmID, NULL, 0)) == (int *) -1) {
        printf("Error attaching segment with key: %d",pShmID);
        exit(1);
    }
    printf("Segment attached ... \n");
    return pointer;
}

