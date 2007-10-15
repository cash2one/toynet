/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_COLORCHATTINGSLOTITEM)
#define INCLUDED_COLORCHATTINGSLOTITEM

#include "PeriodLimitedSlotItem.h"

namespace skynet
{

/**
 * @class ColorChattingSlotItem
 * 컬러 채팅 슬롯 아이템 클래스
 */
class WARNET_API ColorChattingSlotItem : public PeriodLimitedSlotItem
{
public:
    ColorChattingSlotItem(const FunctionalItem& funcItem,
        const SlotItemInfo& itemInfo) :
        PeriodLimitedSlotItem(funcItem, itemInfo) {}
private:
    virtual bool isAllowed(GameGenre gameGenre) const {
        return isValidGameGenre(gameGenre);
    }
};

} // namespace skynet

#endif // !defined(INCLUDED_COLORCHATTINGSLOTITEM)
