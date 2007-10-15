
/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// CommonMessageTest.cpp
#include "stdafx.h"
#include <warnet/test/MessageTest.h>
#include <warnet/detail/CommonMessage.h>

using namespace warnet;

/**
* @class CommonMessageTest
*
* 서버 공통 Message(Packet) 테스트
*/
class CommonMessageTest : public MessageTest
{ 
    CPPUNIT_TEST_SUITE(CommonMessageTest);
    CPPUNIT_TEST(testHackShieldGuidReqMessage);
    CPPUNIT_TEST(testHackShieldGuidAckMessage);
    CPPUNIT_TEST(testHackShieldReqMessage);
    CPPUNIT_TEST(testHackShieldAckMessage);
    CPPUNIT_TEST_SUITE_END(); 
private:
    void testHackShieldGuidReqMessage();
    void testHackShieldGuidAckMessage();
    void testHackShieldReqMessage();
    void testHackShieldAckMessage();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CommonMessageTest);


void CommonMessageTest::testHackShieldGuidReqMessage()
{
    const HackShieldMsg guidReq(100, 'R');
    HackShieldGuidReqMessage msgTo(&(guidReq[0]), guidReq.size());
    serializeTo(msgTo);

    HackShieldGuidReqMessage msgFrom;
    serializeFrom(msgFrom, msgTo);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("req msg size",
        static_cast<int>(msgTo.getReqMsg().size()),
        static_cast<int>(msgFrom.getReqMsg().size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("req msg",
        msgTo.getReqMsg()[2], msgFrom.getReqMsg()[2]);
}


void CommonMessageTest::testHackShieldGuidAckMessage()
{
    const HackShieldMsg guidAck(100, 'A');
    HackShieldGuidAckMessage msgTo(&(guidAck[0]), guidAck.size());
    serializeTo(msgTo);

    HackShieldGuidAckMessage msgFrom;
    serializeFrom(msgFrom, msgTo);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("req msg size",
        static_cast<int>(msgTo.getAckMsg().size()),
        static_cast<int>(msgFrom.getAckMsg().size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("req msg",
        msgTo.getAckMsg()[2], msgFrom.getAckMsg()[2]);
}


void CommonMessageTest::testHackShieldReqMessage()
{
    const HackShieldMsg guidReq(100, 'R');
    HackShieldReqMessage msgTo(&(guidReq[0]), guidReq.size());
    serializeTo(msgTo);

    HackShieldReqMessage msgFrom;
    serializeFrom(msgFrom, msgTo);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("req msg size",
        static_cast<int>(msgTo.getReqMsg().size()),
        static_cast<int>(msgFrom.getReqMsg().size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("req msg",
        msgTo.getReqMsg()[2], msgFrom.getReqMsg()[2]);
}


void CommonMessageTest::testHackShieldAckMessage()
{
    const HackShieldMsg guidAck(100, 'A');
    HackShieldAckMessage msgTo(&(guidAck[0]), guidAck.size());
    serializeTo(msgTo);

    HackShieldAckMessage msgFrom;
    serializeFrom(msgFrom, msgTo);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("req msg size",
        static_cast<int>(msgTo.getAckMsg().size()),
        static_cast<int>(msgFrom.getAckMsg().size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("req msg",
        msgTo.getAckMsg()[2], msgFrom.getAckMsg()[2]);
}
