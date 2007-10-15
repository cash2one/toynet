/**
* Copyright (c) 
*
* @author 
*/
// Message.cpp
#include "stdafx.h"
#include <skynet/Message.h>
#include <skynet/detail/WdrUtil.h>
#include <skynet/security/PacketCipher.h>
#include <skynet/detail/MessageAdaptor.h>
#include <ace/Message_Block.h>
#pragma warning( push )
#pragma warning( disable : 4244 4267 4311 4312 )
#include <ace/OS_Memory.h>
#pragma warning( pop )
#include <skynet/detail/Assert.h>

namespace skynet
{

namespace
{

bool extractHeader(Wdr::InputWdr& wdr, MessageType& msgType, size_t& bodySize)
{
    Wdr::UShort mt, bs;
    if (wdr.read(mt) && wdr.read(bs)) {
        msgType = mt;
        bodySize = bs;
        return true;
    }

    return false;
}

} // namespace

MessageType extractMessageType(ACE_Message_Block& packet)
{
    MessageType msgType;
    size_t bodySize;
    if (extractHeader(packet, msgType, bodySize))
        return msgType;
    return MT_UNKNOWN;
}


size_t extractBodySize(ACE_Message_Block& packet)
{
    MessageType msgType;
    size_t bodySize;
    if (extractHeader(packet, msgType, bodySize))
        return bodySize;
    return 0;
}


bool extractHeader(ACE_Message_Block& packet, MessageType& msgType,
    size_t& bodySize)
{
    Wdr::InputWdr wdr(packet);

    return extractHeader(wdr, msgType, bodySize);
}

// = Message

size_t Message::getHeaderSize()
{
    return 2 * Wdr::sizeShort;
}


bool Message::serializeFrom(ACE_Message_Block& packet,
    MessageAdaptor& adaptor)
{
    const size_t adaptedHeaderSize = adaptor.getHeaderSize();
    if (packet.length() < (getHeaderSize() + adaptedHeaderSize))
        return false;

    Wdr::InputWdr wdr(packet);

    MessageType messageType;
    size_t bodySize;
    if (! extractHeader(wdr, messageType, bodySize))
        return false;
    WN_ASSERT(messageType == getMessageType());

    if (wdr.length() != bodySize)
        return false;

    if (! adaptor.decrypt(wdr))
        return false;

    if (! readBody(wdr))
        return false;
    WN_ASSERT(wdr.isGood());

    return true;
}


bool Message::serializeTo(ACE_Message_Block*& packet,
    MessageAdaptor& adaptor) const
{
    const size_t adaptedHeaderSize = adaptor.getHeaderSize();
    const size_t bodySize = getBodySize();

    // header
    ACE_Message_Block headerBlock(
        getHeaderSize() + adaptedHeaderSize + bodySize);
    Wdr::OutputWdr header(headerBlock);

    // header: message type
    header.write(static_cast<Wdr::UShort>(getMessageType()));
    WN_ASSERT(header.isGood());

    // header: body size
    header.write(static_cast<Wdr::UShort>(bodySize + adaptedHeaderSize));
    WN_ASSERT(header.isGood());

    // body
    ACE_Message_Block bodyBlock(
        header.current().base() + getHeaderSize() + adaptedHeaderSize,
        bodySize);
    Wdr::OutputWdr body(bodyBlock);
    if (! writeBody(body))
        return false;
    WN_ASSERT(body.isGood());

    // encrypt & header - adapted header
    if (! adaptor.encrypt(body.current(), header))
        return false;

    WN_ASSERT(header.length() == (getHeaderSize() + adaptedHeaderSize));
    WN_ASSERT(body.length() == bodySize);

    packet = header.current().duplicate();
    WN_ASSERT(packet->size() == header.size());
    packet->wr_ptr(bodySize);
    WN_ASSERT(packet->length() == (header.length() + body.length()));

    return true;
}

} // namespace skynet
