/**
* Copyright (c)
*
* @author 
*/
// SecureServerMessageAdaptor.h
#if !defined(INCLUDED_SECURESERVERMESSAGEADAPTOR)
#define INCLUDED_SECURESERVERMESSAGEADAPTOR

#include <skynet/detail/ServerMessageAdaptor.h>

namespace skynet
{

/**
 * @class SecureServerMessageAdaptor
 *
 * Cipher Message Adaptor
 */
class SecureServerMessageAdaptor : public ServerMessageAdaptor
{
public:
    SecureServerMessageAdaptor(MessageAdaptor* adaptor);

    virtual bool setFirstSecureSeed(ACE_HANDLE handle);
};

} // namespace skynet

#endif // !defined(INCLUDED_SECURESERVERMESSAGEADAPTOR)
