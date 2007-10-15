/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// PoporuGuildInfo.h
//  - Arcade ��� ����
//
#if !defined(INCLUDED_POPORUGUILDINFO)
#define INCLUDED_POPORUGUILDINFO

#include <string>

namespace skynet
{

/// ��忡���� ��å
enum GuildDuty
{
    GD_UNKNOWN = 255,

    GD_MASTER = 0, ///< �����
    GD_OPERATOR, ///< ���
    GD_FULL_MEMBER, ///< ��ȸ��
};


/**
 * @struct GuildInfo
 *
 * ��� ����
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
