/**

* Copyright (c) 

*

* @author 

*/

// SecureClientMessageAdaptor.cpp

#include "stdafx.h"

#include "SecureClientMessageAdaptor.h"

#include <skynet/detail/EventRepository.h>

#include <skynet/detail/MessageAdaptor.h>

#include <skynet/security/PacketCipher.h>

#include <skynet/detail/Logging.h>

#include <ace/SOCK_Stream.h>

#include <ace/INET_Addr.h>



namespace skynet

{



SecureClientMessageAdaptor::SecureClientMessageAdaptor(

        MessageAdaptor* adaptor) :

    ClientMessageAdaptor(adaptor)

{

}





bool SecureClientMessageAdaptor::setFirstSecureSeed(ACE_HANDLE handle)

{

    ACE_INET_Addr addr;

    ACE_SOCK_Stream tempStream(handle);

    tempStream.get_remote_addr(addr);



    ACE_UINT32 firstSecuritySeed =

        static_cast<ACE_UINT32>(addr.get_port_number());

    //firstSecuritySeed = ntohl(firstSecuritySeed);

    firstSecuritySeed = (firstSecuritySeed << 2) + firstSecuritySeed;

    getMessageAdaptor().setSecuritySeed(firstSecuritySeed);



    return true;

}



} // namespace skynet

