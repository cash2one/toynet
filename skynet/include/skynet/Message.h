/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// Message.h
#if !defined(INCLUDED_MESSAGE)
#define INCLUDED_MESSAGE

#include "Protocol.h"
#include <boost/utility.hpp>
#include <string>

class ACE_Message_Block;

namespace skynet
{

namespace Wdr
{
    class OutputWdr;
    class InputWdr;
} // namespace Wdr

class MessageAdaptor;

/// 메세지 블럭에서 MessageType을 추출한다
MessageType WARNET_API extractMessageType(ACE_Message_Block& packet);

/// 메세지 블럭에서 body-size를 추출한다
size_t WARNET_API extractBodySize(ACE_Message_Block& packet);

/// 메세지 블럭에서 메세지 헤더를 추출한다
bool WARNET_API extractHeader(ACE_Message_Block& packet, MessageType& msgType,
    size_t& bodySize);

/**
* @class Message
*
* Message Abstract Class.
* - 메세지를 encoding/decoding한다
* - <B>CAUTION: wdr stream(operator <<, operator >>)을 쓸 경우 미묘한 문제점이
*   발생할 수 있다.</B>
*/
class WARNET_API Message : public boost::noncopyable
{
public:
    Message() {}
    virtual ~Message() {}

    /**
     * 바이트 스트림으로 데이터를 읽어 메세지를 구성한다.
     * - 반드시 packet은 valid해야 한다.
     */
    bool serializeFrom(ACE_Message_Block& packet, MessageAdaptor& adaptor);

    /**
     * 메세지를 바이트 스트림으로 만든다.
     * - packet을 반드시 release()해야 한다.
     */
    bool serializeTo(ACE_Message_Block*& packet,
        MessageAdaptor& adaptor) const;

    /// 메세지 타입을 얻는다
    MessageType getMessageType() const {
        return getThisMessageType();
    }

    /// 메세지 이름을 얻는다(로그 기록 용)
    const char* getClassName() const {
        return getThisClassName();
    }
public:
    /// 메세지 헤더 크기를 얻는다
    static size_t getHeaderSize();
private:
    /// 메세지 타입 리턴
    virtual MessageType getThisMessageType() const {
        return MT_UNKNOWN;
    }
    /// 클래스 이름 리턴(RTTI-like)
    virtual const char* getThisClassName() const = 0;
    /// 패킷에서 body 부분을 읽어온다.
    virtual bool readBody(Wdr::InputWdr& wdr) = 0;
    /// 패킷 body 부분을 기록한다.
    virtual bool writeBody(Wdr::OutputWdr& wdr) const = 0;
    /// 바이트 스트림으로 변환할 패킷 body의 크기.
    virtual size_t getBodySize() const = 0;
};

} // namespace skynet

#endif // !defined(INCLUDED_MESSAGE)
