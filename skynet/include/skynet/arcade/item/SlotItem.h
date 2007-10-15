/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
 * ���� ������ ���̽� Ŭ����
 */
class WARNET_API SlotItem
{
public:
    SlotItem(const FunctionalItem& funcItem, const SlotItemInfo& itemInfo) :
        funcItem_(funcItem),
        itemInfo_(itemInfo) {}
      
    virtual ~SlotItem() {}

    /// �������� ���Ǿ���
    virtual void used() = 0;

    ItemFuncType getItemFuncType() const {
        return funcItem_.getItemFuncType();
    }

    /// �������� ������ �� �ִ� ���� �帣�ΰ�?
    virtual bool isAllowed(GameGenre gameGenre) const = 0;

    /// ��� �Ⱓ �Ǵ� Ƚ���� ����Ǿ��°�?
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
