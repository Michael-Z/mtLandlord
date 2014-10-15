#ifndef 	__MT_SERVICE_ROOM_CHAT_SHORT_CUT_H
#define 	__MT_SERVICE_ROOM_CHAT_SHORT_CUT_H
#include "mtService.h"
#include "mtQueueMgr.h"

/// ������������
class 	mtServiceRoomChatShortCut : public mtService
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
		long                            lShortCutIdx;           /// ������id
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lUserId;                /// ����������û�id
		long                            lShortCutIdx;           /// ������id
	};
public:
	mtServiceRoomChatShortCut();
	virtual ~mtServiceRoomChatShortCut();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();

	mtQueueUser*	m_pkQueueUser;
	mtQueuePacket*	m_pkQueuePacket;
};

#endif 	/// __MT_SERVICE_ROOM_CHAT_SHORT_CUT_H

