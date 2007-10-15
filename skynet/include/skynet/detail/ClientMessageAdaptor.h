/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ClientMessageAdaptor.h
#if !defined(INCLUDED_CLIENTMESSAGEADAPTOR)
#define INCLUDED_CLIENTMESSAGEADAPTOR

#include <boost/utility.hpp>

namespace skynet
{

class Message;
class MessageAdaptor;

/**
* @class ClientMessageAdaptor
*
* Ŭ���̾�Ʈ�� �޼��� serializing adaptor
*/
class WARNET_API ClientMessageAdaptor : public boost::noncopyable
{
public:
    ClientMessageAdaptor(MessageAdaptor* adaptor) : 
        messageAdaptor_(adaptor) {}

    virtual ~ClientMessageAdaptor() {}

    virtual bool setFirstSecureSeed(ACE_HANDLE handle) = 0;

    MessageAdaptor& getMessageAdaptor() const {
        return *messageAdaptor_;
    }
private:
    std::auto_ptr<MessageAdaptor> messageAdaptor_;
};

} // namespace skynet

#endif // !defined(INCLUDED_CLIENTMESSAGEADAPTOR)
