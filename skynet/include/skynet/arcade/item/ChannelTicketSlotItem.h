/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_CHANNELTICKETSLOTITEM)
#define INCLUDED_CHANNELTICKETSLOTITEM

#include "PeriodLimitedSlotItem.h"
#include <skynet/arcade/ArcadeChannelInfo.h>

namespace skynet
{

/**
 * @class ChannelTicketSlotItem
 * (초보, 중수) 채널 이용권 슬롯 아이템 클래스
 */
class WARNET_API ChannelTicketSlotItem : public PeriodLimitedSlotItem
{
public:
    ChannelTicketSlotItem(const FunctionalItem& funcItem,
        const SlotItemInfo& itemInfo) :
        PeriodLimitedSlotItem(funcItem, itemInfo) {}

    bool isAllowedChannel(ChannelType channelType) const {
        switch (channelType) {
        case CT_BEGINNER:
            return (1 == getSerialNumber()) || (2 == getSerialNumber()) ||
                (3 == getSerialNumber()) || (4 == getSerialNumber()) ||
                (7 == getSerialNumber()) || (8 == getSerialNumber());
        case CT_MIDDLE:
            return (5 == getSerialNumber()) || (6 == getSerialNumber()) ||
                (9 == getSerialNumber());
        //case CT_FREE:
        //case CT_MASTER:
        //case CT_TOURNAMENT:
        }
        WN_ASSERT(false);
        return false;
    }

    virtual bool isAllowed(GameGenre gameGenre) const {
        switch (gameGenre) {
        case GG_KOF_SERIES:
            return (1 == getSerialNumber()) || (2 == getSerialNumber()) ||
                (5 == getSerialNumber()) || (6 == getSerialNumber()) ||
                (7 == getSerialNumber()) || (9 == getSerialNumber());
        case GG_SAMURAI_SERIES:
            return (3 == getSerialNumber()) || (4 == getSerialNumber()) ||
                (8 == getSerialNumber());
        }
        return false;
    }
};

} // namespace skynet

#endif // !defined(INCLUDED_CHANNELTICKETSLOTITEM)
