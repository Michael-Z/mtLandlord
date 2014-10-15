#ifndef		__MT_SERVICE_ROOM_INCREMENT_GOLD_H
#define		__MT_SERVICE_ROOM_INCREMENT_GOLD_H
#include "mtService.h"
#include "mtQueueHall.h"
#include "mtQueueUser.h"
#include "mtQueueMgr.h"


/// ���û����ӽ��
class mtServiceRoomIncrementGold : public mtService
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
		long 							lGold;	                /// ���ӽ����
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lUserId;                /// ���ӽ�ҵ��û�id
		long 							lGold;	                /// ���ӽ����
	};

public:
	mtServiceRoomIncrementGold();
	virtual ~mtServiceRoomIncrementGold();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();

	mtQueueMgr*             m_pkQueueMgr;
};

#endif ///	__MT_SERVICE_ROOM_INCREMENT_GOLD_H
