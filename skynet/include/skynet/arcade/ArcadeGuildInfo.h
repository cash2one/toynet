/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// PoporuGuildInfo.h
//  - Arcade 길드 정보
//
#if !defined(INCLUDED_POPORUGUILDINFO)
#define INCLUDED_POPORUGUILDINFO

#include <string>

namespace skynet
{

/// 길드에서의 직책
enum GuildDuty
{
    GD_UNKNOWN = 255,

    GD_MASTER = 0, ///< 길드장
    GD_OPERATOR, ///< 운영자
    GD_FULL_MEMBER, ///< 정회원
};


/**
 * @struct GuildInfo
 *
 * 길드 정보
 */
struct GuildInfo
{
    std::string guildName_;
    GuildDuty guildDuty_;

    GuildInfo(const std::string& guildName = "",
            GuildDuty guildDuty = GD_UNKNOWN) :
        guildName_(guildName),
        guildDuty_(guildDuty) {}
};

} // namespace skynet

#endif // !defined(INCLUDED_POPORUGUILDINFO)
