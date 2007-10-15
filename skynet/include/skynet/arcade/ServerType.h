/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ServerType.h
//
#if !defined(INCLUDED_SERVERTYPE)
#define INCLUDED_SERVERTYPE

#ifdef _MSC_VER
#  pragma once
#endif // _MSC_VER
#include <vector>

namespace skynet
{
typedef int ServerId;

typedef std::vector<ServerId> ServerIds;
/// ���� ����
enum ServerType
{
    stUnknown = -1,
    stCenterServer = 1, ///<���ͼ���

    stLoginServer, ///< �α��� ����
    stGameServer, ///< ���� ����
    stRelayServer, ///< ���� ����
    stForwardServer, ///< ������ ����

    stLast
};

inline bool isValidServerType(ServerType st)
{
    return (stUnknown < st) && (st < stLast);
}

} // namespace skynet

#endif // !defined(INCLUDED_SERVERTYPE)
