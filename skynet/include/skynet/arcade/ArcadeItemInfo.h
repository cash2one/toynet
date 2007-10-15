/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ArcadeItemInfo.h
//  - Arcade ������ Ÿ�� ����
//
#if !defined(INCLUDED_POPORUITEMINFO)
#define INCLUDED_POPORUITEMINFO

#include "ArcadeRoomInfo.h"
#include "ArcadeDataLimits.h"
#include <skynet/Wdr.h>
#pragma warning ( push )
#pragma warning ( disable: 4702 )
#include <vector>
#pragma warning ( pop )

namespace skynet
{

/// ������ (�з�) �ڵ� ��ȣ (0-0000-0000)
enum ItemCode
{
    INVALID_ITEMCODE = 0
};

inline bool isValidItemCode(ItemCode itemCode)
{
    return itemCode != INVALID_ITEMCODE;
    //return (110001100 <= itemCode) && (itemCode <= 259992599);
}

static const int itemCodeSize = 7;

template <typename T>
ItemCode toItemCode(T value)
{
    return static_cast<ItemCode>(value);
}


/// ������ �ߺ� ���� �ڵ�
typedef Wdr::Short ItemOverlapCode;


/// ������ �ĺ���
enum ItemId
{
    INVALID_ITEMID = 0
};

inline bool isValidItemId(ItemId itemId)
{
    return itemId != INVALID_ITEMID;
}

template <typename T>
ItemId toItemId(T value)
{
    return static_cast<ItemId>(value);
}


/// ������ �� �з�
enum ItemType
{
    itUnknown = 0, ///< ���� �����

    itPower = 1, ///< �Ŀ� ������
    itSlot = 2, ///< ���� ������
};


/// ������ ��� �з�
enum ItemFuncType
{
    iftUnknown = 0, ///< ���� �����

    iftCoinUnlimited = 101, ///< ���� ������
    iftDoubleRupo = 301, ///< ���� ����
    iftDoubleExpPoint = 302, ///< ���� ����ġ
    iftRpProtection = 303, ///< RP 50% ��ȣ
    iftMegaphone = 401, ///< Ȯ����
    iftColorChatting = 402, ///< �÷� ä��
    iftChannelTicket = 501, ///< (�ʺ�, �߼�) ä�� �̿��
    iftSuperRoomPremium = 502, ///< ���۹���(�����̾�)
    iftSuperRoomCooperative = 504, ///< ���۹���(��Ż�ø���/����)

    iftBoundary
};

inline bool isValidItemFuncType(ItemFuncType ift)
{
    return (iftUnknown < ift) && (ift < iftBoundary);
}

inline ItemFuncType toItemFuncType(RoomType roomType)
{
    switch (roomType) {
    case RT_SUPERROOMPREMIUM:
        return iftSuperRoomPremium;
    case RT_SUPERROOMCOOPERATIVE:
        return iftSuperRoomCooperative;
    }
    return iftUnknown;
}


/// ī�� ���� �ε��� (0���� ����)
typedef Wdr::Octet CardSlotIndex;

static const CardSlotIndex INVALID_CARDSLOTINDEX = 255; // -1
static const CardSlotIndex firstCardSlotIndex = 0;

inline bool isValidCardSlotIndex(CardSlotIndex index)
{
    return (firstCardSlotIndex <= index) && (index < DL_MAX_CARD_SLOT_COUNT);
}


/**
 * @struct BasicItemInfo
 * �⺻ ������ ����
 */
struct BasicItemInfo
{
    ItemId itemId_;
    ItemCode itemCode_;

    explicit BasicItemInfo(ItemId itemId = INVALID_ITEMID,
        ItemCode itemCode = INVALID_ITEMCODE) :
        itemId_(itemId),
        itemCode_(itemCode) {}
};


/**
 * @struct ItemInfo
 * ������ ������ ����
 */
struct ItemInfo : public BasicItemInfo
{
    Wdr::Short limitHour_; ///< �ð� ����(0�̸� ���� ����)
    Wdr::Octet limitCount_; ///< ��� Ƚ�� ����(0�̸� ���� ����)
    ItemOverlapCode overlapCode_; ///< ������ �ߺ� �ڵ�

    explicit ItemInfo(ItemId itemId = INVALID_ITEMID,
        ItemCode itemCode = INVALID_ITEMCODE,
        ItemOverlapCode overlapCode = 0,
        Wdr::Short limitHour = 0, Wdr::Octet limitCount = 0) :
        BasicItemInfo(itemId, itemCode),
        overlapCode_(overlapCode),
        limitHour_(limitHour),
        limitCount_(limitCount) {}
};


/// ������ ������ ���
typedef std::vector<ItemInfo> ItemInfos;

inline ItemInfos::const_iterator find(const ItemInfos& infos, ItemId itemId)
{
    ItemInfos::const_iterator pos = infos.begin();
    for (; pos != infos.end(); ++pos) {
        if (itemId == (*pos).itemId_)
            break;
    }
    return pos;
}

inline ItemInfos::iterator find(ItemInfos& infos, ItemId itemId)
{
    ItemInfos::iterator pos = infos.begin();
    for (; pos != infos.end(); ++pos) {
        if (itemId == (*pos).itemId_)
            break;
    }
    return pos;
}


/**
 * @struct SlotItemInfo
 * ���� ������ ����
 */
struct SlotItemInfo : public BasicItemInfo
{
    CardSlotIndex slotIndex_;
    time_t fromTime_;
    time_t toTime_;
    Wdr::Short leftCount_;

    explicit SlotItemInfo(CardSlotIndex slotIndex = INVALID_CARDSLOTINDEX,
        ItemId itemId = INVALID_ITEMID, ItemCode itemCode = INVALID_ITEMCODE,
        time_t fromTime = 0, time_t toTime = 0, Wdr::Short leftCount = 0) :
        BasicItemInfo(itemId, itemCode),
        slotIndex_(slotIndex),
        fromTime_(fromTime),
        toTime_(toTime),
        leftCount_(leftCount) {}

    /// ������ ��� �Ⱓ(Ƚ��)�� ����Ǿ��°�?
    bool isExpired(time_t currentTime) const {
        return (currentTime < toTime_) || (leftCount_ > 0);
    }

    /// �Ⱓ ���� ������?
    bool isPeriodLimited() const {
        return toTime_ > fromTime_;
    }

    /// ��� Ƚ�� ���� ������?
    bool isUseCountLimited() const {
        return ! isPeriodLimited();
    }
};

/// ���� ������ ���
typedef std::vector<SlotItemInfo> SlotItemInfos;

inline SlotItemInfos::const_iterator find(const SlotItemInfos& infos,
    CardSlotIndex index)
{
    SlotItemInfos::const_iterator pos = infos.begin();
    for (; pos != infos.end(); ++pos) {
        if ((*pos).slotIndex_ == index)
            break;
    }
    return pos;
}

inline SlotItemInfos::iterator find(SlotItemInfos& infos,
    CardSlotIndex index)
{
    SlotItemInfos::iterator pos = infos.begin();
    for (; pos != infos.end(); ++pos) {
        if ((*pos).slotIndex_ == index)
            break;
    }
    return pos;
}

} // namespace skynet

#endif // !defined(INCLUDED_POPORUITEMINFO)
