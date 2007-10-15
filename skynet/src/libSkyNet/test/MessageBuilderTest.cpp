/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// MessageBuilderTest.cpp
#include "stdafx.h"
#include <warnet/test/DummyMessage.h>
#include <warnet/test/EchoMessage.h>
#include <warnet/detail/MessageAdaptorFactory.h>
#include <warnet/detail/MessageAdaptor.h>
#include <warnet/detail/MessageBuilder.h>

#pragma warning( push )
#pragma warning( disable : 4244 )
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#pragma warning( pop )
#include <ace/Message_Block.h>

using namespace warnet;

/**
* @class MessageBuilderTest
*
* MessageBuilder 테스트
*/
class MessageBuilderTest : public CppUnit::TestFixture 
{ 
    CPPUNIT_TEST_SUITE(MessageBuilderTest);
    CPPUNIT_TEST(testBuilder);
    CPPUNIT_TEST(testEchoMessageBuilder);
    CPPUNIT_TEST_SUITE_END(); 
public: 
    void setUp();
    void tearDown();
private:
    void testBuilder();
    void testEchoMessageBuilder();
private:
    MessageBuilderRepository builderRepository_;

    MessageAdaptor* encryptAdaptor_;
    MessageAdaptor* decryptAdaptor_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(MessageBuilderTest);

void MessageBuilderTest::setUp()
{
    builderRepository_.add(new MessageBuilderT<DummyMessage>);

    encryptAdaptor_ = MessageAdaptorFactory::create(false);
    decryptAdaptor_ = MessageAdaptorFactory::create(false);
}


void MessageBuilderTest::tearDown()
{
    delete encryptAdaptor_;
    delete decryptAdaptor_;
}


void MessageBuilderTest::testBuilder()
{
    DummyMessage dummyMsg;
    ACE_Message_Block* block;
    CPPUNIT_ASSERT(dummyMsg.serializeTo(block, *encryptAdaptor_));

    std::auto_ptr<Message> msg(
        builderRepository_.build(*block, *decryptAdaptor_));
    block->release();
    CPPUNIT_ASSERT_MESSAGE("builder", msg.get() != 0);
    CPPUNIT_ASSERT_MESSAGE("message-type",
        MT_DUMMY == msg->getMessageType());
}


void MessageBuilderTest::testEchoMessageBuilder()
{
    builderRepository_.add(new MessageBuilderT<EchoMessage>);

    EchoMessage echoMsg;
    ACE_Message_Block* block;
    CPPUNIT_ASSERT(echoMsg.serializeTo(block, *encryptAdaptor_));

    std::auto_ptr<Message> msg(
        builderRepository_.build(*block, *decryptAdaptor_));
    block->release();
    CPPUNIT_ASSERT_MESSAGE("builder", msg.get() != 0);
    CPPUNIT_ASSERT_MESSAGE("message-type",
        MT_ECHO == msg->getMessageType());
}

