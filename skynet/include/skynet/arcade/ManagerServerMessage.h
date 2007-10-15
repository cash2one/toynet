/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ManagerServerMessage.h
#if !defined(INCLUDED_MANAGERSERVERMESSAGE)
#define INCLUDED_MANAGERSERVERMESSAGE

#include "ArcadeProtocol.h"
#include "ArcadeServerInfo.h"
#include "arcadeuserinfo.h"
#include "ServerType.h"
#include <skynet/Message.h>
#include <skynet/ErrorCode.h>
#include <boost/static_assert.hpp>
#include <skynet/arcade/im/Messenger.h>
#include <string>
#include <memory>
#include <vector>
#include <WinBase.h>
#pragma warning (disable :4996)
#pragma warning (disable :4511)
#pragma warning (disable :4512)

namespace skynet
{

/// LoginServer(이하 LS) 관련 패킷
enum ManagerServerMessageType_t
{
    MT_MS_C2S_M_TO_S_LOGIN = (MT_MS_START + 30),
    MT_MS_C2S_M_TO_S_LOGIN_RESULT = (MT_MS_START + 31),

    MT_MS_C2S_MLOGIN = (MT_MS_START + 40),

    MT_MS_S2C_LOGIN_RESULT = (MT_MS_START + 41),//메니저 -> 서버, 툴    
    MT_MS_S2C_LOGIN_RESULT_EX = (MT_MS_START + 42),//메니저 -> 툴
    
    //서버와 메니저 통신 프로토콜
    MT_MS_C2S_REQUEST_SERVERINFO = (MT_MS_START + 50),
    MT_MS_C2S_RESPONSE_GAMESERVERINFO = (MT_MS_START + 52),
    MT_MS_C2S_RESPONSE_SERVERINFO = (MT_MS_START + 53),
   
    MT_MS_C2S_ALRAM_SERVERERROR = (MT_MS_START + 60),

    // 툴과 메니저
    MT_MS_C2S_NOTICE_GENRE_CHANNELS = (MT_MS_START + 70),
    MT_MS_C2S_NOTICE_GENRE_CHANNEL = (MT_MS_START + 71),
    MT_MS_C2S_NOTICE_ALLCHANNELS= (MT_MS_START + 72),
    MT_MS_C2S_NOTICE_SELECT_CHANNELS = (MT_MS_START + 73),
    MT_MS_C2S_NOTICE_TO_USER = (MT_MS_START + 74),
    
    // 메너저와 서버
    MT_MS_S2C_NOTICE_EX_TO_USER = (MT_MS_START + 75),
    MT_MS_S2C_NOTICE_EX = (MT_MS_START + 76),
    MT_MS_NOTICE_RESULT = (MT_MS_START + 77),
    
    
    MT_MS_S2C_SERVERLISTINFO = (MT_MS_START + 80),//메니저 -> 툴
    MT_MS_C2S_REQUEST_MONITORTOOLINFO =  (MT_MS_START + 81), // 툴 -> 메니저
    MT_MS_C2S_REQUEST_SERVERTOOLINFO =  (MT_MS_START + 82), // 툴 -> 메니저
    MT_MS_S2C_RESPONSE_SERVERTOOLINFO =  (MT_MS_START + 83),//응답 메세지
    MT_MS_S2C_RESPONSE_MONITORTOOLINFO = (MT_MS_START + 84),//응답 메세지

    MT_MS_C2S_ALL_SERVER_ON = (MT_MS_START + 90),
    MT_MS_C2S_ALL_SERVER_OFF = (MT_MS_START + 91),
    MT_MS_C2S_CHANNELS_SERVER_ON = (MT_MS_START + 94),
    MT_MS_C2S_CHANNELS_SERVER_OFF = (MT_MS_START + 95),
    MT_MS_S2C_SERVER_ON_RESULT = (MT_MS_START + 96),
    MT_MS_S2C_SERVER_OFF_RESULT = (MT_MS_START + 97),

    MT_MS_C2S_FILE_CREAE = (MT_MS_START + 100),
    MT_MS_C2S_FILE_WRITE = (MT_MS_START + 101),
    MT_MS_C2S_FILE_CLOSE = (MT_MS_START + 102),
    MT_MS_S2C_FILE_RESULT = (MT_MS_START + 103),

    MT_MS_C2S_CREATE_MANAGER_USER = (MT_MS_START + 111),
    MT_MS_C2S_DELETE_MANAGER_USER = (MT_MS_START + 112),
    MT_MS_S2C_MANAGER_USER_ACTION_RESULT = (MT_MS_START + 113),

    MT_MS_BOUNDARY
};

BOOST_STATIC_ASSERT(MT_MS_BOUNDARY <= MT_MS_STOP);



class WARNET_API MtoSLoginMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_M_TO_S_LOGIN };
public:
    explicit MtoSLoginMessage(const std::string& cookie = "") :
    managerCookie_(cookie)
    {}

    const std::string& getManagerCookie() const {
        return managerCookie_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "MtoSLoginMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string managerCookie_;
};


class WARNET_API MtoSLoginReslutMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_M_TO_S_LOGIN_RESULT};
 
public:
    explicit MtoSLoginReslutMessage(ServerId serverId = -1,
        ErrorCode ec = EC_UNKNOWN) :
        serverId_(serverId), ec_(ec)
    {}

    ServerId getServerId() const {
        return serverId_;
    }
    ErrorCode getErrorCode() const {
        return ec_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "MtoSLoginResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ServerId serverId_;
    ErrorCode ec_;
};

class WARNET_API MLoginMessage :public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_MLOGIN };
public:
    MLoginMessage(mode::AuthenticMode aMode = mode::MODE_UNKNOWN,
        const AuthenticImpo& authenticImpo = AuthenticImpo()):
        aMode_(aMode),
        authenticImpo_(authenticImpo){}
public:
    mode::AuthenticMode getAuthenticMode() const
    {
        return aMode_;
    }
    AuthenticImpo getAuthenticImpo() const
    {
        return authenticImpo_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "MLoginMessage ";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    mode::AuthenticMode aMode_;
    AuthenticImpo authenticImpo_;
};


/**
* @class MLoginResultMessage
*
* 로그인한 결과를 반환
* - 응답: 없음
*/
class WARNET_API MLoginResultMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_S2C_LOGIN_RESULT };
public:
    explicit MLoginResultMessage(ErrorCode ec = EC_UNKNOWN) :
    ec_(ec) {}

    ErrorCode getErrorCode() const {
        return ec_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "MLoginResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ErrorCode ec_; ///< 에러 코드
};



/**
* @class MLoginResultMessage
*
* 로그인한 결과를 반환
* - 응답: 없음
*/
class WARNET_API MLoginResultExMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_S2C_LOGIN_RESULT_EX };
public:
    explicit MLoginResultExMessage(ToolUseGrade toolUseGrade = TUG_UNKNOWN,
        ErrorCode ec = EC_UNKNOWN) :
    toolUseGrade_(toolUseGrade),
    ec_(ec) {}

    ToolUseGrade getToolUseGrade() const {
        return toolUseGrade_;
    }
    ErrorCode getErrorCode() const {
        return ec_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "MLoginResultExMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ToolUseGrade toolUseGrade_;
    ErrorCode ec_; ///< 에러 코드
};


/**
* @class NoticeGenreChannelsMessage
*
* 게임 전체로 공지사항을 보낸다 
* - 응답: 없음
*/
class WARNET_API NoticeAllChannelsMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_NOTICE_ALLCHANNELS };
public:
    explicit NoticeAllChannelsMessage(const LoginCookie& cookie = LoginCookie(),
        const std::string& nickname = "",
        const std::string& notice = "") :
        managerCookie_(cookie),
        nickname_(nickname),
        notice_(notice)
        {}

    const std::string& getManagerCookie() const {
        return managerCookie_;
    }
    const std::string& getNickname()const {
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
        return "NoticeAllChannelsMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    LoginCookie managerCookie_;
    std::string nickname_;
    std::string notice_;
};


/**
* @class NoticeSelectChannelsMessage
*
*   게임 선택한 채널에 공지사항을 보낸다
* - 응답: 없음
*/
class WARNET_API NoticeSelectChannelsMessage: public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_NOTICE_SELECT_CHANNELS};
public:
    explicit NoticeSelectChannelsMessage(const LoginCookie& cookie = LoginCookie(),
        const std::string& nickname = "",
        const std::string& notice = "",
        const ServerIds &serverIds = ServerIds()) :
        managerCookie_(cookie),
        nickname_(nickname),
        notice_(notice),
        serverIds_(serverIds)
    {}

    const std::string& getManagerCookie() const {
        return managerCookie_;
    }
    const std::string& getNickname()const {
        return nickname_;
    }
    const std::string& getNotice() const {
        return notice_;
    }
    const  ServerIds& getServerIds() const {
        return serverIds_;
    }

private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "NoticeSelectChannelsMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    LoginCookie managerCookie_;
    std::string nickname_;
    std::string notice_;
    ServerIds serverIds_;
};


/**
* @class NoticeExToUserMessage
*
*   게임 한명의 유저에게  공지사항을 보낸다(메니저->서버)
* - 응답: 없음
*/

class WARNET_API NoticeToUserMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_NOTICE_TO_USER};
public:
    explicit NoticeToUserMessage(const std::string& fromNickname = "",
        const std::string& toNickname = "",
        const std::string& notice = "",
        const ServerId serverId = -1) :
        fromNickname_(fromNickname),
        toNickname_(toNickname),
        notice_(notice),
        serverId_(serverId)
    {}

  
    const std::string& getToNickname()const {
        return toNickname_;
    }

    const std::string& getFromNickname()const {
        return fromNickname_;
    }
    const std::string& getNotice() const {
        return notice_;
    }
    const  ServerId getServerId() const {
        return serverId_;
    }

private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "NoticeToUserMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string fromNickname_;
    std::string toNickname_;
    std::string notice_;
    ServerId serverId_;
};
/**
* @class NoticeExToUserMessage
*
*   게임 한명의 유저에게  공지사항을 보낸다(메니저->서버)
* - 응답: 없음
*/

class WARNET_API NoticeExToUserMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_S2C_NOTICE_EX_TO_USER};
public:
    explicit NoticeExToUserMessage(const std::string& cookie = "",
        const std::string& fromNickname = "", const std::string& toNickname = "",
        const std::string& notice = "", int idx = -1) :
        managerCookie_(cookie), fromNickname_(fromNickname), 
        toNickname_(toNickname), notice_(notice), idx_(idx)
    {}

    const std::string& getManagerCookie() const {
        return managerCookie_;
    }

    const std::string& getToNickname()const {
        return toNickname_;
    }

    const std::string& getFromNickname()const {
        return fromNickname_;
    }
    const std::string& getNotice() const {
        return notice_;
    }
    int getNoticeLogIdx() const {
        return idx_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "NoticeExToUserMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    LoginCookie managerCookie_;
    std::string fromNickname_;
    std::string toNickname_;
    std::string notice_;
    int idx_;
};

/**
* @class NoticeExMessage
*
*   게임서버에게 공지사항을 전달.
* - 응답: 없음
*/
class WARNET_API NoticeExMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_S2C_NOTICE_EX };
public:
    explicit NoticeExMessage(const std::string& managerCookie = "",
        const std::string& nickname = "", const std::string& notice = "",
        int idx = -1) :
        managerCookie_(managerCookie), nickname_(nickname), notice_(notice),
        idx_(idx){}
    
        const std::string& getManagerCookie() const {
            return managerCookie_;
        }

        const std::string& getNickname() const {
            return nickname_;
        }

        const std::string& getNotice() const {
            return notice_;
        }
        int getNoticeLogIdx() const {
            return idx_;
        }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "NoticeExMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    LoginCookie managerCookie_;
    std::string nickname_;
    std::string notice_;
    int idx_;
};


/**
* @class NoticeResultMessage 
*
*   공지사항 결과.
* - 응답: 없음
*/
class WARNET_API NoticeResultMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_NOTICE_RESULT };
public:
    explicit NoticeResultMessage(ServerId serverId =-1,
        const std::string& fromNickname = "", const std::string& toNickname = "",
        const std::string& notice = "", time_t recvTime = 0,
        ErrorCode ec = EC_UNKNOWN, int idx = -1) :
        serverId_(serverId), fromNickname_(fromNickname), toNickname_(toNickname),
        notice_(notice), recvTime_(recvTime), ec_(ec),idx_(idx)

       {}

    ServerId getServerId() const {
        return serverId_;
    }
    const std::string& getFromNickname() const {
        return fromNickname_;
    }
    const std::string& getToNickname() const {
        return toNickname_;
    }
    const std::string& getNotice() const {
        return notice_;
    }
    time_t getRecvTime() const {
        return recvTime_;
    }
    ErrorCode getErrorCode() const {
        return ec_;
    }
    int getNoticeLogIdx() const {
        return idx_;
    }

private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "NoticeResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ServerId serverId_;
    std::string fromNickname_;
    std::string toNickname_;
    std::string notice_;
    time_t recvTime_;
    ErrorCode ec_;
    int idx_;
};


/*
* @class  RequestServerInfoMessage
*
* 여러 서버에게 메니저에서 필요한 정보 요청 메세지.
* - 응답: ResponseGameServerInfoMessage or ResponseServerInfoMessage
*/
class WARNET_API RequestServerInfoMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_REQUEST_SERVERINFO};
public:
    explicit RequestServerInfoMessage ()
    {}
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "RequestServerMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
};

/*
* @class  ResponseGameServerInfoMessage 
*
* 메니저에게 자기 정보를 날림.
* - 응답: 무
*/

class WARNET_API ResponseGameServerInfoMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_RESPONSE_GAMESERVERINFO  };
public:
    explicit ResponseGameServerInfoMessage (
        const GameChannelInfo& gameChannelInfo =GameChannelInfo()):
        channelInfo_(gameChannelInfo) {}
 
public:
    const GameChannelInfo& getGameChannelInfo() const
    {
        return channelInfo_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ResponseGameChannelInfoMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    GameChannelInfo channelInfo_;
};


/*
* @class  ResponseServerInfoMessage 
*
* 메니저에게 자기 정보를 날림. 서버에서(포워드 로그인 센트 릴레이) -> 메니저 
* - 응답: 무
*/
class WARNET_API ResponseServerInfoMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_RESPONSE_SERVERINFO  };
public:
    explicit ResponseServerInfoMessage(ServerId serverId = -1) :
        serverId_(serverId){}
public:
    ServerId getServerId() const
    {
        return serverId_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }

    virtual const char* getThisClassName() const {
        return "ResponseServerInfoMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    //포워드 서버때문에 넣었다...
    ServerId serverId_;
};  


/*
* @class AlramServerErrorMessage
*
* 툴에게 서버에러 알림 메세지.
* - 응답: 무
*/
class WARNET_API AlramServerErrorMessage: public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_ALRAM_SERVERERROR };
public:
    AlramServerErrorMessage(const ServerIds& serverIds = ServerIds()) :
        serverIds_(serverIds){}

    const ServerIds getServerIds() const {
        return serverIds_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "AlramServerErrorMessage  ";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ServerIds serverIds_;
};


/*
* @class RequestServerInfoMessage
*
* 메니저에게 모니터에 필요한 정보 요청 메세지.
* - 응답: ResponseServerToolInfoMessage  
*/
class WARNET_API RequestServerToolInfoMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_REQUEST_SERVERTOOLINFO};
public:
    RequestServerToolInfoMessage  () {}
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "RequestServerToolInfoMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
};




/*
* @class RequestMonitorInfoMessage
*
* 메니저에게 모니터에 필요한 정보 요청 메세지.
* - 응답: ResponseMonitorToolInfoMessage  
*/

class WARNET_API RequestMonitorToolInfoMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_REQUEST_MONITORTOOLINFO};
public:
    RequestMonitorToolInfoMessage  () {}
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "RequestMonitorToolInfoMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
};

/*
* @class ResponseMonitorToolInfoMessage
*
* 모니터에 필요한 정보 응답 메세지.
* - 응답: 무
*/
class WARNET_API ResponseMonitorToolInfoMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_S2C_RESPONSE_MONITORTOOLINFO};
public:
    ResponseMonitorToolInfoMessage (
        const GameChannelInfoMap& gameChannelInfoMap = GameChannelInfoMap()):
      gameChannelInfoMap_(gameChannelInfoMap){}

    const GameChannelInfoMap& getGameServerInfoMap() const {
        return gameChannelInfoMap_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "ResponseMonitorToolInfoMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    GameChannelInfoMap gameChannelInfoMap_;
};



/*
* @class ResponseServerToolInfoMessage
*
* 모니터에 필요한 정보 응답 메세지.
* - 응답: 무
*/
class WARNET_API ResponseServerToolInfoMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_S2C_RESPONSE_SERVERTOOLINFO};
public:
    ResponseServerToolInfoMessage (
        const ServerStateMap& serverStateMap = ServerStateMap()):
        serverStateMap_(serverStateMap){}

    const ServerStateMap& getGameServerInfoMap() const {
        return serverStateMap_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "ResponseServerToolInfoMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ServerStateMap serverStateMap_;
};
/*
* @class ServerListInfoMessage 
*
* 모니터에 필요한 정보 응답 메세지.
* - 응답: 무
*/
class WARNET_API ServerListInfoMessage :public Message
{
public:
    enum { thisMessageType = MT_MS_S2C_SERVERLISTINFO};
public:
    ServerListInfoMessage(ErrorCode ec = EC_UNKNOWN, 
        const ServerListInfos serverlistInfos = ServerListInfos(),
        const std::string& cookie = "") :
        ec_(ec),
        serverListInfos_(serverlistInfos),
        cookie_(cookie)
        {}
public:
    const ServerListInfos& getServerInfo() const
    {
        return serverListInfos_;
    }
    const ErrorCode getErrorCode() const
    {
        return ec_;
    }
    const std::string& getManagerCookie() const
    {
        return cookie_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "ServerListInfoMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ErrorCode ec_;
    ServerListInfos serverListInfos_;
    std::string cookie_;
};


/*
* @class AllServerOnMessage
*
*  서버 서비스 On 요청 메세지.
* - 응답: ServerOnResultMessage
*/
class WARNET_API AllServerOnMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_ALL_SERVER_ON};
public:
    AllServerOnMessage () {}
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "AllServerOnMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;

};


/*
* @class AllServerOffMessage
*
*  서버 서비스 Off 요청 메세지.
* - 응답: ServerOffResultMessage
*/
class WARNET_API AllServerOffMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_ALL_SERVER_OFF};
public:
    AllServerOffMessage () {}
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "AllServerOffMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
};


/*
* @class ChannelServerOnMessage
*
*  서버 서비스 On 요청 메세지.
* - 응답: ServerOffResultMessage
*/
class WARNET_API ChannelsServerOnMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_CHANNELS_SERVER_ON };
public:
    ChannelsServerOnMessage(ServerIds serverIds = ServerIds()):
        serverIds_(serverIds)
        {}
public:
    const ServerIds& getServerIds() const 
    {
        return serverIds_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "ChannelsServerOnMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ServerIds serverIds_;
};


/*
* @class ChannelServerOffMessage
*
*   서버 서비스 Off 요청 메세지.
* - 응답: ServerOffResultMessage
*/
class WARNET_API ChannelsServerOffMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_CHANNELS_SERVER_OFF};
public:
    ChannelsServerOffMessage(ServerIds serverIds = ServerIds()):
        serverIds_(serverIds)
        {}
public:
    const ServerIds& getServerIds() const 
    {
        return serverIds_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "ChannelsServersOffMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ServerIds serverIds_;
};


/*
* @class ServerOnResultMessage
*
*  서버 서비스 On 요청 결과 메세지.
* - 응답: 무
*/
class WARNET_API ServerOnResultMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_S2C_SERVER_ON_RESULT};
public:
    ServerOnResultMessage(ErrorCode ec = EC_UNKNOWN,
        std::vector<std::string> failedServices = std::vector<std::string>()) :
        ec_(ec),
        failedServices_(failedServices)
        {}
public:
    ErrorCode getErrorCode() const
    {
        return ec_;
    }   
    const std::vector<std::string> getFailedSericeNames() const 
    {
        return failedServices_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "ServerOnResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ErrorCode ec_;
    std::vector<std::string> failedServices_;
};


/*
* @class ServerOffResultMessage
*
*  서버 서비스 Off 요청 결과 메세지.
* - 응답: 무
*/
class WARNET_API ServerOffResultMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_S2C_SERVER_OFF_RESULT};
public:
    ServerOffResultMessage(ErrorCode ec = EC_UNKNOWN,
        std::vector<std::string> failedServices = std::vector<std::string>()):
        ec_(ec),
        failedServices_(failedServices)
        {}
public:
    ErrorCode getErrorCode() const
    {
        return ec_;
    }
    const std::vector<std::string> getFailedSericeNames() const 
    {
        return failedServices_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "ServerOffResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ErrorCode ec_;
    std::vector<std::string> failedServices_;
};


/*
* @class FileCreateMessege
*
*  파일 만들어라.
* - 응답: 무
*/
class WARNET_API FileCreateMessege : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_FILE_CREAE};
public:
    FileCreateMessege( const std::string& fileName = "",
        size_t totalFileSize = 0):
      fileName_(fileName),
      totalFileSize_(totalFileSize)
      {}
public:
    const std::string& getFileName() const
    {
        return fileName_;
    }
    size_t getTotalFileSize() const 
    {
        return totalFileSize_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "FileCreateMessege";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    std::string fileName_;
    size_t totalFileSize_;
};


/*
* @class FileWriteMessege
* wanning::4000바이트 이성 넣지마....
*  파일을 다 써라.
* - 응답: 무
*/
class WARNET_API FileWriteMessege : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_FILE_WRITE,
        MAXDATALEN = 4000};
public:
    FileWriteMessege(size_t partFileSize = 0, const char* pData = NULL):
        partFileSize_(partFileSize)
    {
        ::ZeroMemory(data_,MAXDATALEN);  
        memcpy(data_, pData, partFileSize);
    }
public:
    const char* getData() const
    {
        return data_;
    }
    size_t getPartFileSize() const 
    {
        return partFileSize_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "FileWriteMessege";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    size_t partFileSize_;
    char data_[MAXDATALEN];
};


/*
* @class FileCloseMessege
*
*  파일을 다 써라.
* - 응답: FileResultMessege
*/
class WARNET_API FileCloseMessege : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_FILE_CLOSE};
public:
    FileCloseMessege(int index = 0) : index_(index)
    {}
public:
    int getIndex() const
    {
        return index_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "FileCloseMessege";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    int index_;
};


/*
* @class FileResultMessege
*
*  파일을 닫아라.
* - 응답: 결과
*/
class WARNET_API FileResultMessege : public Message
{
public:
    enum { thisMessageType = MT_MS_S2C_FILE_RESULT};
public:
    FileResultMessege(ErrorCode ec= EC_UNKNOWN, int index = 0):
      ec_(ec), index_(index)
    {}
public:
    ErrorCode getErrorCode() const
    {
        return ec_;
    }
    int getIndex() const
    {
        return index_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "FileResultMessege";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ErrorCode ec_;
    int index_;
};


class WARNET_API CreateManagerUserMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_CREATE_MANAGER_USER};
public:
    CreateManagerUserMessage(mode::AuthenticMode mode = mode::MODE_UNKNOWN,
        const std::string& userId = "",
        const std::string& pw = "" , ToolUseGrade grade = TUG_GENERAL):
    mode_(mode), userId_(userId), pw_(pw), grade_(grade) 
    {}
public:
    mode::AuthenticMode getAuthenticMode() const{
        return mode_;
    }
    const std::string& getUserId() const {
        return userId_;
    }
    const std::string& getPassword() const {
        return pw_;
    }
    ToolUseGrade getToolUseGrade() const {
        return grade_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "CreateManagerUserMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    mode::AuthenticMode mode_;
    std::string userId_;
    std::string pw_;
    ToolUseGrade grade_;
};


class WARNET_API DeleteManagerUserMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_C2S_DELETE_MANAGER_USER};
public:
    DeleteManagerUserMessage(mode::AuthenticMode mode = mode::MODE_UNKNOWN,
        const std::string &userId = ""):
      mode_(mode),
          userId_(userId){}
public:
    mode::AuthenticMode getAuthenticMode() const{
        return mode_;
    }
    const std::string& getUserId() const {
        return userId_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "DeleteManagerUserMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    mode::AuthenticMode mode_;
    std::string userId_;
};

class WARNET_API ManagerUserActionResultMessage : public Message
{
public:
    enum { thisMessageType = MT_MS_S2C_MANAGER_USER_ACTION_RESULT};
public:
    ManagerUserActionResultMessage(ErrorCode ec = EC_UNKNOWN) : ec_(ec)
    {}
public:
    ErrorCode getErrorCode() const
    {
        return ec_;
    }
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "ManagerUserActionResultMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
private:
    ErrorCode ec_;
};

} // namespace skynet

#endif // !defined(INCLUDED_MANAGERSERVERMESSAGE)
