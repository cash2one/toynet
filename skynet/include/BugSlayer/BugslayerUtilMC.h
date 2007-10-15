//-----------------------------------------------------------------------
// Debugging Applications for Microsoft .NET and Microsoft Windows
// Copyright ?1997-2003 John Robbins -- All rights reserved.
//-----------------------------------------------------------------------
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_SYSTEM                  0x0
#define FACILITY_STUBS                   0x3
#define FACILITY_RUNTIME                 0x2
#define FACILITY_IO_ERROR_CODE           0x4


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: MSG_BSU_ERROR_SUCCESS
//
// MessageText:
//
//  %1
//
#define MSG_BSU_ERROR_SUCCESS            ((DWORD)0x20020010L)

//
// MessageId: MSG_BSU_ERROR_INFORMATIONAL
//
// MessageText:
//
//  %1
//
#define MSG_BSU_ERROR_INFORMATIONAL      ((DWORD)0x60020020L)

//
// MessageId: MSG_BSU_ERROR_WARNING
//
// MessageText:
//
//  %1
//
#define MSG_BSU_ERROR_WARNING            ((DWORD)0xA0020030L)

//
// MessageId: MSG_BSU_ERROR_ERROR
//
// MessageText:
//
//  %1
//
#define MSG_BSU_ERROR_ERROR              ((DWORD)0xE0020040L)

