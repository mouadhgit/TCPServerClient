
/**
 *  Copyright (C) 2022 
 *
 *  \file     check.h
 *  \brief    check socket api's & malloc function call succeeded or not
 *  \author   Mouadh Dahech
 *  \platform x86 linux machine
 *
 *  \addtogroup 
 *  \{
 */

#ifndef __CHEK_H__
#define __CHEK_H__

#define SOCKETERROR -1
#define SERVER_BACKLOG 1
#define MalloCheck(p) (p == NULL) ? MallocFailed : SUCCESS

enum MalloCheckRet
{
    SUCCESS = 1,
    MallocFailed = 0,
};

typedef enum MalloCheckRet MalloCheckRet_t;

int check(int exp, const char *errormsg, const char *succmsg);
void MallocLogMsg(MalloCheckRet_t MallocReturn);

#endif
