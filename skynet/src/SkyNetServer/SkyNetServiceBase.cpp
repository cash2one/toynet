/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author ±èº´¼ö <kcando@megaking.co.kr>
*/
// WarNetServiceBase.cpp
//
#include "stdafx.h"
#include <skynet/server/SkyNetServiceBase.h>
#include <skynet/server/CrashHandler.h>
#include <skynet/detail/Logging.h>

#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4251 4267 4311 4312 4702 )
#include <ace/Reactor.h>
#include <ace/Proactor.h>
#include <ace/Configuration_Import_Export.h>
#include <ace/Get_Opt.h>
#include <ace/OS.h>
#pragma warning ( pop )
#include <fstream>

namespace skynet
{

namespace
{

ACE_TCHAR s_homeDirEnv[] = ACE_TEXT("WARNET_HOME");
ACE_TCHAR s_binDir[] = ACE_TEXT("bin");
ACE_TCHAR s_configDir[] = ACE_TEXT("conf");
ACE_TCHAR s_logDir[] = ACE_TEXT("log");

void writePid(const std::string& logDir, const std::string& serviceName)
{
    const std::string filename(logDir + '\\' + serviceName + ".pid");
    std::ofstream pidFile(filename.c_str());
    if (! pidFile) {
        WN_LOG_ERROR2(ACE_TEXT("Can't write to PID file(%s).\n"),
            filename.c_str());
    }
    else {
        pidFile << ACE_OS::getpid();
    }
}

} // namespace

// = WarNetServiceBase

WarNetServiceBase::WarNetServiceBase() :
    listenPort_(ACE_DEFAULT_SERVER_PORT),
    workerThreadCount_(DEFAULT_WORKER_THREAD_COUNT)
{
}


WarNetServiceBase::~WarNetServiceBase()
{
}


int WarNetServiceBase::init(int argc, ACE_TCHAR* argv[])
{
    if (! initEnvironment()) {
        return false;
    }

    if (! parseCommandLine(argc, argv)) {
        return -1;
    }

    if (! parseConfigFile()) {
        return -1;
    }

    boost::filesystem::path logDir(homeDir_, boost::filesystem::native);
    logDir /= s_logDir;

    initCrashHandler(logDir.string().c_str(), serviceName_.c_str());

    writePid(logDir.string(), serviceName_);

    if (! onInit())
        return -1;

    return run() ? 0 : -1;
}


int WarNetServiceBase::fini()
{
    onFinish();

    wait();

    onShutdown();

    return 0;
}


bool WarNetServiceBase::run()
{
    return activate(THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED,
        workerThreadCount_) == 0;
}


bool WarNetServiceBase::initEnvironment()
{
    ACE_TCHAR* home_dir = ACE_OS::getenv(s_homeDirEnv);
    if (! home_dir) {
        WN_LOG_ERROR2(ACE_TEXT("%s environment variable is not setted!!!\n"),
            s_homeDirEnv);
        return false;
    }
    homeDir_ = home_dir;
    return true;
}


boost::filesystem::path WarNetServiceBase::getConfigDir() const
{
    boost::filesystem::path configDir(homeDir_, boost::filesystem::native);
    configDir /= s_configDir;
    return configDir;
}


boost::filesystem::path WarNetServiceBase::getBinDir() const
{
    boost::filesystem::path binDir(homeDir_, boost::filesystem::native);
    binDir /= s_binDir;
    return binDir;
}


bool WarNetServiceBase::parseCommandLine(int argc, ACE_TCHAR* argv[])
{
    ACE_Get_Opt cmd_opts(argc, argv, ACE_TEXT("h:c:"), 0);
    if (cmd_opts.long_option(ACE_TEXT("home"), 'h',
            ACE_Get_Opt::ARG_REQUIRED) == -1)
        return false;
    if (cmd_opts.long_option(ACE_TEXT("config"), 'c',
            ACE_Get_Opt::ARG_REQUIRED) == -1)
        return false;

    int option;
    while ((option = cmd_opts()) != EOF) {
        switch (option) {
        case 'h':
            homeDir_ = cmd_opts.opt_arg();
            break;
        case 'c':
            configFile_ = cmd_opts.opt_arg();
            break;
        }
    } // while

    if (configFile_.empty()) {
        WN_LOG_ERROR(ACE_TEXT("Config file(-c option) is not specified.\n"));
        return false;
    }

    return true;
}


bool WarNetServiceBase::parseConfigFile()
{
    WN_ASSERT(! configFile_.empty());

    boost::filesystem::path configPath(getConfigDir() / configFile_);

    ACE_Configuration_Heap config;
    if (config.open() == -1) {
        WN_LOG_ERROR2(ACE_TEXT("%p\n"), configPath.string().c_str());
        return false;
    }
    ACE_Ini_ImpExp config_importer(config);
    if (config_importer.import_config(configPath.string().c_str()) == -1) {
        WN_LOG_ERROR2(ACE_TEXT("%p\n"), configPath.string().c_str());
        return false;
    }

    ACE_TString name;
    if (! getConfigString(name, config, ACE_TEXT("Service"),
            ACE_TEXT("Name")))
        return false;
    serviceName_ = name.c_str();

    int port;
    if (! getConfigInt(port, config, ACE_TEXT("Service"),
            ACE_TEXT("ListenPort")))
        return false;
    listenPort_ = static_cast<unsigned short>(port);

    (void)getConfigInt(workerThreadCount_, config, ACE_TEXT("Service"),
        ACE_TEXT("WorkerThread"), 1);

    return onParseConfigFile(config);
}


bool WarNetServiceBase::getConfigString(ACE_TString& value,
    ACE_Configuration_Heap& config, const ACE_TCHAR* sectionName,
    const ACE_TCHAR* keyName)
{
    ACE_Configuration_Section_Key section;
    if (config.open_section(config.root_section(), sectionName, 0,
            section) == -1) {
        WN_LOG_ERROR2(
            ACE_TEXT("CONFIG: Can't open %s section\n"), sectionName);
        return false;
    }

    ACE_TString keyValue;
    if (config.get_string_value(section, keyName,
            keyValue) == -1) {
        WN_LOG_ERROR3(
            ACE_TEXT("CONFIG: %s/%s does not exist\n"), sectionName, keyName);
        return false;
    }

    value = keyValue;
    return true;
}


bool WarNetServiceBase::getConfigInt(int& value,
    ACE_Configuration_Heap& config, const ACE_TCHAR* sectionName,
    const ACE_TCHAR* keyName, int defaultValue)
{
    ACE_TString keyValue;
    if (getConfigString(keyValue, config, sectionName, keyName)) {
        value = ACE_OS::atoi(keyValue.c_str());
        return true;
    }

    value = defaultValue;
    return false;
}

} // namespace skynet
