#ifndef		__MT_QUEUE_USER_H
#define		__MT_QUEUE_USER_H
#include "mtPlatform.h"
#include "mtDSQueue.h"
#include "mtQueuePacket.h"
#include "mtDSList.h"
// 
// struct mtListNode
// {
// 	void*	pPre;
// 	void*	pSelf;
// 	void*	pNext;
// };

class mtQueueUser 
{
public:
	enum 
	{
		E_LIST_NODE_BEG		= 0,
		E_LIST_NODE_HEART	= E_LIST_NODE_BEG,
		E_LIST_NODE_ONE,
		E_LIST_NODE_END
	};

	enum
	{
		E_SERVICE_ROOM_EXIT_STATUS_BEG         = 0,
		E_SERVICE_ROOM_EXIT_STATUS_INIT        = E_SERVICE_ROOM_EXIT_STATUS_BEG,
		E_SERVICE_ROOM_EXIT_STATUS_RUNNING,
		E_SERVICE_ROOM_EXIT_STATUS_CHANGE,
		E_SERVICE_ROOM_EXIT_STATUS_ROUND_OVER,
		E_SERVICE_ROOM_EXIT_STATUS_EXITING,
		E_SERVICE_ROOM_EXIT_STATUS_EXITED,
		E_SERVICE_ROOM_EXIT_STATUS_RELEASE,

		E_SERVICE_ROOM_EXIT_STATUS_END
	};

	struct DataNode
	{
		long							lStructBytes;
		SOCKET							uiSockets;
		long 							lSpaceId;				        /// ��Id
		long 							lRoomId;				        /// ����Id
		long 							lUserId;				        /// �û�Id
		long                            lUserIconId;                    /// �û�ͷ��id
		long                            lUserGold;                      /// �û�ӵ�н����
		long                            lUserSex;                       /// �Ա� (0--�У�1--Ů)
		long                            lUserLevel;                     /// �û��ȼ�
		long 							lUserScore;				        /// �û�����
		long                            lUserAllChess;                  /// �ܾ���
		long                            lUserWinChess;                  /// ʤ����
		long                            lUserWinRate;                   /// ʤ��
		long                            lUserOffLineCount;              /// ���ߴ���
		long 							lIsWaitStart;			        /// �Ƿ��Ѿ����ȴ���ʼ�� 0��û�еȴ���ʼ 1�Ѿ��ȴ���ʼ��
		long                            lUserDeskIndex;                 /// ��Ա�������ϵ�˳��,��ʼֵ��-1,(0,1,2)
		long                            lUserExit;                      /// ���û���ǰ״̬(0 -��ʼ״̬��1 -�������У�2 -�����˳���3 -�Ѿ��˳�)
		long                            lUserIOCPStatus;                /// 
		long                            plPropsCount[16];               /// �û�������Ŀ
		char                            pcUserAccountNum[12];           /// �û��˺�
		char                            pcUserName[12];                 /// �û�����
		char                            pcUserNickName[12];             /// �û��ǳ�
		ULONGLONG				        ullTimeEnterRun;                /// �ڵ�������ʱ��
 		void*							pkQueueDeskDataNode;		    /// mtQueueDesk::DataNode*
 		mtQueuePacket::DataNode*		pkQueuePacketDataNodeHead;
		mtListNode						pkListNode[E_LIST_NODE_END];
	};

	struct DataInit
	{
		long		lStructBytes;
		int			iSpinCount;
		HANDLE		hIOCP;
	};

public:
	mtQueueUser();
	~mtQueueUser();

	int				init(void* pData);
	int				exit(void);

	bool            bHaveRepeat();

	DataNode*		pop();
	int				push(DataNode* pkDataNode);

	void*			popInitDataNode(SOCKET uiSocket);

	DataNode*		getBaseAddrFromAddr(void* pData);
	int				getNodeNumRemain();
	int				getNodeNumTotal();

	long			getNodeUseCount();


	int				initDataNode(DataNode* pkDataNode, SOCKET uiSocket);
	int				exitDataNode(DataNode* pkQueueUserDataNode, mtQueuePacket* pkQueuePacket);

	int				addDataNodePacket(DataNode* pkDataNodeUser, mtQueuePacket::DataNode* pkDataNodePacket);
	int				removeDataNodePacket(DataNode* pkDataNodeUser, mtQueuePacket::DataNode* pkDataNodePacket);

	int				DataRead(DataNode* pkDataNodeUser, mtQueuePacket::DataNode* pkDataNodePacket);
	int				DataWrite(DataNode* pkDataNodeUser, mtQueuePacket::DataNode* pkDataNodePacket);
	long            getNumberOfReadPacket(DataNode* pkDataNodeUser);
	void printUserPacket(DataNode* pkDataNodeUser);
private:
	DataNode										m_pkNodeList[MT_NODE_QUEUE_USER];
	mtDSQueue<DataNode*, MT_NODE_QUEUE_USER>		m_pkIdQueue;
	int												m_iNodeTotal;
	int												m_iSpinCount;
	HANDLE											m_hIOCP;
	CRITICAL_SECTION								m_kCirticalSection;

};

 /************************************************************************
  * @function:	getIdFromAddr                                                     
  * @describe:	���ݽ����� iNodeIdx
  * @param-in:	
		-pData		�����ַ
  * @return:		����±�
  * @author:		fuke
  ************************************************************************
  */
inline mtQueueUser::DataNode* mtQueueUser::getBaseAddrFromAddr( void* pData )
{
	char*			pcAddrBase	= (char*)m_pkNodeList;
	char*			pcAddrSrc	= (char*)pData;
	return			(DataNode*)(pcAddrBase + 
		(((pcAddrSrc - pcAddrBase) / sizeof(DataNode)) * sizeof(DataNode)));
}

inline int mtQueueUser::getNodeNumRemain()
{
	return	m_pkIdQueue.getNodeNumUsed();
}

inline int mtQueueUser::getNodeNumTotal()
{
	return  m_iNodeTotal;
}

inline long	mtQueueUser::getNodeUseCount()
{
	return m_pkIdQueue.getNodeUseCount();
}

inline bool mtQueueUser::bHaveRepeat()
{
	return m_pkIdQueue.bHaveRepeat();
}

 /************************************************************************
  * @function:	pop                                                     
  * @describe:	�Ӷ����е���һ��Ԫ��
  * @param-out:	pT
  * @return:	
 		-�ɹ�			��NULL
 		-����Ϊ��		NULL
  * @author:		fuke
  ************************************************************************
  */
inline mtQueueUser::DataNode* mtQueueUser::pop()
{
	DataNode*		pkDataNode;
	//return	(m_pkIdQueue.pop(&pkDataNode) > 0) ? pkDataNode : NULL;	
	if(m_pkIdQueue.pop(&pkDataNode) > 0)
	{
		pkDataNode->pkQueueDeskDataNode = NULL;				//���ӽڵ����
		pkDataNode->pkQueuePacketDataNodeHead = NULL;			//��ͷ�ڵ�����
		return pkDataNode;
	}
	return NULL;
}

 /************************************************************************
  * @function:	push                                                     
  * @describe:	��������ѹ��һ��Ԫ��
  * @param-out:	
  * @return:	
 		-�ɹ�			1
 		-����Ϊ��		0
  * @author:		fuke
  ************************************************************************
  */
inline int mtQueueUser::push( DataNode* pkDataNode )
{
	return	(m_pkIdQueue.push(&pkDataNode) > 0) ? 1 : 0;
}



#endif	///	__MT_QUEUE_NODE_USER_H