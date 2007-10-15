/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// MessengerService.h
#if !defined(INCLUDED_CHANNELMESSENGERSERVICE)
#define INCLUDED_CHANNELMESSENGERSERVICE

#include <skynet/arcade/im/Messenger.h>
#include <boost/utility.hpp>
#include <memory>

namespace skynet
{

class DatabasePool;

namespace im
{

class MessengerEventHandler;

/**
 * @class MessengerService
 *
 * 메신저 서비스
 */
class MessengerService : public boost::noncopyable
{
public:
    virtual ~MessengerService() {}

    // = Events from GameServer

    /// 사용자가 로그인하였다
    virtual void enterUser(UserNo userNo) = 0;

    /// 사용자가 로그아웃하였다
    virtual void leaveUser(UserNo userNo) = 0;

    /// 친구를 등록한다
    virtual void registerBuddy(UserNo owner,
        const std::string& buddyNickname, const std::string& memo = "",
        bool isBadBuddy = false) = 0;

    /// 친구를 등록 해제한다
    virtual void unregisterBuddy(UserNo owner, UserNo buddy) = 0;

    /// 사용자의 위치가 변경되었다
    virtual void changePosition(UserNo user, RoomId position) = 0;

    /// 사용자의 상태가 변경되었다
    virtual void changeState(UserNo user, im::UserState buddyState) = 0;

    /// 메모를 갱신한다
    virtual void updateBuddyMemo(UserNo owner, UserNo buddy,
        const std::string& memo) = 0;

    /// userA와 userB가 서로 나쁜 친구 관계인가?
    virtual bool isBadBuddy(UserNo userA, UserNo userB) = 0;

    /// 접속 중인 사용자 수를 얻는다
    virtual size_t getOnlineUserCount() const = 0;
};

} // namespace im

} // namespace skynet

#endif // !defined(INCLUDED_CHANNELMESSENGERSERVICE)
