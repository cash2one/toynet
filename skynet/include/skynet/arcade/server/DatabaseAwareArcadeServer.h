/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// DatabaseAwarePoporuServer.h
#if !defined(INCLUDED_DATABASEAWAREPOPORUSERVER)
#define INCLUDED_DATABASEAWAREPOPORUSERVER

#include <skynet/arcade/server/PoporuServer.h>
#include <memory>
#pragma warning(disable : 4150)
namespace skynet
{

class DatabasePool;

/**
 * @class DatabaseAwarePoporuServer
 *
 * 데이터베이스 기능이 추가된 포포루 서버
 */
class WARNET_API DatabaseAwarePoporuServer : public PoporuServer
{
public:
    DatabaseAwarePoporuServer();

    DatabasePool& getWebDatabasePool() {
        return *webDatabasePool_;
    }

    DatabasePool& getGameDatabasePool() {
        return *gameDatabasePool_;
    }

    DatabasePool& getLogDatabasePool() {
        return *logDatabasePool_;
    }

    DatabasePool& getItemDatabasePool() {
        return *itemDatabasePool_;
    }

    DatabasePool& getMessengerDatabasePool() {
        return *messengerDatabasePool_;
    }
    DatabasePool& getManagerDatabasePool() {
        return *managerDatabasePool_;
    }
protected:
    virtual bool onInit();
    virtual bool onParseConfigFile(ACE_Configuration_Heap& config);
private:
    bool initDatabase();
    bool parseDatabaseSection(ACE_Configuration_Heap& config);
    void parseDatabaseConnectionKey(std::string& connection,
        ACE_Configuration_Heap& config, const ACE_TCHAR* key);
    void parseDatabasePoolCountKey(size_t& poolCount,
        ACE_Configuration_Heap& config, const ACE_TCHAR* key);
private:
    std::auto_ptr<DatabasePool> webDatabasePool_;
    std::auto_ptr<DatabasePool> gameDatabasePool_;
    std::auto_ptr<DatabasePool> logDatabasePool_;
    std::auto_ptr<DatabasePool> itemDatabasePool_;
    std::auto_ptr<DatabasePool> messengerDatabasePool_;
    std::auto_ptr<DatabasePool> managerDatabasePool_;


    std::string webConnection_;
    size_t webPoolCount_;
    std::string gameConnection_;
    size_t gamePoolCount_;
    std::string logConnection_;
    size_t logPoolCount_;
    std::string itemConnection_;
    size_t itemPoolCount_;
    std::string messengerConnection_;
    size_t messengerPoolCount_;
    std::string managerConnection_;
    size_t managerPoolCount_;
};

} // namespace skynet

#endif // !defined(INCLUDED_DATABASEAWAREPOPORUSERVER)
