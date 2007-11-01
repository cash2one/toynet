/**
* Copyright 
*
* @author 
*/
// PacketCipher.cpp
#include "stdafx.h"
#include <skynet/security/PacketCipher.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>

namespace skynet
{

namespace
{

// left circular shift
inline ACE_Byte lcshift(ACE_Byte input, ACE_Byte shiftOperand)
{
    WN_ASSERT((0 <= shiftOperand) && (shiftOperand <= 7));

    return (input << shiftOperand) | (input >> (8 - shiftOperand));
}


// right circular shift
inline ACE_Byte rcshift(ACE_Byte input, ACE_Byte shiftOperand)
{
    WN_ASSERT((0 <= shiftOperand) && (shiftOperand <= 7));

    return (input >> shiftOperand) | (input << (8 - shiftOperand));
}


inline ACE_Byte xor_(ACE_Byte input, ACE_Byte xorOperand)
{
    return input ^ xorOperand;
}


// securityKey : Encryption seed값. 첫번째 1byte는 XorSeed로,
// 두번째 1byte는 ShiftSeed로 쓴다.
inline void getOperand(ACE_UINT32 securityKey, ACE_Byte& xorSeed,
    ACE_Byte& shiftSeed)
{
    const void *keyPtr = &securityKey;
    const ACE_Byte* key_array = static_cast<const ACE_Byte*>(keyPtr);
    xorSeed = key_array[0];
    shiftSeed = key_array[1] % 8;

    WN_ASSERT((0 <= shiftSeed) && (shiftSeed <= 7));
}


/**
* @class Encryptor
*
* function object
*/
template <class T>
class Encryptor
{
public:
    Encryptor(T xorOperand, T shiftOperand) :
        xorOperand_(xorOperand), shiftOperand_(shiftOperand) {}

    T operator ()(const T& elem) const {
        return xor_(lcshift(elem, shiftOperand_), xorOperand_);
    }
private:
    T xorOperand_;
    T shiftOperand_;
};

/**
* @class Decryptor
*
* function object
*/
template <class T>
class Decryptor
{
public:
    Decryptor(T xorOperand, T shiftOperand) :
        xorOperand_(xorOperand), shiftOperand_(shiftOperand) {}

    T operator ()(const T& elem) const {
        return rcshift(xor_(elem, xorOperand_), shiftOperand_);
    }
private:
    T xorOperand_;
    T shiftOperand_;
};

} // namespace


// = PacketCipher

ACE_UINT16 PacketCipher::getFingerPrint(ACE_UINT32 securityKey)
{
    return static_cast<ACE_UINT16>((securityKey >> 3) & 0x7FFF);
}


ACE_UINT32 PacketCipher::getCheckSum(const char* buffer, size_t bufferSize,
    ACE_UINT16 fingerPrint)
{
    return ACE::crc32(buffer, static_cast<int>(bufferSize)) + fingerPrint;
}


void PacketCipher::encrypt(const char* begin, const char* end, char* target,
    ACE_UINT32 securityKey)
{
    ACE_Byte xorSeed, shiftSeed;
    getOperand(securityKey, xorSeed, shiftSeed);

    std::transform(begin, end, target,
        Encryptor<ACE_Byte>(xorSeed, shiftSeed));
}


void PacketCipher::decrypt(const char* begin, const char* end, char* target,
    ACE_UINT32 securityKey)
{
    ACE_Byte xorSeed, shiftSeed;
    getOperand(securityKey, xorSeed, shiftSeed);

    std::transform(begin, end, target,
        Decryptor<ACE_Byte>(xorSeed, shiftSeed));
}

} // namespace skynet
