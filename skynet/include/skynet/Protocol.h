/**
* Copyright (c) 
*
* @author
*/
// Protocol.h
// - 패킷 관련 상수 선언
#if !defined(INCLUDED_PROTOCOL)
#define INCLUDED_PROTOCOL

namespace skynet
{

/**
 * 메세지(패킷) 타입.
 * - C2S: client to server
 * - S2C: server to client
 * - S2S: server to server
 */
typedef int MessageType;

enum MessageType_t
{
    /// 잘못된 메세지 타입
    MT_UNKNOWN = -1,

    /// 테스트용
    MT_TEST_START = 100,

    /// 서버 공통 메세지
    MT_C_START = 5000,

    /// 서버 내부 사용
    MT_SI_START = 10000,

    MT_SERVER_DEFINED = 12000,
};

/// 테스트용 메세지 타입
enum TestMessageType_t
{
    /// 테스트용 메세지 타입
    MT_DUMMY = (MT_TEST_START + 10),
    /// 테스트용 메세지 타입
    MT_ECHO = (MT_TEST_START + 20)
};

} // namespace skynet

#endif // !defined(INCLUDED_PROTOCOL)
