/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// MessengerServiceFactory.h
#if !defined(INCLUDED_CHANNELMESSENGERSERVICEFACTORY)
#define INCLUDED_CHANNELMESSENGERSERVICEFACTORY

namespace skynet
{

class DatabasePool;

namespace im
{

class MessengerService;
class MessengerEventHandler;

/**
 * @class MessengerServiceFactory
 *
 * �޽��� ���� ���丮
 */
struct WARNET_API MessengerServiceFactory
{
    static MessengerService* create(MessengerEventHandler& handler,
        DatabasePool& webDatabase,
        DatabasePool& messengerDatabase);
};

} // namespace im

} // namespace skynet

#endif // !defined(INCLUDED_CHANNELMESSENGERSERVICEFACTORY)
