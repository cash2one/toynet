/**
* Copyright (c) 
*
* @author 
*/
// GameClientSession.h
#if !defined(INCLUDED_GAMECLIENTSESSION)
#define INCLUDED_GAMECLIENTSESSION

#include "ClientSession.h"
#include <skynet/server/ServerInternalMessage.h>
#include <skynet/detail/Assert.h>

namespace skynet
{

class WarNetService;
class ServerMessageAdaptor;
//class HackingMonitor;

/**
 * @class GameClientSession
 *
 * Game Client I/O Handler
 */
class WARNET_API GameClientSession : public ClientSession
{
public:
    GameClientSession(WarNetService* service = 0,
        MessageBuilderRepository* builderRepository = 0,
        bool useCipher = false);
    virtual ~GameClientSession();

    virtual void open(ACE_HANDLE newHandle, ACE_Message_Block& messageBlock);

    void onAuthenticated();
private:
    virtual void handle_time_out(const ACE_Time_Value& tv, const void* act);

    virtual bool onInputQueue(const Message* message);
    virtual size_t getMaxPacketBodySize() const;

    virtual void onDisconnected();
    virtual MessageAdaptor& getMessageAdaptor() const;
private:
    void setupAuthenticateVerifyTimer();
    void cancelAuthenticateVerifyTimer();

    void setupHeartbeatTimer();
    void cancelHeartbeatTimer();
    void onHeartbeat();
    bool isZombie(size_t timeout) const;

    void setupHackingMonitorTimer();
    void cancelHackingMonitorTimer();

    void initHackingMonitor();
    void sendFirstHackingMonitorMessage();
    void sendHackingMonitorMessage();
    bool monitorHacking(const Message* message);
    bool analyzeGuidAckMsg(const Message* message);
    bool analyzeAckMsg(const Message* message);
private:
    std::auto_ptr<ServerMessageAdaptor> messageAdaptor_;
    WarNetService* service_;

    long authenticateVerifyTimer_;
    long heartbeatTimer_;
    long hackingMonitorTimer_;

    ACE_Time_Value lastHeartbeatTime_;

    //std::auto_ptr<HackingMonitor> hackingMonitor_;
};

} // namespace skynet

#endif // !defined(INCLUDED_GAMECLIENTSESSION)
