/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author ±èº´¼ö <kcando@megaking.co.kr>
*/
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#if !defined(INCLUDED_STDAFX)
#define INCLUDED_STDAFX

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

#include <skynet/detail/SkyNetConfig.h>
#include <skynet/warnet_export.h>
#include <tchar.h>
#include <string>
#pragma warning ( push )
#pragma warning ( disable: 4702 )
#include <vector>
#include <list>
#include <hash_map>
#pragma warning ( pop )
#include <skynet/detail/Assert.h>

#include <boost/utility.hpp>

#pragma warning ( push )
#pragma warning ( disable: 4244 4267 4311 4312 )
#include <ace/ACE.h>
#include <ace/OS.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include <ace/Log_Msg.h>
#pragma warning ( pop )

#undef ACE_NLOGGING

// conditional expression is constant
#pragma warning ( disable: 4127 )

// copy constructor could not be generated
#pragma warning ( disable: 4511 )

//assignment operator could not be generated
#pragma warning ( disable: 4512 )

#endif // ! defined(INCLUDED_STDAFX)
