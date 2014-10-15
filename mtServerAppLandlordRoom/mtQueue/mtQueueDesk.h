#ifndef		__MT_QUEUE_DESK_H
#define		__MT_QUEUE_DESK_H
#include "mtPlatform.h"
#include "mtDSQueue.h"
#include "mtQueueUser.h"

class mtQueueDesk 
{
public:
	enum EPlayerType
	{
		E_PLAYER_TYPE_BEG		= 0,
		E_PLAYER_TYPE_LANDLORD	= E_PLAYER_TYPE_BEG,
		E_PLAYER_TYPE_FARMER,
		E_PLAYER_TYPE_END
	};

	enum EPlayerMode
	{
		E_PLAYER_MODE_BEG		= 0,
		E_PLAYER_MODE_USER	= E_PLAYER_MODE_BEG,
		E_PLAYER_MODE_AI,
		E_PLAYER_MODE_END
	};

	struct DataNode
	{
		long		                lStructBytes;
		mtQueueUser::DataNode*		pkQueueUserDataNode[MT_NODE_PLAYER];  /// �����ϵ���Ա��Ϣ
		char						pcCardsBase[MT_NODE_PLAYER];          /// ÿһ�ֵĵ�����Ϣ
		char						pcCardsTotal[MT_NODE_PLAYER][24];     /// ÿ���˵ĵ�ǰ������Ϣ
		char						pcCardsCur[24];                       /// �����ϵ�ǰ���������Ϣ
		char						pcCardsOutAi[24];                     /// aiҪ���������Ϣ(ֻ��ai���õ�)
		char						pcCardsOutTotal[MT_NODE_PLAYER][24];  /// ÿ���˵ĵ�ǰ�Ѿ�������Ϣ
		long						plPlayerMode[MT_NODE_PLAYER];	      /// ��ʶÿ�����ǲ���ai(��ʼֵ��0, 0 -����ʵ���ˣ�1 -��ai)
		long                        plPlayerLots[MT_NODE_PLAYER];         /// ÿ����ҵĳ��Ƶ�����
		long						plPlayerType[MT_NODE_PLAYER];	      /// ÿ���˵�����(��ʼֵ��1�� 0 - landlord, 1 - farmer)
		long 						plIsWaitStart[MT_NODE_PLAYER];        /// �Ƿ��Ѿ����ȴ���ʼ�� 0��û�еȴ���ʼ 1�Ѿ��ȴ���ʼ��
		long						plScore[MT_NODE_PLAYER];	          /// ��ҽеķ���(��ʼֵ��-1, 0 - ���з�, 0< - �еķ���  )
		long						plDoubleScore[MT_NODE_PLAYER];	      /// ��Ҽӱ�����(��ʼֵ��-1, 0 - ���ӱ�, 1 - �ӱ�  )
		long                        plShowCard[MT_NODE_PLAYER];           /// �Ƿ�����(��ʼֵ��-1, 1 -���ƣ�0 -������)
		long						plRoundOver[MT_NODE_PLAYER];	      /// ����Ƿ��ϴ�һ�ֽ������(��ʼֵ��0, 0 - ��û�ϴ�, 1 - �Ѿ��ϴ���  )
		long						plRoundOverGold[MT_NODE_PLAYER];	  /// ����ϴ�һ�ֽ������(��ʼֵ��0, 0 < ��ʾ�ϴ�����)
		long					    plBomb[MT_NODE_PLAYER];               /// ��ҳ���ը����(��ʼֵ��0, 0 < -)
		long                        lCardDeal;                            /// �����Ѿ�������(��ʼֵ��0, 1 -�Ѿ�������)
		long					    lRocket;                              /// �������Ƿ��Ѿ������˻����(��ʼֵ��0, 0 -��û���������1 -�Ѿ������˻��)
		long                        lQueue;                               /// �������ڵĶ���(��ʼֵ0, 1 -���˶��У�2 -���˶��У�3 -���˶���)
		long						lCardType;                            /// �����ϵ�ǰ�����������
		long						lQueueUserDefaultLandlord;            /// �����ϵ�ǰĬ�Ͽ�ʼ�зֵ��û�Id
		long                        lQueueUserDeskIdx;                    /// �����ϵ�ǰ����Ƶ��û��������ϵ�λ��(���Ʋ���)
		long                        lUserDeskIdxCardOutNext;              /// ���ӵ�ǰ��Ҫ���Ƶ��û��������ϵ�λ��
		long                        lDeskExit;                            /// �����ӵ�ǰ״̬(0 -��ʼ״̬��1 -�������У�2 -�����˳���3 -�Ѿ��˳�)
		ULONGLONG				    ullTimeEnterRun;                      /// �ڵ�������ʱ��
		CRITICAL_SECTION	        kCriticalSection;
		int					        iRetCirticalSection;
	};

	struct DataInit
	{
		long		lStructBytes;
		int			iSpinCount;
	};

public:
	mtQueueDesk();
	~mtQueueDesk();

	int				init(void* pData);
	int				exit(void);

	DataNode*		pop();
	int				push(DataNode* pkDataNode);

	DataNode*		getBaseAddrFromAddr(void* pData);
	int				getNodeNumRemain();
	int				getNodeNumTotal();

	long			getNodeUseCount();

	DataNode								m_pkNodeList[MT_NODE_QUEUE_DESK];
	mtDSQueue<DataNode*, MT_NODE_QUEUE_DESK>		m_pkIdQueue;
	int										m_iNodeTotal;
	int										m_iSpinCount;
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

inline mtQueueDesk::DataNode* mtQueueDesk::getBaseAddrFromAddr( void* pData )
{
	char*			pcAddrBase	= (char*)m_pkNodeList;
	char*			pcAddrSrc	= (char*)pData;
	return			(DataNode*)(pcAddrBase + 
		(((pcAddrSrc - pcAddrBase) / sizeof(DataNode)) * sizeof(DataNode)));
}

inline int mtQueueDesk::getNodeNumRemain()
{
	return	m_pkIdQueue.getNodeNumUsed();
}

inline int mtQueueDesk::getNodeNumTotal()
{
	return  m_iNodeTotal;
}

inline long	mtQueueDesk::getNodeUseCount()
{
	return m_pkIdQueue.getNodeUseCount();
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

inline mtQueueDesk::DataNode* mtQueueDesk::pop()
{
	DataNode*		pkDataNode;

	return	(m_pkIdQueue.pop(&pkDataNode) > 0) ? pkDataNode : NULL;	
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
inline int mtQueueDesk::push( DataNode* pkDataNode )
{
	return	(m_pkIdQueue.push(&pkDataNode) > 0) ? 1 : 0;
}

#endif	///	__MT_QUEUE_NODE_USER_H