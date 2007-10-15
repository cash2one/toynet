/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
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
* Popuru_Game 전용 데이터베이스
*/
class GameDatabase : public Database
{
public:
    // = 채널 서버 정보

    /// 채널 서버 정보를 얻는다.
    virtual bool getChannelServerInfo(Channels& channels,
        GameGenre gameGenre) = 0;

    /// 채널군 정보를 얻는다
    virtual bool getChannelGroups(ChannelGroups& channelGroups) = 0;

    // = presence

    /// 사용자가 로그인하였음.
    virtual bool addPresence(UserNo userNo, ChannelId channelId) = 0;

    /// 사용자가 로그아웃하엿음
    virtual bool deletePresence(UserNo userNo, ChannelId channelId) = 0;

    /// 사용자가 로그인하였는가?
    virtual bool isLoggedIn(UserNo userNo) = 0;

    /// 해당 게임/채널에 접속중인 사용자를 지운다
    virtual bool deleteAllPresence(ChannelId channelId) = 0;

    // = 게임 전적

    /// 게임 전적을 읽어온다
    virtual bool getGameScore(DetailedGameScore& score, UserNo player,
        GameType gameType) = 0;

    /// 최고 게임 전적을 갱신한다
    virtual bool updateHighGameScore(UserNo player,
        const GameScore& score) = 0;

    /// 게임 모드 별 게임 전적을 갱신한다
    virtual bool updateHighGameScoreByGameMode(GameModeType gameMode,
        UserNo player, const DetailedGameScore& score) = 0;

    /// 연승 횟수를 갱신한다
    virtual bool updateStraitVictories(GameType gameType, UserNo player,
        size_t svCount, size_t maxSvCount) = 0;

    // = 신고

    /// 신고를 저장한다
    virtual bool saveReport(const std::string& nickname, ReportReason rr,
        const std::string& reportContents, const std::string& reportLog,
        const std::string& reporter) = 0;

    // = 공지사항

    /// 공지사항을 읽어온다
    virtual bool getNotice(std::string& notice,
        GameGenre gameGenre = GG_UNKNOWN) = 0;

    // = 사용자 옵션

    virtual bool saveOptions(UserNo owner, GameType gameType,
        const std::string& options) = 0;

    virtual bool loadOptions(std::string& options, UserNo owner,
        GameType gameType) = 0;

    // = 모니터

    virtual bool blockGameServer(ChannelId channelId) = 0;
    virtual bool resetChannelUserCount(ChannelId channelId) = 0;
    //코인 충전 (중국용)
    virtual bool coinFreeUpdate(UserNo userNo,CardNumber cardNumber,Coin& coin,                ErrorCode& errorCode) = 0;

};

} // namespace skynet

#endif // !defined(INCLUDED_GAMEDATABASE)
