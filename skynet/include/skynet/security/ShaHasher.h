/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ShaHasher.h : SHA-1 hasher
//
#if !defined(INCLUDED_SHAHASHER)
#define INCLUDED_SHAHASHER

#include <string>

namespace skynet
{

/**
 * @class ShaHasher
 *
 * SHA1 해쉬 생성기
 */
class WARNET_API ShaHasher
{
public:    
    ShaHasher();

    /** 
    * Feed additional data into the hashing engine.
    * @return false if toString() has been called and reset() has not.
    */
    bool hash(const char* data, int len = -1);
    bool hash(const std::string& data) {
        return hash(data.c_str(), static_cast<int>(data.size()));
    }

    /**
    * Finalize the data being hashed and return a hexidecimal
    * string representation of the hash. It is illegal to call 
    * ShaHasher::hash() again until after reset() is called.
    */
    std::string toString();

    /**
    * Reset the hashing engine so that a new hash can be
    * generated.
    */
    void reset();
private:    
    unsigned long H_[5];
    unsigned long W_[80];
    int lenW_;
    unsigned long sizeHi_; 
    unsigned long sizeLo_;

    bool finalized_;
}; 

} // namespace skynet

#endif // !defined(INCLUDED_SHAHASHER)
