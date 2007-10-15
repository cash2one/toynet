/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ArcadeServerInfo.h
//  - Arcade 서버 정보(모니터에서 사용)
//
#if !defined(INCLUDED_POPORUSERVERINFO)
#define INCLUDED_POPORUSERVERINFO

#include "ArcadeGameRoomInfo.h"
#include "ArcadeChannelInfo.h"
#include <skynet/Wdr.h>
#include <skynet/arcade/ServerType.h>
#include <vector>
#include <hash_map>

namespace skynet
{

/**
* 서버 상태
*/
enum ServerState
{
    SERVER_UNKNOWN=0,
    SERVER_DEADLOCK=1,
    SERVER_OFF=2,
    SERVER_ON=3
};

typedef std::map<ServerId, ServerState> ServerStateMap;

inline bool isServerAlive(ServerState ss)
{
    return SERVER_ON == ss;
}


struct ServerListInfo
{
    ServerId serverId_;
    AddressPair addrPair_;
    std::string serviceName_;
    ServerType serverType_;
    ChannelId channelId_;
    ChannelType channelType_;
    GameGenre gameGenre_;
    Wdr::UShort maxUser_;
    Wdr::UShort maxRoomCount_;

    ServerListInfo():
        serverId_(-1),
        serverType_(stUnknown),
        channelId_(-1),
        channelType_(CT_PADDING),
        gameGenre_(GG_PADDING),
        maxUser_(0),
        maxRoomCount_(0){}
    ~ServerListInfo()
    {
        serviceName_.clear();
    }
};

typedef std::vector<ServerListInfo> ServerListInfos;

struct GameChannelInfo
{
    Wdr::UShort curLoginUser_;
    Wdr::UShort curRoomCount_;
    GameChannelInfo():
        curLoginUser_(0),
        curRoomCount_(0){}
};

typedef std::hash_map<ServerId,GameChannelInfo> GameChannelInfoMap;

/**
 * @struct CommonServerInfo
 *
 * 공통된 서버 정보
 */
struct CommonServerInfo
{
    ServerState serverState_;

    CommonServerInfo():
        serverState_(SERVER_UNKNOWN) {}
};


/**
* @struct LoginServerInfo
*
* 로그인 서버 정보
*/
struct LoginServerInfo : public CommonServerInfo
{
    ChannelGroups channelGroups_; ///< 채널군 정보
};


/**
* @struct RelayServerInfo
*
* 관전 서버 정보
*/
struct RelayServerInfo : public CommonServerInfo
{
    Wdr::UShort totalRelayRoomCount_; ///< 관전방 갯수

    RelayServerInfo() :
        totalRelayRoomCount_(0) {}
};


/**
* @struct ForWardServerInfo
*
* 포워드 서버 정보
*/
struct ForwardServerInfo : public CommonServerInfo
{
};


/**
* @struct GameServerInfo
*
* 게임 서버 정보
*/
struct GameServerInfo : public CommonServerInfo
{ 
    Wdr::UShort totalUserCount_; ///< 전체 사용자 수
    GameRoomInfos gameRoomInfos_; ///< 전체 게임방 정보
    AddressPair relayServerAddress_; ///< 관전 서버 주소

    RelayServerInfo relayServerInfo_; ///< 관전 서버 정보
    ForwardServerInfo forwardServerInfo_; ///< 포워드 서버 정보

    GameServerInfo() :
        totalUserCount_(0) {}
};

typedef std::map<ChannelId, GameServerInfo> GameServerInfoMap;


struct PoporuServersInfo
{
    LoginServerInfo loginServerInfo_; ///< 로그인 서버 정보
    GameServerInfoMap gameServerInfoMap_; ///< 모든 채널 서버 정보
};

} // namespace skynet

#endif // !defined(INCLUDED_POPORUSERVERINFO)
