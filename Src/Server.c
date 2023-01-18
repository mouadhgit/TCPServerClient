/**
 *  Copyright (C) 2022 
 *
 *  \file     Server.c
 *  \brief    Multiclients TCP server listens to address 127.0.0.1 on port 3456 support 10 Clients
 *  \author   Mouadh Dahech
 *  \platform x86 linux machine
 *
 *  \addtogroup
 *  \{
 */

/******************************************************************************
 * INCLUDE FILES
 ******************************************************************************/
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "queue.h"
#include <stdbool.h>

/******************************************************************************
 * DEFINES AND MACROS
 ******************************************************************************/
#define PORT 3456
#define SA struct sockaddr
#define THREAD_POOL_SIZE 2

#pragma pack(1)
typedef struct ClientFrame
{
    uint32_t FrameNumber;
    uint32_t Timestamp;
    uint16_t Pid;
    uint8_t Data[5];
} ClientFrame;

int clientsNumber = 0, clientSend = 0;
bool ServerBusy = 0;
pthread_t ThreadPool[THREAD_POOL_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conditionVar = PTHREAD_COND_INITIALIZER;

/******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 ******************************************************************************/
static void PrintClientFrame(ClientFrame *pClientFrame, uint8_t DATA_SIZE);
static void RecieveData(int *pconnfd);
static void *ThreadFunction(void *pParam);

int main(void)
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    for (int i = 0; i < THREAD_POOL_SIZE; i++)
    {
        pthread_create(&ThreadPool[i], NULL, ThreadFunction, NULL);
    }

    /*socket create and verification*/
    check(sockfd = socket(AF_INET, SOCK_STREAM, 0), "socket creation failed...\n", "Socket successfully created..\n");
    memset(&servaddr, 0, sizeof(servaddr));
    /*assign IP, PORT*/
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    /*Binding newly created socket to given IP and verification*/
    check(bind(sockfd, (SA *)&servaddr, sizeof(servaddr)), "socket bind failed...\n", "Socket successfully binded..\n");

    /*Now server is ready to listen and verification*/
    check(listen(sockfd, 1), "Listen failed...\n", "Server listening..\n");
    while (1)
    {
        len = sizeof(cli);

        /*Accept the data packet from client and verification*/
        printf("wait Client Accepted\n");
        connfd = accept(sockfd, (SA *)&cli, (socklen_t *)&len);
        clientsNumber++;
        printf("Client Accepted\n");
        if (clientsNumber > THREAD_POOL_SIZE)
        {
            ServerBusy = 1;
            check(send(connfd, &ServerBusy, sizeof(ServerBusy), 0), "Send failed\n", "Send done\n");
            MallocLogMsg(enqueueWaitsClients(connfd));
            printf("Send Flag busy\n");
        }
        else
        {
            clientSend++;
            ServerBusy = 0;
            check(send(connfd, &ServerBusy, sizeof(ServerBusy), 0), "Send failed\n", "Send done\n");
        }
        printf("clients Number = %d\n", clientsNumber);
        printf("clients send data = %d\n", clientSend);
        int *pclient = malloc(sizeof(int));
        *pclient = connfd;
        pthread_mutex_lock(&mutex);
        MallocLogMsg(enqueue(pclient));
        pthread_cond_signal(&conditionVar);
        pthread_mutex_unlock(&mutex);
    }

    close(sockfd);
    for (int i = 0; i < THREAD_POOL_SIZE; i++)
    {
        pthread_join(ThreadPool[i], NULL);
    }
    return 0;
}

/******************************************************************************
 * STATIC FUNCTIONS
 ******************************************************************************/

/** \brief Print recieved frame
 *  \param pbuff pointer to struct
 *  \param size_buff size of data
 *  \return None
 *****************************************************************************/
static void PrintClientFrame(ClientFrame *pClientFrame, uint8_t DATA_SIZE)
{
    assert(pClientFrame != NULL);
    assert(DATA_SIZE != 0);

    time_t now = (time_t)pClientFrame->Timestamp;
    struct tm ts;
    char buf[80];
    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = *localtime(&now);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z,", &ts);
    printf("%s ", buf);
    printf("packet number: %d ", pClientFrame->FrameNumber);
    printf("pid: %d ", pClientFrame->Pid);
    for (uint8_t i = 0; i < DATA_SIZE; i++)
    {
        printf(" %02X", pClientFrame->Data[i]);
    }
    printf("\n");
}

/** \brief Recieve & display client frame
 *  \param pconnfd pointer to file descriptor
 *  \return None
 *****************************************************************************/
static void RecieveData(int *pconnfd)
{
    assert(pconnfd != NULL);
    int connfd = *pconnfd, Readret;
    free(pconnfd);
    ClientFrame *pframe = malloc(sizeof(struct ClientFrame));
    MallocLogMsg(MalloCheck(pframe));
    memset(pframe->Data, 0, sizeof(pframe->Data));
    while (1)
    {
        Readret = read(connfd, pframe, sizeof(struct ClientFrame));
        if (Readret != 0)
        {
            PrintClientFrame(pframe, sizeof(pframe->Data));
        }
        else
        {
            printf("Client Disconnected\n");
            clientSend--;
            clientsNumber--;
            (clientsNumber >= THREAD_POOL_SIZE) ? (ServerBusy = 1) : (ServerBusy = 0);
            if (clientSend < THREAD_POOL_SIZE && ServerBusy)
            {
                ServerBusy = 0;
                check(send(dequeueWaitsClients(), &ServerBusy, sizeof(ServerBusy), 0), "Send failed\n", "Send done\n");
                printf("Send Flag not busy\n");
                clientSend++;
            }
            printf("clients Number = %d\n", clientsNumber);
            printf("clients send data = %d\n", clientSend);
            break;
        }
    }
    free(pframe);
}

/** \brief Threads routine
 *  \param pParam void pointer
 *  \return void pointer
 *****************************************************************************/
static void *ThreadFunction(void *pParam)
{
    while (1)
    {
        int *pclient;
        /*protect shared memory*/
        pthread_mutex_lock(&mutex);
        if ((pclient = dequeue()) == NULL)
        {
            pthread_cond_wait(&conditionVar, &mutex);
            pclient = dequeue();
        }
        pthread_mutex_unlock(&mutex);
        if (pclient != NULL)
        {
            RecieveData(pclient);
        }
    }
    return NULL;
}
