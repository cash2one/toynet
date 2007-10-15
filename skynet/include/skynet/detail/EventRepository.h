/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// EventRepository.h
#if !defined(INCLUDED_EVENTREPOSITORY)
#define INCLUDED_EVENTREPOSITORY

#include <skynet/Protocol.h>
#include <boost/utility.hpp>
#pragma warning ( push)
#pragma warning ( disable: 4702)
//#include <hash_map>
#include "HashMap.h"
#pragma warning ( pop)
#include <skynet/detail/Assert.h>

namespace skynet
{

class Event;
class Message;

/**
* @class EventRepository
*
* - Publish/Subscribe Pattern
*/
class WARNET_API EventRepository : public boost::noncopyable
{
public:
    typedef skynet::MessageType key_t;
public:
    ~EventRepository();

    /// 이벤트를 등록한다.
    void subscribe(Event* event);

    /// 이벤트 등록을 해제한다.
    void unsubscribe(key_t key);

    /// 이벤트를 통지한다.
    /// @return msg의 메모리 해제를 필요로 할 경우 false 리턴
    bool publish(const Message& msg, const void* arg = 0);
protected: // 테스트를 위해서 protected로 선언
    //typedef stdext::hash_map<key_t, Event*> Events;
    typedef Hash_Map<key_t, Event*>  Events;
    Events events_;
};

} // namespace skynet

#endif // !defined(INCLUDED_EVENTREPOSITORY)
