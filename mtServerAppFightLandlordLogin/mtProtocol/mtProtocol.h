#ifndef		__MT_PROTOCOL_H
#define		__MT_PROTOCOL_H
#include "mtPlatform.h"
#include "mtThreadWork.h"

class	mtFileMgr;
class	mtSQLEnv;
//#include "mtSQLEnv.h"

class mtProtocol
{
public:
	enum	EResultId
	{
		E_RESULT_BEG,

		E_RESULT_SUCCESS_NEED_SEND				= 2,
		E_RESULT_SUCCESS						= 1,		/// �ɹ�	
		E_RESULT_FAIL							= 0,
		E_RESULT_FAIL_USER_IS_EXIST				= -1,		/// �û����Ѿ�����
		E_RESULT_FAIL_PWD_INVALID				= -2,		/// �������
		E_RESULT_FAIL_APP_IS_NEWEST				= -3,		/// �Ѿ�����,����Ҫ����
		E_RESULT_FAIL_USER_NOT_EXIST			= -4,		/// �û�������
		E_RESULT_FAIL_ACCOUNT_INVALIDE			= -5,
		E_RESULT_FAIL_RECONNECT					= -6,

		E_RESULT_FALT_FILE_INVALIDE_PATH		= -100000,
		E_RESULT_FALT_FILE_NOT_EXIST			= -100001,
		E_RESULT_FALT_FILE_READ_FUNCTION		= -100002,
		E_RESULT_FALT_FILE_READ_COMPLETE		= -100003,
		E_RESULT_FALT_FILE_WRITE_FUNCTION		= -100004,
		E_RESULT_FALT_FILE_WRITE_COMPLETE		= -100005,
		E_RESULT_FALT_FILE_CREATE_FUNCTION		= -100006,
		E_RESULT_FALT_FILE_CREATE_CALLBACK		= -100007,
		E_RESULT_FALT_FILE_LOCK					= -100008,

		E_RESULT_END,
	};

	enum EProtocolId
	{
		E_PROTOCOL_BEG,

		E_PROTOCOL_REGISTER	= E_PROTOCOL_BEG,	/// ע��
		E_PROTOCOL_LOGIN,						/// ��½
		E_PROTOCOL_CONNECT,						/// ����,������
		E_PROTOCOL_ALTER_PASSWORD,				/// �޸�����

		E_PROTOCOL_END
	};

public:
	struct	DataInit
	{
		long					lStructBytes;
		unsigned long			ulIdBeg;
		mtSQLEnv*				pkSQLEnv;
		mtFileMgr*				pkFileMgr;
		CRITICAL_SECTION*	    pkHallMaxIdCs;
		LPFN_DISCONNECTEX		lpfnDisconnectEx;
	};

	struct  DataSend
	{
		long		lStructBytes;			/// ����С
		long		lProtocolId;			/// ��������
		long		lKey;					/// �����û�ʹ�ã���DataSend->lKey
		long		lResultId;				/// ���ؽ����������Ϣ�鿴 EResultType
		long		plReservation[2];		/// ����
	};											 
												 
	struct	DataRecv 							 
	{											 
		long		lStructBytes;			/// ����С
		long		lProtocolId;			/// ��������
		long		lKey;					/// �����û�ʹ��, ��DataSend->lKey ����
		long		plReservation[2];		/// ����
	};


public:
	mtProtocol();
	virtual ~mtProtocol();

	virtual int	init(DataInit* pkDataInit);
	virtual int exit();

	virtual	int	run(mtThreadWork::DataUser* pkDataUser);

 	static	LPFN_DISCONNECTEX	m_lpfnDisconnectEx;

	mtSQLEnv*					m_pkSQLEnv;
	CRITICAL_SECTION*	        m_pkHallMaxIdCs;
};
#endif	///	__MT_PROTOCOL_H