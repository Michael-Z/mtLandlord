#include "mtServiceRoomRequestCardsLeft.h"
#include "mtServiceRoomWaitStartRecv.h"
#include "mtServiceMgr.h"
#include "mtPrint.h"

mtServiceRoomRequestCardsLeft::mtServiceRoomRequestCardsLeft()
{

}

mtServiceRoomRequestCardsLeft::~mtServiceRoomRequestCardsLeft()
{

}

int mtServiceRoomRequestCardsLeft::init( void* pData )
{
	DataInit*	pkDataInit	= (DataInit*)pData;

	m_pkQueueMgr            = pkDataInit->pkQueueMgr;

	return	1;
}

int mtServiceRoomRequestCardsLeft::run( DataRun* pkDataRun )
{
	mtQueueUser::DataNode*      pkQueueUserDataNode	  = pkDataRun->pkQueueUserDataNode;
	mtQueuePacket::DataNode*	pkQueuePacketDataNode = pkDataRun->pkQueuePacketDataNode;
	DataRead*                   pkDataRead            = (DataRead*)pkQueuePacketDataNode->pcData;
	mtQueueDesk::DataNode* pkQueueDeskDataNode        = (mtQueueDesk::DataNode*)pkQueueUserDataNode->pkQueueDeskDataNode;
	mtPrint(pkDataRead);
	if (NULL == pkQueueDeskDataNode)
	{
		return 1;
	}

	EnterCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
	/// �������¼�ʣ��������Ӧ�������
	DataWrite  kDataWrite;
	memset(&kDataWrite, 0, sizeof(kDataWrite));
	kDataWrite.lStructBytes      = sizeof(kDataWrite);
	kDataWrite.lServiceType      = pkDataRead->lServiceType;

	if (mtQueueDesk::E_PLAYER_MODE_USER == pkQueueDeskDataNode->plPlayerMode[pkQueueUserDataNode->lUserDeskIndex])
	{
		pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("requestcardleft46");
		if (pkQueuePacketDataNode)
		{
			/// ����ϼ��������ϵ�λ��
			int iQueueUserPreIdx = getQueueUserPreQueueUser(pkQueueUserDataNode->lUserDeskIndex);
			/// ����¼��������ϵ�λ��
			int iQueueUserNextIdx = getQueueUserNextQueueUser(pkQueueUserDataNode->lUserDeskIndex);

			memcpy(kDataWrite.pcCardsLast, pkQueueDeskDataNode->pcCardsTotal[iQueueUserPreIdx], sizeof(kDataWrite.pcCardsLast));
			memcpy(kDataWrite.pcCardsNext, pkQueueDeskDataNode->pcCardsTotal[iQueueUserNextIdx], sizeof(kDataWrite.pcCardsNext));
			pkQueuePacketDataNode->kWSABuf.len    = kDataWrite.lStructBytes;
			memcpy (pkQueuePacketDataNode->pcData, &kDataWrite, kDataWrite.lStructBytes);
			mtPrint(&kDataWrite);
			m_pkQueueMgr->getQueueUser()->DataWrite(pkQueueUserDataNode, pkQueuePacketDataNode);
		}
	}

	LeaveCriticalSection(&pkQueueDeskDataNode->kCriticalSection);
	return	1;
}

int mtServiceRoomRequestCardsLeft::getQueueUserNextQueueUser(int iQueueUserCurIdx)
{
	if (0 == iQueueUserCurIdx)
	{
		return iQueueUserCurIdx + 2;
	}

	return iQueueUserCurIdx - 1;
}


int mtServiceRoomRequestCardsLeft::getQueueUserPreQueueUser(int iQueueUserCurIdx)
{
	if (2 == iQueueUserCurIdx)
	{
		return iQueueUserCurIdx - 2;
	}

	return iQueueUserCurIdx + 1;
}

int mtServiceRoomRequestCardsLeft::exit()
{

	return	0;
}

