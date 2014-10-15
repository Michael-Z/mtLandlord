#ifndef		__MT_SERVICE_ROOM_REQUEST_CARDS_LEFT_H
#define		__MT_SERVICE_ROOM_REQUEST_CARDS_LEFT_H
#include "mtService.h"
#include "mtQueueHall.h"
#include "mtQueueUser.h"
#include "mtQueueMgr.h"

class mtServiceMgr;

/// һ����Ϸ��������(�·��������ҵ�ʣ��������Ϣ�������û�)
class mtServiceRoomRequestCardsLeft : public mtService
{
public:
	struct DataInit
	{
		long				    lStructBytes;
		mtQueueMgr*             pkQueueMgr;
	};

	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		char 							pcCardsLast[24];        /// �ϼ�ʣ�����
		char							pcCardsNext[24];		/// �¼�ʣ�����
	};

public:
	mtServiceRoomRequestCardsLeft();
	virtual ~mtServiceRoomRequestCardsLeft();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();

	int getQueueUserNextQueueUser(int iQueueUserCurIdx);
	int getQueueUserPreQueueUser(int iQueueUserCurIdx);

	mtQueueMgr*             m_pkQueueMgr;
};

#endif ///	__MT_SERVICE_ROOM_REQUEST_CARDS_LEFT_H
