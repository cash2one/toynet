/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
 * ���� ������ ���� ��� ���
 */
class WARNET_API CooperativeExperiencePoint : public ExperiencePoint
{
public:
    CooperativeExperiencePoint(GameType gameType, const GameMode& gameMode) :
        ExperiencePoint(gameMode),
        gameType_(gameType) {}

    /**
     * ����ġ�� ���Ѵ�
     * @param clearedStage Ŭ������ �������� ��(> 0)
     * @param earnedScore ȹ�� ����
     * @return ����ġ
     */
    ExpPoint getExpPoint(size_t clearedStage, size_t earnedScore) const {
        return getStageReward(clearedStage) + getScoreReward(earnedScore);
    }

    /// �������� ����
    ExpPoint getStageReward(size_t clearedStage) const;

    /// ���� ����
    ExpPoint getScoreReward(size_t earnedScore) const;
private:
    GameType gameType_;
};

} // namespace skynet

#endif // !defined(INCLUDED_COOPERATIVEEXPERIENCEPOINT)
