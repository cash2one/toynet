/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// DatabasePoolFactory.h
#if !defined(INCLUDED_DATABASEPOOLFACTORY)
#define INCLUDED_DATABASEPOOLFACTORY

#include "../../server/DatabasePool.h"

namespace skynet
{

/**
* @class DatabasePoolFactory
*
* Popuru_Web ���� �����ͺ��̽�
*/
struct WARNET_API DatabasePoolFactory
{
    static DatabasePool* createWebDatabasePool();
    static DatabasePool* createGameDatabasePool();
    static DatabasePool* createLogDatabasePool();
    static DatabasePool* createItemDatabasePool();
    static DatabasePool* createMessengerDatabasePool();
    static DatabasePool* createManagerDatabasePool();
};

} // namespace skynet

#endif // !defined(INCLUDED_DATABASEPOOLFACTORY)
