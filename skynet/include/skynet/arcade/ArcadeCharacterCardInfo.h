/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ArcadeCharacterCardInfo.h
//  - Arcade 캐릭터 카드 정보
//
#if !defined(INCLUDED_POPORUCHARACTERCARDINFO)
#define INCLUDED_POPORUCHARACTERCARDINFO

#include "ArcadeRewardInfo.h"
#include "ArcadeItemInfo.h"
#include "ArcadeDataLimits.h"
#include <skynet/Wdr.h>
#pragma warning ( push )
#pragma warning ( disable: 4702 )
#include <vector>
#pragma warning ( pop )

namespace skynet
{

/// 캐릭터 카드 레벨
typedef Wdr::Octet CardLevel;
static const CardLevel INVALID_CARD_LEVEL = 0;

inline bool isValidCardLevel(CardLevel cardLevel)
{
    return (1 <= cardLevel) && (cardLevel <= DL_MAX_CARD_LEVEL);
}

/// 캐릭터 카드 번호
typedef Wdr::Octet CardNumber;
static const CardNumber INVALID_CARD_NUMBER = 0;

/**
 * @struct CardProperty
 * 카드(개인)의 기본 속성
 * - 공간 및 시간 효율을 위해 따로 빼냄
 */
struct CardProperty : public CoinStorage
{
    CardNumber number_; ///< 카드 번호
    CardLevel level_; ///< 카드 레벨
    ExpPoint exp_; ///< 경험치
    ExpPoint currentLevelExp_; ///< 현재 레벨의 경험치
    ExpPoint nextLevelExp_; ///< 다음 레벨의 경험치

    CardProperty() :
        number_(INVALID_CARD_NUMBER),
        level_(0),
        exp_(0),
        currentLevelExp_(0),
        nextLevelExp_(0) {}
};	

struct UserProperty : public PointStorage
{
	ExpPoint exp_;
	ExpPoint currentLevelExp_;
	ExpPoint nextLevelExp_;

	UserProperty() :
		exp_(0),
		currentLevelExp_(0),
        nextLevelExp_(0) {}

};

/**
 * @struct CharacterCard
 *
 * 캐릭터 카드 정보 (User_ChracterCard 테이블에서 뽑아온 정보)
 */
struct CharacterCard
{
    CardProperty property_; ///< 카드 기본 속성
	UserProperty prop;

    std::string csd_; ///< Card Skin Data
    std::string ccd_; ///< Card Character Data
    Wdr::Octet slotCount_; ///< 카드 슬롯 갯수
    SlotItemInfos slotItems_; ///< 슬롯 아이템 목록
    bool isDefault_; ///< 디폴트 카드인가?

    /// ctor
    CharacterCard() :
        slotCount_(0),
        isDefault_(false) {}

    bool hasCoin() const {
        return property_.hasCoin();
    }

    void consumeCoin(Coin coin) {
        property_.consumeCoin(coin);
    }

    bool isValid() const {
        return slotCount_ == slotItems_.size();
    }
};

/// 캐릭터 카드 목록
typedef std::vector<CharacterCard> CharacterCards;

inline CharacterCards::iterator find(CharacterCards& cards,
    CardNumber cardNum)
{
    CharacterCards::iterator pos = cards.begin();
    for (; pos != cards.end(); ++pos) {
        if ((*pos).property_.number_ == cardNum)
            break;
    }
    return pos;
}


inline CharacterCards::const_iterator find(const CharacterCards& cards,
    CardNumber cardNum)
{
    CharacterCards::const_iterator pos = cards.begin();
    for (; pos != cards.end(); ++pos) {
        if ((*pos).property_.number_ == cardNum)
            break;
    }
    return pos;
}


} // namespace skynet

#endif // !defined(INCLUDED_POPORUCHARACTERCARDINFO)
