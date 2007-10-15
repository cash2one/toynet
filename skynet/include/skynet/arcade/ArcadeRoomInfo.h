/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ArcadeRoomInfo.h
//  - Arcade �� ����
//
#if !defined(INCLUDED_POPORUROOMINFO)
#define INCLUDED_POPORUROOMINFO

#include "ArcadeCommonInfo.h"

namespace skynet
{

/// (����)�� ����
enum RoomType
{
    RT_UNKNOWN = 0,

    RT_NORMAL = 1, ///< �Ϲ� ��(1)

    RT_EVENT, ///< ��� �̺�Ʈ�� ������(2)
    RT_TOURNAMENT, ///< ��ȸ ���ӹ�(3)

    RT_SUPERROOMPREMIUM, ///< ���۹���(�����̾�)(4)
    RT_SUPERROOMCOOPERATIVE, ///< ���۹���(��Ż�ø���)(5)

    RT_BOUNDARY
};

inline bool isValidRoomType(RoomType rt)
{
    return (RT_UNKNOWN < rt) && (rt < RT_BOUNDARY);
}

/// ��ڰ� ���� �� �ִ� ���ӹ��ΰ�?
inline bool isOperatorGameRoom(RoomType rt)
{
    return (RT_EVENT == rt) || (RT_TOURNAMENT == rt);
}

/// ���۹��ΰ�?
inline bool isSuperGameRoom(RoomType rt)
{
    return (RT_SUPERROOMPREMIUM == rt) || (RT_SUPERROOMCOOPERATIVE == rt);
}


/// ���ӹ� ����
enum GameRoomState
{
    GRS_UNKNOWN = 0,

    GRS_WAITING, ///< ��� ��
    GRS_PLAYING, ///< ���� ��

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


/// (����) �� ���̵�(��ȣ)
enum RoomId
{
    INVALID_ROOMID = -1,
    RI_LOBBY = 0, ///< �κ�(ä�� ����)

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


/// �� (����) ���� ����
enum PartReason
{
    PR_UNKNOWN = 0, ///< ���� �����

    PR_HIMSELF, ///< �ڹ��� ����
    PR_KICKED, ///< ������ kick �ߴ�
    PR_BANNED, ///< ������ ban �ߴ�

    PR_NO_COIN, ///< ȥ�� �ִ� �濡�� ������ ���� ������ߴ�

    PR_LOGOUT, ///< �α׾ƿ��� ���� �� ����

    /// ���� ������
    /// - ��ڹ濡�� ��ڰ� �����߰ų�, ���۹濡�� ������ �������� ���
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
