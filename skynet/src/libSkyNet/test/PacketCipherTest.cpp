/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// PacketCipherTest.cpp
#include "stdafx.h"

#include <warnet/security/PacketCipher.h>
#include <ace/Message_Block.h>
#pragma warning( push )
#pragma warning( disable : 4244 4511 )
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#pragma warning( pop )
#include <boost/random/linear_congruential.hpp>
#include <vector>

using namespace warnet;

/**
 * @class PacketCipherTest
 *
 * PacketCipher xUnit 테스트.
 */
class PacketCipherTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(PacketCipherTest);
    CPPUNIT_TEST(testEncryptDecrypt);
    CPPUNIT_TEST(testCheckSum);
    CPPUNIT_TEST(testBoostRand48);
    CPPUNIT_TEST_SUITE_END();
public:
    //void setUp();
    //void tearDown();
private:
    void testEncryptDecrypt();
    void testCheckSum();
    void testBoostRand48();
};


CPPUNIT_TEST_SUITE_REGISTRATION(PacketCipherTest);

void PacketCipherTest::testEncryptDecrypt()
{
    const int dataSize = 9999;
    const ACE_UINT32 securityKey = 1;

    ACE_Message_Block dataBuffer(dataSize), compareBuffer(dataSize);
    for (int i = 0; i < dataSize; ++i) {
        *(dataBuffer.rd_ptr()) = static_cast<char>(i);
        *(compareBuffer.rd_ptr()) = static_cast<char>(i);
    }

    PacketCipher::encrypt(compareBuffer, securityKey);
    PacketCipher::decrypt(compareBuffer, securityKey);

    CPPUNIT_ASSERT(ACE_OS::memcmp(dataBuffer.rd_ptr(), compareBuffer.rd_ptr(),
        dataSize) == 0);
}


void PacketCipherTest::testCheckSum()
{
    const int dataSize = 8888;
    const ACE_UINT16 fingerPrint = 56341;

    ACE_Message_Block dataBuffer1(dataSize), dataBuffer2(dataSize);
    for (int i = 0; i < dataSize; ++i) {
        *(dataBuffer1.rd_ptr()) = static_cast<char>(i);
        *(dataBuffer2.rd_ptr()) = static_cast<char>(i);
    }

    CPPUNIT_ASSERT_EQUAL(
        PacketCipher::getCheckSum(dataBuffer1, fingerPrint),
        PacketCipher::getCheckSum(dataBuffer2, fingerPrint));
}


void PacketCipherTest::testBoostRand48()
{
    const boost::int32_t seed = 0x12345678;
    boost::rand48 r1(seed);
    boost::rand48 r2(seed);

    for (int i = 0; i < 1000; ++i) {
        CPPUNIT_ASSERT_EQUAL(r1(), r2());
    }
}
