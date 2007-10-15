/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author ±èº´¼ö <kcando@megaking.co.kr>
*/
// MatchExperiencePointFactory.h
#if !defined(INCLUDED_MATCHEXPERIENCEPOINTFACTORY)
#define INCLUDED_MATCHEXPERIENCEPOINTFACTORY

#include "MatchExperiencePoint.h"

namespace skynet
{

/**
 * @class MatchExperiencePointFactory
 */
struct WARNET_API MatchExperiencePointFactory
{
    static MatchExperiencePoint* create(GameType gameType,
        const GameMode& gameMode);
};

} // namespace skynet

#endif // !defined(INCLUDED_MATCHEXPERIENCEPOINTFACTORY)
