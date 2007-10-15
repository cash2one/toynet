/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ArcadeChannelInfo.h
//  - Arcade ä�� ����
//
#if !defined(INCLUDED_POPORUCHANNELINFO)
#define INCLUDED_POPORUCHANNELINFO

#include "ArcadeGameInfo.h"
#include "ArcadeCommonInfo.h"
#pragma warning ( push )
#pragma warning ( disable: 4702 )
#include <map>
#include <vector>
#pragma warning ( pop )

namespace skynet
{

/// ä�� ���� ����
enum ChannelType
{
    CT_UNKNOWN = 0,
    
    CT_BEGINNER, ///< �ʺ� ä��(1)
    CT_FREE, ///< ���� ä��(2)
    CT_MASTER, ///< ��� ä��(3)
    CT_MIDDLE, ///< �߼� ä��(4)
    CT_ENTRANCE,///�Թ�
    CT_GENERAL,///�Ϲ�

    CT_TOURNAMENT = 50, ///< ��ȸ ä��(50)
	CT_PADDING	  = 100,
    CT_LAST
};

inline bool isValidChannelType(ChannelType ct)
{
    return (CT_UNKNOWN < ct) && (ct < CT_LAST);
}


/// ä�� �ĺ���
typedef int ChannelId;
typedef std::vector<ChannelId> ChannelIds;

/**
* @struct Channel
*
* ä�� ���� ����
*/
struct Channel
{
    ChannelId channelId_; ///< ä�� �ĺ���
    ChannelType type_; ///< ä�� ����
    std::string name_;
    AddressPair addressPair_; ///< ä�� ���� �ּ�
    Wdr::UShort maxUserCount_; ///< ä�� �ִ� ��� ������ ��
    Wdr::UShort curUserCount_; ///< ���� ä�� ������ ��
    bool enabled_; ///< ä�� ������ �����Ѱ�?

    Channel() :
        channelId_(-1),
        type_(CT_UNKNOWN),
        maxUserCount_(0),
        curUserCount_(0),
        enabled_(false) {}
};

/// ä�� ���� ���
typedef std::vector<Channel> Channels;

/// ä���� ����
typedef std::map<GameGenre, Channels> ChannelGroups;

} // namespace skynet

#endif // !defined(INCLUDED_POPORUCHANNELINFO)
