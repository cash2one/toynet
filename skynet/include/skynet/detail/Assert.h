/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
#if !defined(INCLUDED_ASSERT)
#define INCLUDED_ASSERT

// Release ��忡���� Assert�� �α��ϵ��� �Ѵ�.
#ifdef NDEBUG

#include <skynet/warnet_export.h>

namespace skynet
{

void WARNET_API WarNetAssert(const char* expr, const char* filename,
    size_t lineno);

} // namespace skynet

#define WN_ASSERT(exp) \
    (void)((exp) || (skynet::WarNetAssert(#exp, __FILE__, __LINE__), 0))

#else // NDEBUG

#include <cassert>

#define WN_ASSERT assert

#endif // NDEBUG

#endif // !defined(INCLUDED_ASSERT)
