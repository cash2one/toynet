/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
 * �޽��� �̺�Ʈ �ڵ鷯
 */
class MessengerEventHandler : public boost::noncopyable
{
public:
    virtual ~MessengerEventHandler() {}

    /// ��� ������ �ִ� ģ�� ���� ���Ѵ�
    virtual size_t getMaxBuddyCount() = 0;

    /// ģ�� ����� �����Ѵ�
    virtual void onBuddyList(UserNo owner, const Buddies& buddies,
        const Buddies& registeredBuddies) = 0;

    /// ģ���� ��ϵǾ���
    virtual void onRegisterBuddy(UserNo owner, const Buddy& buddy,
        ErrorCode ec) = 0;

    /// �ٸ� ����ڰ� ���� ����Ͽ���
    virtual void onBuddyRegisterted(UserNo me, const Buddy& buddy) = 0;

    /// ģ���� ��������Ͽ���
    virtual void onUnregisterBuddy(UserNo owner, UserNo buddy) = 0;

    /// �ٸ� ����ڰ� ���� ��������Ͽ���
    virtual void onBuddyUnregistered(UserNo me, UserNo registeredBuddy) = 0;

    /// (����) ģ��(owner)�� �α����Ͽ����� buddies���� �˸���
    virtual void onBuddyOnline(const UserNos& buddies, UserNo owner) = 0;

    /// (����) ģ��(owner)�� �α׾ƿ��Ͽ����� buddies���� �˸���
    virtual void onBuddyOffline(const UserNos& buddies, UserNo owner) = 0;

    /// ��(buddy of buddies)�� ����� ģ���� �α����Ͽ����� �˸���
    virtual void onBuddyWhoRegisterOnline(const UserNos& buddies,
        UserNo owner) = 0;

    /// ��(buddy of buddies)�� ����� ģ���� �α׾ƿ��Ͽ����� �˸���
    virtual void onBuddyWhoRegisterOffline(const UserNos& buddies,
        UserNo owner) = 0;

    /// who�� ��ġ(position)�� �ٲ������ ģ��(buddies)���� �˸���
    virtual void onPositionChanged(const UserNos& buddies, UserNo who,
        RoomId position) = 0;

    /// who�� ����(state)�� �ٲ������ ģ��(buddies)���� �˸���
    virtual void onStateChanged(const UserNos& buddies, UserNo who,
        im::UserState state) = 0;

    /// �޸� ���� ��û�� ���� ���
    virtual void onUpdateMemo(UserNo owner, UserNo buddy,
        const std::string& memo) = 0;
};

} // namespace im

} // namespace skynet

#endif // !defined(INCLUDED_CHANNELMESSENGEREVENTHANDLER)
