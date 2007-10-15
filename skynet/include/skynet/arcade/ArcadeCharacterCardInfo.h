/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ArcadeCharacterCardInfo.h
//  - Arcade ĳ���� ī�� ����
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

/// ĳ���� ī�� ����
typedef Wdr::Octet CardLevel;
static const CardLevel INVALID_CARD_LEVEL = 0;

inline bool isValidCardLevel(CardLevel cardLevel)
{
    return (1 <= cardLevel) && (cardLevel <= DL_MAX_CARD_LEVEL);
}

/// ĳ���� ī�� ��ȣ
typedef Wdr::Octet CardNumber;
static const CardNumber INVALID_CARD_NUMBER = 0;

/**
 * @struct CardProperty
 * ī��(����)�� �⺻ �Ӽ�
 * - ���� �� �ð� ȿ���� ���� ���� ����
 */
struct CardProperty : public CoinStorage
{
    CardNumber number_; ///< ī�� ��ȣ
    CardLevel level_; ///< ī�� ����
    ExpPoint exp_; ///< ����ġ
    ExpPoint currentLevelExp_; ///< ���� ������ ����ġ
    ExpPoint nextLevelExp_; ///< ���� ������ ����ġ

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
 * ĳ���� ī�� ���� (User_ChracterCard ���̺��� �̾ƿ� ����)
 */
struct CharacterCard
{
    CardProperty property_; ///< ī�� �⺻ �Ӽ�
	UserProperty prop;

    std::string csd_; ///< Card Skin Data
    std::string ccd_; ///< Card Character Data
    Wdr::Octet slotCount_; ///< ī�� ���� ����
    SlotItemInfos slotItems_; ///< ���� ������ ���
    bool isDefault_; ///< ����Ʈ ī���ΰ�?

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

/// ĳ���� ī�� ���
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
