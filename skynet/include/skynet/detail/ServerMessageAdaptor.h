/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ServerMessageAdaptor.h
#if !defined(INCLUDED_SERVERMESSAGEADAPTOR)
#define INCLUDED_SERVERMESSAGEADAPTOR

#include <boost/utility.hpp>

class ACE_Message_Block;

namespace skynet
{

class MessageAdaptor;

/**
* @class ServerMessageAdaptor
*
* ������ �޼��� serializing adaptor
*/
class WARNET_API ServerMessageAdaptor : public boost::noncopyable
{
public:
    ServerMessageAdaptor(MessageAdaptor* adaptor) : 
        messageAdaptor_(adaptor) {}

    virtual ~ServerMessageAdaptor() {}

    virtual bool setFirstSecureSeed(ACE_HANDLE handle) = 0;

    MessageAdaptor& getMessageAdaptor() const {
        return *messageAdaptor_;
    }
private:
    std::auto_ptr<MessageAdaptor> messageAdaptor_;
};

} // namespace skynet

#endif // !defined(INCLUDED_SERVERMESSAGEADAPTOR)
