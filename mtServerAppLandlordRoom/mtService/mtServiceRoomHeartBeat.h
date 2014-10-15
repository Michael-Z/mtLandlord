#ifndef 	__MT_SERVICE_ROOM_HEART_BEAT_H
#define 	__MT_SERVICE_ROOM_HEART_BEAT_H
#include "mtService.h"
#include "mtQueueMgr.h"

/// �û���������(ÿ����ʵ�û��ͻ��ˣ���Ҫÿ��20���ӷ���һ��������ѯ����)
class 	mtServiceRoomHeartBeat : public mtService
{
public:
	struct DataInit
	{
		long				    lStructBytes;
		mtQueueUser*	        pkQueueUser;
		mtQueuePacket*	        pkQueuePacket;
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
		long                            lUserId;                /// ���������������û�id(0 -û���û���ʱ��0 < -��ʱ�û���id)
	};
public:
	mtServiceRoomHeartBeat();
	virtual ~mtServiceRoomHeartBeat();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();

	mtQueueUser*	m_pkQueueUser;
	mtQueuePacket*	m_pkQueuePacket;
};

#endif 	/// __MT_SERVICE_ROOM_CHAT_ICON_H

