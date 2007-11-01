/**
* Copyright (c) 
*
* @author 
*/
// ClientSession.h
#if !defined(INCLUDED_CLIENTSESSION)
#define INCLUDED_CLIENTSESSION

#include "ClientId.h"
#include <skynet/Protocol.h>
#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4251 4267 4311 4312 4355 )
#include <ace/Asynch_IO.h>
#include <ace/INET_Addr.h>
#include <ace/Message_Block.h>
#pragma warning(pop)
#include <boost/utility.hpp>
#include <memory>
#include <skynet/detail/Assert.h>

namespace skynet
{

class MessageBuilderRepository;
class Message;
class MessageAdaptor;

/**
 * @class ClientSession
 *
 * Client I/O Handler
 */
class WARNET_API ClientSession :
    public ACE_Service_Handler,
    public boost::noncopyable
{
public:
    ClientSession(MessageBuilderRepository* builderRepository = 0);
    virtual ~ClientSession();

    virtual void open(ACE_HANDLE newHandle, ACE_Message_Block& messsage_block);
    virtual void addresses(const ACE_INET_Addr& remote_address,
        const ACE_INET_Addr& local_address);

    void disconnect();

    void setClientId(ClientId clientId) {
        clientId_ = clientId;
    }

    bool sendMessage(const Message* msg);

    bool pendMessage(const Message* msg);

    ClientId getClientId() const {
        return clientId_;
    }

    std::string getConnectionString() const;
    std::string getIpAddress() const;
protected:
    virtual void handle_read_stream(
        const ACE_Asynch_Read_Stream::Result& result);
    virtual void handle_write_stream(
        const ACE_Asynch_Write_Stream::Result& result);

    virtual bool onInputQueue(const Message* message) = 0;
    virtual size_t getMaxPacketBodySize() const = 0;
private:
    virtual void onDisconnected() = 0;
    virtual MessageAdaptor& getMessageAdaptor() const = 0;
protected:
    bool isConnected() const;
private:
    void initAio(ACE_HANDLE newHandle);

    /// start AIO reading
    bool startRead();

    /// start AIO writing
    void startWrite(ACE_Message_Block* mblk = 0);

    bool readMessage(size_t needed_bytes);

    bool isValidPacket(MessageType msgType, size_t bodySize);

    void closeSocket();

    void putInputQueue(ACE_Message_Block* mblock);

    void destroyMessageBlock();
private:
    MessageBuilderRepository* builderRepository_;

    ACE_Message_Block* mblock_;

    std::auto_ptr<ACE_Asynch_Read_Stream> reader_;
    std::auto_ptr<ACE_Asynch_Write_Stream> writer_;

    ACE_INET_Addr remoteAddress_;
    ACE_INET_Addr localAddress_;

    ClientId clientId_;

    size_t sentBytes_;
    size_t sentPacketCount_;

    size_t recvBytes_;
    size_t recvPacketCount_;
};

} // namespace skynet

#endif // !defined(INCLUDED_CLIENTSESSION)
