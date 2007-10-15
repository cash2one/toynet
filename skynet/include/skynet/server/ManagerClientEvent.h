#if !defined(INCLUDED_MANAGERCLIENTEVENT)
#define INCLUDED_MANAGERCLIENTEVENT

#include <warnet/warnet_export.h>
#include <warnet/Event.h>
#include <warnet/poporu/ManagerServerMessage.h>

namespace warnet{

class ManagerClientServiceHandler;


class WARNET_API ManagerClientEvent :public Event
{
public:
    ManagerClientEvent(MessageType mt, ManagerClientServiceHandler& service) : 
      Event(mt),
          service_(service) {}
protected:
    ManagerClientServiceHandler& service_;
};


class WARNET_API MLoginResultMessageEvent :
    public ManagerClientEvent
{
public:
    MLoginResultMessageEvent(ManagerClientServiceHandler& service) :
      ManagerClientEvent(MT_MS_S2C_LOGIN_RESULT, service) {}
private:
    virtual bool call(const Message& msg, const void* arg);
};


}
#endif