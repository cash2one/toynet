/**
* Copyright (c) 
*
* @author
*/
// SecureMessageAdaptor.h
#if !defined(INCLUDED_SECUREMESSAGEADAPTOR)
#define INCLUDED_SECUREMESSAGEADAPTOR

#include <skynet/detail/MessageAdaptor.h>
#ifdef USE_CHANGED_SECURITY_KEY
#include <boost/random/linear_congruential.hpp>
#endif

namespace skynet
{

/**
* @class SecureMessageAdaptor
*
* 메세지 serializing adaptor
* - 테스트를 위해 export
*/
class WARNET_API SecureMessageAdaptor : public MessageAdaptor
{
public:
    SecureMessageAdaptor();

    virtual size_t getHeaderSize() const;

    virtual bool encrypt(ACE_Message_Block& block, Wdr::OutputWdr& header);
    virtual bool decrypt(Wdr::InputWdr& packet);

    virtual void setSecuritySeed(ACE_UINT32 securitySeed) {
#ifdef USE_CHANGED_SECURITY_KEY
        encryptKey_.seed(static_cast<boost::int32_t>(securitySeed));
        decryptKey_.seed(static_cast<boost::int32_t>(securitySeed));
#else
        securityKey_ = securitySeed;
#endif
    }

//private: 테스트를 위해 public으로 선언
    ACE_UINT32 getEncryptSecurityKey() {
#ifdef USE_CHANGED_SECURITY_KEY
        return encryptKey_();
#else
        return securityKey_;
#endif
    }
    ACE_UINT32 getDecryptSecurityKey() {
#ifdef USE_CHANGED_SECURITY_KEY
        return decryptKey_();
#else
        return securityKey_;
#endif
    }
private:
#ifdef USE_CHANGED_SECURITY_KEY
    boost::rand48 encryptKey_;
    boost::rand48 decryptKey_;
#else
    ACE_UINT32 securityKey_;
#endif
};

} // namespace skynet

#endif // !defined(INCLUDED_SECUREMESSAGEADAPTOR)
