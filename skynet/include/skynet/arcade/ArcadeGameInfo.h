/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// ArcadeGameInfo.h
//  - Arcade 게임 정보
//
#if !defined(INCLUDED_POPORUGAMEINFO)
#define INCLUDED_POPORUGAMEINFO

#include "../DataTypes.h"
#include "ArcadeDataLimits.h"
#include <skynet/Wdr.h>
#include <skynet/detail/Assert.h>

namespace skynet
{

/// 게임 종류
enum GameType
{
    GT_UNKNOWN = 0,
    GT_GLOBAL_OPTIONS = GT_UNKNOWN, ///< 전체 게임 옵션
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

/// 1:1 대전 게임인가? (게임 결과를 기준으로)
inline bool isMatchGame(GameType gt)
{
    return isKofSeries(gt) || isSamuraiShodownSeries(gt) ||
        isActionPuzzleGame(gt) || isSportsGame(gt);
}

/// 협력 게임인가? (게임 결과를 기준으로)
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


/// 게임 장르 (게임 통합 구분)
enum GameGenre
{
    GG_UNKNOWN = 0,
    GG_START = 1,

    /*1*/GG_KOF_SERIES = GG_START, ///< 킹오파 시리즈
    /*2*/GG_SAMURAI_SERIES, ///< 사무라이 시리즈
    /*3*/GG_COOPERATIVE, ///< 협력 게임
    /*4*/GG_SPORTS, ///< 스포츠 게임
    /*5*/GG_ACTION_PUZZLE, ///< 액션 퍼즐 게임
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
        WN_ASSERT(false && "잘못된 게임 장르");
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


/// 게임 모드
enum GameModeType
{
    GM_UNKNOWN = 0, ///< 에러 검사용

    /*1*/ GM_SINGLE, ///< 싱글 모드 (1인 플레이)

    /*2*/ GM_DUAL_MATCH, ///< 1:1 대전 (2인 플레이)
    /*3*/ GM_DUAL_MATCH_TEAM, ///< 1:1 팀 배틀 (2인 플레이)

    /*4*/ GM_ONE_COIN, ///< 1코인 (1/2인 플레이)
    /*5*/ GM_CONTINUE, ///< 컨티뉴 (2인 플레이)

    /*6*/ GM_THREE_INNING, ///< 3이닝 (2인 플레이)
    /*7*/ GM_NINE_INNING,///< 3이닝 (2인 플레이)

    /*8*/ GM_TOURNAMENT, ///< 토너먼트
    /*9*/ GM_LEAGUE, ///< 리그

    /*10*/ GM_DUAL_MATCH_DOUBLE, ///< 1:1 삼판양승 (2인 플레이)

    /*11*/ GM_DUAL_MATCH_HALF, ///< 네오지오컵의 1:1 하프(전반전)(2인 플레이)
    /*12*/ GM_DUAL_MATCH_FULL, ///< 네오지오컵의 1:1 풀(전후반전)(2인 플레이)

    /*13*/ GM_RANDOM_DUAL_MATCH, ///< 랜덤 대전 모드 (8인 플레이)

    /*14*/ GM_THREE_ROUND, ///< 액션 퍼즐 3라운드
    /*15*/ GM_FIVE_ROUND, ///< 액션 퍼즐 5라운드

    /*16*/ GM_FIVE_STAGE, ///< 조이조이키드 5스테이지(기본 모드)

    /*17*/ GM_REGULATION, ///< 리그볼링 레귤러 게임

    //GM_TEAM_MATCH, ///< 팀 매치
    //GM_TEAM_SURVIVAL, ///< 팀서바이벌
    //GM_CHAMPION, ///< 챔피언 모드

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

/// 1:1 대전 게임 모드인가?
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

// 협력 게임 모드인가?
inline bool isCooperativeGame(GameModeType gm)
{
    return (GM_ONE_COIN == gm) || (GM_CONTINUE == gm);
}


/// 게임 계급
enum GameClass
{
    GC_UNKNOWN = 255,

    GC_START = 0,
    GC_MASTER = GC_START, ///< 지존
    GC_HIGHER_GRADER1, ///< 초고수1
    GC_HIGHER_GRADER2, ///< 초고수2
    GC_HIGHER_GRADER3, ///< 초고수3
    GC_HIGH_GRADER1 = 4, ///< 고수1
    GC_HIGH_GRADER2, ///< 고수2
    GC_HIGH_GRADER3, ///< 고수3
    GC_HIGH_GRADER4, ///< 고수4
    GC_HIGH_GRADER5, ///< 고수5
    GC_MIDDLE_HIGH_GRADER1 = 9, ///< 중고수1
    GC_MIDDLE_HIGH_GRADER2, ///< 중고수2
    GC_MIDDLE_HIGH_GRADER3, ///< 중고수3
    GC_MIDDLE_HIGH_GRADER4, ///< 중고수4
    GC_MIDDLE_HIGH_GRADER5, ///< 중고수5
    GC_MIDDLE_GRADER1 = 14, ///< 중수1
    GC_MIDDLE_GRADER2, ///< 중수2
    GC_MIDDLE_GRADER3, ///< 중수3
    GC_MIDDLE_GRADER4, ///< 중수4
    GC_LOW_GRADER1 = 18, ///< 하수1
    GC_LOW_GRADER2, ///< 하수2
    GC_LOW_GRADER3, ///< 하수3
    GC_BEGINNER1 = 21, ///< 초보1
    GC_BEGINNER2, ///< 초보2
    GC_BEGINNER3, ///< 초보3

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
 * 토너먼트 결과
 * - trChampionship를 제외한 나머지 스테이지는 토너먼트 모드에서만 의미가 있다
 */
enum TournamentResult
{
    trUnknown = 0,
    trQuarterFinal, ///< 8강 진출(8강 패배)
    trSemiFinal, ///< 4강 진출(4강 패배)
    trFinal, ///< 결승 진출(준우승)
    trChampionship, ///< 우승
    trBoundary
};

inline bool isValidTournamentResult(TournamentResult stage)
{
    return (trUnknown < stage) && (stage < trBoundary);
}

} // namespace skynet

#endif // !defined(INCLUDED_POPORUGAMEINFO)
