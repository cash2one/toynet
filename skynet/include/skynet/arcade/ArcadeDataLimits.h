/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author 김병수 <kcando@megaking.co.kr>
*/
// PoporuDataLimits.h
//  - Arcade 데이터 크기 제한
//
#if !defined(INCLUDED_POPORUDATALIMITS)
#define INCLUDED_POPORUDATALIMITS

#include <skynet/Wdr.h>

namespace skynet
{

/**
 * 데이터의 최대 바이트수
 */
enum DataLimits
{
    DL_MAX_ADDRESS = 15, ///< 최대 IP 주소 바이트수(xxx.xxx.xxx.xxx)
    DL_MAX_USERID = 12, ///< User_Login.ul_id 최대 바이트수
    DL_MAX_PASSWORD = 12, ///< User_Login.ul_pw 최대 바이트수
    DL_MAX_ENCRYPTED_PASSWORD = 32, ///< 암호화된 비번 최대 바이트수
    DL_MAX_NICKNAME = 20, ///< User_Login.ul_nick 최대 바이트수
    DL_MAX_AREA = 20, ///< 최대 지역명
    DL_MAX_INTRODUCTION = 200, ///< 최대 자기 소개(인사말)

    DL_MAX_CARD_LEVEL = 99, ///< 최대 캐릭터 카드 레벨
    DL_MAX_CSD = 50, ///< CSD 최대 길이
    DL_MAX_CCD = 50, ///< CCD 최대 길이

    DL_MAX_GUILDNAME = 16, ///< 길드명 최대 바이트 수

    DL_MAX_NOTICE = 3000, ///< 공지사항 최대 바이트 수

    DL_MAX_SLOT_COUNT = 8, ///< 게임방 슬롯 수

    DL_MAX_REPORT_CONTENTS = 100, ///< 신고 내용
    DL_MAX_REPORT_LOG = 7000, ///< 신고 로그

    /// 하룻 동안의 동일한 유저의 보상(RP, Rupo) 적용 가능 대전 횟수
    DL_MAX_MATCH_COUNT_FOR_REWARD_IN_A_DAY = 5,

    DL_MAX_OPTIONS_SIZE = 1000, ///< 사용자 옵션의 최대 바이트 수

    // TODO: 확정 안됬음
    DL_MAX_CHATTING = 256, ///< 최대 채팅 메세지 바이트수
    DL_MAX_ROOM_NAME = 100, ///< 최대 대화방 이름 바이트수
    DL_MAX_ROOM_PASSWORD = 20, ///< 최대 대화방 비밀번호 바이트수
    DL_DEFAULT_OBSERVER_COUNT = 30, ///< 기본 관전 대기자 최대 수
    DL_MAX_FORWARD_DATA = 512, ///< 포워딩 데이터의 최대 바이트 수
    DL_MAX_CHANNEL_NAME = 15, ///< 채널 이름 최대 바이트 수

    // = 관전 서버 관련
    DL_LOGINCOOKIE_SIZE = 40, ///< 로그인 쿠키 최대 바이트수
    DL_RELAY_KEY_COUNT_PER_FRAME = 60, ///< 초 당 관전 키 프레임 수
    DL_RELAY_KEY_SIZE = Wdr::sizeLong, ///< 관전 키 바이트 수 (4 바이트)

    // = 아이템 관련
    DL_MAX_CARD_SLOT_COUNT = 8, ///< 캐릭터 카드 최대 슬롯 갯수

    DL_MANAGERCOOKIE_SIZE = 40,
    DL_MAX_SERVICENAME_SIZE = 60,
    DL_MAX_FILE_NAME = 300,
    DL_MAX_PART_FILE_SIZE = 4000,

};

} // namespace skynet

#endif // !defined(INCLUDED_POPORUDATALIMITS)
