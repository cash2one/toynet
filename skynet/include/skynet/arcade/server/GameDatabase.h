/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// GameDatabase.h
#if !defined(INCLUDED_GAMEDATABASE)
#define INCLUDED_GAMEDATABASE

#include <skynet/server/Database.h>
#include <skynet/arcade/ArcadeGameScoreInfo.h>
#include <skynet/arcade/ArcadeReportInfo.h>
#include <skynet/arcade/ArcadeCharacterCardInfo.h>
#include <skynet/ErrorCode.h>
namespace skynet
{

/**
* @class GameDatabase
*
* Popuru_Game ���� �����ͺ��̽�
*/
class GameDatabase : public Database
{
public:
    // = ä�� ���� ����

    /// ä�� ���� ������ ��´�.
    virtual bool getChannelServerInfo(Channels& channels,
        GameGenre gameGenre) = 0;

    /// ä�α� ������ ��´�
    virtual bool getChannelGroups(ChannelGroups& channelGroups) = 0;

    // = presence

    /// ����ڰ� �α����Ͽ���.
    virtual bool addPresence(UserNo userNo, ChannelId channelId) = 0;

    /// ����ڰ� �α׾ƿ��Ͽ���
    virtual bool deletePresence(UserNo userNo, ChannelId channelId) = 0;

    /// ����ڰ� �α����Ͽ��°�?
    virtual bool isLoggedIn(UserNo userNo) = 0;

    /// �ش� ����/ä�ο� �������� ����ڸ� �����
    virtual bool deleteAllPresence(ChannelId channelId) = 0;

    // = ���� ����

    /// ���� ������ �о�´�
    virtual bool getGameScore(DetailedGameScore& score, UserNo player,
        GameType gameType) = 0;

    /// �ְ� ���� ������ �����Ѵ�
    virtual bool updateHighGameScore(UserNo player,
        const GameScore& score) = 0;

    /// ���� ��� �� ���� ������ �����Ѵ�
    virtual bool updateHighGameScoreByGameMode(GameModeType gameMode,
        UserNo player, const DetailedGameScore& score) = 0;

    /// ���� Ƚ���� �����Ѵ�
    virtual bool updateStraitVictories(GameType gameType, UserNo player,
        size_t svCount, size_t maxSvCount) = 0;

    // = �Ű�

    /// �Ű� �����Ѵ�
    virtual bool saveReport(const std::string& nickname, ReportReason rr,
        const std::string& reportContents, const std::string& reportLog,
        const std::string& reporter) = 0;

    // = ��������

    /// ���������� �о�´�
    virtual bool getNotice(std::string& notice,
        GameGenre gameGenre = GG_UNKNOWN) = 0;

    // = ����� �ɼ�

    virtual bool saveOptions(UserNo owner, GameType gameType,
        const std::string& options) = 0;

    virtual bool loadOptions(std::string& options, UserNo owner,
        GameType gameType) = 0;

    // = �����

    virtual bool blockGameServer(ChannelId channelId) = 0;
    virtual bool resetChannelUserCount(ChannelId channelId) = 0;
    //���� ���� (�߱���)
    virtual bool coinFreeUpdate(UserNo userNo,CardNumber cardNumber,Coin& coin,                ErrorCode& errorCode) = 0;

};

} // namespace skynet

#endif // !defined(INCLUDED_GAMEDATABASE)
