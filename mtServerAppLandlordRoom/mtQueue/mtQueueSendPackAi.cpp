#include "mtQueueSendPackAi.h"


mtQueueSendPackAi::mtQueueSendPackAi()
{
	m_iNodeTotal	= MT_NODE_QUEUE_THREE;
	m_iSpinCount	= 0;
}

mtQueueSendPackAi::~mtQueueSendPackAi()
{

}
 
int mtQueueSendPackAi::init(void* pData)
{
	DataInit*	pkDataInit = (DataInit*)pData;

	m_iSpinCount			= pkDataInit->iSpinCount;

	memset (m_pkNodeList, 0, sizeof(m_pkNodeList));

	int		iRet			= m_pkIdQueue.init(m_iSpinCount);

	int			iNodeIdx;
	DataNode*	pkDataNode	= m_pkNodeList;
	for (iNodeIdx = 0; iNodeIdx < m_iNodeTotal; iNodeIdx++)
	{
		if (!m_pkIdQueue.push(&pkDataNode))
		{
			return	0;
		}
		pkDataNode++;
	}

	return	iRet;
}

int mtQueueSendPackAi::exit( void )
{
	m_iNodeTotal	= 0;
	m_iSpinCount	= 0;
	return m_pkIdQueue.exit();
}


 /************************************************************************
  * @function:	popInitDataNode                                                     
  * @describe:	�Ӷ����е���һ���Ѿ���ʼ���õ�Ԫ�ص�ָ��
  * @param-in:	
		-uiSocket		����socket
  * @return:	
 		-�ɹ�			��NULL
 		-ʧ��			NULL
  * @author:		fuke
  ************************************************************************
  */
void* mtQueueSendPackAi::popInitDataNode()
{
	DataNode*	pkDataNode	= pop();

	if (NULL != pkDataNode)
	{
		memset (pkDataNode, 0, sizeof(DataNode));
		return	pkDataNode;
	}

	return	NULL;
}

