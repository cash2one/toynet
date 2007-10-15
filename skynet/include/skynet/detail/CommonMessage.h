/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// CommonMessage.h
#if !defined(INCLUDED_COMMMONMESSAGE)
#define INCLUDED_COMMMONMESSAGE

#include "../Protocol.h"
#include <skynet/Message.h>
#include <skynet/ErrorCode.h>

namespace skynet
{

/// 서버 공통 메세지
enum CommonMessageType_t
{
    MT_C_C2S_HACKSHIELD_GUID_ACK = (MT_C_START + 1),
    MT_C_S2C_HACKSHIELD_GUID_REQ = (MT_C_START + 2),

    MT_C_C2S_HACKSHIELD_ACK = (MT_C_START + 3),
    MT_C_S2C_HACKSHIELD_REQ = (MT_C_START + 4)
};


typedef std::vector<unsigned char> HackShieldMsg;

/**
* @class HackShieldGuidAckMessage
*
* 핵쉴드 CRC 요청에 대한 응답 메세지
* - 요청: HackShieldGuidReqMessage
*/
class WARNET_API HackShieldGuidAckMessage : public Message
{
public:
    enum { thisMessageType = MT_C_C2S_HACKSHIELD_GUID_ACK };
public:
    explicit HackShieldGuidAckMessage(const unsigned char* ackMsg = 0,
        size_t ackMsgSize = 0) {
        if (ackMsgSize > 0) {
            ackMsg_.assign(&(ackMsg[0]), &(ackMsg[ackMsgSize]));
            WN_ASSERT(ackMsg_.size() == ackMsgSize);
        }
    }

    const HackShieldMsg& getAckMsg() const {
        return ackMsg_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "HackShieldGuidAckMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    HackShieldMsg ackMsg_; ///< CRC 응답 메세지
};


/**
* @class HackShieldGuidReqMessage
*
* Common 정보 요청에 대한 응답 메세지.
*/
class WARNET_API HackShieldGuidReqMessage : public Message
{
public:
    enum { thisMessageType = MT_C_S2C_HACKSHIELD_GUID_REQ };
public:
    explicit HackShieldGuidReqMessage(const unsigned char* reqMsg = 0,
        size_t reqMsgSize = 0) {
        if (reqMsgSize > 0) {
            reqMsg_.assign(&(reqMsg[0]), &(reqMsg[reqMsgSize]));
            WN_ASSERT(reqMsg_.size() == reqMsgSize);
        }
    }

    const HackShieldMsg& getReqMsg() const {
        return reqMsg_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "HackShieldGuidReqMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    HackShieldMsg reqMsg_; ///< CRC 요구 메세지
};


/**
* @class HackShieldAckMessage
*
* 핵쉴드 CRC 요청에 대한 응답 메세지
* - 요청: HackShieldReqMessage
*/
class WARNET_API HackShieldAckMessage : public Message
{
public:
    enum { thisMessageType = MT_C_C2S_HACKSHIELD_ACK };
public:
    explicit HackShieldAckMessage(const unsigned char* ackMsg = 0,
        size_t ackMsgSize = 0) {
        if (ackMsgSize > 0) {
            ackMsg_.assign(&(ackMsg[0]), &(ackMsg[ackMsgSize]));
            WN_ASSERT(ackMsg_.size() == ackMsgSize);
        }
    }

    const HackShieldMsg& getAckMsg() const {
        return ackMsg_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "HackShieldAckMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    HackShieldMsg ackMsg_; ///< CRC 응답 메세지
};


/**
* @class HackShieldReqMessage
*
* Common 정보 요청에 대한 응답 메세지.
*/
class WARNET_API HackShieldReqMessage : public Message
{
public:
    enum { thisMessageType = MT_C_S2C_HACKSHIELD_REQ };
public:
    explicit HackShieldReqMessage(const unsigned char* reqMsg = 0,
        size_t reqMsgSize = 0) {
        if (reqMsgSize > 0) {
            reqMsg_.assign(&(reqMsg[0]), &(reqMsg[reqMsgSize]));
            WN_ASSERT(reqMsg_.size() == reqMsgSize);
        }
    }

    const HackShieldMsg& getReqMsg() const {
        return reqMsg_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "HackShieldReqMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    HackShieldMsg reqMsg_; ///< CRC 요구 메세지
};

} // namespace skynet

#endif // !defined(INCLUDED_COMMMONMESSAGE)
