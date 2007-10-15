/**
* Copyright (c) 
*
* @author 
*/
// NullClientMessageAdaptor.h
#if !defined(INCLUDED_NULLCLIENTMESSAGEADAPTOR)
#define INCLUDED_NULLCLIENTMESSAGEADAPTOR

#include <skynet/detail/ClientMessageAdaptor.h>

namespace skynet
{

class WelcomeMessage;

/**
 * @class NullClientMessageAdaptor
 *
 * NullClient Object
 */
class NullClientMessageAdaptor : public ClientMessageAdaptor
{
public:
    NullClientMessageAdaptor(MessageAdaptor* adaptor) :
        ClientMessageAdaptor(adaptor) {}

    virtual bool setFirstSecureSeed(ACE_HANDLE /*handle*/) {
        return true;
    }
};

} // namespace skynet

#endif // !defined(INCLUDED_NULLCLIENTMESSAGEADAPTOR)
