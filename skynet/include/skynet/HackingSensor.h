/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
* �ٽ��� Ŭ���̾�Ʈ ó�� ��
*/
class HackingSensor
{
public:
    /**
     * _AhnHS_MakeGuidAckMsg�� ȣ���Ͽ� ���� �޼����� ������ �Ѵ�
     * @param guidAckMsg ������ ������ ���� �޼���
     * @param guidReqMsg ������ ���� ���޵� �䱸 �޼���
     */
    virtual void makeGuidAckMsg(unsigned char* guidAckMsg,
        const unsigned char* guidReqMsg) = 0;

    /**
    * _AhnHS_MakeAckMsg�� ȣ���Ͽ� CRC ���� �޼����� ������ �Ѵ�
    * @param ackMsg ������ ������ CRC ���� �޼���
    * @param reqMsg ������ ���� ���޵� CRC �䱸 �޼���
    */
    virtual void makeAckMsg(unsigned char* ackMsg,
        const unsigned char* reqMsg) = 0;
};

} // namespace skynet

#endif // #if !defined(INCLUDED_HACKINGSENSOR)
