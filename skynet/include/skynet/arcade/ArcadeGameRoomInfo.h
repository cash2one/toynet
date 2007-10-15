/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ArcadeGameRoomInfo.h
//  - Arcade �� ����
//
#if !defined(INCLUDED_POPORUGAMEROOMINFO)
#define INCLUDED_POPORUGAMEROOMINFO

#include "../DataTypes.h"
#include "ArcadeGameInfo.h"
#include "ArcadeRoomInfo.h"
#include "ArcadeCommonInfo.h"
#include <skynet/Wdr.h>

namespace skynet
{

/// ���� ��� ����
enum CancelReason
{
    CR_UNKNOWN = 0, ///< ���� �����

    // = from Client
    CR_MISSYNC, ///< ��ũ�� ���� ����

    // = from Server
    CR_CHALLENGE, ///< ���� ��û�� ���� ���� ���
    /// ���� ���� ���� �÷��̾ ���� �����Ǿ���
    CR_PLAYER_PARTED_IN_COOPERATIVE_GAME,
    ///< �̱� ���� ���� �÷��̾ ����(���� ����)�ߴ�
    CR_PLAYER_PARTED_IN_SINGLE_MODE,
    ///< �̱� ���� ���� �ٷ� ������ �÷��̾ �����Ͽ���
    CR_PLAYER_JOINED_IN_SINGLE_MODE,
    ///< ī��Ʈ �ٿ� ���� �÷��̾ ����(���� ����)�ߴ�
    CR_PLAYER_PARTED_WHEN_COUNTINGDOWN,

    CR_BOUNDARY
};

inline bool isValidCancelReason(CancelReason reason)
{
    return (CR_UNKNOWN < reason) && (reason < CR_BOUNDARY);
}


/**
* @struct GameRoomInfo
*
* ���� �� ����
*/
struct GameRoomInfo
{
    RoomId idRoom_; ///< �� ��ȣ
    std::string name_; ///< �� ����
    bool isPrivate_; ///< ��й�?
    GameType gameType_; ///< ���� Ÿ��Ʋ
    RoomType roomType_; ///< �� ����
    GameModeType gameMode_; ///< ���� ���
    GameRoomState gameRoomState_; ///< ���ӹ� ����
    Wdr::Octet playerCount_; ///< ���� ����� ��
    Wdr::Octet openedSlotCount_; ///< ���� �ִ� ���� ����
    Wdr::Octet totalSlotCount_; ///< ��ü ���� ����
    Wdr::Octet observerCount_; ///< ������ ��
    Wdr::Long time_; ///< ����� �ð� or ��� �ð�(��ȸ ���ӹ�)

    GameRoomInfo() :
    idRoom_(INVALID_ROOMID),
        isPrivate_(false),
        gameType_(GT_UNKNOWN),
        roomType_(RT_UNKNOWN),
        gameMode_(GM_UNKNOWN),
        gameRoomState_(GRS_UNKNOWN),
        playerCount_(0),
        openedSlotCount_(0),
        totalSlotCount_(0),
        observerCount_(0),
        time_(0) {}
};

/// �� ���
typedef std::vector<GameRoomInfo> GameRoomInfos;


/// ���� �ε���
enum SlotIndex
{
    SI_FIRST = 0, ///< ù��° ���� �ε���
    SI_MAX = SI_FIRST + (DL_MAX_SLOT_COUNT - 1),

    SI_OBSERVER, ///< ������

    INVALID_SLOTINDEX = SI_OBSERVER
};

inline bool isValidSlotIndex(SlotIndex slotIndex)
{
    return (SI_FIRST <= slotIndex) && (slotIndex <= INVALID_SLOTINDEX);
}

inline bool isObserverSlot(SlotIndex slotIndex)
{
    return slotIndex == SI_OBSERVER;
}

inline SlotIndex toSlotIndex(int value)
{
    return static_cast<SlotIndex>(value);
}

/// ���� ����
enum SlotState
{
    SS_CLOSE = 0,
    SS_OPEN,

    SS_UNKNOWN
};

inline bool isValidSlotState(SlotState ss)
{
    return (ss == SS_CLOSE) || (ss == SS_OPEN);
}

inline bool isSlotOpened(SlotState ss)
{
    return ss == SS_OPEN;
}


/// ���� �غ� ����
enum ReadyFlag
{
    RF_OFF = 0,
    RF_ON,

    RF_UNKNOWN
};

inline bool isValidReadyFlag(ReadyFlag rf)
{
    return (rf == RF_OFF) || (rf == RF_ON);
}

inline ReadyFlag toggleReadyFlag(ReadyFlag rf)
{
    if (rf == RF_OFF)
        return RF_ON;
    return RF_OFF;
}

inline bool isReady(ReadyFlag rf)
{
    return rf == RF_ON;
}


/**
 * @struct SlotInfo
 *
 * ���ӹ� ���� ����
 */
struct SlotInfo
{
    SlotState state_; ///< ���� ����
    ReadyFlag flag_; ///< ���� �غ� ����

    SlotInfo(SlotState state = SS_UNKNOWN, ReadyFlag flag = RF_UNKNOWN) :
        state_(state),
        flag_(flag) {}
};

/// �迭 �ε��� = SlotIndex - SI_FIRST
/// - ����� SI_FIRST�� 0�̹Ƿ� �׳� 0���� �����ص� ����
typedef std::vector<SlotInfo> SlotInfos;


/**
 * @struct DetailedSlotInfo
 *
 * ���� ���� ���ӹ� ���� ����
 */
struct DetailedSlotInfo : public SlotInfo
{
    UserNo waiter_; ///< ���� �����

    DetailedSlotInfo(UserNo waiter = INVALID_USERNO,
            SlotState state = SS_UNKNOWN, ReadyFlag flag = RF_UNKNOWN) :
        waiter_(waiter),
        SlotInfo(state, flag) {}
};

typedef std::vector<DetailedSlotInfo> DetailedSlotInfos;


/**
 * @struct DetailedGameRoomInfo
 *
 * ���ӹ� ���� ����
 */
struct DetailedGameRoomInfo : public GameRoomInfo
{
    UserNo ownerUserNo_; ///< ���� UserNo
    SlotInfos slotInfos_; ///< ���� ����

    DetailedGameRoomInfo() :
        ownerUserNo_(INVALID_USERNO) {}
};


/// ���� ���� �ĺ���
enum GameRoundId
{
    INVALID_GAMEROUNDID = -1,

    GRI_FIRST = 0,
    GRI_SECOND,
    GRI_THIRD,
    GRI_FOURTH,
    GRI_BOUNDARY
};

inline bool isValidGameRoundId(GameRoundId gri)
{
    return (GRI_FIRST <= gri) && (gri < GRI_BOUNDARY);
}

inline GameRoundId toGameRoundId(int roundId)
{
    return static_cast<GameRoundId>(roundId);
}

/// ���� ���� �ĺ��� ���
typedef std::vector<GameRoundId> GameRoundIds;


/**
 * ���Ӷ��� ��������
 * - grsFinal�� ������ ������ ���������� ��ʸ�Ʈ ��忡���� �ǹ̰� �ִ�
 */
enum GameRoundStage
{
    grsUnknown = 0,
    grsQuarterFinal, ///< 8��
    grsSemiFinal, ///< 4��
    grsFinal, ///< ���
    grsBoundary
};

inline bool isValidGameRoundStage(GameRoundStage stage)
{
    return (grsUnknown < stage) && (stage < grsBoundary);
}


/**
 * @struct GameRoundInfo
 *
 * ���� ���� ����
 */
struct GameRoundInfo
{
    GameRoundId id_;
    UserNos players_; ///< ���� ���� �÷��̾� ���

    GameRoundInfo() :
        id_(INVALID_GAMEROUNDID) {}

    /// ������ ���Ӷ����ΰ�?
    bool isUnearnedWinGameRound() const {
        return players_.size() == 1;
    }
};

/// ���� ���� ���
typedef std::vector<GameRoundInfo> GameRoundInfos;

/**
 * @struct PlayerAddressInfo
 *
 * ���ӹ� �÷��̾��� �ּ� ����
 */
struct PlayerAddressInfo
{
    UserNo userNo_; ///< �÷��̾�(�����) �Ϸ� ��ȣ
    AddressPairs addressPairs_; ///< UDP �ּҽ� ���

    PlayerAddressInfo() :
        userNo_(INVALID_USERNO) {}
};

/// �÷��̾� �ּ� ���
typedef std::vector<PlayerAddressInfo> PlayerAddressInfos;

} // namespace skynet

#endif // !defined(INCLUDED_POPORUGAMEROOMINFO)
