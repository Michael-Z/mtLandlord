#include "mtServiceRoomChangeDesk.h"
#include "mtServiceMgr.h"
#include "mtServiceRoomGrabLandlord.h"
#include "mtServiceRoomDoubleScore.h"
#include "mtPrint.h"

mtServiceRoomChangeDesk::mtServiceRoomChangeDesk()
{

}

mtServiceRoomChangeDesk::~mtServiceRoomChangeDesk()
{

}

int mtServiceRoomChangeDesk::init( void* pData )
{
	DataInit*	pkDataInit	= (DataInit*)pData;

	m_pkQueueMgr            = pkDataInit->pkQueueMgr;

	return	1;
}

int mtServiceRoomChangeDesk::run( DataRun* pkDataRun )
{
	DataRead* pkDataRead                              = (DataRead*)pkDataRun->pkQueuePacketDataNode->pcData;
	mtQueueUser::DataNode*      pkQueueUserDataNode	  = pkDataRun->pkQueueUserDataNode;
	mtPrint(pkDataRead);
	if (NULL == pkQueueUserDataNode)
	{
		return 0;
	}

	mtQueueHall::DataRoom*	pkQueueHallDataRoom       = m_pkQueueMgr->getQueueHall()->getRoom(pkQueueUserDataNode->lSpaceId
		                                                                                      , pkQueueUserDataNode->lRoomId);
	if (NULL == pkQueueHallDataRoom)
	{
		return 0;
	}

	mtQueuePacket::DataNode*	pkQueuePacketDataNode = pkDataRun->pkQueuePacketDataNode;
	mtQueueDesk::DataNode* pkQueueDeskDataNode        = (mtQueueDesk::DataNode*)pkQueueUserDataNode->pkQueueDeskDataNode;
	DataWrite  kDataWrite;
	memset(&kDataWrite, 0, sizeof(kDataWrite));
	kDataWrite.lStructBytes      = sizeof(kDataWrite);
	kDataWrite.lServiceType      = pkDataRead->lServiceType;
	kDataWrite.lUserId           = pkQueueUserDataNode->lUserId;
	if (NULL == pkQueueDeskDataNode)
	{
		/// �û���û������ �����ڵ��˶���
		pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("changdesk53");
		if (NULL != pkQueuePacketDataNode)
		{
			pkQueuePacketDataNode->kWSABuf.len    = kDataWrite.lStructBytes;
			memcpy (pkQueuePacketDataNode->pcData, &kDataWrite, kDataWrite.lStructBytes);
			mtPrint(&kDataWrite);
			m_pkQueueMgr->getQueueUser()->DataWrite(pkQueueUserDataNode, pkQueuePacketDataNode);
		}

		playRoomChangeDeskOne(pkQueueUserDataNode, pkQueueHallDataRoom);

		return 1;
	}

	EnterCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
	int		iUserIdx;
	mtQueueUser::DataNode*      pkQueueUserDataNodeTmp;

	for (iUserIdx = 0; iUserIdx < MT_NODE_PLAYER; iUserIdx++)
	{
		pkQueueUserDataNodeTmp = pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx];

		if (NULL != pkQueueUserDataNodeTmp && mtQueueDesk::E_PLAYER_MODE_USER == pkQueueDeskDataNode->plPlayerMode[iUserIdx])
		{
			pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("changedesk75");
			if (NULL != pkQueuePacketDataNode)
			{
				pkQueuePacketDataNode->kWSABuf.len    = kDataWrite.lStructBytes;
				memcpy (pkQueuePacketDataNode->pcData, &kDataWrite, kDataWrite.lStructBytes);
				mtPrint(&kDataWrite);
				m_pkQueueMgr->getQueueUser()->DataWrite(pkQueueUserDataNodeTmp, pkQueuePacketDataNode);
			}
		}
	}

	/// ���û����������Ӷ���
	if (2 == pkQueueDeskDataNode->lQueue)
	{
		runRoomChangeDeskTwo(pkQueueDeskDataNode, pkQueueUserDataNode, pkQueueHallDataRoom);
	}
	else if(3 == pkQueueDeskDataNode->lQueue) /// �û�����������
	{
		runRoomChangeDeskThree(pkQueueDeskDataNode, pkQueueUserDataNode, pkQueueHallDataRoom);
	}
	else /// �û��ڵ��˶���
	{
	}

	LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
	return	1;
}

int mtServiceRoomChangeDesk::getQueueUserNextQueueUser(int iQueueUserCurIdx)
{
	if (0 == iQueueUserCurIdx)
	{
		return iQueueUserCurIdx + 2;
	}

	return iQueueUserCurIdx - 1;
}

int mtServiceRoomChangeDesk::getQueueUserPreQueueUser(int iQueueUserCurIdx)
{
	if (2 == iQueueUserCurIdx)
	{
		return iQueueUserCurIdx - 2;
	}

	return iQueueUserCurIdx + 1;
}

int mtServiceRoomChangeDesk::playRoomChangeDeskOne(mtQueueUser::DataNode* pkQueueUserDataNode, mtQueueHall::DataRoom* pkQueueHallDataRoom)
{
	if (NULL != pkQueueUserDataNode && NULL == pkQueueUserDataNode->pkQueueDeskDataNode)
	{
		if (mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_ROUND_OVER == pkQueueUserDataNode->lUserExit
			&& MT_QUEUE_USER_ID_AI_START > pkQueueUserDataNode->lUserId)
		{
			pkQueueUserDataNode->lUserExit  = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING;
			pkQueueUserDataNode->ullTimeEnterRun  = GetTickCount();
			pkQueueHallDataRoom->kQueueOneList.push(&pkQueueUserDataNode);

			return 1;
		}
	}

	return 0;
}

int mtServiceRoomChangeDesk::runRoomChangeDeskTwo(mtQueueDesk::DataNode* pkQueueDeskDataNode,
	                                              mtQueueUser::DataNode* pkQueueUserDataNode,
											      mtQueueHall::DataRoom* pkQueueHallDataRoom)
{
	/// ����¼��û��������ϵ�λ��
	int iQueueUserDeskNextIdx = -1;
	if (0 == pkQueueUserDataNode->lUserDeskIndex)
	{
		iQueueUserDeskNextIdx = 1;
	}
	else
	{
		iQueueUserDeskNextIdx = 0;
	}

	//  �¼���ai�û�
	if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iQueueUserDeskNextIdx])
	{
		/// ����ai�ڵ�
		pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDeskNextIdx]->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;

		/// ���������Ӷ��У��Ƴ�����
		pkQueueDeskDataNode->lDeskExit  = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;

		/// ����������·��뵽���˶��еȴ�����������
		pkQueueUserDataNode->ullTimeEnterRun     = GetTickCount();
		pkQueueUserDataNode->lIsWaitStart        = 0;
	}
	else /// ���»ص����˶���
	{
		/// ���������Ӷ��У��Ƴ�����
		pkQueueDeskDataNode->lDeskExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;

		pkQueueUserDataNode->ullTimeEnterRun     = GetTickCount();
		pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDeskNextIdx]->ullTimeEnterRun     = GetTickCount();
	}

	return 1;
}

int mtServiceRoomChangeDesk::runRoomChangeDeskThree(mtQueueDesk::DataNode* pkQueueDeskDataNode, 
	                                                mtQueueUser::DataNode* pkQueueUserDataNode,
												    mtQueueHall::DataRoom* pkQueueHallDataRoom)
{
	/// ����¼��û��������ϵ�λ��
	int iQueueUserDeskNextIdx = getQueueUserNextQueueUser(pkQueueUserDataNode->lUserDeskIndex);

	/// ������¼��û��������ϵ�λ��
	int iQueueUserDeskNextNextIdx = getQueueUserNextQueueUser(iQueueUserDeskNextIdx);

	//  �¼���ai�û�
	if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iQueueUserDeskNextIdx])
	{
		//  ai�¼һ���ai�û�
		if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iQueueUserDeskNextNextIdx])
		{
			/// ���յ�һ��ai�ڵ�
			pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDeskNextIdx]->lUserExit     = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;

			/// ���յڶ���ai�ڵ�
			pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDeskNextNextIdx]->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;

			/// ���������Ӷ��У��Ƴ�����
			pkQueueDeskDataNode->lDeskExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;

			/// ����������·��뵽���˶��� �ȴ�����������
			pkQueueUserDataNode->lIsWaitStart                                              = 0;
			pkQueueUserDataNode->ullTimeEnterRun                                           = GetTickCount();
		}
		else /// ��Ҫ���»ص���������
		{
			/// ����������·��뵽���˶��� �ȴ�����������
			pkQueueUserDataNode->lUserExit                              = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_CHANGE;
			pkQueueUserDataNode->ullTimeEnterRun                        = GetTickCount();
			pkQueueUserDataNode->lIsWaitStart                           = 0;
		}
	}
	else
	{
		/// ����������·��뵽���˶��� �ȴ�����������
		pkQueueUserDataNode->lUserExit                              = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_CHANGE;
		pkQueueUserDataNode->ullTimeEnterRun                        = GetTickCount();
		pkQueueUserDataNode->lIsWaitStart                           = 0;
	}

	return 1;
}

int mtServiceRoomChangeDesk::exit()
{

	return	0;
}

