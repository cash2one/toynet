/**
* Copyright (c) 2006 Mega Enterprise Co., Ltd.
*
* @author 김영중
*/
// ManaServerProxy.h
#if !defined(INCLUDED_SERVERPROXY)
#define INCLUDED_SERVERPROXY

#include <warnet/detail/MessageBuilder.h>
#include <warnet/detail/EventRepository.h>
#include <warnet/detail/Queue.h>
#include <warnet/server/ServerConnector.h>
#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4251 4267 4311 4312 4702 )
#include <ace/Task.h>
#include <ace/Thread_Mutex.h>
#pragma  warning ( pop )
#include <string>

namespace warnet
{

class ServerClientSession;

/**
* @class ManagerServerProxyImpl
*
* ManagerServer와의 연결을 유지한다
*/
class WARNET_API ServerProxy :public ACE_Task_Base
{
public:
    ServerProxy(bool useCipher);

    void putq(const Message* msg);
    bool getq(Message*& message);
private:
    friend class ServerClientSession;
    void onConnected(ServerClientSession* session);
    void onDisconnected(ServerClientSession* session);
protected:
    /// ManagerServer에 연결한다
    virtual void connect() = 0;
    virtual bool login() = 0;
    virtual void initMessageBuilders() =0;
    virtual void initEvents() =0;

    ServerConnector &getConnector();
private:
    virtual int svc();
    void handle(const Message& msg);
protected:
    bool send(const Message& msg);

    ServerConnector connector_;

    MessageBuilderRepository builderRepository_;
    EventRepository events_;

    ServerClientSession* serverSession_;
    ACE_Thread_Mutex sessionLock_;

    PtrQueue<Message> messages_;
};

} // namespace warnet

#endif // !defined(INCLUDED_SERVERPROXY)
