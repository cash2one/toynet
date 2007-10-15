/**

* Copyright (c) 

*

* @author 

*/

// MessageAdaptorFactory.cpp

#include "stdafx.h"

#include <skynet/detail/MessageAdaptorFactory.h>

#include "SecureMessageAdaptor.h"

#include "NullMessageAdaptor.h"



namespace skynet

{



MessageAdaptor* MessageAdaptorFactory::create(bool useCipher)

{

    if (useCipher) {

        return new SecureMessageAdaptor;

    }

    return new NullMessageAdaptor;

}





} // namespace skynet

