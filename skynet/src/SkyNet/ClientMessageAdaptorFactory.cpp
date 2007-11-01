/**

* Copyright (c) 

*

* @author

*/

// ClientMessageAdaptorFactory.cpp

#include "stdafx.h"

#include <skynet/detail/ClientMessageAdaptorFactory.h>

#include "SecureClientMessageAdaptor.h"

#include "NullClientMessageAdaptor.h"

#include <skynet/detail/MessageAdaptorFactory.h>

#include <skynet/detail/MessageAdaptor.h>



namespace skynet

{



ClientMessageAdaptor* ClientMessageAdaptorFactory::create(bool useCipher)

{

    MessageAdaptor* adaptor = MessageAdaptorFactory::create(useCipher);

    if (useCipher) {

        return new SecureClientMessageAdaptor(adaptor);

    }

    return new NullClientMessageAdaptor(adaptor);

}





} // namespace skynet

