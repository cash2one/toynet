/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ArcadeServerInfo.h
//  - Arcade ���� ����(����Ϳ��� ���)
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
* ���� ����
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
 * ����� ���� ����
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
* �α��� ���� ����
*/
struct LoginServerInfo : public CommonServerInfo
{
    ChannelGroups channelGroups_; ///< ä�α� ����
};


/**
* @struct RelayServerInfo
*
* ���� ���� ����
*/
struct RelayServerInfo : public CommonServerInfo
{
    Wdr::UShort totalRelayRoomCount_; ///< ������ ����

    RelayServerInfo() :
        totalRelayRoomCount_(0) {}
};


/**
* @struct ForWardServerInfo
*
* ������ ���� ����
*/
struct ForwardServerInfo : public CommonServerInfo
{
};


/**
* @struct GameServerInfo
*
* ���� ���� ����
*/
struct GameServerInfo : public CommonServerInfo
{ 
    Wdr::UShort totalUserCount_; ///< ��ü ����� ��
    GameRoomInfos gameRoomInfos_; ///< ��ü ���ӹ� ����
    AddressPair relayServerAddress_; ///< ���� ���� �ּ�

    RelayServerInfo relayServerInfo_; ///< ���� ���� ����
    ForwardServerInfo forwardServerInfo_; ///< ������ ���� ����

    GameServerInfo() :
        totalUserCount_(0) {}
};

typedef std::map<ChannelId, GameServerInfo> GameServerInfoMap;


struct PoporuServersInfo
{
    LoginServerInfo loginServerInfo_; ///< �α��� ���� ����
    GameServerInfoMap gameServerInfoMap_; ///< ��� ä�� ���� ����
};

} // namespace skynet

#endif // !defined(INCLUDED_POPORUSERVERINFO)
