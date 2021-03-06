/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// CrashHandler.cpp
//
#include "stdafx.h"
#include <skynet/detail/Logging.h>
#include <BugSlayer/BugslayerUtil.h>
#include <skynet/detail/Assert.h>

namespace skynet
{

TCHAR dumpDir[MAXPATHLEN];
TCHAR dumpFilenamePrefix[MAXPATHLEN];

LONG __stdcall theCrashHandler(EXCEPTION_POINTERS* pExPtrs)
{
    WN_LOG_ERROR2(ACE_TEXT("Crash(Reason:%W)\n"),
        GetFaultReason(pExPtrs));
    WN_LOG_ERROR2(ACE_TEXT("Crash(Register:%W)\n"),
        GetRegisterString(pExPtrs));

    LPCTSTR trace = GetFirstStackTraceString(
        GSTSO_MODULE | GSTSO_SYMBOL | GSTSO_SRCLINE, pExPtrs);
    while (trace != 0) {
        WN_LOG_ERROR2(ACE_TEXT("StackTrace:%W\n"), trace);
        trace = GetNextStackTraceString(
            GSTSO_MODULE | GSTSO_SYMBOL | GSTSO_SRCLINE, pExPtrs);
    }

    WN_ASSERT(IsMiniDumpFunctionAvailable() && "�̴ϴ����� �� �� ���� �÷���");
    if (IsMiniDumpFunctionAvailable()) {
        TCHAR dumpPath[MAXPATHLEN];
        ACE_OS::sprintf(dumpPath, "%s\\%s_%d_%d_%d.DMP", dumpDir,
            dumpFilenamePrefix, ACE_OS::getpid(), ::GetCurrentThreadId(),
            ::GetTickCount());

        BSUMDRET eRet = CreateCurrentProcessCrashDumpA(MiniDumpWithFullMemory,
            dumpPath, ::GetCurrentThreadId(), pExPtrs);

        if (eDUMP_SUCCEEDED == eRet)
            WN_LOG_ERROR2(ACE_TEXT("Core Dumped(%s)!!!\n"), dumpPath);
        else
            WN_LOG_ERROR2(ACE_TEXT("Core Dump Failed(%s)!!!\n"), dumpPath);
    }
    return EXCEPTION_EXECUTE_HANDLER;
}


void initCrashHandler(const TCHAR* dir, const TCHAR* filenamePrefix)
{
    WN_ASSERT(dir != 0);
    WN_ASSERT(filenamePrefix != 0);

    ACE_OS::strcpy(dumpDir, dir);
    ACE_OS::strcpy(dumpFilenamePrefix, filenamePrefix);
    SetCrashHandlerFilter(&theCrashHandler);
}

} // namespace skynet
