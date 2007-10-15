/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ServerInternalMessageBuilderTest.cpp
#include "stdafx.h"
#include <warnet/test/MessageBuilderTest.h>
#include <warnet/server/ServerInternalMessage.h>

using namespace warnet;

/**
* @class ServerInternalMessageBuilderTest
*
* GameServer ���� Message(Packet) �׽�Ʈ
*/
class ServerInternalMessageBuilderTest : public MessageBuilderTest
{ 
    CPPUNIT_TEST_SUITE(ServerInternalMessageBuilderTest);
    CPPUNIT_TEST(testLogoutMessageBuilder);
    CPPUNIT_TEST_SUITE_END();
private:
    void testLogoutMessageBuilder();
};

CPPUNIT_TEST_SUITE_REGISTRATION(ServerInternalMessageBuilderTest);

void ServerInternalMessageBuilderTest::testLogoutMessageBuilder()
{
    builderRepository_.add(new MessageBuilderT<LogoutMessage>);

    LogoutMessage msg;
    testMessageBuilder(msg, LogoutMessage::thisMessageType);
}
