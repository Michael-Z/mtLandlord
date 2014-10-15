#ifndef 	__MT_SERVICE_ROOM_CHAT_ICON_H
#define 	__MT_SERVICE_ROOM_CHAT_ICON_H
#include "mtService.h"
#include "mtQueueMgr.h"

/// �����������
class 	mtServiceRoomChatIcon : public mtService
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
		long                            lIconIdx;               /// �����id
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lUserId;                /// ����������û�id
		long                            lIconIdx;               /// �����id
	};
public:
	mtServiceRoomChatIcon();
	virtual ~mtServiceRoomChatIcon();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();

	mtQueueUser*	m_pkQueueUser;
	mtQueuePacket*	m_pkQueuePacket;
};

#endif 	/// __MT_SERVICE_ROOM_CHAT_ICON_H

