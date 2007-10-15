/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ItemDatabase.h
#if !defined(INCLUDED_ITEMDATABASE)
#define INCLUDED_ITEMDATABASE

#include <skynet/server/Database.h>
#include <skynet/arcade/ArcadeItemInfo.h>
#include <skynet/arcade/ArcadeUserInfo.h>

namespace skynet
{

/**
* @class ItemDatabase
*
* Popuru_Item 전용 데이터베이스
*/
class ItemDatabase : public Database
{
public:
    /// owner 사용자의 슬롯 아이템 목록을 얻는다
    virtual bool getSlotItemList(ItemInfos& unusedItemInfos,
        ItemInfos& attachedItemInfos, UserNo owner) = 0;

    /// 슬롯 아이템을 장착한다
    virtual bool attachSlotItem(UserNo owner, CardNumber cardNumber,
        CardSlotIndex csIndex, const SlotItemInfo& slotItemInfo) = 0;

    /// 슬롯 아이템을 삭제한다
    virtual bool deleteSlotItem(UserNo owner, ItemId itemId) = 0;

    /// 카드에 장착된 슬롯 아이템들의 정보를 얻어온다
    virtual bool getCardSlotItemInfos(ItemInfos& itemInfos, UserNo owner,
        CardNumber cardNumber) = 0;

    /// 횟수 제한 아이템의 사용횟수를 1 감소시킨다
    virtual bool decreaseSlotItemLeftCount(UserNo owner, ItemId itemId) = 0;
};

} // namespace skynet

#endif // !defined(INCLUDED_ITEMDATABASE)
