/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// GameModeFactory.h
#if !defined(INCLUDED_GAMEMODEFACTORY)
#define INCLUDED_GAMEMODEFACTORY

#include "../ArcadeGameRoomInfo.h"
#include "../GameMode.h"
#include <boost/utility.hpp>
#include <stdexcept>

namespace skynet
{

/**
 * @class InvalidGameModeException
 *
 * GameMode 생성자 호출시 게임 모드가 잘못된 경우 발생함
 */
class InvalidGameModeException : public std::logic_error
{
public:
    InvalidGameModeException() :
        std::logic_error("") {}
};


/**
 * @struct GameModeFactory
 *
 * *GameMode 객체 생성 팩토리
 */

struct WARNET_API GameModeFactory
{
    static GameMode* create(GameType gameType, GameModeType gm, 
        RoomType roomType = RT_NORMAL);
};

} // namespace skynet

#endif // !defined(INCLUDED_GAMEMODEFACTORY)
