/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
* Popuru_Item ���� �����ͺ��̽�
*/
class ItemDatabase : public Database
{
public:
    /// owner ������� ���� ������ ����� ��´�
    virtual bool getSlotItemList(ItemInfos& unusedItemInfos,
        ItemInfos& attachedItemInfos, UserNo owner) = 0;

    /// ���� �������� �����Ѵ�
    virtual bool attachSlotItem(UserNo owner, CardNumber cardNumber,
        CardSlotIndex csIndex, const SlotItemInfo& slotItemInfo) = 0;

    /// ���� �������� �����Ѵ�
    virtual bool deleteSlotItem(UserNo owner, ItemId itemId) = 0;

    /// ī�忡 ������ ���� �����۵��� ������ ���´�
    virtual bool getCardSlotItemInfos(ItemInfos& itemInfos, UserNo owner,
        CardNumber cardNumber) = 0;

    /// Ƚ�� ���� �������� ���Ƚ���� 1 ���ҽ�Ų��
    virtual bool decreaseSlotItemLeftCount(UserNo owner, ItemId itemId) = 0;
};

} // namespace skynet

#endif // !defined(INCLUDED_ITEMDATABASE)
