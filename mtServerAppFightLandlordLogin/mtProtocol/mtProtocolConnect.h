#pragma once 
#include "mtProtocol.h"
#include "mtQueueUser.h"

class	mtProtocolConnect : public mtProtocol
{
public:
	enum	EAddrType
	{
		E_ADDR_TYPE_BEG			= 0,

		E_ADDR_TYPE_DOWNLOAD	= E_ADDR_TYPE_BEG,
		E_ADDR_TYPE_CHARGE,
		E_ADDR_TYPE_HALL,

		E_ADDR_TYPE_END			= 3
	};


	struct	DataSendAddr 
	{
		unsigned long	ulIP;
		long			lPort;
	};

	struct	DataSend
	{
		mtProtocol::DataSend	kDataHead;
		long					lVersion[4];
		long                    lUpdateVersion;                     /// �ͻ����Ƿ�ǿ������(0 -��ǿ���������ɿͻ��˾�����1 -ǿ����������������)
		long                    lUpdateUrlType;                     /// 0 -��ʱû�����������Ϣ 1 -��ip��ַ(127.0.0.1)��2 -url��ַ(http://www.dshine.com.cn/update/update.apk)
		long                    lUpdateServerPort;                  /// ������·������Ķ˿�(��ʱδʹ��)
		char                    pcUpdateUrl[MT_APP_UPDATE_URL_LEN]; /// ������·�������ַ(http://www.dshine.com.cn/update/update.apk)
		DataSendAddr			pkAddr[E_ADDR_TYPE_END];
	};

	struct	DataRecv 
	{
		mtProtocol::DataRecv	kDataHead;
		long					lVersion[4];
		char                    pcPatchId[MT_BYTES_OF_PATCH_ID];      /// ���κ�id
		char                    pcVendorId[MT_BYTES_OF_VENDOR_ID];    /// ���̴���
	};

public:
	mtProtocolConnect();
	virtual ~mtProtocolConnect();

	int		init(DataInit* pkDataInit);
	virtual int	exit();

	virtual	int	run(mtThreadWork::DataUser* pkDataUser);
	int     updateVersion(DataRecv* pkDataRecv, DataSend* pkDataSend);
	int     getUpdateInfo(mtThreadWork::DataUser* pkDataUser);

	//SOCKET					m_uiSocker;
	mtQueueUser::ProjectInfo	kProjectInfo;
};