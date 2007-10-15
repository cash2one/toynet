/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// WebDatabase.h
#if !defined(INCLUDED_WEBDATABASE)
#define INCLUDED_WEBDATABASE

#include <skynet/server/Database.h>
#include <skynet/arcade/ArcadeUserInfo.h>

namespace skynet
{

/**
* @class WebDatabase
*
* Popuru_Web ���� �����ͺ��̽�
*/
class WebDatabase : public Database
{
public:
    /// DB�� ���� user_no�� ��´�.
    virtual bool getUserNo(UserNo& userNo, const std::string& id,
        const std::string& password) = 0;

    /// ����� ������ ��´�.

	virtual bool getLoginUserInfo(std::string& nickname, Sex& sex, PointStorage& coinBox, UserNo userNo)=0;

	/// ���� ����� ������ ��´�
    virtual bool getUserInfo(DetailedUserInfo& ui, UserNo userNo)=0;//,CardNumber cardNum) = 0;

    /// ���� ���� ���μ��� �����Ѵ�
    virtual bool updateCurrentCoin(UserNo userNo, CardNumber cardNo,
        Coin currentCoin) = 0;

    /// ĳ���� ī�� ����(����ġ, ����, ����)�� �����Ѵ�
    virtual bool updateCardProperty(UserNo userNo,
        const CardProperty& property) = 0;

    /// CoreUserInfo�� ��´�
    virtual bool getCoreUserInfo(CoreUserInfo& sui, UserNo userNo) = 0;

    /// CoreUserInfo�� ��´�
    virtual bool getCoreUserInfo(CoreUserInfo& sui,
        const std::string& nickname) = 0;

    /// ������ �����Ѵ�
    virtual bool updateRupo(UserNo player, Rupo increment) = 0;

    /// ������ �̵��Ѵ�
    virtual bool moveCoin(CoinStorage& coinBox, CoinStorage& card,
        CardNumber cardNumber, Coin coin, UserNo userNo) = 0;

    /// ĵ�� ������Ų��
   /* virtual bool updateCandy(UserNo userNo, Candy earnedCandy) = 0;*/

    /// �⺻ ĳ���� ī�带 �����Ѵ�
    virtual bool changeDefaultCharacterCard(UserNo userNo,
        CardNumber cardNumber) = 0;
};

} // namespace skynet

#endif // !defined(INCLUDED_WEBDATABASE)
