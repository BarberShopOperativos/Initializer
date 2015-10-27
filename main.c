#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "ClientThread.c"
#include "SemaphoreStruct.c"
#include "BarberShopStructures.c"
#include "FileHandler.c"

#define INT_SEG_SIZE 5
#define STRUCT_SEG_SIZE 10
#define CASHIER_QUEUE_SIZE 100

char CHAIRS_SEM[] = "ChairsSem";
char BARBERS_SEM[] = "BarbersSem";
char CASHIER_SEM[] = "CashierSem";
char FILE_SEM[] = "FileSem";
char S_CLIENTS_COUNTER_SEM[] = "SpecialClientsCounter";
char FILE_HEADER[] = "--------------- Barber Shop Execution Log --------------- \n";


int createSegment(key_t pKey, int pSegmentSize);
Container *attachContainerSegment(int pShmID);
int *attachIntSegment(int pShmID);
Node *attachNodeSegment(int pShmID);
void initializeSemaphores();
Node *addNodeToSharedQueueContainer(Container *pContainer, int pId, key_t pKey);
Node *addNodeToSharedListContainer(Container *pContainer, int pId, key_t pKey);

int main()
{

    initializeSemaphores();

     // Declare values to store data in the segments
    int chairsShmID, barbersShmID, cashiershmID, specialClientsCounterShmID, stopClientsShmID, stopSpecialClientesShmID,
        chairsQuantityShmID, barbersQuantityShmID, nodeShmID,
        chairsQuantity, barbersQuantity, cashierQueueSize = 100,
        *specialClientsCounterPtr, *stopClientesPtr, *stopSpecialClientsPtr, *chairsQuantityPtr, *barbersQuantityPtr, *cashierQueuePtr;

    Container *chairsQueue , *barbersList;
    key_t chairsKey, barbersKey, cashierKey, specialClientsCounterKey, stopClientsKey,
        stopSpecialClientsKey,chairsQuantityKey,barbersQuantityKey,baseNodeKey;
    Node *actualNode;

    int cashierQueue[CASHIER_QUEUE_SIZE];
    cashierQueuePtr = cashierQueue;

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

    // Get the maximun amount of chairs
    printf("Ingrese la cantidad de sillas: ");
    scanf("%d", &chairsQuantity);

    // Get the maximum amout of barbers
    printf("Ingrese la cantidad de barberos: ");
    scanf("%d", &barbersQuantity);

    // Create the neccessary segments
    chairsShmID = createSegment(chairsKey,STRUCT_SEG_SIZE);
    barbersShmID = createSegment(barbersKey,STRUCT_SEG_SIZE);
    cashiershmID = createSegment(cashierKey,cashierQueueSize);
    specialClientsCounterShmID = createSegment(specialClientsCounterKey,INT_SEG_SIZE);
    stopClientsShmID = createSegment(stopClientsKey,INT_SEG_SIZE);
    stopSpecialClientesShmID = createSegment(stopSpecialClientsKey,INT_SEG_SIZE);
    chairsQuantityShmID = createSegment(chairsQuantityKey,INT_SEG_SIZE);
    barbersQuantityShmID = createSegment(barbersQuantityKey,INT_SEG_SIZE);

    printf("Segmentos creados ... \n");

    // Attach the segment structures
    chairsQueue = attachContainerSegment(chairsShmID);
    barbersList = attachContainerSegment(barbersShmID);
    cashierQueuePtr = attachIntSegment(cashiershmID);
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
    //cashierQueue->firstNode = cashierQueue->lastNode = NULL;
    *specialClientsCounterPtr = 0;
    *stopClientesPtr = 1;
    *stopSpecialClientsPtr = 1;
    *chairsQuantityPtr = chairsQuantity;
    *barbersQuantityPtr = barbersQuantity;

    // Initialize the chairs queue spaces

    baseNodeKey = barbersQuantityKey;
    int chairIndex;
    for(chairIndex = 0; chairIndex < chairsQuantity; chairIndex++)
    {
        baseNodeKey += 1;
        actualNode = addNodeToSharedQueueContainer(chairsQueue,chairIndex,baseNodeKey);
    }

    // Initialize the barbers list spaces
    int barberIndex;
    for(barberIndex = 0; barberIndex < barbersQuantity; barberIndex++)
    {
        baseNodeKey += 1;
        actualNode = addNodeToSharedListContainer(barbersList,barberIndex,baseNodeKey);
    }
    clearCashierQueue(cashierQueuePtr,CASHIER_QUEUE_SIZE,0);

    printf("Structuras inicializadas... \n");
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
/// Attaches a node struct to a segment
/// </summary>
Node *attachNodeSegment(int pShmID)
{

    Node *node;
    if ((node = shmat(pShmID, NULL, 0)) == (Node *) -1) {
        printf("Error adjuntando segmento con llave: %d \n",pShmID);
        exit(1);
    }
    //printf("Segment attached ... \n");
    return node;
}

/// <summary>
/// Initialize all the semphores to be used
/// </summary>
void initializeSemaphores()
{
    printf("Inicializando Semaforos... \n");

    createSemaphore(CHAIRS_SEM);
    createSemaphore(BARBERS_SEM);
    createSemaphore(CASHIER_SEM);
    createSemaphore(S_CLIENTS_COUNTER_SEM);
    Semaphore *fileSem = createSemaphore(FILE_SEM);

    sem_wait(fileSem->mutex);
    writeFile(FILE_HEADER);
    sem_post(fileSem->mutex);


    printf("Semaforos creados exitosamente! \n");
}

/// <summary>
/// Struct method to add a node to a shared queue container
/// </summary>
Node *addNodeToSharedQueueContainer(Container *pContainer, int pId, key_t pKey)
{
    Node *node = createNode(pId);
    int nodeShmID = createSegment(pKey,STRUCT_SEG_SIZE);
    node = attachNodeSegment(nodeShmID);
    node->id = pId;

    if (pContainer->firstNode == NULL)
    {
        pContainer->firstNode = pContainer->lastNode = node;
    }
    else
    {
        pContainer->firstNode->before = node;
        node->next = pContainer->firstNode;
        pContainer->firstNode = node;
    }
    pContainer->length += 1;
    return node;
}

/// <summary>
/// Struct method to add a node to a shared queue container
/// </summary>
Node *addNodeToSharedListContainer(Container *pContainer, int pId, key_t pKey)
{
    Node *node = createNode(pId);
    int nodeShmID = createSegment(pKey,STRUCT_SEG_SIZE);
    node = attachNodeSegment(nodeShmID);
    node->id = pId;

    if (pContainer->firstNode == NULL)
    {
        pContainer->firstNode = pContainer->lastNode = node;
    }
    else
    {
        pContainer->firstNode->before = node;
        node->next = pContainer->firstNode;
        pContainer->firstNode = node;
    }

    pContainer->length += 1;
    return node;
}
