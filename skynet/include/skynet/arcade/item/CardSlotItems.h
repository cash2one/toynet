/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_CARDSLOTITEMS)
#define INCLUDED_CARDSLOTITEMS

#include "../ArcadeGameInfo.h"
#include "../ArcadeItemInfo.h"
#include "../ArcadeDataLimits.h"
#pragma warning ( push )
#pragma warning ( disable: 4244 4267 4311 4312 )
#include <ace/Thread_Mutex.h>
#pragma warning ( pop )
#pragma warning ( push )
#pragma warning ( disable: 4702 )
#include <map>
#include <vector>
#pragma warning ( pop )

namespace skynet
{

class SlotItem;

/**
 * @class CardSlotItems
 * 캐릭터 카드 슬롯 아이템 보관소
 * - Thread-Safe해야 한다!!
 */
class WARNET_API CardSlotItems
{
    typedef std::vector<SlotItem*> SlotItems;
    typedef std::map<ItemFuncType, SlotItems> SlotItemsMap;
public:
    CardSlotItems(const SlotItemInfos& slotItemInfos,
        size_t maxCardSlot = DL_MAX_CARD_SLOT_COUNT);
    ~CardSlotItems();

    /// 슬롯 아이템을 추가한다
    void addSlotItem(const SlotItemInfo& itemInfo);

    /// 슬롯 아이템을 삭제한다
    void deleteSlotItem(ItemId itemId);

    /**
     * 아이템을 얻는다
     * Exposed State Pattern - from PLoP3 (pp.134)
     */
    SlotItem* getSlotItem(ItemFuncType itemFuncType,
        GameGenre gameGenre = GG_UNKNOWN);

    size_t getSlotItemCount() const;
private:
    const size_t maxCardSlot_;
    SlotItemsMap slotItemsMap_;

    mutable ACE_Thread_Mutex lockThis_;
};

} // namespace skynet

#endif // !defined(INCLUDED_CARDSLOTITEMS)
