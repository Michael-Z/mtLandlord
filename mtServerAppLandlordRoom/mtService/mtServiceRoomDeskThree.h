#ifndef		__MT_SERVICE_ROOM_DESK_THREE_H
#define		__MT_SERVICE_ROOM_DESK_THREE_H
#include "mtService.h"
#include "mtQueueHall.h"
#include "mtQueueUser.h"
#include "mtQueueMgr.h"

class mtServiceMgr;

/// ���˽����ӷ���
class mtServiceRoomDeskThree : public mtService
{
public:
	struct DataInit
	{
		long				    lStructBytes;
		mtQueueUser*	        pkQueueUser;
		mtQueuePacket*	        pkQueuePacket;
		mtServiceMgr*	        pkServiceMgr;
	};

	struct DataRead
	{
		long 							lStructBytes;			             /// ����С
		long                        	lServiceType;			             /// ��������
		long 							plReservation[2];		             /// �����ֶ�
		mtQueueUser::DataNode*	        pkQueueUserDataNode[MT_NODE_PLAYER]; /// ��Ա��Ϣ
	} ;

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lResult;                /// �����ӳɹ��� 0ʧ�ܣ�1�ɹ�
		DataUserInfo             		kUserInfo[2];           /// ���������Ϣ
	};

public:
	mtServiceRoomDeskThree();
	virtual ~mtServiceRoomDeskThree();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();

	int setWriteData(mtQueueUser::DataNode* pkDataNode1, mtQueueUser::DataNode* pkDataNode2, DataWrite* pkDataWrite);

    /// ai�����������ӷ�����Ӧ
	int playThreeDeskAi(){ return 1;}
	int runThreeDeskWaitStartRecvAi(mtQueueUser::DataNode* pkQueueUserDataNode);

	mtQueueUser*	m_pkQueueUser;
	mtQueuePacket*	m_pkQueuePacket;
	mtServiceMgr*	m_pkServiceMgr;
};

#endif ///	__MT_SERVICE_ROOM_DESK_THREE_H