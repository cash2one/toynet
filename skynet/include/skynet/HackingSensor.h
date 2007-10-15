/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// HackingSensor.h
//

#if !defined(INCLUDED_HACKINGSENSOR)
#define INCLUDED_HACKINGSENSOR

namespace skynet
{

/**
* @class HackingSensor
*
* 핵쉴드 클라이언트 처리 부
*/
class HackingSensor
{
public:
    /**
     * _AhnHS_MakeGuidAckMsg를 호출하여 응답 메세지를 만들어야 한다
     * @param guidAckMsg 서버로 전송할 응답 메세지
     * @param guidReqMsg 서버로 부터 전달된 요구 메세지
     */
    virtual void makeGuidAckMsg(unsigned char* guidAckMsg,
        const unsigned char* guidReqMsg) = 0;

    /**
    * _AhnHS_MakeAckMsg를 호출하여 CRC 응답 메세지를 만들어야 한다
    * @param ackMsg 서버로 전송할 CRC 응답 메세지
    * @param reqMsg 서버로 부터 전달된 CRC 요구 메세지
    */
    virtual void makeAckMsg(unsigned char* ackMsg,
        const unsigned char* reqMsg) = 0;
};

} // namespace skynet

#endif // #if !defined(INCLUDED_HACKINGSENSOR)
