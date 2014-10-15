#ifndef		__MT_PROTOCOL_REGISTER_H
#define		__MT_PROTOCOL_REGISTER_H

#include "mtProtocol.h"

class	mtProtocolRegister : public mtProtocol
{
public:
	enum	EFileType 
	{
		E_REGISTER_TYPE_BEG	= 0,

		E_REGISTER_TYPE_ID	= E_REGISTER_TYPE_BEG,
		E_REGISTER_TYPE_ACCOUNT,
		E_REGISTER_TYPE_QQ,
		E_REGISTER_TYPE_PHONE,
		E_REGISTER_TYPE_TRY,

		E_REGISTER_TYPE_END
	};
	struct	DataSend
	{
		mtProtocol::DataSend	kDataHead;
		unsigned long			uUserId;                              /// �û�id
		char                    pcAccount[MT_BYTES_OF_ACCOUNT];       /// �û��˺�
		char                    pcPwd[MT_BYTES_OF_PWD];               /// ע���˺�����(���ٵ�¼Ĭ����000000)
		long                    lRegister;                            /// �Ƿ��Ѿ�ע��(0 -δע�ᣬ1 -��ע��)��δע���¸��ֶ�lUserRegisterDate��ʾ�״ο��ٵ�¼��ʱ��
		long                    lUserRegisterDate;                    /// ע�����ڸ�2�ֽڱ�ʾ�꣬�ε��ֽڱ�ʾ�£����ֽڱ�ʾ��(XXXX-XX-XX)
	};	

	struct	DataRecv 
	{
		mtProtocol::DataRecv	kDataHead;
		unsigned long           uUserId;                              /// �û�id(0 -��ʾ��idע�ᣬ������ʾ��idע��)
		long					lAccountType;
		char                    pcAccount[MT_BYTES_OF_ACCOUNT];
		char                    pcQQ[MT_BYTES_OF_QQ];
		char                    pcPhone[MT_BYTES_OF_PHONE];
		char                    pcPwd[MT_BYTES_OF_PWD];               /// ע���˺�����(���ٵ�¼Ĭ����000000)
		char                    pcCompanyId[MT_BYTES_OF_COMPANY_ID];  /// ��˾id
		char                    pcPatchId[MT_BYTES_OF_PATCH_ID];      /// ���κ�id
		char                    pcVendorId[MT_BYTES_OF_VENDOR_ID];    /// ���̴���
		char                    pcResevation[MT_BYTES_OF_RESERVATION];/// ��չ�ֶ�
	};

public:
	mtProtocolRegister();
	virtual ~mtProtocolRegister();

	int		init(DataInit* pkDataInit);
	virtual int	exit();

	virtual	int	run(mtThreadWork::DataUser* pkDataUser);

	unsigned long	getNewId();

	unsigned long	m_ulId;

};
#endif	///	__MT_PROTOCOL_REGISTER_H