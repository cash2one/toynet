/**
* Copyright
*
* @author 
*/
// Win32Util.h : WIN32 utilities
//
#if !defined(INCLUDED_WIN32UTIL)
#define INCLUDED_WIN32UTIL

#include <ctime>

struct tagVARIANT;

namespace skynet
{

/**
 * COM library�� �ʱ�ȭ�Ѵ�.
 * - from MSDN
 */
struct WARNET_API InitOle
{
    InitOle();
    ~InitOle();
};


time_t WARNET_API variantTimeToUnixTime(const tagVARIANT& vt);

tagVARIANT WARNET_API unixTimeToVariantTime(time_t ut);

} // namespace skynet

#endif // !defined(INCLUDED_WIN32UTIL)
