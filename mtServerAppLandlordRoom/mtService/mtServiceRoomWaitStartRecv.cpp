#include "mtServiceRoomWaitStartRecv.h"
#include "mtQueueDesk.h"
#include "mtServiceMgr.h"
#include "mtServiceRoomExit.h"
#include "mtPrint.h"

mtServiceRoomWaitStartRecv::mtServiceRoomWaitStartRecv()
{

}

mtServiceRoomWaitStartRecv::~mtServiceRoomWaitStartRecv()
{

}

int mtServiceRoomWaitStartRecv::init( void* pData )
{
	DataInit*	pkDataInit	= (DataInit*)pData;

	m_hIOCP                 = pkDataInit->hIOCP;
	m_pkQueueMgr            = pkDataInit->pkQueueMgr;
	m_pkServiceMgr          = pkDataInit->pkServiceMgr;

	return	1;
}

int mtServiceRoomWaitStartRecv::run( DataRun* pkDataRun )
{
	/// ���������Ϸ(һ������֮��)
	playRoundOver(pkDataRun->pkQueueUserDataNode);

	DataRead* pkDataRead     = (DataRead*)pkDataRun->pkQueuePacketDataNode->pcData;
	mtQueueUser::DataNode* pkQueueUserDataNode = pkDataRun->pkQueueUserDataNode;
	mtQueueDesk::DataNode* pkQueueDeskDataNode  = (mtQueueDesk::DataNode*)pkDataRun->pkQueueUserDataNode->pkQueueDeskDataNode;

	mtPrint(pkDataRead);
	
	if (NULL == pkQueueDeskDataNode)
	{
	    if (NULL != pkQueueUserDataNode)
	    {
			pkQueueUserDataNode->lIsWaitStart    = 1;
	    }
		
		return 1;
	}

	EnterCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
	/// ���¸��˵ȴ���ʼ��־
	pkQueueUserDataNode->lUserExit     = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING;
	pkQueueDeskDataNode->plIsWaitStart[pkQueueUserDataNode->lUserDeskIndex]    = 1;
	pkQueueUserDataNode->lIsWaitStart  = pkQueueDeskDataNode->plIsWaitStart[pkQueueUserDataNode->lUserDeskIndex];

    /// ���������Ա�� ���ȴ���ʼ�� ��Ϣ �������ӵ��������Ա(��ʶĳĳ���Ѿ��ȴ���ʼ��)
	/// ��������ϵ���Ա�б�
	DataWrite kDataWrite;
	memset(&kDataWrite, 0, sizeof(kDataWrite));
	mtQueuePacket::DataNode* pkQueuePacketDataNode;
	int iQueueUserDataNodeIdx;

	kDataWrite.lStructBytes      = sizeof(kDataWrite);
	kDataWrite.lUserId           = pkQueueUserDataNode->lUserId;
	kDataWrite.lServiceType      = pkDataRead->lServiceType;

	for (iQueueUserDataNodeIdx = 0; iQueueUserDataNodeIdx < MT_NODE_PLAYER; iQueueUserDataNodeIdx++)
	{
		if (NULL != pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx])
		{
			///  ���͡��ȴ���ʼ�� ��Ϣ
			///  ��ǰ����ʵ�û�
			if (mtQueueDesk::E_PLAYER_MODE_USER == pkQueueDeskDataNode->plPlayerMode[iQueueUserDataNodeIdx])
			{
				pkQueueDeskDataNode->plIsWaitStart[iQueueUserDataNodeIdx]  = pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx]->lIsWaitStart;
				pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("waitstartrecv72");
				if (NULL != pkQueuePacketDataNode)
				{
					pkQueuePacketDataNode->kWSABuf.len  = kDataWrite.lStructBytes;
					memcpy(pkQueuePacketDataNode->pcData, &kDataWrite, kDataWrite.lStructBytes);
					mtPrint(&kDataWrite);
					m_pkQueueMgr->getQueueUser()->DataWrite(pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx], pkQueuePacketDataNode);
				}
			}
		}
	}

	///
	//  ��ǰ����ʵ�û��Ŀ�ʼ�ȴ�1~12
	if (mtQueueDesk::E_PLAYER_MODE_USER == pkQueueDeskDataNode->plPlayerMode[pkQueueUserDataNode->lUserDeskIndex])
	{
		/// ����¼��û��������ϵ�λ��
		int iQueueDeskUserNextIdx = getQueueUserNextQueueUser(pkQueueUserDataNode->lUserDeskIndex);

		//  �¼���ai�û�, �ڴ�֪ͨ�����û� ��ai��ʼ�ȴ���
		if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iQueueDeskUserNextIdx])
		{
			if (MT_QUEUE_USER_ID_AI_START < pkQueueDeskDataNode->pkQueueUserDataNode[iQueueDeskUserNextIdx]->lUserId)
			{
				/// ai����ʼ�ȴ��߼�
				playWaitStartRecvAi(pkQueueDeskDataNode, pkDataRead->lServiceType, iQueueDeskUserNextIdx);
			}

			/// ���ai�û����¼��������ϵ�λ��
			int iQueueDeskUserNextNextIdx = getQueueUserNextQueueUser(iQueueDeskUserNextIdx);

			//  ai�¼һ�����ai�û�
			if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iQueueDeskUserNextNextIdx])
			{
				if (MT_QUEUE_USER_ID_AI_START < pkQueueDeskDataNode->pkQueueUserDataNode[iQueueDeskUserNextNextIdx]->lUserId)
				{
					playWaitStartRecvAi(pkQueueDeskDataNode, pkDataRead->lServiceType, iQueueDeskUserNextNextIdx);
				}
			}
		}
	}

	/// �жϵ�ǰ��������û��ʹ��ai�������ʵ�û��ڵ�(����û��ʼ�ȴ�)
	playReplaceAiExitRoom(pkQueueDeskDataNode);

	/// ���������ӵ�������Ա�Ƿ� ���Ѿ��ȴ���ʼ��
	/// û��ȫ���ȴ���ʼ  ֱ�ӷ���
	for (iQueueUserDataNodeIdx = 0; iQueueUserDataNodeIdx < MT_NODE_PLAYER; iQueueUserDataNodeIdx++)
	{
		if (NULL != pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx])
		{
			if (!(pkQueueDeskDataNode->plIsWaitStart[iQueueUserDataNodeIdx]))
			{
				LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
				return 1;
			}
		}
		else
		{
			LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
			return 1;
		}
	}

	if (0 == pkQueueDeskDataNode->lCardDeal)
	{  
		pkQueueDeskDataNode->lCardDeal = 1;
		
		/// post��Ϣ����˿ڣ���������ϴ�ƺͷ��ƹ��ܣ���ÿ���˷���
		pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("waitstartrecv141");
		if (NULL != pkQueuePacketDataNode)
		{
			kDataWrite.lServiceType             = mtServiceMgr::E_SERVICE_ROOM_CARD_DEAL;
			kDataWrite.lUserId                  = -1;
			pkQueuePacketDataNode->kWSABuf.len  = kDataWrite.lStructBytes;
			memcpy(pkQueuePacketDataNode->pcData, &kDataWrite, kDataWrite.lStructBytes);
			pkQueuePacketDataNode->eDataType    = mtQueuePacket::E_DATA_TYPE_NOT_USED;
			if (m_pkQueueMgr->getQueueUser()->addDataNodePacket(pkDataRun->pkQueueUserDataNode, pkQueuePacketDataNode))
			{		
				PostQueuedCompletionStatus(m_hIOCP, MT_QUEUE_COMPLETION_STATUS_CUSTOM, (ULONG_PTR)pkDataRun->pkQueueUserDataNode, &(pkQueuePacketDataNode->kDataOverLapped));
			}
			else
			{
				m_pkQueueMgr->getQueuePacket()->push(pkQueuePacketDataNode);
			}
		}
		LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
	}
	else
	{
		LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
	}

	return	1;
}

int mtServiceRoomWaitStartRecv::exit()
{

	return	0;
}

int mtServiceRoomWaitStartRecv::playReplaceAiExitRoom(mtQueueDesk::DataNode* pkQueueDeskDataNode)
{
	mtQueueUser::DataNode*      pkQueueUserDataNodeReplaceAi1 = NULL;
	mtQueueUser::DataNode*      pkQueueUserDataNodeReplaceAi2 = NULL;
	int		iUserIdx = 0;

	if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iUserIdx]
	&& MT_QUEUE_USER_ID_AI_START > pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserId)
	{
		pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;

		iUserIdx++;
		if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iUserIdx]
		&& MT_QUEUE_USER_ID_AI_START > pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserId)
		{
			pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
		}
		else
		{
			iUserIdx++;
			if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iUserIdx]
			&& MT_QUEUE_USER_ID_AI_START > pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserId)
			{
				pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
			}
		}
	}
	else
	{
		iUserIdx++;
		if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iUserIdx]
		&& MT_QUEUE_USER_ID_AI_START > pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserId)
		{
			pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;

			iUserIdx++;
			if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iUserIdx]
			&& MT_QUEUE_USER_ID_AI_START > pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserId)
			{
				pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
			}
		}
		else
		{
			iUserIdx++;
			if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iUserIdx]
			&& MT_QUEUE_USER_ID_AI_START > pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserId)
			{
				pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
			}
		}
	}

	return 1;
}

int mtServiceRoomWaitStartRecv::playRoundOver(mtQueueUser::DataNode* pkQueueUserDataNode)
{
	if (NULL != pkQueueUserDataNode)
	{
		if (mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_ROUND_OVER == pkQueueUserDataNode->lUserExit
		 && MT_QUEUE_USER_ID_AI_START > pkQueueUserDataNode->lUserId)
		{
			mtQueueHall::DataRoom*	pkQueueHallDataRoom = m_pkQueueMgr->getQueueHall()->getRoom(pkQueueUserDataNode->lSpaceId, pkQueueUserDataNode->lRoomId);
			if (NULL != pkQueueHallDataRoom)
			{
				mtQueueDesk::DataNode * pkQueueDeskDataNode = (mtQueueDesk::DataNode *)pkQueueUserDataNode->pkQueueDeskDataNode;
				if(NULL != pkQueueDeskDataNode)
				{   
					EnterCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
				    pkQueueDeskDataNode->pkQueueUserDataNode[pkQueueUserDataNode->lUserDeskIndex] = NULL;
					pkQueueUserDataNode->pkQueueDeskDataNode = NULL;   
					LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
				}

				pkQueueUserDataNode->lUserExit  = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING;

				pkQueueUserDataNode->ullTimeEnterRun              = GetTickCount();
				pkQueueHallDataRoom->kQueueOneList.push(&pkQueueUserDataNode);
				
				return 1;
			}
		}
	}

	return 0;
}

int mtServiceRoomWaitStartRecv::playWaitStartRecvAi(mtQueueDesk::DataNode* pkQueueDeskDataNode, long lServiceType, int iQueueUserIdx)
{
	/// ��ai�Ѿ��ȴ���ʼ��
	if (1 == pkQueueDeskDataNode->plIsWaitStart[iQueueUserIdx])
	{
		return 1;
	}

	/// �����Լ�Ϊ�Ѿ���ʼ�ȴ�״̬
	pkQueueDeskDataNode->plIsWaitStart[iQueueUserIdx] = 1;

	/// ��������ϵ���ʵ��Ա�б�
	DataWrite kDataWrite;
	
	kDataWrite.lStructBytes      = sizeof(kDataWrite);
	kDataWrite.lUserId           = pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserIdx]->lUserId;
	kDataWrite.lServiceType      = lServiceType;

	mtQueuePacket::DataNode* pkQueuePacketDataNode;
	int iQueueUserDataNodeIdx;
	for (iQueueUserDataNodeIdx = 0; iQueueUserDataNodeIdx < MT_NODE_PLAYER; iQueueUserDataNodeIdx++)
	{
		if (NULL != pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx])
		{
			/// ���͡��ȴ���ʼ�� ��Ϣ
			//  ��ǰ��ʵ�û�
			if (mtQueueDesk::E_PLAYER_MODE_USER == pkQueueDeskDataNode->plPlayerMode[iQueueUserDataNodeIdx])
			{
				pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("waitstartrecv277");
				if (NULL != pkQueuePacketDataNode)
				{
					pkQueuePacketDataNode->kWSABuf.len  = kDataWrite.lStructBytes;
					memcpy(pkQueuePacketDataNode->pcData, &kDataWrite, kDataWrite.lStructBytes);
					mtPrint(&kDataWrite);
					m_pkQueueMgr->getQueueUser()->DataWrite(pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx], pkQueuePacketDataNode);
				}
			}
		}
	}

	return 1;
}

int mtServiceRoomWaitStartRecv::getQueueUserNextQueueUser(int iQueueUserCurIdx)
{
	if (0 == iQueueUserCurIdx)
	{
		return iQueueUserCurIdx + 2;
	}

	return iQueueUserCurIdx - 1;
}
