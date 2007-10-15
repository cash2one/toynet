/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ExperiencePointTable.h
#if !defined(INCLUDED_EXPERIENCEPOINTTABLE)
#define INCLUDED_EXPERIENCEPOINTTABLE

#include <skynet/arcade/ArcadeCharacterCardInfo.h>

namespace skynet
{

/**
 * @class ExperiencePointTable
 *
 * 경험치, 레벨, 최대 코인 테이블
 */
class WARNET_API ExperiencePointTable
{
public:
    /// 경험치에 해당하는 레벨을 얻는다
    CardLevel getCardLevel(size_t exp) const;

    /// 레벨당 최저 경험치
    /// @param level DL_MAX_CARD_LEVEL 보다 클 경우
    ///        DL_MAX_CARD_LEVEL로 설정
    size_t getMinExpByCardLevel(CardLevel level) const;

    /// 레벨에 해당하는 최대 코인 수를 얻는다
    size_t getMaxCoin(CardLevel level) const;
};

} // namespace skynet

#endif // !defined(INCLUDED_EXPERIENCEPOINTTABLE)
