/**
* Copyright (c) 
*
* @author 
*/
// WarNetService.cpp
//
#include "stdafx.h"
#include <skynet/server/SkyNetService.h>
#include "HackShield.h"
#include <skynet/server/ServerInternalMessage.h>
#include <skynet/detail/CommonMessage.h>
#include <skynet/detail/MessageBuilder.h>
#include <skynet/detail/Logging.h>
#include <skynet/detail/AceUtil.h>
#include <skynet/Message.h>

#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4251 4267 4311 4312 4702 )
#include <ace/Get_Opt.h>
#include <ace/SString.h>
#include <ace/OS.h>
#pragma warning ( pop )

namespace skynet
{

/**
 * @struct WarNetService::QueueItem
 */
struct WarNetService::QueueItem
{
    QueueItem(ClientId clientId, const Message* message) :
        clientId_(clientId),
        message_(message) {}

    ClientId clientId_;
    const Message* message_;
};

// = WarNetService

#pragma warning ( push )
#pragma warning ( disable: 4355 )

WarNetService::WarNetService() :
    clientId_(INVALID_CLIENTID),
    ioThreadCount_(0),
    useHackShield_(false),
    monitorInterval_(0)
{
}

#pragma warning ( pop )

WarNetService::~WarNetService()
{
    WN_ASSERT(thr_count() == 0);

    removeAll();

    destroyMessageQueues();
}


bool WarNetService::onInit()
{
    if (! initHackShield()) {
        WN_LOG_ERROR("HackShield initialization FAILED!!!\n");
        return false;
    }
    builderRepository_.add(new MessageBuilderT<LogoutMessage>);

    acceptor_.reset(new ClientAcceptor(*this, builderRepository_,
        isUsingCipher()));

    initService();
    initMessageBuilders();
    initEvents();
    initMessageQueues();

    const int pass_addresses = 1;
    const int validate_new_connection = 0;
    ACE_INET_Addr listen_addr(getListenPort());
    if (acceptor_->open(listen_addr, 0, pass_addresses, ACE_DEFAULT_BACKLOG,
            1, ACE_Proactor::instance(), validate_new_connection) == -1) {
        WN_LOG_ERROR2(ACE_TEXT("WarNetService::init() Failed(%d:%m)...\n"),
            ACE_OS::last_error());
        return false;
    }

    if (! proactorEventLooper_.start(ioThreadCount_))
        return false;

    if (! reactorEventLooperForLogRotation_.start(1))
        return false;

    return true;
}


void WarNetService::onFinish()
{
    if (acceptor_.get() != 0)
        acceptor_->close();

    proactorEventLooper_.stop();
    reactorEventLooperForLogRotation_.stop();
}


void WarNetService::onShutdown()
{
    proactorEventLooper_.wait();
    reactorEventLooperForLogRotation_.wait();
}


void WarNetService::onAuthenticated(ClientId clientId)
{
    ACE_GUARD(ACE_Thread_Mutex, guard, clientsLock_);
    //Clients::iterator pos = clients_.find(clientId);
    GameClientSession* pSession = 0;
    clients_.find(clientId, pSession);
    if (pSession) {
        //((*pos).second)->onAuthenticated();
        pSession->onAuthenticated();
    }
}


bool WarNetService::onParseConfigFile(ACE_Configuration_Heap& config)
{
    return parseServiceSection(config) &&
        parseSecuritySection(config);
}


int WarNetService::svc()
{
    WN_LOG_INFO3(ACE_TEXT("%s Starting(%d)...\n"),
        getServiceName().c_str(), getListenPort());

    QueueType& queue = getMessageQueue();

    while (! ACE_Proactor::instance()->proactor_event_loop_done()) {
        QueueItem item(INVALID_CLIENTID, 0);
        if (getq(queue, item)) {
            if (! handle(item.clientId_, *item.message_)) {
                WN_ASSERT(false && "뭔 경우다냐?");
            }
            delete item.message_;
        }
    }

    WN_LOG_INFO2(ACE_TEXT("%s Shutdown...\n"), getServiceName().c_str());

    return 0;
}


bool WarNetService::handle(ClientId clientId, const Message& msg)
{
    return events_.publish(msg, &clientId);
}


bool WarNetService::putq(ClientId clientId, const Message* message)
{
    if (! isValidClientId(clientId))
        return false;
    if (! message)
        return false;

    QueueType& queue = getMessageQueue(clientId);
    queue.push(QueueItem(clientId, message));
    return true;
}


bool WarNetService::getq(QueueType& queue, QueueItem& item)
{
    ACE_Time_Value timeout(ACE_OS::gettimeofday() + ACE_Time_Value(1)); // 1초

    return queue.pop(item, &timeout);
}


bool WarNetService::insert(GameClientSession* session)
{
    WN_ASSERT(session != 0);

    ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, clientsLock_, false);

    incrementClientId(clientId_);
    //if ((clients_.insert( Clients::value_type(clientId_, session))).second) {
    if ((clients_.bind( clientId_, session))) {
        session->setClientId(clientId_);

        WN_LOG_INFO3( ACE_TEXT("Client(#%d) is connected. Total %d.\n"), //clientId_, clients_.size());
            clientId_, clients_.current_size()); // total_size()?
        return true;
    }

    return false;
}


void WarNetService::remove(GameClientSession* session)
{
    WN_ASSERT(session != 0);

    bool found = false;
    {
        ACE_GUARD(ACE_Thread_Mutex, guard, clientsLock_);

        //Clients::iterator pos = clients_.find(session->getClientId());
        GameClientSession* pSession = 0;
	clients_.find(session->getClientId(), pSession);
        //if (pos != clients_.end()) {
        if (pSession  != 0) {
            //clients_.erase(pos);
            clients_.unbind(session->getClientId());
            found = true;
        }

        WN_LOG_INFO3(ACE_TEXT("Client(#%d) is disconnected. Total=%d.\n"),
            session->getClientId(), clients_.current_size());
    }

    if (found) {
        acceptor_->release(session);
    }
}


void WarNetService::removeAll()
{
    ACE_GUARD(ACE_Thread_Mutex, guard, clientsLock_);

    //clients_.clear();
    clients_.unbind_all();
}


bool WarNetService::isExist(ClientId clientId) const
{
    ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, clientsLock_, false);

    GameClientSession* pSession = 0;
    //return clients_.find(clientId) != clients_.end();
    clients_.find(clientId, pSession);
    return pSession;
}


size_t WarNetService::getClientCount() const
{
    ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, clientsLock_, 0);

    return clients_.current_size();
}


std::string WarNetService::getIpAddress(ClientId clientId) const
{
    ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, clientsLock_, false);

    //Clients::const_iterator pos = clients_.find(clientId);
    GameClientSession* pSession = 0 ;
     clients_.find(clientId, pSession);
    if (pSession) {
        return pSession->getIpAddress();
    }
    return "unknown";
}


// GameClientSessionPool을 이용하므로 임계영역 이후 session이 잘못될 일이 없다
bool WarNetService::postClientMessage(ClientId clientId,
    const Message* msg)
{
    GameClientSession* session = getGameClientSession(clientId);
    if (session != 0) {
        return session->sendMessage(msg);
    }
    return false;
}


bool WarNetService::postClientMessage(const ClientIds& clientIds,
    const Message* msg)
{
    ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, clientsLock_, false);

    for (ClientIds::const_iterator pos = clientIds.begin();
            pos != clientIds.end(); ++pos) {
        GameClientSession* session = getGameClientSession_i(*pos);
        if (session != 0)
            (void)session->sendMessage(msg);
    }
    return true;
}


bool WarNetService::postClientMessageToAll(const Message* msg)
{
    ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, clientsLock_, false);

    //for (Clients::const_iterator pos = clients_.begin(); pos != clients_.end(); ++pos) {
    for (Clients::iterator pos = clients_.begin(); pos != clients_.end(); pos++) {
        //GameClientSession* session = (*pos).second;
        GameClientSession* session = (*pos).int_id_;
        WN_ASSERT(session != 0);
        (void)session->sendMessage(msg);
    }
    return true;
}


bool WarNetService::postHangupClientMessage(ClientId clientId)
{
    return postClientMessage(clientId, 0);
}


bool WarNetService::postHangupClientMessage(const ClientIds& clientIds)
{
    return postClientMessage(clientIds, 0);
}


bool WarNetService::pendClientMessage(ClientId clientId, const Message* msg)
{
    GameClientSession* session = getGameClientSession(clientId);
    if (session != 0) {
        return session->pendMessage(msg);
    }
    return false;
}


GameClientSession* WarNetService::getGameClientSession(ClientId clientId)
{
    ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, clientsLock_, false);

    return getGameClientSession_i(clientId);
}


GameClientSession* WarNetService::getGameClientSession_i(ClientId clientId)
{
    GameClientSession* pSession = 0;
    //Clients::iterator pos = clients_.find(clientId);
    clients_.find(clientId, pSession);
    //if (pos != clients_.end()) {
    //    return (*pos).second;
    //}
    //return 0;
    return pSession;
}


void WarNetService::initMessageQueues()
{
    for (int i = getWokerThreadCount(); i > 0; --i) {
        msgQueues_.push_back(new QueueType);
    }
}


void WarNetService::destroyMessageQueues()
{
    for (Queues::const_iterator pos = msgQueues_.begin();
            pos != msgQueues_.end(); ++pos) {
        delete *pos;
    }
}


WarNetService::QueueType& WarNetService::getMessageQueue(ClientId clientId)
{
    const Queues::size_type queueIndex =
        (static_cast<size_t>(clientId) % msgQueues_.size());
    //WN_ASSERT((queueIndex >= 0) && (queueIndex < msgQueues_.size())); // queueIndex : 항상 0 
    WN_ASSERT(queueIndex < msgQueues_.size());
    return *(msgQueues_[queueIndex]);
}


WarNetService::QueueType& WarNetService::getMessageQueue()
{
    WN_ASSERT(getWokerThreadCount() == static_cast<int>(msgQueues_.size()));
    WN_ASSERT(msgQueues_.size() == thr_count());

    typedef std::vector<ACE_thread_t> ThreadTypes;
    ThreadTypes threads(msgQueues_.size(), 0);

    ACE_Thread_Manager::instance()->thread_list(this, &(threads[0]),
        threads.size());

    ThreadTypes::const_iterator pos =
        std::find(threads.begin(), threads.end(), ACE_OS::thr_self());
    WN_ASSERT(pos != threads.end());

    const Queues::size_type queueIndex = (pos - threads.begin());
    //WN_ASSERT((queueIndex >= 0) && (queueIndex < msgQueues_.size()));
    WN_ASSERT(queueIndex < msgQueues_.size());
    return *(msgQueues_[queueIndex]);
}


bool WarNetService::parseServiceSection(ACE_Configuration_Heap& config)
{
    int io_thread;
    if (! getConfigInt(io_thread, config, ACE_TEXT("Service"),
        ACE_TEXT("IoThread")))
        return false;
    ioThreadCount_ = io_thread;

    return true;
}


bool WarNetService::parseSecuritySection(ACE_Configuration_Heap& config)
{
    int useHackShield;
    if (getConfigInt(useHackShield, config, ACE_TEXT("Security"),
        ACE_TEXT("UseHackShield"), 0)) {
        useHackShield_ = (useHackShield != 0);
    }

    if (useHackShield_) {
        ACE_TString crcFilename;
        if (getConfigString(crcFilename, config, ACE_TEXT("Security"),
            ACE_TEXT("CrcFilename"))) {
            boost::filesystem::path crcPath(
                getConfigDir() / crcFilename.c_str());
            crcFilename_ = crcPath.string();
        }

        int monitorInterval;
        if (getConfigInt(monitorInterval, config, ACE_TEXT("Security"),
            ACE_TEXT("MonitorInterval"), 0)) {
            monitorInterval_ =
                static_cast<size_t>(monitorInterval );
        }
    }

    return true;
}


bool WarNetService::initHackShield()
{
    if (useHackShield_) {
        hackShield_.reset(new HackShield);
        if (! hackShield_->initialize(crcFilename_)) {
            return false;
        }

        builderRepository_.add(new MessageBuilderT<HackShieldGuidAckMessage>);
        builderRepository_.add(new MessageBuilderT<HackShieldAckMessage>);
    }

    return true;
}

} // namespace skynet
