/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// WarNetSessionEventHandler.h
//

#if !defined(INCLUDED_WARNETSESSIONEVENTHANDLER)
#define INCLUDED_WARNETSESSIONEVENTHANDLER

#include <skynet/Protocol.h>

namespace skynet
{

/**
* @class WarNetSessionEventHandler
*
* 서버와의 세션에서 연결/해제 이벤트를 처리한다
*/
class WARNET_API WarNetSessionEventHandler
{
public:
    virtual ~WarNetSessionEventHandler() {}

private:
    friend class SkyNetClient;

    /// 서버와의 접속이 해제되었다.
    virtual void onDisconnected() = 0;

    /// 메세지 파싱 에러가 발생하였다
    virtual void onMessageParsingError(MessageType messageType) = 0;
};

} // namespace skynet

#endif // #if !defined(INCLUDED_WARNETSESSIONEVENTHANDLER)
