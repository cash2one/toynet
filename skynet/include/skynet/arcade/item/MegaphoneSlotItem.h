/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_MEGAPHONESLOTITEM)
#define INCLUDED_MEGAPHONESLOTITEM

#include "CountLimitedSlotItem.h"

namespace skynet
{

/**
 * @class MegaphoneSlotItem
 * Ȯ���� ���� ������ Ŭ����
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
