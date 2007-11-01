/**
* Copyright (c) 
*
* @author 
*/
// ClientAcceptor.cpp
//
#include "stdafx.h"
#include <skynet/server/ClientAcceptor.h>
#include <skynet/server/SkyNetService.h>
#include <skynet/server/GameClientSession.h>
#include <skynet/detail/Logging.h>
#include <ace/Guard_T.h>
#include <memory>

namespace skynet
{

ClientAcceptor::ClientAcceptor(WarNetService& service,
        MessageBuilderRepository& builderRepository, bool useCipher) :
    pool_(&service, builderRepository,
        GameClientSessionPool::DEFAULT_LOW_WARTER_MARK, useCipher)
{
}


ClientAcceptor::~ClientAcceptor()
{
    close();
}


// 당장은 필요 없음
//int ClientAcceptor::validate_connection(
//    const ACE_Asynch_Accept::Result& /*result*/,
//    const ACE_INET_Addr &remote, const ACE_INET_Addr& /*local*/)
//{
//    TCHAR address[MAXHOSTNAMELEN];
//    WN_LOG_INFO3(ACE_TEXT("Client is connecting from %s:%d...\n"),
//        remote.get_host_addr(address, MAXHOSTNAMELEN),
//        remote.get_port_number());
//    return 0;
//}


GameClientSession* ClientAcceptor::make_handler()
{
    return pool_.acquire();
}


void ClientAcceptor::close()
{
    cancel();

    ACE_OS::closesocket(handle());
    handle(ACE_INVALID_HANDLE);
}


void ClientAcceptor::release(GameClientSession* session)
{
    pool_.release(session);
}

} // namespace skynet
