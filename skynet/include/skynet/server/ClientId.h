/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ClientId.h
#if !defined(INCLUDED_CLIENTID)
#define INCLUDED_CLIENTID

#pragma warning ( push )
#pragma warning ( disable: 4702 )
#include <vector>
#pragma warning ( pop )

namespace skynet
{

/// ClientAcceptor에서 사용하는 클라이언트 식별자
//typedef unsigned long ClientId;
enum ClientId { INVALID_CLIENTID = 0 };

inline bool isValidClientId(ClientId id)
{
    return INVALID_CLIENTID != id;
}

inline ClientId toClientId(int value)
{
    return static_cast<ClientId>(value);
}

inline ClientId incrementClientId(ClientId& id)
{
    id = toClientId(id + 1);
    return id;
}

typedef std::vector<ClientId> ClientIds;

} // namespace skynet

#endif // !defined(INCLUDED_CLIENTID)
