/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// PacketCipher.h
#if !defined(INCLUDED_PACKETCIPHER)
#define INCLUDED_PACKETCIPHER

#include <ace/Message_Block.h>

namespace skynet
{

/**
* @struct PacketCipher
*
* 패킷 암호화.
*/
struct WARNET_API PacketCipher
{
    enum { DEFAULT_SECURITY_SEED = 0 };

    static ACE_UINT16 getFingerPrint(ACE_UINT32 securityKey);

    static ACE_UINT32 getCheckSum(const char* buffer, size_t bufferSize,
        ACE_UINT16 fingerPrint);

    static ACE_UINT32 getCheckSum(const ACE_Message_Block& buffer,
            ACE_UINT16 fingerPrint) {
        return getCheckSum(buffer.rd_ptr(), buffer.length(), fingerPrint);
    }

    static void encrypt(const char* begin, const char* end, char* target,
        ACE_UINT32 securityKey);

    static void decrypt(const char* begin, const char* end, char* target,
        ACE_UINT32 securityKey);

    static void encrypt(ACE_Message_Block& buffer, ACE_UINT32 securityKey) {
        encrypt(buffer.rd_ptr(), buffer.wr_ptr(), buffer.rd_ptr(),
            securityKey);
    }

    static void decrypt(ACE_Message_Block& buffer, ACE_UINT32 securityKey) {
        decrypt(buffer.rd_ptr(), buffer.wr_ptr(), buffer.rd_ptr(),
            securityKey);
    }

};

} // namespace skynet

#endif // !INCLUDED_PACKETCIPHER
