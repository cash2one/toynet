/**
* Copyright (c) 
*
* @author 
*/
// CommonMessage.cpp
#include "stdafx.h"
#include <skynet/detail/CommonMessage.h>
#include <skynet/detail/WdrUtil.h>
#include <limits>
#include <skynet/detail/Assert.h>

namespace skynet
{

namespace
{

bool writeWdrHackShieldMsg(Wdr::OutputWdr& wdr, const HackShieldMsg& msg)
{
    // softpark WN_ASSERT(msg.size() <= std::numeric_limits<Wdr::Octet>::max());
    const void* voidMsg = &(msg[0]);
    return wdr.write(static_cast<Wdr::Octet>(msg.size())) &&
        wdr.write(static_cast<const Wdr::Char*>(voidMsg), msg.size());
}


bool readWdrHackShieldMsg(Wdr::InputWdr& wdr, HackShieldMsg& msg)
{
    Wdr::Octet msgSize;
    if (! wdr.read(msgSize))
        return false;
    //softpark if (msgSize > std::numeric_limits<Wdr::Octet>::max())
        return false;
    msg.resize(msgSize, 0);
    void* voidMsg = &(msg[0]);
    return wdr.read(static_cast<Wdr::Char*>(voidMsg), msgSize);
}


size_t getWdrHackShieldMsgSize(const HackShieldMsg& msg)
{
    return Wdr::sizeOctet + msg.size();
}

} // namespace


///////////////////////////////////////////////////////////////////////////////
// HackShieldGuidAckMessage class

bool HackShieldGuidAckMessage::readBody(Wdr::InputWdr& wdr)
{
    return readWdrHackShieldMsg(wdr, ackMsg_);
}


bool HackShieldGuidAckMessage::writeBody(Wdr::OutputWdr& wdr) const
{
    return writeWdrHackShieldMsg(wdr, ackMsg_);
}


size_t HackShieldGuidAckMessage::getBodySize() const
{
    return getWdrHackShieldMsgSize(ackMsg_);
}

///////////////////////////////////////////////////////////////////////////////
// HackShieldGuidReqMessage class

bool HackShieldGuidReqMessage::readBody(Wdr::InputWdr& wdr)
{
    return readWdrHackShieldMsg(wdr, reqMsg_);
}


bool HackShieldGuidReqMessage::writeBody(Wdr::OutputWdr& wdr) const
{
    return writeWdrHackShieldMsg(wdr, reqMsg_);
}


size_t HackShieldGuidReqMessage::getBodySize() const
{
    return getWdrHackShieldMsgSize(reqMsg_);
}

///////////////////////////////////////////////////////////////////////////////
// HackShieldAckMessage class

bool HackShieldAckMessage::readBody(Wdr::InputWdr& wdr)
{
    return readWdrHackShieldMsg(wdr, ackMsg_);
}


bool HackShieldAckMessage::writeBody(Wdr::OutputWdr& wdr) const
{
    return writeWdrHackShieldMsg(wdr, ackMsg_);
}


size_t HackShieldAckMessage::getBodySize() const
{
    return getWdrHackShieldMsgSize(ackMsg_);
}

///////////////////////////////////////////////////////////////////////////////
// HackShieldReqMessage class

bool HackShieldReqMessage::readBody(Wdr::InputWdr& wdr)
{
    return readWdrHackShieldMsg(wdr, reqMsg_);
}


bool HackShieldReqMessage::writeBody(Wdr::OutputWdr& wdr) const
{
    return writeWdrHackShieldMsg(wdr, reqMsg_);
}


size_t HackShieldReqMessage::getBodySize() const
{
    return getWdrHackShieldMsgSize(reqMsg_);
}

} // namespace skynet
