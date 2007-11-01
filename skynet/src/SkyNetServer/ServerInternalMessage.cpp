/**

* Copyright (c) 

*

* @author 

*/

// ServerInternalMessage.cpp

#include "stdafx.h"

#include <skynet/server/ServerInternalMessage.h>

#include <skynet/detail/WdrUtil.h>



namespace skynet

{



///////////////////////////////////////////////////////////////////////////////

// LogoutMessage class



bool LogoutMessage::readBody(Wdr::InputWdr& wdr)

{

    return wdr.isGood();

}





bool LogoutMessage::writeBody(Wdr::OutputWdr& wdr) const

{

    return wdr.isGood();

}





size_t LogoutMessage::getBodySize() const

{

    return 0;

}



} // namespace skynet

