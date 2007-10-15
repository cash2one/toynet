/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// LoginServerMessage.h
#if !defined(INCLUDED_LOGINSERVERMESSAGE)
#define INCLUDED_LOGINSERVERMESSAGE

#include "ArcadeProtocol.h"
#include "ArcadeServerInfo.h"
#include "ArcadeUserInfo.h"
#include <skynet/Message.h>
#include <skynet/ErrorCode.h>
#include <boost/static_assert.hpp>

namespace skynet
{

/// LoginServer(이하 LS) 관련 패킷
enum LoginServerMessageType_t
{
    MT_LS_C2S_SERVERINFO = (MT_LS_START + 1),
    MT_LS_S2C_SERVERINFO = (MT_LS_START + 2),

    MT_LS_C2S_AUTHENTICATE = (MT_LS_START + 10),
    MT_LS_S2C_AUTHENTICATE_FAIL = (MT_LS_START + 20),
    MT_LS_S2C_GAMEINFO = (MT_LS_START + 30),

    MT_LS_C2S_MOVE_COIN = (MT_LS_START + 40),
    MT_LS_S2C_MOVE_COIN = (MT_LS_START + 41),

    MT_LS_C2S_CARD_SLOTITEM_INFOS = (MT_LS_START + 50),
    MT_LS_S2C_CARD_SLOTITEM_INFOS = (MT_LS_START + 51),

    MT_LS_C2S_COIN_FREE_UPDATE = (MT_LS_START + 61),
    MT_LS_S2C_COIN_FREE_UPDATE = (MT_LS_START + 62),

    MT_LS_BOUNDARY
};

BOOST_STATIC_ASSERT(MT_LS_BOUNDARY <= MT_LS_STOP);

/**
* @class LoginServerInfoMessage
*
* LoginServer 정보 요청 메세지.
* - 응답: LoginServerInfoResultMessage
*/
class WARNET_API LoginServerInfoMessage : public Message
{
public:
    enum { thisMessageType = MT_LS_C2S_SERVERINFO };
public:
    explicit LoginServerInfoMessage(const LoginCookie& monitorCookie = "") :
        monitorCookie_(monitorCookie) {}

    const LoginCookie& getMonitorCookie() const {
        return monitorCookie_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "LoginServerInfoMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    LoginCookie monitorCookie_; ///< 인증 쿠키
};


/**
* @class LoginServerInfoResultMessage
*
* LoginServer 정보 요청에 대한 응답 메세지.
*/
class WARNET_API LoginServerInfoResultMessage : public Message
{
public:
    enum { thisMessageType = MT_LS_S2C_SERVERINFO };
public:
    explicit LoginServerInfoResultMessage(const LoginServerInfo& info =
            LoginServerInfo()) :
        loginServerInfo_(info) {}

    const LoginServerInfo& getLoginServerInfo() const {
        return loginServerInfo_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "LoginServerInfoResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    LoginServerInfo loginServerInfo_;
};


/**
* @class AuthMessage
*
* LoginServer 인증 메세지.
* - 응답
*   - 성공: GameInfoMessage
*   - 실패: AuthFailMessage
*/
class WARNET_API AuthMessage : public Message
{
public:
    enum { thisMessageType = MT_LS_C2S_AUTHENTICATE };
public:
    explicit AuthMessage(const std::string& id = "",
            const std::string& password = "", GameType gameType = GT_UNKNOWN) :
        userId_(id),
        password_(password),
        gameType_(gameType) {}

    const std::string& getUserId() const {
        return userId_;
    }
    const std::string& getPassword() const {
        return password_;
    }
    GameType getGameType() const {
        return gameType_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "AuthMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string userId_; ///< 사용자 아이디
    std::string password_; ///< 비밀번호
    GameType gameType_;
};


/**
* @class AuthFailMessage
*
* LoginServer 인증 실패 메세지.
*/
class WARNET_API AuthFailMessage : public Message
{
public:
    enum { thisMessageType = MT_LS_S2C_AUTHENTICATE_FAIL };
public:
    AuthFailMessage(ErrorCode ec = EC_UNKNOWN) :
        errorCode_(ec) {}

    ErrorCode getErrorCode() const {
        return errorCode_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "AuthFailMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ErrorCode errorCode_; ///< 에러 코드
};


/**
* @class GameInfoMessage
*
* LoginServer로 부터 개인 정보, 채널 정보, 서버 관련 정보 리턴
*/
class WARNET_API GameInfoMessage : public Message
{
public:
    enum { thisMessageType = MT_LS_S2C_GAMEINFO };
public:
    explicit GameInfoMessage(UserNo userNo = INVALID_USERNO,
                const std::string& nickname = "", Sex sex = SEX_UNKNOWN,
                const Channels& channels = Channels(),
                const std::string& notice = "",
                const PointStorage& coinBox = PointStorage(),
                time_t serverTime = 0) :
            userNo_(userNo),
            nickname_(nickname),
            sex_(sex),
            notice_(notice),
            coinBox_(coinBox),
            serverTime_(serverTime) 
	{
		        channels_.assign(channels.begin(), channels.end());
    }

    UserNo getUserNo() const {
        return userNo_;
    }
    const std::string& getNickname() const {
        return nickname_;
    }
    Sex getSex() const {
        return sex_;
    }
    const Channels& getChannels() const {
        return channels_;
    }
    const std::string& getNotice() const {
        return notice_;
    }
    const PointStorage& getPointStorage() const {
        return coinBox_;
    }
    time_t getServerTime() const {
        return serverTime_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "GameInfoMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo userNo_;
    std::string nickname_;
    Sex sex_;
    Channels channels_;
    AddressPair resolverAddress_;
    std::string notice_;
    PointStorage coinBox_;
    GuildInfo guildInfo_;
    time_t serverTime_;
};


/**
* @class MoveCoinMessage
*
* 보관함과 캐릭터 카드간 코인 이동 요청 메세지.
* - 응답: MoveCoinResultMessage
*/
class WARNET_API MoveCoinMessage : public Message
{
public:
    enum { thisMessageType = MT_LS_C2S_MOVE_COIN };
public:
    explicit MoveCoinMessage(const std::string& id = "",
            const std::string& password = "",
            CardNumber cardNumber = INVALID_CARD_NUMBER,
            Coin coin = 0) :
        userId_(id),
        password_(password),
        cardNumber_(cardNumber),
        coin_(coin) {}

    const std::string& getUserId() const {
        return userId_;
    }
    const std::string& getPassword() const {
        return password_;
    }
    CardNumber getCharacterCardNumber() const {
        return cardNumber_;
    }
    Coin getCoin() const {
        return coin_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "MoveCoinMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string userId_; ///< 사용자 아이디
    std::string password_; ///< 비밀번호
    CardNumber cardNumber_; ///< 사용할 캐릭터 카드
    /// 이동할 코인 수
    /// - 0보다 크면 보관함에서 카드로, 0보다 작으면 카드에서 보관함으로 이동
    Coin coin_;
};


/**
* @class MoveCoinResultMessage
*
* 보관함과 캐릭터 카드간 코인 이동 요청에 대한 결과 메세지.
* - 에러 코드: 실패 - EC_CANT_MOVE_COIN, 성공 - EC_OK
*/
class WARNET_API MoveCoinResultMessage : public Message
{
public:
    enum { thisMessageType = MT_LS_S2C_MOVE_COIN };
public:
    explicit MoveCoinResultMessage(ErrorCode ec = EC_UNKNOWN,
            const CoinStorage& coinBox = CoinStorage(),
            const CoinStorage& card = CoinStorage(), 
            CardNumber cardNumber = INVALID_CARD_NUMBER) :
        errorCode_(ec),
        coinBox_(coinBox),
        card_(card),
        cardNumber_(cardNumber) {}

    ErrorCode getErrorCode() const {
        return errorCode_;
    }

    const CoinStorage& getCoinBox() const {
        return coinBox_;
    }

    const CoinStorage& getCard() const {
        return card_;
    }

    CardNumber getCharacterCardNumber() const {
        return cardNumber_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "MoveCoinResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ErrorCode errorCode_;
    CoinStorage coinBox_; ///< 코인 보관함에 현재 상황
    CoinStorage card_; ///< 카드(cardNumber_)의 현재 상황
    CardNumber cardNumber_; ///< 사용된 캐릭터 카드
};


/**
* @class CardSlotItemInfosMessage
*
* 카드 슬롯 아이템 정보 요청 메세지.
* - 응답: CardSlotItemInfosResultMessage
*/
class WARNET_API CardSlotItemInfosMessage : public Message
{
public:
    enum { thisMessageType = MT_LS_C2S_CARD_SLOTITEM_INFOS };
public:
    explicit CardSlotItemInfosMessage(const std::string& id = "",
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
        return "CardSlotItemInfosMessage";
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
* @class CardSlotItemInfosResultMessage
*
* 카드 슬롯 아이템 정보 요청에 대한 결과 메세지.
*/
class WARNET_API CardSlotItemInfosResultMessage : public Message
{
public:
    enum { thisMessageType = MT_LS_S2C_CARD_SLOTITEM_INFOS };
public:
    explicit CardSlotItemInfosResultMessage(
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
        return "CardSlotItemInfosResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    CardNumber cardNumber_; ///< 캐릭터 카드 번호
    ItemInfos itemInfos_; ///< 슬롯 아이템 정보 목록
};


/**
* @class CoinFreeUpdateMessage
*
* 코인 충전 메세지.
*/
class WARNET_API CoinFreeUpdateMessage : public Message
{
public:
    enum { thisMessageType = MT_LS_C2S_COIN_FREE_UPDATE };
public:
    explicit CoinFreeUpdateMessage(
        CardNumber cardNumber = INVALID_CARD_NUMBER,
        const std::string& userId= "",
        const std::string& password = "") :
    cardNumber_(cardNumber),
        userId_(userId),
        password_(password){}

        CardNumber getCharacterCardNumber() const {
            return cardNumber_;
        }

        const std::string& getUserId() const {
            return userId_;
        }
        const std::string& getPassword() const {
            return password_;
        }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "CoinFreeUpdateMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string userId_;
    std::string password_;
    CardNumber cardNumber_; ///< 슬롯 아이템 정보 목록
};



/**
* @class CoinFreeUpdateResultMessage
*
* 코인 충전 결과 메세지.
*/
class WARNET_API CoinFreeUpdateResultMessage : public Message
{
public:
    enum { thisMessageType = MT_LS_S2C_COIN_FREE_UPDATE };
public:
    explicit CoinFreeUpdateResultMessage(
        CardNumber cardNumber = INVALID_CARD_NUMBER,
        const Coin coin = 0 , const ErrorCode errorCode= 0) :
    cardNumber_(cardNumber),
        coin_(coin),
        errorCode_(errorCode){}

        CardNumber getCharacterCardNumber() const {
            return cardNumber_;
        }

        const Coin getCoin() const {
            return coin_;
        }

        const ErrorCode getErrorCode() const {
            return errorCode_;
        }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "CoinFreeUpdateResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ErrorCode errorCode_;
    CardNumber cardNumber_; ///< 슬롯 아이템 정보 목록
    Coin coin_;//코인갯수
};

} // namespace skynet

#endif // !defined(INCLUDED_LOGINSERVERMESSAGE)
