/**
* Copyright (c) 
*
* @author 
*/
// GameClientSession.cpp
//
#include "stdafx.h"
#include <skynet/server/GameClientSession.h>
#include <skynet/detail/ServerMessageAdaptorFactory.h>
#include <skynet/detail/ServerMessageAdaptor.h>
#include <skynet/server/SkyNetService.h>
//#include <skynet/server/HackingMonitor.h>
#include <skynet/detail/CommonMessage.h>
#include <skynet/detail/MessageAdaptor.h>
#include <skynet/detail/AceUtil.h>
#include <skynet/detail/Logging.h>
#include <vector>
#include <skynet/detail/Assert.h>

namespace skynet
{

GameClientSession::GameClientSession(WarNetService* service,
        MessageBuilderRepository* builderRepository, bool useCipher) :
    ClientSession(builderRepository),
    service_(service),
    messageAdaptor_(ServerMessageAdaptorFactory::create(useCipher)),
    authenticateVerifyTimer_(-1),
    heartbeatTimer_(-1),
    hackingMonitorTimer_(-1)
{
    // GameClientSessionPoolTest 때문에 WN_ASSERT하지 않음
    //WN_ASSERT(service_ != 0);
}


GameClientSession::~GameClientSession()
{
    WN_ASSERT(authenticateVerifyTimer_ == -1);
}


void GameClientSession::open(ACE_HANDLE newHandle,
    ACE_Message_Block& messageBlock)
{
    WN_ASSERT(service_ != 0);

    if (! messageAdaptor_->setFirstSecureSeed(newHandle)) {
        WN_LOG_ERROR(ACE_TEXT("Can't set Secure Seed.\n"));
        disconnect();
        return;
    }

    initHackingMonitor();

    setupAuthenticateVerifyTimer();
    ClientSession::open(newHandle, messageBlock);
    (void)service_->insert(this);

    sendFirstHackingMonitorMessage();
}


void GameClientSession::handle_time_out(const ACE_Time_Value& /*tv*/,
    const void* act)
{
    if (! isValidClientId(getClientId()))
        return;

    const long timer = *static_cast<const long*>(act);
    if (timer == authenticateVerifyTimer_) {
        WN_LOG_INFO3(ACE_TEXT("if(timer(%d) == authenticateVerifyTimer_(%d)******)\n"),
         timer, authenticateVerifyTimer_);
        authenticateVerifyTimer_ = -1;
        WN_LOG_INFO2(ACE_TEXT("Client(#%d) is forcingly disconnected,")
            ACE_TEXT("because authentication timeout.\n"), getClientId());
        disconnect();
    }
    else if (timer == heartbeatTimer_) {
        if (isZombie(service_->getHeartbeatTimeout())) {
            WN_LOG_INFO2(ACE_TEXT("Client(#%d) is forcingly disconnected,")
                ACE_TEXT("because zombie.\n"), getClientId());
            disconnect();
        }
    }
    else if (timer == hackingMonitorTimer_) {
        WN_ASSERT(service_->useHackShield());
        sendHackingMonitorMessage();
    }
    else {
        WN_ASSERT(false && "왠 놈이냐?");
    }
}


size_t GameClientSession::getMaxPacketBodySize() const
{
    return service_->getMaxPacketBodySize();
}


void GameClientSession::setupAuthenticateVerifyTimer()
{
    if (service_->getAuthenticateTimeout() == 0) {
         WN_LOG_INFO2(ACE_TEXT("service_->getAuthenticateTimeout() == 0"),
            service_->getAuthenticateTimeout());
        return;
    }
    authenticateVerifyTimer_ = setupTimer(*this,
        static_cast<long>(service_->getAuthenticateTimeout()),
        &authenticateVerifyTimer_);
}


void GameClientSession::setupHeartbeatTimer()
{
    if (service_->getHeartbeatTimeout() == 0)
        return;

    lastHeartbeatTime_ = ACE_OS::gettimeofday();
    heartbeatTimer_ = setupRepeatingTimer(*this,
        static_cast<long>(service_->getHeartbeatTimeout()),
        &heartbeatTimer_);
}


void GameClientSession::setupHackingMonitorTimer()
{
    if (! service_->useHackShield()) {
        return;
    }

    if (service_->getHackingMonitorInterval() == 0) {
        return;
    }

    hackingMonitorTimer_ = setupRepeatingTimer(*this,
        static_cast<long>(service_->getHackingMonitorInterval()),
        &hackingMonitorTimer_);
}


void GameClientSession::cancelAuthenticateVerifyTimer()
{
    cancelTimer(authenticateVerifyTimer_);
}


void GameClientSession::cancelHeartbeatTimer()
{
    cancelTimer(heartbeatTimer_);
}


void GameClientSession::cancelHackingMonitorTimer()
{
    cancelTimer(hackingMonitorTimer_);
}


bool GameClientSession::onInputQueue(const Message* message)
{
    WN_ASSERT(message != 0);

    onHeartbeat();

    if (! monitorHacking(message)) {
        return false;
    }

    if (! service_->putq(getClientId(), message)) {
        return false;
    }
    return true;
}


void GameClientSession::onDisconnected()
{
    WN_ASSERT(! isConnected());

    cancelAuthenticateVerifyTimer();
    cancelHeartbeatTimer();
    cancelHackingMonitorTimer();

    std::auto_ptr<LogoutMessage> logoutMsg(new LogoutMessage);
    if (onInputQueue(logoutMsg.get()))
        logoutMsg.release();

    service_->remove(this);
}


void GameClientSession::onAuthenticated()
{
    cancelAuthenticateVerifyTimer();
    
    if (service_->useHackShield()) {
		/*
        if (! hackingMonitor_->guidAcked()) {
            WN_LOG_ERROR("First HackShield GuidAckMsg didn't arrived!!!\n");
            disconnect();
            return;
        }
		*/
    }
    
    setupHeartbeatTimer();
    setupHackingMonitorTimer();
}


void GameClientSession::onHeartbeat()
{
    lastHeartbeatTime_ = ACE_OS::gettimeofday();
}


MessageAdaptor& GameClientSession::getMessageAdaptor() const
{
    WN_ASSERT(messageAdaptor_.get() != 0);
    return messageAdaptor_->getMessageAdaptor();
}


bool GameClientSession::isZombie(size_t timeout) const
{
    return (ACE_OS::gettimeofday() - lastHeartbeatTime_).sec() >=
        static_cast<long>(timeout);
}


void GameClientSession::initHackingMonitor()
{
	/*
    if (! service_->useHackShield()) {
        return;
    }

    hackingMonitor_.reset(new HackingMonitor(service_->getHackShield()));
	*/
}


void GameClientSession::sendFirstHackingMonitorMessage()
{
	/*
    if (! service_->useHackShield()) {
        return;
    }
    WN_ASSERT(hackingMonitor_.get() != 0);

    HackingMonitor::Buffer guidRequest;
    if (hackingMonitor_->makeGuidRequest(guidRequest)) {
        const HackShieldGuidReqMessage msg(&(guidRequest[0]),
            guidRequest.size());
        if (! sendMessage(&msg)) {
            disconnect();
        }
    }
    else {
        disconnect();
        WN_ASSERT(false && "우째 이런 일이!!!");
    }
	*/
}


void GameClientSession::sendHackingMonitorMessage()
{
	/*
    WN_ASSERT(hackingMonitor_.get() != 0);
    WN_ASSERT(hackingMonitor_->guidAcked());

    HackingMonitor::Buffer request;
    if (! hackingMonitor_->makeRequest(request)) {
        disconnect();
        WN_ASSERT(false && "우째 이런 일이!!!");
    }
    else {
        const HackShieldGuidReqMessage msg(&(request[0]), sizeof(request));
        (void)sendMessage(&msg);
    }
	*/
}


bool GameClientSession::monitorHacking(const Message* message)
{
	/*
    WN_ASSERT(message != 0);
    if (! hackingMonitor_.get()) {
        return true;
    }

    if (HackShieldGuidAckMessage::thisMessageType
        == message->getMessageType()) {
        (void)analyzeGuidAckMsg(message);
        return false;
    }
    if (HackShieldAckMessage::thisMessageType
        == message->getMessageType()) {
        (void)analyzeAckMsg(message);
        return false;
    }
	*/

    return true;
}


bool GameClientSession::analyzeGuidAckMsg(const Message* message)
{
    const HackShieldGuidAckMessage& realMsg =
        dynamic_cast<const HackShieldGuidAckMessage&>(*message);

	/*
    if (hackingMonitor_->analyzeGuidResponse(realMsg.getAckMsg())) {
        return true;
    }*/

    WN_LOG_ERROR2(ACE_TEXT("Client(#%d) is HACKED!!!(Guid CRC mismatch)\n"),
        getClientId());
    disconnect();
    return false;
}


bool GameClientSession::analyzeAckMsg(const Message* message)
{
    const HackShieldAckMessage& realMsg =
        dynamic_cast<const HackShieldAckMessage&>(*message);

	/*
    if (hackingMonitor_->analyzeResponse(realMsg.getAckMsg())) {
        return true;
    }*/

    WN_LOG_ERROR2(ACE_TEXT("Client(#%d) is HACKED!!!(CRC mismatch)\n"),
        getClientId());
    disconnect();
    return false;
}

} // namespace skynet
