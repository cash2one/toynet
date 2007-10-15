/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
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
 * 협력 게임 랭킹 포인트 계산
 */
class WARNET_API CooperativeGameRankingPointSupplier
{
public:
    CooperativeGameRankingPointSupplier(GameType gameType) :
        gameType_(gameType) {}

    /**
     * 랭킹 포인트를 구한다
     * @param clearedStage 클리어한 스테이지 수(> 0)
     * @param earnedScore 획득 점수
     * @return 랭킹 포인트
     */
    RankingPoint getRankingPoint(int clearedStage, int earnedScore) const {
        return getStageReward(clearedStage) + getScoreReward(earnedScore);
    }

    /// 스테이지 보상
    RankingPoint getStageReward(int clearedStage) const;

    /// 점수 보상
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
