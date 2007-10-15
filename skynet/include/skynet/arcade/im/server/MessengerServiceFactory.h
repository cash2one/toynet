/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
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
 * 메신저 서비스 팩토리
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
