/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
 * ���� ���� ���� ���
 */
class WARNET_API CooperativeGameRupoSupplier
{
public:
    CooperativeGameRupoSupplier(GameType gameType) :
        gameType_(gameType) {}

    /**
     * ������ ���Ѵ�
     * @param clearedStage Ŭ������ �������� ��(> 0)
     * @param earnedScore ȹ�� ����
     * @return ����
     */
    Rupo getRupo(size_t clearedStage, size_t earnedScore) const {
        return getStageReward(clearedStage) + getScoreReward(earnedScore);
    }

    /// �������� ����
    Rupo getStageReward(size_t clearedStage) const;

    /// ���� ����
    Rupo getScoreReward(size_t earnedScore) const;
private:
    GameType gameType_;
};

} // namespace skynet

#endif // !defined(INCLUDED_COOPERATIVEGAMERUPOSUPPLIER)
