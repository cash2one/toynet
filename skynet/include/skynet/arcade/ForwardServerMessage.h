/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ForwardServerMessage.h
#if !defined(INCLUDED_FORWARDSERVERMESSAGE)
#define INCLUDED_FORWARDSERVERMESSAGE

#include "ArcadeProtocol.h"
#include <skynet/Message.h>
#include <skynet/Wdr.h>
#include <boost/static_assert.hpp>

namespace skynet
{

/// ForwardServer(이하 RS) 관련 패킷
enum ForwardServerMessageType_t
{
    MT_FS_C2S_RESOLVING = (MT_FS_START + 1),
    MT_FS_S2C_RESOLVING = (MT_FS_START + 2),

    MT_FS_FORWARDING = (MT_FS_START + 3),

    MT_FS_BOUNDARY
};

BOOST_STATIC_ASSERT(MT_FS_BOUNDARY <= MT_FS_STOP);

/**
* @class ResolvingMessage
*
* UDP 주소 resolving 요청 메세지.
* - 응답 : ResolvingResultMessage
*/
class WARNET_API ResolvingMessage : public Message
{
public:
    enum { thisMessageType = MT_FS_C2S_RESOLVING };
public:
    ResolvingMessage() {}
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ResolvingMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
};


/**
* @class ResolvingResultMessage
*
* UDP 주소 resolving 요청에 대한 응답 메세지.
*/
class WARNET_API ResolvingResultMessage : public Message
{
public:
    enum { thisMessageType = MT_FS_S2C_RESOLVING };
public:
    explicit ResolvingResultMessage(Wdr::ULong ip = 0,
            Wdr::UShort port = 0) :
        ip_(ip),
        port_(port) {}

    Wdr::ULong getIp() const {
        return ip_;
    }
    Wdr::UShort getPort() const {
        return port_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ResolvingResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    Wdr::ULong ip_; ///< IP 주소. host byte order.
    Wdr::UShort port_; ///< 포트 번호
};


/**
* @class ForwardingMessage
*
* Forwarding 서버에 포워딩을 요청하거나 요청에 대한 응답 메세지
* - 응답: ForwardingMessage
*/
class WARNET_API ForwardingMessage : public Message
{
public:
    enum { thisMessageType = MT_FS_FORWARDING };
    typedef std::vector<char> DataType;
public:
    ForwardingMessage(Wdr::ULong ip = 0, Wdr::UShort port = 0,
                const char* data = 0, size_t dataSize = 0) :
            ip_(ip),
            port_(port),
            dataSize_(static_cast<Wdr::UShort>(dataSize)) {
        if (dataSize > 0) {
            data_.reserve(dataSize);
            data_.assign(&data[0], &data[dataSize]);
        }
    }

    Wdr::ULong getIp() const {
        return ip_;
    }
    Wdr::UShort getPort() const {
        return port_;
    }

    const DataType& getData() const {
        return data_;
    }
    Wdr::UShort getDataSize() const {
        WN_ASSERT(data_.size() == dataSize_);
        return dataSize_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ForwardingMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    Wdr::ULong ip_; ///< IP 주소. host byte order.
    Wdr::UShort port_; ///< 포트 번호
    DataType data_;
    Wdr::UShort dataSize_;
};

} // namespace skynet

#endif // !defined(INCLUDED_FORWARDSERVERMESSAGE)
