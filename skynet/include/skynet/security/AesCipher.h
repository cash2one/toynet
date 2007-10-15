/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
 * AES ��ȣȭ �˰���
 */
class WARNET_API AesCipher
{
public:
    /**
     * ctor
     * @param key ��ĪŰ. ���̰� 16, 24, 32����Ʈ �� �ϳ��̾�� �Ѵ�
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
