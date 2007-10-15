/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author ±èº´¼ö <kcando@megaking.co.kr>
*/
// Sha1Test.cpp
#include "stdafx.h"
#include <warnet/security/ShaHasher.h>

#pragma warning( push )
#pragma warning( disable : 4244 4511 )
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#pragma warning( pop )
#include <string>

using namespace warnet;

/**
* @class Sha1Test
*/
class Sha1Test : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(Sha1Test);
    CPPUNIT_TEST(testHash);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testHash();
private:
    ShaHasher* hasher_;
    std::string crypted_;
};


void Sha1Test::setUp()
{
    ShaHasher hash;
    hash.hash(std::string("1234567890"));
    crypted_ = hash.toString();

    hasher_ = new ShaHasher;
}


void Sha1Test::tearDown()
{
    delete hasher_;
}


CPPUNIT_TEST_SUITE_REGISTRATION(Sha1Test);

void Sha1Test::testHash()
{
    hasher_->hash(std::string("1234567890"));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("equal",
        crypted_, hasher_->toString());
}
