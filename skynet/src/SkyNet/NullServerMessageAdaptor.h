/**
* Copyright 
*
* @author 
*/
// NullServerMessageAdaptor.h
#if !defined(INCLUDED_NULLSERVERMESSAGEADAPTOR)
#define INCLUDED_NULLSERVERMESSAGEADAPTOR

#include <skynet/detail/ServerMessageAdaptor.h>

namespace skynet
{

/**
 * @class NullServerMessageAdaptor
 *
 * NullServer Object
 */
class NullServerMessageAdaptor : public ServerMessageAdaptor
{
public:
    NullServerMessageAdaptor(MessageAdaptor* adaptor) :
        ServerMessageAdaptor(adaptor) {}

    virtual bool setFirstSecureSeed(ACE_HANDLE /*handle*/) {
        return true;
    }
};

} // namespace skynet

#endif // !defined(INCLUDED_NULLSERVERMESSAGEADAPTOR)
