/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_RPPROTECTIONSLOTITEM)
#define INCLUDED_RPPROTECTIONSLOTITEM

#include "PeriodLimitedSlotItem.h"
#include <skynet/arcade/ArcadeRewardInfo.h>

namespace skynet
{

/**
 * @class RpProtectionSlotItem
 * RP 보호 슬롯 아이템 클래스
 */
class WARNET_API RpProtectionSlotItem : public PeriodLimitedSlotItem
{
public:
    RpProtectionSlotItem(const FunctionalItem& funcItem,
        const SlotItemInfo& itemInfo) :
        PeriodLimitedSlotItem(funcItem, itemInfo) {}

    /// @param randomValue 100으로 나눈 나머지가 30 미만 경우 보호
    RankingPoint protect(RankingPoint rp, int randomValue) const {
        if (rp >= 0) {
            return rp;
        }
        return ((! isExpired(time(0))) && ((randomValue % 100) < 30)) ?
            (rp / 2) : rp;
    }
private:
    virtual bool isAllowed(GameGenre gameGenre) const {
        return isValidGameGenre(gameGenre);
    }
};

} // namespace skynet

#endif // !defined(INCLUDED_RPPROTECTIONSLOTITEM)
