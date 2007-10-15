/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_PERIODLIMITEDSLOTITEM)
#define INCLUDED_PERIODLIMITEDSLOTITEM

#include "SlotItem.h"
#include "../ArcadeItemInfo.h"

namespace skynet
{

/**
 * @class PeriodLimitedSlotItem
 * 기간 제한 슬롯 아이템 베이스 클래스
 */
class PeriodLimitedSlotItem : public SlotItem
{
public:
    PeriodLimitedSlotItem(const FunctionalItem& funcItem,
        const SlotItemInfo& itemInfo) :
        SlotItem(funcItem, itemInfo) {}

    virtual bool isExpired(time_t aTime) const {
        return (aTime < getSlotItemInfo().fromTime_) ||
            (aTime >= getSlotItemInfo().toTime_);
    }
private:
    virtual void used() {}
};

} // namespace skynet

#endif // !defined(INCLUDED_PERIODLIMITEDSLOTITEM)
