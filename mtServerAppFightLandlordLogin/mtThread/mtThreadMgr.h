#pragma once

#include "mtThreadWork.h"
#include "mtPlatform.h"
#include "mtThreadMonitor.h"

class mtSQLEnv;

class mtThreadMgr : public mtException
{
public:
	friend class mtThreadMonitor;
public:
	enum 
	{
		E_EXCEPTION_BEG	= 0,

		E_EXCEPTION_IOCP_CREATE,

		E_EXCEPTION_SERVER_SOCKET,
		E_EXCEPTION_SERVER_OPT_RCVBUF,
		E_EXCEPTION_SERVER_OPT_SNDBUF,
		E_EXCEPTION_SERVER_OPT_REUSEADDR,
		E_EXCEPTION_SERVER_IOCP_BIND,
		E_EXCEPTION_SERVER_BIND,
		E_EXCEPTION_SERVER_LISTEN,
		E_EXCEPTION_SERVER_ACCEPTEX,

		E_EXCEPTION_CLIENT_MALLOC,
		E_EXCEPTION_CLIENT_SOCKET,
		E_EXCEPTION_CLIENT_OPT_RCVBUF,
		E_EXCEPTION_CLIENT_OPT_SNDBUF,
		E_EXCEPTION_CLIENT_IOCP_BIND,
		

		E_EXCEPTION_THREADWORK_ALLOC,
		E_EXCEPTION_END
	};
public:
	struct DataInit
	{
		long				lStructBytes;
		unsigned long		ulWorkIP;
		unsigned short		usWorkPort;
		unsigned long		ulTimeWaitMilliseconds;
		int					iTimeConnectLimitSencods;
		mtSQLEnv*			pkSQLEnv;
		char*				pcFolder;
		LPFN_ACCEPTEX		lpfnAcceptEx;
		LPFN_DISCONNECTEX	lpfnDisconnectEx;
	};
public:
	mtThreadMgr();
	~mtThreadMgr();

	int		init(DataInit* pkDataInit);
	int		exit();
	int		run();

	int		exception();

private:
	int		initThreadWork();
	int		initThreadMonitor();

	HANDLE	createIOCP(int iThreadTotal);
	SOCKET	createSocketServer(HANDLE hIOCP, int iListenTotal, unsigned long ulIP, unsigned short usPort);
	SOCKET	createSocketClient(HANDLE hIOCP, void* pData);
	 
	LPFN_ACCEPTEX					m_lpfnAcceptEx;
	LPFN_DISCONNECTEX				m_lpfnDisconnectEx;

	DataInit						m_kThreadMgrDataInit;
	
	mtThreadWork::DataInit			m_kThreadWorkDataInit;		/// �����߳�ʹ�õĳ�ʼ������
	mtThreadWork*					m_pkThreadWorkList;			/// �����߳�����
	int								m_iThreadWorkTotal;			/// �����̵߳�����
	mtThreadWork::DataUser*			m_pkThreadWorkUserList;
	int								m_iThreadWorkUserTotal;

	mtSQLEnv*						m_pkSQLEnv;

	mtThreadMonitor::DataInit		m_kThreadMonitorDataInit;
	mtThreadMonitor					m_kThreadMonitor;

	CRITICAL_SECTION	            m_kHallMaxIdCs;				/// �ٽ�������
};

