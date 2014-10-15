#include "mtServiceRoomDoubleScore.h"
#include "mtQueueDesk.h"

mtServiceRoomDoubleScore::mtServiceRoomDoubleScore():
	m_pkQueueMgr(NULL)
{

}

mtServiceRoomDoubleScore::~mtServiceRoomDoubleScore()
{

}

int mtServiceRoomDoubleScore::init( void* pData )
{
	DataInit*	pkDataInit	= (DataInit*)pData;

	m_pkQueueMgr            = pkDataInit->pkQueueMgr;
	return	1;
}

int mtServiceRoomDoubleScore::run( DataRun* pkDataRun )
{
	DataRead* pkDataRead                            = (DataRead*)pkDataRun->pkQueuePacketDataNode->pcData;
	mtQueueUser::DataNode* pkQueueUserDataNode      = pkDataRun->pkQueueUserDataNode;

	if (NULL == pkQueueUserDataNode)
	{
		return 1;
	}

	mtQueueDesk::DataNode* pkQueueDeskDataNode      = (mtQueueDesk::DataNode*)pkQueueUserDataNode->pkQueueDeskDataNode;
	if (NULL == pkQueueDeskDataNode)
	{
		return 1;
	}

	/// ���¸��˼ӱ�����
	int		iUserIdx;
	if (-1 != pkDataRead->lDoubleFlag) /// ��ʵ�˽����˼ӱ�����
	{
		pkQueueDeskDataNode->plDoubleScore[pkQueueUserDataNode->lUserDeskIndex]    = pkDataRead->lDoubleFlag;

		//  ��ǰ�û�����ʵ��
		DataWrite  kDataWrite;
		memset(&kDataWrite, 0, sizeof(kDataWrite));
		kDataWrite.lStructBytes      = sizeof(kDataWrite);
		kDataWrite.lServiceType      = pkDataRead->lServiceType;
		kDataWrite.lUserId           = pkQueueUserDataNode->lUserId;
		kDataWrite.lDoubleFlag       = pkDataRead->lDoubleFlag;
		mtQueuePacket::DataNode* pkQueuePacketDataNode;

		for (iUserIdx = 0; iUserIdx < MT_NODE_PLAYER; iUserIdx++)
		{
			//  ��ǰ��ʵ�û�
			if (NULL != pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx]
			 && mtQueueDesk::E_PLAYER_MODE_USER == pkQueueDeskDataNode->plPlayerMode[iUserIdx])
			{
				pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("doubleScore60");
				if (NULL != pkQueuePacketDataNode)
				{
					pkQueuePacketDataNode->kWSABuf.len    = kDataWrite.lStructBytes;
					memcpy (pkQueuePacketDataNode->pcData, &kDataWrite, kDataWrite.lStructBytes);
					m_pkQueueMgr->getQueueUser()->DataWrite(pkQueueDeskDataNode->pkQueueUserDataNode[iUserIdx], pkQueuePacketDataNode);
				}
			}
		}
	}

	//  ��ǰ����ʵ�û�
	if (mtQueueDesk::E_PLAYER_MODE_USER == pkQueueDeskDataNode->plPlayerMode[pkQueueUserDataNode->lUserDeskIndex])
	{
		/// ����¼��û��������ϵ�λ��
		int iQueueUserNextIdx = getQueueUserNextQueueUser(pkQueueUserDataNode->lUserDeskIndex);

		//  �¼���ai�û�
		if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iQueueUserNextIdx])
		{
			playDoubleScoreAi(pkQueueDeskDataNode, pkDataRead->lServiceType, iQueueUserNextIdx);

			/// ���ai�û����¼��������ϵ�λ��
			iQueueUserNextIdx = getQueueUserNextQueueUser(iQueueUserNextIdx);

			//  ai�¼һ�����ai�û�
			if (mtQueueDesk::E_PLAYER_MODE_AI == pkQueueDeskDataNode->plPlayerMode[iQueueUserNextIdx])
			{
				playDoubleScoreAi(pkQueueDeskDataNode, pkDataRead->lServiceType, iQueueUserNextIdx);
			}
		}
	}

	return	1;
}

int mtServiceRoomDoubleScore::exit()
{
	return	1;
}

int mtServiceRoomDoubleScore::playDoubleScoreAi(mtQueueDesk::DataNode* pkQueueDeskDataNode, long lServiceType, int iQueueUserIdx)
{
	/// ��ai�Ѿ��й��ӱ���(�����ǲ��ӱ�)
	if (0 <= pkQueueDeskDataNode->plDoubleScore[iQueueUserIdx])
	{
		return 1;
	}

	/// �����Լ�Ϊ�Ѿ��ӱ�״̬
	pkQueueDeskDataNode->plDoubleScore[iQueueUserIdx] = 0;

	/// ��������ϵ���ʵ��Ա�б�
	DataWrite kDataWrite;

	kDataWrite.lStructBytes      = sizeof(kDataWrite);
	kDataWrite.lUserId           = pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserIdx]->lUserId;
	kDataWrite.lServiceType      = lServiceType;
	kDataWrite.lDoubleFlag       = 1;
	
	mtQueuePacket::DataNode* pkQueuePacketDataNode;
	int iQueueUserDataNodeIdx;
	for (iQueueUserDataNodeIdx = 0; iQueueUserDataNodeIdx < MT_NODE_PLAYER; iQueueUserDataNodeIdx++)
	{
		/// ���͡���0�֡� ��Ϣ
		//  ��ǰ��ʵ�û�
		if (mtQueueDesk::E_PLAYER_MODE_USER == pkQueueDeskDataNode->plPlayerMode[iQueueUserDataNodeIdx])
		{
			pkQueuePacketDataNode = (mtQueuePacket::DataNode*)m_pkQueueMgr->getQueuePacket()->popInitDataNode("doubleScore128");
			if (pkQueuePacketDataNode)
			{
				pkQueuePacketDataNode->kWSABuf.len  = kDataWrite.lStructBytes;
				memcpy(pkQueuePacketDataNode->pcData, &kDataWrite, kDataWrite.lStructBytes);
				m_pkQueueMgr->getQueueUser()->DataWrite(pkQueueDeskDataNode->pkQueueUserDataNode[iQueueUserDataNodeIdx], pkQueuePacketDataNode);
			}
		}
	}

	return 1;
}

int mtServiceRoomDoubleScore::getQueueUserNextQueueUser(int iQueueUserCurIdx)
{
	if (0 == iQueueUserCurIdx)
	{
		return iQueueUserCurIdx + 2;
	}

	return iQueueUserCurIdx - 1;
}

