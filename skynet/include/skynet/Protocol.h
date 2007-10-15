/**
* Copyright (c) 
*
* @author
*/
// Protocol.h
// - ��Ŷ ���� ��� ����
#if !defined(INCLUDED_PROTOCOL)
#define INCLUDED_PROTOCOL

namespace skynet
{

/**
 * �޼���(��Ŷ) Ÿ��.
 * - C2S: client to server
 * - S2C: server to client
 * - S2S: server to server
 */
typedef int MessageType;

enum MessageType_t
{
    /// �߸��� �޼��� Ÿ��
    MT_UNKNOWN = -1,

    /// �׽�Ʈ��
    MT_TEST_START = 100,

    /// ���� ���� �޼���
    MT_C_START = 5000,

    /// ���� ���� ���
    MT_SI_START = 10000,

    MT_SERVER_DEFINED = 12000,
};

/// �׽�Ʈ�� �޼��� Ÿ��
enum TestMessageType_t
{
    /// �׽�Ʈ�� �޼��� Ÿ��
    MT_DUMMY = (MT_TEST_START + 10),
    /// �׽�Ʈ�� �޼��� Ÿ��
    MT_ECHO = (MT_TEST_START + 20)
};

} // namespace skynet

#endif // !defined(INCLUDED_PROTOCOL)
