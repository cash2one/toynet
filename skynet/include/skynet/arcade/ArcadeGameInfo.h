/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// ArcadeGameInfo.h
//  - Arcade ���� ����
//
#if !defined(INCLUDED_POPORUGAMEINFO)
#define INCLUDED_POPORUGAMEINFO

#include "../DataTypes.h"
#include "ArcadeDataLimits.h"
#include <skynet/Wdr.h>
#include <skynet/detail/Assert.h>

namespace skynet
{

/// ���� ����
enum GameType
{
    GT_UNKNOWN = 0,
    GT_GLOBAL_OPTIONS = GT_UNKNOWN, ///< ��ü ���� �ɼ�
    GT_START = 1,

    /*1*/GT_KOF98 = GT_START, ///< King Of Fighter 98
    /*2*/GT_METALSLUG2, ///< Metal Slug 2
    /*3*/GT_SAMURAISHODOWN4, ///< Samurai Shodown 4
    /*4*/GT_TOPHUNTER, ///< Top Hunter
    /*5*/GT_NEOGEOCUP98, ///< NeoGeo Cup 98
    /*6*/GT_BASEBALLSTARS2, ///< BaseBall Stars 2
    /*7*/GT_KOF99, /// King Of Fighter 99
    /*8*/GT_TWINKLESTARSPRITES, /// Twinkle Star Sprites
    /*9*/GT_KOF2002, /// King Of Fighter 2002
    /*10*/GT_SAMURAISHODOWN5, ///< Samurai Shodown 5
    /*11*/GT_METALSLUG1, ///< Metal Slug 1
    /*12*/GT_METALSLUG3, ///< Metal Slug 3
    /*13*/GT_METALSLUGX, ///< Metal Slug X
    /*14*/GT_STREETHOOP, ///< Street Hoop
    /*15*/GT_JOYJOYKID, ///< Joy Joy Kid (Puzzled)
    /*16*/GT_LASTBLADE2, ///< Last Blade 2
    /*17*/GT_LEAGUEBOWLING, ///< League Bowling
    /*18*/GT_KOF2000, /// King Of Fighter 2000
    /*19*/GT_KOF2003, /// King Of Fighter 2003
    /*20*/GT_MARKOFWOLFS, /// FINAL FREE 4
    /*21*/GT_METALSLUG4, //Metal Slug4

    GT_LAST_BOUNDARY
};

inline GameType toGameType(int gt)
{
    return static_cast<GameType>(gt);
}

inline bool isValidGameType(GameType gt)
{
    return (GT_UNKNOWN < gt) && (gt < GT_LAST_BOUNDARY);
}

inline bool isKofSeries(GameType gt)
{
    return (GT_KOF98 == gt) || (GT_KOF99 == gt) || (GT_KOF2000 == gt) ||
        (GT_KOF2002 == gt) || (GT_KOF2003 == gt) || (GT_MARKOFWOLFS == gt);
}

inline bool isSamuraiShodownSeries(GameType gt)
{
    return (GT_SAMURAISHODOWN4 == gt) || (GT_SAMURAISHODOWN5 == gt) ||
        (GT_LASTBLADE2 == gt);
}

inline bool isMetalSlugSeries(GameType gt)
{
    return (GT_METALSLUG1 == gt) || (GT_METALSLUG2 == gt) ||
        (GT_METALSLUG3 == gt) || (GT_METALSLUGX == gt) ||
        (GT_METALSLUG4 == gt);;
}

inline bool isActionPuzzleGame(GameType gt)
{
    return (GT_TWINKLESTARSPRITES == gt) || (GT_JOYJOYKID == gt);
}

inline bool isSportsGame(GameType gt)
{
    return (GT_NEOGEOCUP98 == gt) || (GT_BASEBALLSTARS2 == gt) ||
        (GT_STREETHOOP == gt) || (GT_LEAGUEBOWLING == gt);
}

/// 1:1 ���� �����ΰ�? (���� ����� ��������)
inline bool isMatchGame(GameType gt)
{
    return isKofSeries(gt) || isSamuraiShodownSeries(gt) ||
        isActionPuzzleGame(gt) || isSportsGame(gt);
}

/// ���� �����ΰ�? (���� ����� ��������)
inline bool isCooperativeGame(GameType gt)
{
    return isMetalSlugSeries(gt) ||
        (GT_TOPHUNTER == gt);
}

inline const char* toString(GameType gameType)
{
    switch (gameType) {
    case GT_KOF98:
        return "Kof98";
    case GT_KOF99:
        return "Kof99";
    case GT_KOF2000:
        return "Kof2000";
    case GT_KOF2002:
        return "Kof2002";
    case GT_KOF2003:
        return "Kof2003";
    case GT_SAMURAISHODOWN4:
        return "SamuraiShodown4";
    case GT_SAMURAISHODOWN5:
        return "SamuraiShodown5";
    case GT_NEOGEOCUP98:
        return "NeoGeoCup98";
    case GT_STREETHOOP:
        return "StreetHoop";
    case GT_BASEBALLSTARS2:
        return "BaseballStars2";
    case GT_METALSLUG1:
        return "MetalSlug1";
    case GT_METALSLUG2:
        return "MetalSlug2";
    case GT_METALSLUG3:
        return "MetalSlug3";
    case GT_METALSLUGX:
        return "MetalSlugX";
    case GT_TOPHUNTER:
        return "TopHunter";
    case GT_TWINKLESTARSPRITES:
        return "TwinkleStarSprites";
    case GT_JOYJOYKID:
        return "JoyJoyKid";
    case GT_LASTBLADE2:
        return "LastBlade2";
    case GT_LEAGUEBOWLING:
        return "LeagueBowling";
    case GT_MARKOFWOLFS:
        return "MarkOfWolfs";
    case GT_METALSLUG4:
        return "MetalSlug4";
    }
    WN_ASSERT(false);
    return "";
}


/// ���� �帣 (���� ���� ����)
enum GameGenre
{
    GG_UNKNOWN = 0,
    GG_START = 1,

    /*1*/GG_KOF_SERIES = GG_START, ///< ŷ���� �ø���
    /*2*/GG_SAMURAI_SERIES, ///< �繫���� �ø���
    /*3*/GG_COOPERATIVE, ///< ���� ����
    /*4*/GG_SPORTS, ///< ������ ����
    /*5*/GG_ACTION_PUZZLE, ///< �׼� ���� ����
	GG_PADDING = 100,

    GG_BOUNDARY
};

inline bool isValidGameGenre(GameGenre gg, GameType gt = GT_UNKNOWN)
{
    if (isValidGameType(gt)) {
        switch (gg) {
        case GG_KOF_SERIES:
            return isKofSeries(gt);
        case GG_SAMURAI_SERIES:
            return isSamuraiShodownSeries(gt);
        case GG_SPORTS:
            return isSportsGame(gt);
        case GG_COOPERATIVE:
            return isCooperativeGame(gt);
        case GG_ACTION_PUZZLE:
            return isActionPuzzleGame(gt);
        }
        WN_ASSERT(false && "�߸��� ���� �帣");
        return false;
    }

    return (GG_UNKNOWN < gg) && (gg < GG_BOUNDARY);
}

inline GameGenre extractGameGenre(GameType gt)
{
    for (int i = GG_START; i < GG_BOUNDARY; ++i) {
        GameGenre gg = static_cast<GameGenre>(i);
        if (isValidGameGenre(gg, gt)) {
            return gg;
        }
    }
    return GG_UNKNOWN;
}


/// ���� ���
enum GameModeType
{
    GM_UNKNOWN = 0, ///< ���� �˻��

    /*1*/ GM_SINGLE, ///< �̱� ��� (1�� �÷���)

    /*2*/ GM_DUAL_MATCH, ///< 1:1 ���� (2�� �÷���)
    /*3*/ GM_DUAL_MATCH_TEAM, ///< 1:1 �� ��Ʋ (2�� �÷���)

    /*4*/ GM_ONE_COIN, ///< 1���� (1/2�� �÷���)
    /*5*/ GM_CONTINUE, ///< ��Ƽ�� (2�� �÷���)

    /*6*/ GM_THREE_INNING, ///< 3�̴� (2�� �÷���)
    /*7*/ GM_NINE_INNING,///< 3�̴� (2�� �÷���)

    /*8*/ GM_TOURNAMENT, ///< ��ʸ�Ʈ
    /*9*/ GM_LEAGUE, ///< ����

    /*10*/ GM_DUAL_MATCH_DOUBLE, ///< 1:1 ���Ǿ�� (2�� �÷���)

    /*11*/ GM_DUAL_MATCH_HALF, ///< �׿��������� 1:1 ����(������)(2�� �÷���)
    /*12*/ GM_DUAL_MATCH_FULL, ///< �׿��������� 1:1 Ǯ(���Ĺ���)(2�� �÷���)

    /*13*/ GM_RANDOM_DUAL_MATCH, ///< ���� ���� ��� (8�� �÷���)

    /*14*/ GM_THREE_ROUND, ///< �׼� ���� 3����
    /*15*/ GM_FIVE_ROUND, ///< �׼� ���� 5����

    /*16*/ GM_FIVE_STAGE, ///< ��������Ű�� 5��������(�⺻ ���)

    /*17*/ GM_REGULATION, ///< ���׺��� ���ַ� ����

    //GM_TEAM_MATCH, ///< �� ��ġ
    //GM_TEAM_SURVIVAL, ///< �������̹�
    //GM_CHAMPION, ///< è�Ǿ� ���

    GM_BOUNDARY
};

inline bool isValidGameMode(GameModeType gm)
{
    return (GM_UNKNOWN < gm) && (gm <= GM_BOUNDARY);
}

inline bool isSingleGameMode(GameModeType gm)
{
    return GM_SINGLE == gm;
}

/// 1:1 ���� ���� ����ΰ�?
inline bool isMatchGame(GameModeType gm)
{
    return (GM_SINGLE == gm) || (GM_DUAL_MATCH == gm) ||
        (GM_DUAL_MATCH_TEAM == gm) || (GM_THREE_INNING == gm) ||
        (GM_NINE_INNING == gm) || (GM_DUAL_MATCH_DOUBLE == gm) ||
        (GM_DUAL_MATCH_HALF == gm) || (GM_DUAL_MATCH_FULL == gm) ||
        (GM_THREE_ROUND == gm) || (GM_FIVE_ROUND == gm) ||
        (GM_RANDOM_DUAL_MATCH == gm) || (GM_FIVE_STAGE == gm) ||
        (GM_RANDOM_DUAL_MATCH == gm) || (GM_FIVE_STAGE == gm) ||
        (GM_REGULATION == gm) || (GM_TOURNAMENT == gm);
}

// ���� ���� ����ΰ�?
inline bool isCooperativeGame(GameModeType gm)
{
    return (GM_ONE_COIN == gm) || (GM_CONTINUE == gm);
}


/// ���� ���
enum GameClass
{
    GC_UNKNOWN = 255,

    GC_START = 0,
    GC_MASTER = GC_START, ///< ����
    GC_HIGHER_GRADER1, ///< �ʰ��1
    GC_HIGHER_GRADER2, ///< �ʰ��2
    GC_HIGHER_GRADER3, ///< �ʰ��3
    GC_HIGH_GRADER1 = 4, ///< ���1
    GC_HIGH_GRADER2, ///< ���2
    GC_HIGH_GRADER3, ///< ���3
    GC_HIGH_GRADER4, ///< ���4
    GC_HIGH_GRADER5, ///< ���5
    GC_MIDDLE_HIGH_GRADER1 = 9, ///< �߰��1
    GC_MIDDLE_HIGH_GRADER2, ///< �߰��2
    GC_MIDDLE_HIGH_GRADER3, ///< �߰��3
    GC_MIDDLE_HIGH_GRADER4, ///< �߰��4
    GC_MIDDLE_HIGH_GRADER5, ///< �߰��5
    GC_MIDDLE_GRADER1 = 14, ///< �߼�1
    GC_MIDDLE_GRADER2, ///< �߼�2
    GC_MIDDLE_GRADER3, ///< �߼�3
    GC_MIDDLE_GRADER4, ///< �߼�4
    GC_LOW_GRADER1 = 18, ///< �ϼ�1
    GC_LOW_GRADER2, ///< �ϼ�2
    GC_LOW_GRADER3, ///< �ϼ�3
    GC_BEGINNER1 = 21, ///< �ʺ�1
    GC_BEGINNER2, ///< �ʺ�2
    GC_BEGINNER3, ///< �ʺ�3

    GC_BOUNDARY
};

inline bool isValidGameClass(GameClass gameClass)
{
    return (GC_START <= gameClass) && (gameClass < GC_BOUNDARY);
}

inline GameClass toGameClass(int gameClass)
{
    return static_cast<GameClass>(gameClass);
}


/**
 * ��ʸ�Ʈ ���
 * - trChampionship�� ������ ������ ���������� ��ʸ�Ʈ ��忡���� �ǹ̰� �ִ�
 */
enum TournamentResult
{
    trUnknown = 0,
    trQuarterFinal, ///< 8�� ����(8�� �й�)
    trSemiFinal, ///< 4�� ����(4�� �й�)
    trFinal, ///< ��� ����(�ؿ��)
    trChampionship, ///< ���
    trBoundary
};

inline bool isValidTournamentResult(TournamentResult stage)
{
    return (trUnknown < stage) && (stage < trBoundary);
}

} // namespace skynet

#endif // !defined(INCLUDED_POPORUGAMEINFO)
