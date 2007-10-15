#if !defined(INCLUDED_RUPOSUPPLIERFACTROY)
#define INCLUDED_RUPOSUPPLIERFACTROY

#include <skynet/arcade/rupo/MatchGameRupoSupplier.h>
#include <skynet/arcade/ArcadeChannelInfo.h>
namespace skynet {
 
class WARNET_API RupoSupplierFactory
{
public:
    static MatchGameRupoSupplier* create(GameType gameType,
        const GameMode& gameMode, ChannelType ch, GameGenre gg);
};


}

#endif
