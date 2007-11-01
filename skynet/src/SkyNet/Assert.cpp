/**
* Copyright (c) 
*
* @author 
*/
#include "stdafx.h"
#include <skynet/detail/Assert.h>
#include <skynet/detail/Logging.h>

namespace skynet
{

#ifdef NDEBUG

void WarNetAssert(const char* expr, const char* filename, size_t lineno)
{
    WN_LOG_ERROR4("ASSERT FAILED: Why? %s, Where? %s:%d\n",
        expr, filename, lineno);
}

#endif

} // namespace skynet
