#ifndef		__MT_THREAD_CONNECT_H
#define		__MT_THREAD_CONNECT_H
#include "mtThread.h"
#include "mtQueueWork.h"

class	mtThreadConnect : public mtThread
{
	/// ���ڲ��ṹ��
public:
	/// ��ʼ�����ڲ��쳣��������
	enum 
	{
		EXCEPTION_BEG = 0,
		EXCEPTION_INIT_DATA_IS_NULL,
		EXCEPTION_SERVER_socket,
		EXCEPTION_SERVER_bind,
		EXCEPTION_SERVER_listen,
		EXCEPTION_SERVER_ioctlsocket_get,
		EXCEPTION_SERVER_ioctlsocket_set,
		EXCEPTION__beginthread,
		EXCEPTION_END
	};

	/// ��ʼ����ṹ�����
	struct DataInit
	{
	public:
		int		            iStructBytes;		/// �ṹ��Ĵ�С
		unsigned long	    ulIpAddress;		/// IP ��ַ
		int		            iPort;				/// �˿ں�
		int		            iListenNum;			/// ������
		mtQueueWorkList*	pkQueueWorkList;	/// ��������
	};

public:
	mtThreadConnect();
	virtual ~mtThreadConnect();

	int		init(DataInit* pkInitData);
	int		exit();

	virtual int run();
	virtual int exception();

	SOCKET			m_iSocketServer;
	SOCKADDR_IN		m_kSockAddrInServer;

	SOCKET			m_iSocketClient;
	SOCKADDR_IN		m_kSockAddrInClient;

// 	unsigned long	m_ulIoctlSocketFionbioFlagOld;
 	unsigned long	m_ulIoctlSocketFionbioFlagCur;

	uintptr_t		m_uiThread;	
	mtQueueWorkList*	m_pkQueueWorkList;



};

#endif	///	__MT_THREAD_CONNECT_H