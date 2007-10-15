/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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

    /// �̺�Ʈ�� ����Ѵ�.
    void subscribe(Event* event);

    /// �̺�Ʈ ����� �����Ѵ�.
    void unsubscribe(key_t key);

    /// �̺�Ʈ�� �����Ѵ�.
    /// @return msg�� �޸� ������ �ʿ�� �� ��� false ����
    bool publish(const Message& msg, const void* arg = 0);
protected: // �׽�Ʈ�� ���ؼ� protected�� ����
    //typedef stdext::hash_map<key_t, Event*> Events;
    typedef Hash_Map<key_t, Event*>  Events;
    Events events_;
};

} // namespace skynet

#endif // !defined(INCLUDED_EVENTREPOSITORY)
