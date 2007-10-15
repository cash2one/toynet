/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author ±Ë∫¥ºˆ <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_SLOTITEMFACTORY)
#define INCLUDED_SLOTITEMFACTORY

#include <skynet/arcade/ArcadeItemInfo.h>

namespace skynet
{

class SlotItem;

/**
 * @class SlotItemFactory
 * ΩΩ∑‘ æ∆¿Ã≈€ ∆—≈‰∏Æ ≈¨∑°Ω∫
 */
struct WARNET_API SlotItemFactory
{
    static std::auto_ptr<SlotItem> create(const SlotItemInfo& itemInfo);
};

} // namespace skynet

#endif // !defined(INCLUDED_SLOTITEMFACTORY)
