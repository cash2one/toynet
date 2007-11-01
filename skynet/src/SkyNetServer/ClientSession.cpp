/**
* Copyright (c) 
*
* @author 
*/
// ClientSession.cpp
//
#include "stdafx.h"
#include <skynet/server/ClientSession.h>
#include <skynet/Message.h>
#include <skynet/detail/MessageAdaptor.h>
#include <skynet/detail/MessageBuilder.h>
#include <skynet/detail/Logging.h>
#include <skynet/detail/AceUtil.h>

#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4251 4267 4311 4312 )
#include <ace/Proactor.h>
#pragma warning (pop)

#include <memory>
#include <skynet/detail/Assert.h>

namespace skynet
{

// = ClientSession

ClientSession::ClientSession(MessageBuilderRepository* builderRepository) :
    builderRepository_(builderRepository),
    mblock_(0),
    clientId_(INVALID_CLIENTID),
    sentBytes_(0),
    sentPacketCount_(0),
    recvBytes_(0),
    recvPacketCount_(0)
{
    WN_ASSERT(builderRepository_ != 0);
}


ClientSession::~ClientSession()
{
    destroyMessageBlock();
}


void ClientSession::open(ACE_HANDLE newHandle,
    ACE_Message_Block& /*messageBlock*/)
{
    clientId_ = INVALID_CLIENTID;
    destroyMessageBlock();

    setMaximumSocketBufferSize(newHandle);

    initAio(newHandle);

    (void)startRead();
}


void ClientSession::addresses(const ACE_INET_Addr &remote_address,
    const ACE_INET_Addr &local_address)
{
    remoteAddress_ = remote_address;
    localAddress_ = local_address;

    WN_LOG_INFO2(ACE_TEXT("Client(%s) is connecting.\n"),
        getConnectionString().c_str());
}


void ClientSession::handle_read_stream(
    const ACE_Asynch_Read_Stream::Result& result)
{
    if (! isConnected())
        return;

    if (result.success() && (result.bytes_transferred() != 0)) {
        recvBytes_ += result.bytes_transferred();
        if (readMessage(result.bytes_to_read() - result.bytes_transferred())) {
            putInputQueue(mblock_);
            mblock_ = 0;
            if (! startRead()) {
                disconnect();
            }
        }
    }
    else {
        WN_LOG_DEBUG3(
            ACE_TEXT("ClientSession::handle_read_stream() error")
            ACE_TEXT("(%d,%m,#%d)\n"),
                result.error(), clientId_);
        disconnect();
    }
}


void ClientSession::handle_write_stream(
    const ACE_Asynch_Write_Stream::Result& result)
{
    result.message_block().release();

    if (! isConnected())
        return;

    if (result.success()) {
        WN_ASSERT(result.bytes_to_write() == result.bytes_transferred());
        sentBytes_ += result.bytes_transferred();
        ++sentPacketCount_;
    }
    else {
        WN_LOG_DEBUG2(
            ACE_TEXT("ClientSession::handle_write_stream() error(%d,%m)\n"),
                result.error());
        disconnect();
    }
}


void ClientSession::disconnect()
{
    if (! isConnected()) {
        return;
    }

    if (reader_.get() != 0) {
        reader_->cancel();
    }
    if (writer_.get() != 0) {
        writer_->cancel();
    }
    closeSocket();

    onDisconnected();

    WN_LOG_INFO6(ACE_TEXT("Client(#%d) STATISTICS(sent:%d/%d, recv:%d/%d)\n"),
        clientId_, sentBytes_, sentPacketCount_, recvBytes_, recvPacketCount_);
}


void ClientSession::closeSocket()
{
    ACE_HANDLE h = handle();
    handle(ACE_INVALID_HANDLE);
    ACE_OS::closesocket(h);
}


void ClientSession::initAio(ACE_HANDLE newHandle)
{
    reader_.reset(new ACE_Asynch_Read_Stream);
    writer_.reset(new ACE_Asynch_Write_Stream);
    reader_->open(*this, newHandle, 0, proactor());
    writer_->open(*this, newHandle, 0, proactor());
}


bool ClientSession::startRead()
{
    const size_t defaultBlockSize = ACE_DEFAULT_CDR_BUFSIZE;

    ACE_NEW_RETURN(mblock_, ACE_Message_Block(defaultBlockSize), false);
    if (reader_->read(*mblock_, Message::getHeaderSize()) == -1) {
        WN_LOG_DEBUG2(
            ACE_TEXT("ACE_Asynch_Read_Stream::read() error(%d,%m)\n"),
            ACE_OS::last_error());
        return false;
    }
    return true;
}


void ClientSession::startWrite(ACE_Message_Block* mblk /*= 0*/)
{
    if (! mblk)
        return;

    if (writer_->write(*mblk, mblk->length()) == -1) {
        WN_LOG_DEBUG3(
            ACE_TEXT("ACE_Asynch_Write_Stream::write() error")
            ACE_TEXT("(%d,%m,#%d)\n"),
            ACE_OS::last_error(), clientId_);
        mblk->release();
        disconnect();
    }
}


bool ClientSession::readMessage(size_t needed_bytes)
{
    WN_ASSERT(isConnected());

    if (needed_bytes > 0) {
        if (reader_->read(*mblock_, needed_bytes) == -1) {
            WN_LOG_DEBUG2(
                ACE_TEXT("ACE_Asynch_Read_Stream::read() error(%d,%m)\n"),
                ACE_OS::last_error());
            disconnect();
        }
        return false;
    }

    if (mblock_->length() == Message::getHeaderSize()) {
        MessageType msgType;
        size_t bodySize;
        if ((! extractHeader(*mblock_, msgType, bodySize)) ||
                (! isValidPacket(msgType, bodySize))) {
            WN_LOG_INFO4(ACE_TEXT("Client(#%d) is forcingly disconnected,")
                ACE_TEXT("because invalid packet(%d,%d).\n"),
                clientId_, msgType, bodySize);
            disconnect();
            return false;
        }
        if (bodySize > 0) {
            mblock_->size(Message::getHeaderSize() + bodySize);
            if (reader_->read(*mblock_, bodySize) == -1) {
                WN_LOG_DEBUG2(
                    ACE_TEXT("ACE_Asynch_Read_Stream::read() error(%d,%m)\n"),
                    ACE_OS::last_error());
                disconnect();
            }
            return false;
        }
    }
    
    return mblock_->length() >= Message::getHeaderSize();
}


void ClientSession::putInputQueue(ACE_Message_Block* mblock)
{
    WN_ASSERT(builderRepository_ != 0);
    WN_ASSERT(mblock != 0);

    ++recvPacketCount_;

    //WN_LOG_DEBUG3("#%d packet is arrived(#%d).\n",
    //    recvPacketCount_, clientId_);

    AceMessageBlockGuard guard(mblock);
    if (isConnected()) {
        const Message* msg = builderRepository_->build(*guard,
            getMessageAdaptor());
        if (! msg) {
            WN_LOG_INFO2(ACE_TEXT("Disconnect Client(#%d),")
                ACE_TEXT("because failing a message parsing.\n"), clientId_);
            disconnect();
        }
        else {
            if (! onInputQueue(msg))
                delete msg;
        }
    }
}


bool ClientSession::sendMessage(const Message* msg)
{
    if (! isConnected())
        return false;

    if (msg != 0) {
        ACE_Message_Block* mb;
        if (! msg->serializeTo(mb, getMessageAdaptor()))
            return false;
        startWrite(mb);
    }
    else {
        disconnect();
    }
   
    return true;
}


bool ClientSession::pendMessage(const Message* msg)
{
    WN_ASSERT(msg != 0);

    if (! isConnected())
        return false;

    return onInputQueue(msg);
}


bool ClientSession::isConnected() const
{
    return handle() != ACE_INVALID_HANDLE;
}


bool ClientSession::isValidPacket(MessageType msgType, size_t bodySize)
{
    if (MT_UNKNOWN >= msgType) {
        return false;
    }

    return bodySize <= getMaxPacketBodySize();
}


std::string ClientSession::getConnectionString() const
{
    ACE_TCHAR remote[MAXHOSTNAMELEN];
    if (remoteAddress_.addr_to_string(remote, MAXHOSTNAMELEN) != 0) {
        ACE_OS::strcpy(remote, "unknown");
    }

    ACE_TCHAR local[MAXHOSTNAMELEN];
    if (localAddress_.addr_to_string(local, MAXHOSTNAMELEN) != 0) {
        ACE_OS::strcpy(local, "unknown");
    }

    return std::string(remote) + "->" + local;
}


std::string ClientSession::getIpAddress() const
{
    ACE_TCHAR remote[MAXHOSTNAMELEN] = { '\0' };
    (void)remoteAddress_.get_host_addr(remote, MAXHOSTNAMELEN);

    return remote;
}


void ClientSession::destroyMessageBlock()
{
    if (mblock_ != 0) {
        mblock_->release();
        mblock_ = 0;
    }
}

} // namespace skynet
