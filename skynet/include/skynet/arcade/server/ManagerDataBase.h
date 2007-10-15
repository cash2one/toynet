#if !defined(INCLUDED_MANAGERDATABASE)
#define INCLUDED_MANAGERDATABASE

#include <skynet/server/Database.h>
#include <skynet/arcade/ArcadeServerInfo.h>
#include <skynet/arcade/ArcadeErrorCode.h>
#include <skynet/arcade/ArcadeUserInfo.h>
#include <string>

namespace skynet {


class ManagerDatabase :public Database
{
public:
    virtual bool createMonitorToolUser(const std::string& id,
        const std::string& pw, ToolUseGrade& grade, ErrorCode& ec) = 0;
    virtual bool createServerToolUser(const std::string& id,
        const std::string& pw, ToolUseGrade& grade, ErrorCode& ec) = 0;
    virtual bool deleteMonitorToolUser(const std::string& id, ErrorCode& ec) = 0;
    virtual bool deleteServerToolUser(const std::string& id, ErrorCode& ec) = 0;

    virtual bool serverToolLogin(const std::string& id, const std::string& pw,
        ToolUseGrade& grade, ErrorCode& ec) = 0;
    virtual bool monitorToolLogin(const std::string& id, const std::string& pw,
        ToolUseGrade& grade, ErrorCode& ec) = 0;
    virtual bool monitorToolLogout(const std::string& id) = 0;
    virtual bool serverToolLogout(const std::string& id) = 0;    
    virtual bool ToolAllLogout() = 0;    
    //���� �ʱ�ȭ�� ���� ����Ʈ ��ΰ�������(���� ����)
    virtual bool getMyServerList(const std::string& ip,
        ServerListInfos& serverListInfos) = 0;
    //������ �α��� �Ҷ� ���� �ѱ�� ��������Ʈ(�� ����)
    virtual bool getLoginServerList(ServerListInfos& serverListInfos) = 0;
    //�α� ��� �ø���
    virtual bool saveNoticeLog(const std::string& serviceName,
        const std::string& fromNickname, const std::string& toNickname,
        const std::string& notice, ErrorCode ec, int& idx) = 0;
    virtual bool saveServerOnOffLog(const std::string& serviceName,
        const std::string& userId, bool isOn, ErrorCode ec, int& idx) = 0;
    virtual bool saveServerUpdateLog(const std::string& serverIp,
        const std::string& userId, ErrorCode ec, int& idx) = 0;
    //�α� �ϼ� �ø���
    virtual bool updaterecvNoticeCheck(int index, ErrorCode ec, 
        bool isSuccess) = 0;
    virtual bool updateServerOnOffLog(int index, ErrorCode ec,
        bool isSuccess) = 0;
    virtual bool updateServerUpdateLog(int index, ErrorCode ec, 
        bool isSuccess) = 0;
};
}
#endif