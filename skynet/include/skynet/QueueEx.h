/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// QueueEx.h
#if !defined(INCLUDED_QUEUEEX)
#define INCLUDED_QUEUEEX

#include <boost/utility.hpp>
#include <vector>
#include <skynet/detail/Assert.h>

#ifdef _MSC_VER
#pragma warning ( push )
// C++ Exception Specification ignored
#pragma warning( disable : 4290 )
#endif

namespace skynet
{

/**
* @class QueueEx
*
* Extended thread-unsafe QueueEx.
*
* ���� �ڷ�: http://www.samblackburn.com/wfc/index.html
*
* - ���� �ٸ� �����尡 ���ÿ� enqueue/dequeue�� �����Ѵ�.
* - ������ ������ highWaterMark�̻��� ��� runtime_error �߻�.
*/
template <typename T, size_t highWaterMark>
class QueueEx : public boost::noncopyable
{
public:
    typedef T Item;
    typedef Item Items[highWaterMark];
    typedef int IndexType;

    /// ctor
    QueueEx() :
        iAdd_(0),
        iGet_(0) {
        WN_ASSERT(highWaterMark > 0);

        class_invariant();
    }

    /// dtor
    ~QueueEx() {
    }

public:
    /**
    * Enqueue.
    * - ������(reentrance)�� �ϸ� �ȵȴ�.
    */
    void add(const Item& item) throw (std::overflow_error) {
        WN_ASSERT(item != 0);

        container_[iAdd_] = item;
        IndexType new_add_index = advance_i(iAdd_);
        if (new_add_index == iGet_) { // full ?
            throw std::overflow_error("queue overflow");
        }
        iAdd_ = new_add_index;
        class_invariant();
    }

    /**
    * Dequeue.
    * - ������(reentrance)�� �ϸ� �ȵȴ�.
    */
    bool get(Item& item) {
        bool exist = false;
        if (iAdd_ != iGet_) {
            item = container_[iGet_];
            iGet_ = advance_i(iGet_);
            exist = true;
        }
        class_invariant();
        return exist;
    }

    /**
    * �׽�Ʈ �뵵�θ� ���Ǿ�� �Ѵ�.
    */
    size_t size_do_not_call_me() const {
        return size_i();
    }

protected:
    size_t size_i() const {
        return (iAdd_ >= iGet_) ?
            (iAdd_ - iGet_) : ((iAdd_ + highWaterMark) - iGet_);
    }

    IndexType advance_i(IndexType index) {
        return (index + 1) % highWaterMark;
        //same as return (index + 1) >= highWaterMark ? 0 : (index + 1);
    }

    /// ��� ������ ����.
    void clear_i() {
        iAdd_ = 0;
        iGet_ = 0;
    }

    /// Class Invariant (Design By Contract)
    void class_invariant() {
        WN_ASSERT(size_i() <= highWaterMark);
    }

    // Attributes
private:
    volatile IndexType iAdd_;
    volatile IndexType iGet_;

    /// container.
    Items container_;
};

#ifdef _MSC_VER
#  pragma warning ( pop )
#endif

} // namespace skynet

#endif // !defined(INCLUDED_QUEUEEX)
