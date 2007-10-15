/**

* Copyright 

*

* @author 

*/

// SecureServerMessageAdaptor.cpp

#include "stdafx.h"

#include "SecureServerMessageAdaptor.h"

#include <skynet/detail/MessageAdaptor.h>

#include <skynet/security/PacketCipher.h>

#include <ace/SOCK_Stream.h>

#include <ace/INET_Addr.h>



namespace skynet

{



SecureServerMessageAdaptor::SecureServerMessageAdaptor(

        MessageAdaptor* adaptor) :

    ServerMessageAdaptor(adaptor)

{

}





bool SecureServerMessageAdaptor::setFirstSecureSeed(ACE_HANDLE handle)

{

    ACE_INET_Addr addr;

    ACE_SOCK_Stream tempStream(handle);

    tempStream.get_local_addr(addr);

    ACE_UINT32 firstSecuritySeed = addr.get_port_number();

    //firstSecuritySeed = htonl(firstSecuritySeed);

    firstSecuritySeed = (firstSecuritySeed << 2) + firstSecuritySeed;



    getMessageAdaptor().setSecuritySeed(firstSecuritySeed);



    return true;

}



} // namespace skynet

