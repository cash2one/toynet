/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// PoporuRewardInfo.h
//  - Arcade 보상 관련 정보
//
#if !defined(INCLUDED_POPORUREWARDINFO)
#define INCLUDED_POPORUREWARDINFO

#include <skynet/Wdr.h>

namespace skynet
{

/// 경험치
typedef Wdr::ULong ExpPoint;

/// 랭킹 포인트(음수가 될 수 있다)
typedef Wdr::Long RankingPoint;

/// 랭킹(순위)
typedef Wdr::ULong Ranking;

/// 루포(마일리지)
typedef Wdr::ULong Rupo;

/// 코인(코인 이동 기능 때문에 음의 값을 가질 수 있어야 한다)
typedef Wdr::Short Coin;

typedef Wdr::ULong Cash;

/**
* @struct CoinStorage
* 코인 저장소 상태
* - 코인 보관함 또는 캐릭터 카드의 상태
*/
struct CoinStorage
{
    Coin maxCoin_; ///< 보관 가능한 최대 코인 수
    Coin curCoin_; ///< 현재 남아있는 코인 수

    CoinStorage(Coin maxCoin = 0, Coin currentCoin = 0) :
        maxCoin_(maxCoin),
        curCoin_(currentCoin) {}

    void consumeCoin(Coin coin) {
        if (curCoin_ > coin) {
            curCoin_ = curCoin_ - coin;
        }
        else {
            curCoin_ = 0;
        }
    }

    void repayCoin(Coin coin) {
        curCoin_ = curCoin_ + coin;
    }

    bool hasCoin() const {
        return curCoin_ != 0;
    }
};

struct PointStorage
{
	Cash	cashPoint_;
	Cash	bonusPoint_;
	
	PointStorage(Cash cash = 0, Cash bonus = 0 ) :
		cashPoint_(cash), 
		bonusPoint_(bonus) {}

	void applyBonus(){
		cashPoint_ = bonusPoint_;
	}

	bool hasCoin() const {
		return cashPoint_ !=0;
	}
	
	bool hasBonus() const{
		return bonusPoint_ !=0;
	}

};

} // namespace skynet

#endif // !defined(INCLUDED_POPORUREWARDINFO)
