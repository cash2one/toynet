/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// PoporuServer.h
#if !defined(INCLUDED_POPORUSERVER)
#define INCLUDED_POPORUSERVER

#include <skynet/server/WarNetService.h>

namespace skynet
{

/**
 * @class PoporuServer
 *
 * 포포루 서버
 */
class WARNET_API PoporuServer : public WarNetService
{
public:
    PoporuServer();
protected:
    virtual bool onParseConfigFile(ACE_Configuration_Heap& config);

    virtual size_t getAuthenticateTimeout() const {
        return authenticateTimeout_;
    }

    const std::string getCipherKey() const {
        return cipherKey_;
    }
private:
    bool parseClientSection(ACE_Configuration_Heap& config);
    bool parseSecuritySection(ACE_Configuration_Heap& config);
private:
    size_t authenticateTimeout_;
    std::string cipherKey_;
};

} // namespace skynet

#endif // !defined(INCLUDED_POPORUSERVER)
