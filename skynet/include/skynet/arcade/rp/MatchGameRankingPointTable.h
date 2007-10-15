/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
 * ���� ���� ��ŷ ����Ʈ ��꿡 �ʿ��� ��ġ ���̺�
 */
class WARNET_API MatchGameRankingPointTable : public boost::noncopyable
{
public:
    /// PDC(Point Difference Coefficient; ��ŷ ����Ʈ ���̿� ���� ���) ��
    /// - �׽�Ʈ�ϱ� ���� ������ ����
    struct WARNET_API PdcValue {
        int highRpDiffBasis_; ///< ��ŷ����Ʈ ���̿� ���� ���� ���ذ�
        int highWinFactor_; ///< �̰��� ��� ���� ���ذ��� ��� * 100
        int highLoseFactor_; ///< ���� ��� ���� ���ذ��� ��� * 100

        int lowRpDiffBasis_; ///< ��ŷ����Ʈ ���̿� ���� ���� ���ذ�
        int lowWinFactor_; ///< �̰��� ��� ���� ���ذ��� ��� * 100
        int lowLoseFactor_; ///< ���� ��� ���� ���ذ��� ��� * 100

        RankingPoint rpDiff_; ///< ��ŷ ����Ʈ ����

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

    /// �¸����� ����� ��ŷ ����Ʈ�� ���Ѵ�
    virtual RankingPoint computeRpAtWin(RankingPoint myRp, RankingPoint otherRp,
        GameRoundStage stage = grsFinal) const;

    /// ���� ����� ��ŷ ����Ʈ�� ���Ѵ�
    virtual RankingPoint computeRpAtLose(RankingPoint myRp, RankingPoint otherRp,
        GameRoundStage stage = grsFinal) const;

//private: // for Test
    /// �¸����� ���� MP(Match Point; ���� ����Ʈ)
    int getMpAtWin(RankingPoint rp) const;

    /// �й����� ���� MP(Match Point; ���� ����Ʈ)
    int getMpAtLose(RankingPoint rp) const;

    /// PDC ���� ������ ��´�
    PdcValue getPdcValue(RankingPoint rpd) const;

    /// MDC(Mode Difference Coefficient; ���� ��忡 ���� ���)�� ��´�
    /// - 10��: 0.1 -> 10
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
