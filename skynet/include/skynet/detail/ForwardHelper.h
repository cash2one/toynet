/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ForwardHelper.h : UDP 포워딩 서버 관련 도우미
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
 * Message 객체에서 바이트 스트림을 얻는다
 */
class WARNET_API MessageToBuffer : public boost::noncopyable
{
public:
    MessageToBuffer();
    MessageToBuffer(const Message& msg);
    ~MessageToBuffer();

    /// msg를 파싱하여 객체를 재초기화한다
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
 * 바이트 스트림에서 메세지 객체를 얻는다
 */
class WARNET_API BufferToMessage : public boost::noncopyable
{
public:
    BufferToMessage();
    ~BufferToMessage();

    /**
     * @return Message 파생 객체. 사용 후 메모리를 해제해야 한다
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
