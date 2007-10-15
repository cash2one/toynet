/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author ±èº´¼ö <kcando@megaking.co.kr>
*/
// WdrUtil.h : WDR utilities
//
#if !defined(INCLUDED_WDRUTIL)
#define INCLUDED_WDRUTIL

#include <skynet/detail/WdrStream.h>
#include <skynet/ErrorCode.h>
#include <skynet/DataTypes.h>

// = WDR-related utility functions
namespace skynet
{

// = bool

inline bool writeWdrBool(Wdr::OutputWdr& wdr, bool input)
{
    return wdr.write(static_cast<Wdr::Octet>(input ? 1 : 0));
}

inline bool readWdrBool(Wdr::InputWdr& wdr, bool& output)
{
    Wdr::Octet value;
    if (! wdr.read(value))
        return false;
    output = (value != 0);
    return true;
}

inline size_t getWdrBoolSize()
{
    return Wdr::sizeOctet;
}

// = string

bool WARNET_API writeWdrString(Wdr::OutputWdr& wdr, const std::string& str);
bool WARNET_API readWdrString(Wdr::InputWdr& wdr, std::string& str,
    size_t maxStrLen);
size_t WARNET_API getWdrStringSize(const std::string& str);

// = StringList

bool WARNET_API readWdrStringList(Wdr::InputWdr& wdr, StringList& strings,
    size_t maxStrLen);
bool WARNET_API writeWdrStringList(Wdr::OutputWdr& wdr,
    const StringList& strings);
size_t WARNET_API getWdrStringListSize(const StringList& strings);

} // namespace skynet

#endif // !defined(INCLUDED_WDRUTIL)
