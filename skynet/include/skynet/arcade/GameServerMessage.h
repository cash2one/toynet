/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// GameServerMessage.h
#if !defined(INCLUDED_GAMESERVERMESSAGE)
#define INCLUDED_GAMESERVERMESSAGE

#include "ArcadeUserInfo.h"
#include "ArcadeReportInfo.h"
#include "ArcadeServerInfo.h"
#include "ArcadeProtocol.h"
#include <skynet/Message.h>
#include <skynet/ErrorCode.h>
#include <boost/static_assert.hpp>
#include <skynet/detail/Assert.h>

namespace skynet
{

/// GameServer(이하 GS) 관련 패킷
enum GameServerMessageType_t
{
    MT_GS_C2S_SERVERINFO = (MT_GS_START + 1),
    MT_GS_S2C_SERVERINFO = (MT_GS_START + 2),

    MT_GS_C2S_NOTICE_EX = (MT_GS_START + 5),

    MT_GS_C2S_LOGIN = (MT_GS_START + 10),
    MT_GS_S2C_LOGIN = (MT_GS_START + 11),

    MT_GS_C2S_USERINFO = (MT_GS_START + 15),
    MT_GS_S2C_USERINFO = (MT_GS_START + 16),

    MT_GS_C2S_DETAILEDUSERINFO = (MT_GS_START + 20),
    MT_GS_S2C_DETAILEDUSERINFO = (MT_GS_START + 21),

    MT_GS_C2S_UDP_ADDRESSES = (MT_GS_START + 30),

    MT_GS_C2S_CHANNEL_WAITER_LIST = (MT_GS_START + 40),
    MT_GS_S2C_CHANNEL_WAITER_LIST = (MT_GS_START + 41),

    MT_GS_S2C_CHANNEL_WAITER_JOINED = (MT_GS_START + 50),
    MT_GS_S2C_CHANNEL_WAITER_PARTED = (MT_GS_START + 51),

    MT_GS_C2S_CREATE_GAME_ROOM = (MT_GS_START + 60),
    MT_GS_S2C_CREATE_GAME_ROOM = (MT_GS_START + 61),

    MT_GS_C2S_GAME_ROOM_LIST = (MT_GS_START + 70),
    MT_GS_S2C_GAME_ROOM_LIST = (MT_GS_START + 71),

    MT_GS_C2S_JOIN_GAME_ROOM = (MT_GS_START + 80),
    MT_GS_S2C_JOIN_GAME_ROOM = (MT_GS_START + 81),

    MT_GS_S2C_GAME_ROOM_CREATED = (MT_GS_START + 90),

    MT_GS_S2C_GAME_ROOM_DESTROYED = (MT_GS_START + 91),

    MT_GS_S2C_GAME_ROOM_JOINED = (MT_GS_START + 100),

    MT_GS_S2C_GAME_ROOM_INFO = (MT_GS_START + 101),

    MT_GS_C2S_GAME_ROOM_WAITER_LIST = (MT_GS_START + 110), 
    MT_GS_S2C_GAME_ROOM_WAITER_LIST = (MT_GS_START + 111), 

    MT_GS_S2C_GAME_ROOM_WAITER_COUNT_CHANGED = (MT_GS_START + 120), 

    MT_GS_C2S_PART_GAME_ROOM = (MT_GS_START + 130),
    MT_GS_S2C_PART_GAME_ROOM = (MT_GS_START + 131),

    MT_GS_S2C_GAME_ROOM_PARTED = (MT_GS_START + 140),

    MT_GS_C2S_TOGGLE_GAME_ROOM_READY_FLAG = (MT_GS_START + 150),
    MT_GS_S2C_TOGGLE_GAME_ROOM_READY_FLAG = (MT_GS_START + 151),

    MT_GS_C2S_READY_GAME = (MT_GS_START + 160),
    MT_GS_S2C_READY_GAME = (MT_GS_START + 161),
    MT_GS_S2C_GAME_READIED = (MT_GS_START + 162),

    MT_GS_S2C_GAME_STARTED = (MT_GS_START + 170),

    MT_GS_C2S_STOP_GAME = (MT_GS_START + 180),
    MT_GS_S2C_STOP_GAME = (MT_GS_START + 181),

    MT_GS_S2C_GAME_STOPPED = (MT_GS_START + 190),

    MT_GS_S2C_GAME_ROOM_STATE_CHANGED = (MT_GS_START + 200),

    MT_GS_C2S_SET_GAME_ROOM_SLOT_STATE = (MT_GS_START + 210),
    MT_GS_S2C_GAME_ROOM_SLOT_STATE_CHANGED = (MT_GS_START + 211),

    MT_GS_C2S_LEAVE_GAME_ROOM_SLOT = (MT_GS_START + 220),
    MT_GS_S2C_LEAVE_GAME_ROOM_SLOT = (MT_GS_START + 221),

    MT_GS_C2S_ENTER_GAME_ROOM_SLOT = (MT_GS_START + 230),
    MT_GS_S2C_ENTER_GAME_ROOM_SLOT = (MT_GS_START + 231),

    MT_GS_S2C_GAME_ROOM_OWNER_CHANGED = (MT_GS_START + 240),

    MT_GS_S2C_RELAY_ROOM_CREATED = (MT_GS_START + 250),

    MT_GS_S2C_COIN_UPDATED = (MT_GS_START + 260),
    MT_GS_S2C_CHARACTER_CARD_CHANGED = (MT_GS_START + 261),

    MT_GS_C2S_CANCEL_GAME = (MT_GS_START + 270),
    MT_GS_S2C_GAME_CANCELED = (MT_GS_START + 271),

    MT_GS_C2S_INVITE = (MT_GS_START + 280),
    MT_GS_S2C_INVITE = (MT_GS_START + 281),
    MT_GS_S2C_INVITED = (MT_GS_START + 282),

    MT_GS_C2S_CANCEL_INVITATION = (MT_GS_START + 290),
    MT_GS_S2C_INVITATION_CANCELED = (MT_GS_START + 291),

    MT_GS_C2S_REJECT_INVITATION = (MT_GS_START + 300),
    MT_GS_S2C_INVITATION_REJECTED = (MT_GS_START + 301),

    MT_GS_C2S_KICK_ROOM_USER = (MT_GS_START + 310),



    MT_GS_C2S_CHANGE_GAME_MODE = (MT_GS_START + 320),
    MT_GS_S2C_CHANGE_GAME_MODE = (MT_GS_START + 321),
    MT_GS_S2C_GAME_MODE_CHANGED = (MT_GS_START + 322),

    MT_GS_C2S_CHALLENGE = (MT_GS_START + 330),
    MT_GS_S2C_CHALLENGE = (MT_GS_START + 331),
    MT_GS_S2C_CHALLENGED = (MT_GS_START + 332),

    MT_GS_C2S_INSERT_COIN = (MT_GS_START + 340),
    MT_GS_S2C_INSERT_COIN = (MT_GS_START + 341),

    MT_GS_C2S_PINGTIME = (MT_GS_START + 350),
    MT_GS_S2C_PINGTIME_CHANGED = (MT_GS_START + 351),

    MT_GS_C2S_SELECT_GAMEROUND = (MT_GS_START + 360),
    MT_GS_S2C_SELECT_GAMEROUND = (MT_GS_START + 361),
    MT_GS_S2C_GAMEROUND_SELECTED = (MT_GS_START + 362),

    MT_GS_S2C_GAMEROUND_CREATED = (MT_GS_START + 370),

    MT_GS_S2C_GAMESLOTS_CHANGED = (MT_GS_START + 380),

    MT_GS_C2S_CHAT = (MT_GS_START + 390),
    MT_GS_S2C_CHAT = (MT_GS_START + 391),

    MT_GS_WHISPER = (MT_GS_START + 400),

    MT_GS_C2S_FORWARD = (MT_GS_START + 410),
    MT_GS_S2C_FORWARDED = (MT_GS_START + 411),

    MT_GS_C2S_NOTICE = (MT_GS_START + 420),
    MT_GS_S2C_NOTICE = (MT_GS_START + 421),

    MT_GS_C2S_REPORT = (MT_GS_START + 430),
    MT_GS_S2C_REPORT = (MT_GS_START + 431),

    MT_GS_S2C_MARQUEE = (MT_GS_START + 440),

    MT_GS_C2S_CHANGE_GAMETYPE = (MT_GS_START + 450),
    MT_GS_S2C_GAMETYPE_CHANGED = (MT_GS_START + 451),

    MT_GS_C2S_GAMER_LIST = (MT_GS_START + 460),
    MT_GS_S2C_GAMER_LIST = (MT_GS_START + 461),
    MT_GS_S2C_GAMER_ONLINE = (MT_GS_START + 462),
    MT_GS_S2C_GAMER_OFFLINE = (MT_GS_START + 463),
    MT_GS_S2C_POSITION_CHANGED = (MT_GS_START + 464),

    MT_GS_C2S_LOAD_GAME_OPTIONS = (MT_GS_START + 470),
    MT_GS_S2C_LOAD_GAME_OPTIONS = (MT_GS_START + 471),
    MT_GS_C2S_SAVE_GAME_OPTIONS = (MT_GS_START + 472),

    MT_GS_C2S_LOAD_GLOBAL_OPTIONS = (MT_GS_START + 480),
    MT_GS_S2C_LOAD_GLOBAL_OPTIONS = (MT_GS_START + 481),
    MT_GS_C2S_SAVE_GLOBAL_OPTIONS = (MT_GS_START + 482),

    MT_GS_C2S_SET_ROOM_CHATTING = (MT_GS_START + 490),
    MT_GS_S2C_ROOM_CHATTING_SETTED = (MT_GS_START + 491),

    MT_GS_C2S_KICK_USER = (MT_GS_START + 500),
    MT_GS_S2C_USER_KICKED = (MT_GS_START + 501),
    MT_GS_S2C_KICK_LOGINUSER = (MT_GS_START +502),
    MT_GS_S2C_ALTER_LOGIN_OVERLAPUSER = (MT_GS_START +503),

    MT_GS_C2S_MAKE_OBSERVER = (MT_GS_START + 510),
    MT_GS_C2S_MAKE_SLOTWAITER = (MT_GS_START + 511),

    MT_GS_S2C_TOURNAMENT_REWARD = (MT_GS_START + 520),

    MT_GS_C2S_UNUSED_ITEM_LIST = (MT_GS_START + 530),
    MT_GS_S2C_UNUSED_ITEM_LIST = (MT_GS_START + 531),

    MT_GS_C2S_CARD_SLOTITEM_LIST = (MT_GS_START + 540),
    MT_GS_S2C_CARD_SLOTITEM_LIST = (MT_GS_START + 541),

    MT_GS_C2S_ATTACH_SLOT_ITEM = (MT_GS_START + 550),
    MT_GS_S2C_ATTACH_SLOT_ITEM = (MT_GS_START + 551),

    MT_GS_C2S_DELETE_SLOT_ITEM = (MT_GS_START + 560),
    MT_GS_S2C_DELETE_SLOT_ITEM = (MT_GS_START + 561),

    MT_GS_S2C_SLOT_ITEM_USED = (MT_GS_START + 570),

    MT_GS_C2S_MEGAPHONE = (MT_GS_START + 580),
    MT_GS_S2C_MEGAPHONE = (MT_GS_START + 581),

    MT_GS_C2S_HEARTBEAT = (MT_GS_START + 590),
    

    MT_GS_C2S_M_TO_G_LOGIN = (MT_GS_START + 1000),
    MT_GS_S2C_M_TO_G_LOGIN_RESULT = (MT_GS_START + 1001),


    MT_GS_C2S_REQUEST_GAMESERVER_INFO = (MT_GS_START + 1011),
    MT_GS_S2C_RESPONSE_GAMESERVER_INFO = (MT_GS_START + 1012),


    MT_GS_BOUNDARY
};

BOOST_STATIC_ASSERT(MT_GS_BOUNDARY <= MT_GS_STOP);


/**
* @class GameServerInfoMessage
*
* GameServer 모니터링을 위한 서버 정보 요청 메세지.
* - 응답 : GameServerInfoResultMessage
*/
class WARNET_API GameServerInfoMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_SERVERINFO };
public:
    explicit GameServerInfoMessage(
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
        return "GameServerInfoMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    LoginCookie monitorCookie_;
};


/**
* @class GameServerInfoResultMessage
*
* GameServer 모니터링을 위한 서버 정보 요청에 대한 응답 메세지
*/
class WARNET_API GameServerInfoResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_SERVERINFO };
public:
    explicit GameServerInfoResultMessage(
            const GameServerInfo& gsi = GameServerInfo()) :
        gameServerInfo_(gsi) {}

    const GameServerInfo& getGameServerInfo() const {
        return gameServerInfo_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameServerInfoResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    GameServerInfo gameServerInfo_;
};




/**
* @class LoginMessage
*
* GameServer 인증 메세지.
* - 응답 : LoginResultMessage
*/
class WARNET_API LoginMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_LOGIN };
public:
    explicit LoginMessage(const std::string& id = "",
            const std::string& password = "",
            GameType currentGameType = GT_UNKNOWN,
            bool fromPcRoom = false) :
        userId_(id),
        password_(password),
        currentGameType_(currentGameType),
        fromPcRoom_(fromPcRoom) {}

    const std::string& getUserId() const {
        return userId_;
    }
    const std::string& getPassword() const {
        return password_;
    }
    GameType getGameType() const {
        return currentGameType_;
    }
    bool isFromPcRoom() const {
        return fromPcRoom_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "LoginMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string userId_; ///< 사용자 아이디
    std::string password_; ///< 비밀번호
    GameType currentGameType_; ///< 로딩 중인 게임 타이틀
    bool fromPcRoom_; ///< PC방에서 접속했는가?
};


/**
* @class LoginResultMessage
*
* GameServer 인증 요청에 대한 결과 메세지.
*/
class WARNET_API LoginResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_LOGIN };
public:
    explicit LoginResultMessage(ErrorCode errorCode = EC_UNKNOWN,
        time_t serverTime = 0) :
        errorCode_(errorCode),
        serverTime_(serverTime) {}

    ErrorCode getErrorCode() const {
        return errorCode_;
    }

    time_t getServerTime() const {
        return serverTime_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "LoginResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ErrorCode errorCode_;
    time_t serverTime_;
};


/**
* @class UserInfoMessage
*
* 사용자 정보를 요청한다
* - 응답: UserInfoResultMessage
*/
class WARNET_API UserInfoMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_USERINFO };
public:
    explicit UserInfoMessage(UserNo userNo = INVALID_USERNO,
             GameType gameType = GT_UNKNOWN) :
        userNo_(userNo),
        gameType_(gameType) {}

    UserNo getUserNo() const {
        return userNo_;
    }
    GameType getGameType() const {
        return gameType_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "UserInfoMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo userNo_;
    GameType gameType_;
};


/**
* @class UserInfoResultMessage
*
* 사용자 정보에 대한 응답 메세지
*/
class WARNET_API UserInfoResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_USERINFO };
public:
    explicit UserInfoResultMessage(
            const UserInfo& userInfo = UserInfo()) :
        userInfo_(userInfo) {}

    const UserInfo& getUserInfo() const {
        return userInfo_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "UserInfoResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserInfo userInfo_;
};


/**
* @class DetailedUserInfoMessage
*
* 상세한 사용자 정보를 요청한다
* - 응답: DetailedUserInfoResultMessage
*/
class WARNET_API DetailedUserInfoMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_DETAILEDUSERINFO };
public:
    explicit DetailedUserInfoMessage(UserNo userNo = INVALID_USERNO) :
        userNo_(userNo) {}

    UserNo getUserNo() const {
        return userNo_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "DetailedUserInfoMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo userNo_;
};


/**
* @class DetailedUserInfoResultMessage
*
* 상세한 사용자 정보에 대한 응답 메세지
*/
class WARNET_API DetailedUserInfoResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_DETAILEDUSERINFO };
public:
    explicit DetailedUserInfoResultMessage(
            const DetailedUserInfo& userInfo = DetailedUserInfo()) :
        userInfo_(userInfo) {}

    const DetailedUserInfo& getUserInfo() const {
        return userInfo_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "DetailedUserInfoResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    DetailedUserInfo userInfo_;
};


/**
* @class UdpAddressesMessage
*
* UDP resolving 주소를 설정한다(바뀔 때마다 재설정해야 한다)
* - 응답: 없음
*/
class WARNET_API UdpAddressesMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_UDP_ADDRESSES };
public:
    explicit UdpAddressesMessage(const AddressPairs& pairs = AddressPairs()) :
        pairs_(pairs) {}

    const AddressPairs& getAddressPairs() const {
        return pairs_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "UdpAddressesMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    AddressPairs pairs_;
};


/**
 * @class ChannelWaitersMessage
 *
 * 채널 대기자 목록 요청 메세지
 * - 응답: ChannelWaitersResultMessage
 */
class WARNET_API ChannelWaitersMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_CHANNEL_WAITER_LIST };
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ChannelWaitersMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
};


/**
* @class ChannelWaitersResultMessage
*
* 채널 대기자 목록 요청에 대한 응답 메세지
*/
class WARNET_API ChannelWaitersResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_CHANNEL_WAITER_LIST };
public:
    explicit ChannelWaitersResultMessage(
            const ChannelWaiters& waiters = ChannelWaiters()) :
        waiters_(waiters) {}

    const ChannelWaiters& getChannelWaiters() const {
        return waiters_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ChannelWaitersResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ChannelWaiters waiters_;
};


/**
* @class ChannelWaiterJoinedMessage
*
* 채널 대기실에 새로운 사용자가 입장하였다.
* - 채널 대기실에 사용자가 입장할 때 마다 발생한다.
*/
class WARNET_API ChannelWaiterJoinedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_CHANNEL_WAITER_JOINED };
public:
    explicit ChannelWaiterJoinedMessage(
            const ChannelWaiter& waiter = ChannelWaiter()) :
        waiter_(waiter) {}

    const ChannelWaiter& getChannelWaiter() const {
        return waiter_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ChannelWaiterJoinedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ChannelWaiter waiter_;
};


/**
* @class ChannelWaiterPartedMessage
*
* 채널 대기실에 사용자가 퇴장하였다.
* - 채널 대기실에 사용자가 퇴장할 때 마다 발생한다.
*/
class WARNET_API ChannelWaiterPartedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_CHANNEL_WAITER_PARTED };
public:
    explicit ChannelWaiterPartedMessage(UserNo userNo = INVALID_USERNO) :
        userNo_(userNo) {}

    UserNo getUserNo() const {
        return userNo_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ChannelWaiterPartedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo userNo_;
};


/**
 * @class GamersMessage
 *
 * 접속자 목록 요청 메세지
 * - 응답: GamersResultMessage
 */
class WARNET_API GamersMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_GAMER_LIST };
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GamersMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
};


/**
* @class GamersResultMessage
*
* 접속자 목록 요청에 대한 응답 메세지
*/
class WARNET_API GamersResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAMER_LIST };
public:
    explicit GamersResultMessage(const Gamers& gamers = Gamers()) :
        gamers_(gamers) {}

    const Gamers& getGamers() const {
        return gamers_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GamersResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    Gamers gamers_;
};


/**
* @class ChatMessage
*
* 채팅(대기실, 게임방) 요청
* - 응답: ChatResultMessage
* - 이벤트: ChatResultMessage
*/
class WARNET_API ChatMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_CHAT };
public:
    explicit ChatMessage(const std::string& msg = "") :
        chattingMessage_(msg) {}

    const std::string& getChatMessage() const {
        return chattingMessage_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ChatMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string chattingMessage_;
};


/**
* @class ChatResultMessage
*
* 채팅(대기실, 게임방) 요청에 대한 응답 & 이벤트
*/
class WARNET_API ChatResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_CHAT };
public:
    explicit ChatResultMessage(UserNo from = INVALID_USERNO,
            const std::string& msg = "",
            ItemCode usedItemCode = INVALID_ITEMCODE, ErrorCode ec = EC_OK) :
        from_(from),
        chattingMessage_(msg),
        usedItemCode_(usedItemCode),
        errorCode_(ec) {}

    UserNo getFrom() const {
        return from_;
    }
    const std::string& getChatMessage() const {
        return chattingMessage_;
    }
    ItemCode getUsedItemCode() const {
        return usedItemCode_;
    }
    ErrorCode getErrorCode() const {
        return errorCode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ChatResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string chattingMessage_;
    UserNo from_; ///< 보낸 사용자
    ItemCode usedItemCode_;
    ErrorCode errorCode_; ///< 에러코드(요청시에는 사용되지 않는다)
};


/**
* @class WhisperMessage
*
* 귓속말 요청 or 응답
* - 응답: WhisperMessage
*/
class WARNET_API WhisperMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_WHISPER };
public:
    explicit WhisperMessage(const std::string& from = "",
            const std::string& to = "", const std::string& msg = "",
            ErrorCode ec = EC_UNKNOWN) :
        fromNickname_(from),
        toNickname_(to),
        chattingMessage_(msg),
        errorCode_(ec) {}

    const std::string& getFrom() const {
        return fromNickname_;
    }
    const std::string& getTo() const {
        return toNickname_;
    }
    const std::string& getWhisperMessage() const {
        return chattingMessage_;
    }
    ErrorCode getErrorCode() const {
        return errorCode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "WhisperMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string chattingMessage_;
    std::string fromNickname_; ///< 보낸 사용자 닉네임
    std::string toNickname_; ///< 받을 사용자 닉네임
    ErrorCode errorCode_; ///< 에러코드(현재는 귓속말일 경우에만 사용됨)
};


/**
* @class ForwardMessage
*
* 전달 메세지
* - 응답: ForwardedMessage
*/
class WARNET_API ForwardMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_FORWARD };
public:
    /**
     * ctor
     *
     * @param to 전달 받을 사용자. 로비나 방에 전달할 경우 TO_ALL로 설정해야 함
     */
    explicit ForwardMessage(const std::string& data = "", UserNo to = TO_ALL) :
        data_(data),
        to_(to) {}

    const std::string& getData() const {
        return data_;
    }
    UserNo getTo() const {
        return to_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ForwardMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string data_;
    UserNo to_;
};


/**
* @class ForwardedMessage
*
* 전달 요청에 대한 응답 메세지
* - 전달 요청자에게도 전송된다
*/
class WARNET_API ForwardedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_FORWARDED };
public:
    /**
     * ctor
     *
     * @param from 전달 요청자
     * @param to 전달 받은 사용자.
     * @param where 전달 받은 로비(방)
     * @param ec 에러 코드
     *
     * @c to와 @c where는 서로 배타적이다.
     */
    explicit ForwardedMessage(const std::string& data = "",
        UserNo from = INVALID_USERNO, UserNo to = INVALID_USERNO,
        RoomId where = INVALID_ROOMID, ErrorCode ec = EC_UNKNOWN) :
        data_(data),
        from_(from),
        to_(to),
        where_(where),
        errorCode_(ec) {}

    const std::string& getData() const {
        return data_;
    }
    UserNo getFrom() const {
        return from_;
    }
    UserNo getTo() const {
        return to_;
    }
    RoomId getWhere() const {
        return where_;
    }
    ErrorCode getErrorCode() const {
        return errorCode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ForwardedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string data_;
    UserNo from_;
    UserNo to_;
    RoomId where_;
    ErrorCode errorCode_;
};


/**
* @class CreateGameRoomMessage
*
* 대화방 생성 요청
* - 응답: CreateGameRoomResultMessage
*/
class WARNET_API CreateGameRoomMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_CREATE_GAME_ROOM };
public:
    explicit CreateGameRoomMessage(const std::string& name = "",
            const std::string& password = "", GameModeType gm = GM_UNKNOWN,
            unsigned short maxObserverCount = DL_DEFAULT_OBSERVER_COUNT,
            RoomType roomType = RT_NORMAL,
            const StringList& slotWaiterNicknames = StringList(),
            Wdr::Long matchTime = 0) :
        name_(name),
        password_(password),
        gameMode_(gm),
        maxObserverCount_(maxObserverCount),
        roomType_(roomType),
        slotWaiterNicknames_(slotWaiterNicknames),
        matchTime_(matchTime) {}

    const std::string& getName() const {
        return name_;
    }
    const std::string& getPassword() const {
        return password_;
    }
    GameModeType getGameMode() const {
        return gameMode_;
    }
    unsigned short getMaxObserverCount() const {
        return maxObserverCount_;
    }
    RoomType getRoomType() const {
        return roomType_;
    }
    const StringList& getSlotWaiterNicknames() const {
        return slotWaiterNicknames_;
    }
    Wdr::Long getMatchTime() const {
        return matchTime_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "CreateGameRoomMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string name_;
    std::string password_;
    GameModeType gameMode_;
    unsigned short maxObserverCount_;
    RoomType roomType_;
    StringList slotWaiterNicknames_;
    Wdr::Long matchTime_; ///< 경기 시작 시간
};


/**
* @class CreateGameRoomMessage
*
* 대화방 생성 요청
* - 응답: GameRoomCreatedMessage
*/
class WARNET_API CreateGameRoomResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_CREATE_GAME_ROOM };
public:
    explicit CreateGameRoomResultMessage(RoomId roomId = INVALID_ROOMID,
            const std::string& name = "",
            const std::string& password = "",
            GameModeType gm = GM_UNKNOWN,
            RoomType roomType = RT_UNKNOWN,
            ErrorCode errorCode = EC_UNKNOWN) :
        idRoom_(roomId),
        name_(name),
        password_(password),
        gameMode_(gm),
        roomType_(roomType),
        errorCode_(errorCode) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    const std::string& getName() const {
        return name_;
    }
    const std::string& getPassword() const {
        return password_;
    }
    GameModeType getGameMode() const {
        return gameMode_;
    }
    RoomType getRoomType() const {
        return roomType_;
    }
    ErrorCode getErrorCode() const {
        return errorCode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "CreateGameRoomResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    std::string name_;
    std::string password_;
    GameModeType gameMode_;
    RoomType roomType_;
    ErrorCode errorCode_;
};


/**
* @class GameRoomListMessage
*
* 게임방 목록 요청 메세지
* - 응답: GameRoomListResultMessage
*/
class WARNET_API GameRoomListMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_GAME_ROOM_LIST };
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameRoomListMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
};


/**
* @class GameRoomListResultMessage
*
* 게임방 목록 요청에 대한 응답 메세지
*/
class WARNET_API GameRoomListResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAME_ROOM_LIST };
public:
    explicit GameRoomListResultMessage(
            const GameRoomInfos& roomInfos = GameRoomInfos()) :
        roomInfos_(roomInfos) {}

    const GameRoomInfos& getRoomInfos() const {
        return roomInfos_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameRoomListResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    GameRoomInfos roomInfos_;
};


/**
* @class JoinGameRoomMessage
*
* 대화방 참여 요청
* - 응답: JoinGameRoomResultMessage
*/
class WARNET_API JoinGameRoomMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_JOIN_GAME_ROOM };
public:
    explicit JoinGameRoomMessage(RoomId roomId = INVALID_ROOMID,
            const std::string& password = "") :
        idRoom_(roomId),
        password_(password) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    const std::string& getPassword() const {
        return password_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "JoinGameRoomMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    std::string password_;
};


/**
* @class JoinGameRoomResultMessage
*
* 대화방 참여 요청에 대한 응답
*/
class WARNET_API JoinGameRoomResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_JOIN_GAME_ROOM };
public:
    explicit JoinGameRoomResultMessage(RoomId roomId = INVALID_ROOMID,
            SlotIndex slotIndex = INVALID_SLOTINDEX,
            ErrorCode errorCode = EC_UNKNOWN) :
        idRoom_(roomId),
        slotIndex_(slotIndex),
        errorCode_(errorCode) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    SlotIndex getSlotIndex() const {
        return slotIndex_;
    }
    ErrorCode getErrorCode() const {
        return errorCode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "JoinGameRoomResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    SlotIndex slotIndex_;
    ErrorCode errorCode_;
};


/**
* @class GameRoomCreatedMessage
*
* 대화방이 생성되었음을 알린다(모든 채널 대기자에게 알린다)
*/
class WARNET_API GameRoomCreatedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAME_ROOM_CREATED };
public:
    explicit GameRoomCreatedMessage(const GameRoomInfo& ri = GameRoomInfo()) :
        roomInfo_(ri) {}

    const GameRoomInfo& getRoomInfo() const {
        return roomInfo_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameRoomCreatedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    GameRoomInfo roomInfo_;
};


/**
* @class GameRoomDestroyedMessage
*
* 대화방이 폐쇄되었음을 알린다(모든 채널 대기자에게 알린다)
*/
class WARNET_API GameRoomDestroyedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAME_ROOM_DESTROYED };
public:
    explicit GameRoomDestroyedMessage(RoomId roomId = INVALID_ROOMID) :
        idRoom_(roomId) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameRoomDestroyedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
};


/**
* @class GameRoomJoinedMessage
*
* 게임방에 사용자가 입장하였음을 알린다(모든 방 대기자에게 알린다)
*/
class WARNET_API GameRoomJoinedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAME_ROOM_JOINED };
public:
    explicit GameRoomJoinedMessage(
            const GameRoomWaiter& grw = GameRoomWaiter()) :
        gameRoomWaiter_(grw) {}

    const GameRoomWaiter& getGameRoomWaiter() const {
        return gameRoomWaiter_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameRoomJoinedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    GameRoomWaiter gameRoomWaiter_;
};


/**
* @class GameRoomInfoMessage
*
* 게임방에 접속할 경우 방정보를 자동으로 알려준다
*/
class WARNET_API GameRoomInfoMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAME_ROOM_INFO };
public:
    explicit GameRoomInfoMessage(
            const DetailedGameRoomInfo& roomInfo = DetailedGameRoomInfo()) :
        roomInfo_(roomInfo) {}

    const DetailedGameRoomInfo& getGameRoomInfo() const {
        return roomInfo_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameRoomInfoMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    DetailedGameRoomInfo roomInfo_;
};


/**
* @class GameRoomWaitersMessage
*
* 대화방에 대기자(참여자) 목록을 요청한다
* 응답: GameRoomWaitersResultMessage
*/
class WARNET_API GameRoomWaitersMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_GAME_ROOM_WAITER_LIST };
public:
    explicit GameRoomWaitersMessage(RoomId idRoom = INVALID_ROOMID) :
        idRoom_(idRoom) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameRoomWaitersMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
};


/**
* @class GameRoomWaitersResultMessage
*
* 대화방에 사용자가 입장할 경우 자동으로 자신을 포함한 게임방 대기자(참여자)
* 목록을 받는다
*/
class WARNET_API GameRoomWaitersResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAME_ROOM_WAITER_LIST };
public:
    explicit GameRoomWaitersResultMessage(
            const GameRoomWaiters& grws = GameRoomWaiters()) :
        gameRoomWaiters_(grws) {}

    const GameRoomWaiters& getGameRoomWaiters() const {
        return gameRoomWaiters_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameRoomWaitersResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    GameRoomWaiters gameRoomWaiters_;
};


/**
* @class GameRoomWaiterCountChangedMessage
*
* 대화방에서 슬롯 또는 대기자의 수가 변경될 경우 모든 채널 대기자에게 알린다
*/
class WARNET_API GameRoomWaiterCountChangedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAME_ROOM_WAITER_COUNT_CHANGED };
public:
    explicit GameRoomWaiterCountChangedMessage(RoomId idRoom = INVALID_ROOMID,
                size_t waiterCount = 0, size_t slotCount = 0,
                size_t observerCount = 0) :
            idRoom_(idRoom),
            waiterCount_(waiterCount),
            slotCount_(slotCount),
            observerCount_(observerCount) {
        WN_ASSERT(slotCount <= DL_MAX_SLOT_COUNT);
        WN_ASSERT(waiterCount <= DL_MAX_SLOT_COUNT);
        // 클라이언트에서 최대 관전자 수를 설정할 수 있다
        //WN_ASSERT(observerCount <= DL_DEFAULT_OBSERVER_COUNT);
    }

    RoomId getRoomId() const {
        return idRoom_;
    }
    size_t getSlotCount() const {
        return slotCount_;
    }
    size_t getWaiterCount() const {
        return waiterCount_;
    }
    size_t getObserverCount() const {
        return observerCount_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameRoomWaiterCountChangedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    size_t waiterCount_;
    size_t slotCount_;
    size_t observerCount_;
};


/**
* @class PartGameRoomMessage
*
* 대화방 퇴장 요청
* - 응답: PartGameRoomResultMessage
*/
class WARNET_API PartGameRoomMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_PART_GAME_ROOM };
public:
    explicit PartGameRoomMessage(RoomId roomId = INVALID_ROOMID,
            PartReason reason = PR_HIMSELF) :
        idRoom_(roomId),
        partReason_(reason) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    PartReason getPartReason() const {
        return partReason_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "PartGameRoomMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    PartReason partReason_;
};


/**
* @class PartGameRoomResultMessage
*
* 대화방 퇴장 요청에 대한 결과
*/
class WARNET_API PartGameRoomResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_PART_GAME_ROOM };
public:
    explicit PartGameRoomResultMessage(RoomId roomId = INVALID_ROOMID,
            PartReason reason = PR_UNKNOWN, ErrorCode errorCode = EC_UNKNOWN) :
        idRoom_(roomId),
        partReason_(reason),
        errorCode_(errorCode) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    PartReason getPartReason() const {
        return partReason_;
    }
    ErrorCode getErrorCode() const {
        return errorCode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "PartGameRoomResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    PartReason partReason_;
    ErrorCode errorCode_;
};


/**
* @class GameRoomPartedMessage
*
* 사용자가 퇴장하였음을 방 대기자들에게 알린다
*/
class WARNET_API GameRoomPartedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAME_ROOM_PARTED };
public:
    explicit GameRoomPartedMessage(UserNo userNo = INVALID_USERNO,
            PartReason reason = PR_UNKNOWN) :
        userNo_(userNo),
        partReason_(reason) {}

    UserNo getUserNo() const {
        return userNo_;
    }
    PartReason getPartReason() const {
        return partReason_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameRoomPartedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo userNo_;
    PartReason partReason_;
};


/**
* @class ToggleGameRoomReadyFlagMessage
*
* 대화방 준비 상태 변경(토글) 요청
* - 응답: ToggleGameRoomReadyFlagResultMessage
*/
class WARNET_API ToggleGameRoomReadyFlagMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_TOGGLE_GAME_ROOM_READY_FLAG };
public:
    explicit ToggleGameRoomReadyFlagMessage(RoomId roomId = INVALID_ROOMID) :
        idRoom_(roomId) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ToggleGameRoomReadyFlagMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
};


/**
* @class ToggleGameRoomReadyFlagResultMessage
*
* 대화방 준비 상태 변경(토글) 요청에 대한 응답 (모든 방 대기자에게 전송됨)
*/
class WARNET_API ToggleGameRoomReadyFlagResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_TOGGLE_GAME_ROOM_READY_FLAG };
public:
    explicit ToggleGameRoomReadyFlagResultMessage(
            RoomId roomId = INVALID_ROOMID, UserNo userNo = INVALID_USERNO,
            ReadyFlag readyFlag = RF_UNKNOWN) :
        idRoom_(roomId),
        userNo_(userNo),
        readyFlag_(readyFlag) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    UserNo getUserNo() const {
        return userNo_;
    }
    ReadyFlag getReadyFlag() const {
        return readyFlag_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ToggleGameRoomReadyFlagResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    UserNo userNo_;
    ReadyFlag readyFlag_;
};


/**
* @class ReadyGameMessage
*
* 게임 준비 요청
* - 응답: 실패시 - ReadyGameResultMessage,
*         성공시 - GameReadiedMessage (모든 방 대기자에게 전송)
*/
class WARNET_API ReadyGameMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_READY_GAME };
public:
    explicit ReadyGameMessage(RoomId roomId = INVALID_ROOMID,
            Wdr::Octet startTimeout = 0) :
        idRoom_(roomId),
        startTimeout_(startTimeout) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    Wdr::Octet getStartTimeout() const {
        return startTimeout_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ReadyGameMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    Wdr::Octet startTimeout_; ///< 게임 시작 대기 시간(초). 0이면 시작하지 않음
};


/**
* @class ReadyGameResultMessage
*
* 게임 준비 요청에 대한 결과
*
* 에러코드:
* - EC_OK
* - EC_ROOM_CANT_READY
* - EC_UDP_ADDRESS_IS_NOT_SETTED
*/
class WARNET_API ReadyGameResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_READY_GAME };
public:
    explicit ReadyGameResultMessage(RoomId roomId = INVALID_ROOMID,
            ErrorCode errorCode = EC_UNKNOWN) :
        idRoom_(roomId),
        errorCode_(errorCode) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    ErrorCode getErrorCode() const {
        return errorCode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ReadyGameResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    ErrorCode errorCode_;
};


/**
* @class GameReadiedMessage
*
* 게임이 준비되었음을 모든 방 참여자들에게 알린다
*/
class WARNET_API GameReadiedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAME_READIED };
public:
    explicit GameReadiedMessage(RoomId roomId = INVALID_ROOMID) :
        idRoom_(roomId) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameReadiedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
};


/**
* @class GameStartedMessage
*
* 게임 시작 요청
*/
class WARNET_API GameStartedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAME_STARTED };
public:
    explicit GameStartedMessage(RoomId roomId = INVALID_ROOMID,
            GameRoundId idGameRound = INVALID_GAMEROUNDID,
            const PlayerAddressInfos& infos = PlayerAddressInfos()) :
        idRoom_(roomId),
        idGameRound_(idGameRound),
        addressInfos_(infos) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    GameRoundId getGameRoundId() const {
        return idGameRound_;
    }
    const PlayerAddressInfos& getPlayerAddressInfos() const {
        return addressInfos_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameStartedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    GameRoundId idGameRound_;
    PlayerAddressInfos addressInfos_;
};


/**
* @class StopGameMessage
*
* 게임 중지 요청
* - 응답: 실패시 - StopGameResultMessage,
*         성공시 - GameStopedMessage (모든 방 대기자에게 전송)
*/
class WARNET_API StopGameMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_STOP_GAME };
public:
    explicit StopGameMessage(RoomId roomId = INVALID_ROOMID,
            const GameResult& result = GameResult()) :
        idRoom_(roomId),
        gameResult_(result) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    const GameResult& getGameResult() const {
        return gameResult_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "StopGameMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    GameResult gameResult_;
};


/**
* @class StopGameResultMessage
*
* 게임 중지 요청에 대한 결과
*/
class WARNET_API StopGameResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_STOP_GAME };
public:
    explicit StopGameResultMessage(RoomId roomId = INVALID_ROOMID,
            GameRoundId idGameRound = INVALID_GAMEROUNDID,
            ErrorCode errorCode = EC_UNKNOWN) :
        idRoom_(roomId),
        idGameRound_(idGameRound),
        errorCode_(errorCode) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    GameRoundId getGameRoundId() const {
        return idGameRound_;
    }
    ErrorCode getErrorCode() const {
        return errorCode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "StopGameResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    GameRoundId idGameRound_;
    ErrorCode errorCode_;
};


/**
* @class GameStoppedMessage
*
* 게임 중지 요청
*/
class WARNET_API GameStoppedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAME_STOPPED };
public:
    explicit GameStoppedMessage(RoomId roomId = INVALID_ROOMID,
            GameRoundId idGameRound = INVALID_GAMEROUNDID,
            const GameResult& result = GameResult(),
            const GameScores& scores = GameScores()/*,
            const CandyInfos& candyInfos = CandyInfos()*/) :
        idRoom_(roomId),
        idGameRound_(idGameRound),
        gameResult_(result),
        gameScores_(scores)/*,
        candyInfos_(candyInfos)*/ {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    GameRoundId getGameRoundId() const {
        return idGameRound_;
    }
    const GameResult& getGameResult() const {
        return gameResult_;
    }
    const GameScores& getGameScores() const {
        return gameScores_;
    }
    /*const CandyInfos& getCandyInfos() const {
        return candyInfos_;
    }*/
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameStoppedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    GameRoundId idGameRound_;
    GameResult gameResult_;
    GameScores gameScores_;
    /*CandyInfos candyInfos_;*/
};


/**
* @class GameRoomStateChangedMessage
*
* 게임방 상태가 변경됬음을 모든 채널 대기자들에게 알린다
*/
class WARNET_API GameRoomStateChangedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAME_ROOM_STATE_CHANGED };
public:
    explicit GameRoomStateChangedMessage(RoomId roomId = INVALID_ROOMID,
            GameRoomState state = GRS_UNKNOWN) :
        idRoom_(roomId),
        state_(state) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    GameRoomState getState() const {
        return state_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameRoomStateChangedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    GameRoomState state_;
};


/**
* @class SetGameRoomSlotStateMessage
*
* 게임방 슬롯을 닫거나 연다(방장 권한)
* - 응답: GameRoomSlotStateChangedMessage
* - 통지: GameRoomWaiterCountChangedMessage (채널 대기자에게 알린다)
*/
class WARNET_API SetGameRoomSlotStateMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_SET_GAME_ROOM_SLOT_STATE };
public:
    explicit SetGameRoomSlotStateMessage(RoomId roomId = INVALID_ROOMID,
            SlotIndex slotIndex = INVALID_SLOTINDEX,
            SlotState slotState = SS_UNKNOWN) :
        idRoom_(roomId),
        slotIndex_(slotIndex),
        slotState_(slotState) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    SlotState getSlotState() const {
        return slotState_;
    }
    SlotIndex getSlotIndex() const {
        return slotIndex_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "SetGameRoomSlotStateMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    SlotIndex slotIndex_;
    SlotState slotState_;
};


/**
* @class GameRoomSlotStateChangedMessage
*
* 게임방 슬롯의 상태(열리거나 닫힌)가 변경되었음을 방 참여자에게 알린다
*/
class WARNET_API GameRoomSlotStateChangedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAME_ROOM_SLOT_STATE_CHANGED };
public:
    explicit GameRoomSlotStateChangedMessage(RoomId roomId = INVALID_ROOMID,
            SlotIndex slotIndex = INVALID_SLOTINDEX,
            SlotState slotState = SS_UNKNOWN) :
        idRoom_(roomId),
        slotIndex_(slotIndex),
        slotState_(slotState) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    SlotState getSlotState() const {
        return slotState_;
    }
    SlotIndex getSlotIndex() const {
        return slotIndex_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameRoomSlotStateChangedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    SlotIndex slotIndex_;
    SlotState slotState_;
};


/**
* @class LeaveGameRoomSlotMessage
*
* 슬롯 대기자가 관전 대기자로 전환을 요청한다
* - 응답: LeaveGameRoomSlotResultMessage
*/
class WARNET_API LeaveGameRoomSlotMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_LEAVE_GAME_ROOM_SLOT };
public:
    explicit LeaveGameRoomSlotMessage(RoomId roomId = INVALID_ROOMID) :
        idRoom_(roomId) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "LeaveGameRoomSlotMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
};


/**
* @class LeaveGameRoomSlotResultMessage
*
* 슬롯 대기자의 관전 대기자 전환을 요청에 대한 응답 & 이벤트
* - 성공할 경우 모든 게임방 대기자들에게 전송한다
*/
class WARNET_API LeaveGameRoomSlotResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_LEAVE_GAME_ROOM_SLOT };
public:
    explicit LeaveGameRoomSlotResultMessage(RoomId roomId = INVALID_ROOMID,
            UserNo userNo = INVALID_USERNO, ErrorCode errorCode = EC_UNKNOWN, Wdr::Short entryNumber = 0) :
        idRoom_(roomId),
        userNo_(userNo),
        errorCode_(errorCode),
        entryNumber_(entryNumber)
        {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    UserNo getUserNo() const {
        return userNo_;
    }
    ErrorCode getErrorCode() const {
        return errorCode_;
    }
    Wdr::Short getEntryNumber() const {
        return entryNumber_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "LeaveGameRoomSlotResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    UserNo userNo_;
    ErrorCode errorCode_;
    Wdr::Short entryNumber_;
};


/**
* @class EnterGameRoomSlotMessage
*
* 관전 대기자가 슬롯 대기자로 전환을 요청한다
* - 응답: EnterGameRoomSlotResultMessage
*/
class WARNET_API EnterGameRoomSlotMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_ENTER_GAME_ROOM_SLOT };
public:
    explicit EnterGameRoomSlotMessage(RoomId roomId = INVALID_ROOMID) :
        idRoom_(roomId) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "EnterGameRoomSlotMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
};


/**
* @class EnterGameRoomSlotResultMessage
*
* 관전 대기자의 슬롯 대기자 전환을 요청에 대한 응답 & 이벤트
* - 성공할 경우 모든 게임방 대기자들에게 전송한다
*/
class WARNET_API EnterGameRoomSlotResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_ENTER_GAME_ROOM_SLOT };
public:
    explicit EnterGameRoomSlotResultMessage(RoomId roomId = INVALID_ROOMID,
            UserNo userNo = INVALID_USERNO,
            SlotIndex slotIndex = INVALID_SLOTINDEX,
            ErrorCode errorCode = EC_UNKNOWN) :
        idRoom_(roomId),
        userNo_(userNo),
        slotIndex_(slotIndex),
        errorCode_(errorCode) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    UserNo getUserNo() const {
        return userNo_;
    }
    ErrorCode getErrorCode() const {
        return errorCode_;
    }
    SlotIndex getSlotIndex() const {
        return slotIndex_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "EnterGameRoomSlotResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    UserNo userNo_;
    SlotIndex slotIndex_;
    ErrorCode errorCode_;
};


/**
* @class GameRoomOwnerChangedMessage
*
* 방장이 바꼈음을 모든 방 참여자에게 알린다
*/
class WARNET_API GameRoomOwnerChangedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAME_ROOM_OWNER_CHANGED };
public:
    explicit GameRoomOwnerChangedMessage(RoomId roomId = INVALID_ROOMID,
            UserNo userNo = INVALID_USERNO) :
        idRoom_(roomId),
        ownerUserNo_(userNo) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    UserNo getOwnerUserNo() const {
        return ownerUserNo_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameRoomOwnerChangedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    UserNo ownerUserNo_;
};


/**
* @class RelayRoomCreatedMessage
*
* 관전방이 생성되었음을 관전 대기자들에게 알린다
*/
class WARNET_API RelayRoomCreatedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_RELAY_ROOM_CREATED };
public:
    explicit RelayRoomCreatedMessage(const AddressPair& pair = AddressPair(),
            RoomId idGameRoom = INVALID_ROOMID,
            RoomId idRelayRoom = INVALID_ROOMID,
            const LoginCookie& cookie = LoginCookie()) :
        relayServerAddressPair_(pair),
        idGameRoom_(idGameRoom),
        idRelayRoom_(idRelayRoom),
        cookie_(cookie) {}

    const AddressPair& getRelayServerAddress() const {
        return relayServerAddressPair_;
    }
    RoomId getGameRoomId() const {
        return idGameRoom_;
    }
    RoomId getRelayRoomId() const {
        return idRelayRoom_;
    }
    const LoginCookie& getCookie() const {
        return cookie_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "RelayRoomCreatedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    AddressPair relayServerAddressPair_;
    RoomId idGameRoom_;
    RoomId idRelayRoom_;
    LoginCookie cookie_;
};


/**
* @class CoinUpdatedMessage
*
* 코인이 소모/반환되었음을 모든 방 참여자들에게 알린다
*/
class WARNET_API CoinUpdatedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_COIN_UPDATED };
public:
    explicit CoinUpdatedMessage(UserNo userNo = INVALID_USERNO,
            Coin coin = 0) :
        userNo_(userNo),
        coin_(coin) {}

    UserNo getUserNo() const {
        return userNo_;
    }
    Coin getCoin() const {
        return coin_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "CoinUpdatedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo userNo_;
    Coin coin_;
};


/**
* @class CharacterCardChangedMessage
*
* 게임 후 캐릭터 카드가 변경됬음을 모든 방 참여자들에게 알린다
*/
class WARNET_API CharacterCardChangedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_CHARACTER_CARD_CHANGED };
public:
    explicit CharacterCardChangedMessage(UserNo userNo = INVALID_USERNO,
            const CardProperty& property = CardProperty()) :
        userNo_(userNo),
        cardProperty_(property) {}

    UserNo getUserNo() const {
        return userNo_;
    }
    const CardProperty& getCardProperty() const {
        return cardProperty_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "CharacterCardChangedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo userNo_;
    CardProperty cardProperty_;
};


/**
* @class CancelGameMessage
*
* 게임을 취소한다(방장 권한)
*/
class WARNET_API CancelGameMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_CANCEL_GAME };
public:
    explicit CancelGameMessage(RoomId idRoom = INVALID_ROOMID,
            CancelReason reason = CR_MISSYNC) :
        idRoom_(idRoom),
        cancelReason_(reason) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    CancelReason getCancelReason() const {
        return cancelReason_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "CancelGameMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    CancelReason cancelReason_;
};


/**
* @class GameCanceledMessage
*
* 게임이 취소되었음을 방 참여자 전원에게 알린다
*/
class WARNET_API GameCanceledMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAME_CANCELED };
public:
    explicit GameCanceledMessage(RoomId idRoom = INVALID_ROOMID,
            GameRoundId idGameRound = INVALID_GAMEROUNDID,
            CancelReason reason = CR_UNKNOWN) :
        idRoom_(idRoom),
        idGameRound_(idGameRound),
        cancelReason_(reason) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    GameRoundId getGameRoundId() const {
        return idGameRound_;
    }
    CancelReason getCancelReason() const {
        return cancelReason_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameCanceledMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    GameRoundId idGameRound_;
    CancelReason cancelReason_;
};


/**
* @class InviteMessage
*
* 채널 대기자를 게임방에 초대한다(방장 권한)
* - 응답: InviteResultMessage
* - 상대방: InvitedMessage
*/
class WARNET_API InviteMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_INVITE };
public:
    explicit InviteMessage(RoomId idRoom = INVALID_ROOMID,
            UserNo invitee = INVALID_USERNO) :
        idRoom_(idRoom),
        invitee_(invitee) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    UserNo getInvitee() const {
        return invitee_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "InviteMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    UserNo invitee_;
};


/**
* @class InviteResultMessage
*
* 채널 대기자를 게임방에 초대한 결과
*/
class WARNET_API InviteResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_INVITE };
public:
    explicit InviteResultMessage(RoomId idRoom = INVALID_ROOMID,
            UserNo invitee = INVALID_USERNO,
            ErrorCode errorCode = EC_UNKNOWN) :
        idRoom_(idRoom),
        invitee_(invitee),
        errorCode_(errorCode) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    UserNo getInvitee() const {
        return invitee_;
    }
    ErrorCode getErrorCode() const {
        return errorCode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "InviteResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    UserNo invitee_;
    ErrorCode errorCode_;
};


/**
* @class InvitedMessage
*
* 게임방에서 초대 메세지가 도착하였다
*/
class WARNET_API InvitedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_INVITED };
public:
    explicit InvitedMessage(UserNo userNo = INVALID_USERNO,
            const std::string& nickname = "",
            RoomId idRoom = INVALID_ROOMID, const std::string& password = "") :
        userNo_(userNo),
        nickname_(nickname),
        idRoom_(idRoom),
        password_(password) {}

    UserNo getUserNo() const {
        return userNo_;
    }
    const std::string& getNickname() const {
        return nickname_;
    }
    RoomId getRoomId() const {
        return idRoom_;
    }
    const std::string& getPassword() const {
        return password_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "InvitedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo userNo_; ///< 초대한 방장의 userNo
    std::string nickname_; ///< 초대한 방장의 대화명
    RoomId idRoom_; ///< 초대한 방 번호
    std::string password_; ///< 초대한 방 비밀번호
};


/**
* @class CancelInvitationMessage
*
* 게임방 초대를 취소한다
* - 응답: 상대방 - InvitationCanceledMessage
*/
class WARNET_API CancelInvitationMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_CANCEL_INVITATION };
public:
    explicit CancelInvitationMessage(RoomId idRoom = INVALID_ROOMID,
            UserNo invitee = INVALID_USERNO) :
        idRoom_(idRoom),
        invitee_(invitee) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    UserNo getInvitee() const {
        return invitee_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "CancelInvitationMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    UserNo invitee_;
};


/**
* @class InvitationCanceledMessage
*
* 게임방 초대가 취소되었다.
*/
class WARNET_API InvitationCanceledMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_INVITATION_CANCELED };
public:
    explicit InvitationCanceledMessage(RoomId idRoom = INVALID_ROOMID,
            UserNo inviter = INVALID_USERNO,
            InvitationRejectionReason reason = IRR_UNKNOWN) :
        idRoom_(idRoom),
        inviter_(inviter),
        reason_(reason) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    UserNo getInviter() const {
        return inviter_;
    }
    InvitationRejectionReason getReason() const {
        return reason_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "InvitationCanceledMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    UserNo inviter_; ///< 초대한 방장 UserNo
    InvitationRejectionReason reason_;
};


/**
* @class RejectInvitationMessage
*
* 초대를 거절한다
* - 응답: 상대방 - InvitationRejectedMessage
*/
class WARNET_API RejectInvitationMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_REJECT_INVITATION };
public:
    explicit RejectInvitationMessage(RoomId idRoom = INVALID_ROOMID,
            UserNo inviter = INVALID_USERNO,
            InvitationRejectionReason reason = IRR_UNKNOWN) :
        idRoom_(idRoom),
        inviter_(inviter),
        reason_(reason) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    UserNo getInviter() const {
        return inviter_;
    }
    InvitationRejectionReason getReason() const {
        return reason_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "RejectInvitationMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    UserNo inviter_; ///< 초대한 UserNo
    InvitationRejectionReason reason_; ///< 거절 사유
};


/**
* @class InvitationRejectedMessage
*
* 상대방이 초대를 거절하였다
*/
class WARNET_API InvitationRejectedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_INVITATION_REJECTED };
public:
    explicit InvitationRejectedMessage(RoomId idRoom = INVALID_ROOMID,
            UserNo invitee = INVALID_USERNO,
            InvitationRejectionReason reason = IRR_UNKNOWN) :
        idRoom_(idRoom),
        invitee_(invitee),
        reason_(reason) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    UserNo getInvitee() const {
        return invitee_;
    }
    InvitationRejectionReason getReason() const {
        return reason_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "InvitationRejectedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    UserNo invitee_; ///< 초대받은 UserNo
    InvitationRejectionReason reason_; ///< 거절 사유
};


/**
* @class KickRoomUserMessage
*
* 게임방에서 사용자를 kick/ban한다(방장 기능)
* - 응답: 상대방 - GameRoomPartedMessage
*/
class WARNET_API KickRoomUserMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_KICK_ROOM_USER };
public:
    explicit KickRoomUserMessage(RoomId idRoom = INVALID_ROOMID,
            UserNo kickee = INVALID_USERNO, PartReason reason = PR_UNKNOWN) :
        idRoom_(idRoom),
        kickee_(kickee),
        partReason_(reason) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    UserNo getKickee() const {
        return kickee_;
    }
    PartReason getPartReason() const {
        return partReason_;
    }

private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "KickRoomUserMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    UserNo kickee_; ///< 강퇴시킬 UserNo
    PartReason partReason_;
};


/**
* @class ChangeGameModeMessage
*
* 게임 모드 변경 요청
* - 응답: ChangeGameModeResultMessage
* - 통지: GameModeChangedMessage (모든 방 참여자 및 채널 대기자에게)
*/
class WARNET_API ChangeGameModeMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_CHANGE_GAME_MODE };
public:
    explicit ChangeGameModeMessage(RoomId roomId = INVALID_ROOMID,
            GameModeType gameMode = GM_UNKNOWN) :
        idRoom_(roomId),
        gameMode_(gameMode) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    GameModeType getGameMode() const {
        return gameMode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ChangeGameModeMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    GameModeType gameMode_;
};


/**
* @class ChangeGameModeResultMessage
*
* 게임 모드 변경 요청에 대한 응답
*/
class WARNET_API ChangeGameModeResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_CHANGE_GAME_MODE };
public:
    explicit ChangeGameModeResultMessage(RoomId roomId = INVALID_ROOMID,
            GameModeType gameMode = GM_UNKNOWN,
            ErrorCode errorCode = EC_UNKNOWN) :
        idRoom_(roomId),
        gameMode_(gameMode),
        errorCode_(errorCode) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    GameModeType getGameMode() const {
        return gameMode_;
    }
    ErrorCode getErrorCode() const {
        return errorCode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ChangeGameModeResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    GameModeType gameMode_;
    ErrorCode errorCode_;
};


/**
* @class GameModeChangedMessage
*
* 게임 모드가 변경되었음을 모든 방 참여자 및 채널 대기자에게 알린다
*/
class WARNET_API GameModeChangedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAME_MODE_CHANGED };
public:
    explicit GameModeChangedMessage(RoomId roomId = INVALID_ROOMID,
            GameModeType gameMode = GM_UNKNOWN) :
        idRoom_(roomId),
        gameMode_(gameMode) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    GameModeType getGameMode() const {
        return gameMode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameModeChangedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    GameModeType gameMode_;
};


/**
* @class ChallengeMessage
*
* 싱글 모드 게임 중인 방에서 대전 신청을 한다
* - 응답: ChallengeResultMessage
* - 통지: ChallengedMessage (방장에게 대전 신청이 왔음을 알린다)
*/
class WARNET_API ChallengeMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_CHALLENGE };
public:
    explicit ChallengeMessage(RoomId roomId = INVALID_ROOMID) :
        idRoom_(roomId) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ChallengeMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
};


/**
* @class ChallengeResultMessage
*
* 대전 신청에 대한 결과
*/
class WARNET_API ChallengeResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_CHALLENGE };
public:
    explicit ChallengeResultMessage(RoomId roomId = INVALID_ROOMID,
            ErrorCode errorCode = EC_UNKNOWN) :
        idRoom_(roomId),
        errorCode_(errorCode) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    ErrorCode getErrorCode() const {
        return errorCode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ChallengeResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    ErrorCode errorCode_;
};


/**
* @class ChallengedMessage
*
* 대전 신청이 왔음을 방장에게 알린다
*/
class WARNET_API ChallengedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_CHALLENGED };
public:
    explicit ChallengedMessage(RoomId roomId = INVALID_ROOMID,
            UserNo from = INVALID_USERNO) :
        idRoom_(roomId),
        from_(from) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    UserNo getFrom() const {
        return from_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ChallengedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    UserNo from_;
};


/**
* @class InsertCoinMessage
*
* 게임 중 코인을 투여한다
* - 응답: InsertCoinResultMessage
* - 이벤트: CoinUpdatedMessage
*/
class WARNET_API InsertCoinMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_INSERT_COIN };
public:
    explicit InsertCoinMessage(RoomId roomId = INVALID_ROOMID) :
        idRoom_(roomId) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "InsertCoinMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
};


/**
* @class InsertCoinResultMessage
*
* 게임 중 코인을 투여한다
* - 응답: 없음
*/
class WARNET_API InsertCoinResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_INSERT_COIN };
public:
    explicit InsertCoinResultMessage(RoomId roomId = INVALID_ROOMID,
            ErrorCode errorCode = EC_UNKNOWN) :
        idRoom_(roomId),
        errorCode_(errorCode) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    ErrorCode getErrorCode() const {
        return errorCode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "InsertCoinResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    ErrorCode errorCode_;
};


/**
* @class PingTimeMessage
*
* 핑타임을 설정을 요청한다
* - 슬롯 대기자가 게임방 안에서 요청할 경우 다른 모든 참여자에게
*   PingTimeChangedMessage 통지가 온다
*/
class WARNET_API PingTimeMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_PINGTIME };
public:
    PingTimeMessage(Wdr::UShort pingTime = 0xFFFF) :
        pingTime_(pingTime) {}

    Wdr::UShort getPingTime() const {
        return pingTime_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "PingTimeMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    Wdr::UShort pingTime_;
};


/**
* @class PingTimeChangedMessage
*
* 게임방에서 플레이어의 핑타임이 변경되었다
*/
class WARNET_API PingTimeChangedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_PINGTIME_CHANGED };
public:
    explicit PingTimeChangedMessage(RoomId roomId = INVALID_ROOMID,
            UserNo userNo = INVALID_USERNO, Wdr::UShort pingTime = 0xFFFF) :
        idRoom_(roomId),
        userNo_(userNo),
        pingTime_(pingTime) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    UserNo getUserNo() const {
        return userNo_;
    }
    Wdr::UShort getPingTime() const {
        return pingTime_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "PingTimeChangedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    UserNo userNo_;
    Wdr::UShort pingTime_;
};


/**
* @class SelectGameRoundMessage
*
* 관전할 게임 라운드를 선택한다
* - 응답: SelectGameRoundResultMessage
* - 이벤트: GameRoundSelectedMessage
*/
class WARNET_API SelectGameRoundMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_SELECT_GAMEROUND };
public:
    explicit SelectGameRoundMessage(RoomId roomId = INVALID_ROOMID,
            GameRoundId idGameRound = INVALID_GAMEROUNDID) :
        idRoom_(roomId),
        idGameRound_(idGameRound) {}

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
        return "SelectGameRoundMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    GameRoundId idGameRound_;
};


/**
* @class SelectGameRoundResultMessage
*
* 관전할 게임 라운드 선택에 대한 응답
*
* 에러코드:
* - EC_OK
* - EC_PLAYER_CANT_SELECT_GAMEROUND
* - EC_ALREADY_SELECT_GAMEROUND
* - EC_GAMEROUND_NOT_FOUND
*/
class WARNET_API SelectGameRoundResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_SELECT_GAMEROUND };
public:
    explicit SelectGameRoundResultMessage(RoomId roomId = INVALID_ROOMID,
            GameRoundId idGameRound = INVALID_GAMEROUNDID,
            ErrorCode errorCode = EC_UNKNOWN) :
        idRoom_(roomId),
        idGameRound_(idGameRound),
        errorCode_(errorCode) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    GameRoundId getGameRoundId() const {
        return idGameRound_;
    }
    ErrorCode getErrorCode() const {
        return errorCode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "SelectGameRoundResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    GameRoundId idGameRound_;
    ErrorCode errorCode_;
};


/**
* @class GameRoundSelectedMessage
*
* 관전자의 게임 라운드 선택을 모든 참여자에게 알린다
*/
class WARNET_API GameRoundSelectedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAMEROUND_SELECTED };
public:
    explicit GameRoundSelectedMessage(RoomId roomId = INVALID_ROOMID,
            GameRoundId idGameRound = INVALID_GAMEROUNDID,
            UserNo observer = INVALID_USERNO) :
        idRoom_(roomId),
        idGameRound_(idGameRound),
        observer_(observer) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    GameRoundId getGameRoundId() const {
        return idGameRound_;
    }
    UserNo getObserver() const {
        return observer_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameRoundSelectedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    GameRoundId idGameRound_;
    UserNo observer_; ///< 게임라운드를 선택한 관전자
};


/**
* @class GameRoundsCreatedMessage
*
* 게임 라운드가 생성되었음을 모든 참여자에게 알린다
*/
class WARNET_API GameRoundsCreatedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAMEROUND_CREATED };
public:
    explicit GameRoundsCreatedMessage(RoomId roomId = INVALID_ROOMID,
            const GameRoundInfos& gameRoundInfos = GameRoundInfos()/*,
            const CandyInfos& candyInfos = CandyInfos()*/) :
        idRoom_(roomId),
        gameRoundInfos_(gameRoundInfos)/*,
        candyInfos_(candyInfos) */{}

    RoomId getRoomId() const {
        return idRoom_;
    }
    const GameRoundInfos& getGameRoundInfos() const {
        return gameRoundInfos_;
    }
    /*const CandyInfos& getCandyInfos() const {
        return candyInfos_;
    }*/
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameRoundsCreatedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    GameRoundInfos gameRoundInfos_;
    /*CandyInfos candyInfos_;*/
};


/**
* @class GameSlotsChangedMessage
*
* 게임 슬롯 정보가 변경되었음을 모든 참여자에게 알린다
*/
class WARNET_API GameSlotsChangedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAMESLOTS_CHANGED };
public:
    explicit GameSlotsChangedMessage(RoomId roomId = INVALID_ROOMID,
            const DetailedSlotInfos& detailedSlotInfos = DetailedSlotInfos()) :
        idRoom_(roomId),
        detailedSlotInfos_(detailedSlotInfos) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    const DetailedSlotInfos& getDetailedSlotInfos() const {
        return detailedSlotInfos_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GameSlotsChangedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    DetailedSlotInfos detailedSlotInfos_;
};


/**
* @class NotifyNoticeMessage
*
* 공지사항 메세지를 접속 중인 모든 사용자에게 뿌린다
* - 현재는 누구나 보낼 수 있다(오픈 버전에서는 기능 제한을 해야됨)
* - 응답: 상대방 - NoticeMessage
*/
class WARNET_API NotifyNoticeMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_NOTICE };
public:
    explicit NotifyNoticeMessage(const std::string& notice = "") :
        notice_(notice) {}

    const std::string& getNotice() const {
        return notice_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "NotifyNoticeMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string notice_;
};


/**
* @class NoticeMessage
*
* 공지사항이 통보되었다
*/
class WARNET_API NoticeMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_NOTICE };
public:
    explicit NoticeMessage(const std::string& nickname = "",
            const std::string& notice = "") :
        nickname_(nickname),
        notice_(notice) {}

    const std::string& getNickname() const {
        return nickname_;
    }
    const std::string& getNotice() const {
        return notice_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "NoticeMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string nickname_;
    std::string notice_;
};


/**
* @class ReportMessage
*
* 신고한다
* - 응답: ReportResultMessage
*/
class WARNET_API ReportMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_REPORT };
public:
    explicit ReportMessage(const std::string& reportNickname = "",
            ReportReason rr = RR_UNKNOWN, const std::string& contents = "",
            const std::string& log = "") :
        reportNickname_(reportNickname),
        reportReason_(rr),
        reportContents_(contents),
        reportLog_(log) {}

    const std::string& getReportNickname() const {
        return reportNickname_;
    }
    ReportReason getReportReason() const {
        return reportReason_;
    }
    const std::string& getReportContents() const {
        return reportContents_;
    }
    const std::string& getReportLog() const {
        return reportLog_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ReportMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string reportNickname_; ///< 신고자 닉네임
    ReportReason reportReason_; ///< 신고 사유
    std::string reportContents_; ///< 신고 내용
    std::string reportLog_; ///< 채팅 로그
};


/**
* @class ReportResultMessage
*
* 신고에 대한 결과 메세지
*/
class WARNET_API ReportResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_REPORT };
public:
    explicit ReportResultMessage(const std::string& reportNickname = "",
            ErrorCode ec = EC_UNKNOWN) :
        reportNickname_(reportNickname),
        errorCode_(ec) {}

    const std::string& getReportNickname() const {
        return reportNickname_;
    }
    ErrorCode getErrorCode() const {
        return errorCode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ReportResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string reportNickname_; ///< 신고자 닉네임
    ErrorCode errorCode_; ///< 에러 코드
};


/**
* @class HeartbeatMessage
*
* heart-beat 메세지 (주기적으로 서버에 보내야 한다)
*/
class WARNET_API HeartbeatMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_HEARTBEAT };
public:
    HeartbeatMessage() {}
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "HeartbeatMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
};


/**
* @class MarqueeMessage
*
* 전광판이 변경되었다
*/
class WARNET_API MarqueeMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_MARQUEE };
public:
    explicit MarqueeMessage(const std::string& marquee = "") :
        marquee_(marquee) {}

    const std::string& getMarquee() const {
        return marquee_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "MarqueeMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string marquee_;
};


/**
* @class ChangeCurrentGameTypeMessage
*
* 게임 타이틀 변경을 요청한다
* - 응답: 없음
* - 통지: CurrentGameTypeChanged
*/
class WARNET_API ChangeCurrentGameTypeMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_CHANGE_GAMETYPE };
public:
    explicit ChangeCurrentGameTypeMessage(GameType gameType = GT_UNKNOWN) :
        gameType_(gameType) {}

    GameType getGameType() const {
        return gameType_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ChangeCurrentGameTypeMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    GameType gameType_;
};


/**
* @class CurrentGameTypeChangedMessage
*
* 게임 타이틀이 변경되었음을 모든 채널 대기자에게 알린다
*/
class WARNET_API CurrentGameTypeChangedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAMETYPE_CHANGED };
public:
    explicit CurrentGameTypeChangedMessage(UserNo userNo = INVALID_USERNO,
            GameType gameType = GT_UNKNOWN) :
        userNo_(userNo),
        gameType_(gameType) {}

    UserNo getUserNo() const {
        return userNo_;
    }
    GameType getGameType() const {
        return gameType_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "CurrentGameTypeChangedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo userNo_;
    GameType gameType_;
};


/**
* @class GamerOnlineMessage
*
* 게이머가 로그인했음을 모든 채널 대기자에게 알린다
*/
class WARNET_API GamerOnlineMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAMER_ONLINE };
public:
    explicit GamerOnlineMessage(const Gamer& gamer = Gamer()) :
        gamer_(gamer) {}

    const Gamer& getGamer() const {
        return gamer_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GamerOnlineMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    Gamer gamer_;
};


/**
* @class GamerOfflineMessage
*
* 게이머가 로그아웃했음을 모든 채널 대기자에게 알린다
*/
class WARNET_API GamerOfflineMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_GAMER_OFFLINE };
public:
    explicit GamerOfflineMessage(UserNo userNo = INVALID_USERNO) :
        userNo_(userNo) {}

    UserNo getUserNo() const {
        return userNo_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "GamerOfflineMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo userNo_;
};


/**
* @class PositionChangedMessage
*
* 게이머의 현재 위치가 변경되었음을 모든 채널 대기자에게 알린다
*/
class WARNET_API PositionChangedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_POSITION_CHANGED };
public:
    explicit PositionChangedMessage(UserNo userNo = INVALID_USERNO,
            RoomId idRoom = INVALID_ROOMID) :
        userNo_(userNo),
        idRoom_(idRoom) {}

    UserNo getUserNo() const {
        return userNo_;
    }
    RoomId getPosition() const {
        return idRoom_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "PositionChangedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo userNo_;
    RoomId idRoom_;
};


/**
* @class LoadGameOptionsMessage
*
* 게임별 사용자 옵션을 요청한다
* - 응답: LoadGameOptionsResultMessage
*/
class WARNET_API LoadGameOptionsMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_LOAD_GAME_OPTIONS };
public:
    explicit LoadGameOptionsMessage(GameType gameType = GT_UNKNOWN) :
        gameType_(gameType) {}

    GameType getGameType() const {
        return gameType_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "LoadGameOptionsMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    GameType gameType_;
};


/**
* @class LoadGameOptionsResultMessage
*
* 게임별 사용자 옵션을 요청에 대한 결과
*/
class WARNET_API LoadGameOptionsResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_LOAD_GAME_OPTIONS };
public:
    explicit LoadGameOptionsResultMessage(GameType gameType = GT_UNKNOWN,
            const std::string& options = "") :
        gameType_(gameType),
        options_(options) {}

    GameType getGameType() const {
        return gameType_;
    }

    const std::string& getOptions() const {
        return options_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "LoadGameOptionsResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    GameType gameType_;
    std::string options_;
};


/**
* @class SaveGameOptionsMessage
*
* 게임별 사용자 옵션을 저장할 것을 요청한다
* - 응답: 없음
*/
class WARNET_API SaveGameOptionsMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_SAVE_GAME_OPTIONS };
public:
    explicit SaveGameOptionsMessage(GameType gameType = GT_UNKNOWN,
            const std::string& options = "") :
        gameType_(gameType),
        options_(options) {}

    GameType getGameType() const {
        return gameType_;
    }

    const std::string& getOptions() const {
        return options_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "SaveGameOptionsMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    GameType gameType_; ///< 게임 옵션 게임 타입
    std::string options_; ///< 게임 옵션
};


/**
* @class LoadGlobalOptionsMessage
*
* 전체 사용자 옵션을 요청한다
* - 응답: LoadGlobalOptionsResultMessage
*/
class WARNET_API LoadGlobalOptionsMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_LOAD_GLOBAL_OPTIONS };
public:
    explicit LoadGlobalOptionsMessage() {}
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "LoadGlobalOptionsMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
};


/**
* @class LoadGlobalOptionsResultMessage
*
* 게임별 사용자 옵션을 요청에 대한 결과
*/
class WARNET_API LoadGlobalOptionsResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_LOAD_GLOBAL_OPTIONS };
public:
    explicit LoadGlobalOptionsResultMessage(const std::string& options = "") :
        options_(options) {}

    const std::string& getOptions() const {
        return options_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "LoadGlobalOptionsResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string options_;
};


/**
* @class SaveGlobalOptionsMessage
*
* 전체 게임 사용자 옵션을 저장할 것을 요청한다
* - 응답: 없음
*/
class WARNET_API SaveGlobalOptionsMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_SAVE_GLOBAL_OPTIONS };
public:
    explicit SaveGlobalOptionsMessage(const std::string& options = "") :
        options_(options) {}

    const std::string& getOptions() const {
        return options_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "SaveGlobalOptionsMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string options_; ///< 옵션
};


/**
* @class SetRoomChattingMessage
*
* 방 채팅 가능 여부를 설정한다(운영자 기능)
* - 응답: 없음
* - 통지: RoomChattingSettedMessage
*/
class WARNET_API SetRoomChattingMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_SET_ROOM_CHATTING };
public:
    explicit SetRoomChattingMessage(RoomId idRoom = INVALID_ROOMID,
            bool enable = false) :
        idRoom_(idRoom),
        enable_(enable) {}

    RoomId getRoomId() const {
        return idRoom_;
    }
    bool isEnabled() const {
        return enable_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "SetRoomChattingMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    /// true면 모든 참여자 채팅 가능(default),
    /// false이면 플레이어, 운영자만 채팅 가능
    bool enable_;
};


/**
* @class RoomChattingSettedMessage
*
* 방 채팅 옵션이 설정되었다(방 참여자 전원에게 통지)
*/
class WARNET_API RoomChattingSettedMessage : public SetRoomChattingMessage
{
public:
    enum { thisMessageType = MT_GS_S2C_ROOM_CHATTING_SETTED };
public:
    explicit RoomChattingSettedMessage(RoomId idRoom = INVALID_ROOMID,
            bool enable = false) :
        SetRoomChattingMessage(idRoom, enable) {}
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "RoomChattingSettedMessage";
    }
};


/**
* @class KickUserMessage
*
* 사용자를 강제로 접속해제 시킨다 (운영자 기능)
* - 응답: 없음
* - 이벤트: UserKickedMessage
*/
class WARNET_API KickUserMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_KICK_USER };
public:
    explicit KickUserMessage(const std::string& kickee = "") :
        kickee_(kickee) {}

    const std::string& getKickee() const {
        return kickee_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "KickUserMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string kickee_; ///< 강제 접속해제 당할 놈 닉네임
};


/**
* @class UserKickedMessage
*
* 사용자가 강제로 접속 해제되었다 (현재 접속 중인 모든 사용자에게)
*/
class WARNET_API UserKickedMessage : public KickUserMessage
{
public:
    enum { thisMessageType = MT_GS_S2C_USER_KICKED };
public:
    explicit UserKickedMessage(const std::string& kickee = "") :
        KickUserMessage(kickee) {}
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "UserKickedMessage";
    }
};


/**
* @class MakeObserverMessage
*
* 사용자를 강제로 관전 대기자로 전환시킨다 (운영자 기능)
* - 응답: LeaveGameRoomSlotResultMessage
*/
class WARNET_API MakeObserverMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_MAKE_OBSERVER };
public:
    explicit MakeObserverMessage(RoomId idRoom = INVALID_ROOMID,
        const std::string& nickname = "") :
        idRoom_(idRoom),
        nickname_(nickname) {}

    RoomId getRoomId() const {
        return idRoom_;
    }

    const std::string& getNickname() const {
        return nickname_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "MakeObserverMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    std::string nickname_; ///< 관전 전환될 놈 닉네임
};


/**
* @class MakeSlotWaiterMessage
*
* 사용자를 강제로 슬롯 대기자로 전환시킨다 (운영자 기능)
* - 응답: EnterGameRoomSlotResultMessage
*/
class WARNET_API MakeSlotWaiterMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_MAKE_SLOTWAITER };
public:
    explicit MakeSlotWaiterMessage(RoomId idRoom = INVALID_ROOMID,
        const std::string& nickname = "") :
        idRoom_(idRoom),
        nickname_(nickname) {}

    RoomId getRoomId() const {
        return idRoom_;
    }

    const std::string& getNickname() const {
        return nickname_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "MakeSlotWaiterMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    RoomId idRoom_;
    std::string nickname_; ///< 관전 전환될 놈 닉네임
};


/**
* @class TournamentRewardMessage
*
* 토너먼트 보상을 받았다
* - 방에 있는 모든 사용자에게 전송된다
*/
class WARNET_API TournamentRewardMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_TOURNAMENT_REWARD };
public:
    explicit TournamentRewardMessage(UserNo player = INVALID_USERNO,
        TournamentResult result = trUnknown, Rupo rupo = 0) :
        player_(player),
        result_(result),
        rupo_(rupo) {}

    UserNo getPlayer() const {
        return player_;
    }

    TournamentResult getTournamentResult() const {
        return result_;
    }

    Rupo getRupo() const {
        return rupo_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "TournamentRewardMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo player_;
    TournamentResult result_;
    Rupo rupo_;
};


/**
* @class UnusedItemListMessage
*
* 미사용 슬롯 아이템 목록을 요청한다
* - 응답: UnusedItemListResultMessage
*/
class WARNET_API UnusedItemListMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_UNUSED_ITEM_LIST };
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "UnusedItemListMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
};


/**
* @class UnusedItemListResultMessage
*
* 미사용 슬롯 아이템 목록 요청에 대한 응답을 한다
*/
class WARNET_API UnusedItemListResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_UNUSED_ITEM_LIST };
public:
    explicit UnusedItemListResultMessage(
        const ItemInfos& unusedItemInfos = ItemInfos()) :
        unusedItemInfos_(unusedItemInfos) {}

    const ItemInfos& getUnusedItemInfos() const {
        return unusedItemInfos_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "UnusedItemListResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ItemInfos unusedItemInfos_; ///< 미사용 슬롯 아이템 목록
};


/**
* @class CardSlotItemInfoListMessage
*
* 카드 슬롯 아이템 정보 요청 메세지.
* - 응답: CardSlotItemInfoListResultMessage
*/
class WARNET_API CardSlotItemInfoListMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_CARD_SLOTITEM_LIST };
public:
    explicit CardSlotItemInfoListMessage(const std::string& id = "",
            const std::string& password = "",
            CardNumber cardNumber = INVALID_CARD_NUMBER) :
        userId_(id),
        password_(password),
        cardNumber_(cardNumber) {}

    const std::string& getUserId() const {
        return userId_;
    }
    const std::string& getPassword() const {
        return password_;
    }
    CardNumber getCharacterCardNumber() const {
        return cardNumber_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "CardSlotItemInfoListMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string userId_; ///< 사용자 아이디
    std::string password_; ///< 비밀번호
    CardNumber cardNumber_; ///< 캐릭터 카드 번호
};


/**
* @class CardSlotItemInfoListResultMessage
*
* 카드 슬롯 아이템 정보 요청에 대한 결과 메세지.
*/
class WARNET_API CardSlotItemInfoListResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_CARD_SLOTITEM_LIST };
public:
    explicit CardSlotItemInfoListResultMessage(
        CardNumber cardNumber = INVALID_CARD_NUMBER,
        const ItemInfos& itemInfos = ItemInfos()) :
        cardNumber_(cardNumber),
        itemInfos_(itemInfos) {}

    CardNumber getCharacterCardNumber() const {
        return cardNumber_;
    }

    const ItemInfos& getItemInfos() const {
        return itemInfos_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "CardSlotItemInfoListResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    CardNumber cardNumber_; ///< 캐릭터 카드 번호
    ItemInfos itemInfos_; ///< 슬롯 아이템 정보 목록
};


/**
* @class AttachSlotItemMessage
*
* 슬롯에 아이템을 장착한다
* - 응답: AttachSlotItemResultMessage
*/
class WARNET_API AttachSlotItemMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_ATTACH_SLOT_ITEM };
public:
    explicit AttachSlotItemMessage(CardNumber cardNumber = INVALID_CARD_NUMBER,
        ItemId itemId = INVALID_ITEMID) :
        cardNumber_(cardNumber),
        itemId_(itemId) {}

    CardNumber getCardNumber() const {
        return cardNumber_;
    }

    ItemId getItemId() const {
        return itemId_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "AttachSlotItemMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    CardNumber cardNumber_;
    ItemId itemId_;
};


/**
* @class AttachSlotItemResultMessage
*
* 아이템 장착 요청에 대한 응답을 한다
*/
class WARNET_API AttachSlotItemResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_ATTACH_SLOT_ITEM };
public:
    explicit AttachSlotItemResultMessage(ErrorCode ec = EC_UNKNOWN,
        CardNumber cardNumber = INVALID_CARD_NUMBER,
        CardSlotIndex slotIndex = INVALID_CARDSLOTINDEX,
        ItemId itemId = INVALID_ITEMID) :
        errorCode_(ec),
        cardNumber_(cardNumber),
        slotIndex_(slotIndex),
        itemId_(itemId) {}

    ErrorCode getErrorCode() const {
        return errorCode_;
    }

    CardNumber getCardNumber() const {
        return cardNumber_;
    }

    CardSlotIndex getCardSlotIndex() const {
        return slotIndex_;
    }

    ItemId getItemId() const {
        return itemId_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "AttachSlotItemResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ErrorCode errorCode_;
    CardNumber cardNumber_;
    CardSlotIndex slotIndex_;
    ItemId itemId_;
};


/**
* @class DeleteSlotItemMessage
*
* 슬롯 아이템을 삭제한다
* - 응답: DeleteSlotItemResultMessage
*/
class WARNET_API DeleteSlotItemMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_DELETE_SLOT_ITEM };
public:
    explicit DeleteSlotItemMessage(CardNumber cardNumber = INVALID_CARD_NUMBER,
        CardSlotIndex slotIndex = INVALID_CARDSLOTINDEX) :
        cardNumber_(cardNumber),
        slotIndex_(slotIndex) {}

    CardNumber getCardNumber() const {
        return cardNumber_;
    }

    CardSlotIndex getCardSlotIndex() const {
        return slotIndex_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "DeleteSlotItemMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    CardNumber cardNumber_;
    CardSlotIndex slotIndex_;
};


/**
* @class DeleteSlotItemResultMessage
*
* 슬롯 아이템 삭제 요청에 대한 응답을 한다
*/
class WARNET_API DeleteSlotItemResultMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_DELETE_SLOT_ITEM };
public:
    explicit DeleteSlotItemResultMessage(ErrorCode ec = EC_UNKNOWN,
        CardNumber cardNumber = INVALID_CARD_NUMBER,
        CardSlotIndex slotIndex = INVALID_CARDSLOTINDEX) :
        errorCode_(ec),
        cardNumber_(cardNumber),
        slotIndex_(slotIndex) {}

    ErrorCode getErrorCode() const {
        return errorCode_;
    }

    CardNumber getCardNumber() const {
        return cardNumber_;
    }

    CardSlotIndex getCardSlotIndex() const {
        return slotIndex_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "DeleteSlotItemResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ErrorCode errorCode_;
    CardNumber cardNumber_;
    CardSlotIndex slotIndex_;
};


/**
* @class SlotItemUsedMessage
*
* 슬롯 아이템 삭제 요청에 대한 응답을 한다
*/
class WARNET_API SlotItemUsedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_SLOT_ITEM_USED };
public:
    explicit SlotItemUsedMessage(UserNo userNo = INVALID_USERNO,
        ItemCode itemCode = INVALID_ITEMCODE) :
        userNo_(userNo),
        itemCode_(itemCode) {}

    UserNo getUserNo() const {
        return userNo_;
    }

    ItemCode getItemCode() const {
        return itemCode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "SlotItemUsedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo userNo_;
    ItemCode itemCode_;
};


/**
* @class UseMegaphoneMessage
*
* 확성기를 사용한다
* - 응답: MegaphoneUsedMessage, SlotItemUsedMessage
* - 이벤트: MegaphoneUsedMessage
*/
class WARNET_API UseMegaphoneMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_C2S_MEGAPHONE };
public:
    explicit UseMegaphoneMessage(const std::string& message = "") :
        message_(message) {}

    const std::string& getMessage() const {
        return message_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "UseMegaphoneMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string message_;
};


/**
* @class MegaphoneUsedMessage
*
* 확성기를 사용에 대한 응답 & 이벤트
*/
class WARNET_API MegaphoneUsedMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_MEGAPHONE };
public:
    explicit MegaphoneUsedMessage(const std::string& nickname = "",
        const std::string& message = "") :
        nickname_(nickname),
        message_(message) {}

    const std::string& getNickname() const {
        return nickname_;
    }
    const std::string& getMessage() const {
        return message_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "MegaphoneUsedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string nickname_;
    std::string message_;
};



////KIM YOUNG JUNG

/**
* @class KickLoginUserMessage
*
* 이미 접속되있는 사용자를 다시 접속한 유저가 차버린다.
*/

class WARNET_API KickLoginUserMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_KICK_LOGINUSER };
public:
    explicit KickLoginUserMessage() {}
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "KickLoginUserMessage";
    }
private:
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;

};


class WARNET_API AlterLoginOverlapUserMessage : public Message
{
public:
    enum { thisMessageType = MT_GS_S2C_ALTER_LOGIN_OVERLAPUSER };
public:
    explicit AlterLoginOverlapUserMessage() {}
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "AlterLoginOverlapUserMessage";
    }
private:
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
};


//class WARNET_API MtoGLoginMessage : public Message
//{
//public:
//    enum { thisMessageType = MT_GS_C2S_M_TO_G_LOGIN};
//public:
//    MtoGLoginMessage(const std::string& managerCookie) : 
//      managerCookie_(managerCookie){}
//public:
//    const std::string& getManagerCookie() const 
//    {
//        return managerCookie_;
//    }
//private:
//    virtual MessageType getThisMessageType() const {
//        return thisMessageType;
//    }
//    virtual const char* getThisClassName() const {
//        return "MtoGLoginMessage";
//    }
//private:
//    virtual bool readBody(Wdr::InputWdr& wdr);
//    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
//    virtual size_t getBodySize() const;
//private:
//    std::string managerCookie_;
//};

//
//class WARNET_API MtoGLoginResultMessage : public Message
//{
//public:
//    enum { thisMessageType = MT_GS_S2C_M_TO_G_LOGIN_RESULT};
//public:
//    MtoGLoginResultMessage(ServerId serverId, ErrorCode ec) : 
//      serverId_(serverId), ec_(ec){}
//public:
//    ServerId getServerId() const 
//    {
//        return serverId_;
//    }
//    ErrorCode getErrorCode() const 
//    {
//        return ec_;
//    }
//private:
//    virtual MessageType getThisMessageType() const {
//        return thisMessageType;
//    }
//    virtual const char* getThisClassName() const {
//        return "MtoGLoginResultMessage";
//    }
//private:
//    virtual bool readBody(Wdr::InputWdr& wdr);
//    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
//    virtual size_t getBodySize() const;
//private:
//    ServerId serverId_;
//    ErrorCode ec_;
//};

} // namespace skynet

#endif // !defined(INCLUDED_GAMESERVERMESSAGE)
