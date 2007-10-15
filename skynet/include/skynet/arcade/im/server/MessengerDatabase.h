/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
* Popuru_Messenger ���� �����ͺ��̽�
*/
class MessengerDatabase : public Database
{
public:
    /**
     * ģ�� ����� �о�´�
     * @param buddies ���� ����� ģ�� ���
     * @param registeredBuddies ���� ����� ģ�� ���
     * @param owner ������
     */
    virtual bool getBuddyList(Buddies& buddies, Buddies& registeredBuddies,
        UserNo owner) = 0;

    /// ģ���� ����Ѵ�
    virtual bool registerBuddy(UserNo owner, const Buddy& buddy) = 0;

    /// ģ���� ��������Ѵ�
    virtual bool unregisterBuddy(UserNo owner, UserNo buddy) = 0;

    /// ģ�� �޸� �����Ѵ�
    virtual bool updateBuddyMemo(UserNo owner, const Buddy& buddy) = 0;
};

} // namespace im

} // namespace skynet

#endif // !defined(INCLUDED_MESSENGERDATABASE)
