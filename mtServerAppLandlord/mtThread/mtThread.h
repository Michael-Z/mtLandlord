#ifndef		__MT_THREAD_H
#define		__MT_THREAD_H
#include "mtPlatform.h"
#include "mtException.h"

class	mtThread : public mtException
{
public:
	enum EControlMode
	{
		E_CONTROL_MODE_BEG	= 0,					
		E_CONTROL_MODE_NORMAL	= E_CONTROL_MODE_BEG,	/// ����ģʽ
		E_CONTROL_MODE_DEBUG,							/// ����ģʽ
		E_CONTROL_MODE_END
	};
	enum EThreadState
	{
		E_THREAD_STATE_BEG	= 0,
		E_THREAD_STATE_RUN	= E_THREAD_STATE_BEG,		/// ����״̬
		E_THREAD_STATE_STOP,							/// ֹͣ״̬���߳��Ѿ�����
		E_THREAD_STATE_PAUSE,							/// ��ͣ״̬
		E_THREAD_STATE_TERMINATE,						/// ��ֹ״̬�����ܴ���ֹͣ������
		E_THREAD_STATE_END
	};
public:
	mtThread();
	virtual ~mtThread();

	virtual	int	run()		= 0;
	virtual int exception() = 0;

	EControlMode	getControlMode();
	EThreadState	getThreadState();

	int				setControlMode(EControlMode eControlMode);
	int				setThreadState(EThreadState eThreadState);

	static void	CallBackRun(void* pData);
	
protected:
	EControlMode			m_eControlMode;
	EThreadState			m_eThreadState;
};

inline mtThread::EControlMode mtThread::getControlMode() 
{
	return	m_eControlMode;
}

inline mtThread::EThreadState	mtThread::getThreadState()
{
	return	m_eThreadState;
}

inline int mtThread::setControlMode(EControlMode eControlMode) 
{
	if (eControlMode > E_CONTROL_MODE_BEG && eControlMode < E_CONTROL_MODE_END)
	{
		m_eControlMode	= eControlMode;
		return	1;
	}

	return	0;
}

inline int mtThread::setThreadState(EThreadState eThreadState)
{
	if (eThreadState > E_THREAD_STATE_BEG && eThreadState < E_THREAD_STATE_END)
	{
		m_eThreadState	= eThreadState;
		return	1;
	}

	return	0;

}

#endif	///	__MT_THREAD_H