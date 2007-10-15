/**
* Copyright (c) 
*
* @author 
*/
// ForwardServerMessage.cpp
#include "stdafx.h"
#include <skynet/arcade/ForwardServerMessage.h>
#include <skynet/detail/WdrUtil.h>

namespace skynet
{

///////////////////////////////////////////////////////////////////////////////
// ResolvingMessage class

bool ResolvingMessage::readBody(Wdr::InputWdr& /*wdr*/)
{
    return true;
}


bool ResolvingMessage::writeBody(Wdr::OutputWdr& /*wdr*/) const
{
    return true;
}


size_t ResolvingMessage::getBodySize() const
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// ResolvingResultMessage class

bool ResolvingResultMessage::readBody(Wdr::InputWdr& wdr)
{
    return wdr.read(ip_) &&
        wdr.read(port_);
}


bool ResolvingResultMessage::writeBody(Wdr::OutputWdr& wdr) const
{
    return wdr.write(ip_) &&
        wdr.write(port_);
}


size_t ResolvingResultMessage::getBodySize() const
{
    return Wdr::sizeLong +
        Wdr::sizeShort;
}

///////////////////////////////////////////////////////////////////////////////
// ForwardingMessage class

bool ForwardingMessage::readBody(Wdr::InputWdr& wdr)
{
    if (! wdr.read(ip_))
        return false;
    if (! wdr.read(port_))
        return false;
    if (! wdr.read(dataSize_))
        return false;
    if (dataSize_ > 0) {
        data_.resize(dataSize_, 0);
        return wdr.read(&data_[0], dataSize_);
    }
    return wdr.isGood();
}


bool ForwardingMessage::writeBody(Wdr::OutputWdr& wdr) const
{
    WN_ASSERT(dataSize_ >= 0);
    WN_ASSERT(data_.size() >= 0);

    return wdr.write(ip_) &&
        wdr.write(port_) &&
        wdr.write(dataSize_) &&
        wdr.write(&data_[0], dataSize_);
}


size_t ForwardingMessage::getBodySize() const
{
    WN_ASSERT(dataSize_ == data_.size());
    return Wdr::sizeLong +
        Wdr::sizeShort +
        Wdr::sizeShort +
        dataSize_;
}


} // namespace skynet
