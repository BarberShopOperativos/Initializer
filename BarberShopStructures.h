#ifndef BARBERSHOPSTRUCTURES_H_INCLUDED
#define BARBERSHOPSTRUCTURES_H_INCLUDED
#endif // BARBERSHOPSTRUCTURES_H_INCLUDED
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

// Struct declaration
typedef struct ClientThread ClientThread;
typedef struct ClientThreadList ClientThreadList;
typedef struct Node Node;
typedef struct Container Container;


/// <summary>
/// Struct to handle the matrix threads
/// </summary>
struct ClientThread
{
    int id;
    bool isActive;
    bool hasPriority;
    bool isInChairsQueue;
    bool isInBarbersList;
    bool isInCashierQueue;
    pthread_t thread;
    Node *actualNode;
    ClientThread *nextClient;
    Container *chairsQueue;
    Container *barbersList;
    Container *cashiersQueue;
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

/// <summary>
/// Struct to handle queue or list to be used
/// </summary>
typedef struct Container
{
    struct Node *firstNode, *lastNode;
    int length;
    int maxLenght;
};


/// <summary>
/// Struct to handle queue nodes
/// </summary>
typedef struct Node
{
    int id;
    bool isOcupied;
    struct Node *next, *before;
    ClientThread *actualClient;
};

void *threadRun(void * threadArg);
ClientThread *createClient (int pId, bool pHasPriority,
    Node *pActualNode, ClientThreadList *pList, Container *pChairsQueue,
    Container *pBarbersList,Container *pCashiersQueue);
ClientThreadList *createClientTreadList();
void addNodeToClientThreadList(ClientThreadList *pList, ClientThread *pClient);
Node *createNode (int pId);
Container *createContainer();
Node *addNodeToListContainer(Container *pContainer, int pId);
void printListContainer(Container *pContainer);
Node *addNodeToQueueContainer(Container *pContainer, int pId);
void removeQueueContainerHead(Container *pContainer);
void printQueueContainer(Container *pContainer);
void *threadRun(void  *threadArg);
void joinThreadList(ClientThreadList* pList);
int generateRandomInRange(int pMin, int pMax);
void addExistingNodeToListContainer(Container *pContainer, Node *pNode);
void addExistingNodeToQueueContainer(Container *pContainer, Node *pNode);
