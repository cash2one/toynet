/**

* Copyright (c) 

*

* @author 

*/

// LogManager.cpp

//



#include "stdafx.h"

#include <skynet/detail/LogManager.h>

#include <ace/OS.h>

#include <string>

#include <sstream>



namespace skynet

{



LogManager::LogManager() : output_stream_(0), log_stream_(0)

{

    ACE_LOG_MSG->priority_mask(LM_DEBUG | LM_NOTICE | LM_TRACE | LM_INFO |

        LM_ERROR, ACE_Log_Msg::PROCESS);

}





LogManager::~LogManager()

{

    if( log_stream_) {

        log_stream_->close();

        delete log_stream_;

    }

}





void LogManager::setMask(u_long flag)

{

    ACE_LOG_MSG->priority_mask(flag, ACE_Log_Msg::PROCESS);

}





void LogManager::silent()

{

    ACE_LOG_MSG->set_flags(ACE_Log_Msg::SILENT);

}





void LogManager::redirectToOStream(ACE_OSTREAM_TYPE* output,

    bool onlyOstream)

{

    output_stream_ = output;

    ACE_LOG_MSG->msg_ostream(output_stream_);



    if (onlyOstream) {

        ACE_LOG_MSG->clr_flags(ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER |

            ACE_Log_Msg::MSG_CALLBACK);

    }



    ACE_LOG_MSG->set_flags(ACE_Log_Msg::OSTREAM);    

}





void LogManager::redirectToFile(const std::string& filename, 

    bool onlyOstream)

{

    log_stream_ = new std::ofstream();

    log_stream_->open(filename.c_str(), ios::out | ios::app);

    redirectToOStream(log_stream_, onlyOstream);

}





void LogManager::redirectToStderr(bool onlyStderr)

{

    if (! onlyStderr) {

        ACE_LOG_MSG->clr_flags(ACE_Log_Msg::OSTREAM | ACE_Log_Msg::LOGGER |

            ACE_Log_Msg::MSG_CALLBACK);

    }

    ACE_LOG_MSG->set_flags(ACE_Log_Msg::STDERR);

}





void LogManager::redirectToSyslog(const ACE_TCHAR* prog_name)

{

    ACE_LOG_MSG->open(prog_name, ACE_Log_Msg::SYSLOG, prog_name);

}





ACE_Log_Msg_Callback* LogManager::redirectToCallback(

        ACE_Log_Msg_Callback* callback, bool onlyCallback)

{

    ACE_Log_Msg_Callback* previous = ACE_LOG_MSG->msg_callback(callback);



    if (callback != 0)

        ACE_LOG_MSG->set_flags(ACE_Log_Msg::MSG_CALLBACK);

    else

        ACE_LOG_MSG->clr_flags(ACE_Log_Msg::MSG_CALLBACK);



    if (onlyCallback) {

        ACE_LOG_MSG->clr_flags(ACE_Log_Msg::OSTREAM | ACE_Log_Msg::LOGGER |

            ACE_Log_Msg::STDERR);

    }



    return previous;

}



} // namespace skynet

