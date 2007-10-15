/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// PoporuReportInfo.h
//  - Arcade 신고 관련 정보
//
#if !defined(INCLUDED_POPORUREPORTINFO)
#define INCLUDED_POPORUREPORTINFO

namespace skynet
{

/// 신고 사유
enum ReportReason
{
    RR_UNKNOWN = 255,

    RR_START = 0,

    /// 고의적인 게임종료, 또는 비매너행위를 했다. (신고사유 디폴트)
    RR_BAD_MANNER = RR_START,
    /// 고의적인 욕설, 또는 음란대화를 했다
    RR_FUCK_YOU,
    /// 운영자행세를 하면서 속이려고 했다
    RR_FAKE_OPERATOR,
    /// 고의적인 도배로 유저들의 채팅을 방해했다
    RR_PAPERING,
    /// 기타
    RR_ETC,

    RR_BOUNDARY
};

inline bool isValidReportReason(ReportReason rr)
{
    return (RR_START <= rr) && (rr < RR_BOUNDARY);
}

} // namespace skynet

#endif // !defined(INCLUDED_POPORUREPORTINFO)
