/**
* Copyright (c) 
*
* @author 
*/
#include "stdafx.h"
#include <skynet/detail/ForwardHelper.h>
#include <skynet/detail/MessageBuilder.h>
#include <skynet/detail/MessageAdaptorFactory.h>
#include <skynet/detail/MessageAdaptor.h>
#include <skynet/arcade/ForwardServerMessage.h>
#include <ace/Message_Block.h>

namespace skynet
{

// = MessageToBuffer

MessageToBuffer::MessageToBuffer()
{
    init();
}


MessageToBuffer::MessageToBuffer(const Message& msg)
{
    init();
    (void)parse(msg);
}


MessageToBuffer::~MessageToBuffer()
{
    release();
}


void MessageToBuffer::init()
{
    mblock_ = 0;
    messageAdaptor_.reset(MessageAdaptorFactory::create(false));
}


void MessageToBuffer::release()
{
    if (mblock_ != 0) {
        mblock_->release();
        mblock_ = 0;
    }
}


bool MessageToBuffer::parse(const Message& msg)
{
    release();
    return msg.serializeTo(mblock_, *messageAdaptor_);
}


const char* MessageToBuffer::getBuffer() const
{
    if (mblock_ != 0) {
        return mblock_->base();
    }

    return 0;
}


size_t MessageToBuffer::getBufferLength() const
{
    if (mblock_ != 0) {
        return mblock_->length();
    }
    return 0;
}

// = BufferToMessage

BufferToMessage::BufferToMessage() :
    builderRepository_(new MessageBuilderRepository),
    messageAdaptor_(MessageAdaptorFactory::create(false)),
    mblock_(new ACE_Message_Block)
{
    builderRepository_->add(new MessageBuilderT<ResolvingResultMessage>);
    builderRepository_->add(new MessageBuilderT<ForwardingMessage>);
}


BufferToMessage::~BufferToMessage()
{
}


const Message* BufferToMessage::getMessage(const char* buffer,
    size_t bufferLength) const
{
    WN_ASSERT(buffer != 0);
    WN_ASSERT(bufferLength > 0);

    mblock_->reset();
    mblock_->size(bufferLength);
    mblock_->copy(buffer, bufferLength);
    return builderRepository_->build(*mblock_, *messageAdaptor_);
}

} // namespace skynet
