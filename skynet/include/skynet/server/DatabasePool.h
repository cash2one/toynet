/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// DatabasePool.h
#if !defined(INCLUDED_DATABASEPOOL)
#define INCLUDED_DATABASEPOOL

#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4251 4267 4311 4312 4355 )
#include <ace/Thread_Mutex.h>
#include <ace/Condition_Thread_Mutex.h>
#pragma warning ( pop )
#include <boost/utility.hpp>
#include <string>
#include <vector>

namespace skynet
{

class Database;

/**
* @class DatabasePool
*
* Database connection Pooling
*/
class WARNET_API DatabasePool : public boost::noncopyable
{
private:
    typedef std::pair<bool, Database*> Item;
    typedef std::vector<Item> Databases;
public:
    DatabasePool();
    virtual ~DatabasePool();

    /// connectionCount만큼 db 접속을 한다.
    bool open(size_t connectionCount, const std::string& connectionString);

    /// 휴지 중인 db를 얻는다.
    Database* acquire();

    /// db를 휴지 상태로 바꾼다.
    void release(Database* db);

    DatabasePool* getThis() {
        return this;
    }

protected:
    /// template method.
    virtual Database* createDatabase(const std::string& connectionString) = 0;

private:
    Databases databases_;

    size_t idleDatabase_;
    ACE_Thread_Mutex monitor_;
    ACE_Condition_Thread_Mutex available_;
};


/**
* @class DatabaseGuard
*
* scoped database guard.
*/
template <class DB>
class DatabaseGuard : public boost::noncopyable
{
public:
    DatabaseGuard(DatabasePool& dbp) : dbp_(dbp) {
        database_ = dynamic_cast<DB*>(dbp_.acquire());
    }

    ~DatabaseGuard() {
        dbp_.release(database_);
    }

    DB* operator->() const {
        return database_;
    }

    DB* get() const {
        return database_;
    }
private:
    DatabasePool& dbp_;
    DB* database_;
}; 

} // namespace skynet

#endif // !defined(INCLUDED_DATABASEPOOL)
