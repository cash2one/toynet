/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_COUNTLIMITEDSLOTITEM)
#define INCLUDED_COUNTLIMITEDSLOTITEM

#include "SlotItem.h"
#include "../ArcadeItemInfo.h"

namespace skynet
{

/**
 * @class CountLimitedSlotItem
 * 횟수 제한 슬롯 아이템 베이스 클래스
 */
class CountLimitedSlotItem : public SlotItem
{
public:
    CountLimitedSlotItem(const FunctionalItem& funcItem,
        const SlotItemInfo& itemInfo) :
        SlotItem(funcItem, itemInfo) {}
protected:
    virtual bool isExpired(time_t /*aTime*/) const {
        return getSlotItemInfo().leftCount_ <= 0;
    }
private:
    virtual void used() {
        WN_ASSERT(getSlotItemInfo().leftCount_ > 0);
        if (getSlotItemInfo().leftCount_ > 0) {
            getSlotItemInfo().leftCount_--;
        }
    }
};

} // namespace skynet

#endif // !defined(INCLUDED_COUNTLIMITEDSLOTITEM)
