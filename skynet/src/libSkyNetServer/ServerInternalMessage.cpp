/**

* Copyright (c) 2004 Mega Enterprise Co., Ltd.

*

* @author ±èº´¼ö <kcando@megaking.co.kr>

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

