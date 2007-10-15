/**

* Copyright (c) 2004 Mega Enterprise Co., Ltd.

*

* @author 김병수 <kcando@megaking.co.kr>

*/

#if !defined(INCLUDED_LOGMANAGER)

#define INCLUDED_LOGMANAGER



#include <ace/Singleton.h>

#include "ace/streams.h"

#include <ace/Log_Msg_Callback.h>



namespace skynet

{



/**

* @class LogManager

*  - ACE 로그의 출력을 바꿔준다

*/ 

class WARNET_API LogManager

{

public:

    LogManager();

    ~LogManager();



    void setMask(u_long flag);



    void silent();

    

    void redirectToOStream(ACE_OSTREAM_TYPE* output, bool onlyStderr = true);

    void redirectToFile(const std::string& filename, bool onlyOstream);



    void redirectToStderr(bool onlyStderr = true);

    void redirectToSyslog(const ACE_TCHAR* prog_name);

    ACE_Log_Msg_Callback* redirectToCallback(ACE_Log_Msg_Callback* callback,

        bool onlyCallback = true);

private:

    ACE_OSTREAM_TYPE* output_stream_;

    std::ofstream* log_stream_;

};



typedef ACE_Singleton<LogManager, ACE_Thread_Mutex> LOG_MANAGER;



} // namespace skynet



#endif // !defined(INCLUDED_LOGMANAGER)

