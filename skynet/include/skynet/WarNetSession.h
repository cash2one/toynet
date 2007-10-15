/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// WarNetSession.h
//
#if !defined(INCLUDED_WARNETSESSION)
#define INCLUDED_WARNETSESSION

#include <skynet/Protocol.h>
#include <boost/utility.hpp>
#include <string>
#include <memory>

class ACE_Log_Msg_Callback;

namespace skynet
{

class SessionAdaptor;
class SkyNetClient;
class Message;
class WarNetSessionEventHandler;
class HackingSensor;
class Event;
class MessageBuilder;
class ReactorLifeCycle;

/**
 * @class WarNetSession
 * 클라이언트가 생성해서 커넥션 객체로 사용한다
 * 실질적인 Reactor Service Handler를 합성으로 가려준다 
 */
class WARNET_API WarNetSession : public boost::noncopyable
{
    enum { DEFAULT_CONNECTION_TIMEOUT = 3 }; // 초    

public:    
    /**
    * ctor.
    * @param adaptor MessageBuilder와 Event 등록을 위한 어댑터
    * @param useHackShield 핵쉴드를 사용할 것인가?
    */
    WarNetSession(SessionAdaptor* adaptor = 0,
        ACE_Log_Msg_Callback* callback = 0,
        ReactorLifeCycle* lifeCycle = 0,
        bool useHackShield = false);
    virtual ~WarNetSession();

    /**
     * 서버에 연결을 시도한다
     * @param address 서버의 주소 
     * @param port 서버의 포트
     * @param connect_timeout 연결 시도 시간 제한(초 단위)
     */
    bool connect(const std::string& address, unsigned short port,
        int connect_timeout = DEFAULT_CONNECTION_TIMEOUT);

    /**
     * WarNet문맥에서 이 메서드를 호출하지 못한다, 클라이언트 스레드에서
     * 호출 해야함. Event::call() 모두 WarNet 문맥이기 때문임.
     * - 네트웍 메세지가 왔다는 call() 콜백이 왔을때 이메서드에서
     *   disconnect를 호출하면 안됨.
     * @param fireOnDisconnect WarNetSessionEventHandler의 onDisconnected
     *                         호출 여부를 결정한다.
     */
    void disconnect(bool callback = false);

    /// 메세지를 전송한다
    bool send(const Message& msg);

    /// 이 인스턴스의 이벤트를 통지받아 구현한 인터페이스를 등록한다
    void registerEventHandler(WarNetSessionEventHandler* eventHandler);

    /// 핵쉴드 클라이언트 처리 부를 등록한다
    void registerHackingSensor(HackingSensor* sensor);

    /// 네트워크 메세지를 처리할 이벤트 처리기를 등록한다
    void registerMessageHandler(Event* messageHandler);

    /// 메세지 빌더를 등록한다(builder는 반드시 동적할당해야 한다)
    void registerMessageBuilder(MessageBuilder* builder);      

    bool isConnected() const;
private:
    ReactorLifeCycle* reactorLifeCycle_;
    std::auto_ptr<SkyNetClient> wnClient_;
};

} // namespace skynet

#endif // !defined(INCLUDED_WARNETSESSION)
