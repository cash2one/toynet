/**
* Copyright (c) 
*
* @author 
*/
// WarNetServiceBase.h
#if !defined(INCLUDED_WARNETSERVICEBASE)
#define INCLUDED_WARNETSERVICEBASE

#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4251 4267 4311 4312 4702 )
#include <ace/Task.h>
#include <ace/SStringfwd.h>
#pragma  warning ( pop )
#include <boost/filesystem/operations.hpp>
#include <string>

class ACE_Message_Block;
class ACE_Configuration_Heap;

namespace skynet
{

/**
 * @class WarNetServiceBase
 *
 * 서버의 메인 쓰레드.
 */
class WARNET_API WarNetServiceBase : public ACE_Task_Base
{
private:
    enum { DEFAULT_WORKER_THREAD_COUNT = 1 };

public:
    WarNetServiceBase();
    virtual ~WarNetServiceBase();

    // Service Configurator hook methods.
    virtual int init(int argc, ACE_TCHAR* argv[]);
    virtual int fini();

protected:
    /// 서비스를 시작한다
    bool run();

    const std::string& getServiceName() const {
        return serviceName_;
    }
    unsigned short getListenPort() const {
        return listenPort_;
    }
    boost::filesystem::path getConfigDir() const;
    boost::filesystem::path getBinDir() const;

    bool getConfigInt(int& value, ACE_Configuration_Heap& config,
        const ACE_TCHAR* sectionName, const ACE_TCHAR* keyName,
        int defaultValue = 0);
    bool getConfigString(ACE_TString& value, ACE_Configuration_Heap& config,
        const ACE_TCHAR* sectionName, const ACE_TCHAR* keyName);

    int getWokerThreadCount() const {
        return workerThreadCount_;
    }
private:
    bool initEnvironment();
    /// 초기화
    virtual bool onInit() = 0;
    /// 종료
    virtual void onFinish() = 0;
    /// 서비스 종료 처리 후
    virtual void onShutdown() {}
    /// config. 파일을 파싱하고 처리한다
    virtual bool onParseConfigFile(ACE_Configuration_Heap& config) = 0;
private:
    bool parseCommandLine(int argc, ACE_TCHAR* argv[]);
    bool parseConfigFile();
private:
    std::string serviceName_;

    /// WN_ASSERT(homeDir_ == $WARNET_HOME);
    std::string homeDir_;

    std::string configFile_;

    /// 접속 받을 포트 번호
    unsigned short listenPort_;

    /// woker thread 갯수
    int workerThreadCount_;
};

} // namespace skynet

#endif // !defined(INCLUDED_WARNETSERVICEBASE)
