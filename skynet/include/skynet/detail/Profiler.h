/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// Profiler.h
#if !defined(INCLUDED_PROFILER)
#define INCLUDED_PROFILER

#ifdef USE_PROFILER
#include "Logging.h"
#include <ace/Profile_Timer.h>
#include <boost/utility.hpp>
#endif // USE_PROFILER
#include <string>

namespace skynet
{

/**
 * @class Profiler
 *
 * 게임 모드 베이스 클래스
 */
#ifdef USE_PROFILER
class Profiler : public boost::noncopyable
{
public:
    Profiler(const std::string& entry) :
            entry_(entry) {
        profileTimer_.start();
    }

    ~Profiler() {
        profileTimer_.stop();
        ACE_Profile_Timer::ACE_Elapsed_Time et;
        profileTimer_.elapsed_time(et);
        WN_LOG_INFO5(
            ACE_TEXT("Transaction<%s>: real time = %.3f secs, ")
            ACE_TEXT("user time = %.3f secs, system time = %.3f secs\n"),
            entry_.c_str(), et.real_time, et.user_time, et.system_time);
    }
private:
    std::string entry_;
    ACE_Profile_Timer profileTimer_;
};
#else // USE_PROFILER
class Profiler
{
public:
    Profiler(const std::string& /*entry*/) {}
    ~Profiler() {}
};
#endif // USE_PROFILER

} // namespace skynet

#endif // !defined(INCLUDED_PROFILER)
