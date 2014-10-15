#ifndef			__MT_SERVER_H_
#define			__MT_SERVER_H_

#include "mtPlatform.h"
#include "mtException.h"
#include "mtSQLEnv.h"
#include "mtThreadMgr.h"


class mtServer : public mtException
{
public:
	enum   {				//�쳣����ö��
		E_EXCEPTION_BEG	= 0,

		E_EXCEPTION_WSA_STARTUP,					//��ʼ��socket���쳣
		E_EXCEPTION_CLIENT_SOCKET,					//��ʼ��socket�쳣
		E_EXCEPTION_WSAIOCTL_ACCEPTEX,
		E_EXCEPTION_WSAIOCTL_DISCONNECTEX,
		E_EXCEPTION_CLOSE_SOCKET,

		E_EXCEPTION_END,
	};
	struct DataInit {
		long				lStructBytes;			//DataInit�ṹ���С
		unsigned long		ulWorkIP;				//��¼������IP
		unsigned short		usWorkPort;				//��½�������˿�

		int					iUserListTotal;						//
		unsigned long		ulTimeWaitMilliseconds;				//
		int					iTimeConnectLimitSencods;			//
	};

public:
	mtServer();
	~mtServer();
	int				init(DataInit* pkDataInit);
	int				run();
	int				exit();
	virtual int		exception();

	int             initParams();
	int				initDataBase();
	int				initSocketFunction();
	int				initThreadMgr();
private:
	DataInit					m_kDataInit;						//
	WSADATA						m_kWSAData;
	int							m_iWSADataRet;						//WSAStartup����ֵ
	LPFN_ACCEPTEX				m_lpfnAcceptEx;						//AcceptEx����ָ��
	LPFN_DISCONNECTEX			m_lpfnDisconnectEx;					//DisconnectEx����ָ��
	
	mtThreadMgr					m_kThreadMgr;
	mtSQLEnv					m_kSQLEnv;							//���ݿ�ʵ��
};

#endif		/// __MT_SERVER_H_