/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ManagerServerConnector.h
#if !defined(INCLUDED_SERVERCONNECTOR)
#define INCLUDED_SERVERCONNECTOR

#include <warnet/poporu/PoporuCommonInfo.h>
#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4251 4267 4311 4312 4355 )
#include <ace/Asynch_Connector.h>
#pragma warning(pop)
#include <memory>

namespace warnet
{

class ServerClientSession;
class ServerProxy;

/**
* @class ManagerServerConnector
*
* ManagerServer에 연결한다
* - 연결될 때까지 주기적으로 연결을 시도한다
*/
class WARNET_API ServerConnector :
    public ACE_Asynch_Connector<ServerClientSession>
{
    enum { INITIAL_RETRY_DELAY = 1, MAX_RETRY_DELAY = 10 };

    typedef ACE_Asynch_Connector<ServerClientSession> PARENT;
public:
    ServerConnector(ServerProxy& proxy,
        MessageBuilderRepository& builderRepository, bool useCipher);
    virtual ~ServerConnector();

    void connect(const AddressPair& pair);

    /// Re-establish a connection to the logging server.
    void reconnect();
private:
    virtual int validate_connection(const ACE_Asynch_Connect::Result& result,
        const ACE_INET_Addr& remote, const ACE_INET_Addr& local);

    virtual void handle_time_out(const ACE_Time_Value&, const void*);

    /// Template method to create a new handler
    virtual ServerClientSession* make_handler();
private:
    void connect();
private:
    ServerProxy& proxy_;
    std::auto_ptr<ServerClientSession> session_;

    ACE_INET_Addr remoteAddr_;
    long retryDelay_;
};

} // namespace warnet

#endif // !defined(INCLUDED_MANAGERSERVERCONNECTOR)
