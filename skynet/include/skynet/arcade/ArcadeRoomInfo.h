/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ArcadeRoomInfo.h
//  - Arcade 방 정보
//
#if !defined(INCLUDED_POPORUROOMINFO)
#define INCLUDED_POPORUROOMINFO

#include "ArcadeCommonInfo.h"

namespace skynet
{

/// (게임)방 종류
enum RoomType
{
    RT_UNKNOWN = 0,

    RT_NORMAL = 1, ///< 일반 방(1)

    RT_EVENT, ///< 운영자 이벤트용 게임장(2)
    RT_TOURNAMENT, ///< 대회 게임방(3)

    RT_SUPERROOMPREMIUM, ///< 슈퍼방장(프리미엄)(4)
    RT_SUPERROOMCOOPERATIVE, ///< 슈퍼방장(메탈시리즈)(5)

    RT_BOUNDARY
};

inline bool isValidRoomType(RoomType rt)
{
    return (RT_UNKNOWN < rt) && (rt < RT_BOUNDARY);
}

/// 운영자가 만들 수 있는 게임방인가?
inline bool isOperatorGameRoom(RoomType rt)
{
    return (RT_EVENT == rt) || (RT_TOURNAMENT == rt);
}

/// 슈퍼방인가?
inline bool isSuperGameRoom(RoomType rt)
{
    return (RT_SUPERROOMPREMIUM == rt) || (RT_SUPERROOMCOOPERATIVE == rt);
}


/// 게임방 상태
enum GameRoomState
{
    GRS_UNKNOWN = 0,

    GRS_WAITING, ///< 대기 중
    GRS_PLAYING, ///< 게임 중

    GRS_BOUNDARY
};

inline bool isValidGameRoomState(GameRoomState state)
{
    return (GRS_UNKNOWN < state) && (state < GRS_BOUNDARY);
}

inline bool isGamePlaying(GameRoomState state)
{
    return GRS_PLAYING == state;
}


/// (게임) 방 아이디(번호)
enum RoomId
{
    INVALID_ROOMID = -1,
    RI_LOBBY = 0, ///< 로비(채널 대기실)

    RI_ROOM_START = 1
};

inline bool isValidRoomId(RoomId idRoom)
{
    return INVALID_ROOMID != idRoom;
}


inline bool isLobby(RoomId idRoom)
{
    return RI_LOBBY == idRoom;
}

inline RoomId toRoomId(int roomId)
{
    return static_cast<RoomId>(roomId);
}

inline RoomId incrementRoomId(RoomId& roomId)
{
    roomId = static_cast<RoomId>(roomId + 1);
    return roomId;
}


/// 방 (강제) 퇴장 사유
enum PartReason
{
    PR_UNKNOWN = 0, ///< 에러 검출용

    PR_HIMSELF, ///< 자발적 퇴장
    PR_KICKED, ///< 방장이 kick 했다
    PR_BANNED, ///< 방장이 ban 했다

    PR_NO_COIN, ///< 혼자 있는 방에서 코인이 없어 강퇴당했다

    PR_LOGOUT, ///< 로그아웃에 의한 방 퇴장

    /// 방이 닫혔다
    /// - 운영자방에서 운영자가 퇴장했거나, 슈퍼방에서 방장이 퇴장했을 경우
    PR_DESTROYED,

    PR_BOUNDARY
};

inline bool isValidPartReason(PartReason reason)
{
    return (PR_UNKNOWN < reason) && (reason < PR_BOUNDARY);
}

inline bool isBanned(PartReason reason)
{
    return PR_BANNED == reason;
}

inline bool isLogout(PartReason reason)
{
    return PR_LOGOUT == reason;
}

} // namespace skynet

#endif // !defined(INCLUDED_POPORUROOMINFO)
