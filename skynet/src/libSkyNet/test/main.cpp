/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author ±èº´¼ö <kcando@megaking.co.kr>
*/
// main.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <warnet/test/TestUtil.h>
#include <ace/Log_Msg.h>

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
    ACE_LOG_MSG->priority_mask(0, ACE_Log_Msg::PROCESS);

    return warnet::testMain(argc, argv);
} 
