/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// warnet_export.h
//  - DLL export 관련 매크로 정의
//
#if !defined(INCLUDED_WARNET_EXPORT)
#define INCLUDED_WARNET_EXPORT

#ifdef WARNET_DLL
#    ifdef WARNET_EXPORTS
#        define WARNET_API __declspec(dllexport)
#    else
#        define WARNET_API __declspec(dllimport)
#    endif
// struct 'XXX' needs to have dll-interface to be used by clients of class 'XX'
#    pragma warning ( disable: 4251 )

//non dll-interface class 'XXX' used as base for dll-interface class 'XXX'
#    pragma warning ( disable: 4275 )
#else
#    define WARNET_API
#endif // WARNET_DLL

#endif // !INCLUDED_WARNET_EXPORT
