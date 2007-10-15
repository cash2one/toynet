/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ArcadeUserInfo.h
//  - Arcade ����� ����
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

/// ����
enum Sex {
    SEX_MALE = 0,
    SEX_FEMALE,
    SEX_UNKNOWN
};

inline bool isValidSex(Sex sex)
{
    return (sex == SEX_MALE) || (sex == SEX_FEMALE);
}


/// ����� ���
enum Grade {
    G_UNKNOWN = 255,

    G_START = 0,
    G_USER = G_START, ///< �Ϲ� ����
    G_OPERATOR = 100, ///< ���

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

//����ü �ƹ����̳� ��ü���
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
* ����� �⺻ ����
*/
struct CoreUserInfo
{
    UserNo userNo_; ///< ����� �Ϸù�ȣ
    std::string nickname_; ///< �г���
    Sex sex_; ///< ����

    /// ctor
    CoreUserInfo() :
        userNo_(INVALID_USERNO),
        sex_(SEX_MALE) {}
};


/**
 * @struct CommonUserInfo
 *
 * �Ϲ����� ����� ����
 */
struct CommonUserInfo : public CoreUserInfo
{
    Grade grade_; ///< ����� ���
    GuildInfo guildInfo_; ///< ��� ����

    CommonUserInfo() :
        grade_(G_UNKNOWN) {}
};

/**
* @struct ChannelWaiter
*
* ä�� ����� ����
*/
struct ChannelWaiter : public CommonUserInfo
{
    /// ���� ����
    struct GameInfo {
        GameClass gameClass_;
        RankingPoint rankingPoint_;

        GameInfo(GameClass gc = GC_UNKNOWN, RankingPoint rp = 0) :
            gameClass_(gc),
            rankingPoint_(rp) {}
    };
    typedef std::map<GameType, GameInfo> GameInfos;

    GameInfos gameInfos_; ///< ���Ӻ� ����
    GameType gameType_; ///< ���� �ε� ���� ���� Ÿ��Ʋ

    ChannelWaiter() :
        gameType_(GT_UNKNOWN) {}
};

/// ä�� ����� ���
typedef std::vector<ChannelWaiter> ChannelWaiters;


/**
* @struct UserInfo
*
* ����� ����(�κ�, ����뿡�� ���)
*/
struct UserInfo : public ChannelWaiter
{
    GameScore gameScore_; ///< ���� ��û�ڰ� �ε��� ������ �ش� ����
};


/**
* @struct Gamer
*
* ���� ���� ����� ����
*/
struct Gamer : public ChannelWaiter
{
    RoomId position_; ///< ���� ��ġ

    Gamer() :
        position_(INVALID_ROOMID) {}
};


/// ���� ���� ����� ���
typedef std::vector<Gamer> Gamers;


/**
* @struct FullUserInfo
*
* �ڼ��� ����� ����
*/
struct FullUserInfo : public CommonUserInfo
{
    CharacterCard card_; ///< ���� ��� ���� ĳ���� ī�� ����
    std::string area_; ///< ����
    std::string introduction_; ///< �ڱ� �Ұ�(�λ縻)
    bool fromPcRoom_; ///< PC�濡�� �����ߴ°�?

    FullUserInfo() :
        fromPcRoom_(false) {}
};


/**
 * @struct DetailedUserInfo
 *
 * ���� ���� ����� ����
 * - ����� ����
 * - �ش� ���� �帣�� ���Ӻ�/��庰 ����
 */
struct DetailedUserInfo : public FullUserInfo
{
    CharacterCards cards_; ///< ������ ĳ���� ī���


    GameType currentGameType_; ///< ���� �ε� ���� ���� Ÿ��Ʋ
    DetailedGameScores gameScores_; ///< ���Ӻ�/��庰 �� ����

    //Candy totalCandy_; ///< �� ���� ĵ��

    DetailedUserInfo() :
        currentGameType_(GT_UNKNOWN)/*,
        totalCandy_(0)*/ {}
};


/**
* @struct GameRoomWaiter
*
* ���ӹ� �����(����, ���� ����� ����) ����
* - isObserverSlot(slotIndex_) == true: ������
*/
struct GameRoomWaiter : public FullUserInfo
{
	Wdr::Short entryNumber_;
    GameScore gameScore_;///< ����

    SlotIndex slotIndex_; ///< ���� �ε���
    AddressPairs udpAddresses_; ///< ���� ������� ��� UDP �ּ�
    Wdr::UShort pingTime_; ///< ��Ÿ��(ms)

    GameRoomWaiter() :
        slotIndex_(INVALID_SLOTINDEX),
        pingTime_(0) {}
};


/// ���ӹ� �����(����, ���� ����� ����) ���
typedef std::vector<GameRoomWaiter> GameRoomWaiters;


/// �ʴ� ���� ����
/// - Ŭ���̾�Ʈ���� �˾Ƽ� ���� ��
enum InvitationRejectionReason
{
    IRR_UNKNOWN = 0,
    /// ����ڰ� ����
    IRR_CANCEL,
    /// �ε� ��
    IRR_LOADING,
    /// �ʴ� ���� ��
    IRR_REJECTING,

    IRR_MAX = 255
};


inline bool isValidInvitationRejectionReason(InvitationRejectionReason reason)
{
    return (IRR_UNKNOWN <= reason) && (reason <= IRR_MAX);
}


/// ĵ�� ���
//typedef std::map<UserNo, CandyInfo> CandyInfos;

} // namespace skynet

#endif // !defined(INCLUDED_POPORUUSERINFO)
