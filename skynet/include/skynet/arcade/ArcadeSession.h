/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// PoporuSession.h
//
#if !defined(INCLUDED_POPORUSESSION)
#define INCLUDED_POPORUSESSION

#include "../WarNetSession.h"

namespace skynet
{

class MsgQueue;

/**
 * @class PoporuSession
 * 포포루 전용 클라이언트 세션.
 * - 서버로 부터 도착한 메세지를 큐에 저장한다
 * - 메세지 전송/수신 이벤트 로그를 큐에 저장한다
 */
class WARNET_API PoporuSession : public WarNetSession
{
public:
    /**
     * @param adaptor
     * @param useHackShield 핵쉴드를 사용할 것인가? true라면
     *        반드시 HackingSensor를 정의하여 registerHackingSensor를
     *        호출해야 한다
     * @param debugOff true이면 디버그 정보를 로그 큐에 저장하지 않는다
     */
    PoporuSession(SessionAdaptor* adaptor = 0, bool useHackShield = false,
        bool debugOff = true);

    /// dtor
    virtual ~PoporuSession();

    /**
     * 서버로 부터 도착한 메세지를 얻는다(내부적으로 큐에 저장함)
     * @return Message 객체 - !!! 사용한 후에 반드시 메모리를 해제해야 한다 !!! 
     */
    const Message* getMessage();
    MessageType getHeadMsgType();

    /**
     * 메세지 전송/수신에 대한 로그를 얻는다(내부적으로 큐에 저장함)
     * @return 로그가 없으면 false, 그렇지 않으면 true
     */
    bool getLog(std::string& log);

    void putMessage(const Message* msg);

    static void initialize();
    static void finalize();
protected:
    MsgQueue* msgQ_;
};

} // namespace skynet

#endif // !defined(INCLUDED_POPORUSESSION)
