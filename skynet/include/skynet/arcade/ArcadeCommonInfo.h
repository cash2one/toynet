/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// PoporuCommonInfo.h
//  - Arcade ���� ����
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

/// ����� �Ϸù�ȣ(User_login.user_no)
enum UserNo
{
    INVALID_USERNO = -1,
    TO_ALL = -1, ///< ���� �Ǵ� ���ӹ� �������� ��� ����ڸ� �ǹ�

    GAMESERVER_USERNO = -1, ///< ���Ӽ����� ���������� �α����� �� ����

    DRAWED_MATCH = 0 ///< MatchGameResult���� ��� ��쿡 ���
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


/// �α��� ��Ű
typedef std::string LoginCookie;


/**
* @struct AddressPair
*
* IP �ּ�, ��Ʈ��ȣ
*/
struct AddressPair
{
    std::string ip_; ///< IP �ּ�
    Wdr::UShort port_; ///< ��Ʈ ��ȣ

    /// ctor
    explicit AddressPair(const std::string& ip = "", Wdr::UShort port = 0) :
        ip_(ip),
        port_(port) {}

    bool isValid() const {
        return (! ip_.empty()) && (port_ > 0);
    }
};

/// IP �ּҵ�
typedef std::vector<AddressPair> AddressPairs;

} // namespace skynet

#endif // !defined(INCLUDED_POPORUCOMMONINFO)
