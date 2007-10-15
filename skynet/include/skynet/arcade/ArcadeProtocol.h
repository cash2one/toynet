/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ArcadeProtocol.h
#if !defined(INCLUDED_POPORUPROTOCOL)
#define INCLUDED_POPORUPROTOCOL

#include "../Protocol.h"

namespace skynet
{

/**
 * 메세지(패킷) 타입.
 * - LS: LoginServer
 * - GS: GameServer
 * - RS: RelayServer
 * - MS: MonitorServer
 * - FS: ForwardServer
 * - IM: Messenger
 */
enum PoporuMessageType_t
{
    /// LoginServer 관련
    MT_LS_START = (MT_SERVER_DEFINED + 1),
    MT_LS_STOP = (MT_LS_START + 1000),

    /// GameServer 관련
    MT_GS_START = (MT_LS_STOP + 1),
    MT_GS_STOP = (MT_GS_START + 2000),

    /// RelayServer 관련
    MT_RS_START = (MT_GS_STOP + 1),
    MT_RS_STOP = (MT_RS_START + 1000),

    /// MonitorServer 관련
    MT_MS_START = (MT_RS_STOP + 1),
    MT_MS_STOP = (MT_MS_START + 1000),

    /// ForwardServer 관련
    MT_FS_START = (MT_MS_STOP + 1),
    MT_FS_STOP = (MT_FS_START + 1000),

    /// Messenger 관련
    MT_IM_START = (MT_FS_STOP + 1),
    MT_IM_STOP = (MT_IM_START + 1000)
};

} // namespace skynet

#endif // !defined(INCLUDED_POPORUPROTOCOL)
