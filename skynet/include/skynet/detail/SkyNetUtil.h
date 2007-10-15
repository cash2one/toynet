/**
* Copyright 
*
* @author 
*/
// SkyNetUtil.h : WIN32 utilities
//
#if !defined(INCLUDED_WARNETUTIL)
#define INCLUDED_WARNETUTIL

#include <string>

namespace skynet
{

/**
 * @class ValueGuard
 *
 */
template <typename T>
class ValueGuard
{
public:
    ValueGuard(T& value, T firstValue, T lastValue) :
            value_(value),
            lastValue_(lastValue) {
        value_ = firstValue;
    }

    ~ValueGuard() {
        value_ = lastValue_;
    }
private:
    T& value_;
    const T lastValue_;
};


inline const std::string& chop(std::string& str, size_t maxLen)
{
    if (str.size() > maxLen)
        str.resize(maxLen);
    return str;
}


/// È¦¼öÀÎ°¡?
template <typename T>
bool isOdd(T value)
{
    return (value & 1) != 0;
}


/// Â¦¼öÀÎ°¡?
template <typename T>
bool isEven(T value)
{
    return ! isOdd(value);
}


template <class T, class U>
T safe_cast(U obj)
{
#ifndef NDEBUG
    return dynamic_cast<T>(obj);
#else
    return static_cast<T>(obj);
#endif
}


} // namespace skynet

#endif // !defined(INCLUDED_WARNETUTIL)
