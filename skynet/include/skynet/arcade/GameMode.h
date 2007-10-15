/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
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
 * ���� ��� ���̽� Ŭ����
 */
class WARNET_API GameMode : public boost::noncopyable
{
public:
    /// ���� ���� �غ��� �� �ִ°�?
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
    /// ���� ��� �� ���� ���� ��
    virtual size_t getOpenSlotCount() const = 0;

    /// ���� ��尡 ���� Ÿ�Կ� �����ϴ°�?
    virtual bool isValidGameMode(GameType gt) const = 0;

    /// ������ ������ �� �ִ°�?
    virtual bool canInsertCoin() const {
        return false;
    }

    /// ���ӹ� ���¸� ������ �� �ִ°�?
    virtual bool canChangeRoomState(GameRoomState state) const {
        state;
        return true;
    }

    /// ���ӹ��� ���� �� �ִ°�?
    virtual bool canCreateGameRoom(CardLevel level, Grade grade) const {
        level, grade;
        return true;
    }

    /// ���ӹ濡 ������ �� �ִ°�?
    virtual bool canJoinGameRoom(bool isGameReady) const {
        return ! isGameReady;
    }

    /// ���ӹ濡 ������ �� �ִ°�?
    virtual bool canJoinGameRoom(CardLevel /*level*/) const {
        return true;
    }

    /// ���ӹ濡�� ������ �� �ִ°�?
    virtual bool canPartGameRoom(bool isGameReady,
            bool /*isPartable*/) const {
        return ! isGameReady;
    }

    /// ������ ��ȯ�� �� �� �ִ°�?
    virtual bool canLeaveGameRoomSlot(bool isGameStarted) const {
        return ! isGameStarted;
    }

    /// ���� ����� ��ȯ�� �� �� �ִ°�?
    virtual bool canEnterGameRoomSlot(bool isGameStarted) const {
        return ! isGameStarted;
    }

    /// (���� ��� ������) ���� ��ȯ�� �� �� �ִ°�?
    virtual bool canRepayCoin() const {
        return true;
    }

    /// ���� ��� ��ȯ�� �� �� �ִ°�?
    virtual bool canChangeGameMode(bool isGameStarted) const {
        return ! isGameStarted;
    }

    /// ���� ��带 ������ �� �ִ°�?
    virtual bool canChangeGameMode(size_t slotWaiterCount) const {
        return slotWaiterCount <= getMaxSlotWaiterCount();
    }

    /// ���� ��� ��ȯ�� �� �ִ°�?
    virtual bool canChangeGameMode(CardLevel level, Grade grade) const {
        return canCreateGameRoom(level, grade);
    }

    /// ���� ��û�� �� �� �ִ°�?
    virtual bool canChallenge() const {
        return false;
    }

    /// ���� ���� ���� ������ ���Ӷ��带 �����ؾ� �ϴ°�?
    virtual bool shouldSelectGameRoundBeforeGameIsStarted() const {
        return true;
    }

    /// ��ŷ ����Ʈ�� ������ �� �ִ°�?
    virtual bool canRewardRankingPoint() const {
        return true;
    }

    /// ���� ������ ������ �� �ִ°�?
    virtual bool canSaveGameScoreAsHighScore(ChannelType ch) const {
        ch;
        return true;
    }

    /// ��庰 ���� ������ ������ �� �ִ°�?
    virtual bool canUpdateGameScoreByGameMode() const {
        return true;
    }

    // = ����ġ ��� ��
    virtual ExpPoint getWinExp(GameRoundStage stage) const = 0;
    virtual ExpPoint getLoseExp(GameRoundStage stage) const = 0;
    virtual ExpPoint getDrawExp(GameRoundStage stage) const = 0;
    virtual ExpPoint getTournamentExp(TournamentResult result,
        size_t players) const {
        result, players;
        WN_ASSERT(false && "��ʸ�Ʈ ��忡���� �ǹ� ����");
        return 0;
    }

    /// ���� ����� �Ҹ�Ǵ� ���� ��
    virtual Coin getInitialCoin() const = 0;
    
    /// ���ӿ��� �̰��� ��� ��� ���� ��
    virtual Coin getEarnedCoin() const = 0;

    /// ��ʸ�Ʈ ��� ���� ����
    virtual Coin getEarnedCoin(TournamentResult result, size_t players) const {
        result, players;
        WN_ASSERT(false && "��ʸ�Ʈ ��忡���� �ǹ� ����");
        return 0;
    }

    // = ���� ��� ��
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
        WN_ASSERT(false && "��ʸ�Ʈ ��忡���� �ǹ� ����");
        return 0;
    }

    // = ��ŷ ����Ʈ ��� ��

    /// MDC*10�� ��´�
    virtual int getMdc(GameRoundStage stage) const = 0;

    /// ���� ��� �� �ּ� �ο���
    virtual size_t getMinSlotWaiterCount() const = 0;

    /// ���� ��� �� �ִ� �ο���
    virtual size_t getMaxSlotWaiterCount() const = 0;

    // = static
public:
    /// �� ���Ӻ� �⺻ ���� ��带 ��´�
    static GameModeType getDefaultGameMode(GameType gt);

    /// �� ���Ӻ� Ȯ�� ���� ��带 ��´�
    static GameModeType getExtendedGameMode(GameType gt);

    // = attributes
private:
    GameModeType gameMode_;

    GameType gameType_;
};

} // namespace skynet

#endif // !defined(INCLUDED_GAMEMODE)
