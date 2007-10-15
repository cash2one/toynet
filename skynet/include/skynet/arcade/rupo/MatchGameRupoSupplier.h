/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// MatchGameRupoSupplier.h
#if !defined(INCLUDED_MATCHGAMERUPOSUPPLIER)
#define INCLUDED_MATCHGAMERUPOSUPPLIER

#include <skynet/arcade/ArcadeRewardInfo.h>
#include <skynet/arcade/ArcadeGameRoomInfo.h>
#include <skynet/arcade/ArcadeChannelInfo.h>
#include <boost/utility.hpp>
namespace skynet
{

namespace
{
struct PdcValue;
}

class GameMode;

/**
 * @class MatchGameRupoSupplier
 *
 * 대전게임 루포 계산기
 */
class WARNET_API MatchGameRupoSupplier
{
public:
    MatchGameRupoSupplier(GameType gameType, const GameMode& gameMode);

    virtual Rupo getRupoAtWin(RankingPoint yourRp, RankingPoint myRp,
        GameRoundStage stage = grsQuarterFinal) const {
        return static_cast<Rupo>(
            getDefaultRupoAtWin(stage) * getPdc(yourRp - myRp));
    }
    virtual Rupo getRupoAtLose(RankingPoint yourRp, RankingPoint myRp,
        GameRoundStage stage = grsQuarterFinal) const {
        return static_cast<Rupo>(
            getDefaultRupoAtLose(stage) * getPdc(yourRp - myRp));
    }
    virtual Rupo getRupoAtDraw(RankingPoint yourRp, RankingPoint myRp,
        GameRoundStage stage = grsQuarterFinal) const {
        return static_cast<Rupo>(
            getDefaultRupoAtDraw(stage) * getPdc(yourRp - myRp));
    }

    Rupo getTournamentRupo(RankingPoint yourRp, RankingPoint myRp,
        TournamentResult tourneyResult, size_t players) const {
        WN_ASSERT(isValidTournamentResult(tourneyResult));
        WN_ASSERT((6 <= players) && (8 >= players));
        return static_cast<Rupo>(
            getDefaultTournamentRupo(tourneyResult, players) * 
            getPdc(yourRp - myRp));
    }
protected:
    double getPdc(RankingPoint diffRp) const;
    Rupo getDefaultRupoAtWin(GameRoundStage stage) const;
    Rupo getDefaultRupoAtLose(GameRoundStage stage) const;
    Rupo getDefaultRupoAtDraw(GameRoundStage stage) const;
    Rupo getDefaultTournamentRupo(TournamentResult tourneyResult,
        size_t players) const;
protected:
    GameType gameType_;
    const GameMode& gameMode_;
};


class WARNET_API KofFreeChMatchGameRupoSupplier : public MatchGameRupoSupplier
{
public:
    KofFreeChMatchGameRupoSupplier(GameType gameType, const GameMode& gameMode):
        MatchGameRupoSupplier(gameType, gameMode) {}
private:
    virtual Rupo getRupoAtWin(RankingPoint yourRp, RankingPoint myRp,
        GameRoundStage stage = grsQuarterFinal) const {
            return static_cast<Rupo>(
                MatchGameRupoSupplier::getRupoAtWin(yourRp, myRp, stage)/2);
        }
    virtual Rupo getRupoAtLose(RankingPoint yourRp, RankingPoint myRp,
        GameRoundStage stage = grsQuarterFinal) const {
            return static_cast<Rupo>(
                MatchGameRupoSupplier::getRupoAtLose(yourRp, myRp, stage)/2);
    }
    virtual Rupo getRupoAtDraw(RankingPoint yourRp, RankingPoint myRp,
        GameRoundStage stage = grsQuarterFinal) const {
            return static_cast<Rupo>(
                MatchGameRupoSupplier::getRupoAtDraw(yourRp, myRp, stage)/2);
    }

};

class WARNET_API KofMidChMatchGameRupoSupplier : public MatchGameRupoSupplier
{
public:
    KofMidChMatchGameRupoSupplier(GameType gameType, const GameMode& gameMode):
        MatchGameRupoSupplier(gameType, gameMode) {}
private:
    virtual Rupo getRupoAtWin(RankingPoint yourRp, RankingPoint myRp,
        GameRoundStage stage = grsQuarterFinal) const {
            return static_cast<Rupo>(
                MatchGameRupoSupplier::getRupoAtWin(yourRp, myRp, stage) * 1.1);
        }
    virtual Rupo getRupoAtLose(RankingPoint yourRp, RankingPoint myRp,
        GameRoundStage stage = grsQuarterFinal) const {
            return static_cast<Rupo>(
                MatchGameRupoSupplier::getRupoAtLose(yourRp, myRp, stage));
    }
    virtual Rupo getRupoAtDraw(RankingPoint yourRp, RankingPoint myRp,
        GameRoundStage stage = grsQuarterFinal) const {
            return static_cast<Rupo>(
                MatchGameRupoSupplier::getRupoAtDraw(yourRp, myRp, stage) * 1.1);
    }

};


} // namespace skynet

#endif // !defined(INCLUDED_MATCHGAMERUPOSUPPLIER)
