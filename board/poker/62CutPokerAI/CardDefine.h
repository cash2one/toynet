
// CardDefine.h

#ifndef CARDDEFINE_H_
#define CARDDEFINE_H_

#include <windows.h>

enum EVENT_ENUM
{
	// [ ���� ��Ŀ ]
	EVENT_THREEFLYEND = 1,		   // ù 3���� ��� ���ȴ�
	EVENT_ONEFLYEND,			   // ���徿
	EVENT_ONEFLYTOTEND,			   // ���徿 ���
	EVENT_CHANGEDEL_ONEFLYEND,		   // 62cut
	EVENT_CHANGESET_ONEFLYEND,




	EVENT_IAMREADY,				// ������ ������ �غ� �Ǿ���	
								// (�� �̺�Ʈ�� ���� ī�尡 �÷��̾�3���� ���ư��� ī������ ��Ÿ���� �̺�Ʈ�� ������.)
	EVENT_DUMMYTOFLOOR,			// ī�� ���̿��� ������ ������ �ٴڿ� ���
	EVENT_FLOORTOGETCARD,		// ���� ī�带 ������
	EVENT_DUMMYTOMYCARD,		// ī�� ���̿��� ������ ������ �÷��̾�� �ѱ��
	EVENT_ENDMYTURN,			// �����ʰ� ����
	EVENT_NOTIFYSTRING,			// �˸� �޽����� �ʿ���..
	EVENT_DISTRIBUTETOPLAYER0,	// 
	EVENT_DISTRIBUTETOPLAYER1,	// 
	EVENT_DISTRIBUTETOPLAYER2,	// 
	EVENT_DISTRIBUTETOPLAYER3,	// 
	EVENT_DISTRIBUTETOBOARD,	// �ٴڿ� ��°�..
	EVENT_SORTBOARDCARD,		// �ٴڿ� �� �и� �����ϱ�..
	EVENT_SORTMYHANDCARD,		// ���� ���� �и� �����ϱ�..
	EVENT_JUMP,					// �������� ���� ����~
	EVENT_SORTPEA,
//	EVENT_GETPART,			// ���� ���� �и� ��������..
	EVENT_ROTATECARD,			// ī���� ȸ���ϱ�( Rotate.. )
	EVENT_EXECUTERESERVE,
	EVENT_DISTRIBUTETOBOARD_AND_RESERVEMYGETCARD, // �ٴڿ� �� �Ŀ� ���� ���� �з� �� ���� �����Ѵ�.
	
	EVENT_TURN,					// �� ������..
	EVENT_TURNANDGOTOMYCARD,	// ������ �Ŀ� ���� �� ��ġ�� ����..
	EVENT_TURNANDGOTOMYHAND,	// ������ �Ŀ� �� ������ ...
	EVENT_DUMMYTURN_GO_AND_GETPART,	// ������ �Ŀ� �ٴڿ� ��Ƽ� ���� �и� �����´�.
									// ( ���а� 2���� ����� 3�� �̻��̸� ���縦 �� ����̹Ƿ� �����´�.)
	EVENT_DUMMYTURN_GO_AND_SULSA,	// ����..
	EVENT_DUMMYTURN_GO_AND_JJOK,	// ��
	EVENT_DUMMYTURN_GO_AND_DDADAK	// ����..
};

enum RESERVE_ENUM
{
	RESERVE_GOTOGETCARD = 1,
	RESERVE_TAKEPEA,
	RESERVE_GOTOGETCARD_AND_TAKEPEA
};

enum ATTR_ENUM
{
	ATTR_NONE,					// �ƹ��͵� �ƴ�.
	ATTR_NORMALPEA,				// �׳� ��
	ATTR_NORMALFIVEEND,			// �ټ� ��
	ATTR_NORMALTENEND,			// �� ��
	ATTR_DOUBLEPEA,				// ����
	ATTR_SERVICEDOUBLEPEA,		// ���� ����
	ATTR_SERVICETHREEPEA,		// ���� 3��
	ATTR_RED,					// ȫ��
	ATTR_BLUE,					// û��
	ATTR_GRASS,					// �ʴ�
	ATTR_BIRD,					// ����
	ATTR_MUMEND,				// ������
	ATTR_RAINLIGHT,				// ��
	ATTR_NORMALLIGHT			// ��
};

typedef struct
{
	int CardNo;			// ī�� ��ȣ
	int EventCode;		// �̺�Ʈ �ڵ�
	int Option;			// �̺�Ʈ �ɼ�
	int Option2;		// �ɼ� 2
	int Option3;
} CARDEVENT;

typedef struct
{
	int Code;
	int Option;			// �̺�Ʈ �ɼ�
	int Option2;		// �ɼ� 2
	int Option3;
}RESERVATION;

typedef struct
{
	int Month;	// ��� ������ ���ΰ�( 1 ~ 12.. �� ������, ���� ������ ���.. �� �����д� 13���� �Ѵ�. )
	int Part;	// ��(0)�ΰ�, ����(1)�ΰ�, �ټ���(2)�ΰ�, ��(3)�ΰ�..( 
	int attr;	// �������� �𸣴� ����( ���� �Ӽ��� �˷����� ���� ���. �� ī�尡 �޸��� ���̴� ����. 0 );
				// �׳� �� 1
				// �׳� �ټ� �� 2	
				// �׳� ���� 3
				// ������ ��� 4
				// ������( ���� ) 5
				// ������( 3�� ) 6
				// ȫ��		7
				// û��		8 
				// �ʴ�		9	
				// ����  10
				// ���� �� 11
				// ��	   12
				// �׳� �� 13
				// ��ź    14
}CARD_INFO;


#endif