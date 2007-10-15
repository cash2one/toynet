/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ArcadeRelayInfo.h
//  - Arcade 관전 관련 정보
//
#if !defined(INCLUDED_POPORURELAYINFO)
#define INCLUDED_POPORURELAYINFO

#include "ArcadeDataLimits.h"
#include <skynet/Wdr.h>
#include <boost/static_assert.hpp>

namespace skynet
{

/// 관전 데이터(초 당 키 프레임)
typedef Wdr::Long KeyFrame[DL_RELAY_KEY_COUNT_PER_FRAME];

/// 키 프레임 일련 번호(동기화를 맞추기 위해서 필요함)
typedef Wdr::Long KeyFrameNumber;
enum KeyFrameNumber_t
{
    INVALID_KEYFRAMENUMBER = -1,

    KFN_START = 0 ///< 첫번째 키프레임 일련 번호
};

inline bool isValidKeyFrameNumber(KeyFrameNumber keyFrameNumber)
{
    return keyFrameNumber > INVALID_KEYFRAMENUMBER;
}

BOOST_STATIC_ASSERT(sizeof(KeyFrame) ==
    (DL_RELAY_KEY_COUNT_PER_FRAME * DL_RELAY_KEY_SIZE));
BOOST_STATIC_ASSERT((DL_RELAY_KEY_COUNT_PER_FRAME * DL_RELAY_KEY_SIZE) ==
    240);

} // namespace skynet

#endif // !defined(INCLUDED_POPORURELAYINFO)
