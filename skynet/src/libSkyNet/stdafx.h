/**
* Copyright 
*
* @author 
*/
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#if !defined(INCLUDED_STDAFX)
#define INCLUDED_STDAFX

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

// You must include the #define _WIN32_DCOM preprocessor directive at the
// beginning of your code to be able to use CoInitializeEx.
#define _WIN32_DCOM

//BOOST Microsoft Visual C++ 6.0 (sp5, with and without STLport), 7.0, 7.1, 8.0. 
//Note: Boost does not support the non-standard "Safe" C++ Library shipping with Visual C++ 8.0, 
//which may result in many spurious warnings from Boost headers and other standards-conforming C++ code. 
//To suppress these warnings, define the macro _SCL_SECURE_NO_DEPRECATE. 
#define _SCL_SECURE_NO_DEPRECATE

#include <skynet/detail/SkyNetConfig.h>
#include <skynet/warnet_export.h>
#ifdef _WINDOWS
	#include <tchar.h>
#endif
#include <string>
#pragma warning ( push )
#pragma warning ( disable: 4702 )
#include <vector>
#include <list>
//#include <hash_map>
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
