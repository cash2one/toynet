/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// MessengerMessage.h
#if !defined(INCLUDED_MESSENGERMESSAGE)
#define INCLUDED_MESSENGERMESSAGE

#include "Messenger.h"
#include "../ArcadeProtocol.h"
#include "../ArcadeCommonInfo.h"
#include <skynet/Message.h>
#include <skynet/ErrorCode.h>
#include <boost/static_assert.hpp>

namespace skynet
{

namespace im
{

/// Messenger(이하 LS) 관련 패킷
enum MessengerMessageType_t
{
    MT_IM_S2C_BUDDY_LIST = (MT_IM_START + 10),

    MT_IM_C2S_REGISTER_BUDDY = (MT_IM_START + 20),
    MT_IM_S2C_REGISTER_BUDDY = (MT_IM_START + 21),
    MT_IM_S2C_BUDDY_REGISTERED = (MT_IM_START + 23),

    MT_IM_S2C_BUDDY_ONLINE = (MT_IM_START + 24),
    MT_IM_S2C_BUDDY_OFFLINE = (MT_IM_START + 25),

    MT_IM_C2S_UNREGISTER_BUDDY = (MT_IM_START + 30),
    MT_IM_S2C_UNREGISTER_BUDDY = (MT_IM_START + 31),
    MT_IM_S2C_BUDDY_UNREGISTERED = (MT_IM_START + 32),

    MT_IM_S2C_REGISTERED_BUDDY_ONLINE = (MT_IM_START + 40),
    MT_IM_S2C_REGISTERED_BUDDY_OFFLINE = (MT_IM_START + 42),

    MT_IM_S2C_BUDDY_POSITION_CHANGED = (MT_IM_START + 43),
    MT_IM_S2C_BUDDY_STATE_CHANGED = (MT_IM_START + 44),

    MT_IM_C2S_UPDATE_BUDDY_MEMO = (MT_IM_START + 45),
    MT_IM_S2C_UPDATE_BUDDY_MEMO = (MT_IM_START + 46),

    MT_IM_BOUNDARY
};

BOOST_STATIC_ASSERT(MT_IM_BOUNDARY <= MT_IM_STOP);


/**
* @class RegisterBuddyMessage
*
* 친구 등록 요청 메세지.
* - 응답: RegisterBuddyResultMessage
* - 통지: BuddyRegisteredMessage
*/
class WARNET_API RegisterBuddyMessage : public Message
{
public:
    enum { thisMessageType = MT_IM_C2S_REGISTER_BUDDY };
public:
    explicit RegisterBuddyMessage(const std::string& buddyNickname = "",
            const std::string& memo = "", bool isBadBuddy = false) :
        buddyNickname_(buddyNickname),
        memo_(memo),
        isBadBuddy_(isBadBuddy) {}

    const std::string& getBuddyNickname() const {
        return buddyNickname_;
    }

    const std::string& getMemo() const {
        return memo_;
    }

    bool isBadBuddy() const {
        return isBadBuddy_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "RegisterBuddyMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string buddyNickname_; ///< 등록할 친구 닉네임
    std::string memo_; ///< 메모
    bool isBadBuddy_; ///< 나쁜 친구?
};


/**
* @class RegisterBuddyResultMessage
*
* Messenger 정보 요청에 대한 응답 메세지.
*/
class WARNET_API RegisterBuddyResultMessage : public Message
{
public:
    enum { thisMessageType = MT_IM_S2C_REGISTER_BUDDY };
public:
    explicit RegisterBuddyResultMessage(const Buddy& buddy = Buddy(),
			ErrorCode ec = EC_UNKNOWN) :
        buddy_(buddy),
		errorCode_(ec) {}

    const Buddy& getBuddy() const {
        return buddy_;
    }

	ErrorCode getErrorCode() const {
		return errorCode_;
	}
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "RegisterBuddyResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    Buddy buddy_;
	ErrorCode errorCode_;
};


/**
* @class BuddyRegisteredMessage
*
* 친구로 등록되었음을 알리는 메세지.
*/
class WARNET_API BuddyRegisteredMessage : public Message
{
public:
    enum { thisMessageType = MT_IM_S2C_BUDDY_REGISTERED };
public:
    explicit BuddyRegisteredMessage(const Buddy& buddy = Buddy()) :
        buddy_(buddy) {}

    const Buddy& getBuddy() const {
        return buddy_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "BuddyRegisteredMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    Buddy buddy_; ///< 나를 등록한 친구
};


/**
* @class BuddyListMessage
*
* 친구 목록 메세지.
*/
class WARNET_API BuddyListMessage : public Message
{
public:
    enum { thisMessageType = MT_IM_S2C_BUDDY_LIST };
public:
    explicit BuddyListMessage(const Buddies& buddies = Buddies(),
            const Buddies& registeredBuddies = Buddies()) :
        buddies_(buddies),
        registeredBuddies_(registeredBuddies) {}

    const Buddies& getBuddies() const {
        return buddies_;
    }
    const Buddies& getRegisteredBuddies() const {
        return registeredBuddies_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "BuddyListMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    Buddies buddies_; ///< 내가 등록한 친구
    Buddies registeredBuddies_; ///< 나를 등록한 친구
};


/**
* @class BuddyOnlineMessage
*
* 내가 등록한 친구가 온라인임을 알림 메세지.
*/
class WARNET_API BuddyOnlineMessage : public Message
{
public:
    enum { thisMessageType = MT_IM_S2C_BUDDY_ONLINE };
public:
    explicit BuddyOnlineMessage(UserNo buddy = INVALID_USERNO) :
        buddy_(buddy) {}

    UserNo getBuddy() const {
        return buddy_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "BuddyOnlineMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo buddy_; ///< 내가 등록한 온라인 중인 친구
};


/**
* @class BuddyOfflineMessage
*
* 내가 등록한 친구가 오프라인임을 알림 메세지.
*/
class WARNET_API BuddyOfflineMessage : public Message
{
public:
    enum { thisMessageType = MT_IM_S2C_BUDDY_OFFLINE };
public:
    explicit BuddyOfflineMessage(UserNo buddy = INVALID_USERNO) :
        buddy_(buddy) {}

    UserNo getBuddy() const {
        return buddy_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "BuddyOfflineMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo buddy_; ///< 내가 등록한 오프라인 중인 친구
};


/**
* @class UnregisterBuddyMessage
*
* 친구 등록 해제 요청 메세지.
* - 응답: UnregisterBuddyResultMessage
* - 통지: BuddyUnregisteredMessage
*/
class WARNET_API UnregisterBuddyMessage : public Message
{
public:
    enum { thisMessageType = MT_IM_C2S_UNREGISTER_BUDDY };
public:
    explicit UnregisterBuddyMessage(UserNo buddy = INVALID_USERNO) :
        buddy_(buddy) {}

    UserNo getBuddy() const {
        return buddy_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "UnregisterBuddyMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo buddy_; ///< 등록해제할 친구
};


/**
* @class UnregisterBuddyResultMessage
*
* 친구 등록 해제 결과 메세지.
*/
class WARNET_API UnregisterBuddyResultMessage : public Message
{
public:
    enum { thisMessageType = MT_IM_S2C_UNREGISTER_BUDDY };
public:
    explicit UnregisterBuddyResultMessage(
            UserNo registeredBuddy = INVALID_USERNO) :
        unregisteredBuddy_(registeredBuddy) {}

    UserNo getBuddy() const {
        return unregisteredBuddy_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "UnregisterBuddyResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo unregisteredBuddy_; ///< 등록해제된 친구
};


/**
* @class BuddyUnregisteredMessage
*
* 나를 등록한 친구가 등록해제되었음을 알리는 메세지.
*/
class WARNET_API BuddyUnregisteredMessage : public Message
{
public:
    enum { thisMessageType = MT_IM_S2C_BUDDY_UNREGISTERED };
public:
    explicit BuddyUnregisteredMessage(UserNo buddy = INVALID_USERNO) :
        unregisteredBuddy_(buddy) {}

    UserNo getBuddy() const {
        return unregisteredBuddy_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "BuddyUnregisteredMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo unregisteredBuddy_; ///< 등록 해제된 친구
};


/**
* @class BuddyWhoRegisterOnlineMessage
*
* 나를 등록한 친구가 온라인임을 알림 메세지.
*/
class WARNET_API BuddyWhoRegisterOnlineMessage : public Message
{
public:
    enum { thisMessageType = MT_IM_S2C_REGISTERED_BUDDY_ONLINE };
public:
    explicit BuddyWhoRegisterOnlineMessage(UserNo buddy = INVALID_USERNO) :
        buddy_(buddy) {}

    UserNo getBuddy() const {
        return buddy_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "BuddyWhoRegisterOnlineMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo buddy_; ///< 나를 등록한 친구
};


/**
* @class BuddyWhoRegisterOfflineMessage
*
* 나를 등록한 친구가 오프라인임을 알림 메세지.
*/
class WARNET_API BuddyWhoRegisterOfflineMessage : public Message
{
public:
    enum { thisMessageType = MT_IM_S2C_REGISTERED_BUDDY_OFFLINE };
public:
    explicit BuddyWhoRegisterOfflineMessage(UserNo buddy = INVALID_USERNO) :
        buddy_(buddy) {}

    UserNo getBuddy() const {
        return buddy_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "BuddyWhoRegisterOfflineMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo buddy_; ///< 나를 등록한 친구
};


/**
* @class BuddyPositionChangedMessage
*
* 둘 다 등록한 친구의 현재 위치가 변경되었다
* - 주의!! 이 메세지를 받을 경우 사용자의 현재 상태는 US_WAITING이 된다!
*/
class WARNET_API BuddyPositionChangedMessage : public Message
{
public:
    enum { thisMessageType = MT_IM_S2C_BUDDY_POSITION_CHANGED };
public:
    explicit BuddyPositionChangedMessage(UserNo buddy = INVALID_USERNO,
            RoomId position = INVALID_ROOMID) :
        buddy_(buddy),
        position_(position) {}

    UserNo getBuddy() const {
        return buddy_;
    }

    RoomId getPosition() const {
        return position_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "BuddyPositionChangedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo buddy_;
    RoomId position_;
};


/**
* @class BuddyStateChangedMessage
*
* 둘 다 등록한 친구의 현재 위치가 변경되었다
*/
class WARNET_API BuddyStateChangedMessage : public Message
{
public:
    enum { thisMessageType = MT_IM_S2C_BUDDY_STATE_CHANGED };
public:
    explicit BuddyStateChangedMessage(UserNo buddy = INVALID_USERNO,
        UserState state = US_UNKNOWN) :
    buddy_(buddy),
        state_(state) {}

    UserNo getBuddy() const {
        return buddy_;
    }

    UserState getState() const {
        return state_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "BuddyStateChangedMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo buddy_;
    UserState state_;
};


/**
* @class UpdateBuddyMemoMessage
*
* 친구 메모 수정 요청 메세지.
* - 응답: UpdateBuddyMemoResultMessage
*/
class WARNET_API UpdateBuddyMemoMessage : public Message
{
public:
    enum { thisMessageType = MT_IM_C2S_UPDATE_BUDDY_MEMO };
public:
    explicit UpdateBuddyMemoMessage(UserNo buddy = INVALID_USERNO,
            const std::string& memo = "") :
        buddy_(buddy),
        memo_(memo) {}

    UserNo getBuddy() const {
        return buddy_;
    }

    const std::string& getMemo() const {
        return memo_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "UpdateBuddyMemoMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo buddy_; ///< 수정할 친구
    std::string memo_; ///< 수정할 메모
};


/**
* @class UpdateBuddyMemoResultMessage
*
* 친구 메모 수정 요청에 대한 결과 메세지.
*/
class WARNET_API UpdateBuddyMemoResultMessage : public Message
{
public:
    enum { thisMessageType = MT_IM_S2C_UPDATE_BUDDY_MEMO };
public:
    explicit UpdateBuddyMemoResultMessage(UserNo buddy = INVALID_USERNO,
            const std::string& memo = "") :
        buddy_(buddy),
        memo_(memo) {}

    UserNo getBuddy() const {
        return buddy_;
    }

    const std::string& getMemo() const {
        return memo_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "UpdateBuddyMemoResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    UserNo buddy_; ///< 수정할 친구
    std::string memo_; ///< 수정할 메모
};

} // namespace im

} // namespace skynet

#endif // !defined(INCLUDED_MESSENGERMESSAGE)
