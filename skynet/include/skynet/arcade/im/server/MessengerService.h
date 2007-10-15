/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
 * �޽��� ����
 */
class MessengerService : public boost::noncopyable
{
public:
    virtual ~MessengerService() {}

    // = Events from GameServer

    /// ����ڰ� �α����Ͽ���
    virtual void enterUser(UserNo userNo) = 0;

    /// ����ڰ� �α׾ƿ��Ͽ���
    virtual void leaveUser(UserNo userNo) = 0;

    /// ģ���� ����Ѵ�
    virtual void registerBuddy(UserNo owner,
        const std::string& buddyNickname, const std::string& memo = "",
        bool isBadBuddy = false) = 0;

    /// ģ���� ��� �����Ѵ�
    virtual void unregisterBuddy(UserNo owner, UserNo buddy) = 0;

    /// ������� ��ġ�� ����Ǿ���
    virtual void changePosition(UserNo user, RoomId position) = 0;

    /// ������� ���°� ����Ǿ���
    virtual void changeState(UserNo user, im::UserState buddyState) = 0;

    /// �޸� �����Ѵ�
    virtual void updateBuddyMemo(UserNo owner, UserNo buddy,
        const std::string& memo) = 0;

    /// userA�� userB�� ���� ���� ģ�� �����ΰ�?
    virtual bool isBadBuddy(UserNo userA, UserNo userB) = 0;

    /// ���� ���� ����� ���� ��´�
    virtual size_t getOnlineUserCount() const = 0;
};

} // namespace im

} // namespace skynet

#endif // !defined(INCLUDED_CHANNELMESSENGERSERVICE)
