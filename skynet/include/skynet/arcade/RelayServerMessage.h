/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// RelayServerMessage.h
#if !defined(INCLUDED_RELAYSERVERMESSAGE)
#define INCLUDED_RELAYSERVERMESSAGE

#include "ArcadeRelayInfo.h"
#include "ArcadeServerInfo.h"
#include "ArcadeProtocol.h"
#include <skynet/Message.h>
#include <skynet/ErrorCode.h>
#include <boost/static_assert.hpp>

namespace skynet
{

/// RelayServer(이하 RS) 관련 패킷
enum RelayServerMessageType_t
{
    MT_RS_C2S_SERVERINFO = (MT_RS_START + 1),
    MT_RS_S2C_SERVERINFO = (MT_RS_START + 2),

    MT_RS_C2S_LOGIN = (MT_RS_START + 10),
    MT_RS_S2C_LOGIN = (MT_RS_START + 20),

    MT_RS_C2S_CREATE_RELAY_ROOM = (MT_RS_START + 30),
    MT_RS_S2C_CREATE_RELAY_ROOM = (MT_RS_START + 40),

    MT_RS_KEY_FRAME = (MT_RS_START + 50),

    MT_RS_C2S_STOP_RELAY = (MT_RS_START + 60),

    MT_RS_S2C_RELAY_ROOM_DESTROYED = (MT_RS_START + 70),

    MT_RS_BOUNDARY
};

BOOST_STATIC_ASSERT(MT_RS_BOUNDARY <= MT_RS_STOP);


/**
* @class RelayServerInfoMessage
*
* RelayServer 모니터링을 위한 서버 정보 요청 메세지.
* - 응답 : RelayServerInfoResultMessage
*/
class WARNET_API RelayServerInfoMessage : public Message
{
public:
    enum { thisMessageType = MT_RS_C2S_SERVERINFO };
public:
    explicit RelayServerInfoMessage(
            const LoginCookie& monitorCookie = LoginCookie()) :
        monitorCookie_(monitorCookie) {}

    const LoginCookie& getMonitorCookie() const {
        return monitorCookie_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "RelayServerInfoMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    LoginCookie monitorCookie_;
};


/**
* @class RelayServerInfoResultMessage
*
* RelayServer 모니터링을 위한 서버 정보 요청에 대한 응답 메세지.
*/
class WARNET_API RelayServerInfoResultMessage : public Message
{
public:
    enum { thisMessageType = MT_RS_S2C_SERVERINFO };
public:
    explicit RelayServerInfoResultMessage(
            const RelayServerInfo& relayServerInfo = RelayServerInfo()) :
        relayServerInfo_(relayServerInfo) {}

    const RelayServerInfo& getServerInfo() const {
        return relayServerInfo_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "RelayServerInfoResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RelayServerInfo relayServerInfo_;
};


/**
* @class RelayLoginMessage
*
* GameServer 인증 메세지.
* - 응답 : LoginResultMessage
*/
class WARNET_API RelayLoginMessage : public Message
{
public:
    enum { thisMessageType = MT_RS_C2S_LOGIN };
public:
    explicit RelayLoginMessage(UserNo userNo = INVALID_USERNO,
            const LoginCookie& cookie = "", RoomId idRoom = INVALID_ROOMID,
            GameRoundId idGameRound = INVALID_GAMEROUNDID) :
        userNo_(userNo),
        cookie_(cookie),
        idRoom_(idRoom),
        idGameRound_(idGameRound) {}

    UserNo getUserNo() const {
        return userNo_;
    }
    const LoginCookie& getCookie() const {
        return cookie_;
    }
    RoomId getRoomId() const {
        return idRoom_;
    }
    GameRoundId getGameRoundId() const {
        return idGameRound_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "RelayLoginMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo userNo_; ///< 사용자 번호
    LoginCookie cookie_; ///< 로그인 쿠키
    RoomId idRoom_; ///< 입장할 관전방 번호
    GameRoundId idGameRound_; ///< 입장할 게임라운드 번호
};


/**
* @class RelayLoginResultMessage
*
* GameServer 인증 결과 메세지.
*/
class WARNET_API RelayLoginResultMessage : public Message
{
public:
    enum { thisMessageType = MT_RS_S2C_LOGIN };
public:
    explicit RelayLoginResultMessage(ErrorCode ec = EC_UNKNOWN) :
        ec_(ec) {}

    ErrorCode getErrorCode() const {
        return ec_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "RelayLoginResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ErrorCode ec_; ///< 에러 코드
};


/**
* @class CreateRelayRoomMessage
*
* 관전방 생성 요청
* - 서버간 통신에만 사용된다
*/
class WARNET_API CreateRelayRoomMessage : public Message
{
public:
    enum { thisMessageType = MT_RS_C2S_CREATE_RELAY_ROOM };
public:
    explicit CreateRelayRoomMessage(RoomId idRoom = INVALID_ROOMID,
        const GameRoundIds& gameRoundIds = GameRoundIds()) :
        idGameRoom_(idRoom),
        gameRoundIds_(gameRoundIds) {}

    RoomId getGameRoomId() const {
        return idGameRoom_;
    }
    const GameRoundIds& getGameRoundIds() const {
        return gameRoundIds_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "CreatedRelayRoomMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idGameRoom_;
    GameRoundIds gameRoundIds_;
};


/**
* @class CreateRelayRoomResultMessage
*
* 관전방 생성 요청에 대한 결과
* - 서버간 통신에만 사용된다
*/
class WARNET_API CreateRelayRoomResultMessage : public Message
{
public:
    enum { thisMessageType = MT_RS_S2C_CREATE_RELAY_ROOM };
public:
    explicit CreateRelayRoomResultMessage(RoomId idGameRoom = INVALID_ROOMID,
            RoomId idRelayRoom = INVALID_ROOMID,
            const LoginCookie& playerCookie = LoginCookie(),
            const LoginCookie& observerCookie = LoginCookie(),
            ErrorCode ec = EC_UNKNOWN) :
        idGameRoom_(idGameRoom),
        idRelayRoom_(idRelayRoom),
        playerCookie_(playerCookie),
        observerCookie_(observerCookie),
        ec_(ec) {
        WN_ASSERT(playerCookie.length() <= DL_LOGINCOOKIE_SIZE);
        WN_ASSERT(observerCookie.length() <= DL_LOGINCOOKIE_SIZE);
    }

    RoomId getGameRoomId() const {
        return idGameRoom_;
    }
    RoomId getRelayRoomId() const {
        return idRelayRoom_;
    }
    const LoginCookie& getPlayerCookie() const {
        return playerCookie_;
    }
    const LoginCookie& getObserverCookie() const {
        return observerCookie_;
    }
    ErrorCode getErrorCode() const {
        return ec_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "CreateRelayRoomResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idGameRoom_;
    RoomId idRelayRoom_;
    LoginCookie playerCookie_;
    LoginCookie observerCookie_;
    ErrorCode ec_;
};


/**
* @class KeyFrameMessage
*
* 관전 데이터(키 프레임) 송신/수신
*/
class WARNET_API KeyFrameMessage : public Message
{
public:
    enum { thisMessageType = MT_RS_KEY_FRAME };
public:
    explicit KeyFrameMessage(RoomId idRoom = INVALID_ROOMID,
                KeyFrameNumber keyFrameNumber = INVALID_KEYFRAMENUMBER,
                const KeyFrame* frame = 0,
                GameRoundId idGameRound = GRI_FIRST) :
            idRelayRoom_(idRoom),
            keyFrameNumber_(keyFrameNumber),
            idGameRound_(idGameRound) {
        if (frame != 0) {
            memcpy(&keyFrame_, frame, sizeof(keyFrame_));
        }
        else {
            memset(&keyFrame_, 0, sizeof(keyFrame_));
        }
    }

    RoomId getRelayRoomId() const {
        return idRelayRoom_;
    }
    const KeyFrame& getKeyFrame() const {
        return keyFrame_;
    }
    KeyFrameNumber getKeyFrameNumber() const {
        return keyFrameNumber_;
    }
    GameRoundId getGameRoundId() const {
        return idGameRound_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "KeyFrameMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRelayRoom_;
    KeyFrameNumber keyFrameNumber_;
    KeyFrame keyFrame_;
    GameRoundId idGameRound_;
};


/**
* @class RelayRoomDestroyedMessage
*
* 관전방이 닫혔음을 게임 서버에게 알린다
*/
class WARNET_API RelayRoomDestroyedMessage : public Message
{
public:
    enum { thisMessageType = MT_RS_S2C_RELAY_ROOM_DESTROYED };
public:
    explicit RelayRoomDestroyedMessage(RoomId idGameRoom = INVALID_ROOMID,
            RoomId idRelayRoom = INVALID_ROOMID) :
        idGameRoom_(idGameRoom),
        idRelayRoom_(idRelayRoom) {}

    RoomId getGameRoomId() const {
        return idGameRoom_;
    }
    RoomId getRelayRoomId() const {
        return idRelayRoom_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "RelayRoomDestroyedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idGameRoom_;
    RoomId idRelayRoom_;
};

} // namespace skynet

#endif // !defined(INCLUDED_RELAYSERVERMESSAGE)
