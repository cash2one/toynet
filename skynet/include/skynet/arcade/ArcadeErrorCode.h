/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author �躴�� <kcando@megaking.co.kr>
*/
// PoporuErrorCode.h
//  - Arcade ���� �ڵ� ����
//
#if !defined(INCLUDED_POPORUERRORCODE)
#define INCLUDED_POPORUERRORCODE

#include "../ErrorCode.h"

namespace skynet
{

/// ���̵� �Ǵ� ����� ��ġ���� �ʴ´�
DEFINE_ERRORCODE EC_INVALID_ACCOUNT = -10;
/// ���� �������� �̹� �α��� ���̴�
DEFINE_ERRORCODE EC_ALREADY_LOGIN = -20;
/// ä�� ������ ���̻� ����ڸ� ������ �� ����
DEFINE_ERRORCODE EC_TOO_MANY_USERS = -30;
/// �߸��� ����Ÿ��
DEFINE_ERRORCODE EC_INVALID_GAMETYPE = -36;

/// ����ڸ� ã�� �� ����
DEFINE_ERRORCODE EC_USER_NOT_FOUND = -40;
/// ī�� ��ȣ�� �߸��Ǿ���(ī�尡 �������� �ʴ´�)
DEFINE_ERRORCODE EC_USER_INVALID_CARD_NUMBER = -45;

/// ���̻� (����)���� ����� ����.
DEFINE_ERRORCODE EC_ROOM_LIMIT_EXCEEDED = -50;
/// ���� ã�� �� ����
DEFINE_ERRORCODE EC_ROOM_NOT_FOUND = -60;
/// �� ����� ��ġ���� �ʴ´�
DEFINE_ERRORCODE EC_ROOM_INVALID_PASSWORD = -70;
/// ��(�κ�)�� �̹� ���� ���̴�
DEFINE_ERRORCODE EC_ROOM_ALREADY_JOINED = -80;
/// �濡 ��� �ִ� �ڸ��� ����
DEFINE_ERRORCODE EC_ROOM_NO_MORE_SEAT = -90;
/// BAN ���� �濡�� �ٽ� �� �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_JOIN_WHEN_BANNED = -97;
/// ���� ���� ���� ������ �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_JOIN_WHEN_PLAYING = -98;
/// ���� ���� �濡�� ������ �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_PART_WHEN_PLAYING = -99;
/// �濡�� ����ڸ� ã�� �� ����
DEFINE_ERRORCODE EC_ROOM_USER_NOT_FOUND = -100;
/// ������ ������ �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_START = -110;
/// ������ ������ �� ����(������ �����ϱ� ���� �غ� �ؾ� �Ѵ�)
DEFINE_ERRORCODE EC_ROOM_MUST_READY_BEFORE_START_GAME = -111;
/// ������ �ߴ��� �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_STOP = -120;
/// �߸��� ���� ���
DEFINE_ERRORCODE EC_INVALID_GAME_RESULT = -125;
/// ���� ����ڷ� ��ȯ�� �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_LEAVE_SLOT = -130;
/// ���� ����ڷ� ��ȯ�� �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_ENTER_SLOT = -140;
/// 1�δ� �� �ϳ��� ���� �� �ִ�
DEFINE_ERRORCODE EC_ROOM_CREATE_ONLY_ONE = -160;
/// 1�δ� �� �ϳ����� ������ �� �ִ� 
DEFINE_ERRORCODE EC_ROOM_JOIN_ONLY_ONE = -170;
/// �÷��̾��� UDP ������ �������� �ʾƼ� ������ ������ �� ����
DEFINE_ERRORCODE EC_UDP_ADDRESS_IS_NOT_SETTED = -175;
/// ���� ���̾ ������ ������ �� ����
DEFINE_ERRORCODE EC_OBSERVING_IS_NOT_FINISHED = -177;

/// ����ڸ� �ʴ��� �� ����(����ڰ� ���ų� �濡 �̹� ���� ���� ���)
DEFINE_ERRORCODE EC_CANT_INVITE_ROOM_USER = -190;

/// ���� ��带 ������ �� ����(����; �� ����; �ο� ���� ������ ����)
DEFINE_ERRORCODE EC_CANT_CHANGE_GAME_MODE = -200;

/// ���� ��û�� �� �� ����
DEFINE_ERRORCODE EC_CANT_CHALLENGE = -210;

/// ���� ��尡 �߸��Ǿ� ���� ������ �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_CREATE_ROOM_BECAUSE_INVALID_GAME_MODE = -220;
/// ������ ���� ��� ���� ���� �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_CREATE_ROOM_BECAUSE_NO_COIN = -221;
/// �� ���� �ڰ� ������ ���� �ʾ� ���� ������ �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_CREATE_ROOM_BECAUSE_NO_PERMISSION = -230;
/// �� ���� �ڰ� ������ ���� �ʾ� �� ������ �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_JOIN_ROOM_BECAUSE_NO_PERMISSION = -240;

/// RP �������� ���� ������ �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_CREATE_ROOM_BECAUSE_OVER_RP = -241;
/// RP �������� �濡 ������ �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_JOIN_ROOM_BECAUSE_OVER_RP = -242;

/// RP �������� ���� ������ �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_CREATE_ROOM_BECAUSE_SHORTAGE_RP = -243;
/// RP �������� �濡 ������ �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_JOIN_ROOM_BECAUSE_SHORTAGE_RP = -244;


/// ������ ���� �� ����
DEFINE_ERRORCODE EC_CANT_INSERT_COIN = -250;

/// ������ �غ��� �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_READY = -300;
/// �÷��̾�� ���Ӷ��带 ������ �� ����
DEFINE_ERRORCODE EC_PLAYER_CANT_SELECT_GAMEROUND = -310;
/// ���Ӷ��带 ã�� �� ����
DEFINE_ERRORCODE EC_GAMEROUND_NOT_FOUND = -312;
/// �̹� ���Ӷ��带 �����Ͽ���(�� �ѹ��� ������ �� �ִ�)
DEFINE_ERRORCODE EC_ALREADY_SELECT_GAMEROUND = -314;
/// ������ �� ���� ���Ӷ���(��ʸ�Ʈ ������ ���Ӷ���)
DEFINE_ERRORCODE EC_CANT_OBSERVE_UNEARNEDWIN_GAMEROUND = -315;

/// ����Ÿ��Ʋ�� �ٸ��� ������ �濡 ������ �� ����
DEFINE_ERRORCODE EC_ROOM_CANT_JOIN_BECAUSE_DIFFERENT_GAMETYPE = -320;

/// ������ �̵��� �� ����
DEFINE_ERRORCODE EC_CANT_MOVE_COIN = -330;
/// �����Կ� ������ ������ �ִ�.
DEFINE_ERRORCODE EC_HAVE_COIN_BAG	= -331;

/// �������� �ɸ��� ī�忡 ������ ���� �ִ�.
DEFINE_ERRORCODE EC_CHARACTER_CARD_COIN	= -332;

/// ������ ������ ī�尡 DB�� ����.
DEFINE_ERRORCODE EC_CHARACTER_CARD_NOT_FOUND = -333;

/// ä���� �����Ǿ���
DEFINE_ERRORCODE EC_CHATTING_IS_DISABLED = -340;

// = ������ ����

/// �߸��� ī�� ���� �ε���
DEFINE_ERRORCODE EC_INVALID_CARDSLOTINDEX = -10000;

/// �߸��� ItemId
DEFINE_ERRORCODE EC_INVALID_ITEMID = -10010;

/// �̹� ��� ����(���Կ� ����) ������
DEFINE_ERRORCODE EC_ALREADY_USED_ITEM = -10020;

/// ī�� ������ �� á��
DEFINE_ERRORCODE EC_CARDSLOT_IS_FULL = -10030;

/// ���� Ÿ���� �������� ���ÿ� ������ �� ����
DEFINE_ERRORCODE EC_CANT_ATTACH_SAME_TYPE_ITEM = -10040;

/// �Ŀ�(��) �������� ������ �� ����
DEFINE_ERRORCODE EC_CANT_ATTACH_POWERITEM = -10050;

/// ���� �ȵ� �������� ������ �� ����
DEFINE_ERRORCODE EC_SLOTITEM_IS_NOT_ATTACHED = -10060;

// = �޽��� ���� ����

DEFINE_ERRORCODE EC_IM_START = -30000;
/// ģ���� �̹� ��ϵǾ� �ִ�
DEFINE_ERRORCODE EC_IM_ALREADY_REGISTERED_BUDDY = (EC_IM_START - 1);
/// �ʹ� ���� ģ���� ��ϵǾ� �־ ���̻� ����� �� ����
DEFINE_ERRORCODE EC_IM_CANT_REGISTER_BUDDY_BECAUSE_TOO_MANY_BUDDY =
    (EC_IM_START - 2);
/// ���� ģ��(������ ���� ģ���� ����ص�)�ʹ� �ӼӸ��� �� �� ����
DEFINE_ERRORCODE EC_IM_CANT_WHISPER_WITH_BAD_BUDDY = (EC_IM_START - 3);
/// ���� ģ��(������ ���� ģ���� ����ص�)�� �ʴ��� �� ����
DEFINE_ERRORCODE EC_IM_CANT_INVITE_BAD_BUDDY = (EC_IM_START - 4);
/// �ڱ� �ڽ��� ����� �� ����
DEFINE_ERRORCODE EC_IM_CANT_REGISTER_YOURSELF = (EC_IM_START - 5);

/// ���� ���� ����
DEFINE_ERRORCODE EC_SERVER_FAULT = -40000;

/// �����ͺ��̽� ���� ����
DEFINE_ERRORCODE EC_DATABASE_FAULT = -50000;

/// �޴��� �������� ���� different 
//�޴��� ��Ű�� �ٸ���
DEFINE_ERRORCODE EC_DIFFERENT_MANAGERCOOKIE = -60000;

//�޴��� DB �������̵� �ٸ���.
DEFINE_ERRORCODE EC_DEFFERENT_MANAGERUSERID = -61000;
//�޴��� DB �̹� �ִ� ���� .
DEFINE_ERRORCODE EC_EXISTS_USERID = -61001;
//�޴��� DB ����PW �ٸ���.
DEFINE_ERRORCODE EC_DEFFERENT_MANAGERPASSWORD = -61100;
//�޴��� DB �̹� ������
DEFINE_ERRORCODE EC_MANAGERLOGIN_ALREADY = -61200;


//���� ü�ο� �ƹ��� ����.
DEFINE_ERRORCODE EC_NOT_ANY_USERS = -61300;
//�߸��� ���� Ÿ��
DEFINE_ERRORCODE EC_MISS_AUTHENTIC_MODE_TYPE = -61400;
//����� �ٷ�� ������ ����. 
DEFINE_ERRORCODE EC_ASK_AUTHORITY = -61500;
// �߸��� ���� �ڵ�
DEFINE_ERRORCODE EC_WRONG_FILE_HANDLE = -61600;
// ���� ���� ����
DEFINE_ERRORCODE EC_WRITE_FILE_ERROR = -61601;
// ���� �ϼ��� ����� �ٸ���.
DEFINE_ERRORCODE EC_DEFFERENT_COMPLETION_FILE_SIZE = -61602;

DEFINE_ERRORCODE EC_FIND_NOT_SERVER = -61700;

DEFINE_ERRORCODE EC_ACTION_SERVICE_BAD = -61800;

inline bool isAlreadyLogin(ErrorCode ec) {
    return ec != EC_ALREADY_LOGIN;
}
} // namespace skynet

#endif // !defined(INCLUDED_POPORUERRORCODE)
