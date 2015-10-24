#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "BarberShopStructures.h"
#include "SemaphoreStruct.h"

char CHAIRS_SEM[] = "ChairsSem";
char BARBERS_SEM[] = "BarbersSem";
char CASHIER_SEM[] = "CashierSem";
char FILE_SEM[] = "FileSem";
char S_CLIENTS_COUNTER_SEM[] = "SpecialClientsCounter";


int createSegment(key_t pKey, int pSegmentSize);
Container *attachContainerSegment(int pShmID);
int *attachIntSegment(int pShmID);
void initializeSemaphores();

int main()
{

    initializeSemaphores();

     // Declare values to store data in the segments
    int chairsShmID, barbersShmID, cashiershmID, specialClientsCounterShmID, stopClientsShmID, stopSpecialClientesShmID,
        chairsQuantityShmID, barbersQuantityShmID,
        chairsQuantity, barbersQuantity,
        *specialClientsCounterPtr, *stopClientesPtr, *stopSpecialClientsPtr, *chairsQuantityPtr, *barbersQuantityPtr;
    Container *chairsQueue , *barbersList, *cashierQueue;
    key_t chairsKey, barbersKey, cashierKey, specialClientsCounterKey, stopClientsKey, stopSpecialClientsKey,chairsQuantityKey,barbersQuantityKey;

    // Segment keys to be used
    chairsKey = 5677;
    barbersKey = 5678;
    cashierKey = 5679;
    specialClientsCounterKey = 5680;
    stopClientsKey = 5681;
    stopSpecialClientsKey = 5682;
    chairsQuantityKey = 5683;
    barbersQuantityKey = 5684;

    // Initialize structures data
    chairsQueue = createContainer();
    barbersList = createContainer();
    cashierQueue = createContainer();


    // Get the maximun amount of chairs
    printf("Ingrese la cantidad de sillas: ");
    scanf("%d", &chairsQuantity);

    // Get the maximum amout of barbers
    printf("Ingrese la cantidad de barberos: ");
    scanf("%d", &barbersQuantity);

    printf("Structuras inicializadas... \n");

    // Create the neccessary segments
    chairsShmID = createSegment(chairsKey,chairsQuantity * 25);
    barbersShmID = createSegment(barbersKey,barbersQuantity * 25);
    cashiershmID = createSegment(cashierKey,25);
    specialClientsCounterShmID = createSegment(specialClientsCounterKey,10);
    stopClientsShmID = createSegment(stopClientsKey,10);
    stopSpecialClientesShmID = createSegment(stopSpecialClientsKey,10);
    chairsQuantityShmID = createSegment(chairsQuantityKey,10);
    barbersQuantityShmID = createSegment(barbersQuantityKey,10);

    printf("Segmentos creados ... \n");

    // Attach the segment structures
    chairsQueue = attachContainerSegment(chairsShmID);
    barbersList = attachContainerSegment(barbersShmID);
    cashierQueue = attachContainerSegment(cashiershmID);
    specialClientsCounterPtr = attachIntSegment(specialClientsCounterShmID);
    stopClientesPtr = attachIntSegment(stopClientsShmID);
    stopSpecialClientsPtr = attachIntSegment(stopSpecialClientesShmID);
    chairsQuantityPtr = attachIntSegment(chairsQuantityShmID);
    barbersQuantityPtr = attachIntSegment(barbersQuantityShmID);

    printf("Estructuras asignadas a los segmentos ... \n");

    // Set structures initial values
    chairsQueue->firstNode = chairsQueue->lastNode = NULL;
    chairsQueue->maxLenght = chairsQuantity;
    barbersList->firstNode = barbersList->lastNode = NULL;
    barbersList->maxLenght = barbersQuantity;
    cashierQueue->firstNode = cashierQueue->lastNode = NULL;
    *specialClientsCounterPtr = 0;
    *stopClientesPtr = 1;
    *stopSpecialClientsPtr = 1;
    *chairsQuantityPtr = chairsQuantity;
    *barbersQuantityPtr = barbersQuantity;

    // Initialize the chairs queue spaces
    int chairIndex;
    for(chairIndex = 0; chairIndex < chairsQuantity; chairIndex++){
        addNodeToQueueContainer(chairsQueue,chairIndex);
    }

    // Initialize the barbers list spaces
    int barberIndex;
    for(barberIndex = 0; barberIndex < barbersQuantity; barberIndex++){
        addNodeToListContainer(barbersList,barberIndex);
    }

    printf("Recursos inicializados exitosamente! \n");
    exit(0);
    return 0;
}

/// <summary>
/// Creates a new shared memory segment
/// </summary>
int createSegment(key_t pKey, int pSegmentSize) {

    int shmID;
    if ((shmID = shmget(pKey, pSegmentSize, IPC_CREAT | 0666)) < 0) {
        printf("Error creando segmento con llave: %d \n", pKey);
        exit(1);
    }
    //printf("Segment created ... \n");
    return shmID;
}

/// <summary>
/// Attaches a container struct to a segment
/// </summary>
Container *attachContainerSegment(int pShmID){

    Container *container;
    if ((container = shmat(pShmID, NULL, 0)) == (Container *) -1) {
        printf("Error adjuntando segmento con llave: %d \n",pShmID);
        exit(1);
    }
    //printf("Segment attached ... \n");
    return container;
}

/// <summary>
/// Attaches an int value to a segment
/// </summary>
int *attachIntSegment(int pShmID){

    int *pointer;
    if ((pointer = shmat(pShmID, NULL, 0)) == (int *) -1) {
        printf("Error adjuntando segmento con llave: %d \n",pShmID);
        exit(1);
    }
    //printf("Segment attached ... \n");
    return pointer;
}

/// <summary>
/// Initialize all the semphores to be used
/// </summary>
void initializeSemaphores()
{
    printf("Inicializando Semaforos... \n");

    Semaphore *chairsSem = createSemaphore(CHAIRS_SEM);
    Semaphore *barbersSem = createSemaphore(BARBERS_SEM);
    Semaphore *cashierSem = createSemaphore(CASHIER_SEM);
    Semaphore *fileSem = createSemaphore(FILE_SEM);
    Semaphore *specialClientsCounterSem = createSemaphore(S_CLIENTS_COUNTER_SEM);

    printf("Semaforos creados exitosamente! \n");
}



