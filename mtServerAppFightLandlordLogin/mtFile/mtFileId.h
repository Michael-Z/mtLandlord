#pragma once 
#include "mtPlatform.h"
#include "mtFile.h"

class	mtFileId : public mtFile
{
public:
	struct	DataFile
	{
		unsigned long	ulId;
		char	        pcAccount[MT_BYTES_OF_ACCOUNT];
		char            pcQQ[MT_BYTES_OF_QQ];
		char            pcPhone[MT_BYTES_OF_PHONE];
		char            pcPwd[MT_BYTES_OF_PWD];
		char            pcCompanyId[MT_BYTES_OF_COMPANY_ID];   /// ��˾id
		char            pcPatchId[MT_BYTES_OF_PATCH_ID];       /// ���κ�id
		char            pcVendorId[MT_BYTES_OF_VENDOR_ID];     /// ���̴���
		char            pcResevation[MT_BYTES_OF_RESERVATION]; /// ��չ�ֶ�
		long            lRegister;                             /// �Ƿ��Ѿ�ע��(0 -δע�ᣬ1 -��ע��)��δע���¸��ֶ�lUserRegisterDate��ʾ�״ο��ٵ�¼��ʱ��
		long            lUserRegisterDate;                     /// ע�����ڸ�2�ֽڱ�ʾ�꣬�ε��ֽڱ�ʾ�£����ֽڱ�ʾ��(XXXX-XX-XX)
	};

public:
	mtFileId();
	~mtFileId();

	virtual	int				init(DataInit* pkDataInit);
	virtual	char*			getFileName(char* pcAccount);
	virtual unsigned long	getFileOffset(char* pcAccount);
	virtual	unsigned long 	getCreateFileBytes();
	virtual	unsigned long	getDataFileBytes();
	virtual unsigned long	getFolderBytes();

protected:

	char	m_pcFullFileName[256];
	char*	m_pcFileName;
	int		m_iFileNameBytes;
	int		m_iFolderNameBytes;

	static	unsigned long 	m_ulCreateFileBytes;
};

inline unsigned long	mtFileId::getFolderBytes()
{
	return		m_iFolderNameBytes;
}

inline unsigned long	mtFileId::getDataFileBytes()
{
	return		sizeof(DataFile);
}

inline unsigned long 	mtFileId::getCreateFileBytes()
{
	return		m_ulCreateFileBytes;
}


inline unsigned long mtFileId::getFileOffset( char* pcAccount )
{
	unsigned long ulOffset = (*(unsigned long*)pcAccount) & 0x0000FFFF;

	return ulOffset * sizeof(DataFile);
}