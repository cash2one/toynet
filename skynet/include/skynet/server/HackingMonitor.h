/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// HackingMonitor.h
//
#if !defined(INCLUDED_HACKINGMONITOR)
#define INCLUDED_HACKINGMONITOR

#include <string>
#include <memory>

namespace skynet
{

class HackShield;

/**
 * @class HackingMonitor
 *
 * ��ŷ �����
 * - �ٽ����� ���� ����
 */
class WARNET_API HackingMonitor
{
public:
    typedef std::vector<unsigned char> Buffer;
public:
    HackingMonitor(HackShield& hackShield);

    bool makeGuidRequest(Buffer& guidRequest);
    bool analyzeGuidResponse(const Buffer& guidResponse);

    bool makeRequest(Buffer& request);
    bool analyzeResponse(const Buffer& response);

    bool guidAcked() const {
        return guidAcked_;
    }
private:
    HackShield& hackShield_;
    Buffer guidReqInfo_;
    unsigned long* crcInfo_;

    Buffer reqInfo_;

    bool strictCheck_;
    bool guidAcked_;
}; 

} // namespace skynet

#endif // !defined(INCLUDED_HACKINGMONITOR)
