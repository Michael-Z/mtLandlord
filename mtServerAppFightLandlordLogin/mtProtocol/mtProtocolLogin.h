#pragma once 
#include "mtProtocol.h"

class mtFileMgr;

class	mtProtocolLogin : public mtProtocol
{
public:
	enum	EFileType 
	{
		E_ACCOUNT_TYPE_BEG	= 0,

		E_ACCOUNT_TYPE_ID	= E_ACCOUNT_TYPE_BEG,
		E_ACCOUNT_TYPE_ACCOUNT,
		E_ACCOUNT_TYPE_QQ,
		E_ACCOUNT_TYPE_PHONE,

		E_ACCOUNT_TYPE_END
	};

	struct	DataSend
	{
		mtProtocol::DataSend		kDataHead;
		unsigned long				ulId;
		long                        lRegister;             /// �Ƿ��Ѿ�ע��(0 -δע�ᣬ1 -��ע��)��δע���¸��ֶ�lUserRegisterDate��ʾ�״ο��ٵ�¼��ʱ��
		long                        lUserRegisterDate;     /// ע�����ڸ�2�ֽڱ�ʾ�꣬�ε��ֽڱ�ʾ�£����ֽڱ�ʾ��(XXXX-XX-XX)
	};	

	struct	DataRecv 
	{
		mtProtocol::DataRecv		kDataHead;
		long						lAccountType;
		char                        pcAccount[MT_BYTES_OF_ACCOUNT];
		char                        pcPwd[MT_BYTES_OF_PWD];
	};

public:
	mtProtocolLogin();
	virtual ~mtProtocolLogin();

	int		init(DataInit* pkDataInit);
	virtual int	exit();

	virtual	int	run(mtThreadWork::DataUser* pkDataUser);
};