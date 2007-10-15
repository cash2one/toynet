/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// PoporuDataLimits.h
//  - Arcade ������ ũ�� ����
//
#if !defined(INCLUDED_POPORUDATALIMITS)
#define INCLUDED_POPORUDATALIMITS

#include <skynet/Wdr.h>

namespace skynet
{

/**
 * �������� �ִ� ����Ʈ��
 */
enum DataLimits
{
    DL_MAX_ADDRESS = 15, ///< �ִ� IP �ּ� ����Ʈ��(xxx.xxx.xxx.xxx)
    DL_MAX_USERID = 12, ///< User_Login.ul_id �ִ� ����Ʈ��
    DL_MAX_PASSWORD = 12, ///< User_Login.ul_pw �ִ� ����Ʈ��
    DL_MAX_ENCRYPTED_PASSWORD = 32, ///< ��ȣȭ�� ��� �ִ� ����Ʈ��
    DL_MAX_NICKNAME = 20, ///< User_Login.ul_nick �ִ� ����Ʈ��
    DL_MAX_AREA = 20, ///< �ִ� ������
    DL_MAX_INTRODUCTION = 200, ///< �ִ� �ڱ� �Ұ�(�λ縻)

    DL_MAX_CARD_LEVEL = 99, ///< �ִ� ĳ���� ī�� ����
    DL_MAX_CSD = 50, ///< CSD �ִ� ����
    DL_MAX_CCD = 50, ///< CCD �ִ� ����

    DL_MAX_GUILDNAME = 16, ///< ���� �ִ� ����Ʈ ��

    DL_MAX_NOTICE = 3000, ///< �������� �ִ� ����Ʈ ��

    DL_MAX_SLOT_COUNT = 8, ///< ���ӹ� ���� ��

    DL_MAX_REPORT_CONTENTS = 100, ///< �Ű� ����
    DL_MAX_REPORT_LOG = 7000, ///< �Ű� �α�

    /// �Ϸ� ������ ������ ������ ����(RP, Rupo) ���� ���� ���� Ƚ��
    DL_MAX_MATCH_COUNT_FOR_REWARD_IN_A_DAY = 5,

    DL_MAX_OPTIONS_SIZE = 1000, ///< ����� �ɼ��� �ִ� ����Ʈ ��

    // TODO: Ȯ�� �ȉ���
    DL_MAX_CHATTING = 256, ///< �ִ� ä�� �޼��� ����Ʈ��
    DL_MAX_ROOM_NAME = 100, ///< �ִ� ��ȭ�� �̸� ����Ʈ��
    DL_MAX_ROOM_PASSWORD = 20, ///< �ִ� ��ȭ�� ��й�ȣ ����Ʈ��
    DL_DEFAULT_OBSERVER_COUNT = 30, ///< �⺻ ���� ����� �ִ� ��
    DL_MAX_FORWARD_DATA = 512, ///< ������ �������� �ִ� ����Ʈ ��
    DL_MAX_CHANNEL_NAME = 15, ///< ä�� �̸� �ִ� ����Ʈ ��

    // = ���� ���� ����
    DL_LOGINCOOKIE_SIZE = 40, ///< �α��� ��Ű �ִ� ����Ʈ��
    DL_RELAY_KEY_COUNT_PER_FRAME = 60, ///< �� �� ���� Ű ������ ��
    DL_RELAY_KEY_SIZE = Wdr::sizeLong, ///< ���� Ű ����Ʈ �� (4 ����Ʈ)

    // = ������ ����
    DL_MAX_CARD_SLOT_COUNT = 8, ///< ĳ���� ī�� �ִ� ���� ����

    DL_MANAGERCOOKIE_SIZE = 40,
    DL_MAX_SERVICENAME_SIZE = 60,
    DL_MAX_FILE_NAME = 300,
    DL_MAX_PART_FILE_SIZE = 4000,

};

} // namespace skynet

#endif // !defined(INCLUDED_POPORUDATALIMITS)
