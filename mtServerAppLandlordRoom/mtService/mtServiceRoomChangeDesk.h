#ifndef		__MT_SERVICE_ROOM_CHANGE_DESK_H
#define		__MT_SERVICE_ROOM_CHANGE_DESK_H
#include "mtService.h"
#include "mtQueueHall.h"
#include "mtQueueUser.h"
#include "mtQueueMgr.h"

class mtServiceMgr;
///
class mtServiceRoomChangeDesk : public mtService
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
		long                            lUserId;                /// �û�id
	};

public:
	mtServiceRoomChangeDesk();
	virtual ~mtServiceRoomChangeDesk();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();

	int playRoomChangeDeskOne(mtQueueUser::DataNode* pkQueueUserDataNode, mtQueueHall::DataRoom* pkQueueHallDataRoom);
	int runRoomChangeDeskTwo(mtQueueDesk::DataNode* pkQueueDeskDataNode, mtQueueUser::DataNode* pkQueueUserDataNode
		                        , mtQueueHall::DataRoom* pkQueueHallDataRoom);
	int runRoomChangeDeskThree(mtQueueDesk::DataNode* pkQueueDeskDataNode, mtQueueUser::DataNode* pkQueueUserDataNode
		                        , mtQueueHall::DataRoom* pkQueueHallDataRoom);
	int getQueueUserNextQueueUser(int iQueueUserCurIdx);
	int getQueueUserPreQueueUser(int iQueueUserCurIdx);

	mtQueueMgr*             m_pkQueueMgr;
};

#endif ///	__MT_SERVICE_ROOM_CHANGE_DESK_H
