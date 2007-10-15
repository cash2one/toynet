/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// EventRepositoryTest.cpp
#include "stdafx.h"
#include <warnet/Event.h>
#include <warnet/detail/EventRepository.h>
#include <warnet/test/DummyMessage.h>
#pragma warning( push )
#pragma warning( disable : 4244 )
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#pragma warning( pop )

using namespace warnet;

namespace
{

/**
 * @class MockEventRepository
 *
 * - Subscribe/Unsubsribe Pattern
 */
class MockEventRepository : public EventRepository
{
public:
    bool isEventSubscribed() const {
        return ! events_.empty();
    }
};


/**
 * @class MockEvent
 *
 */
class MockEvent : public Event
{
public:
    MockEvent(MessageType mt = MT_DUMMY) :
        Event(mt),
        calledMt_(MT_UNKNOWN) {}

    virtual bool call(const Message& msg, const void* /*arg*/) {
        calledMt_ = msg.getMessageType();
        return true;
    }

    MessageType getCalledMessageType() const {
        return calledMt_;
    }
private:
    MessageType calledMt_;
};

} // namespace


/**
* @class EventRepositoryTest
*
* MockEventRepository(Event Handling) 테스트
*/
class EventRepositoryTest : public CppUnit::TestFixture 
{ 
    CPPUNIT_TEST_SUITE(EventRepositoryTest);
    CPPUNIT_TEST(testSubscribe);
    CPPUNIT_TEST(testUnsubscribe);
    CPPUNIT_TEST(testPublish);
    CPPUNIT_TEST_SUITE_END(); 
public: 
    void setUp();
    void tearDown();
private:
    void testSubscribe();
    void testUnsubscribe();
    void testPublish();
private:
    MockEventRepository* eventChain_;
    MockEvent* event_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(EventRepositoryTest);

void EventRepositoryTest::setUp()
{
    eventChain_ = new MockEventRepository;
    event_ = new MockEvent;

    eventChain_->subscribe(event_);
}


void EventRepositoryTest::tearDown()
{
    delete eventChain_;
    //delete event_;
}


void EventRepositoryTest::testSubscribe()
{
    CPPUNIT_ASSERT_MESSAGE("subscribe", eventChain_->isEventSubscribed());
}


void EventRepositoryTest::testUnsubscribe()
{
    eventChain_->unsubscribe(MT_DUMMY);
    CPPUNIT_ASSERT_MESSAGE("unsubscribe", ! eventChain_->isEventSubscribed());
}


void EventRepositoryTest::testPublish()
{
    MockEvent* event2 = new MockEvent(MT_UNKNOWN);
    eventChain_->subscribe(event2);

    DummyMessage msg;
    eventChain_->publish(msg);
    CPPUNIT_ASSERT_MESSAGE("event must be called",
        MT_DUMMY == event_->getCalledMessageType());
}
