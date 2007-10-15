/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_DOUBLEEXPPOINTSLOTITEM)
#define INCLUDED_DOUBLEEXPPOINTSLOTITEM

#include "PeriodLimitedSlotItem.h"
#include <skynet/arcade/ArcadeRewardInfo.h>

namespace skynet
{

/**
 * @class DoubleExpPointSlotItem
 * 더블 경험치 슬롯 아이템 클래스
 */
class WARNET_API DoubleExpPointSlotItem : public PeriodLimitedSlotItem
{
public:
    DoubleExpPointSlotItem(const FunctionalItem& funcItem,
        const SlotItemInfo& itemInfo) :
        PeriodLimitedSlotItem(funcItem, itemInfo) {}

    ExpPoint reward(ExpPoint expPoint, time_t aTime) const {
        return (! isExpired(aTime)) ? expPoint * 2 : expPoint;
    }
private:
    virtual bool isAllowed(GameGenre gameGenre) const {
        return isValidGameGenre(gameGenre);
    }
};

} // namespace skynet

#endif // !defined(INCLUDED_DOUBLEEXPPOINTSLOTITEM)
