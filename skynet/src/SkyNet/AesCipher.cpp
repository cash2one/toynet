/**
* Copyright (c) 
*
* @author 
*/
#include "stdafx.h"
#include <skynet/security/AesCipher.h>
#include <skynet/detail/SkyNetUtil.h>
#include "AES.h"
#include <stdexcept>
#include <sstream>

namespace skynet
{

namespace
{

std::string getPadding(const std::string& src)
{
    std::string result;
    const size_t padding = BLOCK_SIZE - (src.size() % BLOCK_SIZE);
    for (size_t i = 0; i < padding; ++i) {
        result += static_cast<char>(i);
    }
    return result;
}


std::string string2Hex(const std::string& str)
{
    static const char hex[] = "0123456789abcdef";

    const std::string::size_type str_len = str.size();
    std::string hex_str;
    hex_str.reserve(str_len * 2);
    for (std::string::size_type i = 0; i < str_len; ++i) {
        unsigned char c = str[i];
        hex_str += hex[c / 16];
        hex_str += hex[c % 16];
    }
    WN_ASSERT(hex_str.size() == (str_len * 2));
    return hex_str;
}


template <typename T>
T hex2dec(T h)
{
    if (('0' <= h) && (h <= '9')) {
        return h - '0';
    }
    else if (('a' <= h) && (h <= 'f')) {
        return (h - 'a') + 10;
    }
    return 0;
}


std::string hex2String(const std::string& hex)
{
    WN_ASSERT(isEven(hex.size() % 2));

    std::string dec_str;
    const std::string::size_type src_len = hex.size();

    dec_str.reserve(src_len / 2);
    for (std::string::size_type i = 0; i < src_len; i += 2) {
        dec_str += ((hex2dec(hex[i]) * 16) + hex2dec(hex[i+1]));
    }
    WN_ASSERT(dec_str.size() == (src_len / 2));
    return dec_str;
}

} // namespace

// = AesCipher

AesCipher::AesCipher(const std::string& key) :
    blockState_(new block_state)
{
    memset(blockState_.get(), 0, sizeof(block_state));

    if (AES_init(blockState_.get(), key.c_str(),
            static_cast<int>(key.size())) != 0)
        throw std::logic_error("invalid key");
}


AesCipher::~AesCipher()
{
}


std::string AesCipher::encrypt(const std::string& data)
{
    std::ostringstream oss;
    oss << static_cast<unsigned int>(data.size()) << '\0' << data;
    oss << getPadding(oss.str());
    const std::string src(oss.str());

    std::vector<u8> encrypted(src.size(), 0);

    for (size_t i = 0; i < src.size(); i += BLOCK_SIZE) {
        AES_encrypt(blockState_.get(), &(src[i]), &(encrypted[i]));
    }
    void* resultPtr = &(encrypted[0]);
    std::string original =
        std::string(static_cast<std::string::value_type*>(resultPtr),
            encrypted.size());
    return string2Hex(original);
}


std::string AesCipher::decrypt(const std::string& data)
{
    if ((data.size() % BLOCK_SIZE) != 0)
        return "";

    std::string rawData(hex2String(data));
    std::vector<u8> buffer(rawData.size(), 0);

    for (size_t i = 0; i < rawData.size(); i += BLOCK_SIZE) {
        AES_decrypt(blockState_.get(), &(rawData[i]), &(buffer[i]));
    }

    void* resultPtr = &(buffer[0]);
    std::string decrypted(static_cast<std::string::value_type*>(resultPtr),
        buffer.size());

    size_t delimiterIndex = decrypted.find('\0');
    if (delimiterIndex == std::string::npos)
        return "";
    int srcLength = atoi(decrypted.substr(0, delimiterIndex).c_str());
    if (srcLength <= 0)
        return "";
    return decrypted.substr(delimiterIndex + 1, srcLength);
}

} // namespace skynet
