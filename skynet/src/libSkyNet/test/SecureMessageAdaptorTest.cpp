/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// SecureMessageAdaptorTest.cpp
#include "stdafx.h"
#include "../SecureMessageAdaptor.h"
#include <warnet/test/EchoMessage.h>
#include <warnet/security/PacketCipher.h>
#include <warnet/detail/ServerMessageAdaptorFactory.h>
#include <warnet/detail/ServerMessageAdaptor.h>
#include <warnet/detail/ClientMessageAdaptorFactory.h>
#include <warnet/detail/ClientMessageAdaptor.h>

#pragma warning( push )
#pragma warning( disable : 4244 )
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#pragma warning( pop )

using namespace warnet;

/**
* @class SecureMessageAdaptorTest
*
* SecureMessageAdaptor 테스트
*/
class SecureMessageAdaptorTest : public CppUnit::TestFixture
{ 
    CPPUNIT_TEST_SUITE(SecureMessageAdaptorTest);
    CPPUNIT_TEST(testSecureMessageAdaptor);
    CPPUNIT_TEST(testSecureMessageAdaptorSecurityKey);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testSecureMessageAdaptor();
    void testSecureMessageAdaptorSecurityKey();
private:
    ServerMessageAdaptor* serverAdaptor_;
    ClientMessageAdaptor* clientAdaptor_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(SecureMessageAdaptorTest);

void SecureMessageAdaptorTest::setUp()
{
    const ACE_UINT32 firstSecuritySeed = 0x12345678;

    serverAdaptor_ = ServerMessageAdaptorFactory::create(true);
    serverAdaptor_->getMessageAdaptor().setSecuritySeed(firstSecuritySeed);

    clientAdaptor_ = ClientMessageAdaptorFactory::create(true);
    clientAdaptor_->getMessageAdaptor().setSecuritySeed(firstSecuritySeed);
}


void SecureMessageAdaptorTest::tearDown()
{
    delete serverAdaptor_;
    delete clientAdaptor_;
}


void SecureMessageAdaptorTest::testSecureMessageAdaptor()
{
    for (int i = 0; i < 10000; ++i) {
        if ((i % 100) == 1) {
            serverAdaptor_->getMessageAdaptor().setSecuritySeed(i);
            clientAdaptor_->getMessageAdaptor().setSecuritySeed(i);
        }

        EchoMessage msgTo("어금니꽉깨물어라엉아한테존내맞는거다");
        ACE_Message_Block* block;
        CPPUNIT_ASSERT_MESSAGE("serializeTo",
            msgTo.serializeTo(block, clientAdaptor_->getMessageAdaptor()));

        EchoMessage msgFrom;
        CPPUNIT_ASSERT_MESSAGE("serializeFrom",
            msgFrom.serializeFrom(*block, serverAdaptor_->getMessageAdaptor()));
        CPPUNIT_ASSERT_EQUAL_MESSAGE("message type",
            msgTo.getMessageType(), msgFrom.getMessageType());

        CPPUNIT_ASSERT_EQUAL_MESSAGE("message",
            msgTo.getEchoMessage(), msgFrom.getEchoMessage());

        block->release();
    }
}


void SecureMessageAdaptorTest::testSecureMessageAdaptorSecurityKey()
{
    for (int i = 0; i < 2000; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("encrypt",
            static_cast<SecureMessageAdaptor&>(
                clientAdaptor_->getMessageAdaptor()).getEncryptSecurityKey(),
            static_cast<SecureMessageAdaptor&>(
                serverAdaptor_->getMessageAdaptor()).getEncryptSecurityKey());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("decrypt",
            static_cast<SecureMessageAdaptor&>(
                clientAdaptor_->getMessageAdaptor()).getDecryptSecurityKey(),
            static_cast<SecureMessageAdaptor&>(
                serverAdaptor_->getMessageAdaptor()).getDecryptSecurityKey());
    }
}
