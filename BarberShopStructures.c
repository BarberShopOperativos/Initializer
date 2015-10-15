#include "BarberShopStructures.h"

/// <summary>
/// Method to allocate memory for a simpre node
/// </summary>
Node *createNode (int pId)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->id = pId;
    newNode->isOcupied = false;
    newNode->actualClient = false;
    newNode->next = newNode->before = NULL;
    return newNode;
}

/// <summary>
/// Method to allocate memory for the Container
/// </summary>
Container *createContainer()
{
    Container *container = malloc(sizeof(Container));
    container->firstNode = container->lastNode = NULL;
    return container;
}

/// <summary>
/// Struct method to add a node to a list container
/// </summary>
void addNodeToListContainer(Container *pContainer, int pId)
{
    Node *node = createNode(pId);
    if (pContainer->firstNode == NULL)
    {
        pContainer->firstNode = pContainer->lastNode = node;
    }
    else
    {
        pContainer->lastNode->next = node;
        pContainer->lastNode = node;
    }
    pContainer->length += 1;
}

/// <summary>
/// Struct method to print all the list container node values
/// </summary>
void printListContainer(Container *pContainer)
{
    Node *tempNode = pContainer->firstNode;

    while(tempNode != NULL)
    {
        printf("Node Value: %d: \n", tempNode->id);
        tempNode = tempNode->next;
    }

    printf("List End");
}

/// <summary>
/// Struct method to add a node to a queue container
/// </summary>
void addNodeToQueueContainer(Container *pContainer, int pId)
{
    Node *node = createNode(pId);

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
}

/// <summary>
/// Struct method remove the Queue head
/// </summary>
void removeQueueContainerHead(Container *pContainer)
{
    Node *removedNode = pContainer->lastNode;
    pContainer->lastNode = pContainer->lastNode->before;
    pContainer->lastNode->next = NULL;
    free(removedNode);
}


/// <summary>
/// Struct method to print all the queue container node values
/// </summary>
void printQueueContainer(Container *pContainer)
{
    Node *tempNode = pContainer->lastNode;

    while(tempNode != NULL)
    {
        printf("Node Value: %d: \n", tempNode->id);
        tempNode = tempNode->before;
    }

    printf("List End");
}



