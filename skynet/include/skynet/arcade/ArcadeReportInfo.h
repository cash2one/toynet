/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// PoporuReportInfo.h
//  - Arcade �Ű� ���� ����
//
#if !defined(INCLUDED_POPORUREPORTINFO)
#define INCLUDED_POPORUREPORTINFO

namespace skynet
{

/// �Ű� ����
enum ReportReason
{
    RR_UNKNOWN = 255,

    RR_START = 0,

    /// �������� ��������, �Ǵ� ��ų������� �ߴ�. (�Ű���� ����Ʈ)
    RR_BAD_MANNER = RR_START,
    /// �������� �弳, �Ǵ� ������ȭ�� �ߴ�
    RR_FUCK_YOU,
    /// ����༼�� �ϸ鼭 ���̷��� �ߴ�
    RR_FAKE_OPERATOR,
    /// �������� ����� �������� ä���� �����ߴ�
    RR_PAPERING,
    /// ��Ÿ
    RR_ETC,

    RR_BOUNDARY
};

inline bool isValidReportReason(ReportReason rr)
{
    return (RR_START <= rr) && (rr < RR_BOUNDARY);
}

} // namespace skynet

#endif // !defined(INCLUDED_POPORUREPORTINFO)
