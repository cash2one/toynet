/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// DatabasePool.cpp
//
#include "stdafx.h"
#include <skynet/server/DatabasePool.h>
#include <skynet/server/Database.h>
#include <skynet/detail/Logging.h>
#include <skynet/detail/Assert.h>

/// ADO connection pool을 사용할 것인가?
//#define USE_ADO_CONNECTION_POOL

namespace skynet
{

DatabasePool::DatabasePool() :
    idleDatabase_(0),
    available_(monitor_)
{
}


DatabasePool::~DatabasePool()
{
    for (Databases::iterator pos = databases_.begin();
            pos != databases_.end(); ++pos) {
        delete (*pos).second;
    }
}


bool DatabasePool::open(size_t connectionCount,
    const std::string& conectionString)
{
    for (size_t count = 0; count < connectionCount; ++count) {
        std::auto_ptr<Database> database(createDatabase(conectionString));
        if (! database->open()) {
            WN_LOG_ERROR3(ACE_TEXT("DatabasePool::open(%d,%s)\n"),
                database->getLastResult(), database->getLastError().c_str());
            return false;
        }
#ifdef USE_ADO_CONNECTION_POOL
        database->close();
#endif
        databases_.push_back(Item(false, database.release()));
    }
    idleDatabase_ = connectionCount;
    return true;
}


Database* DatabasePool::acquire()
{
    ACE_GUARD_RETURN(ACE_Thread_Mutex, lock, monitor_, 0);

    while (idleDatabase_ <= 0) {
        available_.wait();
    }
    --idleDatabase_;

    for (Databases::iterator pos = databases_.begin();
            pos != databases_.end(); ++pos) {
        if (! (*pos).first) {
            (*pos).first = true;
            Database* db = (*pos).second;
            WN_ASSERT(db != 0);
#ifdef USE_ADO_CONNECTION_POOL
            (void)db->open();
#endif
            return db;
        }
    }
    WN_ASSERT(false);
    return 0;
}


void DatabasePool::release(Database* db)
{
    WN_ASSERT(db != 0);

    ACE_GUARD(ACE_Thread_Mutex, lock, monitor_);

    for (Databases::iterator pos = databases_.begin();
            pos != databases_.end(); ++pos) {
        if ((*pos).second == db) {
            WN_ASSERT((*pos).first == true);
#ifdef USE_ADO_CONNECTION_POOL
            db->close();
#endif
            (*pos).first = false;
            idleDatabase_++;
            available_.signal();
            return;
        }
    }
    WN_ASSERT(false);
}

} // namespace skynet
