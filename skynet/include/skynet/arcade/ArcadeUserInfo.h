/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ArcadeUserInfo.h
//  - Arcade 사용자 정보
//
#if !defined(INCLUDED_POPORUUSERINFO)
#define INCLUDED_POPORUUSERINFO

#include "../DataTypes.h"
#include "ArcadeDataLimits.h"
#include "ArcadeCharacterCardInfo.h"
#include "ArcadeGameScoreInfo.h"
#include "ArcadeGameRoomInfo.h"
#include "ArcadeChannelInfo.h"
#include "ArcadeGameInfo.h"
#include "ArcadeRewardInfo.h"
#include "ArcadeGuildInfo.h"
#include "ArcadeItemInfo.h"
#include "ArcadeCommonInfo.h"
#include <skynet/Wdr.h>
#include <boost/static_assert.hpp>
#include "ServerType.h"
#pragma warning ( push )
#pragma warning ( disable: 4702 )
#include <map>
#pragma warning ( pop )
#include <skynet/detail/Assert.h>

namespace skynet
{

/// 성별
enum Sex {
    SEX_MALE = 0,
    SEX_FEMALE,
    SEX_UNKNOWN
};

inline bool isValidSex(Sex sex)
{
    return (sex == SEX_MALE) || (sex == SEX_FEMALE);
}


/// 사용자 등급
enum Grade {
    G_UNKNOWN = 255,

    G_START = 0,
    G_USER = G_START, ///< 일반 유저
    G_OPERATOR = 100, ///< 운영자

    G_BOUNDARY
};



inline bool isValidGrade(Grade grade)
{
    return (G_START <= grade) && (grade < G_BOUNDARY);
}

inline bool isOperator(Grade grade)
{
    return G_OPERATOR == grade;
}


enum ToolUseGrade {
    TUG_UNKNOWN = -1,
    TUG_START = 0,

    TUG_GENERAL = TUG_START + 10,
    TUG_OPERATOR = TUG_START + 20,
    TUG_ADMIN = TUG_START + 30,

    TUG_BOUNDARY
};

inline bool isAdmin(ToolUseGrade grade)
{
    return TUG_ADMIN == grade ? true : false;
}

inline bool isUseGrade(ToolUseGrade grade)
{
    if( TUG_OPERATOR == grade || TUG_ADMIN == grade )
        return true;
    return false;
}
namespace mode 
{
    enum AuthenticMode
    {
        MODE_UNKNOWN = 0x00,
        MODE_SERVERTOOL =0x01,
        MODE_MONITORTOOL = 0x02,
        MODE_COOKIE_MONITORTOOL = 0x04,
        MODE_COOKIE_SERVERTOOL = 0x08,
    };
}

//구조체 아무값이나 꼭체울것
struct AuthenticImpo
{
    std::string cookie_;
    std::string id_;
    std::string pw_;
    ToolUseGrade grade_;
    ServerId serverId_;
};


inline bool isValidToolUseGrade(ToolUseGrade toolUseGrade)
{
    return (TUG_START <= toolUseGrade) && (toolUseGrade < TUG_BOUNDARY);
}

inline bool isToolUseGeneral(ToolUseGrade toolUseGrade)
{
    return TUG_GENERAL == toolUseGrade;
}

inline bool isToolUseOperator(ToolUseGrade toolUseGrade)
{
    return TUG_OPERATOR == toolUseGrade;
}

inline bool isToolUseAdmin(ToolUseGrade toolUseGrade)
{
    return TUG_ADMIN == toolUseGrade;
}

/**
* @struct CoreUserInfo
*
* 사용자 기본 정보
*/
struct CoreUserInfo
{
    UserNo userNo_; ///< 사용자 일련번호
    std::string nickname_; ///< 닉네임
    Sex sex_; ///< 성별

    /// ctor
    CoreUserInfo() :
        userNo_(INVALID_USERNO),
        sex_(SEX_MALE) {}
};


/**
 * @struct CommonUserInfo
 *
 * 일반적인 사용자 정보
 */
struct CommonUserInfo : public CoreUserInfo
{
    Grade grade_; ///< 사용자 등급
    GuildInfo guildInfo_; ///< 길드 정보

    CommonUserInfo() :
        grade_(G_UNKNOWN) {}
};

/**
* @struct ChannelWaiter
*
* 채널 대기자 정보
*/
struct ChannelWaiter : public CommonUserInfo
{
    /// 게임 정보
    struct GameInfo {
        GameClass gameClass_;
        RankingPoint rankingPoint_;

        GameInfo(GameClass gc = GC_UNKNOWN, RankingPoint rp = 0) :
            gameClass_(gc),
            rankingPoint_(rp) {}
    };
    typedef std::map<GameType, GameInfo> GameInfos;

    GameInfos gameInfos_; ///< 게임별 정보
    GameType gameType_; ///< 현재 로딩 중인 게임 타이틀

    ChannelWaiter() :
        gameType_(GT_UNKNOWN) {}
};

/// 채널 대기자 목록
typedef std::vector<ChannelWaiter> ChannelWaiters;


/**
* @struct UserInfo
*
* 사용자 정보(로비, 레디룸에서 사용)
*/
struct UserInfo : public ChannelWaiter
{
    GameScore gameScore_; ///< 정보 요청자가 로딩한 게임의 해당 전적
};


/**
* @struct Gamer
*
* 접속 중인 사용자 정보
*/
struct Gamer : public ChannelWaiter
{
    RoomId position_; ///< 현재 위치

    Gamer() :
        position_(INVALID_ROOMID) {}
};


/// 접속 중인 사용자 목록
typedef std::vector<Gamer> Gamers;


/**
* @struct FullUserInfo
*
* 자세한 사용자 정보
*/
struct FullUserInfo : public CommonUserInfo
{
    CharacterCard card_; ///< 현재 사용 중인 캐릭터 카드 정보
    std::string area_; ///< 지역
    std::string introduction_; ///< 자기 소개(인사말)
    bool fromPcRoom_; ///< PC방에서 접속했는가?

    FullUserInfo() :
        fromPcRoom_(false) {}
};


/**
 * @struct DetailedUserInfo
 *
 * 가장 상세한 사용자 정보
 * - 사용자 정보
 * - 해당 게임 장르의 게임별/모드별 전적
 */
struct DetailedUserInfo : public FullUserInfo
{
    CharacterCards cards_; ///< 소유한 캐릭터 카드들


    GameType currentGameType_; ///< 현재 로딩 중인 게임 타이틀
    DetailedGameScores gameScores_; ///< 게임별/모드별 상세 전적

    //Candy totalCandy_; ///< 총 보유 캔디

    DetailedUserInfo() :
        currentGameType_(GT_UNKNOWN)/*,
        totalCandy_(0)*/ {}
};


/**
* @struct GameRoomWaiter
*
* 게임방 대기자(슬롯, 관전 대기자 포함) 정보
* - isObserverSlot(slotIndex_) == true: 관전자
*/
struct GameRoomWaiter : public FullUserInfo
{
	Wdr::Short entryNumber_;
    GameScore gameScore_;///< 전적

    SlotIndex slotIndex_; ///< 슬롯 인덱스
    AddressPairs udpAddresses_; ///< 슬롯 대기자인 경우 UDP 주소
    Wdr::UShort pingTime_; ///< 핑타임(ms)

    GameRoomWaiter() :
        slotIndex_(INVALID_SLOTINDEX),
        pingTime_(0) {}
};


/// 게임방 대기자(슬롯, 관전 대기자 포함) 목록
typedef std::vector<GameRoomWaiter> GameRoomWaiters;


/// 초대 거절 사유
/// - 클라이언트에서 알아서 쓰면 됨
enum InvitationRejectionReason
{
    IRR_UNKNOWN = 0,
    /// 사용자가 거절
    IRR_CANCEL,
    /// 로딩 중
    IRR_LOADING,
    /// 초대 거절 중
    IRR_REJECTING,

    IRR_MAX = 255
};


inline bool isValidInvitationRejectionReason(InvitationRejectionReason reason)
{
    return (IRR_UNKNOWN <= reason) && (reason <= IRR_MAX);
}


/// 캔디 목록
//typedef std::map<UserNo, CandyInfo> CandyInfos;

} // namespace skynet

#endif // !defined(INCLUDED_POPORUUSERINFO)
