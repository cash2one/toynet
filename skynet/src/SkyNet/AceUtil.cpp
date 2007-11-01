/**
* Copyright (c) 
*
* @author 
*/
// AceUtil.cpp
#include "stdafx.h"
#include <skynet/detail/AceUtil.h>
#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4251 4267 4312 )
#include <ace/SOCK_Stream.h>
#include <ace/Proactor.h>
#include <ace/SStringfwd.h>
#pragma warning ( pop )
#include <boost/static_assert.hpp>

#ifndef _LINUX
//BOOST_STATIC_ASSERT(sizeof(ACE_WSTRING_TYPE) == 2);
#endif

namespace skynet
{

bool setMaximumSocketBufferSize(ACE_HANDLE handle)
{
    ACE_SOCK_Stream temp_peer(handle);
    int bufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
    return (temp_peer.set_option(SOL_SOCKET, SO_SNDBUF,
            &bufsize, sizeof(bufsize)) != -1) &&
        (temp_peer.set_option(SOL_SOCKET, SO_RCVBUF,
            &bufsize, sizeof(bufsize)) != -1);
}


long setupTimer(ACE_Handler& handler, long timeout,
    const void* act)
{
    ACE_Time_Value time_out(timeout);
#ifdef _LINUX
	long timerId = ACE_Reactor::instance()->schedule_timer(handler, act, time_out);
#else
	long timerId = ACE_Proactor::instance()->schedule_timer(handler, act, time_out);
#endif

    

    WN_ASSERT(timerId != -1);
    return timerId;
}


long setupRepeatingTimer(ACE_Handler& handler, long timeout,
    const void* act)
{
    ACE_Time_Value time_out(timeout);
    long timerId = ACE_Proactor::instance()->schedule_repeating_timer(handler,
        act, time_out);
    WN_ASSERT(timerId != -1);
    return timerId;
}


void cancelTimer(long& timer_id)
{
    if (timer_id != -1) {
        ACE_Proactor::instance()->cancel_timer(timer_id);
        timer_id = -1;
    }
}

} // namespace skynet
