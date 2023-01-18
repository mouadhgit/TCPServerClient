/**
 *  Copyright (C) 2022 
 *
 *  \file     check.c
 *  \brief    check socket api's & malloc function call succeeded or not
 *  \author   Mouadh Dahech
 *  \platform x86 linux machine
 *
 *  \addtogroup 
 *  \{
 */

#include "check.h"
#include "stdio.h"
#include "stdlib.h"

/** \brief check socket api's call succeeded or not
 *  \param exp function
 *  \param errormsg error message to be returned if failed
 *  \param succmsg message to be returned if not failed
 *  \return exp function output
 *****************************************************************************/
int check(int exp, const char *errormsg, const char *succmsg)
{
    if (exp != SOCKETERROR)
    {
        printf("%s", succmsg);
    }
    else
    {
        perror(errormsg);
        exit(1);
    }
    return exp;
}

/** \brief check malloc function
 *  \param MalloCheckRet_t SUCCESS if ok, MallocFailed if not  
 *  \return none
 *****************************************************************************/
void MallocLogMsg(MalloCheckRet_t MallocReturn)
{
    if (MallocReturn == MallocFailed)
    {
        perror("Malloc failed..\n");
        exit(1);
    }
    else
    {
    }
}
