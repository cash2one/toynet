

/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// BasicMessageTest.cpp
#include "stdafx.h"
#include <warnet/test/MessageTest.h>
#include <warnet/test/DummyMessage.h>
#include <warnet/test/EchoMessage.h>

using namespace warnet;

/**
* @class BasicMessageTest
*
* Message(Packet) 테스트
*/
class BasicMessageTest : public MessageTest
{ 
    CPPUNIT_TEST_SUITE(BasicMessageTest);
    CPPUNIT_TEST(testSerializeDummyMessage);
    CPPUNIT_TEST(testExtractMessageType);
    CPPUNIT_TEST(testExtractBodySize);
    CPPUNIT_TEST(testExtractHeader);
    CPPUNIT_TEST(testEchoMessage);
    CPPUNIT_TEST_SUITE_END(); 
private:
    void testSerializeDummyMessage();
    void testExtractMessageType();
    void testExtractBodySize();
    void testExtractHeader();
    void testEchoMessage();
};

CPPUNIT_TEST_SUITE_REGISTRATION(BasicMessageTest);

void BasicMessageTest::testSerializeDummyMessage()
{
    const char a = 127;
    const Wdr::Long b[3] = { 1, 2, 3 };
    DummyMessage msgTo(a, b, sizeof(b)/sizeof(Wdr::sizeLong));
    serializeTo(msgTo);

    DummyMessage msgFrom;
    serializeFrom(msgFrom, msgTo);
    CPPUNIT_ASSERT_MESSAGE("char", a == msgFrom.getChar());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("array[0]", b[0], (msgFrom.getLongArray())[0]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("array[1]", b[1], (msgFrom.getLongArray())[1]);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("array[2]", b[2], (msgFrom.getLongArray())[2]);
}


void BasicMessageTest::testExtractMessageType()
{
    DummyMessage msgTo;

    CPPUNIT_ASSERT(msgTo.serializeTo(block_, *encryptAdaptor_));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("extract message-type",
        static_cast<MessageType>(MT_DUMMY),
        extractMessageType(*block_));
}


void BasicMessageTest::testExtractBodySize()
{
    DummyMessage msgTo;

    CPPUNIT_ASSERT(msgTo.serializeTo(block_, *encryptAdaptor_));
    CPPUNIT_ASSERT_MESSAGE("extract body-size",
        static_cast<int>(msgTo.getBodySize()) <=
        static_cast<int>(extractBodySize(*block_)));
}


void BasicMessageTest::testExtractHeader()
{
    DummyMessage msgTo;

    CPPUNIT_ASSERT(msgTo.serializeTo(block_, *encryptAdaptor_));
    MessageType msg_type;
    size_t body_size;
    CPPUNIT_ASSERT_MESSAGE("extract header",
        extractHeader(*block_, msg_type, body_size));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("message type",
        msgTo.getMessageType(), msg_type);
    CPPUNIT_ASSERT_MESSAGE("body-size",
        static_cast<int>(msgTo.getBodySize()) <=
        static_cast<int>(body_size));
}


void BasicMessageTest::testEchoMessage()
{
    EchoMessage msgTo("echo");
    serializeTo(msgTo);

    EchoMessage msgFrom;
    serializeFrom(msgFrom, msgTo);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("echo message",
        msgTo.getEchoMessage(),
        msgFrom.getEchoMessage());
}
