#if !defined(INCLUDED_MANAGERCLIENTSERVICEHANDLER)
#define INCLUDED_MANAGERCLIENTSERVICEHANDLER

#include <skynet/server/ClientId.h>
#include <string>
#include <skynet/detail/Assert.h>
#pragma  warning (disable: 4100)
namespace skynet
{

class WARNET_API ManagerClientServiceHandler
{
public:
    virtual void requestServerInfo(ClientId clientId) = 0;
    virtual void noticeToUser(const std::string& managerCookie,
        const std::string& from, const std::string& to, const std::string& msg)
    {
            WN_ASSERT(false);
    }
    virtual void noticeEx(const std::string& managerCookie,
        const std::string& from, const std::string& msg)
    {
            WN_ASSERT(false);
    }
};
}


#endif