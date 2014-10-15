#ifndef		__MT_QUEUE_MGR_H
#define		__MT_QUEUE_MGR_H
#include "mtQueueUser.h"
#include "mtQueueDesk.h"
#include "mtQueueList.h"
#include "mtQueueHall.h"
#include "mtQueueSendPackAi.h"
#include "mtListManagerAi.h"

class mtQueueMgr 
{
public:
	struct DataInit
	{
		long	lStructBytes;
		int		iSpinCount;
		HANDLE	hIOCP;
		long	lSpaceTotal;
		long	plRoomTotal[MT_NODE_QUEUE_SPACE];
	};
public:
	struct UserNodeReplaceAi
	{
		long	lStructBytes;
		int     iReplaceAi;                    /// �Ƿ�Ϊ����û���ai(0 -���������1 -�����)
		int     iStatusExit;                   /// �˳���ʽ(��ʼֵ0,1 -�û��˳���0 -����ԭ��)
		int     iStatusRun;                    /// �ڷ���״̬(��ʼֵ0,1 -���ڷ����У�2 -�Ѿ��˳�����)
		mtQueueUser::DataNode*   pkUserNode;
	};

	mtQueueMgr();
	~mtQueueMgr();

	int		init(void* pData);
	int		exit();

	mtQueuePacket*	    getQueuePacket();
	mtQueueUser*	    getQueueUser();
	mtQueueDesk*	    getQueueDesk();
	mtQueueHall*	    getQueueHall();
	UserNodeReplaceAi*	getUserNodeReplaceAi(long lUserId);

	HANDLE			   m_hIOCP;

	mtQueuePacket*	   m_pkQueuePacket;
	mtQueueUser*	   m_pkQueueUser;
	mtQueueDesk*	   m_pkQueueDesk;
	mtQueueSendPackAi* m_pkQueueSendPackAi;
	mtListManagerAi*   m_pkListManagerAi;
	mtQueueHall*	   m_pkQueueHall;
	UserNodeReplaceAi  m_pkUserNodeReplaceAi[MT_QUEUE_USER_ID_AI_START];
};

inline mtQueuePacket* mtQueueMgr::getQueuePacket()
{
	return	m_pkQueuePacket;
}

inline mtQueueUser*	mtQueueMgr::getQueueUser()
{
	return	m_pkQueueUser;
}
inline mtQueueDesk*	mtQueueMgr::getQueueDesk()
{
	return	m_pkQueueDesk;
}

inline mtQueueHall*	mtQueueMgr::getQueueHall()
{
	return	m_pkQueueHall;
}

inline mtQueueMgr::UserNodeReplaceAi* mtQueueMgr::getUserNodeReplaceAi(long lUserId)
{
	if (MT_QUEUE_USER_ID_AI_START < lUserId || 1024 > lUserId)
	{
		return NULL;
	}

	return &m_pkUserNodeReplaceAi[lUserId];
}

#endif /// __MT_QUEUE_MGR_H