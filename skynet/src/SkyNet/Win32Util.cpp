/**
* Copyright (c) 
*
* @author 
*/
#include "stdafx.h"
#ifdef _WINDOWS

#include <skynet/detail/Win32Util.h>

// You must include the #define _WIN32_DCOM preprocessor directive at the
// beginning of your code to be able to use CoInitializeEx.
#define _WIN32_DCOM
#include <objbase.h>

namespace skynet
{

InitOle::InitOle()
{
    ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
}


InitOle::~InitOle()
{
    ::CoUninitialize();
}


time_t variantTimeToUnixTime(const tagVARIANT& vt)
{
    WN_ASSERT(vt.vt == VT_DATE);
    if (vt.vt != VT_DATE) {
        return 0;
    }

    SYSTEMTIME sysTime;
    int status = ::VariantTimeToSystemTime(vt.date, &sysTime);
    WN_ASSERT(status != 0); status;

    struct tm atm;
    atm.tm_sec = sysTime.wSecond;
    atm.tm_min = sysTime.wMinute;
    atm.tm_hour = sysTime.wHour;
    atm.tm_mday = sysTime.wDay;
    atm.tm_mon = sysTime.wMonth - 1;        // tm_mon is 0 based
    atm.tm_year = sysTime.wYear - 1900;     // tm_year is 1900 based
    atm.tm_isdst = -1;
    return mktime(&atm);
}


tagVARIANT unixTimeToVariantTime(time_t ut)
{
    tm* localtm = localtime(&ut);
    WN_ASSERT(localtm != 0);
    if (! localtm) {
        const time_t curTime = time(0);
        localtm = localtime(&curTime);
        WN_ASSERT(localtm != 0);
    }

    SYSTEMTIME sysTime;
    sysTime.wYear = static_cast<WORD>(1900 + localtm->tm_year);
    sysTime.wMonth = static_cast<WORD>(localtm->tm_mon + 1);
    sysTime.wDayOfWeek = 0;
    sysTime.wDay = static_cast<WORD>(localtm->tm_mday);
    sysTime.wHour = static_cast<WORD>(localtm->tm_hour);
    sysTime.wMinute = static_cast<WORD>(localtm->tm_min);
    sysTime.wSecond = static_cast<WORD>(localtm->tm_sec);
    sysTime.wMilliseconds = 0;

    tagVARIANT vt;
    vt.vt = VT_DATE;
    int status = ::SystemTimeToVariantTime(&sysTime, &vt.date);
    WN_ASSERT(status != 0); status;
    return vt;
}

} // namespace skynet

#endif
