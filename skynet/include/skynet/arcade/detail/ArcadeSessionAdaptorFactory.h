/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
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
 * 포포루 관련 초기화 담당
 */
struct WARNET_API PoporuSessionAdaptorFactory
{
    /// 로그인 서버 관련 어댑터 생성
    static SessionAdaptor& createLoginServerSessionAdaptor();

    /// 게임 서버 관련 어댑터 생성
    static SessionAdaptor& createGameServerSessionAdaptor();

    /// 관전 서버 관련 어댑터 생성
    static SessionAdaptor& createRelayServerSessionAdaptor();

    ///// 모니터 서버 관련 어댑터 생성 
    //static SessionAdaptor& createMonitorServerSessionAdaptor();
};

} // namespace skynet

#endif // !defined(INCLUDED_POPORUSESSIONADAPTORFACTORY)
