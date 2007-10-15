/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_COUNTLIMITEDSLOTITEM)
#define INCLUDED_COUNTLIMITEDSLOTITEM

#include "SlotItem.h"
#include "../ArcadeItemInfo.h"

namespace skynet
{

/**
 * @class CountLimitedSlotItem
 * Ƚ�� ���� ���� ������ ���̽� Ŭ����
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
