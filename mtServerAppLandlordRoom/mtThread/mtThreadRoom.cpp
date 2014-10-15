#include "mtThreadRoom.h"
#include "mtPlatform.h"
#include "mtService.h"
#include "mtServiceRoomDeskThree.h"
#include "mtServiceRoomDeskTwo.h"
#include "mtServiceRoomWaitStartRecv.h"
#include "mtServiceCardDeal.h"
#include "mtServiceRoomHeartBeat.h"

mtThreadRoom::mtThreadRoom() :m_hThread(INVALID_HANDLE_VALUE)
{
	memset (&m_kFdSet, 0, sizeof(m_kFdSet));
	memset (&m_kTimeVal, 0, sizeof(m_kTimeVal));
}

mtThreadRoom::~mtThreadRoom()
{

}

int mtThreadRoom::init( DataInit* pkInitData )
{
	memset (&m_kFdSet, 0, sizeof(m_kFdSet));
	m_kTimeVal.tv_usec		= 10;

	m_ullTimeEnterRun		= GetTickCount();

	m_pkQueueMgr            = pkInitData->pkQueueMgr;
	m_pkServiceMgr          = pkInitData->pkServiceMgr;
	m_pkQueueHallDataRoom   = pkInitData->pkQueueHallDataRoom;

	/// �����߳�
	m_hThread	= (HANDLE)_beginthread(CallBackRun, 0, (void*)this);
	MT_EXCEPTION_DEBUG (INVALID_HANDLE_VALUE == m_hThread, EXCEPTION__BEGINTHREAD);	

	m_eThreadState			= E_THREAD_STATE_RUN;

	return	1;
}

int mtThreadRoom::exit()
{
	if (INVALID_HANDLE_VALUE != m_hThread)
	{
		TerminateThread(m_hThread, 0);
		m_hThread	= INVALID_HANDLE_VALUE;
	}

	m_pkQueueHallDataRoom->kQueueOneList.exit();
	m_pkQueueHallDataRoom->kQueueTwoList.exit();
	m_pkQueueHallDataRoom->kQueueThreeList.exit();

	memset (&m_kFdSet, 0, sizeof(m_kFdSet));
	memset (&m_kTimeVal, 0, sizeof(m_kTimeVal));

	return	1;
}

int mtThreadRoom::exception()
{
	switch (m_iExceptionId)
	{
		MT_EXCETPION_STRING(EXCEPTION__BEGINTHREAD)
	default:
		MT_DEBUG(7, "���쳣û�б�����");
		return	0;
	}

	return	1;
}

int mtThreadRoom::run()
{
	m_ullTimeEnterRun		= GetTickCount();

	//Sleep(1000);
	/// ���˶�������
	//MT_EXCEPTION_DEBUG(m_pkQueueHallDataRoom->kQueueThreeList.bHaveRepeat(),EXCEPTION_BEG);
	runQueueThreeList();

	/// ���˶�������
    //MT_EXCEPTION_DEBUG(m_pkQueueHallDataRoom->kQueueTwoList.bHaveRepeat(),EXCEPTION_BEG);
	runQueueTwoList();

	/// ���˶���
    //MT_EXCEPTION_DEBUG(m_pkQueueHallDataRoom->kQueueOneList.bHaveRepeat(),EXCEPTION_BEG);
	runQueueOneList();

	return	1;
}
/// ���˶���
int mtThreadRoom::runQueueOneList()
{
	int iStartCardDeal = 0;
	mtQueueUser::DataNode*	 pkQueueUserDataNodeThree[MT_NODE_PLAYER] = {NULL,NULL,NULL};
	/// ���������ˣ���ʼ������
	if (0 < m_pkQueueHallDataRoom->kQueueOneList.pop(pkQueueUserDataNodeThree, MT_NODE_PLAYER))
	{
		/// ��������˶���ai
		if (   MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNodeThree[0]->lUserId
			&& MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNodeThree[1]->lUserId
			&& MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNodeThree[2]->lUserId)
		{
			/// ��������ѹ��
			pkQueueUserDataNodeThree[0]->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
			pkQueueUserDataNodeThree[1]->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
			pkQueueUserDataNodeThree[2]->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
			m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNodeThree[0]);
			m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNodeThree[1]);
			m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNodeThree[2]);
			///m_pkQueueHallDataRoom->kQueueOneList.push(pkQueueUserDataNodeThree, MT_NODE_PLAYER);
			return 1;
		}

		/// ����Ѿ����������˳��������������ɸѡ��
		if (runFilterUserExitOneList(m_pkQueueHallDataRoom, pkQueueUserDataNodeThree, MT_NODE_PLAYER))
		{
			return 1;
		}

		mtQueuePacket::DataNode* pkQueuePacketDataNode = NULL;
		pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("threadroom119");
		if (NULL != pkQueuePacketDataNode)
		{
			/// ��������
			mtQueueDesk::DataNode* pkQueueDeskDataNode = m_pkQueueMgr->getQueueDesk()->pop();
			if (NULL != pkQueueDeskDataNode)
			{
				EnterCriticalSection(&pkQueueDeskDataNode->kCriticalSection);

				pkQueueDeskDataNode->lDeskExit                 = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING;
				pkQueueDeskDataNode->lQueue                    = 0;
				pkQueueDeskDataNode->plPlayerMode[0]		   = 0;
				pkQueueDeskDataNode->plPlayerMode[1]		   = 0;
				pkQueueDeskDataNode->plPlayerMode[2]		   = 0;
				pkQueueDeskDataNode->plIsWaitStart[0]		   = 0;
				pkQueueDeskDataNode->plIsWaitStart[1]		   = 0;
				pkQueueDeskDataNode->plIsWaitStart[2]		   = 0;
				pkQueueDeskDataNode->pkQueueUserDataNode[0]    = NULL;
				pkQueueDeskDataNode->pkQueueUserDataNode[1]    = NULL;
				pkQueueDeskDataNode->pkQueueUserDataNode[2]    = NULL;

				long iPlayerIdx;
				for (iPlayerIdx = 0; iPlayerIdx < MT_NODE_PLAYER; iPlayerIdx++)
				{
					pkQueueDeskDataNode->plPlayerMode[iPlayerIdx]               = 0;
					pkQueueDeskDataNode->plPlayerType[iPlayerIdx]               = mtQueueDesk::E_PLAYER_TYPE_FARMER;
					pkQueueUserDataNodeThree[iPlayerIdx]->lUserDeskIndex        = iPlayerIdx;
					pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]        = pkQueueUserDataNodeThree[iPlayerIdx];
					pkQueueUserDataNodeThree[iPlayerIdx]->pkQueueDeskDataNode	= pkQueueDeskDataNode;
					pkQueueDeskDataNode->plIsWaitStart[iPlayerIdx]              = pkQueueUserDataNodeThree[iPlayerIdx]->lIsWaitStart;

					/// ��ǰ�������ai
					if (MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNodeThree[iPlayerIdx]->lUserId)
					{
						pkQueueDeskDataNode->plPlayerMode[iPlayerIdx]           = mtQueueDesk::E_PLAYER_MODE_AI;
					}
				}

				/// ��Ӧ�û������ӳɹ�
				iStartCardDeal = 1;

				runQueueThreeListDesk(m_pkQueueHallDataRoom, pkQueueDeskDataNode, pkQueuePacketDataNode);
				LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
				if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[2])
				{
					runWaitStartRecvAi(pkQueueDeskDataNode->pkQueueUserDataNode[2]);
				}

				EnterCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
				/// �Ƿ�ʼ����
				if (NULL != pkQueueDeskDataNode
					&& 1 == iStartCardDeal
					&& 0 == pkQueueDeskDataNode->lCardDeal
					&& 1 == pkQueueDeskDataNode->plIsWaitStart[0]
					&& 1 == pkQueueDeskDataNode->plIsWaitStart[1]
					&& 1 == pkQueueDeskDataNode->plIsWaitStart[2])
				{
					pkQueueDeskDataNode->lCardDeal = 1;
					LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);

					runStartCardDeal(pkQueueDeskDataNode->pkQueueUserDataNode[0]);
				}
				else
				{
					LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
				}
			}
			else
			{
				/// ʧ������ѹ��
				m_pkQueueMgr->getQueuePacket()->push(pkQueuePacketDataNode);
				
				/// ��������ѹ��
				m_pkQueueHallDataRoom->kQueueOneList.push(pkQueueUserDataNodeThree, MT_NODE_PLAYER);
			}
		}
		else
		{
			/// ʧ������ѹ�ص��˶���
			m_pkQueueHallDataRoom->kQueueOneList.push(pkQueueUserDataNodeThree, MT_NODE_PLAYER);
		}
	}
	else  /// û������������
	{
		int iQueueNodeNumUsed = m_pkQueueHallDataRoom->kQueueOneList.getNodeNumUsed();

		/// ֻ��һ����
		if (1 == iQueueNodeNumUsed)
		{
			mtQueueUser::DataNode*	 pkQueueUserDataNode = NULL;
			if (0 < m_pkQueueHallDataRoom->kQueueOneList.pop(&pkQueueUserDataNode))
			{
				/// ����Ѿ����������˳��������������ɸѡ��
				pkQueueUserDataNodeThree[0] = pkQueueUserDataNode;
				if (runFilterUserExitOneList(m_pkQueueHallDataRoom, pkQueueUserDataNodeThree, (MT_NODE_PLAYER - 2)))
				{
					return 1;
				}

				/// ���������ʵ���û��˽ڵ�
				if (MT_QUEUE_USER_ID_AI_START > pkQueueUserDataNode->lUserId)
				{
					/// ������һ���˽��뵥�˶��еĳ�ʱ���������Ԥ��ֵ�����ai�����˶���
					if (MT_NODE_QUEUE_TIMEOUT <= (GetTickCount() - pkQueueUserDataNode->ullTimeEnterRun))
					{
						/// ����һ��ai�ڵ�
						/// �����û����
						mtQueueUser::DataNode*		pkQueueUserDataNodeAi = (mtQueueUser::DataNode*)m_pkQueueMgr->getQueueUser()->pop();
						if (pkQueueUserDataNodeAi)
						{
							initQueueUserDataNodeAi(pkQueueUserDataNodeAi,pkQueueUserDataNode->lRoomId);
							pkQueueUserDataNodeAi->lSpaceId      = pkQueueUserDataNode->lSpaceId;
							pkQueueUserDataNodeAi->lRoomId       = pkQueueUserDataNode->lRoomId;
							pkQueueUserDataNodeAi->pkListNode[mtQueueUser::E_LIST_NODE_ONE].pSelf = NULL;
							pkQueueUserDataNodeAi->pkListNode[mtQueueUser::E_LIST_NODE_ONE].pNext = NULL;
							pkQueueUserDataNodeAi->pkListNode[mtQueueUser::E_LIST_NODE_ONE].pPre  = NULL;

							/// ����ѹ�ص��˶���
							m_pkQueueHallDataRoom->kQueueOneList.push(&pkQueueUserDataNode);

							/// ���뵥�˶���
							m_pkQueueHallDataRoom->kQueueOneList.push(&pkQueueUserDataNodeAi);
						}
					}
					else
					{
						/// δ��ʱ ����ѹ�ص��˶���

						m_pkQueueHallDataRoom->kQueueOneList.push(&pkQueueUserDataNode);
					}
				}
				else
				{
					/// ��Ai
					m_pkQueueHallDataRoom->kQueueOneList.push(&pkQueueUserDataNode);
				}
			}
		}
		else if (2 == iQueueNodeNumUsed)  /// ֻ��������
		{
			/// ���������ˣ���ʼ������
			if (0 < m_pkQueueHallDataRoom->kQueueOneList.pop(pkQueueUserDataNodeThree, MT_NODE_PLAYER - 1))
			{
				/// ��������˶���ai
				if (MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNodeThree[0]->lUserId
					&& MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNodeThree[1]->lUserId)
				{
					/// ��������ѹ��
					pkQueueUserDataNodeThree[0]->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
					pkQueueUserDataNodeThree[1]->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
					m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNodeThree[0]);
					m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNodeThree[1]);
					/// m_pkQueueHallDataRoom->kQueueOneList.push(pkQueueUserDataNodeThree, MT_NODE_PLAYER - 1);
					return 1;
				}

				/// ����Ѿ����������˳��������������ɸѡ��
				if (runFilterUserExitOneList(m_pkQueueHallDataRoom, pkQueueUserDataNodeThree, (MT_NODE_PLAYER - 1)))
				{
					return 1;
				}

				mtQueuePacket::DataNode* pkQueuePacketDataNode = NULL;
				pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("threadroom276");
				if (NULL != pkQueuePacketDataNode)
				{
					/// ��������
					mtQueueDesk::DataNode* pkQueueDeskDataNode = m_pkQueueMgr->getQueueDesk()->pop();

					if (NULL != pkQueueDeskDataNode)
					{
						EnterCriticalSection(&pkQueueDeskDataNode->kCriticalSection);

						pkQueueDeskDataNode->lDeskExit              = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING;
						pkQueueDeskDataNode->lQueue                 = 0;
						pkQueueDeskDataNode->plPlayerMode[0]		   = 0;
						pkQueueDeskDataNode->plPlayerMode[1]		   = 0;
						pkQueueDeskDataNode->plPlayerMode[2]		   = 0;
						pkQueueDeskDataNode->plIsWaitStart[0]		   = 0;
						pkQueueDeskDataNode->plIsWaitStart[1]		   = 0;
						pkQueueDeskDataNode->plIsWaitStart[2]		   = 0;
						pkQueueDeskDataNode->pkQueueUserDataNode[0] = NULL;
						pkQueueDeskDataNode->pkQueueUserDataNode[1] = NULL;
						pkQueueDeskDataNode->pkQueueUserDataNode[2] = NULL;

						long iPlayerIdx;

						for (iPlayerIdx = 0; iPlayerIdx < (MT_NODE_PLAYER - 1); iPlayerIdx++)
						{
							pkQueueDeskDataNode->plPlayerMode[iPlayerIdx]               = 0;
							pkQueueDeskDataNode->plPlayerType[iPlayerIdx]               = mtQueueDesk::E_PLAYER_TYPE_FARMER;
							pkQueueUserDataNodeThree[iPlayerIdx]->lUserDeskIndex        = iPlayerIdx;
							pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]        = pkQueueUserDataNodeThree[iPlayerIdx];
							pkQueueUserDataNodeThree[iPlayerIdx]->pkQueueDeskDataNode	= pkQueueDeskDataNode;
							pkQueueDeskDataNode->plIsWaitStart[iPlayerIdx]              = pkQueueUserDataNodeThree[iPlayerIdx]->lIsWaitStart;

							/// ��ǰ�������ai
							if (MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNodeThree[iPlayerIdx]->lUserId)
							{
								pkQueueDeskDataNode->plPlayerMode[iPlayerIdx]           = mtQueueDesk::E_PLAYER_MODE_AI;
							}
						}

						/// ��Ӧ�û������ӳɹ�
						runQueueTwoListDesk(m_pkQueueHallDataRoom, pkQueueDeskDataNode, pkQueuePacketDataNode);

						LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);

						if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[1])
						{
							runWaitStartRecvAi(pkQueueDeskDataNode->pkQueueUserDataNode[1]);
						}
					}
					else
					{
						/// ʧ������ѹ��
						m_pkQueueMgr->getQueuePacket()->push(pkQueuePacketDataNode);

						/// ��������ѹ��
						m_pkQueueHallDataRoom->kQueueOneList.push(pkQueueUserDataNodeThree, MT_NODE_PLAYER - 1);
					}
				}
				else
				{
					/// ʧ������ѹ�ص��˶���
					m_pkQueueHallDataRoom->kQueueOneList.push(pkQueueUserDataNodeThree, MT_NODE_PLAYER - 1);
				}
			}
		}

		/// ������������
		int iSpaceIdx;
		int iRoomIdx;
		mtQueueHall*           pkQueueHall           = m_pkQueueMgr->getQueueHall();
		mtQueueHall::DataSpace* pkQueueHallDataSpace = NULL;
		mtQueueHall::DataRoom* pkQueueHallDataRoom   = NULL;
		for (iSpaceIdx = 0; iSpaceIdx < pkQueueHall->m_lSpaceTotal; iSpaceIdx++)
		{
			pkQueueHallDataSpace = pkQueueHall->getSpace(iSpaceIdx);

			for (iRoomIdx = 0; iRoomIdx < pkQueueHallDataSpace->lRomTotal; iRoomIdx++)
			{
				pkQueueHallDataRoom = pkQueueHall->getRoom(iSpaceIdx, iRoomIdx);

				if (MT_NODE_QUEUE_HELP_TWO_LIST <= pkQueueHallDataRoom->kQueueOneList.getNodeNumUsed())
				{
					/// �з�����Ҫ��æ
					return runRoomHelpTwoList(pkQueueHallDataRoom);
				}
			}
		}
	}

	return 1;
}
/// ���˶�������
int mtThreadRoom::runQueueTwoList()
{
	mtQueueDesk::DataNode*   pkQueueDeskDataNode = NULL;
	int iThreeDeskFlag = 0;
	int iStartCardDeal = 0;
	if (0 < m_pkQueueHallDataRoom->kQueueTwoList.pop(&pkQueueDeskDataNode))
	{
		EnterCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
		/// �ж������Ƿ��Ѿ��˳�
		if (runFilterUserExitTwoList(m_pkQueueHallDataRoom, pkQueueDeskDataNode))
		{
			LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
			return 1;
		}

		/// ���˶���û���ˣ�������Ҫ��ai
		if (0 >= m_pkQueueHallDataRoom->kQueueOneList.getNodeNumUsed())
		{
			/// �ȴ���ʱ������ai ��
			if (MT_NODE_QUEUE_TIMEOUT <= (GetTickCount() - pkQueueDeskDataNode->ullTimeEnterRun))
			{
				/// ����һ��ai�ڵ㣬�������˶���
				/// �����û����
					mtQueueUser::DataNode*		pkQueueUserDataNodeAi = (mtQueueUser::DataNode*)m_pkQueueMgr->getQueueUser()->pop();
				if (NULL != pkQueueUserDataNodeAi)
				{
				   initQueueUserDataNodeAi(pkQueueUserDataNodeAi,pkQueueDeskDataNode->pkQueueUserDataNode[0]->lRoomId);
					pkQueueUserDataNodeAi->lSpaceId      = pkQueueDeskDataNode->pkQueueUserDataNode[0]->lSpaceId;
					pkQueueUserDataNodeAi->lRoomId       = pkQueueDeskDataNode->pkQueueUserDataNode[0]->lRoomId;
					pkQueueUserDataNodeAi->pkListNode[mtQueueUser::E_LIST_NODE_ONE].pSelf = NULL;
					pkQueueUserDataNodeAi->pkListNode[mtQueueUser::E_LIST_NODE_ONE].pNext = NULL;
					pkQueueUserDataNodeAi->pkListNode[mtQueueUser::E_LIST_NODE_ONE].pPre  = NULL;

					mtQueuePacket::DataNode* pkQueuePacketDataNode = NULL;
					pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("threadroom397");
					if (NULL == pkQueuePacketDataNode)
					{
						/// ��������ѹ�ض���
						m_pkQueueHallDataRoom->kQueueTwoList.push(&pkQueueDeskDataNode);

						/// ����ai�û��ڵ�
						m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNodeAi);

						LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
						return 0;
					}

					long iPlayerIdx = 2;
					pkQueueDeskDataNode->plPlayerType[iPlayerIdx]        = mtQueueDesk::E_PLAYER_TYPE_FARMER;
					pkQueueDeskDataNode->plPlayerMode[iPlayerIdx]        = 1;
					pkQueueUserDataNodeAi->lUserDeskIndex                = iPlayerIdx;
					pkQueueUserDataNodeAi->pkQueueDeskDataNode	         = pkQueueDeskDataNode;
					pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx] = pkQueueUserDataNodeAi;

					/// ��Ӧ�û������ӳɹ�
					iStartCardDeal = 1;

					runQueueThreeListDesk(m_pkQueueHallDataRoom, pkQueueDeskDataNode, pkQueuePacketDataNode);
					iThreeDeskFlag = 1;
				}
				else
				{
					/// ��������ѹ�ض���
					m_pkQueueHallDataRoom->kQueueTwoList.push(&pkQueueDeskDataNode);
				}
			}
			else  /// �ȴ�û�г�ʱ��������������ѹ�ض���
			{
				/// ��������ѹ�ض���
				m_pkQueueHallDataRoom->kQueueTwoList.push(&pkQueueDeskDataNode);
			}
		}
		else  /// ���˶�������
		{
			mtQueuePacket::DataNode* pkQueuePacketDataNode = NULL;
			pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("threadroom437");
			if (NULL == pkQueuePacketDataNode)
			{
				/// ��������ѹ�ض���
				m_pkQueueHallDataRoom->kQueueTwoList.push(&pkQueueDeskDataNode);

				LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
				return 0;

			}

			/// �ӵ��˶��е���һ����
			mtQueueUser::DataNode*	 pkQueueUserDataNode = NULL;
			m_pkQueueHallDataRoom->kQueueOneList.pop(&pkQueueUserDataNode);
			if (NULL != pkQueueUserDataNode)
			{
				/// ������Ƿ��Ѿ��˳���
				if (mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING != pkQueueUserDataNode->lUserExit)
				{
					if (MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNode->lUserId)
					{
						pkQueueUserDataNode->lUserExit   = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
						m_pkQueueHallDataRoom->kQueueOneList.push(&pkQueueUserDataNode);
					}


					/// ��������ѹ�ض���
					m_pkQueueHallDataRoom->kQueueTwoList.push(&pkQueueDeskDataNode);

					m_pkQueueMgr->getQueuePacket()->push(pkQueuePacketDataNode);
					LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
					return 1;
				}

				/// ��ͬһ���˺��ڵ�¼(ǿ���˳�������ֿ��ٽ���)
				if (pkQueueUserDataNode->lUserId == pkQueueDeskDataNode->pkQueueUserDataNode[0]->lUserId
				 || pkQueueUserDataNode->lUserId == pkQueueDeskDataNode->pkQueueUserDataNode[1]->lUserId)
				{
					/// ������������ѹ��
					m_pkQueueHallDataRoom->kQueueTwoList.push(&pkQueueDeskDataNode);

					/// ����һ��ai�ڵ�
					/// �����û����          
					mtQueueUser::DataNode*		pkQueueUserDataNodeAi = (mtQueueUser::DataNode*)m_pkQueueMgr->getQueueUser()->pop();
					if (pkQueueUserDataNodeAi)
					{
						initQueueUserDataNodeAi(pkQueueUserDataNodeAi,pkQueueUserDataNode->lRoomId);
						pkQueueUserDataNodeAi->lSpaceId      = pkQueueDeskDataNode->pkQueueUserDataNode[0]->lSpaceId;
						pkQueueUserDataNodeAi->lRoomId       = pkQueueDeskDataNode->pkQueueUserDataNode[0]->lRoomId;
						pkQueueUserDataNodeAi->pkListNode[mtQueueUser::E_LIST_NODE_ONE].pSelf = NULL;
						pkQueueUserDataNodeAi->pkListNode[mtQueueUser::E_LIST_NODE_ONE].pNext = NULL;
						pkQueueUserDataNodeAi->pkListNode[mtQueueUser::E_LIST_NODE_ONE].pPre  = NULL;

						/// ���뵥�˶���
						m_pkQueueHallDataRoom->kQueueOneList.push(&pkQueueUserDataNodeAi);
					}

					/// ������ʵ�˶���
					m_pkQueueHallDataRoom->kQueueOneList.push(&pkQueueUserDataNode);
					LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
					return 1;
				}

				long iPlayerIdx = 2;
				pkQueueDeskDataNode->plPlayerType[iPlayerIdx]        = mtQueueDesk::E_PLAYER_TYPE_FARMER;
				pkQueueDeskDataNode->plPlayerMode[iPlayerIdx]        = 0;

				/// ��ǰ�������ai
				if (MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNode->lUserId)
				{
					pkQueueDeskDataNode->plPlayerMode[iPlayerIdx]     = mtQueueDesk::E_PLAYER_MODE_AI;;
				}

				pkQueueUserDataNode->lUserDeskIndex                   = iPlayerIdx;
				pkQueueUserDataNode->pkQueueDeskDataNode	          = pkQueueDeskDataNode;
				pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]  = pkQueueUserDataNode;
				pkQueueDeskDataNode->plIsWaitStart[iPlayerIdx]        = pkQueueUserDataNode->lIsWaitStart;

				/// ��Ӧ�û������ӳɹ�
				iStartCardDeal = 1;
				runQueueThreeListDesk(m_pkQueueHallDataRoom, pkQueueDeskDataNode, pkQueuePacketDataNode);
				iThreeDeskFlag = 1;
			}
			else
			{
				/// ��������ѹ�ض���
				m_pkQueueHallDataRoom->kQueueTwoList.push(&pkQueueDeskDataNode);

				/// ʧ������ѹ��
				m_pkQueueMgr->getQueuePacket()->push(pkQueuePacketDataNode);
			}
		}

		LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
		if (1 == iThreeDeskFlag && mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[2])
		{
			runWaitStartRecvAi(pkQueueDeskDataNode->pkQueueUserDataNode[2]);
		}

		/// �Ƿ�ʼ����
		EnterCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
		if (pkQueueDeskDataNode
		 && 1 == iStartCardDeal
		 && 0 == pkQueueDeskDataNode->lCardDeal
		 && 1 == pkQueueDeskDataNode->plIsWaitStart[0]
		 && 1 == pkQueueDeskDataNode->plIsWaitStart[1]
		 && 1 == pkQueueDeskDataNode->plIsWaitStart[2])
		{
			pkQueueDeskDataNode->lCardDeal = 1;
			LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
			runStartCardDeal(pkQueueDeskDataNode->pkQueueUserDataNode[0]);
		}
		else
		{
			LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
		}
	}

	return 1;
}
/// ���˶�������
int mtThreadRoom::runQueueThreeList()
{
	mtQueueDesk::DataNode*   pkQueueDeskDataNode = NULL;
	if (0 < m_pkQueueHallDataRoom->kQueueThreeList.pop(&pkQueueDeskDataNode))
	{
		EnterCriticalSection(&pkQueueDeskDataNode->kCriticalSection);

		/// �ж������Ƿ��Ѿ��˳�
		if (!runFilterUserExitThreeList(m_pkQueueHallDataRoom, pkQueueDeskDataNode))
		{
			m_pkQueueHallDataRoom->kQueueThreeList.push(&pkQueueDeskDataNode);
		}
		LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
	}
	else
	{
// 		if (MT_NODE_QUEUE_HELP_THREE_LIST > m_pkQueueHallDataRoom->kQueueThreeList.getNodeNumUsed())
// 		{
// 			/// ������������
// 			int iSpaceIdx;
// 			int iRoomIdx;
// 			mtQueueHall*           pkQueueHall           = m_pkQueueMgr->getQueueHall();
// 			mtQueueHall::DataSpace* pkQueueHallDataSpace = NULL;
// 			mtQueueHall::DataRoom* pkQueueHallDataRoom   = NULL;
// 			for (iSpaceIdx = 0; iSpaceIdx < pkQueueHall->m_lSpaceTotal; iSpaceIdx++)
// 			{
// 				pkQueueHallDataSpace = pkQueueHall->getSpace(iSpaceIdx);
// 
// 				for (iRoomIdx = 0; iRoomIdx < pkQueueHallDataSpace->lRomTotal; iRoomIdx++)
// 				{
// 					pkQueueHallDataRoom = pkQueueHall->getRoom(iSpaceIdx, iRoomIdx);
// 
// 					if (MT_NODE_QUEUE_HELP_THREE_LIST * 2 < pkQueueHallDataRoom->kQueueThreeList.getNodeNumUsed())
// 					{
// 						/// �з�����Ҫ��æ
// 						return runRoomHelpThreeList(pkQueueHallDataRoom);
// 					}
// 				}
// 			}
// 		}
	}

	return 1;
}

int mtThreadRoom::runQueueTwoListDesk(mtQueueHall::DataRoom*  pkQueueHallDataRoom, mtQueueDesk::DataNode* pkQueueDeskDataNode, mtQueuePacket::DataNode* pkQueuePacketDataNode)
{
	/// ѹ������������Ϣ
	pkQueueDeskDataNode->ullTimeEnterRun  = GetTickCount();
	pkQueueDeskDataNode->lQueue           = 2;
	pkQueueDeskDataNode->lCardDeal        = 0;

	pkQueueHallDataRoom->kQueueTwoList.push(&pkQueueDeskDataNode);

	/// ��Ӧ������������
	mtServiceRoomDeskTwo::DataRead* pkDataRead = (mtServiceRoomDeskTwo::DataRead*)pkQueuePacketDataNode->pcData;

	pkDataRead->lStructBytes           = sizeof(mtServiceRoomDeskTwo::DataRead);
	pkDataRead->lServiceType           = mtServiceMgr::E_SERVICE_ROOM_DESK_TWO;
// 	pkDataRead->pkQueueUserDataNode[0] = pkQueueDeskDataNode->pkQueueUserDataNode[0];
// 	pkDataRead->pkQueueUserDataNode[1] = pkQueueDeskDataNode->pkQueueUserDataNode[1];

	mtService::DataRun kDataRun;
	kDataRun.pkQueuePacketDataNode	= pkQueuePacketDataNode;
	kDataRun.pkQueueUserDataNode	= pkQueueDeskDataNode->pkQueueUserDataNode[0];

	m_pkServiceMgr->run(&kDataRun);
	if (pkQueueDeskDataNode->pkQueueUserDataNode[0])
	{
		pkQueueDeskDataNode->plIsWaitStart[0] = pkQueueDeskDataNode->pkQueueUserDataNode[0]->lIsWaitStart;
	}

	if (pkQueueDeskDataNode->pkQueueUserDataNode[1])
	{
		pkQueueDeskDataNode->plIsWaitStart[1] = pkQueueDeskDataNode->pkQueueUserDataNode[1]->lIsWaitStart;
	}

	return 1;
}

int mtThreadRoom::runQueueThreeListDesk(mtQueueHall::DataRoom*  pkQueueHallDataRoom, mtQueueDesk::DataNode* pkQueueDeskDataNode, mtQueuePacket::DataNode* pkQueuePacketDataNode)
{
	/// ѹ������������Ϣ
	pkQueueDeskDataNode->ullTimeEnterRun  = GetTickCount();
	pkQueueDeskDataNode->lQueue           = 3;
	pkQueueDeskDataNode->lCardDeal        = 0;
	pkQueueHallDataRoom->kQueueThreeList.push(&pkQueueDeskDataNode);

	/// ��Ӧ������������
	mtServiceRoomDeskThree::DataRead* pkDataRead = (mtServiceRoomDeskThree::DataRead*)pkQueuePacketDataNode->pcData;

	pkDataRead->lStructBytes           = sizeof(mtServiceRoomDeskThree::DataRead);
	pkDataRead->lServiceType           = mtServiceMgr::E_SERVICE_ROOM_DESK_THREE;

	mtService::DataRun kDataRun;
	kDataRun.pkQueuePacketDataNode	= pkQueuePacketDataNode;
	kDataRun.pkQueueUserDataNode	= pkQueueDeskDataNode->pkQueueUserDataNode[0];

	m_pkServiceMgr->run(&kDataRun);

	if (NULL != pkQueueDeskDataNode->pkQueueUserDataNode[0])
	{
		pkQueueDeskDataNode->plIsWaitStart[0] = pkQueueDeskDataNode->pkQueueUserDataNode[0]->lIsWaitStart;
	}

	if (NULL != pkQueueDeskDataNode->pkQueueUserDataNode[1])
	{
		pkQueueDeskDataNode->plIsWaitStart[1] = pkQueueDeskDataNode->pkQueueUserDataNode[1]->lIsWaitStart;
	}

	if (NULL != pkQueueDeskDataNode->pkQueueUserDataNode[2])
	{
		pkQueueDeskDataNode->plIsWaitStart[2] = pkQueueDeskDataNode->pkQueueUserDataNode[2]->lIsWaitStart;
	}

	return 1;
}

int mtThreadRoom::runRoomHelpTwoList(mtQueueHall::DataRoom*  pkQueueHallDataRoom)
{
	int iStartCardDeal = 0;
	mtQueueUser::DataNode*	 pkQueueUserDataNodeThree[MT_NODE_PLAYER];
	if (0 < pkQueueHallDataRoom->kQueueOneList.pop(pkQueueUserDataNodeThree, MT_NODE_PLAYER))
	{
		mtQueuePacket::DataNode* pkQueuePacketDataNode = NULL;
		pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("threadroom687");
		if (NULL != pkQueuePacketDataNode)
		{
			/// ��������
			mtQueueDesk::DataNode* pkQueueDeskDataNode = m_pkQueueMgr->getQueueDesk()->pop();
			if (NULL != pkQueueDeskDataNode)
			{
				EnterCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
				pkQueueDeskDataNode->lDeskExit              = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING;
				pkQueueDeskDataNode->lQueue                 = 0;
				pkQueueDeskDataNode->plPlayerMode[0]		   = 0;
				pkQueueDeskDataNode->plPlayerMode[1]		   = 0;
				pkQueueDeskDataNode->plPlayerMode[2]		   = 0;
				pkQueueDeskDataNode->plIsWaitStart[0]		   = 0;
				pkQueueDeskDataNode->plIsWaitStart[1]		   = 0;
				pkQueueDeskDataNode->plIsWaitStart[2]		   = 0;
				pkQueueDeskDataNode->pkQueueUserDataNode[0] = NULL;
				pkQueueDeskDataNode->pkQueueUserDataNode[1] = NULL;
				pkQueueDeskDataNode->pkQueueUserDataNode[2] = NULL;

				long iPlayerIdx;
				for (iPlayerIdx = 0; iPlayerIdx < MT_NODE_PLAYER; iPlayerIdx++)
				{
					pkQueueDeskDataNode->plPlayerMode[iPlayerIdx]               = 0;
					pkQueueDeskDataNode->plPlayerType[iPlayerIdx]               = mtQueueDesk::E_PLAYER_TYPE_FARMER;
					pkQueueUserDataNodeThree[iPlayerIdx]->lUserDeskIndex        = iPlayerIdx;
					pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]        = pkQueueUserDataNodeThree[iPlayerIdx];
					pkQueueUserDataNodeThree[iPlayerIdx]->pkQueueDeskDataNode	= pkQueueDeskDataNode;
					pkQueueDeskDataNode->plIsWaitStart[iPlayerIdx]              = pkQueueUserDataNodeThree[iPlayerIdx]->lIsWaitStart;

					/// ��ǰ�������ai
					if (MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNodeThree[iPlayerIdx]->lUserId)
					{
						pkQueueDeskDataNode->plPlayerMode[iPlayerIdx]           = mtQueueDesk::E_PLAYER_MODE_AI;;
					}
				}
				/// ��Ӧ�û������ӳɹ�
				iStartCardDeal = 1;

				runQueueThreeListDesk(pkQueueHallDataRoom, pkQueueDeskDataNode, pkQueuePacketDataNode);
				LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
				if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[2])
				{
					runWaitStartRecvAi(pkQueueDeskDataNode->pkQueueUserDataNode[2]);
				}    

				/// �Ƿ�ʼ����
				EnterCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
				if (pkQueueDeskDataNode
					&& 1 == iStartCardDeal
					&& 0 == pkQueueDeskDataNode->lCardDeal
					&& 1 == pkQueueDeskDataNode->plIsWaitStart[0]
					&& 1 == pkQueueDeskDataNode->plIsWaitStart[1]
					&& 1 == pkQueueDeskDataNode->plIsWaitStart[2])
				{
					pkQueueDeskDataNode->lCardDeal = 1;
					LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);

					runStartCardDeal(pkQueueDeskDataNode->pkQueueUserDataNode[0]);
				}
				else
				{
					LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
				}
			}
			else
			{
				/// ʧ������ѹ��
				m_pkQueueMgr->getQueuePacket()->push(pkQueuePacketDataNode);

				/// ��
				pkQueueHallDataRoom->kQueueOneList.push(pkQueueUserDataNodeThree, MT_NODE_PLAYER);
			}
		}
		else
		{
			/// ʧ������ѹ�ص��˶���
			pkQueueHallDataRoom->kQueueOneList.push(pkQueueUserDataNodeThree, MT_NODE_PLAYER);
		}
	}

	return 1;
 }

int mtThreadRoom::runFilterUserExitOneList(mtQueueHall::DataRoom* pkQueueHallDataRoom, mtQueueUser::DataNode** pkQueueUserDataNodes, int iUserCount)
{
	long iHasUserExit = 0;
	long iPlayerIdx;
	//�û��ڵ�ȥ�ظ�����
	if (pkQueueUserDataNodes[0] && pkQueueUserDataNodes[1] && pkQueueUserDataNodes[2])
	{
		if (3 == iUserCount)
		{
			if (pkQueueUserDataNodes[0] == pkQueueUserDataNodes[1]
				 || pkQueueUserDataNodes[0]->lUserId == pkQueueUserDataNodes[1]->lUserId)
			{
				pkQueueUserDataNodes[0]  = NULL;
				m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNodes[1]);
				m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNodes[2]);
			}
			else if (pkQueueUserDataNodes[0] == pkQueueUserDataNodes[2]
			      || pkQueueUserDataNodes[0]->lUserId == pkQueueUserDataNodes[2]->lUserId)
			{
				pkQueueUserDataNodes[0]  = NULL;
				m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNodes[1]);
				m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNodes[2]);
			}
			else if (pkQueueUserDataNodes[1] == pkQueueUserDataNodes[2]
			      || pkQueueUserDataNodes[1]->lUserId == pkQueueUserDataNodes[2]->lUserId)
			{
				pkQueueUserDataNodes[2]  = NULL;
				m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNodes[0]);
				m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNodes[1]);
			}
		}
		else if (2 == iUserCount)
		{
			if (pkQueueUserDataNodes[0] == pkQueueUserDataNodes[1]
			|| pkQueueUserDataNodes[0]->lUserId == pkQueueUserDataNodes[1]->lUserId)
			{
				pkQueueUserDataNodes[0]  = NULL;
				m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNodes[1]);
			}
		}
	}

	mtQueueUser::DataNode* pkQueueUserDataNode;
	for (iPlayerIdx = 0; iPlayerIdx < iUserCount; iPlayerIdx++)
	{
		pkQueueUserDataNode = pkQueueUserDataNodes[iPlayerIdx];
		if (pkQueueUserDataNode)
		{
			if (mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING != pkQueueUserDataNode->lUserExit
				&& mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_CHANGE !=  pkQueueUserDataNode->lUserExit
				&& mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_ROUND_OVER != pkQueueUserDataNode->lUserExit)
			{
				iHasUserExit  = 1;
				/// �����ai
				if (MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNode->lUserId)
				{
					pkQueueUserDataNode->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
					m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNodes[iPlayerIdx]);
				}

				pkQueueUserDataNodes[iPlayerIdx]  = NULL;
			}
			else
			{
				if (mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_CHANGE == pkQueueUserDataNode->lUserExit)
				{
					pkQueueUserDataNode->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING;
				}

				if (mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_ROUND_OVER == pkQueueUserDataNode->lUserExit)
				{
					iHasUserExit                      = 1;
					if (MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNode->lUserId)
					{
						pkQueueUserDataNode->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
						m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNodes[iPlayerIdx]);
					}
					else		
					{
						pkQueueUserDataNodes[iPlayerIdx]  = NULL;
					}
				}
			}
		}
		else
		{
			iHasUserExit                      = 1;
			pkQueueUserDataNodes[iPlayerIdx]  = NULL;
		}
	}

	if (iHasUserExit)
	{
		for (iPlayerIdx = 0; iPlayerIdx < iUserCount; iPlayerIdx++)
		{
			if (NULL != pkQueueUserDataNodes[iPlayerIdx])
			{
				/// ��������ѹ��
				pkQueueHallDataRoom->kQueueOneList.push(&pkQueueUserDataNodes[iPlayerIdx]);
			}
		}
	}

	return iHasUserExit;
}

int mtThreadRoom::runFilterUserExitTwoList(mtQueueHall::DataRoom* pkQueueHallDataRoom, mtQueueDesk::DataNode* pkQueueDeskDataNode)
{
	long iHasUserExit = 0;
	if (NULL != pkQueueDeskDataNode->pkQueueUserDataNode[0] && NULL != pkQueueDeskDataNode->pkQueueUserDataNode[1])
	{
		if (pkQueueDeskDataNode->pkQueueUserDataNode[0] == pkQueueDeskDataNode->pkQueueUserDataNode[1]
		 || pkQueueDeskDataNode->pkQueueUserDataNode[0]->lUserId == pkQueueDeskDataNode->pkQueueUserDataNode[1]->lUserId)
		{
			iHasUserExit  = 1;
			pkQueueDeskDataNode->pkQueueUserDataNode[1]->lUserDeskIndex                            = -1;
			pkQueueDeskDataNode->pkQueueUserDataNode[1]->pkQueueDeskDataNode                       = NULL;
			pkQueueHallDataRoom->kQueueOneList.push(&pkQueueDeskDataNode->pkQueueUserDataNode[1]);

			/// ���ӻ���
			pkQueueDeskDataNode->lDeskExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
			m_pkQueueMgr->getQueueDesk()->push(pkQueueDeskDataNode);
			return iHasUserExit;
		}
	}

	mtQueueUser::DataNode* pkQueueUserDataNode;
	if (mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING != pkQueueDeskDataNode->lDeskExit)
	{
		iHasUserExit                 = 1;
		pkQueueDeskDataNode->lQueue  = 0;
		long iPlayerIdx;
		for (iPlayerIdx = 0; iPlayerIdx < (MT_NODE_PLAYER - 1); iPlayerIdx++)
		{
			pkQueueUserDataNode = pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx];
			if (NULL != pkQueueUserDataNode)
			{
				if (mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING != pkQueueUserDataNode->lUserExit
					&& mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_CHANGE != pkQueueUserDataNode->lUserExit
					&& mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_ROUND_OVER != pkQueueUserDataNode->lUserExit)
				{
					/// �����ai
					if (MT_QUEUE_USER_ID_AI_START     < pkQueueUserDataNode->lUserId
						&& mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iPlayerIdx])
					{
						pkQueueUserDataNode->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
						pkQueueUserDataNode->lUserDeskIndex                            = -1;
						pkQueueUserDataNode->pkQueueDeskDataNode                       = NULL;

						/////////////////////////////////////////////////
						pkQueueDeskDataNode->plPlayerMode[iPlayerIdx]					= 0;

						pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]           = NULL;
						m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNode);
					}
				}
				else
				{
					if (mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_CHANGE == pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]->lUserExit
						|| mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING == pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]->lUserExit)
					{
						pkQueueUserDataNode->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING;
						pkQueueUserDataNode->lUserDeskIndex                           = -1;
						pkQueueUserDataNode->pkQueueDeskDataNode                      = NULL;
						pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]          = NULL;
						pkQueueHallDataRoom->kQueueOneList.push(&pkQueueUserDataNode);
					}
					else
					{
						/// ��������ai
						if (MT_QUEUE_USER_ID_AI_START > pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]->lUserId
							&& mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iPlayerIdx])
						{
							pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
						}
						else
						{
							/// ��ai
							if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iPlayerIdx])
							{
								pkQueueUserDataNode->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
								pkQueueUserDataNode->lUserDeskIndex                            = -1;
								pkQueueUserDataNode->pkQueueDeskDataNode                       = NULL;

								/////////////////////////////////////////////////
								pkQueueDeskDataNode->plPlayerMode[iPlayerIdx]					= 0;

								pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]           = NULL;
								m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNode);
							}
						}
					}

					pkQueueUserDataNode->lUserDeskIndex                           = -1;
					pkQueueUserDataNode->pkQueueDeskDataNode                      = NULL;
				}
			}

			/////////////////////////////////////////////////
			pkQueueDeskDataNode->plPlayerMode[iPlayerIdx]					= 0;

			pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]              = NULL;
		}

		/// ���ӻ���
		pkQueueDeskDataNode->lDeskExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
		m_pkQueueMgr->getQueueDesk()->push(pkQueueDeskDataNode);
	}
	else /// ���������У��ٶ������ϵ���Ա�ж�
	{
		long iPlayerIdx;
		for (iPlayerIdx = 0; iPlayerIdx < (MT_NODE_PLAYER - 1); iPlayerIdx++)
		{
			pkQueueUserDataNode = pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx];
			if (pkQueueUserDataNode)
			{
				if (mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING != pkQueueUserDataNode->lUserExit)
				{
					iHasUserExit  = 1;
					pkQueueDeskDataNode->lDeskExit  = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
					pkQueueHallDataRoom->kQueueTwoList.push(&pkQueueDeskDataNode);
					break;
				}
			}
			else
			{
				iHasUserExit = 1;
				pkQueueDeskDataNode->lDeskExit  = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
				pkQueueHallDataRoom->kQueueTwoList.push(&pkQueueDeskDataNode);
				//break;			//�޸�2
			}
		}
	}

	return iHasUserExit;
}

int mtThreadRoom::runFilterUserExitThreeList(mtQueueHall::DataRoom* pkQueueHallDataRoom, mtQueueDesk::DataNode* pkQueueDeskDataNode)
{
	long iDeskExit = 0;
	long iHasUserExit = 0;
	mtQueueUser::DataNode* pkQueueUserDataNode;


	if (mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING != pkQueueDeskDataNode->lDeskExit)
	{
		iDeskExit     = 1;
		iHasUserExit  = 1;
		pkQueueDeskDataNode->lQueue = 0;
		long iPlayerIdx;
		for (iPlayerIdx = 0; iPlayerIdx < MT_NODE_PLAYER; iPlayerIdx++)
		{
			pkQueueUserDataNode = pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx];
			if (NULL != pkQueueUserDataNode)
			{
				if (   mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING    != pkQueueUserDataNode->lUserExit
					&& mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_CHANGE     != pkQueueUserDataNode->lUserExit
					&& mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_ROUND_OVER != pkQueueUserDataNode->lUserExit)
				{
					/// �����ai
					//if (MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNode->lUserId
						//&& mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iPlayerIdx])
					if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iPlayerIdx])
					{
						pkQueueUserDataNode->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
						pkQueueUserDataNode->lUserDeskIndex                            = -1;
						pkQueueUserDataNode->pkQueueDeskDataNode                       = NULL;

						/////////////////////////////////////////////////
						pkQueueDeskDataNode->plPlayerMode[iPlayerIdx]				   = 0;

						pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]           = NULL;
						m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNode);
					}
				}
				else
				{
					if (mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_CHANGE == pkQueueUserDataNode->lUserExit
						|| mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING == pkQueueUserDataNode->lUserExit)
					{
						pkQueueUserDataNode->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING;
						pkQueueUserDataNode->lUserDeskIndex                           = -1;
						pkQueueUserDataNode->pkQueueDeskDataNode                      = NULL;
						pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]          = NULL;
						pkQueueHallDataRoom->kQueueOneList.push(&pkQueueUserDataNode);
					}
					else
					{
						/// ��������ai
						if (MT_QUEUE_USER_ID_AI_START > pkQueueUserDataNode->lUserId
							&& mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iPlayerIdx])
						{
							pkQueueUserDataNode->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
						}
						else
						{
							/// ��ai
							if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iPlayerIdx])
							{
								pkQueueUserDataNode->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
								pkQueueUserDataNode->lUserDeskIndex                            = -1;
								pkQueueUserDataNode->pkQueueDeskDataNode                       = NULL;

								
								pkQueueDeskDataNode->plPlayerMode[iPlayerIdx]					= 0;

								pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]           = NULL;
								m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNode);
							}
						}
					}
				}

				pkQueueUserDataNode->lUserDeskIndex                           = -1;
				pkQueueUserDataNode->pkQueueDeskDataNode                      = NULL;
			}

			/////////////////////////////////////////////////
			pkQueueDeskDataNode->plPlayerMode[iPlayerIdx]					= 0;

			pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]            = NULL;
		
		}

		/// ���ӻ���
		pkQueueDeskDataNode->lDeskExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
		m_pkQueueMgr->getQueueDesk()->push(pkQueueDeskDataNode);
	}
	else /// ���������У��ٶ������ϵ���Ա�ж�
	{
		if(MT_SERVICE_USER_CARD_OUT_TIME < GetTickCount() - pkQueueDeskDataNode->ullTimeEnterRun)
		{		
			for(int iPlayerIdx = 0; iPlayerIdx < MT_NODE_PLAYER; ++iPlayerIdx)
			{
				if(NULL != pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx] && mtQueueDesk::E_PLAYER_MODE_USER ==pkQueueDeskDataNode->plPlayerMode[iPlayerIdx]
				&& MT_SERVICE_USER_CARD_OUT_TIME < (GetTickCount() - pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]->ullTimeEnterRun))
				{
					pkQueueDeskDataNode->ullTimeEnterRun = GetTickCount();
					MT_DEBUG_PTCL("\nmtThreadRoom Desk time out,runHeartBeat");
					runHeartBeat(pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx + 1 <MT_NODE_PLAYER ? iPlayerIdx + 1 : 0]);
				
					break;
				}
			}
		}

		long iPlayerExitCount = 0;
		long iPlayerIdx;
		for (iPlayerIdx = 0; iPlayerIdx < MT_NODE_PLAYER; iPlayerIdx++)
		{
			pkQueueUserDataNode = pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx];
			if (NULL != pkQueueUserDataNode)
			{
				if (   mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING != pkQueueUserDataNode->lUserExit
					&& mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_CHANGE != pkQueueUserDataNode->lUserExit
					&& mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_ROUND_OVER != pkQueueUserDataNode->lUserExit)
				{
					long lUserExit = pkQueueUserDataNode->lUserExit;
					iPlayerExitCount++;
					iHasUserExit  = 1;
					/// �����ai
					if (MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNode->lUserId
						&& mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iPlayerIdx])
					{
						pkQueueUserDataNode->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
						m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNode);
					}

					pkQueueUserDataNode->lUserDeskIndex                  = -1;
					pkQueueUserDataNode->pkQueueDeskDataNode             = NULL;
					pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx] = NULL;
					
				}
				else
				{
					if (mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_CHANGE == pkQueueUserDataNode->lUserExit)
					{
						iHasUserExit  = 1;
						pkQueueUserDataNode->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING;

						/// ��������ѹ��
						pkQueueUserDataNode->pkQueueDeskDataNode              = NULL;
						pkQueueUserDataNode->lUserDeskIndex                   = -1;
						pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]  = NULL;
						pkQueueHallDataRoom->kQueueOneList.push(&pkQueueUserDataNode);
						
					}
					else if (mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_ROUND_OVER == pkQueueUserDataNode->lUserExit)
					{
						iHasUserExit = 1;
						if (mtQueueDesk::E_PLAYER_MODE_USER == pkQueueDeskDataNode->plPlayerMode[iPlayerIdx])
						{
							pkQueueUserDataNode->pkQueueDeskDataNode              = NULL;
							pkQueueUserDataNode->lUserDeskIndex                   = -1;
							pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]  = NULL;
							
						}
						else
						{
							if (MT_QUEUE_USER_ID_AI_START >= pkQueueUserDataNode->lUserId)
							{
								pkQueueUserDataNode->lUserExit                        = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
								pkQueueUserDataNode->pkQueueDeskDataNode              = NULL;
								pkQueueUserDataNode->lUserDeskIndex                   = -1;
								pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]  = NULL;
								
							}
							else
							{
								pkQueueUserDataNode->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RELEASE;
								m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNode);
								pkQueueUserDataNode->pkQueueDeskDataNode              = NULL;
								pkQueueUserDataNode->lUserDeskIndex                   = -1;
								pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]  = NULL;
							}
						}

					}
				}
			}
			else
			{
				iHasUserExit = 1;
				pkQueueDeskDataNode->pkQueueUserDataNode[iPlayerIdx]  = NULL;
			}
		}

		if (iHasUserExit && 0 == iDeskExit)
		{
			return runReFilterUserExitThreeList(pkQueueHallDataRoom, pkQueueDeskDataNode);
		}
	}

	return iHasUserExit;
}

int mtThreadRoom::runReFilterUserExitThreeList(mtQueueHall::DataRoom* pkQueueHallDataRoom, mtQueueDesk::DataNode* pkQueueDeskDataNode)
{
	MT_DEBUG_PTCL("\n runReFilterUserExitThreeList [%x] [%x-%x-%x]",pkQueueDeskDataNode,
		pkQueueDeskDataNode->pkQueueUserDataNode[0],
		pkQueueDeskDataNode->pkQueueUserDataNode[1],
		pkQueueDeskDataNode->pkQueueUserDataNode[2]);

	mtQueueUser::DataNode* pkQueueUserDataNode1 = pkQueueDeskDataNode->pkQueueUserDataNode[0];
	mtQueueUser::DataNode* pkQueueUserDataNode2 = pkQueueDeskDataNode->pkQueueUserDataNode[1];
	mtQueueUser::DataNode* pkQueueUserDataNode3 = pkQueueDeskDataNode->pkQueueUserDataNode[2];

	long 	lIsWaitStart1                       = pkQueueDeskDataNode->plIsWaitStart[0];
	long 	lIsWaitStart2                       = pkQueueDeskDataNode->plIsWaitStart[1];
	long 	lIsWaitStart3                       = pkQueueDeskDataNode->plIsWaitStart[2];
	int  	iPlayerMode1                        = pkQueueDeskDataNode->plPlayerMode[0];
	int  	iPlayerMode2                        = pkQueueDeskDataNode->plPlayerMode[1];
	int  	iPlayerMode3                        = pkQueueDeskDataNode->plPlayerMode[2];
	if ((NULL == pkQueueUserDataNode1 && NULL == pkQueueUserDataNode2 && NULL == pkQueueUserDataNode3)
	 || (NULL == pkQueueUserDataNode1 && NULL == pkQueueUserDataNode2)
	 || (NULL == pkQueueUserDataNode1 && NULL == pkQueueUserDataNode3)
	 || (NULL == pkQueueUserDataNode2 && NULL == pkQueueUserDataNode3))
	{
		pkQueueDeskDataNode->lDeskExit  = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;

		return 0;
	}

	/// ��һ�����˳�
	if (NULL == pkQueueUserDataNode1)
	{
		if ((MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNode2->lUserId || mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[1])
			&& (MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNode3->lUserId || mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[2]))
		{	//��ʣ��2�˶��ǻ����˻���AIģʽ��
			
			pkQueueUserDataNode2->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
			pkQueueUserDataNode3->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
			pkQueueDeskDataNode->lDeskExit  = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
			m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNode2);
			m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNode3);
			m_pkQueueMgr->getQueueDesk()->push(pkQueueDeskDataNode);
		}
		else if (MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNode2->lUserId
			|| mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[1])
		{	//����2���ǻ����˻���AIģʽ��
			pkQueueUserDataNode2->lUserDeskIndex         = 1;
			pkQueueUserDataNode3->lUserDeskIndex         = 0;
			pkQueueDeskDataNode->lQueue                  = 2;
			pkQueueDeskDataNode->pkQueueUserDataNode[0]  = pkQueueUserDataNode3;
			pkQueueDeskDataNode->plIsWaitStart[0]        = lIsWaitStart3;
			pkQueueDeskDataNode->plPlayerMode[0]         = iPlayerMode3;
			pkQueueDeskDataNode->pkQueueUserDataNode[1]  = pkQueueUserDataNode2;
			pkQueueDeskDataNode->plIsWaitStart[1]        = lIsWaitStart2;
			pkQueueDeskDataNode->plPlayerMode[1]         = iPlayerMode2;
			pkQueueDeskDataNode->pkQueueUserDataNode[2]  = NULL;
			pkQueueDeskDataNode->plIsWaitStart[2]        = 0;
			pkQueueDeskDataNode->plPlayerMode[2]         = 0;

			pkQueueDeskDataNode->ullTimeEnterRun         = GetTickCount();
			pkQueueHallDataRoom->kQueueTwoList.push(&pkQueueDeskDataNode);
		}
		else
		{	//����3���ǻ����˻���AIģʽ��
			pkQueueUserDataNode2->lUserDeskIndex         = 0;
			pkQueueUserDataNode3->lUserDeskIndex         = 1;
			pkQueueDeskDataNode->lQueue                  = 2;
			pkQueueDeskDataNode->pkQueueUserDataNode[0]  = pkQueueUserDataNode2;
			pkQueueDeskDataNode->plIsWaitStart[0]        = lIsWaitStart2;
			pkQueueDeskDataNode->plPlayerMode[0]         = iPlayerMode2;
			pkQueueDeskDataNode->pkQueueUserDataNode[1]  = pkQueueUserDataNode3;
			pkQueueDeskDataNode->plIsWaitStart[1]        = lIsWaitStart3;
			pkQueueDeskDataNode->plPlayerMode[1]         = iPlayerMode3;
			pkQueueDeskDataNode->pkQueueUserDataNode[2]  = NULL;
			pkQueueDeskDataNode->plIsWaitStart[2]        = 0;
			pkQueueDeskDataNode->plPlayerMode[2]         = 0;
            
			pkQueueDeskDataNode->ullTimeEnterRun         = GetTickCount();
			pkQueueHallDataRoom->kQueueTwoList.push(&pkQueueDeskDataNode);
		}

		return 1;
	}

	/// �ڶ������˳�
	if (NULL == pkQueueUserDataNode2)
	{
		if ((MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNode1->lUserId || mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[0])
			&& (MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNode3->lUserId || mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[2]))
		{
			pkQueueUserDataNode1->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
			pkQueueUserDataNode3->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
			pkQueueDeskDataNode->lDeskExit  = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
			m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNode1);
			m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNode3);
			m_pkQueueMgr->getQueueDesk()->push(pkQueueDeskDataNode);
		}
		else if (MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNode1->lUserId
			|| mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[0])
		{
			pkQueueUserDataNode1->lUserDeskIndex         = 1;
			pkQueueUserDataNode3->lUserDeskIndex         = 0;
			pkQueueDeskDataNode->lQueue                  = 2;
			pkQueueDeskDataNode->pkQueueUserDataNode[0]  = pkQueueUserDataNode3;
			pkQueueDeskDataNode->plIsWaitStart[0]        = lIsWaitStart3;
			pkQueueDeskDataNode->plPlayerMode[0]         = iPlayerMode3;
			pkQueueDeskDataNode->pkQueueUserDataNode[1]  = pkQueueUserDataNode1;
			pkQueueDeskDataNode->plIsWaitStart[1]        = lIsWaitStart1;
			pkQueueDeskDataNode->plPlayerMode[1]         = iPlayerMode1;
			pkQueueDeskDataNode->pkQueueUserDataNode[2]  = NULL;
			pkQueueDeskDataNode->plIsWaitStart[2]        = 0;
			pkQueueDeskDataNode->plPlayerMode[2]         = 0;

			pkQueueDeskDataNode->ullTimeEnterRun         = GetTickCount();
			pkQueueHallDataRoom->kQueueTwoList.push(&pkQueueDeskDataNode);
		}
		else
		{
			pkQueueUserDataNode1->lUserDeskIndex         = 0;
			pkQueueUserDataNode3->lUserDeskIndex         = 1;
			pkQueueDeskDataNode->lQueue                  = 2;
			pkQueueDeskDataNode->pkQueueUserDataNode[0]  = pkQueueUserDataNode1;
			pkQueueDeskDataNode->plIsWaitStart[0]        = lIsWaitStart1;
			pkQueueDeskDataNode->plPlayerMode[0]         = iPlayerMode1;
			pkQueueDeskDataNode->pkQueueUserDataNode[1]  = pkQueueUserDataNode3;
			pkQueueDeskDataNode->plIsWaitStart[1]        = lIsWaitStart3;
			pkQueueDeskDataNode->plPlayerMode[1]         = iPlayerMode3;
			pkQueueDeskDataNode->pkQueueUserDataNode[2]  = NULL;
			pkQueueDeskDataNode->plIsWaitStart[2]        = 0;
			pkQueueDeskDataNode->plPlayerMode[2]         = 0;

			pkQueueDeskDataNode->ullTimeEnterRun         = GetTickCount();
			pkQueueHallDataRoom->kQueueTwoList.push(&pkQueueDeskDataNode);
		}

		return 1;
	}

	/// ���������˳�
	if (NULL == pkQueueUserDataNode3)
	{
		if ((MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNode1->lUserId || mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[0])
			&& (MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNode2->lUserId || mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[1]))
		{
			pkQueueUserDataNode1->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
			pkQueueUserDataNode2->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
			pkQueueDeskDataNode->lDeskExit  = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_EXITING;
			m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNode1);
			m_pkQueueMgr->getQueueUser()->push(pkQueueUserDataNode2);
			m_pkQueueMgr->getQueueDesk()->push(pkQueueDeskDataNode);
		}
		else if (MT_QUEUE_USER_ID_AI_START < pkQueueUserDataNode1->lUserId
			|| mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[0])
		{
			pkQueueUserDataNode2->lUserDeskIndex         = 0;
			pkQueueUserDataNode1->lUserDeskIndex         = 1;
			pkQueueDeskDataNode->lQueue                  = 2;
			pkQueueDeskDataNode->pkQueueUserDataNode[0]  = pkQueueUserDataNode2;
			pkQueueDeskDataNode->plIsWaitStart[0]        = lIsWaitStart2;
			pkQueueDeskDataNode->plPlayerMode[0]         = iPlayerMode2;
			pkQueueDeskDataNode->pkQueueUserDataNode[1]  = pkQueueUserDataNode1;
			pkQueueDeskDataNode->plIsWaitStart[1]        = lIsWaitStart1;
			pkQueueDeskDataNode->plPlayerMode[1]         = iPlayerMode1;
			pkQueueDeskDataNode->pkQueueUserDataNode[2]  = NULL;
			pkQueueDeskDataNode->plIsWaitStart[2]        = 0;
			pkQueueDeskDataNode->plPlayerMode[2]         = 0;

			pkQueueDeskDataNode->ullTimeEnterRun         = GetTickCount();
			pkQueueHallDataRoom->kQueueTwoList.push(&pkQueueDeskDataNode);
		}
		else
		{
			pkQueueDeskDataNode->lQueue                  = 2;
			pkQueueDeskDataNode->pkQueueUserDataNode[2]  = NULL;
			pkQueueDeskDataNode->plIsWaitStart[2]        = 0;
			pkQueueDeskDataNode->plPlayerMode[2]         = 0;

			pkQueueDeskDataNode->ullTimeEnterRun         = GetTickCount();
			pkQueueHallDataRoom->kQueueTwoList.push(&pkQueueDeskDataNode);
		}

		return 1;
	}

	return 0;
}

int mtThreadRoom::initQueueUserDataNodeAi(mtQueueUser::DataNode* pkQueueUserDataNodeAi,long lRoomId)
{
static int iQueueUserDataNodeIdAI = MT_QUEUE_USER_ID_AI_START;
	srand((unsigned)time(NULL));
	iQueueUserDataNodeIdAI++;
	if (iQueueUserDataNodeIdAI >= (MT_QUEUE_USER_ID_AI_START * 20))
	{
		iQueueUserDataNodeIdAI = MT_QUEUE_USER_ID_AI_START;
	}

	long lRoomMultiple = (lRoomId == 0 ? 1 : (lRoomId==1 ? 2 : (lRoomId==2 ? 4 : (lRoomId==3 ? 10 : (lRoomId==4 ? 20 : (lRoomId==5 ? 50 : 0))))));

	pkQueueUserDataNodeAi->lStructBytes              = sizeof(mtQueueUser::DataNode);
	pkQueueUserDataNodeAi->lSpaceId                  = 0;
	pkQueueUserDataNodeAi->lRoomId                   = 0;
	pkQueueUserDataNodeAi->lUserDeskIndex            = 0;
	pkQueueUserDataNodeAi->lUserGold                 = (1000 + rand()%200) * (lRoomMultiple * (lRoomId + 1)) + (lRoomId==1?3000:0);
	pkQueueUserDataNodeAi->lUserId                   = iQueueUserDataNodeIdAI;
	pkQueueUserDataNodeAi->lUserIconId               = 0;
	pkQueueUserDataNodeAi->lUserSex                  = rand() % 2;
	//pkQueueUserDataNodeAi->lUserLevel                = 14 - (rand() % 2);
	pkQueueUserDataNodeAi->lUserLevel                = rand() % ((lRoomId+1)*2+2);
	/*if (14 == pkQueueUserDataNodeAi->lUserLevel)
	{
		pkQueueUserDataNodeAi->lUserScore            = 100000000;
	}
	else
	{
		pkQueueUserDataNodeAi->lUserScore            = 50000000;
	}*/
	pkQueueUserDataNodeAi->lUserScore				= (4000 * lRoomMultiple) * (1 + (lRoomId * (lRoomId + 1)));
	pkQueueUserDataNodeAi->lUserOffLineCount         = rand() % (10 * (lRoomId+1));
	pkQueueUserDataNodeAi->lUserAllChess             = lRoomMultiple * 200 + rand()%150;;
	pkQueueUserDataNodeAi->lUserWinChess             = pkQueueUserDataNodeAi->lUserAllChess*2/5 + rand()%(pkQueueUserDataNodeAi->lUserAllChess*2/10);
	pkQueueUserDataNodeAi->lUserWinRate              = (pkQueueUserDataNodeAi->lUserWinChess * 100 / pkQueueUserDataNodeAi->lUserAllChess);
	pkQueueUserDataNodeAi->lUserExit                 = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING;
	pkQueueUserDataNodeAi->pkQueueDeskDataNode       = NULL;
	pkQueueUserDataNodeAi->pkQueuePacketDataNodeHead = NULL;

	memset(pkQueueUserDataNodeAi->plPropsCount, 0, sizeof(pkQueueUserDataNodeAi->plPropsCount));
	sprintf_s(pkQueueUserDataNodeAi->pcUserAccountNum, "%d", pkQueueUserDataNodeAi->lUserId);
	sprintf_s(pkQueueUserDataNodeAi->pcUserNickName, "%d", pkQueueUserDataNodeAi->lUserId);
	pkQueueUserDataNodeAi->uiSockets                 = INVALID_SOCKET;
	pkQueueUserDataNodeAi->ullTimeEnterRun           = GetTickCount();

	return 1;
}

int mtThreadRoom::runWaitStartRecvAi(mtQueueUser::DataNode* pkQueueUserDataNode)
{
	/// ��Ӧ����������������
	mtQueuePacket::DataNode* pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("threadroom1399");
	if (NULL != pkQueuePacketDataNode)
	{
		mtServiceRoomWaitStartRecv::DataRead* pkDataRead = (mtServiceRoomWaitStartRecv::DataRead*)pkQueuePacketDataNode->pcData;

		pkDataRead->lStructBytes           = sizeof(mtServiceRoomWaitStartRecv::DataRead);
		pkDataRead->lServiceType           = mtServiceMgr::E_SERVICE_ROOM_WAIT_START;

		mtService::DataRun kDataRun;
		kDataRun.pkQueuePacketDataNode	   = pkQueuePacketDataNode;
		kDataRun.pkQueueUserDataNode	   = pkQueueUserDataNode;

		m_pkServiceMgr->run(&kDataRun);

		m_pkQueueMgr->getQueuePacket()->push(pkQueuePacketDataNode);
	}

	return 1;
}

int mtThreadRoom::runStartCardDeal(mtQueueUser::DataNode* pkQueueUserDataNode)
{
	/// ����ϴ�ƺͷ��ƹ��ܣ���ÿ���˷���
	mtQueuePacket::DataNode* pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("threadroom1423");
	if (pkQueuePacketDataNode)
	{
		mtServiceCardDeal::DataRead* pkDataRead = (mtServiceCardDeal::DataRead*)pkQueuePacketDataNode->pcData;
		pkDataRead->lStructBytes                = sizeof(mtServiceCardDeal::DataRead);
		pkDataRead->lServiceType                = mtServiceMgr::E_SERVICE_ROOM_CARD_DEAL;
		pkDataRead->lResult                     = -1;

		mtService::DataRun kDataRun;
		kDataRun.pkQueuePacketDataNode	   = pkQueuePacketDataNode;
		kDataRun.pkQueueUserDataNode	   = pkQueueUserDataNode;


		m_pkServiceMgr->run(&kDataRun);

		/////////////////////////////////////////////				����packet���ڵ�
		m_pkQueueMgr->getQueuePacket()->push(pkQueuePacketDataNode);
	}

	return 1;
}

int mtThreadRoom::runHeartBeat(mtQueueUser::DataNode* pkQueueUserDataNode)
{
	mtQueuePacket::DataNode* pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("threadroom1532");

	if (NULL != pkQueuePacketDataNode)
	{
		mtServiceRoomHeartBeat::DataRead* pkDataRead = (mtServiceRoomHeartBeat::DataRead*)pkQueuePacketDataNode->pcData;

		pkDataRead->lStructBytes           = sizeof(mtServiceRoomHeartBeat::DataRead);
		pkDataRead->lServiceType           = mtServiceMgr::E_SERVICE_ROOM_HEART_BEAT;

		mtService::DataRun kDataRun;
		kDataRun.pkQueuePacketDataNode	= pkQueuePacketDataNode;
		kDataRun.pkQueueUserDataNode	= pkQueueUserDataNode;

		m_pkServiceMgr->run(&kDataRun);

		m_pkQueueMgr->getQueuePacket()->push(pkQueuePacketDataNode);
	}

	return 1;
}