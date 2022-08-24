/**
 *  Copyright (C) 2022 Focus Corporation 
 *
 *  \file     Client.c
 *  \brief    TCP client send 15 bytes of random data to address 127.0.0.1 on port 8080
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

/******************************************************************************
* DEFINES AND MACROS
******************************************************************************/
#define DATA_SIZE 15
#define PORT 8080
#define SA struct sockaddr

/******************************************************************************
* STATIC FUNCTION PROTOTYPES
******************************************************************************/
static void FillBuffer(uint8_t *pbuff,uint8_t size_buff);
static void PrintBuff(uint8_t *pbuff,uint8_t size_buff);
static void SendData(int sockfd);

int main(void)
{
    int sockfd;
    struct sockaddr_in servaddr;

    /*socket create and verification*/
    sockfd = socket(AF_INET, SOCK_STREAM,0);
    if (sockfd == -1)
    {
        perror("socket creation failed...\n");
        exit(1);
    }
    else
        printf("Socket successfully created..\n");
    memset(&servaddr,0,sizeof(servaddr));
    /*assign IP, PORT*/
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    /*connect the client socket to server socket*/
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        perror("connection with the server failed...\n");
        exit(1);
    }
    else
        printf("connected to the server..\n");

    /*function for Send Data to server*/
    SendData(sockfd);

    /*close the socket*/
    close(sockfd);
}

/******************************************************************************
* STATIC FUNCTIONS
******************************************************************************/

/** \brief Fill 15 bytes buffer with random data
 *  \param pbuff pointer to buffer
 *  \param size_buff size of buffer
 *  \return None
 *****************************************************************************/
static void FillBuffer(uint8_t *pbuff,uint8_t size_buff)
{
    assert(pbuff !=NULL);
    assert(size_buff!=0);
    for (uint8_t i = 0; i < size_buff; i++)
    {
        pbuff[i] = ((uint8_t)(rand() % 255));
    }
}

/** \brief Print buffer data
 *  \param pbuff pointer to buffer
 *  \param size_buff size of buffer
 *  \return None
 *****************************************************************************/
static void PrintBuff(uint8_t *pbuff,uint8_t size_buff)
{
    assert(pbuff !=NULL);
    assert(size_buff!=0);
    printf("\n**** Data to be send ****");
    for (uint8_t i = 0; i < size_buff; i++)
    {
        printf(" %02X",pbuff[i]);
    }
}

/** \brief Send 15 bytes data to the server
 *  \param sockfd file descriptor for the new socket
 *  \return None
 *****************************************************************************/
static void SendData(int sockfd)
{
    uint8_t buff[DATA_SIZE];
    for (;;)
    {
        memset(buff,0, sizeof(buff));
        FillBuffer(buff,sizeof(buff));
        PrintBuff(buff,sizeof(buff));
        send(sockfd, buff, sizeof(buff), 0);
        sleep(1);
    }
}