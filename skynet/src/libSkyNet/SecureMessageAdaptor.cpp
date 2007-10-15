/**
* Copyright (c) 
*
* @author 
*/
// SecureMessageAdaptor.cpp
#include "stdafx.h"
#include "SecureMessageAdaptor.h"
#include <skynet/security/PacketCipher.h>
#include <skynet/detail/WdrUtil.h>
#include <skynet/detail/Logging.h>
#include <skynet/detail/Assert.h>

namespace skynet
{

SecureMessageAdaptor::SecureMessageAdaptor()
{
}


size_t SecureMessageAdaptor::getHeaderSize() const
{
    return Wdr::sizeLong + Wdr::sizeShort;

}


bool SecureMessageAdaptor::encrypt(ACE_Message_Block& block,
    Wdr::OutputWdr& header)
{
    const ACE_UINT32 securityKey = getEncryptSecurityKey();
    const ACE_UINT16 fingerPrint = PacketCipher::getFingerPrint(securityKey);
    const ACE_UINT32 checkSum = PacketCipher::getCheckSum(block, fingerPrint);

    PacketCipher::encrypt(block, securityKey);

    return header.write(static_cast<Wdr::ULong>(checkSum)) &&
        header.write(static_cast<Wdr::UShort>(fingerPrint));
}


bool SecureMessageAdaptor::decrypt(Wdr::InputWdr& packet)
{
    Wdr::ULong recvCheckSum;
    if (! packet.read(recvCheckSum))
        return false;
    Wdr::UShort recvFingerPrint;
    if (! packet.read(recvFingerPrint))
        return false;

    const ACE_UINT32 securityKey = getDecryptSecurityKey();

    const ACE_UINT16 fingerPrint = PacketCipher::getFingerPrint(securityKey);
    if (fingerPrint != recvFingerPrint) {
        WN_LOG_DEBUG(ACE_TEXT("Invalid FingerPrint\n"));
        return false;
    }

    PacketCipher::decrypt(packet.current(), packet.end(), packet.current(),
        securityKey);

    const ACE_UINT32 checkSum = PacketCipher::getCheckSum(packet.current(),
        packet.length(), fingerPrint);
    if (checkSum != recvCheckSum) {
        WN_LOG_DEBUG(ACE_TEXT("Invalid CheckSum\n"));
        return false;
    }

    return true;
}

} // namespace skynet
