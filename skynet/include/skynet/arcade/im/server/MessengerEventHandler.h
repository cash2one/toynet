/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_CHANNELMESSENGEREVENTHANDLER)
#define INCLUDED_CHANNELMESSENGEREVENTHANDLER

#include <skynet/arcade/im/Messenger.h>
#include <skynet/ErrorCode.h>
#include <boost/utility.hpp>

namespace skynet
{

namespace im
{

/**
 * @class MessengerEventHandler
 *
 * 메신저 이벤트 핸들러
 */
class MessengerEventHandler : public boost::noncopyable
{
public:
    virtual ~MessengerEventHandler() {}

    /// 등록 가능한 최대 친구 수를 구한다
    virtual size_t getMaxBuddyCount() = 0;

    /// 친구 목록을 전송한다
    virtual void onBuddyList(UserNo owner, const Buddies& buddies,
        const Buddies& registeredBuddies) = 0;

    /// 친구가 등록되었다
    virtual void onRegisterBuddy(UserNo owner, const Buddy& buddy,
        ErrorCode ec) = 0;

    /// 다른 사용자가 나를 등록하였다
    virtual void onBuddyRegisterted(UserNo me, const Buddy& buddy) = 0;

    /// 친구를 등록해제하였다
    virtual void onUnregisterBuddy(UserNo owner, UserNo buddy) = 0;

    /// 다른 사용자가 나를 등록해제하였다
    virtual void onBuddyUnregistered(UserNo me, UserNo registeredBuddy) = 0;

    /// (나쁜) 친구(owner)가 로그인하였음을 buddies에게 알린다
    virtual void onBuddyOnline(const UserNos& buddies, UserNo owner) = 0;

    /// (나쁜) 친구(owner)가 로그아웃하였음을 buddies에게 알린다
    virtual void onBuddyOffline(const UserNos& buddies, UserNo owner) = 0;

    /// 나(buddy of buddies)를 등록한 친구가 로그인하였음을 알린다
    virtual void onBuddyWhoRegisterOnline(const UserNos& buddies,
        UserNo owner) = 0;

    /// 나(buddy of buddies)를 등록한 친구가 로그아웃하였음을 알린다
    virtual void onBuddyWhoRegisterOffline(const UserNos& buddies,
        UserNo owner) = 0;

    /// who의 위치(position)가 바뀌었음을 친구(buddies)에게 알린다
    virtual void onPositionChanged(const UserNos& buddies, UserNo who,
        RoomId position) = 0;

    /// who의 상태(state)가 바뀌었음을 친구(buddies)에게 알린다
    virtual void onStateChanged(const UserNos& buddies, UserNo who,
        im::UserState state) = 0;

    /// 메모 수정 요청에 대한 결과
    virtual void onUpdateMemo(UserNo owner, UserNo buddy,
        const std::string& memo) = 0;
};

} // namespace im

} // namespace skynet

#endif // !defined(INCLUDED_CHANNELMESSENGEREVENTHANDLER)
