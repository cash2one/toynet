/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ClientAcceptor.h
#if !defined(INCLUDED_CLIENTACCEPTOR)
#define INCLUDED_CLIENTACCEPTOR

#include <skynet/server/GameClientSession.h>
#include <skynet/server/GameClientSessionPool.h>

#pragma warning ( push )
#pragma warning ( disable: 4127 4244 4251 4267 4311 4312 )
#include <ace/Asynch_Acceptor.h>
#pragma warning ( pop )

namespace skynet
{

class MessageBuilderRepository;

/**
 * @class ClientAcceptor
 *
 * 클라이언트로부터의 연결을 제어한다.
 * - AIO Acceptor.
 */
class WARNET_API ClientAcceptor : public ACE_Asynch_Acceptor<GameClientSession>
{
public:
    ClientAcceptor(WarNetService& service,
        MessageBuilderRepository& builderRepository, bool useCipher);
    virtual ~ClientAcceptor();

    void close();

    void release(GameClientSession* session);
protected:
    //virtual int validate_connection(const ACE_Asynch_Accept::Result& result,
    //    const ACE_INET_Addr &remote, const ACE_INET_Addr& local);
    virtual GameClientSession* make_handler();
private:
    GameClientSessionPool pool_;
};

} // namespace skynet

#endif // !defined(INCLUDED_CLIENTACCEPTOR)