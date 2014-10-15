#include "mtThreadMgr.h"

mtThreadMgr::mtThreadMgr() :
	m_pkThreadWork(NULL),
	m_iThreadWorkTotal(0)
{
	m_kWSAData.wVersion = 0;
	memset (&m_kThreadWorkInitData, 0, sizeof(m_kThreadWorkInitData));
}

mtThreadMgr::~mtThreadMgr()
{

}

int mtThreadMgr::init(DataInit* pkDataInit)
{
	/// ��ʼ������ģ�鶯̬���ӿ�
	MT_EXCEPTION_DEBUG(NO_ERROR != WSAStartup(MAKEWORD(2,2), &m_kWSAData), EXCEPTION_WSAStartup);

	/// 
	m_pkServiceMgr						= pkDataInit->pkServiceMgr;
	m_pkServiceMonitorMgr               = pkDataInit->pkServiceMonitorMgr;
	m_pkDataHall                        = pkDataInit->pkDataHall;
	/// m_pkSpace                           = pkDataInit->pkSpace;

	/// ��ʼ�� ��������
	m_iThreadWorkTotal = pkDataInit->iThreadWorkTotal;
	m_kQueueWorkList.init(m_iThreadWorkTotal);

	/// ��ʼ�� �����߳�
	mtThreadConnect::DataInit	kThreadListenInitData;
	memset (&kThreadListenInitData, 0, sizeof(kThreadListenInitData));
	kThreadListenInitData.iStructBytes	= sizeof(kThreadListenInitData);
	kThreadListenInitData.ulIpAddress	= pkDataInit->kDataInitListen.ulIpAddress;
	kThreadListenInitData.iPort			= pkDataInit->kDataInitListen.iPort;
	kThreadListenInitData.iListenNum	= pkDataInit->kDataInitListen.iListenNum;
	kThreadListenInitData.pkQueueWorkList	= &m_kQueueWorkList;
	m_kThreadListen.init(&kThreadListenInitData);

	/// ��ʼ�� �����߳�
	MT_EXCEPTION_DEBUG(NULL == (m_pkThreadWork = new mtThreadWork[m_iThreadWorkTotal]), EXCEPTION_THREAD_WORK_NEW);
	int		iThreadWorkdIdx;
	memset (&m_kThreadWorkInitData, 0, sizeof(m_kThreadWorkInitData));
	m_kThreadWorkInitData.lStructBytes		= sizeof(m_kThreadWorkInitData);
	m_kThreadWorkInitData.pkQueueWorkList	= &m_kQueueWorkList;
	m_kThreadWorkInitData.pkServiceMgr		= m_pkServiceMgr;
	for (iThreadWorkdIdx = 0; iThreadWorkdIdx < m_iThreadWorkTotal; iThreadWorkdIdx++)
	{
		m_pkThreadWork[iThreadWorkdIdx].init(&m_kThreadWorkInitData);
	}

	/// ��ʼ�������������������߳�
	mtThreadMonitorConnect::DataInit	kThreadMonitorConnInitData;
	kThreadMonitorConnInitData.lStructBytes 	    = sizeof(kThreadMonitorConnInitData);
	kThreadMonitorConnInitData.iPort                = g_lMonitorServerPort;
	kThreadMonitorConnInitData.iListenNum           = pkDataInit->kDataInitListen.iListenNum;
	kThreadMonitorConnInitData.pkServiceMonitorMgr  = m_pkServiceMonitorMgr;
	kThreadMonitorConnInitData.pkDataHall           = m_pkDataHall;
	/// kThreadMonitorConnInitData.pkSpace              = m_pkSpace;

	m_kThreadMonitorConnect.init(&kThreadMonitorConnInitData);

	/// ��ʼ������߳�
	Sleep(1000);
	mtThreadMonitor::DataInit		kThreadMonitorInitData;
	kThreadMonitorInitData.lStructBytes	= sizeof(kThreadMonitorInitData);
	kThreadMonitorInitData.iThreadWorkTotal	= m_iThreadWorkTotal;
	kThreadMonitorInitData.pkThreadWork		= m_pkThreadWork;
	kThreadMonitorInitData.pkThreadWorkInitData	= &m_kThreadWorkInitData;
	kThreadMonitorInitData.pkDataHall           = m_pkDataHall;
	kThreadMonitorInitData.pkSQLEnv				= pkDataInit->pkSQLEnv;
	m_kThreadMonitor.init(&kThreadMonitorInitData);

	return	1;
}

int mtThreadMgr::exit()
{
	m_kThreadMonitor.exit();
	m_kThreadListen.exit();

	if (NULL != m_pkThreadWork)
	{
		int		iThreadWorkIdx;
		for (iThreadWorkIdx = 0; iThreadWorkIdx < m_iThreadWorkTotal; iThreadWorkIdx++)
		{
			m_pkThreadWork[iThreadWorkIdx].exit();
		}

		delete [] m_pkThreadWork;
		m_pkThreadWork =  NULL;
	}

	/// ����ڴ�
	m_kQueueWorkList.exit();

	if (0 != m_kWSAData.wVersion)
	{
		WSACleanup();
		m_kWSAData.wVersion	= 0;
	}
	return	1;
}

int mtThreadMgr::exception()
{
	switch (m_iExceptionId)
	{
		MT_EXCETPION_STRING(EXCEPTION_WSAStartup)
		MT_EXCETPION_STRING(EXCEPTION_THREAD_LISTEN_NEW)
		MT_EXCETPION_STRING(EXCEPTION_THREAD_WORK_NEW)
	default:
		MT_DEBUG(7, "���쳣û�б�����");
		return	0;
	}

	return	1;
}

int mtThreadMgr::run()
{
	return	1;
}

