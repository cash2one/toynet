/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author ±Ë∫¥ºˆ <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_SUPERROOMCOOPERATIVESLOTITEM)
#define INCLUDED_SUPERROOMCOOPERATIVESLOTITEM

#include "PeriodLimitedSlotItem.h"
#include <skynet/arcade/ArcadeRewardInfo.h>

namespace skynet
{

/**
 * @class SuperRoomCooperativeSlotItem
 * Ω¥∆€πÊ¿Â(∏ﬁ≈ªΩ√∏Æ¡Ó) ΩΩ∑‘ æ∆¿Ã≈€ ≈¨∑°Ω∫
 */
class WARNET_API SuperRoomCooperativeSlotItem : public PeriodLimitedSlotItem
{
public:
    SuperRoomCooperativeSlotItem(const FunctionalItem& funcItem,
        const SlotItemInfo& itemInfo) :
        PeriodLimitedSlotItem(funcItem, itemInfo) {}

    Coin rewardCoin(Coin earnedCoin, GameType gameType, size_t score,
        time_t aTime) const {
        WN_ASSERT(isCooperativeGame(gameType));
        if (! isExpired(aTime)) {
            if (isMetalSlugSeries(gameType)) {
                return static_cast<Coin>(earnedCoin + (score / 500000));
            }
            return static_cast<Coin>(earnedCoin + (score / 50000));
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
    ExpPoint rewardExpPoint(ExpPoint earnedExpPoint, time_t aTime) const {
        if (! isExpired(aTime)) {
            return earnedExpPoint + (earnedExpPoint / 2);
        }
        return earnedExpPoint;
    }
private:
    virtual bool isAllowed(GameGenre gameGenre) const {
        return GG_COOPERATIVE == gameGenre;
    }
};

} // namespace skynet

#endif // !defined(INCLUDED_SUPERROOMCOOPERATIVESLOTITEM)
