/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_MEGAPHONESLOTITEM)
#define INCLUDED_MEGAPHONESLOTITEM

#include "CountLimitedSlotItem.h"

namespace skynet
{

/**
 * @class MegaphoneSlotItem
 * 확성기 슬롯 아이템 클래스
 */
class WARNET_API MegaphoneSlotItem : public CountLimitedSlotItem
{
public:
    MegaphoneSlotItem(const FunctionalItem& funcItem,
        const SlotItemInfo& itemInfo) :
        CountLimitedSlotItem(funcItem, itemInfo) {}
private:
    virtual bool isAllowed(GameGenre gameGenre) const {
        return isValidGameGenre(gameGenre);
    }
};

} // namespace skynet

#endif // !defined(INCLUDED_MEGAPHONESLOTITEM)
