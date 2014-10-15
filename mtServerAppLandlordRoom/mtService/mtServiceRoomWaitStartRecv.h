#ifndef 	__MT_SERVICE_ROOM_WAIT_START_RECV_H
#define 	__MT_SERVICE_ROOM_WAIT_START_RECV_H
#include "mtService.h"
#include "mtQueueMgr.h"
#include "mtServiceMgr.h"

/// �û����"��ʼ"����ʼ�ȴ�����
class 	mtServiceRoomWaitStartRecv : public mtService
{
public:
	struct DataInit
	{
		long				    lStructBytes;
		HANDLE	                hIOCP;
		mtQueueMgr*             pkQueueMgr;
		mtServiceMgr*	        pkServiceMgr;
	};

	struct DataRead
	{
		long 				lStructBytes;
		long 				lServiceType;
		long 				plReservation[2];		/// �����ֶ�
	};

	struct DataWrite
	{
		long 				lStructBytes;
		long 				lServiceType;
		long 				plReservation[2];		/// �����ֶ�
		long 				lUserId;
	};
public:
	mtServiceRoomWaitStartRecv();
	virtual ~mtServiceRoomWaitStartRecv();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();

	int getQueueUserNextQueueUser(int iQueueUserCurIdx);
	 
	/// ai����ʼ�ȴ���������Ӧ
	int playWaitStartRecvAi(mtQueueDesk::DataNode* pkQueueDeskDataNode, long lServiceType, int iQueueUserIdx);
	int playReplaceAiExitRoom(mtQueueDesk::DataNode* pkQueueDeskDataNode);
	int playRoundOver(mtQueueUser::DataNode* pkQueueUserDataNode);

	HANDLE	                m_hIOCP;
	mtQueueMgr*             m_pkQueueMgr;
	mtServiceMgr*	        m_pkServiceMgr;
};

#endif 	/// __MT_SERVICE_ROOM_WAIT_START_RECV_H

