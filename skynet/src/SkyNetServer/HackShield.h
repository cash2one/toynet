/**
* Copyright (c) 
*
* @author 
*/
// HackShield.h
#if !defined(INCLUDED_HACKSHIELD)
#define INCLUDED_HACKSHIELD

#include <boost/utility.hpp>
#include <string>

namespace skynet
{

/**
 * @class HackShield
 *
 * 핵쉴드 서버 크랙 방지 기능
 */
class WARNET_API HackShield : public boost::noncopyable
{
public:
    static const size_t guidReqMsgSize;
    static const size_t guidAckMsgSize;
    static const size_t guidReqInfoSize;
    static const size_t reqMsgSize;
    static const size_t ackMsgSize;
    static const size_t reqInfoSize;
public:
    HackShield();
    ~HackShield();

    bool initialize(const std::string& crcFilename);
    void finalize();

    bool isInitialized() const {
        return initialized_;
    }

    bool makeGuidRequest(unsigned char* guidReqMsg,
        unsigned char* guidReqInfo);
    bool analyzeGuidResponse(const unsigned char* guidAckMsg,
        const unsigned char* guidReqInfo, unsigned long** crcInfo);
    bool makeRequest(const unsigned long* crcInfo, unsigned char* reqMsg,
        unsigned char* reqInfo, bool strictCheck);
    bool analyzeResponse(const unsigned long* crcInfo,
        const unsigned char* ackMsg, const unsigned char* reqInfo);
private:
    bool initialized_;
};

} // namespace skynet

#endif // !defined(INCLUDED_HACKSHIELD)
