/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// AesCipher.h
//
#if !defined(INCLUDED_AESCIPHER)
#define INCLUDED_AESCIPHER

#include <string>
#include <memory>

struct block_state;

namespace skynet
{

/**
 * @class AesCipher
 *
 * AES 암호화 알고리즘
 */
class WARNET_API AesCipher
{
public:
    /**
     * ctor
     * @param key 대칭키. 길이가 16, 24, 32바이트 중 하나이어야 한다
     */
    AesCipher(const std::string& key);
    ~AesCipher();

    std::string encrypt(const std::string& data);
    std::string decrypt(const std::string& data);
private:
    std::auto_ptr<block_state> blockState_;
}; 

} // namespace skynet

#endif // !defined(INCLUDED_AESCIPHER)
