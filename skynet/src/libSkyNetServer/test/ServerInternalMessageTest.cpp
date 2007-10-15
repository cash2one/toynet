/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ServerInternalMessageTest.cpp
#include "stdafx.h"
#include <warnet/test/MessageTest.h>
#include <warnet/server/ServerInternalMessage.h>

using namespace warnet;

/**
* @class ServerInternalMessageTest
*
* ���� ���� �޼��� �׽�Ʈ
*/
class ServerInternalMessageTest : public MessageTest
{ 
    CPPUNIT_TEST_SUITE(ServerInternalMessageTest);
    CPPUNIT_TEST(testLogoutMessage);
    CPPUNIT_TEST_SUITE_END(); 
private:
    void testLogoutMessage();
};

CPPUNIT_TEST_SUITE_REGISTRATION(ServerInternalMessageTest);

void ServerInternalMessageTest::testLogoutMessage()
{
    LogoutMessage msgTo;
    serializeTo(msgTo);

    LogoutMessage msgFrom;
    serializeFrom(msgFrom, msgTo);
}
