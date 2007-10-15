/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// CooperativeGameRupoSupplier.h
#if !defined(INCLUDED_COOPERATIVEGAMERUPOSUPPLIER)
#define INCLUDED_COOPERATIVEGAMERUPOSUPPLIER

#include <skynet/arcade/ArcadeRewardInfo.h>
#include <skynet/arcade/ArcadeGameInfo.h>

namespace skynet
{

/**
 * @class CooperativeGameRupoSupplier
 *
 * 협력 게임 루포 계산
 */
class WARNET_API CooperativeGameRupoSupplier
{
public:
    CooperativeGameRupoSupplier(GameType gameType) :
        gameType_(gameType) {}

    /**
     * 루포를 구한다
     * @param clearedStage 클리어한 스테이지 수(> 0)
     * @param earnedScore 획득 점수
     * @return 루포
     */
    Rupo getRupo(size_t clearedStage, size_t earnedScore) const {
        return getStageReward(clearedStage) + getScoreReward(earnedScore);
    }

    /// 스테이지 보상
    Rupo getStageReward(size_t clearedStage) const;

    /// 점수 보상
    Rupo getScoreReward(size_t earnedScore) const;
private:
    GameType gameType_;
};

} // namespace skynet

#endif // !defined(INCLUDED_COOPERATIVEGAMERUPOSUPPLIER)
