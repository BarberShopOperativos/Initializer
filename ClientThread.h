#ifndef CLIENTTHREAD_H_INCLUDED
#define CLIENTTHREAD_H_INCLUDED



#endif // CLIENTTHREAD_H_INCLUDED

#include "BarberShopStructures.h"
#include "SemaphoreStruct.h"

typedef struct ClientThread ClientThread;
typedef struct ClientThreadList ClientThreadList;

/// <summary>
/// Struct to handle the matrix threads
/// </summary>
struct ClientThread
{
    int id;
    bool isActive;
    bool hasPriority;
    int state; // State 1 -> Chairs Queue, State 2-> BarbersList, State 3 -> Cashier Queue, State 4 -> Finished
    pthread_t thread;
    Node *actualNode;
    ClientThread *nextClient;
    Container *chairsQueue;
    Container *barbersList;
    Container *cashiersQueue;
    Semaphore *chairsSem;
    Semaphore *barbersSem ;
    Semaphore *cashierSem;
    Semaphore *fileSem;
    Semaphore *sClientsCounterSem;
};

/// <summary>
/// Struct to handle queue or list to be used
/// </summary>
typedef struct ClientThreadList
{
    struct ClientThread *first, *last;
    int length;
    int maxLenght;
};

void *threadRun(void * threadArg);
ClientThreadList *createClientTreadList();
void addNodeToClientThreadList(ClientThreadList *pList, ClientThread *pClient);
void joinThreadList(ClientThreadList* pList);
ClientThread *createClient (
    int pId,
    bool pHasPriority,
    Node *pActualNode,
    ClientThreadList *pList,
    Container *pChairsQueue,
    Container *pBarbersList,
    Container *pCashiersQueue,
    Semaphore *pChairsSem,
    Semaphore *pBarbersSem,
    Semaphore *pCashierSem,
    Semaphore *pFileSem,
    Semaphore *pSClientsCounterSem);

