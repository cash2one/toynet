/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ClientMessageAdaptorFactory.h
#if !defined(INCLUDED_CLIENTMESSAGEADAPTORFACTORY)
#define INCLUDED_CLIENTMESSAGEADAPTORFACTORY

namespace skynet
{

class ClientMessageAdaptor;

/**
* @class ClientMessageAdaptorFactory
*
* �޼��� serializing adaptor
*/
struct WARNET_API ClientMessageAdaptorFactory
{

    static ClientMessageAdaptor* create(bool useCipher);

};

} // namespace skynet

#endif // !defined(INCLUDED_CLIENTMESSAGEADAPTORFACTORY)
