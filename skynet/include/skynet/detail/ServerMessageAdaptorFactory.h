/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ServerMessageAdaptorFactory.h
#if !defined(INCLUDED_SERVERMESSAGEADAPTORFACTORY)
#define INCLUDED_SERVERMESSAGEADAPTORFACTORY

namespace skynet
{

class ServerMessageAdaptor;

/**
* @class ServerMessageAdaptorFactory
*
* 메세지 serializing adaptor
*/
struct WARNET_API ServerMessageAdaptorFactory
{

    static ServerMessageAdaptor* create(bool useCipher);

};

} // namespace skynet

#endif // !defined(INCLUDED_SERVERMESSAGEADAPTORFACTORY)
