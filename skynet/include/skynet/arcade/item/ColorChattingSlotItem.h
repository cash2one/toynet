/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_COLORCHATTINGSLOTITEM)
#define INCLUDED_COLORCHATTINGSLOTITEM

#include "PeriodLimitedSlotItem.h"

namespace skynet
{

/**
 * @class ColorChattingSlotItem
 * �÷� ä�� ���� ������ Ŭ����
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
