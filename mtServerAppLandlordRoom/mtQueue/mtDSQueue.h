#ifndef		__MT_DS_QUEUE_H
#define		__MT_DS_QUEUE_H
#include "mtPlatform.h"
#include "mtException.h"

template<class _Tclass, int _iNodeNumTotal>
class mtDSQueue : public mtException
{
public:
	enum 
	{
		E_EXCEPTION_BEG	= 0,
		E_EXCEPTION_INIT_CRITICAL_SECTION_FAILED,
		E_EXCEPTION_END
	};
public:
	mtDSQueue();
	~mtDSQueue();

	int		init(int iSpinCount);
	int		exit(void);

	bool    bHaveRepeat();

	int		push(_In_ _Tclass* pT);
	int		push(_In_ _Tclass* pTList, int iTListNum);
	int		pop(_Out_ _Tclass* pT);
	int		pop(_In_ _Tclass* pTList, int iTListNum);

	int		remove(_In_ _Tclass* pT);

	int		getNodeNumUsed();
	long	getNodeUseCount();

	virtual int exception();

	_Tclass		m_pTHead[_iNodeNumTotal];	/// ����ͷ��

	int			m_iNodeIdxFront;			/// �������±�
	int			m_iNodeIdxBack;				/// ������±�
	const int	mc_iNodeNumTotal; 			/// �������
	int			m_iNodeNumUsed;				/// ��ǰ���ýڵ���
	long		m_iNodeUseCount;				/// ʹ�ô���

	CRITICAL_SECTION	m_kCriticalSection;
	int		m_iRetCirticalSection;
};


 /************************************************************************
  * @function:	pop                                                     
  * @describe:	�Ӷ����е���һ��Ԫ��
  * @param-out:	pT
  * @return:	
 		-�ɹ�			> 0
 		-pTΪ��			-1
 		-����Ϊ��		-2
  * @author:		fuke
  ************************************************************************
  */
template<class _Tclass, int _iNodeNumTotal>
int mtDSQueue<_Tclass, _iNodeNumTotal>::pop( _In_ _Tclass* pTList, int iTListNum )
{
	if (NULL == pTList)
	{
		return	-1;
	}

	EnterCriticalSection(&m_kCriticalSection);
	if (m_iNodeNumUsed < iTListNum)
	{
		LeaveCriticalSection(&m_kCriticalSection);
		return	-2;
	}

	m_iNodeNumUsed	-= iTListNum;

	int		iNodeIdx;
	for (iNodeIdx = 0; iNodeIdx < iTListNum; iNodeIdx++)
	{
		pTList[iNodeIdx]		= m_pTHead[m_iNodeIdxBack++];

		//////////////////////////////////////
		m_pTHead[m_iNodeIdxBack-1] = NULL;
		//////////////////////////////////////

		if (m_iNodeIdxBack >= mc_iNodeNumTotal)
		{
			m_iNodeIdxBack	-= mc_iNodeNumTotal;
		}
	}

	LeaveCriticalSection(&m_kCriticalSection);

	return	1;
}

template<class _Tclass, int _iNodeNumTotal>
int	mtDSQueue<_Tclass, _iNodeNumTotal>::push(_In_ _Tclass* pTList, int iTListNum)
{
	if (NULL == pTList)
	{
		return	-1;	
	}

	EnterCriticalSection(&m_kCriticalSection);
	if (m_iNodeNumUsed >= (mc_iNodeNumTotal - iTListNum + 1))
	{
		LeaveCriticalSection(&m_kCriticalSection);
		return	-2;
	}

	m_iNodeNumUsed += iTListNum;

	int		iNodeIdx;
	for (iNodeIdx = 0; iNodeIdx < iTListNum; iNodeIdx++)
	{
		m_pTHead[m_iNodeIdxFront++] = pTList[iNodeIdx];

		if (m_iNodeIdxFront >= mc_iNodeNumTotal)
		{
			m_iNodeIdxFront	-= mc_iNodeNumTotal;
		}
	}

	LeaveCriticalSection(&m_kCriticalSection);
	return	1;
}

template<class _Tclass, int _iNodeNumTotal>
int mtDSQueue<_Tclass, _iNodeNumTotal>::getNodeNumUsed()
{
	return	m_iNodeNumUsed;
}
template<class _Tclass, int _iNodeNumTotal>
long mtDSQueue<_Tclass, _iNodeNumTotal>::getNodeUseCount()
{
	return	m_iNodeUseCount;
}

template<class _Tclass, int _iNodeNumTotal>
int mtDSQueue<_Tclass, _iNodeNumTotal>::exception()
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

template<class _Tclass, int _iNodeNumTotal>
int mtDSQueue<_Tclass, _iNodeNumTotal>::exit( void )
{
	if (0 != m_iRetCirticalSection) 
	{
		DeleteCriticalSection(&m_kCriticalSection);
	}

	return	1;
}

template<class _Tclass, int _iNodeNumTotal>
int mtDSQueue<_Tclass, _iNodeNumTotal>::init( int iSpinCount )
{
	m_iRetCirticalSection	= InitializeCriticalSectionAndSpinCount(&m_kCriticalSection, iSpinCount);
	MT_EXCEPTION_DEBUG(0 == m_iRetCirticalSection, E_EXCEPTION_INIT_CRITICAL_SECTION_FAILED);
	return	1;
}

template<class _Tclass, int _iNodeNumTotal>
bool mtDSQueue<_Tclass,_iNodeNumTotal>::bHaveRepeat()
{
	EnterCriticalSection(&m_kCriticalSection);
	if(m_iNodeIdxFront >= m_iNodeIdxBack)
	{
		for(int i = m_iNodeIdxBack;i<m_iNodeIdxFront;i++)
			for(int j = i + 1;j <= m_iNodeIdxFront;j++)
			{
				if(m_pTHead[i] == m_pTHead[j] && i != j)
				{					
					LeaveCriticalSection(&m_kCriticalSection);
					return true;
				}
			}
	}
	else
	{
		for(int i = 0;i<mc_iNodeNumTotal - 1;i++)
			for(int j = i + 1;j < mc_iNodeNumTotal;j++)
			{
				if(i > m_iNodeIdxFront && i < m_iNodeIdxBack)
				{
					i =m_iNodeIdxBack;
				}
				if(j > m_iNodeIdxFront && j < m_iNodeIdxBack)
				{
					j =m_iNodeIdxBack;
				}
				if(m_pTHead[i] == m_pTHead[j] && i != j)
				{
					LeaveCriticalSection(&m_kCriticalSection);
					return true;
				}
			}
	}
	LeaveCriticalSection(&m_kCriticalSection);
	return false;
}
 /************************************************************************
  * @function:	pop                                                     
  * @describe:	�Ӷ����е���һ��Ԫ��
  * @param-out:	pT
  * @return:	
 		-�ɹ�			> 0
 		-pTΪ��			-1
 		-����Ϊ��		-2
  * @author:		fuke
  ************************************************************************
  */
template<class _Tclass, int _iNodeNumTotal>
int mtDSQueue<_Tclass, _iNodeNumTotal>::pop( _Out_ _Tclass* pT )
{
	if (NULL == pT)
	{
		return	-1;
	}

//	TryEnterCriticalSection(&m_kCriticalSection);
	EnterCriticalSection(&m_kCriticalSection);
	if (m_iNodeNumUsed <= 0)
	{
		LeaveCriticalSection(&m_kCriticalSection);
		return	-2;
	}

	m_iNodeNumUsed	--;
	*pT		= m_pTHead[m_iNodeIdxBack++];
	 
	//////////////////////////////////////
	m_pTHead[m_iNodeIdxBack-1] = NULL;
	//////////////////////////////////////

	m_iNodeUseCount++;

	//MT_DEBUG_PTCL("\npop %d  %d  !!!",(m_iNodeIdxBack-1),*pT);

	if (m_iNodeIdxBack >= mc_iNodeNumTotal)
	{
		m_iNodeIdxBack	-= mc_iNodeNumTotal;
	}

	LeaveCriticalSection(&m_kCriticalSection);
	return	1;
}

 /************************************************************************
  * @function:	push                                                     
  * @describe:	�������ѹ��һ��Ԫ��
  * @param-in:	pT
  * @return:	
 		-�ɹ�			> 0
 		-pTΪ��			-1
 		-��������		-2
  * @author:		fuke
  ************************************************************************
  */
template<class _Tclass, int _iNodeNumTotal>
int mtDSQueue<_Tclass, _iNodeNumTotal>::push( _In_ _Tclass* pT )
{
	if (NULL == pT)
	{
		return	-1;	
	}

//	TryEnterCriticalSection(&m_kCriticalSection);
	EnterCriticalSection(&m_kCriticalSection);
	if (m_iNodeNumUsed >= mc_iNodeNumTotal)
	{
		LeaveCriticalSection(&m_kCriticalSection);
		return	-2;
	}
	m_iNodeNumUsed ++;
	
	m_pTHead[m_iNodeIdxFront++]	= *pT;


	if (m_iNodeIdxFront >= mc_iNodeNumTotal)
	{
		m_iNodeIdxFront	-= mc_iNodeNumTotal;
	}

	LeaveCriticalSection(&m_kCriticalSection);
	return	1;

}


template<class _Tclass, int _iNodeNumTotal>
int	mtDSQueue<_Tclass, _iNodeNumTotal>::remove(_In_ _Tclass* pT)
{
	if (NULL == pT)
	{
		return	-1;	
	}

	EnterCriticalSection(&m_kCriticalSection);
	if (m_iNodeNumUsed <= 0)
	{
		LeaveCriticalSection(&m_kCriticalSection);
		return	-2;
	}

	int iCount = m_iNodeNumUsed;
	while (0 < iCount)
	{
		/// ��ȡ��һ��
		m_iNodeNumUsed	--;
		_Tclass tTmp;
		tTmp	= m_pTHead[m_iNodeIdxBack++];
		if (m_iNodeIdxBack >= mc_iNodeNumTotal)
		{
			m_iNodeIdxBack	-= mc_iNodeNumTotal;
		}

		if (tTmp == *pT)
		{
			break;
		}

		/// ������Ҫ�ģ��ٷŽ�������ȥ
		m_iNodeNumUsed ++;
		m_pTHead[m_iNodeIdxFront++]	= tTmp;
		if (m_iNodeIdxFront >= mc_iNodeNumTotal)
		{
			m_iNodeIdxFront	-= mc_iNodeNumTotal;
		}

		iCount--;
	}

	LeaveCriticalSection(&m_kCriticalSection);
	return	1;
}

template<class _Tclass, int _iNodeNumTotal>
mtDSQueue<_Tclass, _iNodeNumTotal>::~mtDSQueue()
{

}

template<class _Tclass, int _iNodeNumTotal>
mtDSQueue<_Tclass, _iNodeNumTotal>::mtDSQueue() 
	: mc_iNodeNumTotal(_iNodeNumTotal)
{
	m_iNodeIdxFront		= 0;
	m_iNodeIdxBack		= 0;
	m_iNodeNumUsed		= 0;

	m_iNodeUseCount		= 0;

	m_iRetCirticalSection	= 0;
}

#endif	///	__MT_DS_QUEUE_H