/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// PoporuSessionAdaptorFactory.h
//
#if !defined(INCLUDED_POPORUSESSIONADAPTORFACTORY)
#define INCLUDED_POPORUSESSIONADAPTORFACTORY

#include <skynet/SessionAdaptor.h>

namespace skynet
{

/**
 * @class PoporuSessionAdaptorFactory
 * ������ ���� �ʱ�ȭ ���
 */
struct WARNET_API PoporuSessionAdaptorFactory
{
    /// �α��� ���� ���� ����� ����
    static SessionAdaptor& createLoginServerSessionAdaptor();

    /// ���� ���� ���� ����� ����
    static SessionAdaptor& createGameServerSessionAdaptor();

    /// ���� ���� ���� ����� ����
    static SessionAdaptor& createRelayServerSessionAdaptor();

    ///// ����� ���� ���� ����� ���� 
    //static SessionAdaptor& createMonitorServerSessionAdaptor();
};

} // namespace skynet

#endif // !defined(INCLUDED_POPORUSESSIONADAPTORFACTORY)
