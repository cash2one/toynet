/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ForwardHelper.h : UDP ������ ���� ���� �����
#if !defined(INCLUDED_FORWARDHELPER)
#define INCLUDED_FORWARDHELPER

#include <boost/utility.hpp>
#include <memory>

class ACE_Message_Block;

namespace skynet
{

class Message;
class MessageAdaptor;
class MessageBuilderRepository;

/**
 * @class MessageToBuffer
 *
 * Message ��ü���� ����Ʈ ��Ʈ���� ��´�
 */
class WARNET_API MessageToBuffer : public boost::noncopyable
{
public:
    MessageToBuffer();
    MessageToBuffer(const Message& msg);
    ~MessageToBuffer();

    /// msg�� �Ľ��Ͽ� ��ü�� ���ʱ�ȭ�Ѵ�
    bool parse(const Message& msg);

    const char* getBuffer() const;
    size_t getBufferLength() const;

    ACE_Message_Block* getBlock() const {
        return mblock_;
    }
private:
    void init();
    void release();
private:
    ACE_Message_Block* mblock_;
    std::auto_ptr<MessageAdaptor> messageAdaptor_;
};


/**
 * @class BufferToMessage
 *
 * ����Ʈ ��Ʈ������ �޼��� ��ü�� ��´�
 */
class WARNET_API BufferToMessage : public boost::noncopyable
{
public:
    BufferToMessage();
    ~BufferToMessage();

    /**
     * @return Message �Ļ� ��ü. ��� �� �޸𸮸� �����ؾ� �Ѵ�
     */
    const Message* getMessage(const char* buffer,
        size_t bufferLength) const;
private:
    std::auto_ptr<MessageBuilderRepository> builderRepository_;
    std::auto_ptr<MessageAdaptor> messageAdaptor_;
    std::auto_ptr<ACE_Message_Block> mblock_;
};

} // namespace skynet

#endif // !defined(INCLUDED_FORWARDHELPER)
