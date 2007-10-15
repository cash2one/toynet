/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_SLOTITEM)
#define INCLUDED_SLOTITEM

#include "FunctionalItem.h"
#include "../ArcadeGameInfo.h"
#include "../ArcadeItemInfo.h"
#include <ctime>

namespace skynet
{

/**
 * @class SlotItem
 * 슬롯 아이템 베이스 클래스
 */
class WARNET_API SlotItem
{
public:
    SlotItem(const FunctionalItem& funcItem, const SlotItemInfo& itemInfo) :
        funcItem_(funcItem),
        itemInfo_(itemInfo) {}
      
    virtual ~SlotItem() {}

    /// 아이템이 사용되었다
    virtual void used() = 0;

    ItemFuncType getItemFuncType() const {
        return funcItem_.getItemFuncType();
    }

    /// 아이템을 적용할 수 있는 게임 장르인가?
    virtual bool isAllowed(GameGenre gameGenre) const = 0;

    /// 사용 기간 또는 횟수가 만료되었는가?
    virtual bool isExpired(time_t aTime = 0) const = 0;

    ItemId getItemId() const {
        return itemInfo_.itemId_;
    }

    ItemCode getItemCode() const {
        return itemInfo_.itemCode_;
    }
protected:
    const SlotItemInfo& getSlotItemInfo() const {
        return itemInfo_;
    }
    SlotItemInfo& getSlotItemInfo() {
        return itemInfo_;
    }
    int getSerialNumber() const {
        return funcItem_.getItemSerialNumber();
    }
private:
    const FunctionalItem funcItem_;
    SlotItemInfo itemInfo_;
};

} // namespace skynet

#endif // !defined(INCLUDED_SLOTITEM)
