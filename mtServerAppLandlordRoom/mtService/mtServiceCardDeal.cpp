#include "mtServiceCardDeal.h"
#include "mtServiceCardOut.h"
#include "mtServiceRoomGrabLandlord.h"
#include "mtServiceMgr.h"
#include <random>


mtServiceCardDeal::mtServiceCardDeal():
m_pkQueueUser(NULL),
m_pkQueuePacket(NULL),
m_hEventManagerAi(NULL)
{

}

mtServiceCardDeal::~mtServiceCardDeal()
{

}

int mtServiceCardDeal::init( void* pData )
{
	DataInit*	pkDataInit	= (DataInit*)pData;

	m_pkQueueMgr            = pkDataInit->pkQueueMgr;
	m_pkQueueUser           = pkDataInit->pkQueueUser;
	m_pkQueuePacket         = pkDataInit->pkQueuePacket;
	m_pkServiceMgr          = pkDataInit->pkServiceMgr;
	m_hEventManagerAi       = pkDataInit->hEventManagerAi;

	return	1;
}

void test(char *pcCards,long *PlayerMode)
{
	if(PlayerMode[1] = 0)
	{
		cout<<"sdfsdf"<<endl;
	}
	strcpy(pcCards,"123456789");
}

int mtServiceCardDeal::run( DataRun* pkDataRun )
{
	DataRead*                   pkDataRead            = (DataRead*)pkDataRun->pkQueuePacketDataNode->pcData;
	mtQueueDesk::DataNode* pkQueueDeskDataNode        = (mtQueueDesk::DataNode*)pkDataRun->pkQueueUserDataNode->pkQueueDeskDataNode;

	/// ����ǽ�����Ӧ ��ʱֱ���˳�
	if (NULL == pkQueueDeskDataNode || 0 < pkDataRead->lResult)
	{
		return 1;
	}

	EnterCriticalSection(&pkQueueDeskDataNode->kCriticalSection);

	if (mtQueueUser::E_SERVICE_ROOM_EXIT_STATUS_RUNNING != pkQueueDeskDataNode->lDeskExit)
	{
		LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
		return 1;
	}

	char pcCards[MT_NODE_CARD_COUNT] = {'\0'};

	//if(pkQueueDeskDataNode->plPlayerMode[0] == 0 &&
	//	pkQueueDeskDataNode->plPlayerMode[1] == 0 &&
	//	pkQueueDeskDataNode->plPlayerMode[2] == 0)
	//{
		shuffleCard(pcCards);
	//}
	//else
	//{
	//	shuffleCardEx(pcCards,pkQueueDeskDataNode->plPlayerMode);
	//}
	//

	/// ������������Ϣ
	pkQueueDeskDataNode->plPlayerType[0]           = mtQueueDesk::E_PLAYER_TYPE_FARMER;
	pkQueueDeskDataNode->plPlayerType[1]           = mtQueueDesk::E_PLAYER_TYPE_FARMER;
	pkQueueDeskDataNode->plPlayerType[2]           = mtQueueDesk::E_PLAYER_TYPE_FARMER;
	pkQueueDeskDataNode->plScore[0]                = -1;
	pkQueueDeskDataNode->plScore[1]                = -1;
	pkQueueDeskDataNode->plScore[2]                = -1;
	pkQueueDeskDataNode->plDoubleScore[0]          = -1;
	pkQueueDeskDataNode->plDoubleScore[1]          = -1;
	pkQueueDeskDataNode->plDoubleScore[2]          = -1;
	pkQueueDeskDataNode->plRoundOver[0]            = 0;
	pkQueueDeskDataNode->plRoundOver[1]            = 0;
	pkQueueDeskDataNode->plRoundOver[2]            = 0;
	pkQueueDeskDataNode->plRoundOverGold[0]        = 0;
	pkQueueDeskDataNode->plRoundOverGold[1]        = 0;
	pkQueueDeskDataNode->plRoundOverGold[2]        = 0;
	pkQueueDeskDataNode->plShowCard[0]             = -1;
	pkQueueDeskDataNode->plShowCard[1]             = -1;
	pkQueueDeskDataNode->plShowCard[2]             = -1;
	pkQueueDeskDataNode->plBomb[0]                 = 0;
	pkQueueDeskDataNode->plBomb[1]                 = 0;
	pkQueueDeskDataNode->plBomb[2]                 = 0;
	pkQueueDeskDataNode->lRocket                   = 0;
	pkQueueDeskDataNode->lCardType                 = mtServiceCardOut::MT_CARD_TYPE_END;
	pkQueueDeskDataNode->lQueueUserDeskIdx         = -1;
	pkQueueDeskDataNode->lUserDeskIdxCardOutNext   = -1;
	pkQueueDeskDataNode->lQueueUserDefaultLandlord = -1;

	memset(pkQueueDeskDataNode->pcCardsTotal, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsTotal));
	memset(pkQueueDeskDataNode->pcCardsCur, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsCur));
	memset(pkQueueDeskDataNode->pcCardsOutAi, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutAi));
	memset(pkQueueDeskDataNode->pcCardsBase, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsBase));
	memset(pkQueueDeskDataNode->pcCardsOutTotal, MT_CARD_TERMINATE, sizeof(pkQueueDeskDataNode->pcCardsOutTotal));

	/// �ֱ��� ���ơ���Ϣ ��������
	srand((unsigned)time(NULL));
	DataWrite kDataWrite;
	memset(&kDataWrite, 0, sizeof(kDataWrite));
	kDataWrite.lStructBytes           = sizeof(kDataWrite);

	/// ����ҳ�һ����ʵ�û�����һ�νз�������
	int iLandlordUserIdx =(rand() % 3);
	iLandlordUserIdx = 0;
// 	while (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iLandlordUserIdx])
// 	{
// 		iLandlordUserIdx = (rand() % 3);
// 	}

	kDataWrite.lDefaultLandlordUserId              = pkQueueDeskDataNode->pkQueueUserDataNode[iLandlordUserIdx]->lUserId;
	pkQueueDeskDataNode->lQueueUserDefaultLandlord = kDataWrite.lDefaultLandlordUserId;
	kDataWrite.lServiceType                        = pkDataRead->lServiceType;
	memset(kDataWrite.pcBaseCards, MT_CARD_TERMINATE, sizeof(kDataWrite.pcBaseCards));
	memcpy(kDataWrite.pcBaseCards, pcCards + sizeof(kDataWrite.pcCards) * 3, sizeof(pkQueueDeskDataNode->pcCardsBase));
	memcpy(pkQueueDeskDataNode->pcCardsBase, kDataWrite.pcBaseCards, sizeof(pkQueueDeskDataNode->pcCardsBase));

	int iQueueUserDataNodeIdx;
	mtQueuePacket::DataNode* pkQueuePacketDataNode;
	for (iQueueUserDataNodeIdx = 0; iQueueUserDataNodeIdx < MT_NODE_PLAYER; iQueueUserDataNodeIdx++)
	{
		if (NULL != pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx])
		{
			/// ����ÿ��������Ϣ������
			memcpy(pkQueueDeskDataNode->pcCardsTotal[iQueueUserDataNodeIdx], pcCards + iQueueUserDataNodeIdx * sizeof(kDataWrite.pcCards), sizeof(kDataWrite.pcCards));

			/// ���Ƹ�ÿ����
			//  ��ǰ�û���ai
			if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iQueueUserDataNodeIdx])
			{
				playDealCardAi(pkQueueDeskDataNode, iQueueUserDataNodeIdx);
			}
			else
			{
				pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueuePacket->popInitDataNode("carddeal125");
				if (NULL != pkQueuePacketDataNode)
				{
					pkQueuePacketDataNode->kWSABuf.len  = kDataWrite.lStructBytes;
					memcpy(kDataWrite.pcCards, pcCards + iQueueUserDataNodeIdx * sizeof(kDataWrite.pcCards), sizeof(kDataWrite.pcCards));
					memcpy(pkQueuePacketDataNode->pcData, &kDataWrite, kDataWrite.lStructBytes);
					int iRet = m_pkQueueUser->DataWrite(pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx], pkQueuePacketDataNode);
					if (0 == iRet)
					{
						iRet = 0;
					}
				}
			}
		}
	}

	mtQueueUser::DataNode* pkQueueUserDataNode = pkQueueDeskDataNode->pkQueueUserDataNode[iLandlordUserIdx];
	LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);

	/// ����е�������ai,��ai��ʼ�з�
	if (NULL != pkQueueDeskDataNode 
	 && mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iLandlordUserIdx]
	 && NULL !=pkQueueDeskDataNode->pkQueueUserDataNode[iLandlordUserIdx])
	{
		/// �������ai�еķ�
		int iLandlordScore = (rand() % 3) + 1;
		playAiGrabLandlord(pkQueueUserDataNode, iLandlordScore);
		MT_DEBUG(4, "User(UserId=%d)(score=%d)(deskidx=%d) GrabLandlord  111111......\n", pkQueueUserDataNode->lUserId, iLandlordScore, iLandlordUserIdx);
	}

	return	1;
}

int mtServiceCardDeal::exit()
{

	return	0;
}

int mtServiceCardDeal::shuffleCard(char* pcCards)
{
	/// ������������
	                                    
	int piIndex[MT_NODE_CARD_COUNT]     = { 0 ,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,9 ,10,11,12,13,14,
		                                    15,16,17,18,19,20,21,22,23,24,25,26,27,
		                                    28,29,30,31,32,33,34,35,36,37,38,39,40,
		                                    41,42,43,44,45,46,47,48,49,50,51,52,53
		                                    };

	/// ԭʼ˳������                    ///  3  4  5  6  7  8  9  10 J  Q  K  A  2 
	char pcCardsSrc[MT_NODE_CARD_COUNT] = { 0 ,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,9 ,10,11,12,
		                                    16,17,18,19,20,21,22,23,24,25,26,27,28,
		                                    32,33,34,35,36,37,38,39,40,41,42,43,44,
		                                    48,49,50,51,52,53,54,55,56,57,58,59,60,
		                                    61,62};// С ����

	/// ���������������
	int iIdx;
	int iRandNum;
	srand((unsigned)time(NULL));

	/// ����������̶ֹ�ը��
	int iBombCountFasten = 0;///rand() % 4; /// �������̶�ը��
	int piBomb[3]        = {-1, -1, -1};
	int iBomb            = 0;
	int iBombCount       = iBombCountFasten;
	while (0 < iBombCount)
	{
		iBomb                    = (rand() % 13) + 3;
		if (piBomb[0] != iBomb && piBomb[1] != iBomb && piBomb[2] != iBomb)
		{
			iBombCount--;
			piBomb[iBombCount] = iBomb;
		}
	}

	/// �Գ�ʼ����������(�̶�ը���Ʋ�Ҫ�������)
	iBombCount       = iBombCountFasten;
	iBombCount       = 0;
	while (iBombCount < iBombCountFasten)
	{
		/// ������
		/// ����
		piIndex[piBomb[iBombCount] - 3]                = -1;

		/// ����
		piIndex[piBomb[iBombCount] - 3 + 13]           = -1;

		/// ÷��
		piIndex[piBomb[iBombCount] - 3 + 13 + 13]      = -1;

		/// ��Ƭ
		piIndex[piBomb[iBombCount] - 3 + 13 + 13 + 13] = -1;

		iBombCount++;
	}

	/// ����������λ��
	int iSwapTmp;
	for (iIdx = 0; iIdx < MT_NODE_CARD_COUNT; iIdx++)
	{
		iRandNum = rand() % MT_NODE_CARD_COUNT;
		if (iIdx != iRandNum)
		{
			iSwapTmp          = piIndex[iIdx];
			piIndex[iIdx]     = piIndex[iRandNum];
			piIndex[iRandNum] = iSwapTmp;
		}
	}

	/// ������
	char pcCardsDest[MT_NODE_CARD_COUNT] = {-1};
	for (iIdx = 0; iIdx < MT_NODE_CARD_COUNT; iIdx++)
	{
		if (-1 == piIndex[iIdx])
		{
			pcCardsDest[iIdx] = -1;
		}
		else
		{
			pcCardsDest[iIdx] = pcCardsSrc[piIndex[iIdx]];
		}
	}

	/// ���̶ֹ�ը��Ҫ���õ�λ��(����Щ���,���Զ���һ��)
	int iBombIndex      = 0;
	int iUserOneCard    = 0;  /// ��һ���˵��ƿ�ʼλ��
	int iUserTwoCard    = 17; /// �ڶ����˵��ƿ�ʼλ��
	int iUserThreeCard  = 34; /// �������˵��ƿ�ʼλ��
	iBombCount          = 0;
	memset(pcCards, -1, sizeof(pcCardsDest));
	while (iBombCount < iBombCountFasten)
	{
		iBombIndex                    = (rand() % 3);
		if (0 == iBombIndex)
		{
			/// ����
			pcCards[iUserOneCard++]   = piBomb[iBombCount] - 3;
			/// ����
			pcCards[iUserOneCard++]   = piBomb[iBombCount] - 3 + 16;
			/// ÷��
			pcCards[iUserOneCard++]   = piBomb[iBombCount] - 3 + 16 + 16;
			/// ��Ƭ
			pcCards[iUserOneCard++]   = piBomb[iBombCount] - 3 + 16 + 16 + 16;
		}
		else if (1 == iBombIndex)
		{
			/// ����
			pcCards[iUserTwoCard++]   = piBomb[iBombCount] - 3;
			/// ����
			pcCards[iUserTwoCard++]   = piBomb[iBombCount] - 3 + 16;
			/// ÷��
			pcCards[iUserTwoCard++]   = piBomb[iBombCount] - 3 + 16 + 16;
			/// ��Ƭ
			pcCards[iUserTwoCard++]   = piBomb[iBombCount] - 3 + 16 + 16 + 16;
		}
		else
		{
			/// ����
			pcCards[iUserThreeCard++] = piBomb[iBombCount] - 3;
			/// ����
			pcCards[iUserThreeCard++] = piBomb[iBombCount] - 3 + 16;
			/// ÷��
			pcCards[iUserThreeCard++] = piBomb[iBombCount] - 3 + 16 + 16;
			/// ��Ƭ
			pcCards[iUserThreeCard++] = piBomb[iBombCount] - 3 + 16 + 16 + 16;
		}

		iBombCount++;
	}

	/// �����������
	int iCardsIndex     = 0;
	int iCardsDestIndex = 0;
	while (iCardsIndex < MT_NODE_CARD_COUNT)
	{
		if (-1 != pcCards[iCardsIndex])
		{
			iCardsIndex++;
			continue;
		}

		while (iCardsDestIndex < MT_NODE_CARD_COUNT)
		{
			if (-1 == pcCardsDest[iCardsDestIndex])
			{
				iCardsDestIndex++;
				continue;
			}

			break;
		}

		if (iCardsDestIndex >= MT_NODE_CARD_COUNT)
		{
			return 0;
		}

		pcCards[iCardsIndex] = pcCardsDest[iCardsDestIndex];
		iCardsIndex++;
		iCardsDestIndex++;
	}

	//memcpy(pcCards,pcCardsSrc,13);               memcpy(pcCards + 13,pcCardsSrc + 39 + 8,4);
	//memcpy(pcCards + 17 ,pcCardsSrc + 13,13);     memcpy(pcCards + 17 + 13,pcCardsSrc + 39 + 4,4);
	//memcpy(pcCards + 17 * 2,pcCardsSrc + 26,13);memcpy(pcCards + 17 + 17 + 13,pcCardsSrc + 39,4);

	//memcpy(pcCards + 51,pcCardsSrc + 51,3);

	//swap(pcCards[12], pcCards[49]);
	//swap(pcCards[51], pcCards[48]);

	/// ÿ���˵����ٴλ���
	/// ��һ����
	shuffleCardEveryOne(pcCards);

	/// �ڶ�����
	shuffleCardEveryOne(pcCards + MT_NODE_CARD_COUNT_EVERY_BODY);

	/// ��������
	shuffleCardEveryOne(pcCards + (MT_NODE_CARD_COUNT_EVERY_BODY * 2));

	return 1;
}

int mtServiceCardDeal::shuffleCardEveryOne(char* pcCardsDest)
{
	char  pcCards[MT_NODE_CARD_COUNT_EVERY_BODY] = {100};
	memcpy(pcCards, pcCardsDest, sizeof(pcCards));
	/// ������������
	int piIndex[MT_NODE_CARD_COUNT_EVERY_BODY]     = { 0 ,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,9 ,10,11,12,13,14,15,16};

	srand((unsigned)time(NULL));
	/// ����������λ��
	int iIdx;
	int iSwapTmp;
	int iRandNum;
	for (iIdx = 0; iIdx < MT_NODE_CARD_COUNT_EVERY_BODY; iIdx++)
	{
		iRandNum = rand() % MT_NODE_CARD_COUNT_EVERY_BODY;
		if (iIdx != iRandNum)
		{
			iSwapTmp          = piIndex[iIdx];
			piIndex[iIdx]     = piIndex[iRandNum];
			piIndex[iRandNum] = iSwapTmp;
		}
	}

	/// ���ݻ��ҵ�����λ�û�������Ϣ
	for (iIdx = 0; iIdx < MT_NODE_CARD_COUNT_EVERY_BODY; iIdx++)
	{
		pcCardsDest[iIdx]  = pcCards[piIndex[iIdx]];
	}

	return 1;
}

int mtServiceCardDeal::playDealCardAi(mtQueueDesk::DataNode* pkQueueDeskDataNode, int iQueueUserAiIdx)
{
	return 1;
}

int mtServiceCardDeal::playAiGrabLandlord(mtQueueUser::DataNode* pkQueueUserDataNodeAi, int iLandlordScore)
{
	/// ��Ӧ������
	mtQueuePacket::DataNode* pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueuePacket->popInitDataNode("carddeal381");
	if (NULL != pkQueuePacketDataNode)
	{
		mtServiceRoomGrabLandlord::DataRead* pkDataRead = (mtServiceRoomGrabLandlord::DataRead*)pkQueuePacketDataNode->pcData;

		pkDataRead->lStructBytes                        = sizeof(mtServiceRoomGrabLandlord::DataRead);
		pkDataRead->lServiceType                        = mtServiceMgr::E_SERVICE_ROOM_GRAD_LANDLORD;
		pkDataRead->lScore                              = iLandlordScore;

//		mtService::DataRun kDataRun;
// 		kDataRun.pkQueuePacketDataNode	= pkQueuePacketDataNode;
// 		kDataRun.pkQueueUserDataNode	= pkQueueUserDataNodeAi;
// 		m_pkServiceMgr->run(&kDataRun);
		/// �������ӳٽз�
		mtQueueSendPackAi::DataNode* pkDataNodeAi = (mtQueueSendPackAi::DataNode*)m_pkQueueMgr->m_pkQueueSendPackAi->popInitDataNode();
		if (NULL != pkDataNodeAi)
		{
			int iInsertHead = 0;
			pkDataNodeAi->ullTimeSend           = GetTickCount() + MT_SERVICE_GRAB_LANDLORD_TIME;
			pkDataNodeAi->iType                 = mtQueueSendPackAi::E_SEND_TYPE_SEVICE;
			pkDataNodeAi->pkQueueUserDataNode   = pkQueueUserDataNodeAi;
			pkDataNodeAi->pkQueuePacketDataNode = pkQueuePacketDataNode;
			m_pkQueueMgr->m_pkListManagerAi->push(pkDataNodeAi, &iInsertHead);
			if (iInsertHead)
			{
				SetEvent(m_hEventManagerAi);
			}
		}
	}

	return 1;
}

int GetRandomNumber(int range_min,int range_max)
{	
	static default_random_engine    e((int)time(0));
	uniform_int_distribution<int> u(range_min,range_max);

	return  u(e);
}

void findCard(vector<char> &CardSrc ,int card)
{
	vector<char>::iterator itCard;

	itCard = find(CardSrc.begin(),CardSrc.end(),card);
	CardSrc.erase(itCard);
}

void getCard(vector<char> &CardSrc,char *pcCards,int &index,char card,int number)
{
	for(int i = 0;i < number;i ++)
	{
		card = i << 4 | card;
		findCard(CardSrc,card);
		pcCards[index] = card; 
		index++;
	}
}

int mtServiceCardDeal::shuffleCardEx(char* pcCards,long *PlayerMode)
{
	vector<char> CardsSrc;

	for(int color = 0;color < 4;color++)
		for(int card = 0;card < 13;card++)
		{
			CardsSrc.push_back(color << 4 | card);
		}
		CardsSrc.push_back(61);
		CardsSrc.push_back(62);

		char iSwapTmp;
		for (int iIdx = 0; iIdx < MT_NODE_CARD_COUNT; iIdx++)
		{
			int iRandNum = rand() % MT_NODE_CARD_COUNT;
			if (iIdx != iRandNum)
			{
				iSwapTmp           = CardsSrc[iIdx];
				CardsSrc[iIdx]     = CardsSrc[iRandNum];
				CardsSrc[iRandNum] = iSwapTmp;
			}
		}

		char pcGoodCard[MT_NODE_CARD_COUNT_EVERY_BODY];
		int findcard;
		int cardIndex = 0;

		findcard = GetRandomNumber(0,2);  //(3,4,5) 3
		getCard(CardsSrc,pcGoodCard,cardIndex,findcard,3);

		findcard =  GetRandomNumber(3,4); //(6,7) 2
		getCard(CardsSrc,pcGoodCard,cardIndex,findcard,2);

		findcard =  GetRandomNumber(5,6);  //(8,9) 2
		getCard(CardsSrc,pcGoodCard,cardIndex,findcard,2);

		//findcard =  GetRandomNumber(7,8); //(10,J) 2
		getCard(CardsSrc,pcGoodCard,cardIndex,7,1);

		getCard(CardsSrc,pcGoodCard,cardIndex,8,1);

		findcard =  GetRandomNumber(9,10); //(Q,K) 2
		getCard(CardsSrc,pcGoodCard,cardIndex,findcard,2);

		getCard(CardsSrc,pcGoodCard,cardIndex,11,2);  //(A) 2
		getCard(CardsSrc,pcGoodCard,cardIndex,12,3);  //(2) 3
		getCard(CardsSrc,pcGoodCard,cardIndex,62,1);  //(dawang) 1

		if(PlayerMode[2] == 1)   //M A A  or  M M A
		{
			memcpy(pcCards + MT_NODE_CARD_COUNT_EVERY_BODY * 2,pcGoodCard,MT_NODE_CARD_COUNT_EVERY_BODY);

			for(int i= 0;i < MT_NODE_CARD_COUNT_EVERY_BODY * 2;i++)
			{
				pcCards[i] = CardsSrc[i];
			}
			pcCards[51] = CardsSrc[51 - 15];
			pcCards[52] = CardsSrc[51 - 16];
			pcCards[53] = CardsSrc[51 - 17];
		}
		else  //   M A M
		{
			memcpy(pcCards + MT_NODE_CARD_COUNT_EVERY_BODY,pcGoodCard,MT_NODE_CARD_COUNT_EVERY_BODY);

			for(int i= 0;i < MT_NODE_CARD_COUNT_EVERY_BODY * 2;i++)
			{
				if(i < MT_NODE_CARD_COUNT_EVERY_BODY)
				{
					pcCards[i] = CardsSrc[i];
				}
				else
				{
					pcCards[i + MT_NODE_CARD_COUNT_EVERY_BODY] = CardsSrc[i];
				}

			}
			pcCards[51] = CardsSrc[51 - 15];
			pcCards[52] = CardsSrc[51 - 16];
			pcCards[53] = CardsSrc[51 - 17];

		}

		return 1;
}

int mtServiceCardDeal::shuffleCardEx(char* pcCards)
{
	/// ԭʼ˳������                    ///  3  4  5   6  7  8   9 10  J   Q  K  A  2 
	char pcCardsSrc[MT_NODE_CARD_COUNT] = { 0 ,1 ,2 , 3 ,4 ,5 , 6 ,7 ,8 , 9 ,10,11,12,
											16,17,18, 19,20,21, 22,23,24, 25,26,27,28,
											32,33,34, 35,36,37, 38,39,40, 41,42,43,44,
											48,49,50, 51,52,53, 54,55,56, 57,58,59,60,
											61,62};// С ����
	
	char c1[] = {0, 1, 2,  16,17,18, 6, 7, 8,  9, 10,11, 12,54,55, 56,60};
	char c2[] = {3, 4, 5,  19,20,21, 62,23,24, 25,26,27, 28,51,52, 53,59};
	char c3[] = {32,33,34, 35,36,37, 38,39,40, 41,42,43, 44,48,49, 50,22};
	char cc[] = {57,61,58};

	memcpy(pcCards,c1,17);
	memcpy(pcCards + 17,c2,17);
	memcpy(pcCards + 34,c3,17);
	memcpy(pcCards + 51,cc,3);

	return 1;
}
