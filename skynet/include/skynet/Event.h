/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// Event.h
#if !defined(INCLUDED_EVENT)
#define INCLUDED_EVENT

#include "Protocol.h"
#include <boost/utility.hpp>

namespace skynet
{

class Message;

/**
* @class Event
*
* 메세지 기반의 이벤트.
* - Subscribe/Unsubsribe Pattern
*/
class Event : public boost::noncopyable
{
public:
    virtual ~Event() {}
protected:
    Event(MessageType mt = MT_UNKNOWN) : mt_(mt) {}
private:
    friend class EventRepository;

    /// msg의 메모리 해제가 필요할 경우 false 리턴
    virtual bool call(const Message& msg, const void* arg = 0) = 0;

    MessageType getMessageType() const {
        return mt_;
    }
private:
    MessageType mt_;
};

} // namespace skynet

#endif // !defined(INCLUDED_EVENT)
