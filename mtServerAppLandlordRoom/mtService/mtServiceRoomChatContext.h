#ifndef 	__MT_SERVICE_ROOM_CHAT_CONTEXT_H
#define 	__MT_SERVICE_ROOM_CHAT_CONTEXT_H
#include "mtService.h"
#include "mtQueueMgr.h"

/// ���������������
class 	mtServiceRoomChatContext : public mtService
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
		char                            pcContext[32];          /// �������������
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lUserId;                /// ����������û�id
		char                            pcContext[32];          /// �������������
	};
public:
	mtServiceRoomChatContext();
	virtual ~mtServiceRoomChatContext();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();

	mtQueueUser*	m_pkQueueUser;
	mtQueuePacket*	m_pkQueuePacket;
};

#endif 	/// __MT_SERVICE_ROOM_CHAT_CONTEXT_H

