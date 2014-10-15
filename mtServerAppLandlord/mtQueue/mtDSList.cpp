#include "mtDSList.h"

mtDSList::mtDSList() :
	m_pkListNodeHead(NULL),
	m_pkListNodeTail(NULL),
	m_iNodeNumUsed(0)
{

}

mtDSList::~mtDSList()
{
	m_pkListNodeHead		= NULL;
	m_pkListNodeTail		= NULL;
	m_iNodeNumUsed		= 0;

}

int mtDSList::init( int iSpinCount )
{
	m_pkListNodeHead		= NULL;
	m_pkListNodeTail		= NULL;
	m_iNodeNumUsed		= 0;

	return	1;
}

int mtDSList::exit( void )
{
	m_pkListNodeHead		= NULL;
	m_pkListNodeTail		= NULL;
	m_iNodeNumUsed		= 0;

	return	1;
}

int mtDSList::exception()
{
	switch (m_iExceptionId)
	{
		MT_EXCETPION_STRING(E_EXCEPTION_INIT_CRITICAL_SECTION_FAILED)
	default:
		MT_DEBUG(7, "���쳣û�б�����!\n");
		break;
	}

	return	1;
}

/************************************************************************
 * @function:	push                                                     
 * @describe:	�������ѹ��һ��Ԫ��
 * @param-in:	
		-pListNode		��ѹ��Ľ��
		-pData			��ѹ��Ľ�㸽������Ϣ
 * @return:	
		-�ɹ�				> 0
		-pkListNodeListΪ��			-1
		-���н���Ѿ������	-2
 * @author:		fuke
 ************************************************************************
 */
int mtDSList::push( mtListNode* pListNode, void* pData )
{
	if (NULL == pListNode)
	{
		return	-1;
	}

	pListNode->pNext				= NULL;

	m_iNodeNumUsed++;

	if (NULL == m_pkListNodeTail)
	{
		pListNode->pPre				= NULL;
		m_pkListNodeHead			= pListNode;
		m_pkListNodeTail			= pListNode;
	}
	else
	{
		pListNode->pPre				= m_pkListNodeTail;
		m_pkListNodeTail->pNext		= pListNode;
		m_pkListNodeTail			= pListNode;
	}

	return	1;
}

mtListNode* mtDSList::pop()
{

	mtListNode*	pkListNode			= NULL;

	if (m_iNodeNumUsed > 0)
	{
		m_iNodeNumUsed --;
		pkListNode					= m_pkListNodeHead;
		m_pkListNodeHead			= m_pkListNodeHead->pNext;

		if (NULL == m_pkListNodeHead)
		{
			m_pkListNodeTail		= NULL;
		}
		else 
		{
			m_pkListNodeHead->pPre	= NULL;
		}
	}


	return	pkListNode;

}

/************************************************************************
 * @function:	remove                                                     
 * @describe:	�Ӷ������Ƴ�һ��Ԫ��
 * @param-in:	
	-pkListNode	Ҫ�Ƴ���Ԫ��
 * @return:	
		-�ɹ�			> 0
		pkListNodeΪ��		-1
		-���н�㲻��	-2
 * @author:		fuke
 ************************************************************************
 */
int mtDSList::pop( mtListNode** pkListNodeList, int iListNodeNum )
{
	if (NULL == pkListNodeList)
	{
		return	-1;
	}



	if (m_iNodeNumUsed < iListNodeNum)
	{

		return	-2;
	}
	else 
	{
		m_iNodeNumUsed -= iListNodeNum;

		while (iListNodeNum--)
		{
			pkListNodeList[iListNodeNum]	= m_pkListNodeHead;
			m_pkListNodeHead				= m_pkListNodeHead->pNext;
		}

		if (NULL == m_pkListNodeHead)
		{
			m_pkListNodeTail				= NULL;
		}
		else 
		{
			m_pkListNodeHead->pPre			= NULL;
		}
	}

	return	1;
}

/************************************************************************
 * @function:	remove                                                    
 * @describe:	�Ӷ������Ƴ�һ��Ԫ��
 * @param-in:	
	-pTclass	Ҫ�Ƴ���Ԫ��
 * @return:	
		-�ɹ�			> 0
		pkListNode		-1
		-���н�㲻��	-2
 * @author:		fuke
 ************************************************************************
 */
int mtDSList::remove( mtListNode* pkListNode )
{
	if (NULL == pkListNode)
	{
		return	-1;
	}

	if (pkListNode == m_pkListNodeHead)
	{
		m_pkListNodeHead	= m_pkListNodeHead->pNext;
	}
	else 
	{
		pkListNode->pPre->pNext	= pkListNode->pNext;
	}

	if (pkListNode == m_pkListNodeTail)
	{
		m_pkListNodeTail	= m_pkListNodeTail->pPre;
	}
	else 
	{
		pkListNode->pNext->pPre	= pkListNode->pPre;
	}

	return	1;
}


