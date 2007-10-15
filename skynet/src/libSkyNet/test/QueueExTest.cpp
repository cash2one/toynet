/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author ±èº´¼ö <kcando@megaking.co.kr>
*/
// QueueExTest.cpp
#include "stdafx.h"
#include <warnet/QueueEx.h>

#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4251 4267 4311 4312 4702 )
#include <ace/Task.h>
#pragma warning ( pop )

#pragma warning( push )
#pragma warning( disable : 4244 4511 )
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#pragma warning( pop )

using namespace warnet;

namespace
{

const size_t queueWaterMark = 16384;

typedef QueueEx<int*, queueWaterMark> QueueType;

class QueueProducer : public ACE_Task_Base
{
public:
    QueueProducer(QueueType& queue, int item_count) :
            queue_(queue),
            itemCount_(item_count) {
        activate();
    }

    int svc() {
        ACE_Time_Value pause(0, ACE_U_ONE_SECOND_IN_USECS / 10);
        ACE_OS::sleep(pause);
        for (int i = 0; i < (itemCount_ - 1); ++i) {
            queue_.add(new int(i));
        }
        return 0;
    }
private:
    QueueType& queue_;
    int itemCount_;
};

} // namespace

/**
* @class QueueExTest
*/
class QueueExTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(QueueExTest);
    CPPUNIT_TEST(testEnqueueDequeue_0);
    CPPUNIT_TEST(testEnqueueDequeue_HalfWaterMark);
    CPPUNIT_TEST(testEnqueueDequeue_Random);
    CPPUNIT_TEST(testOverflow);
    CPPUNIT_TEST(testMultithreadedQueuing);
    CPPUNIT_TEST_SUITE_END();
public:
    QueueExTest() : queue_(0) {}
    virtual ~QueueExTest() {}

    void setUp() {
        queue_ = new QueueType;
    }

    void tearDown() {
        QueueType::Item item;
        while (queue_->get(item)) {
            delete item;
        }
        delete queue_;
        queue_ = 0;
    }
private:
    void testEnqueueDequeue_0();
    void testEnqueueDequeue_HalfWaterMark();
    void testEnqueueDequeue_Random();
    void testOverflow();
    void testMultithreadedQueuing();
private:
    QueueType* queue_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(QueueExTest);

void QueueExTest::testEnqueueDequeue_0()
{
    CPPUNIT_ASSERT(0 == queue_->size_do_not_call_me());
    QueueType::Item dummy;
    CPPUNIT_ASSERT(! queue_->get(dummy));
}


void QueueExTest::testEnqueueDequeue_HalfWaterMark()
{
    const int enqueue_count = (queueWaterMark / 2);

    // enqueue
    for (int i = 0; i < enqueue_count; ++i)
        queue_->add(new int(i));
    CPPUNIT_ASSERT_EQUAL(enqueue_count,
        static_cast<int>(queue_->size_do_not_call_me()));

    // half dequeue
    for (int i = 0; i < (enqueue_count / 2); ++i) {
        QueueType::Item item;
        CPPUNIT_ASSERT(queue_->get(item));
        delete item;
    }
    CPPUNIT_ASSERT_EQUAL((enqueue_count / 2),
        static_cast<int>(queue_->size_do_not_call_me()));

    // half dequeue
    for (int i = 0; i < (enqueue_count - (enqueue_count / 2)); ++i) {
        QueueType::Item item;
        CPPUNIT_ASSERT(queue_->get(item));
        delete item;
    }
    CPPUNIT_ASSERT_EQUAL(0,
        static_cast<int>(queue_->size_do_not_call_me()));
}


void QueueExTest::testEnqueueDequeue_Random()
{
    queue_->add(new int(0));
    queue_->add(new int(1));
    queue_->add(new int(2));
    CPPUNIT_ASSERT_EQUAL(3,
        static_cast<int>(queue_->size_do_not_call_me()));

    QueueType::Item item;
    CPPUNIT_ASSERT(queue_->get(item));
    delete item;
    CPPUNIT_ASSERT_EQUAL(2,
        static_cast<int>(queue_->size_do_not_call_me()));

    queue_->add(new int(3));
    CPPUNIT_ASSERT_EQUAL(3,
        static_cast<int>(queue_->size_do_not_call_me()));

    CPPUNIT_ASSERT(queue_->get(item));
    delete item;
    CPPUNIT_ASSERT(queue_->get(item));
    delete item;
    CPPUNIT_ASSERT_EQUAL(1,
        static_cast<int>(queue_->size_do_not_call_me()));
}


void QueueExTest::testOverflow()
{
    const int enqueue_count = queueWaterMark;

    try {
        for (int i = 0; i < enqueue_count; ++i)
            queue_->add(new int(i));
        CPPUNIT_ASSERT(false);
    }
    catch (const std::overflow_error& e) {
        e;
    }
}


void QueueExTest::testMultithreadedQueuing()
{
    const int item_count = queueWaterMark;

    QueueProducer producer(*queue_, item_count);

    int dequeue_count = 0;
    while (dequeue_count < (item_count - 1)) {
        QueueType::Item item;
        if (queue_->get(item)) {
            CPPUNIT_ASSERT_EQUAL(*item, dequeue_count);
            ++dequeue_count;
            delete item;
        }
    }
    CPPUNIT_ASSERT_EQUAL((item_count - 1), dequeue_count);
}
