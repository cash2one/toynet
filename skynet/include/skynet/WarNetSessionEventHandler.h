/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
* �������� ���ǿ��� ����/���� �̺�Ʈ�� ó���Ѵ�
*/
class WARNET_API WarNetSessionEventHandler
{
public:
    virtual ~WarNetSessionEventHandler() {}

private:
    friend class SkyNetClient;

    /// �������� ������ �����Ǿ���.
    virtual void onDisconnected() = 0;

    /// �޼��� �Ľ� ������ �߻��Ͽ���
    virtual void onMessageParsingError(MessageType messageType) = 0;
};

} // namespace skynet

#endif // #if !defined(INCLUDED_WARNETSESSIONEVENTHANDLER)
