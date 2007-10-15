/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
* Popuru_Log ���� �����ͺ��̽�
*/
class LogDatabase : public Database
{
public:
    // = ���� �α��� ���
    virtual bool recordLoginLog(UserNo userNo, const std::string& address,
        GameGenre gameGenre, ChannelType channelType, bool fromPcRoom) = 0;

    virtual bool recordLogoutLog(UserNo userNo, GameGenre gameGenre,
        ChannelType channelType, CardNumber cardNo,
        Coin currentCoin, Coin earnedCoin, Coin consumedCoin,
        unsigned int currentExp, unsigned int earnedExp,
        Rupo earnedRupo) = 0;

    // = ���� ���

    /// ���� ���� ����� ����Ѵ�
    virtual bool saveMatchGameRecord(GameType gameType, UserNo playerA,
        UserNo playerB, UserNo winner, ChannelType channelType,
        GameModeType gameMode) = 0;

    /// ���� ���� ����� ����Ѵ�
    virtual bool saveCooperativeGameRecord(GameType gameType, UserNo player,
        UserNo partner, unsigned int score, unsigned char stage,
        ChannelType channelType, GameModeType gameMode) = 0;

    /// ���� ���� ������� ���� ���� ����ڵ� ���� ���� ���� ��´�
    virtual bool getMatchGameTodayMatchCount(int& count, GameType gameType,
        UserNo playerA, UserNo playerB) = 0;
};

} // namespace skynet

#endif // !defined(INCLUDED_LOGDATABASE)
