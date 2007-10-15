/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// Messenger.h
#if !defined(INCLUDED_MESSENGER)
#define INCLUDED_MESSENGER

#include "../ArcadeUserInfo.h"
#include <string>
#include <vector>

namespace skynet
{

namespace im
{

enum MessengerDataLimits
{
    mdlMaxMemo = 100, ///< �ִ� ģ�� �޸� ����Ʈ ��
};


/// ģ�� ����
enum UserState
{
    US_UNKNOWN = 0,

    US_WAITING, ///< ��� ��
    US_PLAYING, ///< ���� ��
    US_LOGIN, ///<�α� ��
    US_LOGOUT, ///<�α� �ƿ�
    US_THIS_CHLOGOUT ,//���� ��ü�ο��� ����.
    US_BOUNDARY
};

inline bool isValidUserState(UserState state)
{
    return (US_UNKNOWN <= state) && (state < US_BOUNDARY);
}


/**
 * @struct Buddy
 * ģ�� ����
 */
struct Buddy
{
    UserNo userNo_;
    std::string nickname_;
    Sex sex_;
    std::string memo_;
    bool isBadBuddy_; ///< ���� ģ��?
    RoomId position_; ///< ���� ��ġ. �� �� ����� ģ���� ��츸 �ǹ� ����.
    UserState state_; ///< ���� ����. �� �� ����� ģ���� ��츸 �ǹ� ����.
    bool isOnline_; ///< ���� ���� ���ΰ�?

    explicit Buddy(UserNo userNo = INVALID_USERNO,
            const std::string& nickname = "", Sex sex = SEX_UNKNOWN,
            const std::string memo = "",
            bool isBadBuddy = false,
            RoomId position = INVALID_ROOMID, UserState state = US_UNKNOWN,
            bool isOnline = false) :
        userNo_(userNo),
        nickname_(nickname),
        sex_(sex),
        memo_(memo),
        isBadBuddy_(isBadBuddy),
        position_(position),
        state_(state),
        isOnline_(isOnline) {}

    bool isValid() const {
        return isValidUserNo(userNo_);
    }
};

typedef std::vector<Buddy> Buddies;

/// buddy�� buddies�� �����ϴ°�?
inline bool isExist(const Buddies& buddies, UserNo buddy)
{
    for (Buddies::const_iterator pos = buddies.begin();
            pos != buddies.end(); ++pos) {
        if ((*pos).userNo_ == buddy) {
            return true;
        }
    }
    return false;
}


inline Buddies::iterator findBuddy(Buddies& buddies, UserNo buddy)
{
    for (Buddies::iterator pos = buddies.begin();
            pos != buddies.end(); ++pos) {
        if ((*pos).userNo_ == buddy) {
            return pos;
        }
    }
    return buddies.end();
}


inline Buddies::const_iterator findBuddy(const Buddies& buddies, UserNo buddy)
{
    for (Buddies::const_iterator pos = buddies.begin();
            pos != buddies.end(); ++pos) {
        if ((*pos).userNo_ == buddy) {
            return pos;
        }
    }
    return buddies.end();
}

} // namespace im

} // namespace skynet

#endif // !defined(INCLUDED_MESSENGER)
