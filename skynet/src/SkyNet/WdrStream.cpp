/**
* Copyright 
*
* @author 
*/
// WdrStream.cpp
#include "stdafx.h"
#include <skynet/detail/WdrStream.h>
namespace skynet
{
namespace Wdr
{

// = InputWdr

InputWdr::InputWdr(ACE_Message_Block& data) :
    block_(data),
    originalBlockRdPtr_(data.rd_ptr()),
    isGood_(true)
{
}


InputWdr::~InputWdr()
{
    block_.rd_ptr(originalBlockRdPtr_);
}


bool InputWdr::read(Octet& value)
{

    if (! isGood_)
        return false;
    if (block_.length() >= sizeOctet) {
        value = *block_.rd_ptr();
        block_.rd_ptr(sizeOctet);
    } else {
        isGood_ = false;
    }
    return isGood_;
}


bool InputWdr::read(Short& value)
{
    if (! isGood_)
        return false;

    if (block_.length() >= sizeShort) {
        void* rdPtr = block_.rd_ptr();
        value = *static_cast<Short*>(rdPtr);

        block_.rd_ptr(sizeShort);
    } else {
        isGood_ = false;
    }
    return isGood_;
}


bool InputWdr::read(Long& value)
{
    if (! isGood_)
        return false;

    if (block_.length() >= sizeLong) {
        void* rdPtr = block_.rd_ptr();
        value = *static_cast<Long*>(rdPtr);
        block_.rd_ptr(sizeLong);
    } else {
        isGood_ = false;
    }
    return isGood_;
}


bool InputWdr::read(Char* array, size_t length)
{
    if (! isGood_)
        return false;

    if (block_.length() >= length) {
        ACE_OS::memcpy(array, block_.rd_ptr(), length);
        block_.rd_ptr(length);
    } else {
        isGood_ = false;
    }

    return isGood_;

}
// = OutputWdr

//OutputWdr::OutputWdr(size_t blockSize) :

OutputWdr::OutputWdr(ACE_Message_Block& block) :
    //block_(blockSize),
    block_(block),
    isGood_(true)
{
}


bool OutputWdr::write(Octet value)
{

    if (! isGood_)
        return false;

    if (block_.space() >= sizeOctet) {
        void* wrPtr = block_.wr_ptr();
        *static_cast<Octet*>(wrPtr) = value;
        block_.wr_ptr(sizeOctet);
    } else {
        isGood_ = false;
    }

    return isGood_;
}


bool OutputWdr::write(Short value)
{
    if (! isGood_)
        return false;

    if (block_.space() >= sizeShort) {
        void* wrPtr = block_.wr_ptr();
        *static_cast<Short*>(wrPtr) = value;
        block_.wr_ptr(sizeShort);

    } else {
        isGood_ = false;
    }
    return isGood_;

}





bool OutputWdr::write(Long value)
{

    if (! isGood_)
        return false;


    if (block_.space() >= sizeLong) {
        void* wrPtr = block_.wr_ptr();
        *static_cast<Long*>(wrPtr) = value;
        block_.wr_ptr(sizeLong);
    } else {
        isGood_ = false;
    }
    return isGood_;

}

bool OutputWdr::write(const Char* array, size_t length)
{
    if (! isGood_)
        return false;

    if (block_.space() >= length) {
        isGood_ = (block_.copy(array, length) == 0);
    } else {
        isGood_ = false;
    }
    return isGood_;
}
} // namespace Wdr
} // namespace skynet


