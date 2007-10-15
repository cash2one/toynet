/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author ±èº´¼ö <kcando@megaking.co.kr>
*/
// ReactorLifeCycle.h
//
#if !defined(INCLUDED_REACTORLIFECYCLE)
#define INCLUDED_REACTORLIFECYCLE

#pragma warning ( push )
#pragma warning ( disable: 4251 4267 4311 4355 )
#include <ace/Task.h>
#include <ace/Synch.h>
#pragma warning ( pop )
#include <boost/utility.hpp>
#include <memory>

class ACE_Reactor;
class ACE_Log_Msg_Callback;

namespace skynet
{

/**
* @class ReactorLifeCycle
* -
*/
class WARNET_API ReactorLifeCycle :
    public ACE_Task_Base,
    public boost::noncopyable
{
public:
    ReactorLifeCycle();
    ~ReactorLifeCycle();    

    void addRef(ACE_Log_Msg_Callback* callback = 0);
    void release();

    ACE_Reactor* getReactor() const {
        return reactor_.get();
    }

    virtual int svc();
private:    
    std::auto_ptr<ACE_Reactor> reactor_;

    int refCount_;
    ACE_Thread_Mutex lock_;

    ACE_Log_Msg_Callback* callback_;
};

} // namespace skynet

#endif
