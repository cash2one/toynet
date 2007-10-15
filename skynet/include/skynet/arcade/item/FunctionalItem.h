/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// FunctionalItem.h
//  - Poporu 기능 아이템
//
#if !defined(INCLUDED_FUNCTIONALITEM)
#define INCLUDED_FUNCTIONALITEM

#include "../ArcadeItemInfo.h"

namespace skynet
{

/**
 * @class FunctionalItem
 * ItemCode 분석
 */
class WARNET_API FunctionalItem
{
public:
    FunctionalItem();
    FunctionalItem(const FunctionalItem& aItem) :
        itemType_(aItem.itemType_),
        itemFuncType_(aItem.itemFuncType_),
        itemSerialNumber_(aItem.itemSerialNumber_) {}
    FunctionalItem& operator= (const FunctionalItem& aItem) {
        itemType_ = aItem.itemType_;
        itemFuncType_ = aItem.itemFuncType_;
        itemSerialNumber_ = aItem.itemSerialNumber_;
        return *this;
    }

    bool parse(ItemCode itemCode);

    ItemType getItemType() const {
        return itemType_;
    }

    ItemFuncType getItemFuncType() const {
        return itemFuncType_;
    }

    int getItemSerialNumber() const {
        return itemSerialNumber_;
    }
private:
    void reset();
private:
    ItemType itemType_;
    ItemFuncType itemFuncType_;
    int itemSerialNumber_;
};

} // namespace skynet

#endif // !defined(INCLUDED_FUNCTIONALITEM)
