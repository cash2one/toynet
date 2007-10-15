/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
 * ������ ���� �ʱ�ȭ ���
 */
class WARNET_API SessionAdaptor
{
public:
    /// �� ������ heartbeat ���� ����
    virtual long getHeartbeatInterval() const = 0;
    virtual const Message* getHeartbeatMessage() const = 0;
    virtual void initBuilder(WarNetSession& session) = 0;
    virtual void initMessageHandler(WarNetSession& session) = 0;
};

} // namespace skynet

#endif // !defined(INCLUDED_SESSIONADAPTOR)
