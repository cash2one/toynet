/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// CooperativeGameRankingPointSupplier.h
#if !defined(INCLUDED_COOPERATIVEGAMERANKINGPOINTOSUPPLIER)
#define INCLUDED_COOPERATIVEGAMERANKINGPOINTOSUPPLIER

#include <skynet/arcade/ArcadeGameInfo.h>
#include <skynet/arcade/ArcadeRewardInfo.h>

namespace skynet
{

/**
 * @class CooperativeGameRankingPointSupplier
 *
 * ���� ���� ��ŷ ����Ʈ ���
 */
class WARNET_API CooperativeGameRankingPointSupplier
{
public:
    CooperativeGameRankingPointSupplier(GameType gameType) :
        gameType_(gameType) {}

    /**
     * ��ŷ ����Ʈ�� ���Ѵ�
     * @param clearedStage Ŭ������ �������� ��(> 0)
     * @param earnedScore ȹ�� ����
     * @return ��ŷ ����Ʈ
     */
    RankingPoint getRankingPoint(int clearedStage, int earnedScore) const {
        return getStageReward(clearedStage) + getScoreReward(earnedScore);
    }

    /// �������� ����
    RankingPoint getStageReward(int clearedStage) const;

    /// ���� ����
    RankingPoint getScoreReward(int earnedScore) const;
protected:
    GameType getGameType() const {
        return gameType_;
    }
private:
    GameType gameType_;
};

} // namespace skynet

#endif // !defined(INCLUDED_COOPERATIVEGAMERANKINGPOINTOSUPPLIER)
