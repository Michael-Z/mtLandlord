#ifndef		__MT_QUEUE_PACKET_H
#define		__MT_QUEUE_PACKET_H
#include "mtPlatform.h"
#include "mtDSQueue.h"

class mtQueuePacket
{
public:
	enum EDataType
	{
		E_DATA_TYPE_BEG	= 0,
		E_DATA_TYPE_NOT_USED = E_DATA_TYPE_BEG,
		E_DATA_TYPE_READ,
		E_DATA_TYPE_WRITE,
		E_DATA_TYPE_END
	};

	struct DataNode
	{
		OVERLAPPED			kDataOverLapped;
		void*               pvService;
		long                lServiceType;			/// ��������
		void*               pvData;
		SOCKET	            iSocket;
		HANDLE              hFile;
		EDataType			eDataType;
		char				pcData[MT_BYTES_DATA_PACKET];
	};

	struct DataInit
	{
		long		lStructBytes;
		int			iSpinCount;
	};

public:
	mtQueuePacket();
	~mtQueuePacket();

	int				init(void* pData);
	int				exit(void);

	DataNode*		pop();
	int      		pop(DataNode* pkDataNode, int iTListNum);
	int				push(DataNode* pkDataNode);
	int      		push(DataNode** pkDataNode, int iTListNum);

	void*			popInitDataNode();

	DataNode*		getBaseAddrFromAddr(void* pData);
	int				getNodeNumRemain();
	int				getNodeNumTotal();

	DataNode								        m_pkNodeList[MT_NODE_QUEUE_PACKET_MAX];
	mtDSQueue<DataNode*, MT_NODE_QUEUE_PACKET_MAX>	m_pkIdQueue;
	int										        m_iNodeTotal;
	int										        m_iSpinCount;
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
inline mtQueuePacket::DataNode* mtQueuePacket::getBaseAddrFromAddr( void* pData )
{
	char*			pcAddrBase	= (char*)m_pkNodeList;
	char*			pcAddrSrc	= (char*)pData;
	return			(DataNode*)(pcAddrBase + 
		(((pcAddrSrc - pcAddrBase) / sizeof(DataNode)) * sizeof(DataNode)));
}

inline int mtQueuePacket::getNodeNumRemain()
{
	return	m_pkIdQueue.getNodeNumUsed();
}

inline int mtQueuePacket::getNodeNumTotal()
{
	return  m_iNodeTotal;
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
inline mtQueuePacket::DataNode* mtQueuePacket::pop()
{
	DataNode*		pkDataNode;
	return	(m_pkIdQueue.pop(&pkDataNode) > 0) ? pkDataNode : NULL;	
}

inline int mtQueuePacket::pop(DataNode*pkDataNode, int iTListNum)
{
	return	m_pkIdQueue.pop(&pkDataNode, iTListNum);
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
inline int mtQueuePacket::push( DataNode* pkDataNode )
{
	return	(m_pkIdQueue.push(&pkDataNode) > 0) ? 1 : 0;
}

inline int mtQueuePacket::push(DataNode** pkDataNode, int iTListNum)
{
	return	(m_pkIdQueue.push(pkDataNode, iTListNum) > 0) ? 1 : 0;
}

#endif	///	__MT_QUEUE_PACKET_H