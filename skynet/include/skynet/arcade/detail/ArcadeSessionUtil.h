/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author ±èº´¼ö <kcando@megaking.co.kr>
*/
// PoporuSessionUtil.h
//
#if !defined(INCLUDED_POPORUSESSIONUTIL)
#define INCLUDED_POPORUSESSIONUTIL

namespace skynet
{

class WarNetSession;

void WARNET_API setLoginServerBuilders(WarNetSession& session);
void WARNET_API setGameServerBuilders(WarNetSession& session);
void WARNET_API setRelayServerBuilders(WarNetSession& session);
void WARNET_API setMessengerBuilders(WarNetSession& session);
void WARNET_API setMonitorServerBuilders(WarNetSession& session);

} // namespace skynet

#endif // !defined(INCLUDED_POPORUSESSIONUTIL)
