/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ArcadeChannelInfo.h
//  - Arcade 채널 정보
//
#if !defined(INCLUDED_POPORUCHANNELINFO)
#define INCLUDED_POPORUCHANNELINFO

#include "ArcadeGameInfo.h"
#include "ArcadeCommonInfo.h"
#pragma warning ( push )
#pragma warning ( disable: 4702 )
#include <map>
#include <vector>
#pragma warning ( pop )

namespace skynet
{

/// 채널 서버 종류
enum ChannelType
{
    CT_UNKNOWN = 0,
    
    CT_BEGINNER, ///< 초보 채널(1)
    CT_FREE, ///< 자유 채널(2)
    CT_MASTER, ///< 고수 채널(3)
    CT_MIDDLE, ///< 중수 채널(4)
    CT_ENTRANCE,///입문
    CT_GENERAL,///일반

    CT_TOURNAMENT = 50, ///< 대회 채널(50)
	CT_PADDING	  = 100,
    CT_LAST
};

inline bool isValidChannelType(ChannelType ct)
{
    return (CT_UNKNOWN < ct) && (ct < CT_LAST);
}


/// 채널 식별자
typedef int ChannelId;
typedef std::vector<ChannelId> ChannelIds;

/**
* @struct Channel
*
* 채널 서버 정보
*/
struct Channel
{
    ChannelId channelId_; ///< 채널 식별자
    ChannelType type_; ///< 채널 종류
    std::string name_;
    AddressPair addressPair_; ///< 채널 서버 주소
    Wdr::UShort maxUserCount_; ///< 채널 최대 허용 접속자 수
    Wdr::UShort curUserCount_; ///< 현재 채널 접속자 수
    bool enabled_; ///< 채널 접속이 가능한가?

    Channel() :
        channelId_(-1),
        type_(CT_UNKNOWN),
        maxUserCount_(0),
        curUserCount_(0),
        enabled_(false) {}
};

/// 채널 서버 목록
typedef std::vector<Channel> Channels;

/// 채널群 정보
typedef std::map<GameGenre, Channels> ChannelGroups;

} // namespace skynet

#endif // !defined(INCLUDED_POPORUCHANNELINFO)
