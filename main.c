#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "ClientThread.h"

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
int *attachIntSegment(int pShmID);
void initializeSemaphores();

int main()
{

    initializeSemaphores();

     // Declare values to store data in the segments
    int chairsShmID, barbersShmID, cashiershmID, specialClientsCounterShmID, stopClientsShmID, stopSpecialClientesShmID,
        chairsQuantityShmID, barbersQuantityShmID, nodeShmID,
        chairsQuantity, barbersQuantity, cashierQueueSize = 100,
        *specialClientsCounterPtr, *stopClientesPtr, *stopSpecialClientsPtr, *chairsQuantityPtr, *barbersQuantityPtr,
        *cashierQueuePtr,*chairsQueuePtr, *barbersListPtr;
    key_t chairsKey, barbersKey, cashierKey, specialClientsCounterKey, stopClientsKey,
        stopSpecialClientsKey,chairsQuantityKey,barbersQuantityKey;

    // Segment keys to be used
    chairsKey = 5677;
    barbersKey = 5678;
    cashierKey = 5679;
    specialClientsCounterKey = 5680;
    stopClientsKey = 5681;
    stopSpecialClientsKey = 5682;
    chairsQuantityKey = 5683;
    barbersQuantityKey = 5684;

    // Get the maximun amount of chairs
    printf("Ingrese la cantidad de sillas: ");
    scanf("%d", &chairsQuantity);

    // Get the maximum amout of barbers
    printf("Ingrese la cantidad de barberos: ");
    scanf("%d", &barbersQuantity);

    // Initialize necessary arrays
    int chairsQueue[chairsQuantity];
    chairsQueuePtr = chairsQueue;

    int barbersList[barbersQuantity];
    barbersListPtr = barbersList;

    int cashierQueue[CASHIER_QUEUE_SIZE];
    cashierQueuePtr = cashierQueue;

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
    chairsQueuePtr = attachIntSegment(chairsShmID);
    barbersListPtr = attachIntSegment(barbersShmID);
    cashierQueuePtr = attachIntSegment(cashiershmID);
    specialClientsCounterPtr = attachIntSegment(specialClientsCounterShmID);
    stopClientesPtr = attachIntSegment(stopClientsShmID);
    stopSpecialClientsPtr = attachIntSegment(stopSpecialClientesShmID);
    chairsQuantityPtr = attachIntSegment(chairsQuantityShmID);
    barbersQuantityPtr = attachIntSegment(barbersQuantityShmID);

    printf("Estructuras asignadas a los segmentos ... \n");

    // Set structures initial values
    *specialClientsCounterPtr = 0;
    *stopClientesPtr = 1;
    *stopSpecialClientsPtr = 1;
    *chairsQuantityPtr = chairsQuantity;
    *barbersQuantityPtr = barbersQuantity;


    //Initialize share structures with empty spaces
    clearControlArray(chairsQueuePtr,chairsQuantity,0);
    clearControlArray(barbersListPtr,barbersQuantity,0);
    clearControlArray(cashierQueuePtr,CASHIER_QUEUE_SIZE,0);

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

    createSemaphore(CHAIRS_SEM);
    createSemaphore(BARBERS_SEM);
    createSemaphore(CASHIER_SEM);
    createSemaphore(S_CLIENTS_COUNTER_SEM);

    // Write file header
    Semaphore *fileSem = createSemaphore(FILE_SEM);
    sem_wait(fileSem->mutex);
    writeFile(FILE_HEADER);
    sem_post(fileSem->mutex);

    printf("Semaforos creados exitosamente! \n");
}
