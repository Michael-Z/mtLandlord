#include "mtServiceCardOut.h"
#include "mtQueueDesk.h"
#include "mtQueueHall.h"
#include "mtPrint.h"

mtServiceCardOut::mtServiceCardOut():
	m_pkQueueUser(NULL),
	m_pkQueuePacket(NULL)
{

}

mtServiceCardOut::~mtServiceCardOut()
{

}

int mtServiceCardOut::init( void* pData )
{
	DataInit*	pkDataInit	= (DataInit*)pData;

	m_pkQueueMgr            = pkDataInit->pkQueueMgr;
	m_pkQueueUser           = pkDataInit->pkQueueUser;
	m_pkQueuePacket         = pkDataInit->pkQueuePacket;
	m_hEventManagerAi       = pkDataInit->hEventManagerAi;
	return	1;
}

int mtServiceCardOut::run( DataRun* pkDataRun )
{
	DataRead*			   pkDataRead          = (DataRead*)pkDataRun->pkQueuePacketDataNode->pcData;
	mtQueueUser::DataNode* pkQueueUserDataNode = pkDataRun->pkQueueUserDataNode;
	mtQueueDesk::DataNode* pkQueueDeskDataNode = (mtQueueDesk::DataNode*)pkQueueUserDataNode->pkQueueDeskDataNode;
	//mtPrint(pkDataRead);
	if (NULL == pkQueueDeskDataNode)
	{
	 	return 1;
	}

	mtQueueHall::DataRoom*	pkQueueHallDataRoom = m_pkQueueMgr->getQueueHall()->getRoom(pkQueueUserDataNode->lSpaceId, pkQueueUserDataNode->lRoomId);

	EnterCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
	if (mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING != pkQueueDeskDataNode->lDeskExit
	 || mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING != pkQueueUserDataNode->lUserExit)
	{
		LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
		return 1;
	}
	MT_DEBUG_PTCL("\nUser card out![%d]",pkQueueUserDataNode->lUserId);
	char pcCardsCurTmp[80];
	char pcCardsOutTmp[80];
	memset(pcCardsCurTmp, 0, sizeof(pcCardsCurTmp));
	memset(pcCardsOutTmp, 0, sizeof(pcCardsOutTmp));
	getCardsInformation(pkQueueDeskDataNode->pcCardsCur, pcCardsCurTmp);
	getCardsInformation(pkDataRead->pcCard, pcCardsOutTmp);
	pcCardsCurTmp[79] = 0;
	pcCardsOutTmp[79] = 0;


	pkQueueDeskDataNode->ullTimeEnterRun = GetTickCount();
	pkQueueUserDataNode->ullTimeEnterRun = GetTickCount();
	/// ���µ�ǰ���Ӵ����������Ϣ
	if (MT_CARD_TYPE_PASS != pkDataRead->lCardType)
	{
		memcpy(pkQueueDeskDataNode->pcCardsCur, pkDataRead->pcCard, sizeof(pkDataRead->pcCard));
		pkQueueDeskDataNode->lCardType          = pkDataRead->lCardType;
		pkQueueDeskDataNode->lQueueUserDeskIdx  = pkQueueUserDataNode->lUserDeskIndex;

	    /// ������û����������Ϣ
		updateQueueDeskCardsOut(pkQueueDeskDataNode);

		/// ���Ƴɹ������¸��û������϶�Ӧ������Ϣ
		updateCardsFace(pkQueueDeskDataNode->pcCardsTotal[pkQueueUserDataNode->lUserDeskIndex], pkDataRead->pcCard);
		pkQueueDeskDataNode->plPlayerLots[pkQueueUserDataNode->lUserDeskIndex]++;
	}
	else
	{
		memset(pkDataRead->pcCard, MT_CARD_TERMINATE, sizeof(pkDataRead->pcCard));
	}

	DataWrite  kDataWrite;
	memset(&kDataWrite, 0, sizeof(kDataWrite));
	kDataWrite.lStructBytes      = sizeof(kDataWrite);
	kDataWrite.lServiceType      = pkDataRead->lServiceType;
	kDataWrite.lUserId           = pkQueueUserDataNode->lUserId;
	kDataWrite.lCardType         = pkDataRead->lCardType;
	memcpy(kDataWrite.pcCard, pkDataRead->pcCard, sizeof(pkDataRead->pcCard));

	int		iUserIdx;
	mtQueuePacket::DataNode* pkQueuePacketDataNode;
	for (iUserIdx = 0; iUserIdx < MT_NODE_PLAYER; iUserIdx++)
	{
		//  ��Ӧ������Ϣ��������ʵ�û�
		if (pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]
		&& mtQueueDesk::E_PLAYER_MODE_USER == pkQueueDeskDataNode->plPlayerMode[iUserIdx])
		{
			pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueuePacket->popInitDataNode("cardout105");
			if (pkQueuePacketDataNode)
			{
				pkQueuePacketDataNode->kWSABuf.len    = kDataWrite.lStructBytes;
				memcpy (pkQueuePacketDataNode->pcData, &kDataWrite, kDataWrite.lStructBytes);
			//	mtPrint(&kDataWrite);
				m_pkQueueUser->DataWrite(pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx], pkQueuePacketDataNode);
			}
		}
	}

	pkQueueDeskDataNode->lUserDeskIdxCardOutNext = getQueueUserNextQueueUser(pkQueueUserDataNode->lUserDeskIndex);
	if (0 < getQueueUserLeftCards(pkQueueDeskDataNode->pcCardsTotal[pkQueueUserDataNode->lUserDeskIndex]))
	{
		/// ����¼��û��������ϵ�λ��
		int iQueueUserNextIdx = getQueueUserNextQueueUser(pkQueueUserDataNode->lUserDeskIndex);
		
		//  �¼���ai�û�
		if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iQueueUserNextIdx])
		{
			int iContinue;
			ULONGLONG ullStartTime = GetTickCount();
			playOutCardAi(pkQueueDeskDataNode, pkDataRead->lServiceType, iQueueUserNextIdx, &iContinue, &ullStartTime);

			/// ���ai�û����¼��������ϵ�λ��
			int iQueueUserNextNextIdx = getQueueUserNextQueueUser(iQueueUserNextIdx);

			//  ai�¼һ�����ai�û�
			if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iQueueUserNextNextIdx]
			    && 1 == iContinue)
			{
				playOutCardAi(pkQueueDeskDataNode, pkDataRead->lServiceType, iQueueUserNextNextIdx, &iContinue, &ullStartTime);
			}
		}
	}

	pkQueueDeskDataNode->lCardDeal = 0;
	LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
	return 1;
}

int mtServiceCardOut::exit()
{
	return	1;
}

int mtServiceCardOut::getQueueUserNextQueueUser(int iQueueUserCurIdx)
{
	if (0 == iQueueUserCurIdx)
	{
		return iQueueUserCurIdx + 2;
	}

	return iQueueUserCurIdx - 1;
}

int mtServiceCardOut::updateQueueDeskCardsOut(mtQueueDesk::DataNode* pkQueueDeskDataNode)
{
	/// �ӵ������ϸ��û��Ѿ��������
	char* pcQueueDeskCardsOutStart = pkQueueDeskDataNode->pcCardsOutTotal[pkQueueDeskDataNode->lQueueUserDeskIdx];
	while (MT_CARD_TERMINATE != *pcQueueDeskCardsOutStart)
	{
		pcQueueDeskCardsOutStart++;
	}

	/// ��õ�ǰ����Ƶ�����
	int iCardsOutNum = 0;
	char* pcCurCardsOutStart = pkQueueDeskDataNode->pcCardsCur;
	while (MT_CARD_TERMINATE != *pcCurCardsOutStart)
	{
		iCardsOutNum++;
		pcCurCardsOutStart++;
	}

	memcpy(pcQueueDeskCardsOutStart, pkQueueDeskDataNode->pcCardsCur, iCardsOutNum);

	/// �ж��ǲ���ը��������ը����
	if (MT_CARD_TYPE_BOMB == pkQueueDeskDataNode->lCardType)
	{
		pkQueueDeskDataNode->plBomb[pkQueueDeskDataNode->lQueueUserDeskIdx]++;
	}

	/// �ж��ǲ��ǻ�������ӻ����
	if (MT_CARD_TYPE_ROCKET == pkQueueDeskDataNode->lCardType)
	{
		pkQueueDeskDataNode->lRocket  = 1;
	}

	return 1;
}

int mtServiceCardOut::getCardsInformation(char pcCard[], char * pcStr)
{
	int iLen = 0;
	int iIdx = 0;
	char pcTmpStr[24];
	while (MT_CARD_TERMINATE != pcCard[iIdx] && 23 > iIdx)
	{
		sprintf_s(pcTmpStr, " %d", (pcCard[iIdx] & 0x0f) + 3);
		strcpy(pcStr + iLen, pcTmpStr);
		iLen += strlen(pcTmpStr);
		iIdx++;
	}

	return 1;
}

int mtServiceCardOut::getQueueUserLeftCards(char* pcCard)
{
	int iCardNum = 0;
	while(pcCard[iCardNum] != MT_CARD_TERMINATE && 21 > iCardNum)
	{
		iCardNum++;
	}

	return iCardNum;
}

int mtServiceCardOut::playOutCardAi(mtQueueDesk::DataNode* pkQueueDeskDataNode, long lServiceType, int iQueueUserAiIdx, int* piContinue, ULONGLONG* pullStartTime)
{
	printf("\n\n*************************************888");

	/// ��������ϵ���ʵ��Ա�б�
	DataWrite kDataWrite;
	memset(&kDataWrite, 0, sizeof(kDataWrite));
	kDataWrite.lStructBytes      = sizeof(kDataWrite);
	kDataWrite.lUserId           = pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserAiIdx]->lUserId;
	kDataWrite.lServiceType      = lServiceType;
	kDataWrite.lCardType         = pkQueueDeskDataNode->lCardType;
	memset(kDataWrite.pcCard, MT_CARD_TERMINATE, sizeof(kDataWrite.pcCard));

	*piContinue = 1;
	memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));

	/// �����ǰû���˴�ס����ƣ�ai����ѡ�����
	if (pkQueueDeskDataNode->lQueueUserDeskIdx == iQueueUserAiIdx)
	{
		MT_DEBUG(4, "User(UserId=%d) getPlayOutCardFirst  100......\n", kDataWrite.lUserId);
		kDataWrite.lCardType = getPlayOutCardFirst(pkQueueDeskDataNode, iQueueUserAiIdx);
	}
	else
	{
		MT_DEBUG(4, "User(UserId=%d) getPlayOutCard  101......\n", kDataWrite.lUserId);
		getPlayOutCard(pkQueueDeskDataNode, iQueueUserAiIdx, &(kDataWrite.lCardType));
	}

	pkQueueDeskDataNode->ullTimeEnterRun = GetTickCount();

	int curOutCardUserDeskIdx = pkQueueDeskDataNode->lQueueUserDeskIdx;
	int curOutCardUserId = curOutCardUserDeskIdx != -1 ? pkQueueDeskDataNode->pkQueueUserDataNode[pkQueueDeskDataNode->lQueueUserDeskIdx]->lUserId : 0;


	char pcCardsCurTmp[80];
	char pcCardsOutTmp[80];
	memset(pcCardsCurTmp, 0, sizeof(pcCardsCurTmp));
	memset(pcCardsOutTmp, 0, sizeof(pcCardsOutTmp));
	getCardsInformation(pkQueueDeskDataNode->pcCardsCur, pcCardsCurTmp);
	getCardsInformation(pkQueueDeskDataNode->pcCardsOutAi, pcCardsOutTmp);
	pcCardsCurTmp[79] = 0;
	pcCardsOutTmp[79] = 0;

	if (MT_CARD_TERMINATE == pkQueueDeskDataNode->pcCardsOutAi[0])
	{
		kDataWrite.lCardType  = MT_CARD_TYPE_PASS;
	}
	else 
	{
		memcpy(kDataWrite.pcCard, pkQueueDeskDataNode->pcCardsOutAi, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
	}

	mtQueuePacket::DataNode* pkQueuePacketDataNode;
	int iQueueUserDataNodeIdx;
	ULONGLONG ullStartTimeTmp = *pullStartTime;
	for (iQueueUserDataNodeIdx = 0; iQueueUserDataNodeIdx < MT_NODE_PLAYER; iQueueUserDataNodeIdx++)
	{
		/// ���͡������ơ� ��Ϣ
		//  ��ǰ��ʵ�û�
		if (pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx]
		 && mtQueueDesk::E_PLAYER_MODE_USER == pkQueueDeskDataNode->plPlayerMode[iQueueUserDataNodeIdx])
		{
			pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueuePacket->popInitDataNode("cardout285");
			if (pkQueuePacketDataNode)
			{
				pkQueuePacketDataNode->kWSABuf.len  = kDataWrite.lStructBytes;
				memcpy(pkQueuePacketDataNode->pcData, &kDataWrite, kDataWrite.lStructBytes);
			//	mtPrint(&kDataWrite);
				/// �������ӳٴ���ai����
				mtQueueSendPackAi::DataNode* pkDataNodeAi = (mtQueueSendPackAi::DataNode*)m_pkQueueMgr->m_pkQueueSendPackAi->popInitDataNode();
				if (pkDataNodeAi)
				{
					int iInsertHead = 0;
					pkDataNodeAi->ullTimeSend           = *pullStartTime + MT_SERVICE_AI_CARD_OUT_TIME;
					ullStartTimeTmp                     = pkDataNodeAi->ullTimeSend;
					pkDataNodeAi->iType                 = mtQueueSendPackAi::E_SEND_TYPE_SEND;
					pkDataNodeAi->pkQueueUserDataNode   = pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx];
					pkDataNodeAi->pkQueuePacketDataNode = pkQueuePacketDataNode;
					m_pkQueueMgr->m_pkListManagerAi->push(pkDataNodeAi, &iInsertHead);
					if (iInsertHead)
					{
						SetEvent(m_hEventManagerAi);
					}
				}
			}
		}
	}

	*pullStartTime = ullStartTimeTmp;
	if (MT_CARD_TYPE_PASS != kDataWrite.lCardType)
	{
		/// ���Ƴɹ�������ai���е�����Ϣ
		updateCardsFace(pkQueueDeskDataNode->pcCardsTotal[iQueueUserAiIdx], kDataWrite.pcCard);
		if (MT_CARD_TERMINATE == pkQueueDeskDataNode->pcCardsTotal[iQueueUserAiIdx][0])
		{
			*piContinue = 0;
		}

		/// ���µ�ǰ���Ӵ����������Ϣ
		memcpy(pkQueueDeskDataNode->pcCardsCur, kDataWrite.pcCard, sizeof(kDataWrite.pcCard));
		pkQueueDeskDataNode->lQueueUserDeskIdx    = iQueueUserAiIdx;
		pkQueueDeskDataNode->lCardType            = kDataWrite.lCardType;

		/// ������û����������Ϣ
		updateQueueDeskCardsOut(pkQueueDeskDataNode);
	}

	pkQueueDeskDataNode->lUserDeskIdxCardOutNext = getQueueUserNextQueueUser(iQueueUserAiIdx);
	/// �ж�AI�ǲ����Ѿ�Ӯ��

	return 1;
}

int mtServiceCardOut::getPlayOutCardType(char pcOutCard[], long* plCardType)
{
	int	iCardCount = 0;
	while(MT_CARD_TERMINATE != pcOutCard[iCardCount])
	{
		iCardCount++;
	}

	if (iCardCount == 2)
	{
		if (((pcOutCard[0] & 0x0f) == 13 && (pcOutCard[1] & 0x0f) == 14) 
			||((pcOutCard[1] & 0x0f) == 13 && (pcOutCard[0] & 0x0f) == 14) )
		{
			*plCardType  = MT_CARD_TYPE_ROCKET;
		}
	}
	else if (iCardCount == 4)
	{
		if ((pcOutCard[0] & 0x0f) == (pcOutCard[1] & 0x0f) 
			&& (pcOutCard[0] & 0x0f) == (pcOutCard[2] & 0x0f) 
			&& (pcOutCard[0] & 0x0f) == (pcOutCard[3] & 0x0f))
		{
			*plCardType  = MT_CARD_TYPE_BOMB;
		}
	}

	return 1;
}

int mtServiceCardOut::getPlayOutCardFirst(mtQueueDesk::DataNode* pkQueueDeskDataNode, int iQueueUserIdx)
{
	/// ������ǵ���
	if (mtQueueDesk::E_PLAYER_TYPE_LANDLORD == pkQueueDeskDataNode->plPlayerType[iQueueUserIdx])
	{
		/// ֱ�ӳ���
		return getPlayOutCardFirst(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx], pkQueueDeskDataNode->pcCardsOutAi);
	}

	/// �Լ������ǲ���һ����(��������)
	if (-1 != IsSeriesCard(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx], pkQueueDeskDataNode->pcCardsOutAi))
	{
		/// ֱ�ӳ���
		memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
		return getPlayOutCardFirst(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx], pkQueueDeskDataNode->pcCardsOutAi);
	}

	/// ����¼��ǶԼң���öԼ����е�ʣ������(С�ı����߶Լ�)
	int iQueueUserNextIdx = getQueueUserNextQueueUser(iQueueUserIdx);
	if (pkQueueDeskDataNode->plPlayerType[iQueueUserIdx] != pkQueueDeskDataNode->plPlayerType[iQueueUserNextIdx])
	{
		/// ��öԼ�����ʣ������
		int iCardsEnemyNum = getCardsNum(pkQueueDeskDataNode->pcCardsTotal[iQueueUserNextIdx]);

		/// С��5�ţ�����ҪС��(��ѶԼҸ�������)
		if (5 > iCardsEnemyNum)
		{
			/// ����Լ�����ʣ������
			int iCardsSelfNum = getCardsNum(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx]);

			/// �Լ�����ֻ��1��
			if (1 == iCardsEnemyNum)
			{
				/// ����֧�������
				if (1 <= iCardsSelfNum)
				{
					return playOutCardsNotByType(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx]
					                             , pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TYPE_SINGLE, iCardsSelfNum);
				}
			}
			else if(2 == iCardsEnemyNum) /// ����2��
			{
				/// �������������
				if (2 <= iCardsSelfNum)
				{
					return playOutCardsNotByType(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx]
					                             , pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TYPE_PAIR, iCardsSelfNum);
				}
			}
			else if(3 == iCardsEnemyNum) /// ����3��
			{
				/// ����֧�������
				if (3 <= iCardsSelfNum)
				{
					return playOutCardsNotByType(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx]
					                             , pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TYPE_TRPILE, iCardsSelfNum);
				}
			}
			else if(4 == iCardsEnemyNum) /// ����4��
			{
				/// ������һ�������
				if (4 <= iCardsSelfNum)
				{
					return playOutCardsNotByType(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx]
					                             , pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TYPE_TRIPLE_ONE, iCardsSelfNum);
				}
			}
		}
	}
	
	/// ����ҶԷ��Լ�����ʣ�������(���������Լ�)
	/// ����Լ��������ϵ�λ��
	int iQueueUserSameFamilyIdx = -1;
	if (pkQueueDeskDataNode->plPlayerType[iQueueUserIdx] == pkQueueDeskDataNode->plPlayerType[iQueueUserNextIdx])
	{
		iQueueUserSameFamilyIdx = iQueueUserNextIdx;
	}
	else
	{
		iQueueUserSameFamilyIdx = getQueueUserNextQueueUser(iQueueUserNextIdx);
	}

	/// ��öԷ��Լ�����ʣ������
	int iCardsSameFamilyNum = getCardsNum(pkQueueDeskDataNode->pcCardsTotal[iQueueUserSameFamilyIdx]);

	/// ���ڵ���5�ţ�ֱ�ӳ���
	/// С��5�ţ����Խ�������
	if (5 > iCardsSameFamilyNum)
	{
	    /// ����1��
		if (1 == iCardsSameFamilyNum)
		{
			/// ����֧
			if (-1 != getSingleCard(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx], pkQueueDeskDataNode->pcCardsOutAi, 0))
			{
				return MT_CARD_TYPE_SINGLE;
			}
		}
		else if(2 == iCardsSameFamilyNum) /// ����2��
		{
	        /// �����ӣ�û�ж��ӳ���֧
			if (-1 != getPairCard(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx], pkQueueDeskDataNode->pcCardsOutAi, 0))
			{
				return MT_CARD_TYPE_PAIR;
			}

			if (-1 != getSingleCard(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx], pkQueueDeskDataNode->pcCardsOutAi, 0))
			{
				return MT_CARD_TYPE_SINGLE;
			}
		}
	    else if(3 == iCardsSameFamilyNum) /// ����3��
		{
	        /// ����֧��û����֧�����ӣ�û�ж��ӳ���֧
			if (-1 != getTripleCard(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx], pkQueueDeskDataNode->pcCardsOutAi, 0))
			{
				return MT_CARD_TYPE_TRPILE;
			}

			if (-1 != getPairCard(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx], pkQueueDeskDataNode->pcCardsOutAi, 0))
			{
				return MT_CARD_TYPE_PAIR;
			}

			if (-1 != getSingleCard(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx], pkQueueDeskDataNode->pcCardsOutAi, 0))
			{
				return MT_CARD_TYPE_SINGLE;
			}
		}
		else if(4 == iCardsSameFamilyNum) /// ����4��
		{
	        /// ������һ��û����֧�������ӣ�û�ж��ӳ���֧
			if (-1 != getTripleWithOne(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx], pkQueueDeskDataNode->pcCardsOutAi, 0, 4))
			{
				return MT_CARD_TYPE_TRIPLE_ONE;
			}

			if (-1 != getTripleCard(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx], pkQueueDeskDataNode->pcCardsOutAi, 0))
			{
				return MT_CARD_TYPE_TRPILE;
			}

			if (-1 != getPairCard(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx], pkQueueDeskDataNode->pcCardsOutAi, 0))
			{
				return MT_CARD_TYPE_PAIR;
			}

			if (-1 != getSingleCard(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx], pkQueueDeskDataNode->pcCardsOutAi, 0))
			{
				return MT_CARD_TYPE_SINGLE;
			}
		}
	}

	return getPlayOutCardFirst(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx], pkQueueDeskDataNode->pcCardsOutAi);
}

int mtServiceCardOut::getPlayOutCard(mtQueueDesk::DataNode* pkQueueDeskDataNode, int iQueueUserIdx, long* plCardType)
{
	int iQueueUserNextIdx = getQueueUserNextQueueUser(iQueueUserIdx);
	/// �¼��ǶԼ�
	if (pkQueueDeskDataNode->plPlayerType[iQueueUserIdx] != pkQueueDeskDataNode->plPlayerType[iQueueUserNextIdx])
	{
		/// �¼�����ʣ������
		int iUserNextCardsNum = getCardsNum(pkQueueDeskDataNode->pcCardsTotal[iQueueUserNextIdx]);

		/// ��ǰ��������������
		int iCardsRecvNum     = getCardsNum(pkQueueDeskDataNode->pcCardsCur);
		/// ��ǰ�����û���ʣ�������
		int iCardsCurNum      = getCardsNum(pkQueueDeskDataNode->pcCardsTotal[pkQueueDeskDataNode->lQueueUserDeskIdx]);

		/// ��ǰ�ҵ����������
		int iCardsSelfNum     = getCardsNum(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx]);

		/// �����ǰ�������͵��������ʣ���������(��Ҫ�����ƣ�����߶Լ�)
		if (iUserNextCardsNum == iCardsRecvNum)
		{
			/// ��ǰ�����ǵ�֧
			if (1 == iCardsRecvNum || 2 == iCardsRecvNum || 3 == iCardsRecvNum)
			{

				/// ��ǰ�Ѿ�����Ƶ��û����Լ�,�������е���С��3��
				if (pkQueueDeskDataNode->plPlayerType[iQueueUserIdx] == pkQueueDeskDataNode->plPlayerType[pkQueueDeskDataNode->lQueueUserDeskIdx]
				   && iCardsCurNum < 3)
				{
					memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
				}
				else if(-1 != playOutMaxCardsByType(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx], pkQueueDeskDataNode->pcCardsOutAi
					                      , pkQueueDeskDataNode->lCardType, iCardsSelfNum))
				{

					/// ��ǰ�յ����Ƶ�������Ϣ
					int iMainCardNumRecv = (getMainCardFromRecv(pkQueueDeskDataNode->lCardType, pkQueueDeskDataNode->pcCardsCur, iCardsRecvNum) & 0x0f);

					/// ��ǰ�Լ�������Ƶ�������Ϣ
					int iMainCardNumSelf = (getMainCardFromRecv(pkQueueDeskDataNode->lCardType, pkQueueDeskDataNode->pcCardsOutAi, iCardsRecvNum) & 0x0f);
					if (iMainCardNumSelf > iMainCardNumRecv && iMainCardNumRecv != 61)
					{
						return 1;
					}
				}
			}
		}
	}

	char pcCardsCurTmp[80];
	memset(pcCardsCurTmp, 0, sizeof(pcCardsCurTmp));
	getCardsInformation(pkQueueDeskDataNode->pcCardsCur, pcCardsCurTmp);
	pcCardsCurTmp[79] = 0;
	MT_DEBUG(4, "CurDeskIdx=%d, CurUserIdx=%d, CardsCur=%s, CardsCurType=%d\n", pkQueueDeskDataNode->lQueueUserDeskIdx
		    , iQueueUserIdx ,pcCardsCurTmp, pkQueueDeskDataNode->lCardType);
	/// ��ñ�����Ҫ���������Ϣ
	memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));

	bool bSplitCard   =bSplitCards(pkQueueDeskDataNode,iQueueUserIdx) ;//�Ƿ�Ҫ����

	int iMainCardsIdx = getPlayOutCard(pkQueueDeskDataNode->lCardType, pkQueueDeskDataNode->pcCardsCur
		, pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx], pkQueueDeskDataNode->pcCardsOutAi,bSplitCard);

	if (MT_CARD_TERMINATE == pkQueueDeskDataNode->pcCardsOutAi[0])
	{
		////MT_DEBUG(4, "User PlayOutCard  301......\n");
		memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
		*plCardType  = MT_CARD_TYPE_PASS;
		return 1;
	}

	getPlayOutCardType(pkQueueDeskDataNode->pcCardsOutAi, plCardType);
	/// �������һ����()
	/// ����Լ�����ʣ������
	int iCardsSelfNum = getCardsNum(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx]);

	/// ���������ȫ�����Դ�ס(һ�ֳ��꣬Ӯ��)
	if (iCardsSelfNum == getCardsNum(pkQueueDeskDataNode->pcCardsOutAi))
	{
		/// �������ƣ�Ӯ��
	}
	else
	{
		/// ��ǰ�Ѿ�����Ƶ��û����Լ�
		if (pkQueueDeskDataNode->plPlayerType[iQueueUserIdx] == pkQueueDeskDataNode->plPlayerType[pkQueueDeskDataNode->lQueueUserDeskIdx])
		{
			//��ǰ�����û����е�����
			int iCardCurNumber = getCardsNum(pkQueueDeskDataNode->pcCardsTotal[pkQueueDeskDataNode->lQueueUserDeskIdx]);
			/// ��ǰ��������������
			int iCardsRecvNum     = getCardsNum(pkQueueDeskDataNode->pcCardsCur);

			//����ԼҴ�����ƺ������е�����һ����������ֻʣ1,2���ƣ��Ҳ�����
			if(iCardCurNumber == iCardsRecvNum || iCardCurNumber == 1 || iCardCurNumber == 2)
			{
				memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
				*plCardType = MT_CARD_TYPE_PASS;
			}
			////MT_DEBUG(4, "User PlayOutCard  302......\n");
			/// ����ҵ�������ը��
			if (MT_CARD_TYPE_ROCKET == *plCardType || MT_CARD_TYPE_BOMB == *plCardType)
			{
				// 			/// ����Լ�����ʣ������
				// 			int iCardsSelfNum = getCardsNum(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx]);
				if (4 == iCardsSelfNum || 2 == iCardsSelfNum)//???�����ǻ����������������
				{
					/// �������ƣ�Ӯ��
				}
				else 
				{
					/// ����ǻ��(˫��)
					if (MT_CARD_TYPE_ROCKET == *plCardType)
					{
						if (3 == iCardsSelfNum)
						{
							/// �������ƣ�Ӯ��
						}
						else
						{
							/// �Ҳ����� pass
							memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
							*plCardType = MT_CARD_TYPE_PASS;
						}
					}
					else /// �Ҳ����� pass
					{
						memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
						*plCardType = MT_CARD_TYPE_PASS;
					}
				}
			}
			else /// ����ҵ����Ʋ���ը��
			{
				/// ����ҵ�������Ϣ
				int iMainCardsNumSelf = (pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx][iMainCardsIdx] & 0x0f);

				/// ��õ�ǰ�����������Ϣ
				int iCardsRecvNum = getCardsNum(pkQueueDeskDataNode->pcCardsCur);
				int iMainCardNumRecv = (getMainCardFromRecv(pkQueueDeskDataNode->lCardType, pkQueueDeskDataNode->pcCardsCur, iCardsRecvNum) & 0x0f);

				// 			/// ����Լ�����ʣ������
				// 			int iCardsSelfNum = getCardsNum(pkQueueDeskDataNode->pcCardsTotal[iQueueUserIdx]);

				/// ����Լ������������ʹ����������һ��
				if (iCardsRecvNum == iCardsSelfNum)
				{
					/// �������ƣ�Ӯ��
				}
				else 
				{
					if (MT_CARD_TYPE_SINGLE == pkQueueDeskDataNode->lCardType)/// ��֧
					{
						///  ��ǰ�������� >= A
						if (11 <= iMainCardNumRecv)
						{
							/// �Ҳ����� pass
							memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
							*plCardType = MT_CARD_TYPE_PASS;
						}
						else ///  ��ǰ�������� < A
						{
							/// �ҵ����� <= A
							if (11 >= iMainCardsNumSelf)
							{
								/// ��������
							}
							else /// �ҵ����� > A
							{
								/// �Ҳ����� pass
								memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
								*plCardType = MT_CARD_TYPE_PASS;
							}
						}
					}
					else if(MT_CARD_TYPE_PAIR == pkQueueDeskDataNode->lCardType) /// ����
					{
						/// ��ǰ�������� >= K
						if (10 <= iMainCardNumRecv)
						{
							/// �Ҳ����� pass
							memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
							*plCardType = MT_CARD_TYPE_PASS;
						}
						else /// ��ǰ�������� < K
						{
							/// �ҵ����� < K
							if (10 > iMainCardsNumSelf)
							{
								/// ��������
							}
							else /// �ҵ����� >= K
							{
								/// �Ҳ����� pass
								memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
								*plCardType = MT_CARD_TYPE_PASS;
							}
						}
					}
					else if(MT_CARD_TYPE_TRPILE == pkQueueDeskDataNode->lCardType
						|| MT_CARD_TYPE_TRIPLE_ONE == pkQueueDeskDataNode->lCardType
						|| MT_CARD_TYPE_TRIPLE_TWO == pkQueueDeskDataNode->lCardType) /// ��֧
					{
						/// ��ǰ�������� >= K
						if (10 <= iMainCardNumRecv)
						{
							/// �Ҳ����� pass
							memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
							*plCardType = MT_CARD_TYPE_PASS;
						}
						else /// ��ǰ�������� < K
						{
							/// �ҵ����� < J
							if (8 > iMainCardsNumSelf)
							{
								/// ��������
							}
							else /// �ҵ����� >= J
							{
								/// �Ҳ����� pass
								memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
								*plCardType = MT_CARD_TYPE_PASS;
							}
						}
					}
					else /// ��ǰ������������
					{
						/// �Ҳ����� pass
						memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
						*plCardType = MT_CARD_TYPE_PASS;
					}
				}
			}
		}
		else /// ��ǰ�Ѿ�����Ƶ��û��ǶԼ�
		{
			///MT_DEBUG(4, "User PlayOutCard  303......\n");
			/// ��õ�ǰ�����������Ϣ
			int iCardsRecvNum = getCardsNum(pkQueueDeskDataNode->pcCardsCur);
			int iMainCardNumRecv = getMainCardFromRecv(pkQueueDeskDataNode->lCardType, pkQueueDeskDataNode->pcCardsCur, iCardsRecvNum);

			/// ը��
			if (MT_CARD_TYPE_ROCKET == pkQueueDeskDataNode->lCardType 
				|| MT_CARD_TYPE_BOMB == pkQueueDeskDataNode->lCardType)
			{
				////MT_DEBUG(4, "User PlayOutCard  304......\n");
				/// ����������(����ը����Ҫ��)
			}
			else /// ��ǰ������������
			{
				////MT_DEBUG(4, "User PlayOutCard  305......\n");
				/// ����ҵ�������ը��
				if (MT_CARD_TYPE_ROCKET == *plCardType || MT_CARD_TYPE_BOMB == *plCardType)
				{
					/// ����Լ����о�ʣ��1��2���ƣ��Ҿͳ���(��ֹ�Լ�ʣһ�����ߵ�)
					int iUserCardsNumEnemy = getCardsNum(pkQueueDeskDataNode->pcCardsTotal[pkQueueDeskDataNode->lQueueUserDeskIdx]);
					if (1 == iUserCardsNumEnemy || 2 == iUserCardsNumEnemy)
					{
						/// ����������(ը����Ҫ��)
					}
					else
					{
						/// ������¼����Լ�
						if (pkQueueDeskDataNode->plPlayerType[iQueueUserIdx] == pkQueueDeskDataNode->plPlayerType[iQueueUserNextIdx])
						{
							/// ����¼������Ƶĸ���
							int iCardsSameFamilyNum = getCardsNum(pkQueueDeskDataNode->pcCardsTotal[iQueueUserNextIdx]);

							/// ������¼ҵ������������ڵ�ǰ���������,(���ܴ��Ҫ��)
							if (iCardsSameFamilyNum < iCardsRecvNum)
							{
								/// ����������(ը����Ҫ��)
							}
							else /// �Ҳ����� pass
							{
								memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
								*plCardType = MT_CARD_TYPE_PASS;
							}

						}
						else /// ������¼��ǶԼ�
						{
							if(iUserCardsNumEnemy > 10)///����Լҵ��ƴ���10�ţ�������
							{
								memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
								*plCardType = MT_CARD_TYPE_PASS;
							}
							/// ����������(ը����Ҫ��)
						}
					}
				}
				else /// ����ҵ����Ʋ���ը��
				{
					////MT_DEBUG(4, "User PlayOutCard  306......\n");
					/// ����������
				}
			}
		}
	}

	return 1;
}

int mtServiceCardOut::updateCardsFace(char* pcSelfCard,char* pcOutCard)
{
	int iCardIdx	= 0;
	if(*pcOutCard == MT_CARD_TERMINATE)
	{
		return 1;
	}
	while(pcOutCard[iCardIdx] != MT_CARD_TERMINATE)
	{
		char cTmpCard	= pcOutCard[iCardIdx];
		rearrangeCards(pcSelfCard,cTmpCard);
		iCardIdx++;
	}

	return	1;
}

void mtServiceCardOut::rearrangeCards(char* pcSelfCards,char cCard)
{
	int iCardIdx	= 0;
	for(;pcSelfCards[iCardIdx] != MT_CARD_TERMINATE; iCardIdx++)
	{
		if(pcSelfCards[iCardIdx] == cCard)
		{
			break;
		}
	}
	for(;pcSelfCards[iCardIdx] != MT_CARD_TERMINATE;iCardIdx++)
	{
		pcSelfCards[iCardIdx]	= pcSelfCards[iCardIdx + 1];
	}
}

int mtServiceCardOut::IsSeriesCard(char pcSelfCard[],char pcOutCard[])
{
	int iSelfCardsNum = 0;
	while(pcSelfCard[iSelfCardsNum] != MT_CARD_TERMINATE)
	{
		iSelfCardsNum++;
	}

	if(iSelfCardsNum <= 10)
	{
		if(iSelfCardsNum == 10 || iSelfCardsNum == 8)
		{
			if(getAirPlane(pcSelfCard,pcOutCard,0,iSelfCardsNum) >= 0)
			{
				return MT_CARD_TYPE_AIRPLANE;
			}
			if(getPairProgression(pcSelfCard,pcOutCard,0,iSelfCardsNum)>=0)
			{
				return MT_CARD_TYPE_PAIR_PROGRESSION;
			}
		}
		if(iSelfCardsNum == 9)
		{
			if(getTripleProgression(pcSelfCard,pcOutCard,0,iSelfCardsNum)>=0)
			{
				return MT_CARD_TYPE_TRIPLE_PROGRESSION;
			}
		}
		if(iSelfCardsNum == 6)
		{
			if(getTripleProgression(pcSelfCard,pcOutCard,0,iSelfCardsNum)>=0)
			{
				return MT_CARD_TYPE_TRIPLE_PROGRESSION;
			}
			if(getPairProgression(pcSelfCard,pcOutCard,0,iSelfCardsNum)>=0)
			{
				return MT_CARD_TYPE_PAIR_PROGRESSION;
			}
		}
		if(iSelfCardsNum >= 5)
		{
			if(getSingleProgression(pcSelfCard,pcOutCard,0,iSelfCardsNum)>=0)
			{
				return MT_CARD_TYPE_SINGLE_PROGRESSION;
			}
			if(iSelfCardsNum == 5)
			{
				if(getTripleWithTwo(pcSelfCard,pcOutCard,0,iSelfCardsNum)>=0)
				{
					return MT_CARD_TYPE_TRIPLE_TWO;
				}
			}
		}
		if(iSelfCardsNum == 4)
		{
			if(getTripleWithOne(pcSelfCard,pcOutCard,0,iSelfCardsNum)>=0)
			{
				return MT_CARD_TYPE_TRIPLE_ONE;
			}
		}
	}

	return -1;
}

int mtServiceCardOut::getCardsNum(char pcCard[])
{
	int iCardsNum = 0;
	while(pcCard[iCardsNum] != MT_CARD_TERMINATE)
	{
		iCardsNum++;
	}

	return iCardsNum;
}

int mtServiceCardOut::getPlayOutCardFirst(char pcSelfCard[],char pcOutCard[])
{
	int iCardType = 0;

	int iCardsNum = 0;
	while(pcSelfCard[iCardsNum] != MT_CARD_TERMINATE)
	{
		iCardsNum++;
	}

	//if(iCardsNum <= 10)
	{
		if(iCardsNum == 10)
		{
			if(getAirPlane(pcSelfCard,pcOutCard,0,iCardsNum) >= 0)
			{
				return MT_CARD_TYPE_AIRPLANE;
			}
			if(getPairProgression(pcSelfCard,pcOutCard,0,iCardsNum)>=0)
			{
				return MT_CARD_TYPE_PAIR_PROGRESSION;
			}
			if(getSingleProgression(pcSelfCard,pcOutCard,0,iCardsNum)>=0)
			{
				return MT_CARD_TYPE_SINGLE_PROGRESSION;
			}
		}
		if(iCardsNum >= 9)
		{
			if(getTripleProgression(pcSelfCard,pcOutCard,0,9)>=0)
			{
				return MT_CARD_TYPE_TRIPLE_PROGRESSION;
			}
			if(getSingleProgression(pcSelfCard,pcOutCard,0,9)>=0)
			{
				return MT_CARD_TYPE_SINGLE_PROGRESSION;
			}
		}
		if(iCardsNum >= 8)
		{
			if(getAirPlane(pcSelfCard,pcOutCard,0,8) >= 0)
			{
				return MT_CARD_TYPE_AIRPLANE;
			}
			if(getPairProgression(pcSelfCard,pcOutCard,0,8)>=0)
			{
				return MT_CARD_TYPE_PAIR_PROGRESSION;
			}
			if(getSingleProgression(pcSelfCard,pcOutCard,0,8)>=0)
			{
				return MT_CARD_TYPE_SINGLE_PROGRESSION;
			}
		}
		if(iCardsNum >= 6)
		{
			if(getTripleProgression(pcSelfCard,pcOutCard,0,6)>=0)
			{
				return MT_CARD_TYPE_TRIPLE_PROGRESSION;
			}
			if(getPairProgression(pcSelfCard,pcOutCard,0,6)>=0)
			{
				return MT_CARD_TYPE_PAIR_PROGRESSION;
			}
			if(getSingleProgression(pcSelfCard,pcOutCard,0,6)>=0)
			{
				return MT_CARD_TYPE_SINGLE_PROGRESSION;
			}
		}
		if(iCardsNum >= 5)
		{
			if(getSingleProgression(pcSelfCard,pcOutCard,0,5)>=0)
			{
				return MT_CARD_TYPE_SINGLE_PROGRESSION;
			}

			if(getTripleWithTwo(pcSelfCard,pcOutCard,0,5)>=0)
			{
				return MT_CARD_TYPE_TRIPLE_TWO;
			}

		}
		if(iCardsNum >= 4)
		{
			if(getTripleWithOne(pcSelfCard,pcOutCard,0,4)>=0)
			{
				return MT_CARD_TYPE_TRIPLE_ONE;
			}
		}
	}
	if((pcSelfCard[0] & 0x0f) == 13 && (pcSelfCard[1] & 0x0f)==14 && pcSelfCard[2] == MT_CARD_TERMINATE)
	{
		pcOutCard[0]	= pcSelfCard[0];
		pcOutCard[1]	= pcSelfCard[1];
		return MT_CARD_TYPE_ROCKET;
	}

	int iIdx = 0;

	while(pcSelfCard[iIdx] != MT_CARD_TERMINATE)
	{
		if(pcSelfCard[iIdx + 1] == MT_CARD_TERMINATE)
		{
			pcOutCard[0]	= pcSelfCard[iIdx];
			iCardType	= MT_CARD_TYPE_SINGLE;
			break;
		}
		else if((pcSelfCard[iIdx] & 0x0f) != (pcSelfCard[iIdx + 1] & 0x0f))
		{
			pcOutCard[0]	= pcSelfCard[iIdx];
			iCardType	= MT_CARD_TYPE_SINGLE;
			break;
		}
		else if(pcSelfCard[iIdx + 2] == MT_CARD_TERMINATE)
		{
			pcOutCard[0]	= pcSelfCard[iIdx];
			pcOutCard[1]	= pcSelfCard[iIdx+1];
			iCardType		= MT_CARD_TYPE_PAIR;
			break;
		}
		else if((pcSelfCard[iIdx] & 0x0f) != (pcSelfCard[iIdx + 2] & 0x0f))
		{
			pcOutCard[0]	= pcSelfCard[iIdx];
			pcOutCard[1]	= pcSelfCard[iIdx+1];
			iCardType		= MT_CARD_TYPE_PAIR;
			break;
		}
		else if(pcSelfCard[iIdx + 3] == MT_CARD_TERMINATE)
		{
			pcOutCard[0]	= pcSelfCard[iIdx];
			pcOutCard[1]	= pcSelfCard[iIdx+1];
			pcOutCard[2]	= pcSelfCard[iIdx+2];
			iCardType		= MT_CARD_TYPE_TRPILE;
			break;
		}
		else if((pcSelfCard[iIdx] & 0x0f) != (pcSelfCard[iIdx + 3] & 0x0f))
		{
			pcOutCard[0]	= pcSelfCard[iIdx];
			pcOutCard[1]	= pcSelfCard[iIdx+1];
			pcOutCard[2]	= pcSelfCard[iIdx+2];
			int iResult	= getSingleCard(pcSelfCard,pcOutCard+3,0);
			if(iResult >= 0)
			{
				iCardType		= MT_CARD_TYPE_TRIPLE_ONE;
				break;
			}
			else
			{
				iResult	= getPairCard(pcSelfCard,pcOutCard+3,0);
				if(iResult >= 0)
				{
					iCardType		= MT_CARD_TYPE_TRIPLE_TWO;

					break;
				}
				else
				{
					iCardType		= MT_CARD_TYPE_TRPILE;
					break;
				}
			}
		}
		else
		{
			iIdx+=4;
		}
	}
	if(pcSelfCard[iIdx] != MT_CARD_TERMINATE)
	{
		return iCardType;
	}
	else
	{
		pcOutCard[0]	= pcSelfCard[0];
		pcOutCard[1]	= pcSelfCard[1];
		pcOutCard[2]	= pcSelfCard[2];
		pcOutCard[3]	= pcSelfCard[3];
		return MT_CARD_TYPE_BOMB;
	}
}

int mtServiceCardOut::getPlayOutCard(int iCardType,char pcRecvCard[],char pcSelfCard[],char pcOutCard[],bool bSplitCard)
{
	int iSelfIdx = 0;
	int iRecvIdx = 0;
	int iCardsRecvNum = 0;
	int iMainCard;
	int	iResult	= -1;///>=0 ��ʾ�õ����Ƶ���ʼ����

	int iCardsNum = 0;
	while(pcSelfCard[iCardsNum] != MT_CARD_TERMINATE)
	{
		iCardsNum++;
	}
	if(iCardType == MT_CARD_TYPE_ROCKET)
	{
		return iResult;
	}
	while(pcRecvCard[iCardsRecvNum]!= MT_CARD_TERMINATE)
	{
		iCardsRecvNum++;
	}
	iMainCard	= getMainCardFromRecv(iCardType,pcRecvCard,iCardsRecvNum);
	MT_DEBUG(4, "iMainCard=%d\n", (iMainCard & 0X0F));
	for(;pcSelfCard[iSelfIdx] != MT_CARD_TERMINATE;iSelfIdx++)
	{
		if((pcSelfCard[iSelfIdx] & 0x0f) > (iMainCard & 0x0f))
		{
			break;
		}
	}

	if ((pcSelfCard[iSelfIdx] == MT_CARD_TERMINATE))
	{
		if(iCardType != MT_CARD_TYPE_BOMB)
		{
			iSelfIdx = 0;
			iResult = getBombCard(pcSelfCard,pcOutCard,iSelfIdx);
			return 	iResult;
		}
		else
		{
			return iResult;
		}
	}


	switch(iCardType)
	{
	case MT_CARD_TYPE_SINGLE:
		{
			iResult	= getSingleCard(pcSelfCard,pcOutCard,iSelfIdx);

			if(iResult == -1 && bSplitCard)//���û���ҳ����ţ��ҿ��Բ��ƣ����һ������
			{
				iResult	= forceGetSingleCard(pcSelfCard,pcOutCard,iSelfIdx);
			}
		}
		break;
	case MT_CARD_TYPE_PAIR:
		{
			iResult	= getPairCard(pcSelfCard,pcOutCard,iSelfIdx);

			if(iResult == -1 && bSplitCard)//���û���ҳ����ӣ��ҿ��Բ��ƣ����һ������
			{
				iResult	= forceGetPairCard(pcSelfCard,pcOutCard,iSelfIdx);
			}
		}
		break;
	case MT_CARD_TYPE_TRPILE:
		{
			if(iCardsNum > 2)
			{
				iResult	= getTripleCard(pcSelfCard,pcOutCard,iSelfIdx);
			}
			
		}
		break;
	case MT_CARD_TYPE_TRIPLE_ONE:
		{
			if(iCardsNum > 3)
			{
				iResult	= getTripleWithOne(pcSelfCard,pcOutCard,iSelfIdx,iCardsRecvNum);
			}
		}
		break;
	case MT_CARD_TYPE_TRIPLE_TWO:
		{
			if(iCardsNum > 4)
			{
				iResult	= getTripleWithTwo(pcSelfCard,pcOutCard,iSelfIdx,iCardsRecvNum);
			}
		}
		break;
	case MT_CARD_TYPE_SINGLE_PROGRESSION:
		{
			if(iCardsNum > 4)
			{
				iResult	= getSingleProgression(pcSelfCard,pcOutCard,iSelfIdx,iCardsRecvNum);
			}
		}
		break;
	case MT_CARD_TYPE_PAIR_PROGRESSION:
		{
			if(iCardsNum > 5)
			{
				iResult	= getPairProgression(pcSelfCard,pcOutCard,iSelfIdx,iCardsRecvNum);
			}
		}
		break;
	case MT_CARD_TYPE_TRIPLE_PROGRESSION:
		{
			if(iCardsNum > 5)
			{
				iResult	= getTripleProgression(pcSelfCard,pcOutCard,iSelfIdx,iCardsRecvNum);
			}
		}
		break;
	case MT_CARD_TYPE_AIRPLANE:
		{
			if(iCardsNum > 7)
			{
				iResult	= getAirPlane(pcSelfCard,pcOutCard,iSelfIdx,iCardsRecvNum);
			}
		}
		break;
	case MT_CARD_TYPE_FOUR_TWO:
		{
			iResult = getBombCard(pcSelfCard,pcOutCard,0);
		}
	case MT_CARD_TYPE_BOMB:
		{
			iResult = getBombCard(pcSelfCard,pcOutCard,iSelfIdx);
		}
		break;
	default:
		break;
	}

	if(iResult < 0 && iCardType != MT_CARD_TYPE_BOMB)
	{
		iSelfIdx = 0;
		iResult = getBombCard(pcSelfCard,pcOutCard,iSelfIdx);
	}
	if(iResult < 0)
	{
		return getRocketCard(pcSelfCard,pcOutCard);
	}
	else
	{
		return iResult;
	}
}

int mtServiceCardOut::playOutMaxCardsByType(char pcSelfCard[],char pcOutCard[] ,int iCardType, int iSelfCardsNum)
{
	switch(iCardType)
	{
	case MT_CARD_TYPE_SINGLE:
		{
			pcOutCard[0] = pcSelfCard[iSelfCardsNum - 1];
			return MT_CARD_TYPE_SINGLE;
		}
	case MT_CARD_TYPE_PAIR:
		{
			char pcTmpCards[4];
			memset(pcTmpCards,100,4);
			int iCardIdx = getPairCard(pcSelfCard,pcTmpCards,0);
			while(iCardIdx >= 0)
			{
				pcOutCard[0]	= pcTmpCards[0];
				pcOutCard[1]	= pcTmpCards[1];
				iCardIdx = getPairCard(pcSelfCard,pcTmpCards,iCardIdx + 2);
			}
			if(pcOutCard[0] == 100)
			{
				return -1;
			}
			else
			{
				return MT_CARD_TYPE_PAIR;
			}
		}
	case MT_CARD_TYPE_TRPILE:
		{
			char pcTmpCards[4];
			memset(pcTmpCards,100,4);
			int iCardIdx = getTripleCard(pcSelfCard,pcTmpCards,0);
			while(iCardIdx >= 0)
			{
				pcOutCard[0]	= pcTmpCards[0];
				pcOutCard[1]	= pcTmpCards[1];
				pcOutCard[2]	= pcTmpCards[2];
				iCardIdx = getPairCard(pcSelfCard,pcTmpCards,iCardIdx+3);
			}
			if(pcOutCard[0] == 100)
			{
				return -1;
			}
			else
			{
				return MT_CARD_TYPE_TRPILE;
			}
		}
	}

	return -1;
}

int mtServiceCardOut::judgeSingleProgression(char* pcCards,int iCardsNum)
{
	if(iCardsNum < 5)
	{
		return -1;
	}
	int iIdxCount = 1;
	for(;iIdxCount < iCardsNum; iIdxCount++)
	{
		if((pcCards[0] & 0x0f) + iIdxCount != (pcCards[iIdxCount] & 0x0f)
			|| (pcCards[iIdxCount] & 0x0f) >= 12)
		{
			break;
		}
	}
	if(iIdxCount < iCardsNum)
	{
		return -1;
	}
	else
	{
		return pcCards[0];
	}
}

int mtServiceCardOut::judgePairProgression(char* pcCards,int iCardsNum)
{
	if(iCardsNum < 3)
	{
		return -1;
	}
	int iIdxCount = 0;
	for(;iIdxCount  < iCardsNum; iIdxCount+=2)
	{
		if((pcCards[iIdxCount] & 0x0f) >= 12)
		{
			break;
		}
		else if((pcCards[iIdxCount] & 0x0f) != (pcCards[iIdxCount + 1] & 0x0f))
		{
			break;
		}
		else if(iIdxCount < iCardsNum - 2)
		{
			if((pcCards[iIdxCount] & 0x0f) + 1 != (pcCards[iIdxCount + 2] & 0x0f))
			{
				break;
			}
		}
	}
	if(iIdxCount < iCardsNum)
	{
		return -1;
	}
	else
	{
		return pcCards[0];
	}
}

int mtServiceCardOut::judgeTripleProgression(char* pcCards,int iCardsNum)
{
	if(iCardsNum < 2)
	{
		return -1;
	}
	int iIdxCount = 0;
	for(;iIdxCount  < iCardsNum; iIdxCount+=3)
	{
		if((pcCards[iIdxCount] & 0x0f) >= 12)
		{
			break;
		}
		else if((pcCards[iIdxCount] & 0x0f) != (pcCards[iIdxCount + 2] & 0x0f))
		{
			break;
		}
		else if(iIdxCount < iCardsNum - 3)
		{
			if((pcCards[iIdxCount] & 0x0f) + 1 != (pcCards[iIdxCount + 3] & 0x0f))
			{
				break;
			}
		}
	}
	if(iIdxCount < iCardsNum)
	{
		return -1;
	}
	else
	{
		return pcCards[0];
	}
}

int mtServiceCardOut::judgeTripleWithOne(char* pcCards,int iCardsNum)
{
	if(iCardsNum != 4)
	{
		return -1;
	}
	if((pcCards[0] & 0x0f)== (pcCards[2] & 0x0f) && (pcCards[0] & 0x0f) != (pcCards[3] & 0x0f))
	{
		return pcCards[0];
	}
	else if((pcCards[0] & 0x0f) != (pcCards[1] & 0x0f) && (pcCards[1] & 0x0f) == (pcCards[3] & 0x0f))
	{
		return pcCards[1];
	}
	else
	{
		return -1;
	}
}

int mtServiceCardOut::judgeTripleWithTwo(char* pcCards,int iCardsNum)
{
	if(iCardsNum != 5)
	{
		return -1;
	}
	if((pcCards[0] & 0x0f) == (pcCards[2] & 0x0f) && (pcCards[0] & 0x0f) != (pcCards[3] & 0x0f)
		&& (pcCards[3] & 0x0f) == (pcCards[4] & 0x0f))
	{
		return pcCards[0];
	}
	else if((pcCards[0] & 0x0f) == (pcCards[1] & 0x0f) && (pcCards[0] & 0x0f) != (pcCards[2] & 0x0f)
		&& (pcCards[2] & 0x0f) == (pcCards[4] & 0x0f))
	{
		return pcCards[2];
	}
	else
	{
		return -1;
	}
}

int mtServiceCardOut::judgeFourWithTwo(char* pcCards,int iCardsNum)
{
	if(iCardsNum != 6 && iCardsNum != 8)
	{
		return -1;
	}
	if(iCardsNum == 6)
	{
		if((pcCards[0] & 0x0f) == (pcCards[3] & 0x0f) && (pcCards[4] & 0x0f) != (pcCards[5] & 0x0f))
		{
			return pcCards[0];
		}
		else if((pcCards[1] & 0x0f) == (pcCards[4] & 0x0f) && (pcCards[0] & 0x0f) != (pcCards[5] & 0x0f))
		{
			return pcCards[1];
		}
		else if((pcCards[2] & 0x0f) == (pcCards[5] & 0x0f) && (pcCards[0] & 0x0f) != (pcCards[1] & 0x0f))
		{
			return pcCards[2];
		}
		else
		{
			return -1;
		}
	}
	else if(iCardsNum == 8)
	{
		if((pcCards[0] & 0x0f) == (pcCards[3] & 0x0f) && (pcCards[4] & 0x0f) == (pcCards[5] & 0x0f)
			&& (pcCards[6] & 0x0f) == (pcCards[7] & 0x0f))
		{
			return pcCards[0];
		}
		else if((pcCards[2] & 0x0f) == (pcCards[5] & 0x0f) && (pcCards[0] & 0x0f) == (pcCards[1] & 0x0f)
			&& (pcCards[6] & 0x0f) == (pcCards[7] & 0x0f))
		{
			return pcCards[2];
		}
		else if((pcCards[4] & 0x0f) == (pcCards[7] & 0x0f) && (pcCards[0] & 0x0f) == (pcCards[1] & 0x0f)
			&& (pcCards[2] & 0x0f) == (pcCards[3] & 0x0f))
		{
			return pcCards[4];
		}
		else
		{
			return -1;
		}
	}

	return -1;
}

int mtServiceCardOut::judgeAirPlane(char* pcCards,int iCardsNum)
{
	if((iCardsNum % 4) !=  0 && (iCardsNum % 5) != 0)
	{
		return -1;
	}

	if(iCardsNum % 4 == 0)
	{
		int	iCount	= iCardsNum>>2;
		if((pcCards[0] & 0x0f) +1 == (pcCards[5] & 0x0f))
		{
			return judgeTripleProgression(pcCards,iCardsNum - iCount);
		}
		else if((pcCards[1] & 0x0f) +1 == (pcCards[6] & 0x0f))
		{
			return judgeTripleProgression(pcCards + 1,iCardsNum - iCount);
		}
		else if((pcCards[2] & 0x0f) +1 == (pcCards[7] & 0x0f))
		{
			return judgeTripleProgression(pcCards + 2,iCardsNum - iCount);
		}
		else if((pcCards[3] & 0x0f) + 1 == (pcCards[8] & 0x0f))
		{
			return judgeTripleProgression(pcCards + 3,iCardsNum - iCount);
		}
		else
		{
			return -1;
		}
	}
	else
	{
		int	iCount	= iCardsNum/5;
		int iCardIdx = 0;
		int iResult	= -1;
		while (iCardIdx < iCardsNum)
		{
			if((pcCards[iCardIdx] & 0x0f) != (pcCards[iCardIdx+1] & 0x0f))
			{
				iResult = -1;
				break;
			}
			else if((pcCards[iCardIdx] & 0x0f)== (pcCards[iCardIdx+2] & 0x0f))
			{
				if(judgeTripleProgression(pcCards + iCardIdx,(iCardsNum - iCount*3))>=0)
				{
					iResult = pcCards[iCardIdx];
					iCardIdx += (iCardsNum - iCount*3);
				}
				else
				{
					iResult = -1;
					break;
				}
			}
			else
			{
				iCardIdx+=2;
			}
		}
		return iResult;
	}

}

int mtServiceCardOut:: getSingleCard(char* pcSelfCards, char* cOutCard,int iCardPos)
{
	int iSelfIdx	= iCardPos;
	while(pcSelfCards[iSelfIdx + 3] != MT_CARD_TERMINATE)
	{//0,1,2,3
		if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+1] & 0x0f))
		{
			break;
		}
		else if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+2] & 0x0f))
		{
			iSelfIdx += 2;
		}
		else if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+3] & 0x0f))
		{
			iSelfIdx += 3;
		}
		else
		{
			iSelfIdx += 4;
		}
	}
	if(pcSelfCards[iSelfIdx + 3] == MT_CARD_TERMINATE)
	{
		if(pcSelfCards[iSelfIdx + 2] != MT_CARD_TERMINATE)
		{
			if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+1] & 0x0f))
			{
				cOutCard[0]	= pcSelfCards[iSelfIdx];
				return iSelfIdx;
			}
			else if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+2] & 0x0f))
			{
				cOutCard[0]	= pcSelfCards[iSelfIdx+2];
				return iSelfIdx+2;
			}
			else
			{
				return		-1;
			}
		}
		else if(pcSelfCards[iSelfIdx + 1] != MT_CARD_TERMINATE)
		{
			if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+1] & 0x0f))
			{
				cOutCard[0]	= pcSelfCards[iSelfIdx];
				return iSelfIdx;
			}
			else
			{
				return		-1;
			}
		}
		else if(pcSelfCards[iSelfIdx] != MT_CARD_TERMINATE)
		{
			if((pcSelfCards[iSelfIdx - 1] & 0x0f) != (pcSelfCards[iSelfIdx] & 0x0f))
			{
				cOutCard[0]	= pcSelfCards[iSelfIdx];
				return iSelfIdx;
			}
			else
			{
				return -1;
			}
		}

		return		-1;
	}
	else
	{
		cOutCard[0]	= pcSelfCards[iSelfIdx];
		return iSelfIdx;
	}


}

int mtServiceCardOut:: forceGetSingleCard(char* pcSelfCards, char* cOutCard,int iCardPos)
{
	int iSelfIdx = iCardPos;

	//ը�����ܲ�ɵ���
	while(pcSelfCards[iSelfIdx + 3] != MT_CARD_TERMINATE)
	{
		if((pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+1] & 0x0f)
			&&(pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+2] & 0x0f)
			&&(pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+3] & 0x0f)
			)
		{
			iSelfIdx += 4;
		}
		else
		{
			break;
		}
	}

	//������ܲ�ɵ���
	if((pcSelfCards[iSelfIdx] & 0x0f) == 13 && (pcSelfCards[iSelfIdx + 1] & 0x0f) == 14)
	{
		return -1;
	}
	if(pcSelfCards[iSelfIdx] == MT_CARD_TERMINATE)
	{
		return -1;
	}

	cOutCard[0]  = pcSelfCards[iSelfIdx];
	mtLog("\nforceGetSingleCard");
	return iSelfIdx;
}

int mtServiceCardOut::getPairCard(char* pcSelfCards,char* pcOutCard ,int iCardBegPos)
{
	int iSelfIdx	= iCardBegPos;
	while(pcSelfCards[iSelfIdx + 3] != MT_CARD_TERMINATE)
	{
		if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+1] & 0x0f))
		{
			iSelfIdx++;
		}
		else if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+2] & 0x0f))
		{
			break;
		}
		else if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+3] & 0x0f))
		{
			iSelfIdx += 3;
		}
		else
		{
			iSelfIdx += 4;
		}
	}
	if(pcSelfCards[iSelfIdx + 3] == MT_CARD_TERMINATE)
	{
		///������
		if (pcSelfCards[iSelfIdx+2]!= MT_CARD_TERMINATE)
		{
			if((pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+1] & 0x0f)
				&& (pcSelfCards[iSelfIdx + 1] & 0x0f) != (pcSelfCards[iSelfIdx+2] & 0x0f))
			{
				pcOutCard[0]	= pcSelfCards[iSelfIdx];
				pcOutCard[1]	= pcSelfCards[iSelfIdx + 1];
				return	iSelfIdx;
			}
			else if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+1] & 0x0f) && (pcSelfCards[iSelfIdx + 1] & 0x0f) == (pcSelfCards[iSelfIdx+2] & 0x0f))
			{
				pcOutCard[0]	= pcSelfCards[iSelfIdx + 1];
				pcOutCard[1]	= pcSelfCards[iSelfIdx + 2];
				return	iSelfIdx + 1;
			}
			else
			{
				return -1;
			}
		}
		else if(pcSelfCards[iSelfIdx + 1] != MT_CARD_TERMINATE)
		{
			if((pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+1] & 0x0f))
			{
				pcOutCard[0]	= pcSelfCards[iSelfIdx];
				pcOutCard[1]	= pcSelfCards[iSelfIdx + 1];
				return	iSelfIdx;
			}
			else
			{
				return -1;
			}
		}
		return -1;
	}
	else
	{
		pcOutCard[0]	= pcSelfCards[iSelfIdx];
		pcOutCard[1]	= pcSelfCards[iSelfIdx + 1];
		return	iSelfIdx;
		///�������е���
	}
}

int mtServiceCardOut::forceGetPairCard(char* pcSelfCards,char* pcOutCard,int iCardPos)
{
	int iSelfIdx	= iCardPos;
	while(pcSelfCards[iSelfIdx + 3] != MT_CARD_TERMINATE)
	{
		//ը�����ܲ������
		if((pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+1] & 0x0f)
			&&(pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+2] & 0x0f)
			&&(pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+3] & 0x0f)
			)
		{
			iSelfIdx += 4;
			continue;
		}

		if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+1] & 0x0f))
		{
			iSelfIdx++;
		}
		else
		{
			break;
		}
	}

	char pp[17];

	memcpy(pp,pcSelfCards,17);
	if(pcSelfCards[iSelfIdx] != MT_CARD_TERMINATE && pcSelfCards[iSelfIdx + 1] != MT_CARD_TERMINATE
		&&(pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx + 1] & 0x0f))
	{
		pcOutCard[0] = pcSelfCards[iSelfIdx];
		pcOutCard[1] = pcSelfCards[iSelfIdx + 1];

		mtLog("\nforceGetPairCard");
		return iSelfIdx;
	}
	return -1;

}

int mtServiceCardOut::getTripleCard(char* pcSelfCards,char* pcOutCard,int iCardBegPos)
{
	int iSelfIdx	= iCardBegPos;
	while(pcSelfCards[iSelfIdx + 3] != MT_CARD_TERMINATE)
	{
		if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+1] & 0x0f))
		{
			iSelfIdx++;
		}
		else if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+2] & 0x0f))
		{
			iSelfIdx += 2;
		}
		else if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+3] & 0x0f))
		{
			break;
		}
		else
		{
			iSelfIdx += 4;
		}
	}
	if(pcSelfCards[iSelfIdx + 3] == MT_CARD_TERMINATE)
	{
		///������
		if(pcSelfCards[iSelfIdx+2] != MT_CARD_TERMINATE)
		{
			if((pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+1] & 0x0f)
				&& (pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+2] & 0x0f))
			{
				pcOutCard[0]	= pcSelfCards[iSelfIdx];
				pcOutCard[1]	= pcSelfCards[iSelfIdx + 1];
				pcOutCard[2]	= pcSelfCards[iSelfIdx + 2];
				return iSelfIdx;

			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}

	}
	else
	{
		pcOutCard[0]	= pcSelfCards[iSelfIdx];
		pcOutCard[1]	= pcSelfCards[iSelfIdx + 1];
		pcOutCard[2]	= pcSelfCards[iSelfIdx + 2];
		return iSelfIdx;
		///�������е���
	}

}

int mtServiceCardOut::getBombCard(char* pcSelfCards,char* pcOutCard,int iCardBegPos)
{
	int iSelfIdx	= iCardBegPos;
	while(pcSelfCards[iSelfIdx + 3] != MT_CARD_TERMINATE)
	{
		if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+1] & 0x0f))
		{
			iSelfIdx++;
		}
		else if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+2] & 0x0f))
		{
			iSelfIdx += 2;
		}
		else if((pcSelfCards[iSelfIdx] & 0x0f) != (pcSelfCards[iSelfIdx+3] & 0x0f))
		{
			iSelfIdx += 3;
		}
		else
		{
			break;
		}
	}
	if(pcSelfCards[iSelfIdx+3] == MT_CARD_TERMINATE)
	{
		return -1;///������
	}
	else
	{
		pcOutCard[0]	= pcSelfCards[iSelfIdx];
		pcOutCard[1]	= pcSelfCards[iSelfIdx + 1];
		pcOutCard[2]	= pcSelfCards[iSelfIdx + 2];
		pcOutCard[3]	= pcSelfCards[iSelfIdx + 3];
		return	 iSelfIdx;
		///�������е���
	}
}

int mtServiceCardOut::getRocketCard(char* pcSelfCards,char* pcOutCard)
{
	int iCardIdx = 0;
	while(pcSelfCards[iCardIdx] != MT_CARD_TERMINATE)
	{
		iCardIdx++;
	}
	if((pcSelfCards[iCardIdx-1] & 0x0f) == 14 && (pcSelfCards[iCardIdx - 2] & 0x0f) == 13)
	{
		pcOutCard[0] = pcSelfCards[iCardIdx - 2];
		pcOutCard[1] = pcSelfCards[iCardIdx - 1];
		return iCardIdx - 2;
	}
	else
	{
		return -1;
	}
}

int mtServiceCardOut::getSingleProgression(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum)
{
	int iSelfIdx	= iCardBegPos;
	char pcTmpCards[20];
	int	 iTmpCount	= 0;
	memset(pcTmpCards,MT_CARD_TERMINATE,20);
	while(pcSelfCards[iSelfIdx + 1] != MT_CARD_TERMINATE && ((pcSelfCards[iSelfIdx] & 0x0f) < 12))
	{
		if((pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+1] & 0x0f)-1)
		{
			pcTmpCards[iTmpCount++]	= pcSelfCards[iSelfIdx];
			iSelfIdx++;
		}
		else if((pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+1] & 0x0f))
		{
			iSelfIdx ++;
		}
		else
		{
			iTmpCount	= 0;
			iSelfIdx++;
		}
		if(iTmpCount >= iCardsNum)
		{
			break;
		}
	}
	if(pcSelfCards[iSelfIdx + 1] == MT_CARD_TERMINATE && ((pcSelfCards[iSelfIdx] & 0x0f) < 12))
	{
		if((pcSelfCards[iSelfIdx - 1] & 0x0f) == (pcSelfCards[iSelfIdx] & 0x0f)-1)
		{
			pcTmpCards[iTmpCount++]	= pcSelfCards[iSelfIdx];
			iSelfIdx++;
		}
	}
	if(iTmpCount < iCardsNum)
	{
		return -1;///������
	}
	else
	{
		int iIdx  = 0;
		for(;iIdx < iCardsNum;iIdx++)
		{
			pcOutCards[iIdx]	= pcTmpCards[iIdx];
		}
		return	 iSelfIdx;
		///�������е���
	}
}

int	mtServiceCardOut::getPairProgression(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum)
{
	int iSelfIdx	= iCardBegPos;
	char pcTmpCards[20];
	int	 iTmpCount	= 0;
	memset(pcTmpCards,MT_CARD_TERMINATE,20);
	if(iCardsNum < 6 || iCardsNum % 2 != 0)
	{
		return -1;
	}
	while(pcSelfCards[iSelfIdx + 2] != MT_CARD_TERMINATE && ((pcSelfCards[iSelfIdx] & 0x0f) < 12))
	{
		if((pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+1] & 0x0f)
			&& (pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+2] & 0x0f) - 1)
		{
			pcTmpCards[iTmpCount++]	= pcSelfCards[iSelfIdx];
			pcTmpCards[iTmpCount++] = pcSelfCards[iSelfIdx + 1];
			iSelfIdx+=2;
		}
		else if((pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+1] & 0x0f)
			&& (pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+2] & 0x0f))
		{
			if((pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+3] & 0x0f))
			{
				iTmpCount	= 0;
				iSelfIdx+=4;
			}
			else
			{
				iSelfIdx++;
			}
		}
		else
		{
			iTmpCount	= 0;
			iSelfIdx++;
		}
		if(iTmpCount >= iCardsNum)
		{
			break;
		}
	}
	if(pcSelfCards[iSelfIdx + 2] == MT_CARD_TERMINATE && ((pcSelfCards[iSelfIdx] & 0x0f) < 12))
	{
		if((pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+1] & 0x0f))
		{
			pcTmpCards[iTmpCount++]	= pcSelfCards[iSelfIdx];
			pcTmpCards[iTmpCount++] = pcSelfCards[iSelfIdx + 1];
			iSelfIdx+=2;
		}
	}
	if(iTmpCount < iCardsNum)
	{
		return -1;
	}
	else
	{
		int iIdx  = 0;
		for(;iIdx < iCardsNum;iIdx++)
		{
			pcOutCards[iIdx]	= pcTmpCards[iIdx];
		}
		return	 iSelfIdx;
		///�������е���
	}
}

int	mtServiceCardOut::getTripleProgression(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum)
{
	int iSelfIdx	= iCardBegPos;
	char pcTmpCards[20];
	int	 iTmpCount	= 0;
	memset(pcTmpCards,MT_CARD_TERMINATE,20);
	if(iCardsNum < 6 || iCardsNum % 3 != 0)
	{
		return -1;
	}
	while(pcSelfCards[iSelfIdx + 3] != MT_CARD_TERMINATE && ((pcSelfCards[iSelfIdx] & 0x0f) < 12))
	{
		if((pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+2] & 0x0f)
			&& (pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+3] & 0x0f)-1)
		{
			pcTmpCards[iTmpCount++]	= pcSelfCards[iSelfIdx];
			pcTmpCards[iTmpCount++] = pcSelfCards[iSelfIdx + 1];
			pcTmpCards[iTmpCount++] = pcSelfCards[iSelfIdx + 2];
			iSelfIdx+=3;
		}
		else if((pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+2] & 0x0f)
			&& (pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+3] & 0x0f))
		{
			iTmpCount = 0;
			iSelfIdx +=4;
		}
		else
		{
			iTmpCount	= 0;
			iSelfIdx++;
		}
		if(iTmpCount >= iCardsNum)
		{
			break;
		}
	}
	if((pcSelfCards[iSelfIdx + 3] == MT_CARD_TERMINATE) && ((pcSelfCards[iSelfIdx] & 0x0f) < 12))
	{
		if((pcSelfCards[iSelfIdx] & 0x0f) == (pcSelfCards[iSelfIdx+2] & 0x0f))
		{
			pcTmpCards[iTmpCount++]	= pcSelfCards[iSelfIdx];
			pcTmpCards[iTmpCount++] = pcSelfCards[iSelfIdx + 1];
			pcTmpCards[iTmpCount++] = pcSelfCards[iSelfIdx + 2];
			iSelfIdx+=3;
		}
	}
	if(iTmpCount < iCardsNum)
	{
		return -1;///������
	}
	else
	{
		int iIdx  = 0;
		for(;iIdx < iCardsNum;iIdx++)
		{
			pcOutCards[iIdx]	= pcTmpCards[iIdx];
		}
		return	 iSelfIdx;
		///�������е���
	}
}

int	mtServiceCardOut::getTripleWithOne(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum)
{
	int iSelfIdx	= iCardBegPos;
	char pcTmpCards[20];
	int	 iTmpCount	= 0;
	int  iResult	= 0;
	memset(pcTmpCards,MT_CARD_TERMINATE,20);
	if(iCardsNum != 4)
	{
		return -1;
	}
	if((iSelfIdx = getTripleCard(pcSelfCards,pcTmpCards,iCardBegPos))>=0)
	{
		iTmpCount += 3;
		if(getSingleCard(pcSelfCards,pcTmpCards+iTmpCount,0)< 0)
		{
			return -1;
		}
		else
		{
			iTmpCount++;
		}
	}
	else
	{
		return -1;
	}

	int iIdx  = 0;
	for(;iIdx < iCardsNum;iIdx++)
	{
		pcOutCards[iIdx]	= pcTmpCards[iIdx];
	}
	return	 iSelfIdx;
	///�������е���
}

int	mtServiceCardOut::getTripleWithTwo(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum)
{
	int iSelfIdx	= iCardBegPos;
	char pcTmpCards[20];
	int	 iTmpCount	= 0;
	int  iResult	= 0;
	memset(pcTmpCards,MT_CARD_TERMINATE,20);
	if(iCardsNum != 5)
	{
		return -1;
	}
	if((iSelfIdx = getTripleCard(pcSelfCards,pcTmpCards,iCardBegPos))>=0)
	{
		iTmpCount += 3;
		if(getPairCard(pcSelfCards,pcTmpCards+iTmpCount,0)< 0)
		{
			return -1;
		}
		else
		{
			iTmpCount+=2;
		}
	}
	else
	{
		return -1;
	}

	int iIdx  = 0;
	for(;iIdx < iCardsNum;iIdx++)
	{
		pcOutCards[iIdx]	= pcTmpCards[iIdx];
	}
	return	 iSelfIdx;
		///�������е���
}

int	mtServiceCardOut::getAirPlane(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum)
{
	int iSelfIdx	= iCardBegPos;
	char pcTmpCards[20];
	int	 iTmpCount	= 0;
	int  iTripleNum;
	memset(pcTmpCards,MT_CARD_TERMINATE,20);
	if(iCardsNum % 4 == 0)
	{
		iTripleNum = iCardsNum>> 2;
	}
	else if(iCardsNum % 5 == 0)
	{
		iTripleNum = iCardsNum/5;
	}
	else
	{
		return - 1;
	}
	if(getTripleProgression(pcSelfCards,pcTmpCards,iCardBegPos,iTripleNum*3) >= 0)
	{
		int iIdx  = 0;
		int iPos = 0;
		iTmpCount += iTripleNum*3;
		if(iCardsNum % 4 == 0)
		{
			for(iIdx  = 0;iIdx < iTripleNum;iIdx++)
			{
				iPos	= getSingleCard(pcSelfCards,pcTmpCards + iTmpCount,iPos);
				if(iPos < 0)
				{
					break;
				}
				iTmpCount++;
				iPos++;
			}
			if(iIdx < iTripleNum)
			{
				return -1;
			}
		}
		else
		{
			for(iIdx  = 0;iIdx < iTripleNum;iIdx++)
			{
				iPos	= getPairCard(pcSelfCards,pcTmpCards+iTmpCount,iPos);
				if(iPos < 0)
				{
					break;
				}
				iTmpCount+=2;
				iPos += 2;
			}
			if(iIdx < iTripleNum)
			{
				return -1;
			}
		}
		for(iIdx  = 0;iIdx < iCardsNum;iIdx++)
		{
			pcOutCards[iIdx]	= pcTmpCards[iIdx];
		}
		return	 iSelfIdx;
	}
	else
	{
		return -1;
	}

}

void mtServiceCardOut::resortCardsOut(char* pcCardsOut)
{
	int iIdx = 0;
	int jIdx = 0;
	char cTmpCard;
	for(;pcCardsOut[iIdx] != 100;iIdx++)
	{
		for(jIdx = iIdx + 1;pcCardsOut[jIdx] != 100;jIdx++)
		{
			if((pcCardsOut[iIdx] & 0x0f) > (pcCardsOut[jIdx] & 0x0f))
			{
				cTmpCard = pcCardsOut[iIdx];
				pcCardsOut[iIdx] = pcCardsOut[jIdx];
				pcCardsOut[jIdx] = cTmpCard;
			}
		}
	}
}

int mtServiceCardOut::getMainCardFromRecv(int iCardType,char* pcCardsRecv,int iCardsRecvNum)
{
	switch(iCardType)
	{
	case MT_CARD_TYPE_PASS:
		{
			return -1;
		}
	case MT_CARD_TYPE_SINGLE:
	case MT_CARD_TYPE_PAIR:
	case MT_CARD_TYPE_TRPILE:
	case MT_CARD_TYPE_BOMB:
	case MT_CARD_TYPE_SINGLE_PROGRESSION:
	case MT_CARD_TYPE_PAIR_PROGRESSION:
	case MT_CARD_TYPE_TRIPLE_PROGRESSION:
		{
			return pcCardsRecv[0];
		}
	case MT_CARD_TYPE_TRIPLE_ONE:
		{
			return judgeTripleWithOne(pcCardsRecv,iCardsRecvNum);
		}
	case MT_CARD_TYPE_TRIPLE_TWO:
		{
			return judgeTripleWithTwo(pcCardsRecv,iCardsRecvNum);
		}
	case MT_CARD_TYPE_AIRPLANE:
		{
			return judgeAirPlane(pcCardsRecv,iCardsRecvNum);
		}
	case MT_CARD_TYPE_FOUR_TWO:
		{
			return judgeFourWithTwo(pcCardsRecv,iCardsRecvNum);
		}
	}

	return -1;
}

int mtServiceCardOut::playOutCardsNotByType(char pcSelfCard[],char pcOutCard[] ,int iCardType, int iSelfCardsNum)
{
	switch(iCardType)
	{
	case MT_CARD_TYPE_SINGLE:
		{
			if(getTripleWithOne(pcSelfCard,pcOutCard,0,4) >= 0)
			{
				return MT_CARD_TYPE_TRIPLE_ONE;
			}
			else if(getTripleCard(pcSelfCard,pcOutCard,0) >= 0)
			{
				return MT_CARD_TYPE_TRPILE;
			}
			else if(getPairCard(pcSelfCard,pcOutCard,0) >= 0)
			{
				return MT_CARD_TYPE_PAIR;
			}
			else
			{
				pcOutCard[0]= pcSelfCard[iSelfCardsNum -1];
				return MT_CARD_TYPE_SINGLE;
			}
		}
	case MT_CARD_TYPE_PAIR:
		{
			if(getTripleWithOne(pcSelfCard,pcOutCard,0,4) >= 0)
			{
				return MT_CARD_TYPE_TRIPLE_ONE;
			}
			else if(getTripleCard(pcSelfCard,pcOutCard,0) >= 0)
			{
				return MT_CARD_TYPE_TRPILE;
			}
			else
			{
				pcOutCard[0]= pcSelfCard[0];
				return MT_CARD_TYPE_SINGLE;
			}
		}
	case MT_CARD_TYPE_TRPILE:
		{
			if(getTripleWithOne(pcSelfCard,pcOutCard,0,4) >= 0)
			{
				return MT_CARD_TYPE_TRIPLE_ONE;
			}
			else if(getPairCard(pcSelfCard,pcOutCard,0) >= 0)
			{
				return MT_CARD_TYPE_PAIR;
			}
			else
			{
				pcOutCard[0]= pcSelfCard[0];
				return MT_CARD_TYPE_SINGLE;
			}
		}
	case MT_CARD_TYPE_TRIPLE_ONE:
		{
			if(getTripleCard(pcSelfCard,pcOutCard,0) >= 0)
			{
				return MT_CARD_TYPE_TRPILE;
			}
			else if(getPairCard(pcSelfCard,pcOutCard,0) >= 0)
			{
				return MT_CARD_TYPE_PAIR;
			}
			else
			{
				pcOutCard[0]= pcSelfCard[0];
				return MT_CARD_TYPE_SINGLE;
			}
		}
	}

	return -1;
}

bool mtServiceCardOut::bSplitCards(mtQueueDesk::DataNode* pkQueueDeskDataNode,int iQueueUserAiIdx)
{
	//�����û����Լң�������
   if (pkQueueDeskDataNode->plPlayerType[iQueueUserAiIdx] == pkQueueDeskDataNode->plPlayerType[pkQueueDeskDataNode->lQueueUserDeskIdx])
   {
	   return false;
   }

   /// ��ǰ��������������
   int iCardsRecvNum     = getCardsNum(pkQueueDeskDataNode->pcCardsCur);
   int iMainCardNumRecv  = (getMainCardFromRecv(pkQueueDeskDataNode->lCardType, pkQueueDeskDataNode->pcCardsCur, iCardsRecvNum) & 0x0f);
   int iQueueUserNextIdx = getQueueUserNextQueueUser(iQueueUserAiIdx);

   //����Է����Ƶ�����С��10�����¼����Լң�Ҳ������
   if(iMainCardNumRecv < 7 
	   && pkQueueDeskDataNode->plPlayerType[iQueueUserNextIdx] == pkQueueDeskDataNode->plPlayerType[iQueueUserAiIdx])
   {
	   return false;
   }

   return true;
}

void printCards(char *cards)
{
	static char cardtype[] = {'3','4','5','6','7','8','9','10','J','Q','K' ,'A','2','(',')'};

	if(*cards == MT_CARD_TERMINATE)
		mtLog("Pass");
	while(*cards != MT_CARD_TERMINATE)
	{
		char card = cardtype[*cards & 0x0f];
		if(card != '0')
			mtLog("%c ",card);
		else
			mtLog("10 ");
		++cards;
	}
}

int CardsNumber(char *pcCards)
{
	int iNumber  = 0;
	while(*pcCards != MT_CARD_TERMINATE)
	{
		iNumber++;
		pcCards++;
	}
	return iNumber;
}

void mtServiceCardOut::printfOutCards(char *pcCurOutCards,char * pcSelfOutCards,char *pcSelfCards,long UserId,long curOutCardUserId)
{
	static long landlord = 0;
	if(curOutCardUserId != 0)
	{
		if(curOutCardUserId == UserId)
		{
           mtLog("\nOthe one pass,I Continue to Out Card!");
		}
		else
		{
			mtLog("\n[%d] out cards: ",curOutCardUserId);
			printCards(pcCurOutCards);
		}
	}
	else
	{
		mtLog("\n********************** A new start ******************************************");
		mtLog("\n------------------- [%d] is Landlord ---------------------------------------",UserId);
		landlord = UserId;
	}


	mtLog("\n[%d] Self Cards: ",UserId);
	printCards(pcSelfCards);
    
	mtLog("\n[%d] Out Cards: ",UserId);
    printCards(pcSelfOutCards);


	if(CardsNumber(pcSelfOutCards) == CardsNumber(pcSelfCards))
	{
		if(UserId == landlord)
		{
		  mtLog("\n\n-------------The game over,landlord win!-----------------");
		}
		else
		{
		  mtLog("\n\n-------------The game over,farmer win!--------------------");
		}
	}

	mtLog("\n\n");
	
}
