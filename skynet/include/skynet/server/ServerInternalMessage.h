/**
* Copyright (c) 
*
* @author 
*/
// ServerInternalMessage.h
#if !defined(INCLUDED_SERVERINTERNALMESSAGE)
#define INCLUDED_SERVERINTERNALMESSAGE

#include <skynet/Message.h>

namespace skynet
{

/// ���� ���ο��� ���Ǵ� ��Ŷ
enum ServerInternalMessageType_t
{
    MT_SI_LOGOUT = (MT_SI_START + 10)
};


/**
* @class LogoutMessage
*
* GameServer �α׾ƿ� ó���� �޼���.
*/
class WARNET_API LogoutMessage : public Message
{
public:
    enum { thisMessageType = MT_SI_LOGOUT };
public:
    LogoutMessage() {}
private:
    virtual MessageType getThisMessageType() const {
        return thisMessageType;
    }
    virtual const char* getThisClassName() const {
        return "LogoutMessage";
    }
    virtual bool readBody(Wdr::InputWdr& wdr);
    virtual bool writeBody(Wdr::OutputWdr& wdr) const;
    virtual size_t getBodySize() const;
};

} // namespace skynet

#endif // !defined(INCLUDED_SERVERINTERNALMESSAGE)
