#include "ClientThread.h"

/// <summary>
/// Method to allocate memory for a single client
/// </summary>
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
    Semaphore *pSClientsCounterSem)
{
    // Initialize client
    ClientThread *client = malloc(sizeof(ClientThread));

    // Set client props
    client->id = pId;
    client->isActive = true;
    client->state = 1;
    client->hasPriority = pHasPriority;
    client->actualNode = pActualNode;
    client->nextClient = NULL;
    client->chairsQueue = pChairsQueue;
    client->barbersList = pBarbersList;
    client->cashiersQueue = pCashiersQueue;
    client->chairsSem = pChairsSem;
    client->barbersSem = pBarbersSem;
    client->cashierSem = pCashierSem;
    client->fileSem = pFileSem;
    client->sClientsCounterSem = pSClientsCounterSem;

    // Add thread to list
    addNodeToClientThreadList(pList,client);
    printf("Thread: %d:  creado \n", client->id);

    // Initialize client thread
    if(pthread_create(&client->thread, 0, threadRun, client))
    {
		fprintf(stderr, "Error creating thread\n");
	}

    return client;
}

/// <summary>
/// Method to allocate memory for the ClientThreadList structure
/// </summary>
ClientThreadList *createClientTreadList()
{
    ClientThreadList *list = malloc(sizeof(ClientThreadList));
    list->first = list->last = NULL;
    return list;
}

/// <summary>
/// Struct method to add an existing client to a list
/// </summary>
void addNodeToClientThreadList(ClientThreadList *pList, ClientThread *pClient)
{
    if (pList->first == NULL)
    {
        pList->first = pList->last = pClient;
    }
    else
    {
        pList->last->nextClient = pClient;
        pList->last = pClient;
    }
    pList->length += 1;
}

/// <summary>
/// Method to wait all threads to finish!
/// </summary>
void joinThreadList(ClientThreadList* pList)
{
	ClientThread *client = pList->first;
	while(client){
        pthread_join(client->thread,0);
		client = client->nextClient;
	}
}


void *threadRun(void  *threadArg)
{
    ClientThread *client = (ClientThread*)threadArg;

    while (client->isActive)
    {
        int randValue = generateRandomInRange(1,4);
        printf("Soy el thread: %d:  corriendo, rand %d \n", client->id, randValue);
        sleep(randValue);
    }
    pthread_exit(NULL);
    return NULL;
}

