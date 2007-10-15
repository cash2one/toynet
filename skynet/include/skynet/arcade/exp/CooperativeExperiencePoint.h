/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// CooperativeExperiencePoint.h
#if !defined(INCLUDED_COOPERATIVEEXPERIENCEPOINT)
#define INCLUDED_COOPERATIVEEXPERIENCEPOINT

#include <skynet/arcade/exp/ExperiencePoint.h>
#include <skynet/arcade/ArcadeGameRoomInfo.h>

namespace skynet
{

/**
 * @class CooperativeExperiencePoint
 *
 * 협력 형식의 게임 결과 계산
 */
class WARNET_API CooperativeExperiencePoint : public ExperiencePoint
{
public:
    CooperativeExperiencePoint(GameType gameType, const GameMode& gameMode) :
        ExperiencePoint(gameMode),
        gameType_(gameType) {}

    /**
     * 경험치를 구한다
     * @param clearedStage 클리어한 스테이지 수(> 0)
     * @param earnedScore 획득 점수
     * @return 경험치
     */
    ExpPoint getExpPoint(size_t clearedStage, size_t earnedScore) const {
        return getStageReward(clearedStage) + getScoreReward(earnedScore);
    }

    /// 스테이지 보상
    ExpPoint getStageReward(size_t clearedStage) const;

    /// 점수 보상
    ExpPoint getScoreReward(size_t earnedScore) const;
private:
    GameType gameType_;
};

} // namespace skynet

#endif // !defined(INCLUDED_COOPERATIVEEXPERIENCEPOINT)
