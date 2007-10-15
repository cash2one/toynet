/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
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
/// 서버 종류
enum ServerType
{
    stUnknown = -1,
    stCenterServer = 1, ///<센터서버

    stLoginServer, ///< 로그인 서버
    stGameServer, ///< 게임 서버
    stRelayServer, ///< 관전 서버
    stForwardServer, ///< 포워딩 서버

    stLast
};

inline bool isValidServerType(ServerType st)
{
    return (stUnknown < st) && (st < stLast);
}

} // namespace skynet

#endif // !defined(INCLUDED_SERVERTYPE)
