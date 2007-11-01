/**

* Copyright 

*

* @author 

*/

// ServerMessageAdaptorFactory.cpp

#include "stdafx.h"

#include <skynet/detail/ServerMessageAdaptorFactory.h>

#include "SecureServerMessageAdaptor.h"

#include "NullServerMessageAdaptor.h"

#include <skynet/detail/MessageAdaptorFactory.h>

#include <skynet/detail/MessageAdaptor.h>



namespace skynet

{



ServerMessageAdaptor* ServerMessageAdaptorFactory::create(bool useCipher)

{

    MessageAdaptor* adaptor = MessageAdaptorFactory::create(useCipher);

    if (useCipher) {

        return new SecureServerMessageAdaptor(adaptor);

    }

    return new NullServerMessageAdaptor(adaptor);

}



} // namespace skynet

