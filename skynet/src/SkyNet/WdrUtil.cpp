/**
* Copyright (c) 
*
* @author 
*/
// WdrUtil.cpp
#include "stdafx.h"
#include <skynet/detail/WdrUtil.h>

namespace skynet
{

bool writeWdrString(Wdr::OutputWdr& wdr, const std::string& str)
{
    return wdr.write(static_cast<Wdr::UShort>(str.length())) &&
        wdr.write(str.data(), str.length());
}


bool readWdrString(Wdr::InputWdr& wdr, std::string& str, size_t maxStrLen)
{
    Wdr::UShort strLen;
    if (! wdr.read(strLen))
        return false;
    if (static_cast<size_t>(strLen) > maxStrLen)
        return false;
    str.resize(strLen, 0);
    return wdr.read(&(str[0]), strLen);
}


size_t getWdrStringSize(const std::string& str)
{
    return Wdr::sizeShort + str.length();
}


bool readWdrStringList(Wdr::InputWdr& wdr, StringList& strings,
    size_t maxStrLen)
{
    strings.clear();

    Wdr::UShort stringCount;
    if (! wdr.read(stringCount))
        return false;

    strings.reserve(stringCount);

    for (Wdr::UShort i = 0; i < stringCount; ++i) {
        std::string str;
        if (! readWdrString(wdr, str, maxStrLen))
            return false;
        strings.push_back(str);
    }
    return wdr.isGood();
}


bool writeWdrStringList(Wdr::OutputWdr& wdr, const StringList& strings)
{
    if (! wdr.write(static_cast<Wdr::UShort>(strings.size())))
        return false;

    for (StringList::const_iterator pos = strings.begin();
        pos != strings.end(); ++pos) {
            if (! writeWdrString(wdr, *pos))
                return false;
        }
        return wdr.isGood();
}


size_t getWdrStringListSize(const StringList& strings)
{
    size_t size = Wdr::sizeShort;

    for (StringList::const_iterator pos = strings.begin();
        pos != strings.end(); ++pos) {
            size += getWdrStringSize(*pos);
        }
        return size;
}

} // namespace skynet
