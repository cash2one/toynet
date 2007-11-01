/**
* Copyright (c)
*
* @author 
*/
// SecureClientMessageAdaptor.h
#if !defined(INCLUDED_SECURECLIENTMESSAGEADAPTOR)
#define INCLUDED_SECURECLIENTMESSAGEADAPTOR

#include <skynet/detail/ClientMessageAdaptor.h>

namespace skynet
{

class WelcomeMessage;

/**
 * @class SecureClientMessageAdaptor
 *
 * Cipher Message Adaptor
 */
class SecureClientMessageAdaptor : public ClientMessageAdaptor
{
public:
    SecureClientMessageAdaptor(MessageAdaptor* adaptor);

    virtual bool setFirstSecureSeed(ACE_HANDLE handle);
};

} // namespace skynet

#endif // !defined(INCLUDED_SECURECLIENTMESSAGEADAPTOR)
