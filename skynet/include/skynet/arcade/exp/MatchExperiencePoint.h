/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// MatchExperiencePoint.h
#if !defined(INCLUDED_MATCHEXPERIENCEPOINT)
#define INCLUDED_MATCHEXPERIENCEPOINT

#include <skynet/arcade/exp/ExperiencePoint.h>
#include <skynet/arcade/ArcadeGameRoomInfo.h>

namespace skynet
{

/**
 * @class MatchExperiencePoint
 *
 * 1:1 ���� ������ ���� ��� ���
 */
 class WARNET_API MatchExperiencePoint : public ExperiencePoint
{
protected:
    enum ResultType { WIN, LOSE, DRAW };
public:
    MatchExperiencePoint(const GameMode& gameMode) :
        ExperiencePoint(gameMode),
        winnerExp_(0),
        loserExp_(0),
        tournamentEarnedCoin_(0),
        isTournamentReward_(false) {}

    /// ��� ���� ���
    virtual void calculate(bool isDrawed, Coin continueCount = 0,
        GameRoundStage stage = grsFinal);

    /// ��ʸ�Ʈ ���� ���
    virtual void calculate(TournamentResult tourneyResult, size_t players);

    virtual Coin getEarnedCoin() const;

    /// ������ ����ġ�� ��´�
    ExpPoint getWinnerExp() const {
        return winnerExp_;
    }

    /// ������ ����ġ�� ��´�
    ExpPoint getLoserExp() const {
        return loserExp_;
    }
protected:
    void setWinnerExp(ExpPoint exp) {
        winnerExp_ = exp;
    }
    void setLoserExp(ExpPoint exp) {
        loserExp_ = exp;
    }
private:
    ExpPoint winnerExp_;
    ExpPoint loserExp_;

    bool isTournamentReward_;
    Coin tournamentEarnedCoin_;
};

} // namespace skynet

#endif // !defined(INCLUDED_MATCHEXPERIENCEPOINT)
