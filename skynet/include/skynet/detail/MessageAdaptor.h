/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// MessageAdaptor.h
#if !defined(INCLUDED_MESSAGEADAPTOR)
#define INCLUDED_MESSAGEADAPTOR

#include <ace/Basic_Types.h>
#include <boost/utility.hpp>

class ACE_Message_Block;

namespace skynet
{

namespace Wdr
{
    class OutputWdr;
    class InputWdr;
} // namespace Wdr

/**
* @class MessageAdaptor
*
* 메세지 serializing adaptor
*/
class MessageAdaptor : public boost::noncopyable
{
public:
    MessageAdaptor() {}
    virtual ~MessageAdaptor() {}

    virtual size_t getHeaderSize() const = 0;

    virtual bool encrypt(ACE_Message_Block& block, Wdr::OutputWdr& header) = 0;
    virtual bool decrypt(Wdr::InputWdr& packet) = 0;

    virtual void setSecuritySeed(ACE_UINT32 securitySeed) = 0;
};

} // namespace skynet

#endif // !defined(INCLUDED_MESSAGEADAPTOR)
