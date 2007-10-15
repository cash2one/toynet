/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ArcadeGameRoomInfo.h
//  - Arcade 방 정보
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

/// 게임 취소 사유
enum CancelReason
{
    CR_UNKNOWN = 0, ///< 에러 검출용

    // = from Client
    CR_MISSYNC, ///< 싱크가 맞지 않음

    // = from Server
    CR_CHALLENGE, ///< 도전 신청에 의한 게임 취소
    /// 협력 게임 도중 플레이어가 접속 해제되었다
    CR_PLAYER_PARTED_IN_COOPERATIVE_GAME,
    ///< 싱글 게임 도중 플레이어가 퇴장(접속 해제)했다
    CR_PLAYER_PARTED_IN_SINGLE_MODE,
    ///< 싱글 게임 시작 바로 직전에 플레이어가 입장하였다
    CR_PLAYER_JOINED_IN_SINGLE_MODE,
    ///< 카운트 다운 도중 플레이어가 퇴장(접속 해제)했다
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
* 게임 방 정보
*/
struct GameRoomInfo
{
    RoomId idRoom_; ///< 방 번호
    std::string name_; ///< 방 제목
    bool isPrivate_; ///< 비밀방?
    GameType gameType_; ///< 게임 타이틀
    RoomType roomType_; ///< 방 형태
    GameModeType gameMode_; ///< 게임 모드
    GameRoomState gameRoomState_; ///< 게임방 상태
    Wdr::Octet playerCount_; ///< 슬롯 대기자 수
    Wdr::Octet openedSlotCount_; ///< 열려 있는 슬롯 갯수
    Wdr::Octet totalSlotCount_; ///< 전체 슬롯 갯수
    Wdr::Octet observerCount_; ///< 관전자 수
    Wdr::Long time_; ///< 방생성 시간 or 경기 시간(대회 게임방)

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

/// 방 목록
typedef std::vector<GameRoomInfo> GameRoomInfos;


/// 슬롯 인덱스
enum SlotIndex
{
    SI_FIRST = 0, ///< 첫번째 슬롯 인덱스
    SI_MAX = SI_FIRST + (DL_MAX_SLOT_COUNT - 1),

    SI_OBSERVER, ///< 관전자

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

/// 슬롯 상태
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


/// 게임 준비 상태
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
 * 게임방 슬롯 정보
 */
struct SlotInfo
{
    SlotState state_; ///< 슬롯 상태
    ReadyFlag flag_; ///< 슬롯 준비 상태

    SlotInfo(SlotState state = SS_UNKNOWN, ReadyFlag flag = RF_UNKNOWN) :
        state_(state),
        flag_(flag) {}
};

/// 배열 인덱스 = SlotIndex - SI_FIRST
/// - 현재는 SI_FIRST가 0이므로 그냥 0부터 시작해도 무방
typedef std::vector<SlotInfo> SlotInfos;


/**
 * @struct DetailedSlotInfo
 *
 * 좀더 상세한 게임방 슬롯 정보
 */
struct DetailedSlotInfo : public SlotInfo
{
    UserNo waiter_; ///< 슬롯 대기자

    DetailedSlotInfo(UserNo waiter = INVALID_USERNO,
            SlotState state = SS_UNKNOWN, ReadyFlag flag = RF_UNKNOWN) :
        waiter_(waiter),
        SlotInfo(state, flag) {}
};

typedef std::vector<DetailedSlotInfo> DetailedSlotInfos;


/**
 * @struct DetailedGameRoomInfo
 *
 * 게임방 세부 정보
 */
struct DetailedGameRoomInfo : public GameRoomInfo
{
    UserNo ownerUserNo_; ///< 방장 UserNo
    SlotInfos slotInfos_; ///< 슬롯 정보

    DetailedGameRoomInfo() :
        ownerUserNo_(INVALID_USERNO) {}
};


/// 게임 라운드 식별자
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

/// 게임 라운드 식별자 목록
typedef std::vector<GameRoundId> GameRoundIds;


/**
 * 게임라운드 스테이지
 * - grsFinal를 제외한 나머지 스테이지는 토너먼트 모드에서만 의미가 있다
 */
enum GameRoundStage
{
    grsUnknown = 0,
    grsQuarterFinal, ///< 8강
    grsSemiFinal, ///< 4강
    grsFinal, ///< 결승
    grsBoundary
};

inline bool isValidGameRoundStage(GameRoundStage stage)
{
    return (grsUnknown < stage) && (stage < grsBoundary);
}


/**
 * @struct GameRoundInfo
 *
 * 게임 라운드 정보
 */
struct GameRoundInfo
{
    GameRoundId id_;
    UserNos players_; ///< 게임 라운드 플레이어 목록

    GameRoundInfo() :
        id_(INVALID_GAMEROUNDID) {}

    /// 부전승 게임라운드인가?
    bool isUnearnedWinGameRound() const {
        return players_.size() == 1;
    }
};

/// 게임 라운드 목록
typedef std::vector<GameRoundInfo> GameRoundInfos;

/**
 * @struct PlayerAddressInfo
 *
 * 게임방 플레이어의 주소 정보
 */
struct PlayerAddressInfo
{
    UserNo userNo_; ///< 플레이어(사용자) 일련 번호
    AddressPairs addressPairs_; ///< UDP 주소쌍 목록

    PlayerAddressInfo() :
        userNo_(INVALID_USERNO) {}
};

/// 플레이어 주소 목록
typedef std::vector<PlayerAddressInfo> PlayerAddressInfos;

} // namespace skynet

#endif // !defined(INCLUDED_POPORUGAMEROOMINFO)
