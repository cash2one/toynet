/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// AceUtil.h : ACE utilities
//
#if !defined(INCLUDED_ACEUTIL)
#define INCLUDED_ACEUTIL

#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4267 4311 4312 )
#include <ace/Log_Msg.h>
#include <ace/Message_Block.h>
#include <ace/ACE.h>
#pragma warning ( pop )
#include <boost/utility.hpp>
#include <skynet/detail/Assert.h>

class ACE_Handler;

namespace skynet
{

/// ACE 라이브러리를 초기화한다
struct InitAce
{
    InitAce() {
        ACE::init();
    }

    ~InitAce() {
        ACE::fini();
    }
};

/// 소켓의 송신/수신 버퍼를 최대치로 설정한다.
bool WARNET_API setMaximumSocketBufferSize(ACE_HANDLE handle);

/// Proactor 타이머를 설정한다
long WARNET_API setupTimer(ACE_Handler& handler, long timeout,
    const void* act = 0);

/// Proactor 반복 타이머를 설정한다
long WARNET_API setupRepeatingTimer(ACE_Handler& handler, long timeout,
    const void* act = 0);

/// Proactor 타이머를 해제한다
void WARNET_API cancelTimer(long& timer_id);

// Get Reactor Interface 



/**
 * @class AceMessageBlockGuard
 *
 * ACE_Message_Block Guard Class
 */
class AceMessageBlockGuard : public boost::noncopyable
{
public:
    AceMessageBlockGuard(ACE_Message_Block* mblk) :
        mblk_(mblk) {}

    ~AceMessageBlockGuard() {
        if (mblk_ != 0)
            mblk_->release();
    }

    ACE_Message_Block* operator->() const {
        return mblk_;
    }

    ACE_Message_Block& operator*() const {
        WN_ASSERT(mblk_ != 0);
        return *mblk_;
    }

    ACE_Message_Block* get() const {
        return mblk_;
    }
private:
    ACE_Message_Block* mblk_;
};

} // namespace skynet

#endif // !defined(INCLUDED_ACEUTIL)
