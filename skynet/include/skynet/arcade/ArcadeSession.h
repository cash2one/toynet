/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
 * ������ ���� Ŭ���̾�Ʈ ����.
 * - ������ ���� ������ �޼����� ť�� �����Ѵ�
 * - �޼��� ����/���� �̺�Ʈ �α׸� ť�� �����Ѵ�
 */
class WARNET_API PoporuSession : public WarNetSession
{
public:
    /**
     * @param adaptor
     * @param useHackShield �ٽ��带 ����� ���ΰ�? true���
     *        �ݵ�� HackingSensor�� �����Ͽ� registerHackingSensor��
     *        ȣ���ؾ� �Ѵ�
     * @param debugOff true�̸� ����� ������ �α� ť�� �������� �ʴ´�
     */
    PoporuSession(SessionAdaptor* adaptor = 0, bool useHackShield = false,
        bool debugOff = true);

    /// dtor
    virtual ~PoporuSession();

    /**
     * ������ ���� ������ �޼����� ��´�(���������� ť�� ������)
     * @return Message ��ü - !!! ����� �Ŀ� �ݵ�� �޸𸮸� �����ؾ� �Ѵ� !!! 
     */
    const Message* getMessage();
    MessageType getHeadMsgType();

    /**
     * �޼��� ����/���ſ� ���� �α׸� ��´�(���������� ť�� ������)
     * @return �αװ� ������ false, �׷��� ������ true
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
