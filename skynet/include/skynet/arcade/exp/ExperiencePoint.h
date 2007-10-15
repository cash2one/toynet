/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ExperiencePoint.h
#if !defined(INCLUDED_EXPERIENCEPOINT)
#define INCLUDED_EXPERIENCEPOINT

#include "ExperiencePointTable.h"
#include <skynet/arcade/ArcadeRewardInfo.h>

namespace skynet
{

class GameMode;

/**
 * @class ExperiencePoint
 *
 * ���� ��� ���
 */
class WARNET_API ExperiencePoint
{
public:
    ExperiencePoint(const GameMode& gameMode) :
        gameMode_(gameMode) {}
    virtual ~ExperiencePoint() {}

    /// ȹ�� ����
    virtual Coin getEarnedCoin() const;

    /// ��� ����
    Coin getInitialCoin() const;

    const GameMode& getGameMode() const {
        return gameMode_;
    }
    const ExperiencePointTable& getExpTable() const {
        return expTable_;
    }
private:
    const GameMode& gameMode_;

    const ExperiencePointTable expTable_;
};

} // namespace skynet

#endif // !defined(INCLUDED_EXPERIENCEPOINT)
