#include "mtServiceRoomRoundOver.h"
#include "mtServiceRoomWaitStartRecv.h"
#include "mtServiceMgr.h"
#include "mtServiceRoomExit.h"
#include "mtPrint.h"

mtServiceRoomRoundOver::mtServiceRoomRoundOver()
{

}

mtServiceRoomRoundOver::~mtServiceRoomRoundOver()
{

}

int mtServiceRoomRoundOver::init( void* pData )
{
	DataInit*	pkDataInit	= (DataInit*)pData;

	m_pkQueueMgr            = pkDataInit->pkQueueMgr;
	m_pkServiceMgr          = pkDataInit->pkServiceMgr;

	return	1;
}

int mtServiceRoomRoundOver::run( DataRun* pkDataRun )
{
	mtQueueUser::DataNode*      pkQueueUserDataNode	  = pkDataRun->pkQueueUserDataNode;
	mtQueuePacket::DataNode*	pkQueuePacketDataNode = pkDataRun->pkQueuePacketDataNode;
	DataRead*                   pkDataRead            = (DataRead*)pkQueuePacketDataNode->pcData;
	mtQueueDesk::DataNode* pkQueueDeskDataNode        = (mtQueueDesk::DataNode*)pkQueueUserDataNode->pkQueueDeskDataNode;
	mtQueueHall::DataRoom*	pkQueueHallDataRoom       = m_pkQueueMgr->getQueueHall()->getRoom(pkQueueUserDataNode->lSpaceId, pkQueueUserDataNode->lRoomId);
	mtPrint(pkDataRead);
	if (NULL == pkQueueDeskDataNode)
	{
///		SetEvent(pkQueueHallDataRoom->hEvent);
		return 1;
	}

	EnterCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
	MT_DEBUG(4, "User(UserId=%d) mtServiceRoomRoundOver  000000000......\n", pkQueueUserDataNode->lUserId);
	/// ���¸��˵ȴ���һ�ֱ�ʶ
	resetWaitStartStatus(pkQueueDeskDataNode);
	/// ���¸����ϴ�һ�ֽ��������ʶ
	pkQueueDeskDataNode->plRoundOverGold[pkQueueUserDataNode->lUserDeskIndex] = pkDataRead->lGold;
	pkQueueDeskDataNode->plRoundOver[pkQueueUserDataNode->lUserDeskIndex]     = 1;
	pkQueueDeskDataNode->plIsWaitStart[pkQueueUserDataNode->lUserDeskIndex]   = 0;
	pkQueueUserDataNode->lIsWaitStart                                         = 0;

	/// ���������Ϣ
	pkQueueUserDataNode->lUserAllChess++;

	if (0 <= pkDataRead->lGold)
	{
	  pkQueueUserDataNode->lUserWinChess++;
	}

	/// ���û�Ӯ�˱���
	/*if (0 <= pkDataRead->lGold)
	{
	pkQueueUserDataNode->lUserWinChess++;
	pkQueueUserDataNode->lUserScore  += 200 * 5;
	}
	else
	{
	if (pkQueueUserDataNode->lUserGold >= abs(pkDataRead->lGold))
	{
	pkQueueUserDataNode->lUserScore  += 200 * 5;
	}
	else
	{
	long lGoldDeduct = (abs(pkDataRead->lGold) - pkQueueUserDataNode->lUserGold);
	if (200 > lGoldDeduct)
	{
	pkQueueUserDataNode->lUserScore  += (200 - lGoldDeduct) * 5;
	}
	}
	}*/

	pkQueueUserDataNode->lUserWinRate        = 100 * (pkQueueUserDataNode->lUserWinChess / (float)(pkQueueUserDataNode->lUserAllChess));

	pkQueueUserDataNode->lUserGold           += pkDataRead->lGold;
	
	int doubleScore = calcDoubleScore(pkQueueUserDataNode);

	pkQueueUserDataNode->lUserScore          += pkDataRead->lGold > 0 ? (pkQueueUserDataNode->lRoomId + 1) * MT_USER_WIN_SCORE * doubleScore : 0;

	if (0 > pkQueueUserDataNode->lUserGold)
	{
		pkQueueUserDataNode->lUserGold = 0;
	}
	calculatePlayerLevel(pkQueueUserDataNode);

// 	//  ��ǰ����ʵ�û�
// 	if (mtQueueDesk::E_PLAYER_MODE_USER == pkQueueDeskDataNode->plPlayerMode[pkQueueUserDataNode->lUserDeskIndex]
// 	 || MT_QUEUE_USER_ID_AI_START > pkQueueDeskDataNode->pkQueueUserDataNode[pkQueueUserDataNode->lUserDeskIndex]->lUserId)
// 	{
		/// ����¼��û��������ϵ�λ��
		int iQueueUserNextIdx = getQueueUserNextQueueUser(pkQueueUserDataNode->lUserDeskIndex);

		//  �¼���ai�û�
		if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iQueueUserNextIdx])
		{
			playUpdateQueueUserInfoAi(pkQueueDeskDataNode, pkDataRead->lServiceType, pkQueueUserDataNode->lUserDeskIndex
				, pkDataRead->lGold, iQueueUserNextIdx);

			/// ���ai�û����¼��������ϵ�λ��
			int iQueueUserNextNextIdx = getQueueUserNextQueueUser(iQueueUserNextIdx);

			//  ai�¼һ�����ai�û�
			if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iQueueUserNextNextIdx])
			{
				playUpdateQueueUserInfoAi(pkQueueDeskDataNode, pkDataRead->lServiceType, pkQueueUserDataNode->lUserDeskIndex
					, pkDataRead->lGold, iQueueUserNextNextIdx);
			}

			/// �жϵ�ǰ��������û��ʹ��ai�������ʵ�û��ڵ�
			/// playReplaceAiExitRoom(pkQueueDeskDataNode);
		}
//	}

	/// ��ǰ���е��û����Ѿ��ϴ������
	if (1 == pkQueueDeskDataNode->plRoundOver[0]
	&& 1 == pkQueueDeskDataNode->plRoundOver[1]
	&& 1 == pkQueueDeskDataNode->plRoundOver[2])
	{
		MT_DEBUG(4, "User(UserId=%d) mtServiceRoomRoundOver 1111111111........\n", pkQueueUserDataNode->lUserId);
		playRoundOver(pkQueueDeskDataNode, pkDataRead->lServiceType);
		playUpdateQueueDesk(pkQueueDeskDataNode);
		LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
///		SetEvent(pkQueueHallDataRoom->hEvent);
		return 1;
	}

	LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
///	 (pkQueueHallDataRoom->hEvent);
	return	1;
}

int mtServiceRoomRoundOver::playReplaceAiExitRoom(mtQueueDesk::DataNode* pkQueueDeskDataNode)
{
	mtQueueUser::DataNode*      pkQueueUserDataNodeReplaceAi1 = NULL;
	mtQueueUser::DataNode*      pkQueueUserDataNodeReplaceAi2 = NULL;
	int		iUserIdx = 0;
	if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iUserIdx]
	&& MT_QUEUE_USER_ID_AI_START > pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserId)
	{
		pkQueueUserDataNodeReplaceAi1 = pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx];

		iUserIdx++;
		if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iUserIdx]
		&& MT_QUEUE_USER_ID_AI_START > pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserId)
		{
			pkQueueUserDataNodeReplaceAi2 = pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx];
		}
		else
		{
			iUserIdx++;
			if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iUserIdx]
			&& MT_QUEUE_USER_ID_AI_START > pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserId)
			{
				pkQueueUserDataNodeReplaceAi2 = pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx];
			}
		}
	}
	else
	{
		iUserIdx++;
		if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iUserIdx]
		&& MT_QUEUE_USER_ID_AI_START > pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserId)
		{
			pkQueueUserDataNodeReplaceAi1 = pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx];

			iUserIdx++;
			if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iUserIdx]
			&& MT_QUEUE_USER_ID_AI_START > pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserId)
			{
				pkQueueUserDataNodeReplaceAi2 = pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx];
			}
		}
		else
		{
			iUserIdx++;
			if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iUserIdx]
			&& MT_QUEUE_USER_ID_AI_START > pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]->lUserId)
			{
				pkQueueUserDataNodeReplaceAi1 = pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx];
			}
		}
	}

	if (pkQueueUserDataNodeReplaceAi1)
	{
		/// ��Ӧ�����ʵ�û���ai�˳�room
		mtQueuePacket::DataNode* pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("roomroundover186");
		if (pkQueuePacketDataNode)
		{
			mtServiceRoomExit::DataRead* pkDataRead = (mtServiceRoomExit::DataRead*)pkQueuePacketDataNode->pcData;

			pkDataRead->lStructBytes                        = sizeof(mtServiceRoomExit::DataRead);
			pkDataRead->lServiceType                        = mtServiceMgr::E_SERVICE_ROOM_EXIT;
			pkDataRead->lStatusExit                         = 1;
			pkDataRead->lCardType                           = 0;
			memset(pkDataRead->pcCard, 100, sizeof(pkDataRead->pcCard));

			mtService::DataRun kDataRun;
			kDataRun.pkQueuePacketDataNode	= pkQueuePacketDataNode;
			kDataRun.pkQueueUserDataNode	= pkQueueUserDataNodeReplaceAi1;

			m_pkServiceMgr->run(&kDataRun);

			/////////////////////////////////////////////				����packet���ڵ�
			m_pkQueueMgr->getQueuePacket()->push(pkQueuePacketDataNode);
		}

		if (pkQueueUserDataNodeReplaceAi2)
		{
			/// ��Ӧ�����ʵ�û���ai�˳�room
			pkQueuePacketDataNode = NULL;
			pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("roomroundover211");
			if (pkQueuePacketDataNode)
			{
				mtServiceRoomExit::DataRead* pkDataRead = (mtServiceRoomExit::DataRead*)pkQueuePacketDataNode->pcData;

				pkDataRead->lStructBytes                        = sizeof(mtServiceRoomExit::DataRead);
				pkDataRead->lServiceType                        = mtServiceMgr::E_SERVICE_ROOM_EXIT;
				pkDataRead->lStatusExit                         = 1;
				pkDataRead->lCardType                           = 0;
				memset(pkDataRead->pcCard, 100, sizeof(pkDataRead->pcCard));

				mtService::DataRun kDataRun;
				kDataRun.pkQueuePacketDataNode	= pkQueuePacketDataNode;
				kDataRun.pkQueueUserDataNode	= pkQueueUserDataNodeReplaceAi1;

				m_pkServiceMgr->run(&kDataRun);

				/////////////////////////////////////////////				����packet���ڵ�
				m_pkQueueMgr->getQueuePacket()->push(pkQueuePacketDataNode);
			}
		}
	}

	return 1;
}

int mtServiceRoomRoundOver::playUpdateQueueUserInfo(mtQueueUser::DataNode* pkQueueUserDataNode, long lGold)
{
	/// ���û�Ӯ�˱���
	pkQueueUserDataNode->lUserAllChess++;
	if (0 <= lGold)
	{
		pkQueueUserDataNode->lUserWinChess++;

		if (10 < lGold)
		{
			pkQueueUserDataNode->lUserGold += lGold;
			if(pkQueueUserDataNode->lUserId == 2302)
			{
				mtLog("2302[%x]:win gold: %d",pkQueueUserDataNode,lGold);
				mtLog("2302[%x]:have gold: %d",pkQueueUserDataNode,pkQueueUserDataNode->lUserGold);
			}
		}
		else
		{
			pkQueueUserDataNode->lUserGold += 10;
		}
		
	}
	else  /// ���û���������
	{
		pkQueueUserDataNode->lUserGold += lGold;
	}
	
    int doubleScore = calcDoubleScore(pkQueueUserDataNode);;

	pkQueueUserDataNode->lUserScore            += lGold > 0 ? (pkQueueUserDataNode->lRoomId + 1) * MT_USER_WIN_SCORE * doubleScore : 0;

	pkQueueUserDataNode->lUserWinRate           = 100 * (pkQueueUserDataNode->lUserWinChess / (float)pkQueueUserDataNode->lUserAllChess);
	calculatePlayerLevel(pkQueueUserDataNode);

	return 1;
}

int mtServiceRoomRoundOver::getQueueUserNextQueueUser(int iQueueUserCurIdx)
{
	if (0 == iQueueUserCurIdx)
	{
		return iQueueUserCurIdx + 2;
	}

	return iQueueUserCurIdx - 1;
}

int mtServiceRoomRoundOver::playUpdateQueueUserInfoAi(mtQueueDesk::DataNode* pkQueueDeskDataNode, long lServiceType, int iQueueUserIdx, long lGold, int iQueueUserNextAiIdx)
{
	/// ��AI�Ѿ��ϴ������
	if (1 == pkQueueDeskDataNode->plRoundOver[iQueueUserNextAiIdx])
	{
		return 1;
	}

	/// ���¸����ϴ�һ�ֽ��������ʶ
	pkQueueDeskDataNode->plRoundOver[iQueueUserNextAiIdx]                          = 1;
	/// ���¸��˵ȴ���һ�ֱ�ʶ
	pkQueueDeskDataNode->plIsWaitStart[iQueueUserNextAiIdx]                        = 0;

	/// AI���ϼ��ǵ���
	if (mtQueueDesk::E_PLAYER_TYPE_LANDLORD == pkQueueDeskDataNode->plPlayerType[iQueueUserIdx])
	{
		pkQueueDeskDataNode->plRoundOverGold[iQueueUserNextAiIdx]      = -lGold;
		playUpdateQueueUserInfo(pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserNextAiIdx], -lGold);
	}
	else /// AI���ϼ���ũ��
	{
		/// AI�ǵ���
		if (mtQueueDesk::E_PLAYER_TYPE_LANDLORD == pkQueueDeskDataNode->plPlayerType[iQueueUserNextAiIdx])
		{
			pkQueueDeskDataNode->plRoundOverGold[iQueueUserNextAiIdx]      = -lGold;
			playUpdateQueueUserInfo(pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserNextAiIdx], -lGold);
		}
		else /// AI��ũ��
		{
			pkQueueDeskDataNode->plRoundOverGold[iQueueUserNextAiIdx]      = lGold;
			playUpdateQueueUserInfo(pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserNextAiIdx], lGold);
		}
	}

	return 1;
}

int mtServiceRoomRoundOver::playUpdateQueueDesk(mtQueueDesk::DataNode* pkQueueDeskDataNode)
{
	int iQueueUserDataNodeIdx;
	for (iQueueUserDataNodeIdx = 0; iQueueUserDataNodeIdx < MT_NODE_PLAYER; iQueueUserDataNodeIdx++)
	{
		/// �������ϵ���������Ϊ���״̬
		if (NULL != pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx])
		{
			pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx]->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_ROUND_OVER;
		}

// 		/// �����ai��Ϊ���״̬
// 		if (NULL != pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx]
// 		&& mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iQueueUserDataNodeIdx]
// 		&& MT_QUEUE_USER_ID_AI_START > pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx]->lUserId
// 		&& 0 < pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx]->lUserId)
// 		{
// 			pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx]->lUserExit = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_ROUND_OVER;
// 		}
	}

	pkQueueDeskDataNode->lDeskExit          = mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_ROUND_OVER;

	return 1;
}

int mtServiceRoomRoundOver::runWaitStartRecvAi(mtQueueDesk::DataNode* pkQueueDeskDataNode)
{
	int iQueueUserDataNodeIdx;
	for (iQueueUserDataNodeIdx = 0; iQueueUserDataNodeIdx < MT_NODE_PLAYER; iQueueUserDataNodeIdx++)
	{
		/// ���͡��ȴ���ʼ�� ��Ϣ
		//  ��ǰai�û�
		if (NULL != pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx]
		   && mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iQueueUserDataNodeIdx])
		{
			playWaitStartRecvAi(pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx]);
		}
	}

	return 1;
}

int mtServiceRoomRoundOver::playWaitStartRecvAi(mtQueueUser::DataNode* pkQueueUserDataNode)
{
	/// ��Ӧ������������
	mtQueuePacket::DataNode* pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("roomroundover377");
	if (NULL != pkQueuePacketDataNode)
	{
		mtServiceRoomWaitStartRecv::DataRead* pkDataRead = (mtServiceRoomWaitStartRecv::DataRead*)pkQueuePacketDataNode->pcData;

		pkDataRead->lStructBytes           = sizeof(mtServiceRoomWaitStartRecv::DataRead);
		pkDataRead->lServiceType           = mtServiceMgr::E_SERVICE_ROOM_WAIT_START;

		mtService::DataRun kDataRun;
		kDataRun.pkQueuePacketDataNode	= pkQueuePacketDataNode;
		kDataRun.pkQueueUserDataNode	= pkQueueUserDataNode;

		m_pkServiceMgr->run(&kDataRun);

		m_pkQueueMgr->getQueuePacket()->push(pkQueuePacketDataNode);
	}

	return 1;
}

int mtServiceRoomRoundOver::exit()
{
	return	0;
}

int mtServiceRoomRoundOver::calculatePlayerLevel(mtQueueUser::DataNode* pkQueueUserDataNode)
{
	if(pkQueueUserDataNode->lUserScore >= 100000000)
	{
		pkQueueUserDataNode->lUserLevel	= 14;
	}
	else if(pkQueueUserDataNode->lUserScore >= 50000000)
	{
		pkQueueUserDataNode->lUserLevel	= 13;
	}
	else if(pkQueueUserDataNode->lUserScore >= 30000000)
	{
		pkQueueUserDataNode->lUserLevel	= 12;
	}
	else if(pkQueueUserDataNode->lUserScore >= 15000000)
	{
		pkQueueUserDataNode->lUserLevel	= 11;
	}
	else if(pkQueueUserDataNode->lUserScore >= 10000000)
	{
		pkQueueUserDataNode->lUserLevel	= 10;
	}
	else if(pkQueueUserDataNode->lUserScore >= 5000000)
	{
		pkQueueUserDataNode->lUserLevel	= 9;
	}
	else if(pkQueueUserDataNode->lUserScore >= 2000000)
	{
		pkQueueUserDataNode->lUserLevel	= 8;
	}
	else if(pkQueueUserDataNode->lUserScore >= 1500000)
	{
		pkQueueUserDataNode->lUserLevel	= 7;
	}
	else if(pkQueueUserDataNode->lUserScore >= 800000)
	{
		pkQueueUserDataNode->lUserLevel	= 6;
	}
	else if(pkQueueUserDataNode->lUserScore >= 300000)
	{
		pkQueueUserDataNode->lUserLevel	= 5;
	}
	else if(pkQueueUserDataNode->lUserScore >= 100000)
	{
		pkQueueUserDataNode->lUserLevel	= 4;
	}
	else if(pkQueueUserDataNode->lUserScore >= 50000)
	{
		pkQueueUserDataNode->lUserLevel	= 3;
	}
	else if(pkQueueUserDataNode->lUserScore >= 10000)
	{
		pkQueueUserDataNode->lUserLevel	= 2;
	}
	else if(pkQueueUserDataNode->lUserScore >= 5000)
	{
		pkQueueUserDataNode->lUserLevel	= 1;
	}
	else
	{
		pkQueueUserDataNode->lUserLevel	= 0;
	}

	return 1;
}

int mtServiceRoomRoundOver::resetWaitStartStatus(mtQueueDesk::DataNode* pkQueueDeskDataNode)
{
	if (NULL != pkQueueDeskDataNode)
	{
		if (0 == pkQueueDeskDataNode->plRoundOver[0]
		 && 0 == pkQueueDeskDataNode->plRoundOver[1]
		 && 0 == pkQueueDeskDataNode->plRoundOver[2])
		{
			int iQueueUserDataNodeIdx;
			for (iQueueUserDataNodeIdx = 0; iQueueUserDataNodeIdx < MT_NODE_PLAYER; iQueueUserDataNodeIdx++)
			{
				pkQueueDeskDataNode->plIsWaitStart[iQueueUserDataNodeIdx] = 0;

				/// ���á��ȴ���ʼ�� ��Ϣ
				if (NULL != pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx])
				{

					pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx]->lIsWaitStart = 0;
				}
			}
		}
	}
	
	return 1;
}

int mtServiceRoomRoundOver::playRoundOver(mtQueueDesk::DataNode* pkQueueDeskDataNode, long lServiceType)
{
	if (NULL != pkQueueDeskDataNode)
	{
		/// ���͸�����Ӧ�������ϵ�������Ա
		DataWrite  kDataWrite;
		memset(&kDataWrite, 0, sizeof(kDataWrite));
		kDataWrite.lStructBytes        = sizeof(kDataWrite);
		kDataWrite.lServiceType        = lServiceType;

		int		iUserIdx;
		mtQueueUser::DataNode* pkQueueUserDataNode;
		for (iUserIdx = 0; iUserIdx < MT_NODE_PLAYER; iUserIdx++)
		{
			pkQueueUserDataNode   = pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx];
			if (pkQueueUserDataNode && mtQueueDesk::E_PLAYER_MODE_USER == pkQueueDeskDataNode->plPlayerMode[iUserIdx])
			{
				mtQueuePacket::DataNode* pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("roomroundover513");
				if (pkQueuePacketDataNode)
				{
					kDataWrite.kUserInfo.lUserId             = pkQueueUserDataNode->lUserId;
					kDataWrite.kUserInfo.lUserGold           = pkQueueUserDataNode->lUserGold;
					kDataWrite.kUserInfo.lUserLevel          = pkQueueUserDataNode->lUserLevel;
					kDataWrite.kUserInfo.lUserScore          = pkQueueUserDataNode->lUserScore;
					kDataWrite.kUserInfo.lUserAllChess       = pkQueueUserDataNode->lUserAllChess;
					kDataWrite.kUserInfo.lUserWinChess       = pkQueueUserDataNode->lUserWinChess;
					kDataWrite.kUserInfo.lUserWinRate        = pkQueueUserDataNode->lUserWinRate;
					kDataWrite.kUserInfo.lUserOffLineCount   = pkQueueUserDataNode->lUserOffLineCount;
					kDataWrite.kUserInfo.lIsWaitStart        = pkQueueDeskDataNode->plIsWaitStart[pkQueueUserDataNode->lUserDeskIndex];
					kDataWrite.kUserInfo.ulDeskId            = (unsigned long)pkQueueDeskDataNode;
					memcpy(kDataWrite.kUserInfo.plPropsCount,pkQueueUserDataNode->plPropsCount, sizeof(pkQueueUserDataNode->plPropsCount));
					memcpy(kDataWrite.kUserInfo.pcUserAccountNum, pkQueueUserDataNode->pcUserAccountNum, sizeof(kDataWrite.kUserInfo.pcUserAccountNum));
					memcpy(kDataWrite.kUserInfo.pcUserName, pkQueueUserDataNode->pcUserName, sizeof(pkQueueUserDataNode->pcUserName));
					memcpy(kDataWrite.kUserInfo.pcUserNickName, pkQueueUserDataNode->pcUserNickName, sizeof(kDataWrite.kUserInfo.pcUserNickName));

					pkQueuePacketDataNode->kWSABuf.len    = kDataWrite.lStructBytes;
					memcpy (pkQueuePacketDataNode->pcData, &kDataWrite, kDataWrite.lStructBytes);
					mtPrint(&kDataWrite);
					m_pkQueueMgr->getQueueUser()->DataWrite(pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx], pkQueuePacketDataNode);
				}
			}
		}
	}

	return 1;
}

int mtServiceRoomRoundOver::calcDoubleScore(mtQueueUser::DataNode* pkQueueUserDataNode)
{
	int doubleScore = 1;
	if(pkQueueUserDataNode->plPropsCount[0] > 0 ||pkQueueUserDataNode->plPropsCount[1] == -1 )//˫�����鿨
	{
		doubleScore *= 2;
	}
	if(pkQueueUserDataNode->plPropsCount[13] > 0 ||pkQueueUserDataNode->plPropsCount[14] > 0||pkQueueUserDataNode->plPropsCount[15] == -1)//VIP
	{
		doubleScore *= 2;
	}

	return doubleScore;
}
