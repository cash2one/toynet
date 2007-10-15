/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
 * ����ġ, ����, �ִ� ���� ���̺�
 */
class WARNET_API ExperiencePointTable
{
public:
    /// ����ġ�� �ش��ϴ� ������ ��´�
    CardLevel getCardLevel(size_t exp) const;

    /// ������ ���� ����ġ
    /// @param level DL_MAX_CARD_LEVEL ���� Ŭ ���
    ///        DL_MAX_CARD_LEVEL�� ����
    size_t getMinExpByCardLevel(CardLevel level) const;

    /// ������ �ش��ϴ� �ִ� ���� ���� ��´�
    size_t getMaxCoin(CardLevel level) const;
};

} // namespace skynet

#endif // !defined(INCLUDED_EXPERIENCEPOINTTABLE)
