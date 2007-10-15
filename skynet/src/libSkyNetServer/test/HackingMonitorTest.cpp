/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// HackingMonitorTest.cpp
#include "stdafx.h"
#include <warnet/server/HackingMonitor.h>
#include "../HackShield.h"
#pragma warning( push )
#pragma warning( disable : 4312 )
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#pragma warning( pop )
#include <AntiCpSvrFunc.h>

using namespace warnet;

/**
* @class HackingMonitorTest
*
* HackingMonitor 테스트
*/
class HackingMonitorTest : public CppUnit::TestFixture 
{ 
    CPPUNIT_TEST_SUITE(HackingMonitorTest);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testFinalize);
    CPPUNIT_TEST(testMakeGuidRequest);
    CPPUNIT_TEST(testAnalyzeGuidResponse);
    CPPUNIT_TEST(testMakeRequest);
    CPPUNIT_TEST(testAnalyzeResponse);
    CPPUNIT_TEST_SUITE_END(); 
public: 
    void setUp();
    void tearDown();
private:
    void testInitialize();
    void testFinalize();
    void testMakeGuidRequest();
    void testAnalyzeGuidResponse();
    void testMakeRequest();
    void testAnalyzeResponse();
private:
    HackShield* hackShield_;
    HackingMonitor* monitor_;
    bool initialized_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(HackingMonitorTest);

const char crcFilename[] = "..\\conf\\PoporuArcade.crc";

void HackingMonitorTest::setUp()
{
    hackShield_ = new HackShield;
    initialized_ = hackShield_->initialize(crcFilename);

    monitor_ = new HackingMonitor(*hackShield_);
}


void HackingMonitorTest::tearDown()
{
    delete monitor_;
    delete hackShield_;
}


void HackingMonitorTest::testInitialize()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("initialize",
        true, initialized_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("initialized",
        true, hackShield_->isInitialized());
}


void HackingMonitorTest::testFinalize()
{
    hackShield_->finalize();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("finalized",
        false, hackShield_->isInitialized());
}


void HackingMonitorTest::testMakeGuidRequest()
{
    HackingMonitor::Buffer guidRequest;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("_AntiCpSvr_MakeGuidReqMsg",
        true, monitor_->makeGuidRequest(guidRequest));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("msg size",
        SIZEOF_GUIDREQMSG, static_cast<int>(guidRequest.size()));
}


void HackingMonitorTest::testAnalyzeGuidResponse()
{
    HackingMonitor::Buffer guidResponse(SIZEOF_GUIDACKMSG, 'X');
    CPPUNIT_ASSERT_EQUAL_MESSAGE("_AntiCpSvr_AnalyzeGuidAckMsg",
        false, monitor_->analyzeGuidResponse(guidResponse));
}


void HackingMonitorTest::testMakeRequest()
{
    HackingMonitor::Buffer request;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("_AntiCpSvr_MakeReqMsg",
        false, monitor_->makeRequest(request));
    //CPPUNIT_ASSERT_EQUAL_MESSAGE("msg size",
    //    SIZEOF_REQMSG, static_cast<int>(request.size()));
}


void HackingMonitorTest::testAnalyzeResponse()
{
    HackingMonitor::Buffer response(SIZEOF_ACKMSG, 'Y');
    CPPUNIT_ASSERT_EQUAL_MESSAGE("_AntiCpSvr_AnalyzeAckMsg",
        false, monitor_->analyzeResponse(response));
}
