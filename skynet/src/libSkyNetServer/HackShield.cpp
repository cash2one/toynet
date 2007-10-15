/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author ±èº´¼ö <kcando@megaking.co.kr>
*/
#include "stdafx.h"
#include "HackShield.h"
#include <skynet/Wdr.h>
#include <skynet/detail/Logging.h>
#include <AntiCpSvrFunc.h>

// BUG: http://support.microsoft.com/default.aspx?scid=kb;en-us;822039

namespace skynet
{

const size_t HackShield::guidReqMsgSize = SIZEOF_GUIDREQMSG;
const size_t HackShield::guidAckMsgSize = SIZEOF_GUIDACKMSG;
const size_t HackShield::guidReqInfoSize = SIZEOF_GUIDREQINFO;
const size_t HackShield::reqMsgSize = SIZEOF_REQMSG;
const size_t HackShield::ackMsgSize = SIZEOF_ACKMSG;
const size_t HackShield::reqInfoSize = SIZEOF_REQINFO;

HackShield::HackShield() :
    initialized_(0)
{
}


HackShield::~HackShield()
{
    finalize();
}


bool HackShield::initialize(const std::string& crcFilename)
{
    WN_ASSERT(! initialized_);

    unsigned long result = _AntiCpSvr_Initialize(crcFilename.c_str());
    if (ERROR_SUCCESS != result) {
        WN_LOG_ERROR3("_AntiCpSvr_Initialize(%d,%s) FAILED!!!\n",
            result, crcFilename.c_str());
        return false;
    }

    initialized_ = true;
    return true;
}


void HackShield::finalize()
{
    if (initialized_) {
        _AntiCpSvr_Finalize();
        initialized_ = false;
    }
}


bool HackShield::makeGuidRequest(unsigned char* guidReqMsg,
    unsigned char* guidReqInfo)
{
    if (! initialized_) {
        return false;
    }

    const unsigned long result =
        _AntiCpSvr_MakeGuidReqMsg(guidReqMsg, guidReqInfo);
    if (ERROR_SUCCESS != result) {
        WN_LOG_ERROR2("_AntiCpSvr_MakeGuidReqMsg(%d) FAILED!!!\n",
            result);
        return false;
    }

    return true;
}


bool HackShield::analyzeGuidResponse(const unsigned char* guidAckMsg,
    const unsigned char* guidReqInfo, unsigned long** crcInfo)
{
    if (! initialized_) {
        return false;
    }

    const unsigned long result =
        _AntiCpSvr_AnalyzeGuidAckMsg(const_cast<unsigned char*>(guidAckMsg),
        const_cast<unsigned char*>(guidReqInfo), crcInfo);
    if (ERROR_SUCCESS != result) {
        WN_LOG_ERROR2("_AntiCpSvr_AnalyzeGuidAckMsg(%d) FAILED!!!\n",
            result);
        return false;
    }

    return true;
}


bool HackShield::makeRequest(const unsigned long* crcInfo,
    unsigned char* reqMsg, unsigned char* reqInfo, bool strictCheck)
{
    if (! initialized_) {
        return false;
    }

    const unsigned long result =
        _AntiCpSvr_MakeReqMsg(const_cast<unsigned long*>(crcInfo), reqMsg,
        reqInfo,
        strictCheck ? ANTICPSVR_CHECK_ALL : ANTICPSVR_CHECK_GAME_MEMORY);
    if (ERROR_SUCCESS != result) {
        WN_LOG_ERROR2("_AntiCpSvr_MakeReqMsg(%d) FAILED!!!\n",
            result);
        return false;
    }

    return true;
}


bool HackShield::analyzeResponse(const unsigned long* crcInfo,
    const unsigned char* ackMsg, const unsigned char* reqInfo)
{
    if (! initialized_) {
        return false;
    }

    const unsigned long result =
        _AntiCpSvr_AnalyzeAckMsg(const_cast<unsigned long*>(crcInfo),
            const_cast<unsigned char*>(ackMsg),
            const_cast<unsigned char*>(reqInfo));
    if (ERROR_SUCCESS != result) {
        WN_LOG_ERROR2("_AntiCpSvr_AnalyzeAckMsg(%d) FAILED!!!\n",
            result);
        return false;
    }

    return true;
}

} // namespace skynet
