#ifndef		__MT_DATA_STRUCT_QUEUE_H
#define		__MT_DATA_STRUCT_QUEUE_H
#include "mtPlatform.h"
#include "mtException.h"

template<class _Tclass, int _iNodeNumTotal>
class mtDataStructQueue : public mtException
{
public:
	enum 
	{
		E_EXCEPTION_BEG	= 0,
		E_EXCEPTION_INIT_CRITICAL_SECTION_FAILED,
		E_EXCEPTION_END
	};
public:
	mtDataStructQueue();
	~mtDataStructQueue();

	int		init(int iSpinCount);
	int		exit(void);

	int		push(_In_ _Tclass* pT);
	int		pop(_Out_ _Tclass* pT);

	virtual int exception();

	_Tclass		m_pTHead[_iNodeNumTotal];	/// ����ͷ��

	int		m_iNodeIdxFront;			/// �������±�
	int		m_iNodeIdxBack;				/// ������±�
	const int	mc_iNodeNumTotal; 			/// �������
	int		m_iNodeNumUsed;				/// ��ǰ�Ѿ�ʹ���˵Ľ����Ŀ

	CRITICAL_SECTION	m_kCriticalSection;
	int		m_iRetCirticalSection;
};

template<class _Tclass, int _iNodeNumTotal>
int mtDataStructQueue<_Tclass, _iNodeNumTotal>::exception()
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
int mtDataStructQueue<_Tclass, _iNodeNumTotal>::exit( void )
{
	if (0 != m_iRetCirticalSection) 
	{
		DeleteCriticalSection(&m_kCriticalSection);
	}

	return	1;
}

template<class _Tclass, int _iNodeNumTotal>
int mtDataStructQueue<_Tclass, _iNodeNumTotal>::init( int iSpinCount )
{
	m_iRetCirticalSection	= InitializeCriticalSectionAndSpinCount(&m_kCriticalSection, iSpinCount);
	MT_EXCEPTION_DEBUG(0 == m_iRetCirticalSection, E_EXCEPTION_INIT_CRITICAL_SECTION_FAILED);
	return	1;
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
int mtDataStructQueue<_Tclass, _iNodeNumTotal>::pop( _Out_ _Tclass* pT )
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
int mtDataStructQueue<_Tclass, _iNodeNumTotal>::push( _In_ _Tclass* pT )
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

	//MT_DEBUG_PTCL("push socket\n");
	LeaveCriticalSection(&m_kCriticalSection);
	return	1;

}

template<class _Tclass, int _iNodeNumTotal>
mtDataStructQueue<_Tclass, _iNodeNumTotal>::~mtDataStructQueue()
{

}

template<class _Tclass, int _iNodeNumTotal>
mtDataStructQueue<_Tclass, _iNodeNumTotal>::mtDataStructQueue() 
	: mc_iNodeNumTotal(_iNodeNumTotal)
{
	m_iNodeIdxFront		= 0;
	m_iNodeIdxBack		= 0;
	m_iNodeNumUsed		= 0;

	m_iRetCirticalSection	= 0;
}

#endif	///	__MT_DATA_STRUCT_QUEUE_H