/**
 *  Copyright (C) 2022 
 *
 *  \file     queue.c
 *  \brief    Queue linked list to store the coming clients connections, so that an available thread can finde it.
 *  \author   Mouadh Dahech
 *  \platform x86 linux machine
 *
 *  \addtogroup
 *  \{
 */

#include "queue.h"
#include <stdlib.h>

struct node
{
    struct node *next;
    int *FdClient;
};
typedef struct node node_t;

struct nodeClients
{
    struct nodeClients *next;
    int FdClient;
};
typedef struct nodeClients nodeClients_t;

node_t *head = NULL;
node_t *tail = NULL;

nodeClients_t *headClients = NULL;
nodeClients_t *tailClients = NULL;

/** \brief enqueue clients connections
 *  \param FdClient pointer to file descriptor 
 *  \return MalloCheckRet_t
 *****************************************************************************/
MalloCheckRet_t enqueue(int *FdClient)
{
    node_t *newnode = malloc(sizeof(node_t));
    MalloCheckRet_t retval = MalloCheck(newnode);
    newnode->FdClient = FdClient;
    newnode->next = NULL;
    /*if the queue is empty*/
    if (tail == NULL)
    {
        head = newnode;
    }
    else
    {
        tail->next = newnode;
    }
    tail = newnode;
    return retval;
}

/** \brief dequeue clients connections.
 *  \param void
 *  \return MalloCheckRet_t
 *****************************************************************************/
int *dequeue(void)
{

    if (head == NULL)
    {
        return NULL;
    }
    else
    {
        int *result = head->FdClient;
        node_t *temp = head;
        head = head->next;
        if (head == NULL)
        {
            tail = NULL;
        }
        if (temp != NULL)
        {
            free(temp);
            temp = NULL;
        }
        return result;
    }
}

/** \brief enqueue clients connections
 *  \param FdClient file descriptor
 *  \return MalloCheckRet_t
 *****************************************************************************/
MalloCheckRet_t enqueueWaitsClients(int FdClient)
{
    nodeClients_t *newnode = malloc(sizeof(nodeClients_t));
    MalloCheckRet_t retval = MalloCheck(newnode);
    newnode->FdClient = FdClient;
    newnode->next = NULL;
    /*if the queue is empty*/
    if (tail == NULL)
    {
        headClients = newnode;
    }
    else
    {
        tailClients->next = newnode;
    }
    tailClients = newnode;
    return retval;
}

/** \brief dequeue clients connections.
 *  \param void
 *  \return MalloCheckRet_t
 *****************************************************************************/
int dequeueWaitsClients(void)
{
    int result;

    if (headClients == NULL)
    {
        result = 0;
    }
    else
    {
        result = headClients->FdClient;
        nodeClients_t *temp = headClients;
        headClients = headClients->next;
        if (headClients == NULL)
        {
            tailClients = NULL;
        }
        if (temp != NULL)
        {
            free(temp);
            temp = NULL;
        }
    }
    return result;
}
