#include "mtServiceLottery.h"
#include "mtServiceMgr.h"
#include "mtQueuePacket.h"
#include "mtPrint.h"
#include <random>


mtServiceLottery::~mtServiceLottery()
{

}

mtServiceLottery::mtServiceLottery()
{

}

int mtServiceLottery::init( void* pData )
{
	DataInit*	pkDataInit	= (DataInit*)pData;
	m_pkQueuePacket         = pkDataInit->pkQueuePacket;
	m_pkQueueHall           = pkDataInit->pkQueueHall;
	m_pkSQLEnv		= pkDataInit->pkSQLEnv;

	return	1;
}

int mtServiceLottery::run( void* pData )
{
	SOCKET	   iSocket	= *(SOCKET*)pData;
	DataRead   kDataRead;
	DataWrite  kDataWrite;
	memset(&kDataWrite, 0 , sizeof(kDataWrite));
	memset(&kDataRead, 0 , sizeof(kDataRead));

	int		   iRet	= 0;
	if(runRead(iSocket, &kDataRead))
	{
		mtPrint(&kDataRead);
		kDataWrite.lStructBytes            = sizeof(kDataWrite);
		kDataWrite.lServiceType            = kDataRead.lServiceType;


		if (MT_SERVER_WORK_USER_ID_MIN <= kDataRead.lUserId && MT_SERVER_WORK_USER_ID_MAX > kDataRead.lUserId)
		{
			// �����û�id������û��ڵ����ڴ�ĵ�ַ
			mtQueueHall::UserDataNode* pkUserDataNode = (mtQueueHall::UserDataNode*)getUserNodeAddr(kDataRead.lUserId);
			if (!pkUserDataNode)
			{
				kDataWrite.lResult = -1;
			}
			else
			{
				if (mtQueueHall::E_HALL_USER_STATUS_OFFLINE == pkUserDataNode->lIsOnLine)
				{
					kDataWrite.lResult = -1;
					MT_DEBUG_ERROR(PTCL,"\nERROR:user(%d)is offline!Please login!",kDataRead.lUserId);
				}
				else
				{
					initDataWrite(kDataRead,kDataWrite);
				}
			}		
		}

		mtPrint(&kDataWrite);
		iRet = runWrite(iSocket, (char*)&kDataWrite, kDataWrite.lStructBytes, 0, 3);
		if (iRet >= 1)
		{
			shutdown(iSocket, 1);
			closesocket(iSocket);
			*(SOCKET*)pData = INVALID_SOCKET;
		}
	}

	return 1;
}

int mtServiceLottery::exit()
{
	return 1;
}

int	mtServiceLottery::runRead(SOCKET socket, DataRead* pkDataRead)
{	
	int	iReadBytesTotalHas	= recv(socket, (char*)pkDataRead, sizeof(DataRead), MSG_PEEK);
	if (iReadBytesTotalHas < sizeof(pkDataRead->lStructBytes))
	{		
		return	0;
	}

	if (iReadBytesTotalHas >= pkDataRead->lStructBytes) 
	{
		iReadBytesTotalHas	= recv(socket, (char*)pkDataRead, pkDataRead->lStructBytes, 0);
	}

	return (iReadBytesTotalHas < pkDataRead->lStructBytes) ? 0 : 1;
}

int	mtServiceLottery::runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes)
{
	int		iRet		= 0;
	int		iWriteTimes;

	for (iWriteTimes = 0;iWriteTimes < iTimes;iWriteTimes ++)
	{
		iRet	+= send(socket, pcBuffer + iRet, iBytes - iRet, flags);
		if (iRet >= iBytes)
		{
			return 1;
		}
	}

	return 0;
}

void* mtServiceLottery::getUserNodeAddr(long lUserId)
{
	if (MT_SERVER_WORK_USER_ID_MIN <= lUserId && MT_SERVER_WORK_USER_ID_MAX > lUserId)
	{
		return m_pkQueueHall->m_kDataHall.pkUserDataNodeList + lUserId;
	}

	return NULL;
}

void  mtServiceLottery::initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite)
{
	mtQueueHall::PrizeInfo   kPrizeInfoList[10];		
	mtQueueHall::PrizeInfo   kPrizeInfo;
	mtQueueHall::LotteryInfo kLotteryInfo;
	mtQueueHall::UserInfo    kUserInfo;

	memset(&kUserInfo,0,sizeof(kUserInfo));
	memset(&kPrizeInfo,0,sizeof(kPrizeInfo));
	memset(&kLotteryInfo,0,sizeof(kLotteryInfo));

	m_pkSQLEnv->getUserInfo(kDataRead.lUserId,&kUserInfo);
	m_pkSQLEnv->getPrizeInfo(&kPrizeInfo);
	m_pkSQLEnv->getLotteryinfo(kDataRead.lUserId,&kLotteryInfo);

	if(kUserInfo.lUserGold < kPrizeInfo.lNeedGold)//��Ҳ���
	{
		kDataWrite.lResult                  = E_RESULT_GOLD_INADEQUATE;
	}
	else if( kLotteryInfo.lUseTimes <= 0)//��������
	{
		kDataWrite.lResult                  = E_RESULT_NUMBER_FULL;
	}
	else//��ʼ�齱
	{
		tm  kNow;
		__time64_t kNowTime;

		time(&kNowTime);
		_localtime64_s(&kNow, &kNowTime);

		kUserInfo.lUserGold -= kPrizeInfo.lNeedGold;

		kLotteryInfo.lUserId       = kUserInfo.lUserId;
		kLotteryInfo.lLuck        += 1;
		kLotteryInfo.lUseTimes    -= 1;
		kLotteryInfo.lSumGold     += kPrizeInfo.lNeedGold;
		kLotteryInfo.lSumTimes    += 1;
		kLotteryInfo.tUseTime      = kNow;

		kDataWrite.lResult = E_RESULT_SUCCESS;

		if(kLotteryInfo.lLuck >= 100)//����ֵ��100��ֱ�ӻ�ý�Ʒ
		{
			kDataWrite.lPrize = kPrizeInfo.lLuckPrize;
			kLotteryInfo.lLuck = 0;
		}
		else//�齱
		{
			//��ȡ�齱�����б�
			m_pkSQLEnv->getPrizeInfoList(kPrizeInfoList);
			kDataWrite.lPrize     =  Lottery(kPrizeInfoList);
		}

		SaveAwardInfo(kDataWrite.lPrize,&kUserInfo,&kLotteryInfo);

		kDataWrite.lGold       = kUserInfo.lUserGold;
		kDataWrite.lLuck       = kLotteryInfo.lLuck;
		kDataWrite.lFeeCard    = kLotteryInfo.lFeeCard;
	}
}

int GetRandomNumber(int range_min,int range_max)
{	
	/*
	static int seed = 0x93871243;
	srand(seed);
	seed = rand();
	srand(seed);

	return (double)rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min;
	*/
	static default_random_engine    e((int)time(0));
	static uniform_int_distribution<int> u(range_min,range_max);

	return  u(e);
}

long mtServiceLottery::Lottery(mtQueueHall::PrizeInfo* pkPrizeInfoList)
{
	int	  rangeValue = 0;
	
	//unsigned seed = (unsigned)time(NULL);
	//srand(seed);
	//int   pProbability = rand()  % 9999 + 1;

	int pProbability = GetRandomNumber(1,10000);

	//MT_DEBUG_PTCL("\n     rand:%d",pProbability);
	int index;

	for(index = 0;index < 10;index++ )
	{
		rangeValue += pkPrizeInfoList[index].lProbability;
		if(pProbability <= rangeValue)
		{
			return pkPrizeInfoList[index].lPrize;
		}
	}
	return -1;
}

void mtServiceLottery::SaveAwardInfo(long lPrize,mtQueueHall::UserInfo* pkUserInfo,mtQueueHall::LotteryInfo * pkLotteryInfo)
{
	if(lPrize < E_PRIZE_FIVE_HUNDRED_GOLD || lPrize > E_PRIZE_THIRTY_FEE_CARD)
	{
		MT_DEBUG_PTCL("\nmtServiceLottery::SaveAwardInfo error,lPrize:%d",lPrize);
		return;
	}

	static long   rewardGoldList[4]  = {500,1000,2000,50000};
	static long   PrepaidCardList[6] = {1,2,3,5,10,30};

	mtQueueHall::AwardInfo kAwardInfo;
	long                   rewardGold = 0;
	char                   systemMessage[80];

	memset(&kAwardInfo,0,sizeof(kAwardInfo));
	memset(systemMessage,0,sizeof(systemMessage));

	kAwardInfo.lUserid  = pkUserInfo->lUserId;
	kAwardInfo.lPrize   = lPrize;


	if(lPrize < E_PRIZE_ONE_FEE_CARD)
	{
		rewardGold = rewardGoldList[lPrize - E_PRIZE_FIVE_HUNDRED_GOLD];
		kAwardInfo.iIsUse   = 1;

		m_pkSQLEnv->saveAwardInfo(&kAwardInfo);
	}
	else
	{
		pkLotteryInfo->lFeeCard += PrepaidCardList[lPrize - E_PRIZE_ONE_FEE_CARD];
		kAwardInfo.iIsUse   = 0;

		GetMessage(systemMessage,pkUserInfo->pcUserNickName,PrepaidCardList[lPrize - E_PRIZE_ONE_FEE_CARD]);
		m_pkSQLEnv->saveSystemMessage(systemMessage);
	}

	 pkUserInfo->lUserGold += rewardGold;
	 m_pkSQLEnv->updateLotteryinfo(pkLotteryInfo);
	 m_pkSQLEnv->updateUserInfo(pkUserInfo);
}

void mtServiceLottery::GetMessage(char * message,char *name ,long PrepaidCard)
{
	char  * MsgTemplate[] ={
		"��ϲ���*�鵽����ȯ%Ԫ���������������Ц�ݡ�",
		"��ϲ���*�鵽����ȯ%Ԫ��һ�ſ������ʱϲ��ü�ҡ�",
		"���*�ŵ�һ�γ齱��ϲ��%Ԫ����ȯ�������Ǻ��˵�ͷ����",
		"���*����Ϊ���Ѳ�������գ�����ȴ�е�%Ԫ����ȯ����ֱ��ѩ����̿����",
		"���*��������19�ζ�û�н���ϵͳ���������������ڵ�20�γ��л���ȯ%Ԫ��",
		"���*����100 �����ڻ���������ֵ��������Ļ�����%Ԫ����ȯ��",
		"���*���Ÿճ��е�%Ԫ����ȯ������һ�ӣ���ֵ��20Ԫ��",
		"���*������������������һ�ӳ�ֵ20Ԫ�����Ǻ���������"
	};

	srand((unsigned)time(NULL));
	int index = rand() % 7;
	char * Msg;
	char  cPrize[3];

	Msg = MsgTemplate[index];
	sprintf(cPrize,"%2d",PrepaidCard);

	strcpy(message,Msg);

	char * temp = Msg;
	int ind  = 0;
	while(*temp != NULL)
	{
		if(*temp == '*')
		{
			strcpy(message + ind,name);
			ind = strlen(message) - 1;
			strcpy(message + strlen(message),temp + 1);		  
		}
		if(*temp == '%')
		{
			strcpy(message + ind,cPrize);
			strcpy(message + strlen(message),temp + 1);
		}
		temp ++;
		ind ++;
	}
}