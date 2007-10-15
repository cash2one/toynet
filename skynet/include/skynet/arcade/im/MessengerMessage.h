/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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

/// Messenger(���� LS) ���� ��Ŷ
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
* ģ�� ��� ��û �޼���.
* - ����: RegisterBuddyResultMessage
* - ����: BuddyRegisteredMessage
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
    std::string buddyNickname_; ///< ����� ģ�� �г���
    std::string memo_; ///< �޸�
    bool isBadBuddy_; ///< ���� ģ��?
};


/**
* @class RegisterBuddyResultMessage
*
* Messenger ���� ��û�� ���� ���� �޼���.
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
* ģ���� ��ϵǾ����� �˸��� �޼���.
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
    Buddy buddy_; ///< ���� ����� ģ��
};


/**
* @class BuddyListMessage
*
* ģ�� ��� �޼���.
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
    Buddies buddies_; ///< ���� ����� ģ��
    Buddies registeredBuddies_; ///< ���� ����� ģ��
};


/**
* @class BuddyOnlineMessage
*
* ���� ����� ģ���� �¶������� �˸� �޼���.
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
    UserNo buddy_; ///< ���� ����� �¶��� ���� ģ��
};


/**
* @class BuddyOfflineMessage
*
* ���� ����� ģ���� ������������ �˸� �޼���.
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
    UserNo buddy_; ///< ���� ����� �������� ���� ģ��
};


/**
* @class UnregisterBuddyMessage
*
* ģ�� ��� ���� ��û �޼���.
* - ����: UnregisterBuddyResultMessage
* - ����: BuddyUnregisteredMessage
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
    UserNo buddy_; ///< ��������� ģ��
};


/**
* @class UnregisterBuddyResultMessage
*
* ģ�� ��� ���� ��� �޼���.
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
    UserNo unregisteredBuddy_; ///< ��������� ģ��
};


/**
* @class BuddyUnregisteredMessage
*
* ���� ����� ģ���� ��������Ǿ����� �˸��� �޼���.
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
    UserNo unregisteredBuddy_; ///< ��� ������ ģ��
};


/**
* @class BuddyWhoRegisterOnlineMessage
*
* ���� ����� ģ���� �¶������� �˸� �޼���.
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
    UserNo buddy_; ///< ���� ����� ģ��
};


/**
* @class BuddyWhoRegisterOfflineMessage
*
* ���� ����� ģ���� ������������ �˸� �޼���.
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
    UserNo buddy_; ///< ���� ����� ģ��
};


/**
* @class BuddyPositionChangedMessage
*
* �� �� ����� ģ���� ���� ��ġ�� ����Ǿ���
* - ����!! �� �޼����� ���� ��� ������� ���� ���´� US_WAITING�� �ȴ�!
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
* �� �� ����� ģ���� ���� ��ġ�� ����Ǿ���
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
* ģ�� �޸� ���� ��û �޼���.
* - ����: UpdateBuddyMemoResultMessage
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
    UserNo buddy_; ///< ������ ģ��
    std::string memo_; ///< ������ �޸�
};


/**
* @class UpdateBuddyMemoResultMessage
*
* ģ�� �޸� ���� ��û�� ���� ��� �޼���.
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
    UserNo buddy_; ///< ������ ģ��
    std::string memo_; ///< ������ �޸�
};

} // namespace im

} // namespace skynet

#endif // !defined(INCLUDED_MESSENGERMESSAGE)
