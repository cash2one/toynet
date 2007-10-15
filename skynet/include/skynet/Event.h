/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
* �޼��� ����� �̺�Ʈ.
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

    /// msg�� �޸� ������ �ʿ��� ��� false ����
    virtual bool call(const Message& msg, const void* arg = 0) = 0;

    MessageType getMessageType() const {
        return mt_;
    }
private:
    MessageType mt_;
};

} // namespace skynet

#endif // !defined(INCLUDED_EVENT)
