#include "mtException.h"

mtException::mtException()
{
	m_iExceptionId	= 0;			//��ʼ���������
}

mtException::~mtException()
{

}

//bConditionΪfalseʱ����¼�쳣���׳�
void	mtException::condition( bool bCondition, int iExceptionId )
{
	if (!bCondition)
	{
		m_iExceptionId	= iExceptionId;
		throw	this;
	}
}
