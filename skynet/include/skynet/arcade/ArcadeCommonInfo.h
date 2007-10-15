/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// PoporuCommonInfo.h
//  - Arcade 공통 정보
//
#if !defined(INCLUDED_POPORUCOMMONINFO)
#define INCLUDED_POPORUCOMMONINFO

#include <skynet/Wdr.h>
#pragma warning ( push )
#pragma warning ( disable: 4702 )
#include <vector>
#pragma warning ( pop )
#include <string>

namespace skynet
{

/// 사용자 일련번호(User_login.user_no)
enum UserNo
{
    INVALID_USERNO = -1,
    TO_ALL = -1, ///< 대기실 또는 게임방 내에서의 모든 사용자를 의미

    GAMESERVER_USERNO = -1, ///< 게임서버가 관전서버에 로그인할 때 쓰임

    DRAWED_MATCH = 0 ///< MatchGameResult에서 비긴 경우에 사용
};

inline bool isValidUserNo(UserNo userNo)
{
    return INVALID_USERNO != userNo;
}

inline bool isGameServerUserNo(UserNo userNo)
{
    return userNo == GAMESERVER_USERNO;
}

inline UserNo toUserNo(int userNo)
{
    return static_cast<UserNo>(userNo);
}

typedef std::vector<UserNo> UserNos;


/// 로그인 쿠키
typedef std::string LoginCookie;


/**
* @struct AddressPair
*
* IP 주소, 포트번호
*/
struct AddressPair
{
    std::string ip_; ///< IP 주소
    Wdr::UShort port_; ///< 포트 번호

    /// ctor
    explicit AddressPair(const std::string& ip = "", Wdr::UShort port = 0) :
        ip_(ip),
        port_(port) {}

    bool isValid() const {
        return (! ip_.empty()) && (port_ > 0);
    }
};

/// IP 주소들
typedef std::vector<AddressPair> AddressPairs;

} // namespace skynet

#endif // !defined(INCLUDED_POPORUCOMMONINFO)
