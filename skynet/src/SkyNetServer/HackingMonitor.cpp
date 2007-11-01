/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author ±èº´¼ö <kcando@megaking.co.kr>
*/
#include "stdafx.h"
#include <skynet/server/HackingMonitor.h>
#include "HackShield.h"
#include <skynet/detail/Logging.h>

// BUG: http://support.microsoft.com/default.aspx?scid=kb;en-us;822039

namespace skynet
{

HackingMonitor::HackingMonitor(HackShield& hackShield) :
    hackShield_(hackShield),
    guidReqInfo_(HackShield::guidReqInfoSize, 0),
    reqInfo_(HackShield::reqInfoSize, 0),
    crcInfo_(0),
    strictCheck_(true),
    guidAcked_(false)
{
}


bool HackingMonitor::makeGuidRequest(Buffer& guidRequest)
{
    guidRequest.resize(HackShield::guidReqMsgSize, 0);
    const bool result =
        hackShield_.makeGuidRequest(&(guidRequest[0]), &(guidReqInfo_[0]));
    WN_ASSERT(guidRequest.size() == HackShield::guidReqMsgSize);
    return result;
}


bool HackingMonitor::analyzeGuidResponse(const Buffer& guidResponse)
{
    WN_ASSERT(guidResponse.size() <= HackShield::guidAckMsgSize);
    const bool result = hackShield_.analyzeGuidResponse(&(guidResponse[0]),
        &(guidReqInfo_[0]), &crcInfo_);
    if (result) {
        guidAcked_ = true;
    }
    return result;
}


bool HackingMonitor::makeRequest(Buffer& request)
{
    request.resize(HackShield::reqMsgSize, 0);
    const bool result = hackShield_.makeRequest(crcInfo_, &(request[0]),
        &(reqInfo_[0]), strictCheck_);
    if (strictCheck_) {
        strictCheck_ = ! strictCheck_;
    }
    WN_ASSERT(request.size() == HackShield::reqMsgSize);
    return result;
}


bool HackingMonitor::analyzeResponse(const Buffer& response)
{
    WN_ASSERT(response.size() <= HackShield::ackMsgSize);
    return hackShield_.analyzeResponse(crcInfo_,
        &(response[0]), &(reqInfo_[0]));
}

} // namespace skynet
