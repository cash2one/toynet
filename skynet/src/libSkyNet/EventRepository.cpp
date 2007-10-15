/**
* Copyright (c) 
*
* @author 
*/
// EventRepository.cpp
#include "stdafx.h"
#include <skynet/detail/EventRepository.h>
#include <skynet/Event.h>
#include <skynet/Message.h>
#include <skynet/detail/Assert.h>

namespace skynet
{

EventRepository::~EventRepository()
{
    for (Events::iterator pos = events_.begin(); pos != events_.end(); pos++) {
        delete (*pos).int_id_;
        //pos = events_.erase(pos);
    }

    events_.unbind_all();
}

void EventRepository::subscribe(Event* event)
{
    WN_ASSERT(event != 0);
    WN_ASSERT(events_.find(event->getMessageType()));
    //events_.insert(Events::value_type(event->getMessageType(), event));
    events_.bind(event->getMessageType(), event);
}


void EventRepository::unsubscribe(key_t key)
{
    //Events::iterator pos = events_.find(key);
    Event* pEvent = 0;
    events_.find(key, pEvent);
    if (pEvent != 0) {
	events_.unbind(key);
        delete pEvent;
    }
}


bool EventRepository::publish(const Message& msg, const void* arg/* = 0*/)
{
    Event* pEvent = 0;
    events_.find(msg.getMessageType(), pEvent);
    WN_ASSERT(pEvent && "이벤트 저장소에 없는 이벤트");

    if (pEvent)
    	return pEvent->call(msg, arg);
   
    return false;
}

} // namespace skynet
