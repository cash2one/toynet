/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 
*/
// SharedPtr.h
//
#if !defined(INCLUDED_SHAREDPTR)
#define INCLUDED_SHAREDPTR

#include <boost/WN_ASSERT.hpp>
#include <boost/checked_delete.hpp>
#include <boost/throw_exception.hpp>

#ifndef BOOST_NO_AUTO_PTR
# include <memory>          // for std::auto_ptr
#endif

#include <algorithm>        // for std::swap
#include <functional>       // for std::less
#include <new>              // for std::bad_alloc

namespace skynet
{   

/**
 * @class SharedPtr
 *
 * thread-unsafe smart pointer
 * - boost/detail/shared_ptr_nmt.hppø°º≠ »…√ƒø»
 */
template<class T>
class SharedPtr
{
private:
    typedef long count_type; // thread-unsafe!!!

public:

    typedef T element_type;
    typedef T value_type;

    explicit SharedPtr(T * p = 0): px(p)
    {
#ifndef BOOST_NO_EXCEPTIONS

        try  // prevent leak if new throws
        {
            pn = new count_type(1);
        }
        catch(...)
        {
            boost::checked_delete(p);
            throw;
        }

#else

        pn = new count_type(1);

        if(pn == 0)
        {
            boost::checked_delete(p);
            boost::throw_exception(std::bad_alloc());
        }

#endif
    }

    ~SharedPtr()
    {
        if(--*pn == 0)
        {
            boost::checked_delete(px);
            delete pn;
        }
    }

    SharedPtr(SharedPtr const & r): px(r.px)  // never throws
    {
        pn = r.pn;
        ++*pn;
    }

    SharedPtr & operator=(SharedPtr const & r)
    {
        SharedPtr(r).swap(*this);
        return *this;
    }

#ifndef BOOST_NO_AUTO_PTR

    explicit SharedPtr(std::auto_ptr<T> & r)
    { 
        pn = new count_type(1); // may throw
        px = r.release(); // fix: moved here to stop leak if new throws
    } 

    SharedPtr & operator=(std::auto_ptr<T> & r)
    {
        SharedPtr(r).swap(*this);
        return *this;
    }

#endif

    void reset(T * p = 0)
    {
        BOOST_ASSERT(p == 0 || p != px);
        SharedPtr(p).swap(*this);
    }

    T & operator*() const  // never throws
    {
        BOOST_ASSERT(px != 0);
        return *px;
    }

    T * operator->() const  // never throws
    {
        BOOST_ASSERT(px != 0);
        return px;
    }

    T * get() const  // never throws
    {
        return px;
    }

    long use_count() const  // never throws
    {
        return *pn;
    }

    bool unique() const  // never throws
    {
        return *pn == 1;
    }
    
    void swap(SharedPtr<T> & other)  // never throws
    {
        std::swap(px, other.px);
        std::swap(pn, other.pn);
    }

private:

    T * px;            // contained pointer
    count_type * pn;   // ptr to reference counter
};

template<class T, class U> inline bool operator==(SharedPtr<T> const & a, SharedPtr<U> const & b)
{
    return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(SharedPtr<T> const & a, SharedPtr<U> const & b)
{
    return a.get() != b.get();
}

template<class T> inline bool operator<(SharedPtr<T> const & a, SharedPtr<T> const & b)
{
    return std::less<T*>()(a.get(), b.get());
}

template<class T> void swap(SharedPtr<T> & a, SharedPtr<T> & b)
{
    a.swap(b);
}

// get_pointer() enables boost::mem_fn to recognize SharedPtr

template<class T> inline T * get_pointer(SharedPtr<T> const & p)
{
    return p.get();
}

} // namespace skynet

#endif // !defined(INCLUDED_SHAREDPTR)
