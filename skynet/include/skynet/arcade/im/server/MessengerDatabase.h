/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// MessengerDatabase.h
#if !defined(INCLUDED_MESSENGERDATABASE)
#define INCLUDED_MESSENGERDATABASE

#include <skynet/arcade/im/Messenger.h>
#include <skynet/server/Database.h>

namespace skynet
{

namespace im
{

/**
* @class MessengerDatabase
*
* Popuru_Messenger 전용 데이터베이스
*/
class MessengerDatabase : public Database
{
public:
    /**
     * 친구 목록을 읽어온다
     * @param buddies 내가 등록한 친구 목록
     * @param registeredBuddies 나를 등록한 친구 목록
     * @param owner 소유자
     */
    virtual bool getBuddyList(Buddies& buddies, Buddies& registeredBuddies,
        UserNo owner) = 0;

    /// 친구를 등록한다
    virtual bool registerBuddy(UserNo owner, const Buddy& buddy) = 0;

    /// 친구를 등록해제한다
    virtual bool unregisterBuddy(UserNo owner, UserNo buddy) = 0;

    /// 친구 메모를 수정한다
    virtual bool updateBuddyMemo(UserNo owner, const Buddy& buddy) = 0;
};

} // namespace im

} // namespace skynet

#endif // !defined(INCLUDED_MESSENGERDATABASE)
