/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// SessionAdaptor.h
//
#if !defined(INCLUDED_SESSIONADAPTOR)
#define INCLUDED_SESSIONADAPTOR

namespace skynet
{

class Message;
class WarNetSession;

/**
 * @class SessionAdaptor
 * 포포루 관련 초기화 담당
 */
class WARNET_API SessionAdaptor
{
public:
    /// 초 단위의 heartbeat 전송 간격
    virtual long getHeartbeatInterval() const = 0;
    virtual const Message* getHeartbeatMessage() const = 0;
    virtual void initBuilder(WarNetSession& session) = 0;
    virtual void initMessageHandler(WarNetSession& session) = 0;
};

} // namespace skynet

#endif // !defined(INCLUDED_SESSIONADAPTOR)
