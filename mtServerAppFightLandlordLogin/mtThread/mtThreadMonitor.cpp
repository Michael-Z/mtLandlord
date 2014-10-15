#include "mtThreadMonitor.h"
#include "mtThreadMgr.h"

#include "mtGlobal.h"

HANDLE	mtThreadMonitor::ms_hIOCP			= NULL;
SOCKET	mtThreadMonitor::ms_uiSocketServer	= INVALID_SOCKET;
LPFN_ACCEPTEX		mtThreadMonitor::ms_lpfnAcceptEx			= NULL;
LPFN_DISCONNECTEX	mtThreadMonitor::ms_lpfnDisconnectEx		= NULL;

mtThreadMonitor::mtThreadMonitor() :
	m_pkThreadMgr(NULL),
	m_ulTimeWaitMilliseconds(0),
	m_iTimeConnectLimitSencods(0),
	m_pkThreadWorkUserList(NULL),
	m_iThreadWorkUserTotal(0),
	m_pkThreadWorkList(NULL),
	m_iThreadWorkTotal(0),
	m_dwLocalAddressLength(0),
	m_dwRemoteAddressLength(0),
	m_dwReceiveDataLength(0)
{

}

mtThreadMonitor::~mtThreadMonitor()
{

}

int mtThreadMonitor::init( DataInit* pkDataInit )
{
	/// ��ʼ����ֵ
 	memcpy (&m_kDataInit, pkDataInit, sizeof(m_kDataInit));
	ms_hIOCP					= pkDataInit->hIOCP;
	ms_uiSocketServer			= pkDataInit->uiSocketServer;
	ms_lpfnAcceptEx				= pkDataInit->lpfnAcceptEx;
	ms_lpfnDisconnectEx			= pkDataInit->lpfnDisconnectEx;
	m_pkThreadMgr				= pkDataInit->pkThreadMgr;
	m_ulTimeWaitMilliseconds	= pkDataInit->ulTimeWaitMilliseconds;
	m_iTimeConnectLimitSencods	= pkDataInit->iTimeConnectLimitSencods;

	/// �ڲ���ȡ��ֵ
	m_pkThreadWorkUserList		= m_pkThreadMgr->m_pkThreadWorkUserList;
	m_iThreadWorkUserTotal		= m_pkThreadMgr->m_iThreadWorkUserTotal;
	m_pkThreadWorkList			= m_pkThreadMgr->m_pkThreadWorkList;
	m_iThreadWorkTotal			= m_pkThreadMgr->m_iThreadWorkTotal;

	/// ��ʼ�� AcceptEx ����
	m_dwLocalAddressLength		= sizeof(SOCKADDR_IN) + 16;
	m_dwRemoteAddressLength		= sizeof(SOCKADDR_IN) + 16;
	m_dwReceiveDataLength		= sizeof(m_pkThreadWorkUserList->pcDataSend) 
		- m_dwLocalAddressLength - m_dwRemoteAddressLength;
  
	/// ��ʼ�� Monitor Э�������
 	memset (&m_kProcotolMgrDataInit, 0, sizeof(m_kProcotolMgrDataInit));
	m_kProcotolMgrDataInit.lStructBytes		= sizeof(m_kProcotolMgrDataInit);
	m_kProcotolMgr.init(&m_kProcotolMgrDataInit);
 
// 	m_pkThreadMgr				= pkDataInit->pkThreadMgr;
// 
// // 	m_kDataInit.lStructBytes	= sizeof(m_kDataInit);
// // 	m_kDataInit.hIOCP			= m_hIOCP;
// // 	m_kDataInit.uiSocketServer  = m_uiSocketServer;
// // 	m_kDataInit.ulTimeWaitMilliseconds		= m_ulTimeWaitMilliseconds;
// // 	m_kDataInit.iTimeConnectLimitSencods	= m_iTimeConnectLimitSencods;
// // 	m_kDataInit.pkUserList		= m_pkThreadWorkUserList;
// // 	m_kDataInit.iUserTotal		= m_iThreadWorkUserTotal;
// // 	m_kDataInit.lpfnAcceptEx	= m_lpfnAcceptEx;
// // 	m_kDataInit.lpfnDisconnectEx			= m_lpfnDisconnectEx;
// // 	m_kDataInit.pkThreadMgr		= m_pkThreadMgr;
// 
// 	m_kDataInit.lStructBytes	= sizeof(m_kDataInit);
// 	m_kDataInit.pkThreadMgr		= m_pkThreadMgr;
// 
// 	m_kServiceMgr.init(&m_kDataInit);

	return	1;
}

int mtThreadMonitor::exit()
{
	printf ("Thread monitor ��ʼ�ر�\n");



	return	1;

}
 
int mtThreadMonitor::run()
{
	//Sleep(INFINITE);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int iUserIdx = 0; iUserIdx < m_iThreadWorkUserTotal; iUserIdx++)
	{
		DWORD seconds;
		int len = sizeof(seconds);
		mtThreadWork::DataUser* pkDataUser = (mtThreadWork::DataUser*)(m_pkThreadWorkUserList+iUserIdx);
		int res = getsockopt(pkDataUser->uiSocket,SOL_SOCKET,SO_CONNECT_TIME,(char*)&seconds,&len);
		//�������Ӳ������ݵĳ�ʱsocket
		if(res == 0 && (seconds>=MT_SERVER_SOCKET_ACCPET_TIMEOUT && seconds<0xFFFFFFFF))
		{					
			
			MT_DEBUG(PTCL, "\ngetsockopt [res:%d] [seconds:%d] !\n",res,seconds);
			pkDataUser->closeSocketMilliSec = 0;
			pkDataUser->closeFlag = FALSE;
			int iRes = closesocket(pkDataUser->uiSocket);
			//pkDataUser->closeSocketMilliSec = 0;
			//pkDataUser->closeFlag = FALSE;
			MT_DEBUG(PTCL, "\nDELETE ERROR CLIENT [Socket:%d] [res:%d] [error:%d] !\n",pkDataUser->uiSocket,iRes,WSAGetLastError());
										//����Ͷ��DisconnectEx��������ʱsocket
		}
		else if(pkDataUser->closeSocketMilliSec != 0 
			&& (GetTickCount()-(pkDataUser->closeSocketMilliSec)) >= MT_SERVER_SOCKET_DISCONNECT_TIMEOUT)
		{

			DWORD sec = GetTickCount()-(pkDataUser->closeSocketMilliSec);

			pkDataUser->closeSocketMilliSec = 0;
			pkDataUser->closeFlag = FALSE;
			int iRes = closesocket(pkDataUser->uiSocket);
			//pkDataUser->closeSocketMilliSec = 0;
			//pkDataUser->closeFlag = FALSE;
			MT_DEBUG(PTCL, "\nDisconnectEx Timeout [Socket:%d] [seconds:%d] [res:%d] [error:%d] !\n",
				pkDataUser->uiSocket,sec,iRes,WSAGetLastError());
			
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return	0;
}
 
int mtThreadMonitor::exception()
{
	switch (m_iExceptionId)
	{
		MT_EXCETPION_STRING(E_EXCEPTION_IOCP_CREATE)
	default:
		MT_DEBUG(7, "�ô���û�б�����!\n");
		return	0;
	}

	return	1;
}



// #endif