/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// WarNetServiceTest.cpp

#include "stdafx.h"
#include <warnet/server/WarNetService.h>
#include <warnet/server/ProactorEventLooper.h>
#include <warnet/detail/MessageBuilder.h>
#include <warnet/test/DummyMessage.h>
#include <warnet/test/TestClient.h>
#include <warnet/detail/MessageAdaptor.h>

#pragma warning( push )
#pragma warning( disable : 4312 )
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#pragma warning( pop )

#include <sstream>
#include <memory>

using namespace warnet;

namespace
{

class InvalidMessageTypeMessage : public Message
{
private:
    virtual const char* getThisClassName() const {
        return "InvalidMessageTypeMessage";
    }
    virtual bool readBody(Wdr::InputWdr&) {
        return true;
    }
    virtual bool writeBody(Wdr::OutputWdr&) const {
        return true;
    }
    virtual size_t getBodySize() const {
        return 0;
    }
};


class BigBodySizeMessage : public Message
{
public:
    BigBodySizeMessage() {}

    BigBodySizeMessage(size_t bodySize) :
        bodySize_(bodySize) {}
private:
    virtual MessageType getThisMessageType() const {
        return MT_ECHO;
    }
    virtual const char* getThisClassName() const {
        return "BigBodySizeMessage";
    }
    virtual bool readBody(Wdr::InputWdr&) {
        return true;
    }
    virtual bool writeBody(Wdr::OutputWdr& wdr) const {
        std::string dummy(bodySize_, 'A');
        wdr.write(dummy.data(), bodySize_);
        return wdr.isGood();
    }
    virtual size_t getBodySize() const {
        return bodySize_;
    }
private:
    size_t bodySize_;
};


/// DummyMessageBuilder가 이미 등록된 상태에서 다른 내용의 패킷의 올 경우 처리
class InvalidMessage : public Message
{
public:
    InvalidMessage() {}
private:
    virtual MessageType getThisMessageType() const {
        return MT_DUMMY;
    }
    virtual const char* getThisClassName() const {
        return "InvalidMessage";
    }
    virtual bool readBody(Wdr::InputWdr&) {
        return true;
    }
    virtual bool writeBody(Wdr::OutputWdr&) const {
        return true;
    }
    virtual size_t getBodySize() const {
        return 0;
    }
};


/**
 * @class TestWarNetService
 *
 */
class TestWarNetService : public WarNetService
{
public:
    TestWarNetService() :
        recvMessageType_(MT_UNKNOWN) {}

    bool go() {
        if (! onInit())
            return false;
        return run();
    }

    virtual size_t getAuthenticateTimeout() const {
        return 1;
    }

    virtual size_t getHeartbeatTimeout() const {
        return 1;
    }

    MessageType getInputMessageType() const {
        return recvMessageType_;
    }
private:
    virtual bool isUsingCipher() const {
        return false;
    }

    virtual bool onInit() {
        if (! WarNetService::onInit())
            return false;

        getBuilderRepository().add(new MessageBuilderT<DummyMessage>);

        return true;
    }

    virtual bool handle(ClientId clientId, const Message& msg) {
        recvMessageType_ = msg.getMessageType();
        if (MT_DUMMY == recvMessageType_) {
            postClientMessage(clientId, &msg);
        }
        return true;
    }
private:
    virtual void initService() {}
    virtual void initMessageBuilders() {}
    virtual void initEvents() {}
private:
    MessageType recvMessageType_;
};

} // namespace

/**
* @class WarNetServiceTest
*
* WarNetService 인터페이스 테스트
*/
class WarNetServiceTest : public CppUnit::TestFixture 
{
    CPPUNIT_TEST_SUITE(WarNetServiceTest);
    CPPUNIT_TEST(testDisconnect);
    CPPUNIT_TEST(testAccept);
    CPPUNIT_TEST(testMultipleAccept);
    CPPUNIT_TEST(testMultipleDummyEcho);
    CPPUNIT_TEST(testDummyEcho);
    CPPUNIT_TEST(testDisconnectWhenClientIsNotAuthenticated);
    CPPUNIT_TEST(testDisconnectIfMessageTypeIsNotValid);
    CPPUNIT_TEST(testDisconnectIfBodySizeIsTooBig);
    CPPUNIT_TEST(testDisconnectIfPacketIsNotValid);
    CPPUNIT_TEST(testDisconnectWhenNoHeartbeat);
    CPPUNIT_TEST(testNodisconnectWhenHeartbeat);
    CPPUNIT_TEST_SUITE_END(); 
public:
    void setUp();
    void tearDown();
private:
    void testAccept();
    void testDisconnect();
    void testMultipleAccept();
    void testDummyEcho();
    void testMultipleDummyEcho();
    void testDisconnectWhenClientIsNotAuthenticated();
    void testDisconnectIfMessageTypeIsNotValid();
    void testDisconnectIfBodySizeIsTooBig();
    void testDisconnectIfPacketIsNotValid();
    void testDisconnectWhenNoHeartbeat();
    void testNodisconnectWhenHeartbeat();
private:
    TestWarNetService* service_;
    TestClient* client_;
    ProactorEventLooper looper_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(WarNetServiceTest);

const ACE_Time_Value waiting_timeout(0, ACE_U_ONE_SECOND_IN_USECS / 10);

void WarNetServiceTest::setUp()
{
    client_ = new TestClient;

    service_ = new TestWarNetService;
    CPPUNIT_ASSERT(service_->go());

    looper_.start(1);

    client_->connect(1);
    ACE_OS::sleep(waiting_timeout);
}


void WarNetServiceTest::tearDown()
{
    looper_.stop();

    client_->close();
    delete client_;

    service_->fini();
    delete service_;

    // CAUTION: Proactor의 singleton instance를 반드시 초기화해줘야 한다.
    ACE_Proactor::close_singleton();
}


void WarNetServiceTest::testAccept()
{
    service_->onAuthenticated(service_->getClientId());

    ACE_OS::sleep(1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("only one client",
        1, static_cast<int>(service_->getClientCount()));
}


void WarNetServiceTest::testDisconnect()
{
    service_->onAuthenticated(service_->getClientId());

    client_->close();
    ACE_OS::sleep(waiting_timeout);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("no client",
        0, static_cast<int>(service_->getClientCount()));
}


void WarNetServiceTest::testMultipleAccept()
{
    service_->onAuthenticated(service_->getClientId());

    const int client_count = 3;
    TestClient clients[client_count];
    for (int i = 0; i < client_count; ++i) {
        std::ostringstream oss;
        oss << "client(";
        oss << (i + 1);
        oss << ") is connecting";
        CPPUNIT_ASSERT_MESSAGE(oss.str(),
            clients[i].connect(1));
    }
    ACE_OS::sleep(waiting_timeout);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("client is connected",
        client_count + 1, static_cast<int>(service_->getClientCount()));
}


void WarNetServiceTest::testDummyEcho()
{
    service_->onAuthenticated(service_->getClientId());

    int sentSize = client_->sendDummyMessage();
    CPPUNIT_ASSERT_MESSAGE("sent", sentSize > 0);

    ACE_OS::sleep(waiting_timeout);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("DummyMessage Input",
        static_cast<MessageType>(MT_DUMMY),
        service_->getInputMessageType());

    ACE_Message_Block* msgRecv;
    ssize_t recvSize = client_->recv_n(msgRecv, sentSize);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("recv DummyMessage",
        static_cast<int>(sentSize), static_cast<int>(recvSize));
    msgRecv->wr_ptr(recvSize);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("DummyMessage Output",
        static_cast<MessageType>(MT_DUMMY),
        extractMessageType(*msgRecv));
    msgRecv->release();
}


void WarNetServiceTest::testMultipleDummyEcho()
{
    const int message_count = 3;

    service_->onAuthenticated(service_->getClientId());

    int sentSize = 0;
    for (int i = 0; i < message_count; ++i) {
        sentSize = client_->sendDummyMessage();
        CPPUNIT_ASSERT_MESSAGE("sent", sentSize > 0);
    }
    ACE_OS::sleep(waiting_timeout);

    for (int i = 0; i < message_count; ++i) {
        ACE_Message_Block* msgRecv;
        ssize_t recvSize = client_->recv_n(msgRecv, sentSize);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("recv DummyMessage",
            static_cast<int>(sentSize), static_cast<int>(recvSize));
        msgRecv->wr_ptr(recvSize);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("DummyMessage Output",
            static_cast<MessageType>(MT_DUMMY),
            extractMessageType(*msgRecv));
        msgRecv->release();
    }
}


void WarNetServiceTest::testDisconnectWhenClientIsNotAuthenticated()
{
    ACE_OS::sleep(static_cast<long>(service_->getAuthenticateTimeout() + 1));

    CPPUNIT_ASSERT_MESSAGE("disconnect a idle client",
        client_->isDisconnected());
}


void WarNetServiceTest::testDisconnectIfMessageTypeIsNotValid()
{
    service_->onAuthenticated(service_->getClientId());

    InvalidMessageTypeMessage msg;
    int sentSize = client_->sendMessage(msg);
    CPPUNIT_ASSERT_MESSAGE("sent", sentSize > 0);

    ACE_OS::sleep(waiting_timeout);

    CPPUNIT_ASSERT_MESSAGE("disconnected", client_->isDisconnected());
}


void WarNetServiceTest::testDisconnectIfBodySizeIsTooBig()
{
    service_->onAuthenticated(service_->getClientId());

    BigBodySizeMessage msg(service_->getMaxPacketBodySize() + 1);
    int sentSize = client_->sendMessage(msg);
    CPPUNIT_ASSERT_MESSAGE("sent", sentSize > 0);

    ACE_OS::sleep(waiting_timeout);

    CPPUNIT_ASSERT_MESSAGE("disconnected", client_->isDisconnected());
}


void WarNetServiceTest::testDisconnectIfPacketIsNotValid()
{
    service_->onAuthenticated(service_->getClientId());

    InvalidMessage msg;
    int sentSize = client_->sendMessage(msg);
    CPPUNIT_ASSERT_MESSAGE("sent", sentSize > 0);

    ACE_OS::sleep(waiting_timeout);

    CPPUNIT_ASSERT_MESSAGE("disconnected", client_->isDisconnected());
}


void WarNetServiceTest::testDisconnectWhenNoHeartbeat()
{
    service_->onAuthenticated(service_->getClientId());

    ACE_OS::sleep(static_cast<long>(service_->getHeartbeatTimeout() + 1));

    CPPUNIT_ASSERT_MESSAGE("disconnect a zombie client",
        client_->isDisconnected());
}


void WarNetServiceTest::testNodisconnectWhenHeartbeat()
{
    service_->onAuthenticated(service_->getClientId());

    ACE_OS::sleep(static_cast<long>(service_->getHeartbeatTimeout() / 2));
    //service_->onHeartbeat(service_->getClientId());
    client_->sendDummyMessage();

    ACE_OS::sleep(static_cast<long>(service_->getHeartbeatTimeout() / 2));

    CPPUNIT_ASSERT_MESSAGE("heartbeating", ! client_->isDisconnected());
}
