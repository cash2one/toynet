/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// GameClientSessionPoolTest.cpp
#include "stdafx.h"
#include <warnet/server/GameClientSessionPool.h>
#include <warnet/server/GameClientSession.h>
#include <warnet/detail/MessageBuilder.h>
#pragma warning( push )
#pragma warning( disable : 4244 )
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#pragma warning( pop )

using namespace warnet;

/**
* @class GameClientSessionPoolTest
*
* GameClientSessionPool 테스트
*/
class GameClientSessionPoolTest : public CppUnit::TestFixture 
{ 
    const static size_t lowWaterMark = 10;

    CPPUNIT_TEST_SUITE(GameClientSessionPoolTest);
    CPPUNIT_TEST(testAcquire);
    CPPUNIT_TEST(testAcquireOverLowWaterMark);
    CPPUNIT_TEST(testAcquireOverLowWaterMarkAndRelease);
    CPPUNIT_TEST_SUITE_END();
public: 
    void setUp();
    void tearDown();
private:
    void testAcquire();
    void testAcquireOverLowWaterMark();
    void testAcquireOverLowWaterMarkAndRelease();
private:
    MessageBuilderRepository dummyBuilderRepository_;
    GameClientSessionPool* pool_;

    size_t originalSize_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(GameClientSessionPoolTest);

void GameClientSessionPoolTest::setUp()
{
    pool_ = new GameClientSessionPool(0, dummyBuilderRepository_, lowWaterMark,
        false);

    originalSize_ = pool_->getSize();
}


void GameClientSessionPoolTest::tearDown()
{
    delete pool_;
    pool_ = 0;
}


void GameClientSessionPoolTest::testAcquire()
{
    GameClientSession* session = pool_->acquire();
    CPPUNIT_ASSERT(0 != session);

    CPPUNIT_ASSERT_EQUAL(
        static_cast<int>(originalSize_ - 1),
        static_cast<int>(pool_->getSize()));

    pool_->release(session);

    CPPUNIT_ASSERT_EQUAL(
        static_cast<int>(originalSize_),
        static_cast<int>(pool_->getSize()));
}


void GameClientSessionPoolTest::testAcquireOverLowWaterMark()
{
    for (int i = 0; i < (lowWaterMark + 1); ++i) {
        GameClientSession* session = pool_->acquire();
        CPPUNIT_ASSERT(0 != session);
        delete session;
    }

    CPPUNIT_ASSERT_EQUAL(
        static_cast<int>(lowWaterMark * 2) - 1,
        static_cast<int>(pool_->getSize()));
}


void GameClientSessionPoolTest::testAcquireOverLowWaterMarkAndRelease()
{
    std::queue<GameClientSession*> sessions;

    for (int i = 0; i < (lowWaterMark + 1); ++i) {
        GameClientSession* session = pool_->acquire();
        CPPUNIT_ASSERT(0 != session);
        sessions.push(session);
    }

    while (! sessions.empty()) {
        pool_->release(sessions.front());
        sessions.pop();
    }

    CPPUNIT_ASSERT_EQUAL(
        static_cast<int>(lowWaterMark * 3),
        static_cast<int>(pool_->getSize()));
}
