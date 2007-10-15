/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author ±èº´¼ö <kcando@megaking.co.kr>
*/
// GameClientSessionPool.cpp
//
#include "stdafx.h"
#include <skynet/server/GameClientSessionPool.h>
#include <skynet/server/GameClientSession.h>
#include <skynet/detail/Logging.h>
#include <skynet/detail/Assert.h>

namespace skynet
{


GameClientSessionPool::GameClientSessionPool(WarNetService* service,
        MessageBuilderRepository& builderRepository, size_t lowWaterMark,
        bool useCipher) :
    service_(service),
    builderRepository_(builderRepository),
    useCipher_(useCipher),
    lowWaterMark_(lowWaterMark),
    reservedSize_(0)
{
    reserve();
}


GameClientSessionPool::~GameClientSessionPool()
{
    while (! queue_.empty()) {
        GameClientSession* session = queue_.front();
        delete session;
        queue_.pop();
    }
}


GameClientSession* GameClientSessionPool::acquire()
{
    ACE_GUARD_RETURN(ACE_Thread_Mutex, lock, queueLock_, 0);

    if (queue_.size() <= lowWaterMark_) {
        reserve();
    }

    WN_ASSERT(! queue_.empty());
    GameClientSession* session = queue_.front();
    queue_.pop();

    return session;
}


void GameClientSessionPool::release(GameClientSession* session)
{
    WN_ASSERT(session != 0);

    ACE_GUARD(ACE_Thread_Mutex, lock, queueLock_);

    if (session != 0) {
        queue_.push(session);
    }
}


size_t GameClientSessionPool::getSize() const
{
    ACE_GUARD_RETURN(ACE_Thread_Mutex, lock, queueLock_, 0);

    return queue_.size();
}


void GameClientSessionPool::reserve()
{
    for (size_t i = (lowWaterMark_ * 2) - queue_.size(); i > 0 ; --i) {
        queue_.push(new GameClientSession(service_, &builderRepository_,
            useCipher_));
        ++reservedSize_;
    }
    WN_ASSERT(queue_.size() == (lowWaterMark_ * 2));

    WN_LOG_DEBUG2(ACE_TEXT("GameClientSessionPool %d reserved.\n"),
        reservedSize_);
}

} // namespace skynet
