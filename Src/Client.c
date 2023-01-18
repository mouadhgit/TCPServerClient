/**
 *  Copyright (C) 2022
 *
 *  \file     Client.c
 *  \brief    TCP client send 15 bytes of random data to address 127.0.0.1 on port 3456
 *  \author   Mouadh Dahech
 *  \platform x86 linux machine
 *
 *  \addtogroup
 *  \{
 */

/******************************************************************************
 * INCLUDE FILES
 ******************************************************************************/
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <assert.h>
#include <check.h>
#include <stdbool.h>

/******************************************************************************
 * DEFINES AND MACROS
 ******************************************************************************/
#define PORT 3456
#define SA struct sockaddr

#pragma pack(1)
typedef struct ClientFrame
{
    uint32_t FrameNumber;
    uint32_t Timestamp;
    uint16_t Pid;
    uint8_t Data[5];
} ClientFrame;

/******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 ******************************************************************************/
static void FillClientFrame(ClientFrame *pClientFrame, uint8_t DATA_SIZE);
static void PrintClientFrame(ClientFrame *pClientFrame, uint8_t DATA_SIZE);
static void SendData(int sockfd);

int main(void)
{
    int sockfd;
    struct sockaddr_in servaddr;
    /*socket create and verification*/
    check(sockfd = socket(AF_INET, SOCK_STREAM, 0), "socket creation failed...\n", "Socket successfully created..\n");
    memset(&servaddr, 0, sizeof(servaddr));
    /*assign IP, PORT*/
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    /*connect the client socket to server socket*/
    printf("Waiting for server ..... \n");
    while (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)))
        ;
    printf("connecting to the server..\n");

    /*function for Send Data to server*/
    SendData(sockfd);

    /*close the socket*/
    close(sockfd);
}

/******************************************************************************
 * STATIC FUNCTIONS
 ******************************************************************************/

/** \brief Fill client frame bytes
 *  \param pClientFrame pointer to struct
 *  \param DATA_SIZE size of Data field
 *  \return None
 *****************************************************************************/
static void FillClientFrame(ClientFrame *pClientFrame, uint8_t DATA_SIZE)
{
    assert(pClientFrame != NULL);
    assert(DATA_SIZE != 0);

    pClientFrame->FrameNumber++;
    pClientFrame->Pid = getpid();
    pClientFrame->Timestamp = (uint32_t)time(NULL);
    for (uint8_t i = 0; i < DATA_SIZE; i++)
    {
        pClientFrame->Data[i] = ((uint8_t)(rand() % 255));
    }
}

/** \brief Print buffer data
 *  \param pClientFrame pointer to struct
 *  \param DATA_SIZE size of Data field
 *  \return None
 *****************************************************************************/
static void PrintClientFrame(ClientFrame *pClientFrame, uint8_t DATA_SIZE)
{
    assert(pClientFrame != NULL);
    assert(DATA_SIZE != 0);

    printf("packet number: %d ", pClientFrame->FrameNumber);
    printf("pid: %d ", pClientFrame->Pid);
    printf("Timestamp: %d ", pClientFrame->Timestamp);
    for (uint8_t i = 0; i < DATA_SIZE; i++)
    {
        printf(" %02X", pClientFrame->Data[i]);
    }
    printf("\n");
}

/** \brief Send 15 bytes struct to the server
 *  \param sockfd file descriptor for the new socket
 *  \return None
 *****************************************************************************/
static void SendData(int sockfd)
{
    bool ServerBusy = 0;
    ClientFrame *pframe = malloc(sizeof(struct ClientFrame));
    MallocLogMsg(MalloCheck(pframe));
    memset(pframe->Data, 0, sizeof(pframe->Data));
    pframe->FrameNumber = 0;
    check(read(sockfd, &ServerBusy, sizeof(ServerBusy)), "Read failed\n", "\n");
    while (ServerBusy)
    {
        printf("Server Busy\n");
        (read(sockfd, &ServerBusy, sizeof(ServerBusy)) != 0) ? printf("Server Free\n") : exit(1);
    }

    for (;;)
    {
        FillClientFrame(pframe, sizeof(pframe->Data));
        send(sockfd, pframe, sizeof(struct ClientFrame), 0);
        PrintClientFrame(pframe, sizeof(pframe->Data));
        sleep(1);
    }
    free(pframe);
}
