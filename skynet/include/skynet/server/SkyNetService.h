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
 * 포포루 서버의 메인 서비스.
 */
class WARNET_API WarNetService : public WarNetServiceBase
{
private:
    //typedef stdext::hash_map<ClientId, GameClientSession*> Clients;
    typedef Hash_Map<ClientId, GameClientSession*>  Clients;
    struct QueueItem;
    // ACE_Task<>::putq()에서 알 수 없는 데드락이 발생하여 light queue로 대체
    typedef Queue<QueueItem> QueueType;
    typedef std::vector<QueueType*> Queues;
public:
    WarNetService();
    virtual ~WarNetService();

    // Service Configurator hook methods.
    virtual int svc();

    /// 사용자 인증하기까지의 시간 제한 (DoS 공격 방어용)
    virtual size_t getAuthenticateTimeout() const = 0;

    /// 주기적으로 클라이언트의 접속 여부를 검사하기 위한 타이머의 시간(초)
    virtual size_t getHeartbeatTimeout() const = 0;

    /// 패킷 body 최대 바이트 수
    virtual size_t getMaxPacketBodySize() const {
        return 4096; // TODO: config.에서 최대 크기를 지정할 것
    }

    bool putq(ClientId clientId, const Message* message);

    /// GameClientSession의 출력큐에 메세지를 넣는다.
    bool postClientMessage(ClientId clientId, const Message* msg);
    bool postClientMessage(const ClientIds& clientIds, const Message* msg);
    bool postClientMessageToAll(const Message* msg);

    /// GameClientSession의 접속을 해제한다.
    bool postHangupClientMessage(ClientId clientId);
    bool postHangupClientMessage(const ClientIds& clientIds);

    /// GameClientSession의 입력큐에 메세지를 넣는다.
    bool pendClientMessage(ClientId clientId, const Message* msg);

    /// clientId가 인증에 성공하였다
    void onAuthenticated(ClientId clientId);

    /// 연결된 클라이언트의 수를 얻는다.
    size_t getClientCount() const;

    /// 클라이언트가 존재하는가?
    bool isExist(ClientId clientId) const;

    /// 클라이언트의 접속 주소를 얻는다
    std::string getIpAddress(ClientId clientId) const;

    ClientId getClientId() const {
        return clientId_;
    }

    virtual bool isUsingCipher() const {
        return true; // TODO: config.으로 옮길 것
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
    /// 메세지를 처리한다.
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

    Queues msgQueues_; // locking 필요 없음

    ProactorEventLooper proactorEventLooper_;
    ReactorEventLooper reactorEventLooperForLogRotation_;

    bool useHackShield_; ///< 핵쉴드 사용 여부
    std::string crcFilename_; ///< 핵쉴드 CRC 파일명
    size_t monitorInterval_;

    std::auto_ptr<HackShield> hackShield_;
};

} // namespace skynet

#endif // !defined(INCLUDED_WARNETSERVICE)
