/**
* Copyright (c) 
*
* @author 
*/
// SkyNetService.h
#if !defined(INCLUDED_WARNETSERVICE)
#define INCLUDED_WARNETSERVICE

#include <skynet/server/SkyNetServiceBase.h>
#include <skynet/server/ClientAcceptor.h>
#include <skynet/server/ClientId.h>
#include <skynet/server/ProactorEventLooper.h>
#include <skynet/server/ReactorEventLooper.h>
#include <skynet/detail/MessageBuilder.h>
#include <skynet/detail/EventRepository.h>
#include <skynet/detail/Queue.h>

#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4251 4267 4311 4312 4702 )
#include <ace/Thread_Mutex.h>
#pragma  warning ( pop )

#pragma warning ( push )
#pragma warning ( disable: 4702 )
//#include <hash_map>
#pragma warning ( pop )
#include <vector>
#include <memory>

class ACE_Message_Block;
class ACE_Configuration_Heap;

class HackShield
{
public:
  HackShield(){}
  ~HackShield(){}
private:

};

namespace skynet
{

class Message;
class HackShield;

/**
 * @class WarNetService
 *
 * ������ ������ ���� ����.
 */
class WARNET_API WarNetService : public WarNetServiceBase
{
private:
    //typedef stdext::hash_map<ClientId, GameClientSession*> Clients;
    typedef Hash_Map<ClientId, GameClientSession*>  Clients;
    struct QueueItem;
    // ACE_Task<>::putq()���� �� �� ���� ������� �߻��Ͽ� light queue�� ��ü
    typedef Queue<QueueItem> QueueType;
    typedef std::vector<QueueType*> Queues;
public:
    WarNetService();
    virtual ~WarNetService();

    // Service Configurator hook methods.
    virtual int svc();

    /// ����� �����ϱ������ �ð� ���� (DoS ���� ����)
    virtual size_t getAuthenticateTimeout() const = 0;

    /// �ֱ������� Ŭ���̾�Ʈ�� ���� ���θ� �˻��ϱ� ���� Ÿ�̸��� �ð�(��)
    virtual size_t getHeartbeatTimeout() const = 0;

    /// ��Ŷ body �ִ� ����Ʈ ��
    virtual size_t getMaxPacketBodySize() const {
        return 4096; // TODO: config.���� �ִ� ũ�⸦ ������ ��
    }

    bool putq(ClientId clientId, const Message* message);

    /// GameClientSession�� ���ť�� �޼����� �ִ´�.
    bool postClientMessage(ClientId clientId, const Message* msg);
    bool postClientMessage(const ClientIds& clientIds, const Message* msg);
    bool postClientMessageToAll(const Message* msg);

    /// GameClientSession�� ������ �����Ѵ�.
    bool postHangupClientMessage(ClientId clientId);
    bool postHangupClientMessage(const ClientIds& clientIds);

    /// GameClientSession�� �Է�ť�� �޼����� �ִ´�.
    bool pendClientMessage(ClientId clientId, const Message* msg);

    /// clientId�� ������ �����Ͽ���
    void onAuthenticated(ClientId clientId);

    /// ����� Ŭ���̾�Ʈ�� ���� ��´�.
    size_t getClientCount() const;

    /// Ŭ���̾�Ʈ�� �����ϴ°�?
    bool isExist(ClientId clientId) const;

    /// Ŭ���̾�Ʈ�� ���� �ּҸ� ��´�
    std::string getIpAddress(ClientId clientId) const;

    ClientId getClientId() const {
        return clientId_;
    }

    virtual bool isUsingCipher() const {
        return true; // TODO: config.���� �ű� ��
    }

    bool useHackShield() const {
        return useHackShield_;
    }
    const std::string& getCrcFilename() const {
        return crcFilename_;
    }
    size_t getHackingMonitorInterval() const {
        return monitorInterval_;
    }

    HackShield& getHackShield() const {
        WN_ASSERT(useHackShield_);
        return *hackShield_;
    }
protected:
    virtual bool onInit();
    virtual void onFinish();
    virtual void onShutdown();
    virtual bool onParseConfigFile(ACE_Configuration_Heap& config);
private:
    /// �޼����� ó���Ѵ�.
    virtual bool handle(ClientId clientId, const Message& msg);

    virtual void initService() = 0;
    virtual void initMessageBuilders() = 0;
    virtual void initEvents() = 0;
protected:
    MessageBuilderRepository& getBuilderRepository() {
        return builderRepository_;
    }
    EventRepository& getEventRepository() {
        return events_;
    }
private:
    GameClientSession* getGameClientSession(ClientId clientId);
    GameClientSession* getGameClientSession_i(ClientId clientId);
    void initHeartbeatTimer();
    void initMessageQueues();
    void destroyMessageQueues();
    QueueType& getMessageQueue(ClientId clientId);
    QueueType& getMessageQueue();

    bool getq(QueueType& queue, QueueItem& item);

    bool parseServiceSection(ACE_Configuration_Heap& config);
    bool parseSecuritySection(ACE_Configuration_Heap& config);
private:
    friend class GameClientSession;
    bool insert(GameClientSession* session);
    void remove(GameClientSession* session);
    void removeAll();

    bool initHackShield();
private:
    ClientId clientId_;
    std::auto_ptr<ClientAcceptor> acceptor_;

    int ioThreadCount_;

    MessageBuilderRepository builderRepository_;
    EventRepository events_;

    Clients clients_;
    mutable ACE_Thread_Mutex clientsLock_;

    Queues msgQueues_; // locking �ʿ� ����

    ProactorEventLooper proactorEventLooper_;
    ReactorEventLooper reactorEventLooperForLogRotation_;

    bool useHackShield_; ///< �ٽ��� ��� ����
    std::string crcFilename_; ///< �ٽ��� CRC ���ϸ�
    size_t monitorInterval_;

    std::auto_ptr<HackShield> hackShield_;
};

} // namespace skynet

#endif // !defined(INCLUDED_WARNETSERVICE)
