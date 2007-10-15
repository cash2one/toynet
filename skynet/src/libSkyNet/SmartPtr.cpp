/**
* Copyright (c) 
*
* @author 
*/
// SmartPtr.cpp
#include "stdafx.h"
#include <skynet/SmartPtr.h>

namespace skynet
{

void MTSmartPtrTraits::addReference(const SharedObject* rawPtr)
{
    WN_ASSERT(false && "TODO: atomic op 지원");
    if (rawPtr != 0) {
        ++rawPtr->referenceCount_;
    }
}


void MTSmartPtrTraits::removeReference(SharedObject* rawPtr)
{
    WN_ASSERT(false && "TODO: atomic op 지원");
    if (rawPtr != 0) {
        if (--rawPtr->referenceCount_ == 0) {
            delete rawPtr;
            return;
        }
    }
}


void MTSmartPtrTraits::changeReference(const SharedObject* rawPtr1,
    SharedObject* rawPtr2)
{
    WN_ASSERT(false && "TODO: atomic op 지원");
    if (rawPtr1 != 0) {
        ++rawPtr1->referenceCount_;
    }

    if (rawPtr2 != 0) {
        if (--rawPtr2->referenceCount_ == 0) {
            delete rawPtr2;
            return;
        }
    }
}

} // namespace skynet
