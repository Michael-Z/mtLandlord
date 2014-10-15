#ifndef		__MT_QUEUE_WORK_H
#define		__MT_QUEUE_WORK_H

#include "mtPlatform.h"
#include "mtDataStructQueue.h"

struct	mtQueueWorkNode
{
	SOCKET		kSocketClient;				//�ͻ����׽���
	DWORD		timeSec;					//ʱ��(����)
	BOOL		bIsAccept;					//���ӻ�������ݵı�־
};

typedef	mtDataStructQueue<mtQueueWorkNode, MT_QUEUE_WORK_SIZE>	mtQueueWorkList;

#endif	/// __MT_QUEUE_WORK_H