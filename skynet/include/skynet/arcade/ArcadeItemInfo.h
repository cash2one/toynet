/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ArcadeItemInfo.h
//  - Arcade 아이템 타입 선언
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

/// 아이템 (분류) 코드 번호 (0-0000-0000)
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


/// 아이템 중복 검출 코드
typedef Wdr::Short ItemOverlapCode;


/// 아이템 식별자
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


/// 아이템 대 분류
enum ItemType
{
    itUnknown = 0, ///< 에러 검출용

    itPower = 1, ///< 파워 아이템
    itSlot = 2, ///< 슬롯 아이템
};


/// 아이템 기능 분류
enum ItemFuncType
{
    iftUnknown = 0, ///< 에러 검출용

    iftCoinUnlimited = 101, ///< 코인 무제한
    iftDoubleRupo = 301, ///< 더블 루포
    iftDoubleExpPoint = 302, ///< 더블 경험치
    iftRpProtection = 303, ///< RP 50% 보호
    iftMegaphone = 401, ///< 확성기
    iftColorChatting = 402, ///< 컬러 채팅
    iftChannelTicket = 501, ///< (초보, 중수) 채널 이용권
    iftSuperRoomPremium = 502, ///< 슈퍼방장(프리미엄)
    iftSuperRoomCooperative = 504, ///< 슈퍼방장(메탈시리즈/협력)

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


/// 카드 슬롯 인덱스 (0부터 시작)
typedef Wdr::Octet CardSlotIndex;

static const CardSlotIndex INVALID_CARDSLOTINDEX = 255; // -1
static const CardSlotIndex firstCardSlotIndex = 0;

inline bool isValidCardSlotIndex(CardSlotIndex index)
{
    return (firstCardSlotIndex <= index) && (index < DL_MAX_CARD_SLOT_COUNT);
}


/**
 * @struct BasicItemInfo
 * 기본 아이템 정보
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
 * 보유한 아이템 정보
 */
struct ItemInfo : public BasicItemInfo
{
    Wdr::Short limitHour_; ///< 시간 제한(0이면 제한 없음)
    Wdr::Octet limitCount_; ///< 사용 횟수 제한(0이면 제한 없음)
    ItemOverlapCode overlapCode_; ///< 아이템 중복 코드

    explicit ItemInfo(ItemId itemId = INVALID_ITEMID,
        ItemCode itemCode = INVALID_ITEMCODE,
        ItemOverlapCode overlapCode = 0,
        Wdr::Short limitHour = 0, Wdr::Octet limitCount = 0) :
        BasicItemInfo(itemId, itemCode),
        overlapCode_(overlapCode),
        limitHour_(limitHour),
        limitCount_(limitCount) {}
};


/// 보유한 아이템 목록
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
 * 슬롯 아이템 정보
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

    /// 아이템 사용 기간(횟수)이 만료되었는가?
    bool isExpired(time_t currentTime) const {
        return (currentTime < toTime_) || (leftCount_ > 0);
    }

    /// 기간 제한 아이템?
    bool isPeriodLimited() const {
        return toTime_ > fromTime_;
    }

    /// 사용 횟수 제한 아이템?
    bool isUseCountLimited() const {
        return ! isPeriodLimited();
    }
};

/// 슬롯 아이템 목록
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
