/**
* Copyright (c) 2006 Mega Enterprise Co., Ltd.
*
* @author±è ¿µÁß <yjjy2021@megaking.co.kr>
*/
// ServerClientSession.h
#if !defined(INCLUDED_SERVERCLIENTSESSION)
#define INCLUDED_SERVERCLIENTSESSION

#include <warnet/server/ClientSession.h>

namespace warnet
{

class ServerProxy;
class ClientMessageAdaptor;

/**
* @class ServerClientSession
*
* Game Client I/O Handler
*/

class WARNET_API ServerClientSession : public ClientSession
{
public:
    ServerClientSession(ServerProxy* proxy = 0,
        MessageBuilderRepository* builderRepository = 0,
        bool useCipher = false);
    virtual ~ServerClientSession();

    virtual void open(ACE_HANDLE newHandle, ACE_Message_Block& messsageBlock);
private:
    virtual size_t getMaxPacketBodySize() const;
    virtual bool onInputQueue(const Message* message);
    virtual void onDisconnected();

    virtual MessageAdaptor& getMessageAdaptor() const;
private:
    ServerProxy& proxy_;
    std::auto_ptr<ClientMessageAdaptor> messageAdaptor_;
};

} // namespace warnet

#endif // !defined(INCLUDED_ServerCLIENTSESSION)
