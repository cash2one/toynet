/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// WdrStream.h : WarNet Data Representation (XDR의 변형)
//
#if !defined(INCLUDED_WDRSTREAM)
#define INCLUDED_WDRSTREAM

#include <skynet/Wdr.h>
#include <ace/Message_Block.h>

namespace skynet
{

namespace Wdr
{

/**
 * @class InputWdr
 *
 * 입력용 Wdr
 */
class WARNET_API InputWdr
{
public:
    InputWdr(ACE_Message_Block& data);
    ~InputWdr();

    bool read(Octet& value);
    bool read(Short& value);
    bool read(UShort& value) {
        Short sValue;
        if (! read(sValue))
            return false;
        value = static_cast<UShort>(sValue);
        return true;
    }
    bool read(Long& value);
    bool read(ULong& value) {
        Long lValue;
        if (! read(lValue))
            return false;
        value = static_cast<ULong>(lValue);
        return true;
    }
    bool read(Char* array, size_t length);

    bool isGood() const {
        return isGood_;
    }

    char* current() {
        return block_.rd_ptr();
    }
    char* end() {
        return block_.wr_ptr();
    }
    size_t length() const {
        return block_.length();
    }
private:
    ACE_Message_Block& block_; ///< 효율적인 처리를 위해 레퍼런스를 사용
    char* originalBlockRdPtr_; ///< rd_ptr 복원 용
    bool isGood_;
};


/**
* @class OutputWdr
*
* 출력용 Wdr
*/
class WARNET_API OutputWdr
{
public:
    //OutputWdr(size_t blockSize);
    OutputWdr(ACE_Message_Block& block);

    bool write(Octet value);
    bool write(Short value);
    bool write(UShort value) {
        return write(static_cast<Short>(value));
    }
    bool write(Long value);
    bool write(ULong value) {
        return write(static_cast<Long>(value));
    }
    bool write(const Char* array, size_t length);

    bool isGood() const {
        return isGood_;
    }

    ACE_Message_Block& current() {
        return block_;
    }
    size_t length() const {
        return block_.length();
    }
    size_t size() const {
        return block_.size();
    }
private:
    ACE_Message_Block& block_;
    bool isGood_;
};

} // namespace Wdr

} // namespace skynet

#endif // !defined(INCLUDED_WDRSTREAM)
