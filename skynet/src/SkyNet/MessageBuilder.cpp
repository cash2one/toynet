/**
* Copyright (c) 
*
* @author 
*/
// MessageBuilder.cpp
#include "stdafx.h"
#pragma warning( push)
#pragma warning( disable : 4702)
#include <skynet/detail/MessageBuilder.h>
#pragma warning( pop)
#include <skynet/Message.h>
#include <ace/Message_Block.h>
#include <memory>
#include <skynet/detail/Assert.h>

namespace skynet
{

// = MessageBuilderRepository

MessageBuilderRepository::~MessageBuilderRepository()
{
    /*
    for (BuilderMap::const_iterator pos = repository_.begin();
            pos != repository_.end(); ++pos) {
        delete (*pos).second;
    }
    */

    for (BuilderMap::iterator iter = repository_.begin(); iter != repository_.end(); iter++)
    {
        delete (*iter).int_id_;

    }

    repository_.unbind_all();
}


void MessageBuilderRepository::add(MessageBuilder* builder)
{
    WN_ASSERT(builder != 0);
    WN_ASSERT(repository_.find(builder->getMessageType()));

   // repository_.insert( BuilderMap::value_type(builder->getMessageType(), builder));
   repository_.bind( builder->getMessageType(), builder);
}


Message* MessageBuilderRepository::build(ACE_Message_Block& block,
    MessageAdaptor& adaptor) const
{
    MessageType mt = extractMessageType(block);
    //BuilderMap::iterator iter = repository_.find(mt);
    MessageBuilder* pMB = 0 ;
    repository_.find(mt, pMB);
    if (pMB==0)
    {
	printf("Message Not found : %d", mt);
        return 0;
    }

    //MessageBuilder* builder = iter->int_id_;
    WN_ASSERT(pMB != 0);
    std::auto_ptr<Message> msg(pMB->create());
    WN_ASSERT(msg.get() != 0);
    if (! msg->serializeFrom(block, adaptor))
        return 0;
    return msg.release();

/*
  if (this->allocator_->find (ACE_CONFIG_SECTION_INDEX, section_index) == 0)    this->index_ = (SECTION_MAP *) section_index;
*/
}

} // namespace skynet
