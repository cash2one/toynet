/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
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
* Popuru_Web 전용 데이터베이스
*/
class WebDatabase : public Database
{
public:
    /// DB로 부터 user_no를 얻는다.
    virtual bool getUserNo(UserNo& userNo, const std::string& id,
        const std::string& password) = 0;

    /// 사용자 정보를 얻는다.

	virtual bool getLoginUserInfo(std::string& nickname, Sex& sex, PointStorage& coinBox, UserNo userNo)=0;

	/// 상세한 사용자 정보를 얻는다
    virtual bool getUserInfo(DetailedUserInfo& ui, UserNo userNo)=0;//,CardNumber cardNum) = 0;

    /// 현재 소유 코인수를 갱신한다
    virtual bool updateCurrentCoin(UserNo userNo, CardNumber cardNo,
        Coin currentCoin) = 0;

    /// 캐릭터 카드 정보(경험치, 레벨, 코인)를 갱신한다
    virtual bool updateCardProperty(UserNo userNo,
        const CardProperty& property) = 0;

    /// CoreUserInfo를 얻는다
    virtual bool getCoreUserInfo(CoreUserInfo& sui, UserNo userNo) = 0;

    /// CoreUserInfo를 얻는다
    virtual bool getCoreUserInfo(CoreUserInfo& sui,
        const std::string& nickname) = 0;

    /// 루포를 갱신한다
    virtual bool updateRupo(UserNo player, Rupo increment) = 0;

    /// 코인을 이동한다
    virtual bool moveCoin(CoinStorage& coinBox, CoinStorage& card,
        CardNumber cardNumber, Coin coin, UserNo userNo) = 0;

    /// 캔디를 누적시킨다
   /* virtual bool updateCandy(UserNo userNo, Candy earnedCandy) = 0;*/

    /// 기본 캐릭터 카드를 변경한다
    virtual bool changeDefaultCharacterCard(UserNo userNo,
        CardNumber cardNumber) = 0;
};

} // namespace skynet

#endif // !defined(INCLUDED_WEBDATABASE)
