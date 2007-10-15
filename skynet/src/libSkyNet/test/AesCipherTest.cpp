/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author ±èº´¼ö <kcando@megaking.co.kr>
*/
// AesCipherTest.cpp
#include "stdafx.h"
#include <warnet/security/AesCipher.h>

#pragma warning( push )
#pragma warning( disable : 4244 4511 )
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#pragma warning( pop )
#include <string>

using namespace warnet;

/**
* @class AesCipherTest
*/
class AesCipherTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(AesCipherTest);
    CPPUNIT_TEST(testCipher);
    CPPUNIT_TEST(testHangulCipher);
    CPPUNIT_TEST(testTwice);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testCipher();
    void testHangulCipher();
    void testTwice();
};


void AesCipherTest::setUp()
{
}


void AesCipherTest::tearDown()
{
}


CPPUNIT_TEST_SUITE_REGISTRATION(AesCipherTest);

void AesCipherTest::testCipher()
{
    const char key[] = "1234567890123456";;
    const std::string data("abcdefghijklmnopqrstuvwxyz");

    AesCipher cipher(key);
    const std::string encrypted = cipher.encrypt(data);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("encrypt/decrypt",
        data, cipher.decrypt(encrypted));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("PoporuCipher matching",
        std::string("321ea151a7cc04d50f2cc16b78c58c30da81ec7dc32ba849"
            "2007f4caa9646037"),
        encrypted);
}


void AesCipherTest::testHangulCipher()
{
    const char key[] = "1234567890123456";;
    const std::string data("¹«±ÃÈ­ ²ÉÀÌ ÇÇ¾ú½À´Ï´Ù");

    AesCipher cipher(key);
    const std::string encrypted = cipher.encrypt(data);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("encrypt/decrypt",
        data, cipher.decrypt(encrypted));
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("PoporuCipher matching",
        std::string("71441becec388484cc0816616a34db4dfc3c40beceb06c9d1"
            "abedd29d2141023"),
        encrypted);
}


void AesCipherTest::testTwice()
{
    const char key[] = "1234567890123456";;
    const std::string data("¹«±ÃÈ­ ²ÉÀÌ ÇÇ¾ú½À´Ï´Ù");

    AesCipher cipher(key);
    const std::string encrypted = cipher.encrypt(data);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("encrypt",
        encrypted, cipher.encrypt(data));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("decrypt",
        cipher.decrypt(encrypted), cipher.decrypt(encrypted));
}
