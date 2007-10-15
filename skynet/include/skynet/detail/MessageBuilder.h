/**
* Copyright (c)
*
* @author
*/
// MessageBuilder.h
#if !defined(INCLUDED_MESSAGEBUILDER)
#define INCLUDED_MESSAGEBUILDER

#include "../Protocol.h"
#include <boost/utility.hpp>
#pragma warning ( push)
#pragma warning ( disable: 4702)
//#include <hash_map>
#include "HashMap.h"
#pragma warning ( pop)

class ACE_Message_Block;

namespace skynet
{

class Message;
class MessageAdaptor;

// Little helper class
/**
* @class MessageBuilder
*
* byte-stream(ACE_Message_Block)으로 부터 Message 파생 객체를 만든다.
*/
class WARNET_API MessageBuilder : public boost::noncopyable
{
public:
    MessageBuilder() {}
    virtual ~MessageBuilder() {}

    virtual MessageType getMessageType() const = 0;
private:
    friend class MessageBuilderRepository;
    virtual Message* create() = 0;
};


/**
* @class MessageBuilderT
*
*/
template <class MessageClass>
class MessageBuilderT : public MessageBuilder
{
public:
    virtual MessageType getMessageType() const {
        return MessageClass::thisMessageType;
    }
private:
    virtual Message* create() {
        return new MessageClass;
    }
};


/**
 * @class MessageBuilderRepository
 *
 * MessageBuilder 저장소
 */
class WARNET_API MessageBuilderRepository : public boost::noncopyable
{
    //typedef stdext::hash_map<MessageType, MessageBuilder*> BuilderMap;
	typedef Hash_Map<MessageType, MessageBuilder*>  BuilderMap;

public:
    ~MessageBuilderRepository();

    /// MessageBuilder를 등록한다. @c builder는 반드시 동적할당해야 한다
    void add(MessageBuilder* builder);

    /// build now
    Message* build(ACE_Message_Block& block, MessageAdaptor& adaptor) const;
private:
    BuilderMap repository_;
};

} // namespace skynet

#endif // !defined(INCLUDED_MESSAGEBUILDER)
