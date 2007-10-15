/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// SkyNetClient.h
//
#if !defined(INCLUDED_WARNETCLIENT)
#define INCLUDED_WARNETCLIENT

#include "EventRepository.h"
#include "MessageBuilder.h"

#pragma warning( push)
#pragma warning( disable : 4244 4251 4267 4311 4312 4355)
#include <ace/SVC_Handler.h>
#include <ace/Reactor_Notification_Strategy.h>
#include <ace/SOCK_Stream.h>
#pragma warning( pop)

#include <boost/utility.hpp>
#include <string>

class ACE_Reactor;

namespace skynet
{

class Message;
class ClientMessageAdaptor;
class WarNetSessionEventHandler;
class HackingSensor;

/**
 * @class SkyNetClient
 *  - 실질적인 Reactor Service Handler이다
 */
class WARNET_API SkyNetClient :
    public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>,
    public boost::noncopyable
{
    typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> PARENT;
public:
    SkyNetClient(long heartbeatInterval = 0,
        const Message* heartbeatMessage = 0, bool useCipher = false,
        bool useHackShield = false, ACE_Reactor* reactor = 0);
    virtual ~SkyNetClient();

    bool connect(const std::string& ip, u_short port, int timeout);

    void disconnect(bool fireOnDisconnect);
    bool send(const Message& msg);

    void registerEventHandler(WarNetSessionEventHandler* eventHandler) {
        //WN_ASSERT(eventHandler != 0);
        eventHandler_ = eventHandler;
    }  

    void registerHackingSensor(HackingSensor* sensor) {
        //WN_ASSERT(sensor != 0);
        hackingSensor_ = sensor;
    }

    void registerMessageHandler(Event* messageHandler) {
        WN_ASSERT(messageHandler != 0);
        events_.subscribe(messageHandler);
    }

    void registerMessageBuilder(MessageBuilder* builder) {
        WN_ASSERT(builder != 0);
        builderRepository_.add(builder);
    }

    /// 내부적으로 사용함. 절대 호출해선 안됨!!
    void responseHackingShieldGuidReqMessage(const Message& msg);
    void responseHackingShieldReqMessage(const Message& msg);
public:
    virtual int open(void* arg = 0);
    virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
    virtual int handle_output(ACE_HANDLE fd = ACE_INVALID_HANDLE);
    virtual int handle_timeout(const ACE_Time_Value& time, const void *);
    virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask);
private:    
    void handleMessage();
    void closeSocket();

    bool waitForHackingMonitorAck();
private:
    long heartbeatInterval_;
    const Message* heartbeatMessage_;
    bool useHackShield_;
    std::auto_ptr<ClientMessageAdaptor> messageAdaptor_;
    WarNetSessionEventHandler* eventHandler_;
    HackingSensor* hackingSensor_;

    MessageBuilderRepository builderRepository_;
    EventRepository events_;

    ACE_Message_Block mblock_;
    ACE_Reactor_Notification_Strategy notifier_;

    bool isConnecting_;
    size_t neededSize_;

    ACE_Time_Value lastSentTime_;
    long heartbeatTimerId_;

    volatile bool firstHackingMonitorAcked_;
};

} // namespace skynet

#endif // !defined(INCLUDED_WARNETCLIENT)
