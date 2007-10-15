/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// MessageAdaptorFactory.h
#if !defined(INCLUDED_MESSAGEADAPTORFACTORY)
#define INCLUDED_MESSAGEADAPTORFACTORY

namespace skynet
{

class MessageAdaptor;

/**
* @class MessageAdaptorFactory
*
* �޼��� serializing adaptor factory
*/
struct WARNET_API MessageAdaptorFactory
{

    static MessageAdaptor* create(bool useCipher);

};

} // namespace skynet

#endif // !defined(INCLUDED_MESSAGEADAPTORFACTORY)
