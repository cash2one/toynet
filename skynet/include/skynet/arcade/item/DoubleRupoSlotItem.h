/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_DOUBLERUPOSLOTITEM)
#define INCLUDED_DOUBLERUPOSLOTITEM

#include "PeriodLimitedSlotItem.h"
#include <skynet/arcade/ArcadeRewardInfo.h>

namespace skynet
{

/**
 * @class DoubleRupoSlotItem
 * ���� ���� ���� ������ Ŭ����
 */
class WARNET_API DoubleRupoSlotItem : public PeriodLimitedSlotItem
{
public:
    DoubleRupoSlotItem(const FunctionalItem& funcItem,
        const SlotItemInfo& itemInfo) :
        PeriodLimitedSlotItem(funcItem, itemInfo) {}

    Rupo reward(Rupo rupo, time_t aTime) const {
        return (! isExpired(aTime)) ? rupo * 2 : rupo;
    }
private:
    virtual bool isAllowed(GameGenre gameGenre) const {
        return isValidGameGenre(gameGenre);
    }
};

} // namespace skynet

#endif // !defined(INCLUDED_DOUBLERUPOSLOTITEM)
