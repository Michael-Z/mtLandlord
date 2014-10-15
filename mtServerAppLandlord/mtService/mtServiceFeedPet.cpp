#include "mtServiceFeedPet.h"
#include "mtServiceMgr.h"
#include "mtQueuePacket.h"
#include "mtPrint.h"

mtServiceFeedPet::~mtServiceFeedPet()
{
}

mtServiceFeedPet::mtServiceFeedPet()
{

}

int mtServiceFeedPet::init( void* pData )
{
	DataInit*	pkDataInit	= (DataInit*)pData;
	m_pkQueuePacket         = pkDataInit->pkQueuePacket;
	m_pkQueueHall           = pkDataInit->pkQueueHall;
	m_pkSQLEnv		= pkDataInit->pkSQLEnv;

	return	1;
}

int mtServiceFeedPet::run( void* pData )
{
	SOCKET	   iSocket	= *(SOCKET*)pData;
	DataRead   kDataRead;
	DataWrite  kDataWrite;
	memset(&kDataWrite, 0 , sizeof(kDataWrite));
	memset(&kDataRead, 0 , sizeof(kDataRead));

	int		   iRet	= 0;
	if (runRead(iSocket, &kDataRead))
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

int mtServiceFeedPet::exit()
{
	return 1;
}

int	mtServiceFeedPet::runRead(SOCKET socket, DataRead* pkDataRead)
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

int	mtServiceFeedPet::runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes)
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

void* mtServiceFeedPet::getUserNodeAddr(long lUserId)
{
	if (MT_SERVER_WORK_USER_ID_MIN <= lUserId && MT_SERVER_WORK_USER_ID_MAX > lUserId)
	{
		return m_pkQueueHall->m_kDataHall.pkUserDataNodeList + lUserId;
	}
	return NULL;
}

void   mtServiceFeedPet::initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite)
{
	mtQueueHall::PigInfo  kPigInfo;
	static int upgradeExp[]  = {10,30,50,70,90,110,130,150,170};

	if(m_pkSQLEnv->getPigInfo(kDataRead.lUserId,&kPigInfo) && kPigInfo.lLevel > 0 && kPigInfo.lMash > 0)
	{
		kPigInfo.lexperience++;
		kPigInfo.lMash--;

		if(kPigInfo.lexperience >= upgradeExp[kPigInfo.lLevel - 1])
		{
			kPigInfo.lexperience = kPigInfo.lexperience - upgradeExp[kPigInfo.lLevel - 1];
			kPigInfo.lLevel++;
		}
		m_pkSQLEnv->updatePigInfo(&kPigInfo);

		if(kPigInfo.lMash == 0)
		{
			tm  kNow;
			__time64_t kNowTime;

			time(&kNowTime);
			_localtime64_s(&kNow, &kNowTime);
			kNow.tm_hour--;

			m_pkSQLEnv->updateUserPropInfo(kDataRead.lUserId,6,kNow);
		}

		kDataWrite.lResult         = 1;
		kDataWrite.llevel          = kPigInfo.lLevel;
		kDataWrite.lexperience     = kPigInfo.lexperience;
		kDataWrite.lmash           = kPigInfo.lMash;
		kDataWrite.lnextexperience = upgradeExp[kPigInfo.lLevel - 1];
	}
	else
	{
		kDataWrite.lResult = 0;
	}
}
