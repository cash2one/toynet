/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// GameClientSessionPool.h
#if !defined(INCLUDED_GAMECLIENTSESSIONPOOL)
#define INCLUDED_GAMECLIENTSESSIONPOOL

#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4251 4267 4311 4312 4355 )
#include <ace/Thread_Mutex.h>
#pragma warning(pop)
#include <boost/utility.hpp>
#include <queue>

namespace skynet
{

class WarNetService;
class GameClientSession;
class MessageBuilderRepository;

/**
 * @class GameClientSessionPool
 *
 * ClientSession 메모리 pool
 */
class WARNET_API GameClientSessionPool : public boost::noncopyable
{
    typedef std::queue<GameClientSession*> Queue;
public:
    enum { DEFAULT_LOW_WARTER_MARK = 1000 };
public:
    GameClientSessionPool(WarNetService* service,
        MessageBuilderRepository& builderRepository,
        size_t lowWaterMark = DEFAULT_LOW_WARTER_MARK, bool useCipher = false);

    ~GameClientSessionPool();

    GameClientSession* acquire();

    void release(GameClientSession* session);

    size_t getSize() const;
private:
    void reserve();
private:
    WarNetService* service_;
    MessageBuilderRepository& builderRepository_;
    size_t lowWaterMark_;
    bool useCipher_;

    Queue queue_;
    mutable ACE_Thread_Mutex queueLock_;

    size_t reservedSize_;
};

} // namespace skynet

#endif // !defined(INCLUDED_GAMECLIENTSESSIONPOOL)
