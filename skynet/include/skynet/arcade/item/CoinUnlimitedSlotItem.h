/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_COINUNLIMITEDSLOTITEM)
#define INCLUDED_COINUNLIMITEDSLOTITEM

#include "PeriodLimitedSlotItem.h"
#include <skynet/arcade/ArcadeRewardInfo.h>

namespace skynet
{

/**
 * @class CoinUnlimitedSlotItem
 * 코인 무제한 슬롯 아이템 클래스
 */
class WARNET_API CoinUnlimitedSlotItem : public PeriodLimitedSlotItem
{
public:
    CoinUnlimitedSlotItem(const FunctionalItem& funcItem,
        const SlotItemInfo& itemInfo) :
        PeriodLimitedSlotItem(funcItem, itemInfo) {}

    Coin consume(Coin coin) const {
        return (! isExpired(time(0))) ? 0 : coin;
    }
private:
    virtual bool isAllowed(GameGenre gameGenre) const {
        switch (gameGenre) {
        case GG_KOF_SERIES:
            return (1 == getSerialNumber()) || (2 == getSerialNumber()) ||
                (9 == getSerialNumber());
        case GG_SAMURAI_SERIES:
            return (3 == getSerialNumber()) || (4 == getSerialNumber()) ||
                (10 == getSerialNumber());
        //case GG_COOPERATIVE:
        case GG_SPORTS:
            return (5 == getSerialNumber()) || (6 == getSerialNumber()) ||
                (11 == getSerialNumber());
        case GG_ACTION_PUZZLE:
            return (7 == getSerialNumber()) || (8 == getSerialNumber()) ||
                (12 == getSerialNumber());
        }
        return false;
    }
};

} // namespace skynet

#endif // !defined(INCLUDED_COINUNLIMITEDSLOTITEM)
