/**
* Copyright (c) 
*
* @author 
*/
// ProactorEventLooper.h
#if !defined(INCLUDED_PROACTOREVENTLOOPER)
#define INCLUDED_PROACTOREVENTLOOPER

#include <skynet/detail/Logging.h>
#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4251 4267 4311 4312 4702 )
#include <ace/Proactor.h>
#include <ace/Task.h>
#pragma warning ( pop )

namespace skynet
{

/**
* @class ProactorEventLooper
*
*/
class ProactorEventLooper : public ACE_Task_Base
{
public:
    ProactorEventLooper() :
        stop_(false) {}
    ~ProactorEventLooper() {
        stop();
    }

    bool start(int threadCount) {
        const long thread_flag =
            THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED;
        return activate(thread_flag, threadCount) == 0;
    }

    void stop() {
        if (! stop_) {
            stop_ = true;
            while (thr_count() > 0) {
                ACE_Proactor::instance()->end_event_loop();
            }
            wait();
        }
    }
private:
    virtual int svc() {
        while ((! stop_) &&
                (ACE_Proactor::instance()->event_loop_done() == 0)) {
            ACE_Time_Value waitingTimeout(0,
                ACE_U_ONE_SECOND_IN_USECS / 10); // 0.1ÃÊ
            (void)ACE_Proactor::instance()->handle_events(waitingTimeout);
        }
        WN_LOG_DEBUG("ProactorEventLooper thread is destoryed.\n");
        return 0;
    }
private:
    bool stop_;
};

} // namespace skynet

#endif // !defined(INCLUDED_PROACTOREVENTLOOPER)
