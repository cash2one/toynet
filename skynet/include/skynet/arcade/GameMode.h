/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// GameMode.h
#if !defined(INCLUDED_GAMEMODE)
#define INCLUDED_GAMEMODE

#ifdef _MSC_VER
#  pragma once
#endif // _MSC_VER

#include <skynet/arcade/ArcadeDataTypes.h>
#include <boost/utility.hpp>

namespace skynet
{

/**
 * @class GameMode
 *
 * 게임 모드 베이스 클래스
 */
class WARNET_API GameMode : public boost::noncopyable
{
public:
    /// 게임 시작 준비할 수 있는가?
    bool canReadyForGame(size_t playerCount) const {
        return (getMinSlotWaiterCount() <= playerCount) &&
            (playerCount <= getMaxSlotWaiterCount());
    }

    GameModeType getGameMode() const {
        return gameMode_;
    }

    bool isSingleGameMode() const {
        return skynet::isSingleGameMode(gameMode_);
    }
protected:
    GameMode(GameModeType gm, GameType gameType) :
        gameMode_(gm),
        gameType_(gameType) {}

    GameType getGameType() const {
        return gameType_;
    }

    // = virtual
public:
    /// 게임 모드 별 오픈 슬롯 수
    virtual size_t getOpenSlotCount() const = 0;

    /// 게임 모드가 게임 타입에 부합하는가?
    virtual bool isValidGameMode(GameType gt) const = 0;

    /// 코인을 투여할 수 있는가?
    virtual bool canInsertCoin() const {
        return false;
    }

    /// 게임방 상태를 변경할 수 있는가?
    virtual bool canChangeRoomState(GameRoomState state) const {
        state;
        return true;
    }

    /// 게임방을 만들 수 있는가?
    virtual bool canCreateGameRoom(CardLevel level, Grade grade) const {
        level, grade;
        return true;
    }

    /// 게임방에 참여할 수 있는가?
    virtual bool canJoinGameRoom(bool isGameReady) const {
        return ! isGameReady;
    }

    /// 게임방에 참여할 수 있는가?
    virtual bool canJoinGameRoom(CardLevel /*level*/) const {
        return true;
    }

    /// 게임방에서 퇴장할 수 있는가?
    virtual bool canPartGameRoom(bool isGameReady,
            bool /*isPartable*/) const {
        return ! isGameReady;
    }

    /// 관전자 전환을 할 수 있는가?
    virtual bool canLeaveGameRoomSlot(bool isGameStarted) const {
        return ! isGameStarted;
    }

    /// 슬롯 대기자 전환을 할 수 있는가?
    virtual bool canEnterGameRoomSlot(bool isGameStarted) const {
        return ! isGameStarted;
    }

    /// (게임 취소 등으로) 코인 반환을 할 수 있는가?
    virtual bool canRepayCoin() const {
        return true;
    }

    /// 게임 모드 전환을 할 수 있는가?
    virtual bool canChangeGameMode(bool isGameStarted) const {
        return ! isGameStarted;
    }

    /// 게임 모드를 변경할 수 있는가?
    virtual bool canChangeGameMode(size_t slotWaiterCount) const {
        return slotWaiterCount <= getMaxSlotWaiterCount();
    }

    /// 게임 모드 전환할 수 있는가?
    virtual bool canChangeGameMode(CardLevel level, Grade grade) const {
        return canCreateGameRoom(level, grade);
    }

    /// 대전 신청을 할 수 있는가?
    virtual bool canChallenge() const {
        return false;
    }

    /// 게임 시작 전에 관전할 게임라운드를 선택해야 하는가?
    virtual bool shouldSelectGameRoundBeforeGameIsStarted() const {
        return true;
    }

    /// 랭킹 포인트를 보상할 수 있는가?
    virtual bool canRewardRankingPoint() const {
        return true;
    }

    /// 게임 전적을 저장할 수 있는가?
    virtual bool canSaveGameScoreAsHighScore(ChannelType ch) const {
        ch;
        return true;
    }

    /// 모드별 게임 전적을 저장할 수 있는가?
    virtual bool canUpdateGameScoreByGameMode() const {
        return true;
    }

    // = 경험치 계산 용
    virtual ExpPoint getWinExp(GameRoundStage stage) const = 0;
    virtual ExpPoint getLoseExp(GameRoundStage stage) const = 0;
    virtual ExpPoint getDrawExp(GameRoundStage stage) const = 0;
    virtual ExpPoint getTournamentExp(TournamentResult result,
        size_t players) const {
        result, players;
        WN_ASSERT(false && "토너먼트 모드에서만 의미 있음");
        return 0;
    }

    /// 게임 실행시 소모되는 코인 수
    virtual Coin getInitialCoin() const = 0;
    
    /// 게임에서 이겼을 경우 얻는 코인 수
    virtual Coin getEarnedCoin() const = 0;

    /// 토너먼트 경기 보상 코인
    virtual Coin getEarnedCoin(TournamentResult result, size_t players) const {
        result, players;
        WN_ASSERT(false && "토너먼트 모드에서만 의미 있음");
        return 0;
    }

    // = 루포 계산 용
    virtual Rupo getDefaultRupoAtWin(GameRoundStage /*stage*/) const {
        return 0;
    }
    virtual Rupo getDefaultRupoAtLose(GameRoundStage /*stage*/) const {
        return 0;
    }
    virtual Rupo getDefaultRupoAtDraw(GameRoundStage /*stage*/) const {
        return 0;
    }
    virtual Rupo getDefaultTournamentRupo(TournamentResult result,
        size_t players) const {
        result;players;
        WN_ASSERT(false && "토너먼트 모드에서만 의미 있음");
        return 0;
    }

    // = 랭킹 포인트 계산 용

    /// MDC*10을 얻는다
    virtual int getMdc(GameRoundStage stage) const = 0;

    /// 게임 모드 별 최소 인원수
    virtual size_t getMinSlotWaiterCount() const = 0;

    /// 게임 모드 별 최대 인원수
    virtual size_t getMaxSlotWaiterCount() const = 0;

    // = static
public:
    /// 각 게임별 기본 게임 모드를 얻는다
    static GameModeType getDefaultGameMode(GameType gt);

    /// 각 게임별 확장 게임 모드를 얻는다
    static GameModeType getExtendedGameMode(GameType gt);

    // = attributes
private:
    GameModeType gameMode_;

    GameType gameType_;
};

} // namespace skynet

#endif // !defined(INCLUDED_GAMEMODE)
