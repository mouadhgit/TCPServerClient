/**
 *  Copyright (C) 2022 
 *
 *  \file     queue.h
 *  \brief    Queue linked list to store the coming clients connections, so that an available thread can finde it.
 *  \author   Mouadh Dahech
 *  \platform x86 linux machine
 *
 *  \addtogroup 
 *  \{
 */

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "check.h"

MalloCheckRet_t enqueue(int *client_socket);
int *dequeue(void);
MalloCheckRet_t enqueueWaitsClients(int FdClient);
int dequeueWaitsClients(void);
#endif
