/**
* Copyright (c) 
*
* @author 
*/
// NullMessageAdaptor.h
#if !defined(INCLUDED_NULLMESSAGEADAPTOR)
#define INCLUDED_NULLMESSAGEADAPTOR

#include <skynet/detail/MessageAdaptor.h>

namespace skynet
{

/**
* @class NullMessageAdaptor
*
* ¸Þ¼¼Áö serializing adaptor
*/
class NullMessageAdaptor : public MessageAdaptor
{
public:
    NullMessageAdaptor() {}

    virtual size_t getHeaderSize() const {
        return 0;
    }

    virtual bool encrypt(ACE_Message_Block& /*block*/,
            Wdr::OutputWdr& /*header*/) {
        return true;
    }

    virtual bool decrypt(Wdr::InputWdr& /*packet*/) {
        return true;
    }

    virtual void setSecuritySeed(ACE_UINT32 /*securitySeed*/) {}
};

} // namespace skynet

#endif // !defined(INCLUDED_NULLMESSAGEADAPTOR)
