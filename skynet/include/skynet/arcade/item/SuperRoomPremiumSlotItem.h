/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_SUPERROOMPREMIUMSLOTITEM)
#define INCLUDED_SUPERROOMPREMIUMSLOTITEM

#include "PeriodLimitedSlotItem.h"
#include <skynet/arcade/ArcadeRewardInfo.h>

namespace skynet
{

/**
 * @class SuperRoomPremiumSlotItem
 * 슈퍼방장(프리미엄) 슬롯 아이템 클래스
 */
class WARNET_API SuperRoomPremiumSlotItem : public PeriodLimitedSlotItem
{
public:
    SuperRoomPremiumSlotItem(const FunctionalItem& funcItem,
        const SlotItemInfo& itemInfo) :
        PeriodLimitedSlotItem(funcItem, itemInfo) {}

    Coin rewardCoin(Coin earnedCoin, time_t aTime) const {
        if (! isExpired(aTime)) {
            return earnedCoin + 1;
        }
        return earnedCoin;
    }

    Rupo rewardRupoForOwner(Rupo earnedRupo, time_t aTime) const {
        if (! isExpired(aTime)) {
            return earnedRupo * 2;
        }
        return earnedRupo;
    }
    Rupo rewardRupo(Rupo earnedRupo, time_t aTime) const {
        if (! isExpired(aTime)) {
            return earnedRupo + (earnedRupo / 2);
        }
        return earnedRupo;
    }

    ExpPoint rewardExpPointForOwner(ExpPoint earnedExpPoint,
        time_t aTime) const {
        if (! isExpired(aTime)) {
            return earnedExpPoint * 2;
        }
        return earnedExpPoint;
    }
    ExpPoint rewardExpPoint(ExpPoint earnedExpPoint,
        time_t aTime) const {
        if (! isExpired(aTime)) {
            return earnedExpPoint + (earnedExpPoint / 2);
        }
        return earnedExpPoint;
    }
private:
    virtual bool isAllowed(GameGenre gameGenre) const {
        return isValidGameGenre(gameGenre) && (GG_COOPERATIVE != gameGenre);
    }
};

} // namespace skynet

#endif // !defined(INCLUDED_SUPERROOMPREMIUMSLOTITEM)
