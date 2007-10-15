/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// Wdr.h : WarNet Data Representation (XDR의 변형)
//
#if !defined(INCLUDED_WDR)
#define INCLUDED_WDR

#include <boost/static_assert.hpp>

namespace skynet
{

namespace Wdr
{

typedef char Char;
typedef unsigned char Octet;
typedef short Short;
typedef int Long;
typedef unsigned short UShort;
typedef unsigned int ULong;

enum {
    sizeChar = 1,
    sizeOctet = 1,
    sizeShort = 2,
    sizeLong = 4
};

BOOST_STATIC_ASSERT(sizeChar == sizeof(Char));
BOOST_STATIC_ASSERT(sizeOctet == sizeof(Octet));
BOOST_STATIC_ASSERT(sizeShort == sizeof(Short));
BOOST_STATIC_ASSERT(sizeLong == sizeof(Long));

} // namespace Wdr

} // namespace skynet

#endif // !defined(INCLUDED_WDR)
