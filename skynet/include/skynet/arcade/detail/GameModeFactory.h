/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
 * GameMode ������ ȣ��� ���� ��尡 �߸��� ��� �߻���
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
 * *GameMode ��ü ���� ���丮
 */

struct WARNET_API GameModeFactory
{
    static GameMode* create(GameType gameType, GameModeType gm, 
        RoomType roomType = RT_NORMAL);
};

} // namespace skynet

#endif // !defined(INCLUDED_GAMEMODEFACTORY)
