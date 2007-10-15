/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// Message.h
#if !defined(INCLUDED_MESSAGE)
#define INCLUDED_MESSAGE

#include "Protocol.h"
#include <boost/utility.hpp>
#include <string>

class ACE_Message_Block;

namespace skynet
{

namespace Wdr
{
    class OutputWdr;
    class InputWdr;
} // namespace Wdr

class MessageAdaptor;

/// �޼��� ������ MessageType�� �����Ѵ�
MessageType WARNET_API extractMessageType(ACE_Message_Block& packet);

/// �޼��� ������ body-size�� �����Ѵ�
size_t WARNET_API extractBodySize(ACE_Message_Block& packet);

/// �޼��� ������ �޼��� ����� �����Ѵ�
bool WARNET_API extractHeader(ACE_Message_Block& packet, MessageType& msgType,
    size_t& bodySize);

/**
* @class Message
*
* Message Abstract Class.
* - �޼����� encoding/decoding�Ѵ�
* - <B>CAUTION: wdr stream(operator <<, operator >>)�� �� ��� �̹��� ��������
*   �߻��� �� �ִ�.</B>
*/
class WARNET_API Message : public boost::noncopyable
{
public:
    Message() {}
    virtual ~Message() {}

    /**
     * ����Ʈ ��Ʈ������ �����͸� �о� �޼����� �����Ѵ�.
     * - �ݵ�� packet�� valid�ؾ� �Ѵ�.
     */
    bool serializeFrom(ACE_Message_Block& packet, MessageAdaptor& adaptor);

    /**
     * �޼����� ����Ʈ ��Ʈ������ �����.
     * - packet�� �ݵ�� release()�ؾ� �Ѵ�.
     */
    bool serializeTo(ACE_Message_Block*& packet,
        MessageAdaptor& adaptor) const;

    /// �޼��� Ÿ���� ��´�
    MessageType getMessageType() const {
        return getThisMessageType();
    }

    /// �޼��� �̸��� ��´�(�α� ��� ��)
    const char* getClassName() const {
        return getThisClassName();
    }
public:
    /// �޼��� ��� ũ�⸦ ��´�
    static size_t getHeaderSize();
private:
    /// �޼��� Ÿ�� ����
    virtual MessageType getThisMessageType() const {
        return MT_UNKNOWN;
    }
    /// Ŭ���� �̸� ����(RTTI-like)
    virtual const char* getThisClassName() const = 0;
    /// ��Ŷ���� body �κ��� �о�´�.
    virtual bool readBody(Wdr::InputWdr& wdr) = 0;
    /// ��Ŷ body �κ��� ����Ѵ�.
    virtual bool writeBody(Wdr::OutputWdr& wdr) const = 0;
    /// ����Ʈ ��Ʈ������ ��ȯ�� ��Ŷ body�� ũ��.
    virtual size_t getBodySize() const = 0;
};

} // namespace skynet

#endif // !defined(INCLUDED_MESSAGE)
