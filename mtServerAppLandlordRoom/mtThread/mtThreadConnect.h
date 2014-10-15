#ifndef		__MT_THREAD_CONNECT_H
#define		__MT_THREAD_CONNECT_H
#include "mtThread.h"
#include "mtQueueList.h"
#include "mtQueueUser.h"

class	mtThreadConnect : public mtThread
{
	/// ���ڲ��ṹ��
public:
	/// ��ʼ�����ڲ��쳣��������
	enum 
	{
		E_EXCEPTION_BEG = 0,
		E_EXCEPTION_INIT_DATA_IS_NULL,
		E_EXCEPTION_SERVER_SOCKET,
		E_EXCEPTION_SERVER_BIND,
		E_EXCEPTION_SERVER_LISTEN,
		E_EXCEPTION_SERVER_IOCTLSOCKET_GET,
		E_EXCEPTION_SERVER_IOCTLSOCKET_SET,
		E_EXCEPTION__BEGINTHREAD,
		E_EXCEPTION_END
	};

	/// ��ʼ����ṹ�����
	struct DataInit
	{
		long				lStructBytes;		/// �ṹ��Ĵ�С
		unsigned long		ulIpAddress;		/// IP ��ַ
		int					iPort;				/// �˿ں�
		int					iListenNum;			/// ������
		mtQueueUser*		pkQueueUser;
		mtQueuePacket*		pkQueuePacket;
	};

public:
	mtThreadConnect();
	virtual ~mtThreadConnect();

	int		init(DataInit* pkInitData);
	int		exit();

	virtual int	run();
	virtual int exception();

	SOCKET			    m_iSocketServer;
	SOCKADDR_IN		    m_kSockAddrInServer;

	SOCKET			    m_uiSocketClient;
	SOCKADDR_IN		    m_kSockAddrInClient;

	HANDLE				m_hThread;	
	mtQueueUser*		m_pkQueueUser;
	mtQueuePacket*		m_pkQueuePacket;
};

#endif	///	__MT_THREAD_CONNECT_H