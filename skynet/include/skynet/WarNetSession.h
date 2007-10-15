/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
 * Ŭ���̾�Ʈ�� �����ؼ� Ŀ�ؼ� ��ü�� ����Ѵ�
 * �������� Reactor Service Handler�� �ռ����� �����ش� 
 */
class WARNET_API WarNetSession : public boost::noncopyable
{
    enum { DEFAULT_CONNECTION_TIMEOUT = 3 }; // ��    

public:    
    /**
    * ctor.
    * @param adaptor MessageBuilder�� Event ����� ���� �����
    * @param useHackShield �ٽ��带 ����� ���ΰ�?
    */
    WarNetSession(SessionAdaptor* adaptor = 0,
        ACE_Log_Msg_Callback* callback = 0,
        ReactorLifeCycle* lifeCycle = 0,
        bool useHackShield = false);
    virtual ~WarNetSession();

    /**
     * ������ ������ �õ��Ѵ�
     * @param address ������ �ּ� 
     * @param port ������ ��Ʈ
     * @param connect_timeout ���� �õ� �ð� ����(�� ����)
     */
    bool connect(const std::string& address, unsigned short port,
        int connect_timeout = DEFAULT_CONNECTION_TIMEOUT);

    /**
     * WarNet���ƿ��� �� �޼��带 ȣ������ ���Ѵ�, Ŭ���̾�Ʈ �����忡��
     * ȣ�� �ؾ���. Event::call() ��� WarNet �����̱� ������.
     * - ��Ʈ�� �޼����� �Դٴ� call() �ݹ��� ������ �̸޼��忡��
     *   disconnect�� ȣ���ϸ� �ȵ�.
     * @param fireOnDisconnect WarNetSessionEventHandler�� onDisconnected
     *                         ȣ�� ���θ� �����Ѵ�.
     */
    void disconnect(bool callback = false);

    /// �޼����� �����Ѵ�
    bool send(const Message& msg);

    /// �� �ν��Ͻ��� �̺�Ʈ�� �����޾� ������ �������̽��� ����Ѵ�
    void registerEventHandler(WarNetSessionEventHandler* eventHandler);

    /// �ٽ��� Ŭ���̾�Ʈ ó�� �θ� ����Ѵ�
    void registerHackingSensor(HackingSensor* sensor);

    /// ��Ʈ��ũ �޼����� ó���� �̺�Ʈ ó���⸦ ����Ѵ�
    void registerMessageHandler(Event* messageHandler);

    /// �޼��� ������ ����Ѵ�(builder�� �ݵ�� �����Ҵ��ؾ� �Ѵ�)
    void registerMessageBuilder(MessageBuilder* builder);      

    bool isConnected() const;
private:
    ReactorLifeCycle* reactorLifeCycle_;
    std::auto_ptr<SkyNetClient> wnClient_;
};

} // namespace skynet

#endif // !defined(INCLUDED_WARNETSESSION)
