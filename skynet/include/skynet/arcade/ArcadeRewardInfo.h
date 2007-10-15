/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// PoporuRewardInfo.h
//  - Arcade ���� ���� ����
//
#if !defined(INCLUDED_POPORUREWARDINFO)
#define INCLUDED_POPORUREWARDINFO

#include <skynet/Wdr.h>

namespace skynet
{

/// ����ġ
typedef Wdr::ULong ExpPoint;

/// ��ŷ ����Ʈ(������ �� �� �ִ�)
typedef Wdr::Long RankingPoint;

/// ��ŷ(����)
typedef Wdr::ULong Ranking;

/// ����(���ϸ���)
typedef Wdr::ULong Rupo;

/// ����(���� �̵� ��� ������ ���� ���� ���� �� �־�� �Ѵ�)
typedef Wdr::Short Coin;

typedef Wdr::ULong Cash;

/**
* @struct CoinStorage
* ���� ����� ����
* - ���� ������ �Ǵ� ĳ���� ī���� ����
*/
struct CoinStorage
{
    Coin maxCoin_; ///< ���� ������ �ִ� ���� ��
    Coin curCoin_; ///< ���� �����ִ� ���� ��

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
