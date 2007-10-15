/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ArcadeRelayInfo.h
//  - Arcade ���� ���� ����
//
#if !defined(INCLUDED_POPORURELAYINFO)
#define INCLUDED_POPORURELAYINFO

#include "ArcadeDataLimits.h"
#include <skynet/Wdr.h>
#include <boost/static_assert.hpp>

namespace skynet
{

/// ���� ������(�� �� Ű ������)
typedef Wdr::Long KeyFrame[DL_RELAY_KEY_COUNT_PER_FRAME];

/// Ű ������ �Ϸ� ��ȣ(����ȭ�� ���߱� ���ؼ� �ʿ���)
typedef Wdr::Long KeyFrameNumber;
enum KeyFrameNumber_t
{
    INVALID_KEYFRAMENUMBER = -1,

    KFN_START = 0 ///< ù��° Ű������ �Ϸ� ��ȣ
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
