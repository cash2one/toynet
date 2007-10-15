/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ArcadeGamescoreInfo.h
//  - Arcade 게임 전적 관련 정보
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
 * 대전 게임 승패 기록
 */
struct MatchGameRecord
{
    Wdr::UShort win_; ///< 승
    Wdr::UShort draw_; ///< 무
    Wdr::UShort lose_; ///< 패

    MatchGameRecord(Wdr::UShort win = 0, Wdr::UShort draw = 0,
            Wdr::UShort lose = 0) :
        win_(win),
        draw_(draw),
        lose_(lose) {}
};


/**
 * @struct CooperativeGameRecord
 *
 * 협력게임 기록
 */
struct CooperativeGameRecord
{
    Wdr::ULong score_; ///< 획득 점수 합
    Wdr::Octet stage_; ///< 클리어한 최대 스테이지
    Wdr::Octet continueCount_; ///< 컨티뉴 횟수(컨티뉴 모드에서만 사용)

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
* 게임 전적
*/
struct GameScore
{
    /// 대전 게임 (WN_ASSERT(isMatchGame(gameType_)))
    struct MatchGame : public MatchGameRecord
    {
        Wdr::UShort disconnect_; ///< 디스커넥트
		Wdr::UShort currentSvCount_; ///< 현재 연승(straight victories) 횟수
        Wdr::UShort maxSvCount_; ///< 최대 연승 횟수

        MatchGame() :
            disconnect_(0),
			currentSvCount_(0),
            maxSvCount_(0) {}
    };

    /// 협력 게임 (WN_ASSERT(isCooperativeGame(gameType_)))
    struct CooperativeGame : CooperativeGameRecord {};

    GameType gameType_;
    MatchGame matchGame_;
    CooperativeGame cooperativeGame_;

    RankingPoint rankingPoint_; ///< 랭킹 포인트
    Ranking ranking_; ///< 랭킹
    Rupo rewardedRupo_; ///< 보상받은 루포(게임 결과에서만 유효하다)
    GameClass gameClass_; ///< 게임 계급

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

/// 사용자별 전적 목록
typedef std::map<UserNo, GameScore> GameScores;

/// 게임별 전적 목록
typedef std::map<GameType, GameScore> GameScoreByGameTypes;

/// 대전 게임 모드별 전적 목록
typedef std::map<GameModeType, MatchGameRecord> MatchGameRecordByGameModes;

/// 협력 게임 모드별 전적 목록
typedef std::map<GameModeType, CooperativeGameRecord>
    CooperativeGameRecordByGameModes;

/**
 * @struct DetailedGameScore
 *
 * 상세한 게임 전적
 * - 모드별 전적 포함
 */
struct DetailedGameScore : public GameScore
{
    /// 대전게임의 게임모드별 전적
    MatchGameRecordByGameModes matchGameRecords_;

    /// 협력게임의 게임모드별 전적
    CooperativeGameRecordByGameModes cooperativeGameRecords_;
};

/// 게임별 전적 상세 목록(GameType = DetailedGameScore::gameType_)
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
* 1:1 대전 게임 결과(KOF, Samurai Shodown, NeoGeoCup, Baseball Stars).
* - playerA/B의 순서상의 구분은 없다.
* - 싱글 모드일 경우 playerA_와 playerB_를 같게 설정해야 하며, 승리했을 경우
*   winner_를 playerA_로 설정해야 하고, 패했을 경우 winner_를 INVALID_USERNO로
*   설정해야 한다.
* - 비긴 경우 winner_를 DRAWED_MATCH로 설정해야 한다
* - 둘 다 지는 경우는 없다
* - 1:1 대전에서 두 플레이의 게임 결과가 일치하지 않을 경우 INVALID_USERNO
*   (서버에서만 사용)
*/ 
struct MatchGameResult
{
    typedef Wdr::Octet Score;

    UserNo playerA_; ///< 1P UserNo
    UserNo playerB_; ///< 2P UserNo
    UserNo winner_; ///< 승리한 플레이어 UserNo
    Score scoreA_; ///< 1P 점수(축구, 야구에서만 사용)
    Score scoreB_; ///< 2P 점수(축구, 야구에서만 사용)
    bool lotted_; ///< 추첨에 의한 결과인가?

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

    /// 비긴 경우
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
* 협력 게임(메탈슬러그, 탑헌터) 게임 결과
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

    Players players_; ///< 플레이어별 점수

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
* 게임 결과
* - gameType_에 따라 어떤 필드가 사용될지 결정된다
* - if (isMatchGame(gameType_)) matchGameResult_ is valid
* - if (isCooperativeGame(gameType_)) cooperativeGameResult_ is valid
* - 값을 설정한 후 isValid() 함수를 호출해서 유효성을 검증하는 것이 좋다!!
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
        //WN_ASSERT(false && "잘못된 게임 타입");
        return false;
    }
};

} // namespace skynet

#endif // !defined(INCLUDED_POPORUGAMESCOREINFO)
