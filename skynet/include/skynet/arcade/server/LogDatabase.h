/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// LogDatabase.h
#if !defined(INCLUDED_LOGDATABASE)
#define INCLUDED_LOGDATABASE

#include <skynet/server/Database.h>
#include <skynet/arcade/ArcadeGameRoomInfo.h>
#include <skynet/arcade/ArcadeChannelInfo.h>
#include <skynet/arcade/ArcadeCharacterCardInfo.h>

namespace skynet
{

/**
* @class LogDatabase
*
* Popuru_Log 전용 데이터베이스
*/
class LogDatabase : public Database
{
public:
    // = 개인 로그인 기록
    virtual bool recordLoginLog(UserNo userNo, const std::string& address,
        GameGenre gameGenre, ChannelType channelType, bool fromPcRoom) = 0;

    virtual bool recordLogoutLog(UserNo userNo, GameGenre gameGenre,
        ChannelType channelType, CardNumber cardNo,
        Coin currentCoin, Coin earnedCoin, Coin consumedCoin,
        unsigned int currentExp, unsigned int earnedExp,
        Rupo earnedRupo) = 0;

    // = 게임 결과

    /// 대전 게임 결과를 기록한다
    virtual bool saveMatchGameRecord(GameType gameType, UserNo playerA,
        UserNo playerB, UserNo winner, ChannelType channelType,
        GameModeType gameMode) = 0;

    /// 협력 게임 결과를 기록한다
    virtual bool saveCooperativeGameRecord(GameType gameType, UserNo player,
        UserNo partner, unsigned int score, unsigned char stage,
        ChannelType channelType, GameModeType gameMode) = 0;

    /// 대전 게임 결과에서 오늘 동안 사용자들 간의 대전 수를 얻는다
    virtual bool getMatchGameTodayMatchCount(int& count, GameType gameType,
        UserNo playerA, UserNo playerB) = 0;
};

} // namespace skynet

#endif // !defined(INCLUDED_LOGDATABASE)
