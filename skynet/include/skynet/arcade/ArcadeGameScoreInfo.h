/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ArcadeGamescoreInfo.h
//  - Arcade ���� ���� ���� ����
//
#if !defined(INCLUDED_POPORUGAMESCOREINFO)
#define INCLUDED_POPORUGAMESCOREINFO

//#include "candy/ArcadeCandyInfo.h"
#include "ArcadeRewardInfo.h"
#include "ArcadeGameInfo.h"
#include "ArcadeGameRoomInfo.h"
#include "ArcadeChannelInfo.h"
#include "ArcadeCommonInfo.h"
#include "ArcadeDataLimits.h"
#include "../DataTypes.h"
#include <skynet/Wdr.h>
#pragma warning ( push )
#pragma warning ( disable: 4702 )
#include <map>
#include <vector>
#pragma warning ( pop )

namespace skynet
{

/**
 * @struct MatchGameRecord
 *
 * ���� ���� ���� ���
 */
struct MatchGameRecord
{
    Wdr::UShort win_; ///< ��
    Wdr::UShort draw_; ///< ��
    Wdr::UShort lose_; ///< ��

    MatchGameRecord(Wdr::UShort win = 0, Wdr::UShort draw = 0,
            Wdr::UShort lose = 0) :
        win_(win),
        draw_(draw),
        lose_(lose) {}
};


/**
 * @struct CooperativeGameRecord
 *
 * ���°��� ���
 */
struct CooperativeGameRecord
{
    Wdr::ULong score_; ///< ȹ�� ���� ��
    Wdr::Octet stage_; ///< Ŭ������ �ִ� ��������
    Wdr::Octet continueCount_; ///< ��Ƽ�� Ƚ��(��Ƽ�� ��忡���� ���)

    CooperativeGameRecord(Wdr::ULong score = 0, Wdr::Octet stage = 0,
            Wdr::Octet continueCount = 0) :
        score_(score),
        stage_(stage),
        continueCount_(continueCount) {}

    bool greater(const CooperativeGameRecord& record) const {
        if (stage_ > record.stage_)
            return true;
        if (stage_ == record.stage_) {
            if (score_ > record.score_)
                return true;
            if (score_ == record.score_) {
                if (continueCount_ < record.continueCount_)
                    return true;
            }
        }
        return false;
    }
};


/**
* @struct GameScore
*
* ���� ����
*/
struct GameScore
{
    /// ���� ���� (WN_ASSERT(isMatchGame(gameType_)))
    struct MatchGame : public MatchGameRecord
    {
        Wdr::UShort disconnect_; ///< ��Ŀ��Ʈ
		Wdr::UShort currentSvCount_; ///< ���� ����(straight victories) Ƚ��
        Wdr::UShort maxSvCount_; ///< �ִ� ���� Ƚ��

        MatchGame() :
            disconnect_(0),
			currentSvCount_(0),
            maxSvCount_(0) {}
    };

    /// ���� ���� (WN_ASSERT(isCooperativeGame(gameType_)))
    struct CooperativeGame : CooperativeGameRecord {};

    GameType gameType_;
    MatchGame matchGame_;
    CooperativeGame cooperativeGame_;

    RankingPoint rankingPoint_; ///< ��ŷ ����Ʈ
    Ranking ranking_; ///< ��ŷ
    Rupo rewardedRupo_; ///< ������� ����(���� ��������� ��ȿ�ϴ�)
    GameClass gameClass_; ///< ���� ���

    GameScore() :
        gameType_(GT_UNKNOWN),
        rankingPoint_(0),
        ranking_(0),
        rewardedRupo_(0),
        gameClass_(GC_UNKNOWN) {}

    bool isValid() const {
        return isValidGameType(gameType_);
    }
};

/// ����ں� ���� ���
typedef std::map<UserNo, GameScore> GameScores;

/// ���Ӻ� ���� ���
typedef std::map<GameType, GameScore> GameScoreByGameTypes;

/// ���� ���� ��庰 ���� ���
typedef std::map<GameModeType, MatchGameRecord> MatchGameRecordByGameModes;

/// ���� ���� ��庰 ���� ���
typedef std::map<GameModeType, CooperativeGameRecord>
    CooperativeGameRecordByGameModes;

/**
 * @struct DetailedGameScore
 *
 * ���� ���� ����
 * - ��庰 ���� ����
 */
struct DetailedGameScore : public GameScore
{
    /// ���������� ���Ӹ�庰 ����
    MatchGameRecordByGameModes matchGameRecords_;

    /// ���°����� ���Ӹ�庰 ����
    CooperativeGameRecordByGameModes cooperativeGameRecords_;
};

/// ���Ӻ� ���� �� ���(GameType = DetailedGameScore::gameType_)
typedef std::vector<DetailedGameScore> DetailedGameScores;

inline DetailedGameScores::iterator findDetailedGameScore(
    DetailedGameScores& scores, GameType gameType)
{
    for (DetailedGameScores::iterator pos = scores.begin();
            pos != scores.end(); ++pos) {
        if ((*pos).gameType_ == gameType)
            return pos;
    }
    return scores.end();
}

inline DetailedGameScores::const_iterator findDetailedGameScore(
    const DetailedGameScores& scores, GameType gameType)
{
    for (DetailedGameScores::const_iterator pos = scores.begin();
            pos != scores.end(); ++pos) {
        if ((*pos).gameType_ == gameType)
            return pos;
    }
    return scores.end();
}


/**
* @struct MatchGameResult
*
* 1:1 ���� ���� ���(KOF, Samurai Shodown, NeoGeoCup, Baseball Stars).
* - playerA/B�� �������� ������ ����.
* - �̱� ����� ��� playerA_�� playerB_�� ���� �����ؾ� �ϸ�, �¸����� ���
*   winner_�� playerA_�� �����ؾ� �ϰ�, ������ ��� winner_�� INVALID_USERNO��
*   �����ؾ� �Ѵ�.
* - ��� ��� winner_�� DRAWED_MATCH�� �����ؾ� �Ѵ�
* - �� �� ���� ���� ����
* - 1:1 �������� �� �÷����� ���� ����� ��ġ���� ���� ��� INVALID_USERNO
*   (���������� ���)
*/ 
struct MatchGameResult
{
    typedef Wdr::Octet Score;

    UserNo playerA_; ///< 1P UserNo
    UserNo playerB_; ///< 2P UserNo
    UserNo winner_; ///< �¸��� �÷��̾� UserNo
    Score scoreA_; ///< 1P ����(�౸, �߱������� ���)
    Score scoreB_; ///< 2P ����(�౸, �߱������� ���)
    bool lotted_; ///< ��÷�� ���� ����ΰ�?

    /// ctor
    MatchGameResult() :
        playerA_(INVALID_USERNO),
        playerB_(INVALID_USERNO),
        winner_(INVALID_USERNO),
        scoreA_(0),
        scoreB_(0),
        lotted_(false) {}

    bool isSinglePlay() const {
        return playerA_ == playerB_;
    }

    /// ��� ���
    bool isDrawed() const {
        return DRAWED_MATCH == winner_;
    }

    bool isPlayerAWin() const {
        return playerA_ == winner_;
    }

    bool isPlayerBWin() const {
        return playerB_ == winner_;
    }

    bool isValid() const {
        if (isSinglePlay()) {
            if (! isValidUserNo(playerA_))
                return false;
            if ((winner_ != playerA_) && (winner_ != INVALID_USERNO) &&
                    (winner_ != DRAWED_MATCH))
                return false;
        }
        else {
            if ((! isValidUserNo(playerA_)) || (! isValidUserNo(playerB_)))
                return false;
            if ((winner_ != playerA_) && (winner_ != playerB_) &&
                    (winner_ != DRAWED_MATCH))
                return false;
        }
        return true;
    }

    bool equals(const MatchGameResult& result) const {
        return (playerA_ == result.playerA_) &&
            (playerB_ == result.playerB_) && (winner_ == result.winner_);
    }
};


/**
* @struct CooperativeGameResult
*
* ���� ����(��Ż������, ž����) ���� ���
* - WN_ASSERT(players_.size() <= MAX_ACTION_GAME_PLAYER_COUNT);
*/
struct CooperativeGameResult
{
    enum { MAX_ACTION_GAME_PLAYER_COUNT = 2 };
    struct Player : public CooperativeGameRecord {
        UserNo userNo_;

        Player() :
            userNo_(INVALID_USERNO) {}
    };
    typedef std::vector<Player> Players;

    Players players_; ///< �÷��̾ ����

    bool isSinglePlay() const {
        return players_.size() == 1;
    }

    Player getPlayerA() const {
        return players_.empty() ? Player() : players_[0];
    }

    Player getPlayerB() const {
        return players_.size() > 1 ? players_[1]: Player();
    }

    bool isValid() const {
        if ((players_.size() < 1) || (players_.size() > 2))
            return false;
        for (Players::const_iterator pos = players_.begin();
                pos != players_.end(); ++pos) {
            if (! isValidUserNo((*pos).userNo_))
                return false;
        }
        return true;
    }
};


/**
* @struct GameResult
*
* ���� ���
* - gameType_�� ���� � �ʵ尡 ������ �����ȴ�
* - if (isMatchGame(gameType_)) matchGameResult_ is valid
* - if (isCooperativeGame(gameType_)) cooperativeGameResult_ is valid
* - ���� ������ �� isValid() �Լ��� ȣ���ؼ� ��ȿ���� �����ϴ� ���� ����!!
*/
struct GameResult
{
    GameType gameType_;

    MatchGameResult matchGameResult_;
    CooperativeGameResult cooperativeGameResult_;

    /// ctor
    GameResult() :
        gameType_(GT_UNKNOWN) {}

    bool isValid() const {
        if (isMatchGame(gameType_)) {
            return matchGameResult_.isValid();
        }
        else if (isCooperativeGame(gameType_)) {
            return cooperativeGameResult_.isValid();
        }
        //WN_ASSERT(false && "�߸��� ���� Ÿ��");
        return false;
    }
};

} // namespace skynet

#endif // !defined(INCLUDED_POPORUGAMESCOREINFO)
