#ifndef BARBERSHOPSTRUCTURES_H_INCLUDED
#define BARBERSHOPSTRUCTURES_H_INCLUDED

#endif // BARBERSHOPSTRUCTURES_H_INCLUDED

#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <pthread.h>

// Struct declaration
typedef struct Client Client;
typedef struct Node Node;
typedef struct Container Container;

/// <summary>
/// Struct to handle the matrix threads
/// </summary>
struct Client
{
    int id;
    bool isActive;
    bool hasPriority;
    bool isInChairsQueue;
    bool isInBarbersList;
    bool isInCashierQueue;
    Node *actualNode;
    pthread_t thread;
};

/// <summary>
/// Struct to handle queue nodes
/// </summary>
typedef struct Node
{
    int id;
    bool isOcupied;
    struct Node *next, *before;
    Client *actualClient;
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

Node *createNode (int pId);
Container *createContainer();
void addNodeToListContainer(Container *pContainer, int pId);
void printListContainer(Container *pContainer);
void addNodeToQueueContainer(Container *pContainer, int pId);
void removeQueueContainerHead(Container *pContainer);
void printQueueContainer(Container *pContainer);


