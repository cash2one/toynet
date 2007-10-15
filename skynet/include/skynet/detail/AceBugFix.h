/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// AceBugFix.h : ACE bug fix
//  - ace static library를 사용하여 DLL을 만들 경우 아래 정의된 함수를 링크하지
//    못하는 에러가 발생하는 문제를 수정
#if !defined(INCLUDED_ACEBUGFIX)
#define INCLUDED_ACEBUGFIX

#pragma warning ( push )
#pragma warning ( disable: 4244 4312 4267 4311 )
#include <ace/OS.h>
#include <ace/Version.h>
#pragma warning ( pop )

#if defined(ACE_AS_STATIC_LIBS)

#if (ACE_MAJOR_VERSION == 5) && (ACE_MINOR_VERSION == 4) && \
    (ACE_BETA_VERSION == 2)

#if !defined(NDEBUG)

ACE_INLINE size_t
ACE_OS::strlen (const ACE_WCHAR_T *s)
{
# if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSLEN)
    return ACE_OS::wcslen_emulation (s);
# else  /* !ACE_HAS_WCHAR || ACE_LACKS_WCSLEN */
    return ::wcslen (s);
# endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSLEN */
}

ACE_INLINE ACE_WCHAR_T *
ACE_OS::strnchr (ACE_WCHAR_T *s, ACE_WINT_T c, size_t len)
{
    return
        const_cast<ACE_WCHAR_T *> (ACE_OS::strnchr (
        static_cast<const ACE_WCHAR_T *> (s),
        c,
        len));
}

ACE_INLINE ACE_WCHAR_T *
ACE_OS::strnstr (ACE_WCHAR_T *s, const ACE_WCHAR_T *t, size_t len)
{
    return
        const_cast<ACE_WCHAR_T *> (ACE_OS::strnstr (
        static_cast<const ACE_WCHAR_T *> (s),
        t,
        len));
}

ACE_INLINE ACE_WCHAR_T *
ACE_OS::strncat (ACE_WCHAR_T *s, const ACE_WCHAR_T *t, size_t len)
{
#  if !defined (ACE_HAS_WCHAR) || defined (ACE_LACKS_WCSNCAT)
    return ACE_OS::wcsncat_emulation (s, t, len);
#  else /* !ACE_HAS_WCHAR || ACE_LACKS_WCSNCAT */
    return ::wcsncat (s, t, len);
#  endif /* !ACE_HAS_WCHAR || ACE_LACKS_WCSNCAT */
}

#endif // defined(NDEBUG)

ACE_INLINE const ACE_WCHAR_T *
ACE_OS::strnchr (const ACE_WCHAR_T *s, ACE_WINT_T c, size_t len)
{
    for (size_t i = 0; i < len; ++i)
        if (s[i] == static_cast<ACE_WCHAR_T> (c))
            return s + i;

    return 0;
}

ACE_INLINE const ACE_WCHAR_T *
ACE_OS::strnstr (const ACE_WCHAR_T *s1, const ACE_WCHAR_T *s2, size_t len2)
{
    // Substring length
    const size_t len1 = ACE_OS::strlen (s1);

    // Check if the substring is longer than the string being searched.
    if (len2 > len1)
        return 0;

    // Go upto <len>
    const size_t len = len1 - len2;

    for (size_t i = 0; i <= len; i++)
    {
        if (ACE_OS::memcmp (s1 + i, s2, len2 * sizeof (ACE_WCHAR_T)) == 0)
            // Found a match!  Return the index.
            return s1 + i;
    }

    return 0;
}

ACE_INLINE ACE_WCHAR_T *
ACE_OS::strsncpy (ACE_WCHAR_T *dst, const ACE_WCHAR_T *src, size_t maxlen)
{
    register ACE_WCHAR_T *rdst = dst;
    register const ACE_WCHAR_T *rsrc = src;
    register size_t rmaxlen = maxlen;

    if (rmaxlen > 0)
    {
        if (rdst!=rsrc)
        {
            *rdst = ACE_TEXT_WIDE ('\0');
            if (rsrc != 0)
                strncat (rdst, rsrc, --rmaxlen);
        }
        else
        {
            rdst += (rmaxlen - 1);
            *rdst = ACE_TEXT_WIDE ('\0');
        }
    }
    return dst;
}

#endif // (ACE_MAJOR_VERSION == 5) && (ACE_MINOR_VERSION == 4) && \
    // (ACE_BETA_VERSION == 2)

#endif // defined(ACE_AS_STATIC_LIBS)

#endif // !defined(INCLUDED_ACEBUGFIX)
