/**
*Copyright (c) 2004 MegaEnterpriseCo., Ltd.
*
* @author ±èº´¼ö <kcando@megaking.co.kr>
*/
// Logging.h : ACE utilities
//
#if !defined(INCLUDED_LOGGING)
#define INCLUDED_LOGGING

#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4267 4311 4312 )
#include <ace/Log_Msg.h>
#pragma warning ( pop )

// = Log-related Macros (excerpts from ACE)

#if defined (ACE_NLOGGING)
// = Debug
#    define WN_LOG_DEBUG(A) do {} while (0)
#    define WN_LOG_DEBUG2(A,B) do {} while (0)
#    define WN_LOG_DEBUG3(A,B,C) do {} while (0)
#    define WN_LOG_DEBUG4(A,B,C,D) do {} while (0)
#    define WN_LOG_DEBUG5(A,B,C,D,E) do {} while (0)
#    define WN_LOG_DEBUG6(A,B,C,D,E,F) do {} while (0)
#    define WN_LOG_DEBUG7(A,B,C,D,E,F,G) do {} while (0)
#    define WN_LOG_DEBUG8(A,B,C,D,E,F,G,H) do {} while (0)
#    define WN_LOG_DEBUG9(A,B,C,D,E,F,G,H,I) do {} while (0)
// = Info
#    define WN_LOG_INFO(A) do {} while (0)
#    define WN_LOG_INFO2(A,B) do {} while (0)
#    define WN_LOG_INFO3(A,B,C) do {} while (0)
#    define WN_LOG_INFO4(A,B,C,D) do {} while (0)
#    define WN_LOG_INFO5(A,B,C,D,E) do {} while (0)
#    define WN_LOG_INFO6(A,B,C,D,E,F) do {} while (0)
#    define WN_LOG_INFO6(A,B,C,D,E,F,G) do {} while (0)
// = Warning
#    define WN_LOG_WARNING(A) do {} while (0)
#    define WN_LOG_WARNING2(A,B) do {} while (0)
#    define WN_LOG_WARNING3(A,B,C) do {} while (0)
#    define WN_LOG_WARNING4(A,B,C,D) do {} while (0)
#    define WN_LOG_WARNING5(A,B,C,D,E) do {} while (0)
// = Error
#    define WN_LOG_ERROR(A) do {} while (0)
#    define WN_LOG_ERROR2(A,B) do {} while (0)
#    define WN_LOG_ERROR3(A,B,C) do {} while (0)
#    define WN_LOG_ERROR4(A,B,C,D) do {} while (0)
#    define WN_LOG_ERROR5(A,B,C,D,E) do {} while (0)
#    define WN_LOG_ERROR6(A,B,C,D,E,F) do {} while (0)
#    define WN_LOG_ERROR7(A,B,C,D,E,F,G) do {} while (0)
#    define WN_LOG_ERROR8(A,B,C,D,E,F,G,H) do {} while (0)
#else // defined (ACE_NLOGGING)
// = Debug
#    define WN_LOG_DEBUG(A) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, ACE_TEXT("DEBUG [%D] (%P|%t) ") A); \
    } while (0)
#    define WN_LOG_DEBUG2(A,B) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, ACE_TEXT("DEBUG [%D] (%P|%t) ") A,B); \
    } while (0)
#    define WN_LOG_DEBUG3(A,B,C) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, ACE_TEXT("DEBUG [%D] (%P|%t) ") A,B,C); \
    } while (0)
#    define WN_LOG_DEBUG4(A,B,C,D) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, ACE_TEXT("DEBUG [%D] (%P|%t) ") A,B,C,D); \
    } while (0)
#    define WN_LOG_DEBUG5(A,B,C,D,E) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, ACE_TEXT("DEBUG [%D] (%P|%t) ") A,B,C,D,E); \
    } while (0)
#    define WN_LOG_DEBUG6(A,B,C,D,E,F) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, ACE_TEXT("DEBUG [%D] (%P|%t) ") A,B,C,D,E,F); \
    } while (0)
#    define WN_LOG_DEBUG7(A,B,C,D,E,F,G) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, ACE_TEXT("DEBUG [%D] (%P|%t) ") A,B,C,D,E,F,G); \
    } while (0)
#    define WN_LOG_DEBUG8(A,B,C,D,E,F,G,H) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, ACE_TEXT("DEBUG [%D] (%P|%t) ") A,B,C,D,E,F,G,H); \
    } while (0)
#    define WN_LOG_DEBUG9(A,B,C,D,E,F,G,H,I) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_DEBUG, ACE_TEXT("DEBUG [%D] (%P|%t) ") A,B,C,D,E,F,G,H,I); \
    } while (0)
// = Info
#    define WN_LOG_INFO(A) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_INFO, ACE_TEXT("INFO [%D] (%P|%t) ") A); \
    } while (0)
#    define WN_LOG_INFO2(A,B) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_INFO, ACE_TEXT("INFO [%D] (%P|%t) ") A,B); \
    } while (0)
#    define WN_LOG_INFO3(A,B,C) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_INFO, ACE_TEXT("INFO [%D] (%P|%t) ") A,B,C); \
    } while (0)
#    define WN_LOG_INFO4(A,B,C,D) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_INFO, ACE_TEXT("INFO [%D] (%P|%t) ") A,B,C,D); \
    } while (0)
#    define WN_LOG_INFO5(A,B,C,D,E) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_INFO, ACE_TEXT("INFO [%D] (%P|%t) ") A,B,C,D,E); \
    } while (0)
#    define WN_LOG_INFO6(A,B,C,D,E,F) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_INFO, ACE_TEXT("INFO [%D] (%P|%t) ") A,B,C,D,E,F); \
    } while (0)
#    define WN_LOG_INFO7(A,B,C,D,E,F,G) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_INFO, ACE_TEXT("INFO [%D] (%P|%t) ") A,B,C,D,E,F,G); \
    } while (0)
// = Warning
#    define WN_LOG_WARNING(A) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_WARNING, ACE_TEXT("WARNING [%D] (%P|%t) ") A); \
    } while (0)

#    define WN_LOG_WARNING2(A,B) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_WARNING, ACE_TEXT("WARNING [%D] (%P|%t) ") A,B); \
    } while (0)

#    define WN_LOG_WARNING3(A,B,C) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_WARNING, ACE_TEXT("WARNING [%D] (%P|%t) ") A,B,C); \
    } while (0)

#    define WN_LOG_WARNING4(A,B,C,D) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_WARNING, ACE_TEXT("WARNING [%D] (%P|%t) ") A,B,C,D); \
    } while (0)

#    define WN_LOG_WARNING5(A,B,C,D,E) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_WARNING, ACE_TEXT("WARNING [%D] (%P|%t) ") A,B,C,D,E); \
    } while (0)
// = Error
#    define WN_LOG_ERROR(A) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_ERROR, ACE_TEXT("ERROR [%D] (%P|%t) ") A); \
    } while (0)
#    define WN_LOG_ERROR2(A,B) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_ERROR, ACE_TEXT("ERROR [%D] (%P|%t) ") A,B); \
    } while (0)
#    define WN_LOG_ERROR3(A,B,C) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_ERROR, ACE_TEXT("ERROR [%D] (%P|%t) ") A,B,C); \
    } while (0)
#    define WN_LOG_ERROR4(A,B,C,D) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_ERROR, ACE_TEXT("ERROR [%D] (%P|%t) ") A,B,C,D); \
    } while (0)
#    define WN_LOG_ERROR5(A,B,C,D,E) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_ERROR, ACE_TEXT("ERROR [%D] (%P|%t) ") A,B,C,D,E); \
    } while (0)
#    define WN_LOG_ERROR6(A,B,C,D,E,F) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_ERROR, ACE_TEXT("ERROR [%D] (%P|%t) ") A,B,C,D,E,F); \
    } while (0)
#    define WN_LOG_ERROR7(A,B,C,D,E,F,G) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_ERROR, ACE_TEXT("ERROR [%D] (%P|%t) ") A,B,C,D,E,F,G); \
    } while (0)
#    define WN_LOG_ERROR8(A,B,C,D,E,F,G,H) \
    do { \
    int __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance (); \
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log(LM_ERROR, ACE_TEXT("ERROR [%D] (%P|%t) ") A,B,C,D,E,F,G,H); \
    } while (0)
#endif

#endif // !defined(INCLUDED_LOGGING)
