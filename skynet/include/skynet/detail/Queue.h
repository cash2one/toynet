/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// Queue.h
//
#if !defined(INCLUDED_QUEUE)
#define INCLUDED_QUEUE

#include <ace/Thread_Mutex.h>
#include <ace/Condition_Thread_Mutex.h>
#include <queue>
#include <skynet/detail/Assert.h>

namespace skynet
{   

/**
* @class Queue
*  - thread-safe queue
*/
template <typename T>
class Queue
{
    typedef std::queue<T> QueueType;
public:
    Queue() :
        available_(queueLock_) {}
    ~Queue() {}

    void push(const T& item) {
        ACE_GUARD(ACE_Thread_Mutex, guard, queueLock_);
        items_.push(item);
        available_.signal();
    }

    /**
     * @param timeout 절대시간. 0이면 무한 대기
     */
    bool pop(T& item, ACE_Time_Value* timeout = 0) {
        ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, queueLock_, false);

        while (items_.empty()) {
            if (available_.wait(timeout) == -1)
                return false; // time-out
        }

        WN_ASSERT(! items_.empty());
        item = items_.front();
        items_.pop();
        return true;
    }
protected:
    QueueType& getQueue() {
        return items_;
    }
    
    ACE_Thread_Mutex& getLock() {
        return queueLock_;    
    }
private:
    QueueType items_;
    ACE_Thread_Mutex queueLock_;
    ACE_Condition_Thread_Mutex available_;
};


/**
* @class PtrQueue
*  - thread-safe queue
*/
template <typename T>
class PtrQueue : public Queue<T*>
{
public:
    ~PtrQueue() {
        ACE_Time_Value nowait(ACE_Time_Value::zero);
        T* item;
        while (pop(item, &nowait)) {
            delete item;
        }
    }
};


} // namespace skynet

#endif // !defined(INCLUDED_QUEUE)
