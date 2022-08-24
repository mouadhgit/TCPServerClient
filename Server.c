/**
 *  Copyright (C) 2022 Focus Corporation 
 *
 *  \file     Server.c
 *  \brief    TCP server listens to address 127.0.0.1 on port 8080
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

/******************************************************************************
* DEFINES AND MACROS
******************************************************************************/
#define DATA_SIZE 15
#define PORT 8080
#define SA struct sockaddr

/******************************************************************************
* STATIC FUNCTION PROTOTYPES
******************************************************************************/
static void PrintBuff(uint8_t *pbuff, uint8_t size_buff);
static void RecieveData(int connfd);

int main(void)
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    /*socket create and verification*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket creation failed...\n");
        exit(1);
    }
    else
        printf("Socket successfully created..\n");
    memset(&servaddr, 0, sizeof(servaddr));
    /*assign IP, PORT*/
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    /*Binding newly created socket to given IP and verification*/
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        perror("socket bind failed...\n");
        exit(1);
    }
    else
        printf("Socket successfully binded..\n");

    /*Now server is ready to listen and verification*/
    if ((listen(sockfd, 5)) != 0)
    {
        perror("Listen failed...\n");
        exit(1);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    /*Accept the data packet from client and verification*/
    connfd = accept(sockfd, (SA *)&cli, (socklen_t *)&len);
    if (connfd < 0)
    {
        perror("server accept failed...\n");
        exit(1);
    }
    else
        printf("server accept the client...\n");

    /*function for Recieve Data from the client*/
    RecieveData(connfd);

    close(sockfd);
}

/******************************************************************************
* STATIC FUNCTIONS
******************************************************************************/

/** \brief Print buffer data
 *  \param pbuff pointer to buffer
 *  \param size_buff size of buffer
 *  \return None
 *****************************************************************************/
static void PrintBuff(uint8_t *pbuff, uint8_t size_buff)
{
    assert(pbuff != NULL);
    assert(size_buff != 0);
    printf("\n**** Data Recieved ****");
    for (uint8_t i = 0; i < size_buff; i++)
    {
        printf(" %02X", (uint8_t)pbuff[i]);
    }
}

/** \brief Recieve & display client data
 *  \param connfd file descriptor for the accepted socket
 *  \return None
 *****************************************************************************/
static void RecieveData(int connfd)
{
    uint8_t buff[DATA_SIZE];
    for (;;)
    {
        memset(buff, 0, sizeof(buff));
        read(connfd, buff, sizeof(buff));
        PrintBuff(buff, sizeof(buff));
    }
}