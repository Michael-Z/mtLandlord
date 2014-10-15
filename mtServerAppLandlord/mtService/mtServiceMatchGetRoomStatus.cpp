#include "mtServiceMatchGetRoomStatus.h"
#include "mtServiceMgr.h"
#include "mtQueuePacket.h"
#include "mtPrint.h"

mtServiceMatchGetRoomStatus::~mtServiceMatchGetRoomStatus()
{
}

mtServiceMatchGetRoomStatus::mtServiceMatchGetRoomStatus()
{

}

int mtServiceMatchGetRoomStatus::init( void* pData )
{
	DataInit*	pkDataInit	= (DataInit*)pData;
	m_pkQueuePacket         = pkDataInit->pkQueuePacket;
	m_pkQueueHall           = pkDataInit->pkQueueHall;
	m_pkSQLEnv		= pkDataInit->pkSQLEnv;

	return	1;
}


int mtServiceMatchGetRoomStatus::run( void* pData )
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
				//kDataWrite.lResult = -1;
			}
			else
			{
				if (mtQueueHall::E_HALL_USER_STATUS_OFFLINE == pkUserDataNode->lIsOnLine)
				{
					//kDataWrite.lResult = -1;
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

int mtServiceMatchGetRoomStatus::exit()
{
	return 1;
}

int	mtServiceMatchGetRoomStatus::runRead(SOCKET socket, DataRead* pkDataRead)
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

int	mtServiceMatchGetRoomStatus::runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes)
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

void* mtServiceMatchGetRoomStatus::getUserNodeAddr(long lUserId)
{
	if (MT_SERVER_WORK_USER_ID_MIN <= lUserId && MT_SERVER_WORK_USER_ID_MAX > lUserId)
	{
		return m_pkQueueHall->m_kDataHall.pkUserDataNodeList + lUserId;
	}

	return NULL;
}

void   mtServiceMatchGetRoomStatus::initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite)
{
	mtQueueHall::PKRoomInfo  kPKRoomInfo;
	time_t tNow;
	tm     tmNowTime;

	memset(&kPKRoomInfo,0,sizeof(kPKRoomInfo));

	time(&tNow);
	_localtime64_s(&tmNowTime, &tNow);

	m_pkSQLEnv->getPKRoom(kDataRead.lRoomId + 1,&kPKRoomInfo);

	kPKRoomInfo.lStartTime.tm_hour  =  0; kPKRoomInfo.lStartTime.tm_min = 0;kPKRoomInfo.lStartTime.tm_sec = 0;
	kPKRoomInfo.lEndTime.tm_mday += 1;kPKRoomInfo.lEndTime.tm_hour  =  0; kPKRoomInfo.lEndTime.tm_min = 0;kPKRoomInfo.lEndTime.tm_sec = 0;

	time_t tStart   = mktime(&kPKRoomInfo.lStartTime);
	time_t tEnd     = mktime(&kPKRoomInfo.lEndTime);
	long   ldayTime = kPKRoomInfo.lActiveTime;

	if(tNow > tStart && tNow < tEnd)
	{
		if((1 << tmNowTime.tm_hour) & ldayTime)
		{
			kDataWrite.lStatus  = 1;
		}
	}
}
