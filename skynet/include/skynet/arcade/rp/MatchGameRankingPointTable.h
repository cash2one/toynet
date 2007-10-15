/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// MatchGameRankingPointTable.h
#if !defined(INCLUDED_MATCHGAMERANKINGPOINTTABLE)
#define INCLUDED_MATCHGAMERANKINGPOINTTABLE

#include <skynet/arcade/ArcadeRewardInfo.h>
#include <skynet/arcade/ArcadeGameRoomInfo.h>
#include <boost/utility.hpp>
#include <skynet/arcade/ArcadeChannelInfo.h>
namespace skynet
{

class GameMode;

/**
 * @class MatchGameRankingPointTable
 *
 * 대전 게임 랭킹 포인트 계산에 필요한 수치 테이블
 */
class WARNET_API MatchGameRankingPointTable : public boost::noncopyable
{
public:
    /// PDC(Point Difference Coefficient; 랭킹 포인트 차이에 의한 계수) 값
    /// - 테스트하기 위해 밖으로 빼냄
    struct WARNET_API PdcValue {
        int highRpDiffBasis_; ///< 랭킹포인트 차이에 따른 상위 기준값
        int highWinFactor_; ///< 이겼을 경우 상위 기준값의 계수 * 100
        int highLoseFactor_; ///< 졌을 경우 상위 기준값의 계수 * 100

        int lowRpDiffBasis_; ///< 랭킹포인트 차이에 따른 하위 기준값
        int lowWinFactor_; ///< 이겼을 경우 하위 기준값의 계수 * 100
        int lowLoseFactor_; ///< 졌을 경우 하위 기준값의 계수 * 100

        RankingPoint rpDiff_; ///< 랭킹 포인트 차이

        /// ctor
        PdcValue() :
            highRpDiffBasis_(0),
            highWinFactor_(0),
            highLoseFactor_(0),
            lowRpDiffBasis_(0),
            lowWinFactor_(0),
            lowLoseFactor_(0),
            rpDiff_(0) {}

        int getPdcAtWin() const;
        int getPdcAtLose() const;

        bool isValid() const {
            return highRpDiffBasis_ != lowRpDiffBasis_;
        }
    };
public:
    MatchGameRankingPointTable(GameType gameType, const GameMode& gameMode);

    /// 승리했을 경우의 랭킹 포인트를 구한다
    virtual RankingPoint computeRpAtWin(RankingPoint myRp, RankingPoint otherRp,
        GameRoundStage stage = grsFinal) const;

    /// 졌을 경우의 랭킹 포인트를 구한다
    virtual RankingPoint computeRpAtLose(RankingPoint myRp, RankingPoint otherRp,
        GameRoundStage stage = grsFinal) const;

//private: // for Test
    /// 승리했을 때의 MP(Match Point; 대전 포인트)
    int getMpAtWin(RankingPoint rp) const;

    /// 패배했을 때의 MP(Match Point; 대전 포인트)
    int getMpAtLose(RankingPoint rp) const;

    /// PDC 관련 값들을 얻는다
    PdcValue getPdcValue(RankingPoint rpd) const;

    /// MDC(Mode Difference Coefficient; 게임 모드에 따른 계수)를 얻는다
    /// - 10배: 0.1 -> 10
    int getMdc(GameRoundStage stage = grsFinal) const;
private:
    const GameType gameType_;
    const GameMode& gameMode_;
};

class KofFreeChMatchGameRankingPointTable :public MatchGameRankingPointTable
{
public:
    KofFreeChMatchGameRankingPointTable(GameType gameType,
        const GameMode& gameMode) : MatchGameRankingPointTable(gameType, gameMode)
    {

    }
    virtual RankingPoint computeRpAtWin(RankingPoint myRp, RankingPoint otherRp,
        GameRoundStage stage = grsFinal) const
    {
        RankingPoint earRp = 
            MatchGameRankingPointTable::computeRpAtWin(myRp, otherRp, stage) /2;
        return earRp;
    }

    virtual RankingPoint computeRpAtLose(RankingPoint myRp, RankingPoint otherRp,
        GameRoundStage stage = grsFinal) const
    {
        RankingPoint earRp = 
            MatchGameRankingPointTable::computeRpAtLose(myRp, otherRp, stage) ;
        return earRp;
    }
};

class MatchGameRankingPointTableFactroy
{
public:
    static MatchGameRankingPointTable* create(GameType gameType,
        const GameMode& gameMode, ChannelType ch, GameGenre gg)
    {
        ch;
        gg;
        //if(CT_FREE == ch && GG_KOF_SERIES == gg) {
        //    return new KofFreeChMatchGameRankingPointTable(gameType, gameMode); 
        //}
        return new MatchGameRankingPointTable(gameType, gameMode);
    }
};

} // namespace skynet

#endif // !defined(INCLUDED_MATCHGAMERANKINGPOINTTABLE)
