/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
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
    mdlMaxMemo = 100, ///< 최대 친구 메모 바이트 수
};


/// 친구 상태
enum UserState
{
    US_UNKNOWN = 0,

    US_WAITING, ///< 대기 중
    US_PLAYING, ///< 게임 중
    US_LOGIN, ///<로그 인
    US_LOGOUT, ///<로그 아웃
    US_THIS_CHLOGOUT ,//지금 이체널에는 없다.
    US_BOUNDARY
};

inline bool isValidUserState(UserState state)
{
    return (US_UNKNOWN <= state) && (state < US_BOUNDARY);
}


/**
 * @struct Buddy
 * 친구 정보
 */
struct Buddy
{
    UserNo userNo_;
    std::string nickname_;
    Sex sex_;
    std::string memo_;
    bool isBadBuddy_; ///< 나쁜 친구?
    RoomId position_; ///< 현재 위치. 둘 다 등록한 친구인 경우만 의미 있음.
    UserState state_; ///< 현재 상태. 둘 다 등록한 친구인 경우만 의미 있음.
    bool isOnline_; ///< 현재 접속 중인가?

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

/// buddy가 buddies에 존재하는가?
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
