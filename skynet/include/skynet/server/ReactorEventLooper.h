/**
* Copyright (c) 
*
* @author 
*/
// ReactorEventLooper.h
#if !defined(INCLUDED_REACTOREVENTLOOPER)
#define INCLUDED_REACTOREVENTLOOPER

#include <skynet/detail/Logging.h>
#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4251 4267 4311 4312 4702 )
#include <ace/Reactor.h>
#include <ace/Task.h>
#pragma warning ( pop )

namespace skynet
{

/**
* @class ReactorEventLooper
*
*/
class ReactorEventLooper : public ACE_Task_Base
{
public:
    ReactorEventLooper() :
        stop_(false) {}
    ~ReactorEventLooper() {
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
                ACE_Reactor::instance()->owner(ACE_OS::thr_self());
                ACE_Reactor::instance()->end_reactor_event_loop();
            }
            wait();
        }
    }
private:
    virtual int svc() {
        ACE_Reactor::instance()->owner(ACE_OS::thr_self());
        while ((! stop_) &&
                (ACE_Reactor::instance()->reactor_event_loop_done() == 0)) {
            (void)ACE_Reactor::instance()->handle_events ();
        }
        WN_LOG_DEBUG("ReactorEventLooper thread is destoryed.\n");
        return 0;
    }
private:
    bool stop_;
};

} // namespace skynet

#endif // !defined(INCLUDED_REACTOREVENTLOOPER)
