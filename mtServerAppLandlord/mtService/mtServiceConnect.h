#ifndef		__MT_SERVICE_CONNECT_H
#define		__MT_SERVICE_CONNECT_H
#include "mtQueueWork.h"
#include "mtService.h"
///#include "mtSQLQDBC.h"
#include "mtQueueList.h"
#include "mtQueueMgr.h"

/// ���Ӵ�������
class mtServiceConnect	: public mtService
{

public:
// 	enum 
// 	{
// 		E_LOGIN_HALL_TYPE_BEG          = 0,
// 		E_LOGIN_HALL_TYPE_NO_ID        = E_LOGIN_HALL_TYPE_BEG,    /// ûId��¼(��ʼ��¼)
// 		E_LOGIN_HALL_TYPE_ID,                                      /// Id��¼
// 		E_LOGIN_HALL_TYPE_ACCOUNT,                                 /// �˺ŵ�¼ XL0000 �οͣ� ����Ϊ�û�ע���˺�
// 		E_LOGIN_HALL_TYPE_PHONE,                                   /// �ֻ�����
// 		E_LOGIN_HALL_TYPE_QQ,                                      /// QQ����
// 
// 		E_LOGIN_HALL_TYPE_END
// 	};

	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long 							plVersion[4];			/// �汾�� 0�޸Ļ���bug�� 1��ʾ���ӹ��ܣ� 2��ʾ�ش��޸�
		long                            lUserId;                /// �û�id > 0
		long							lBatchId;				/// ���κ�id
		long							lVendorId;				/// ���̴���
		long                            lRegister;              /// �Ƿ��Ѿ�ע��(0 -δע�ᣬ1 -��ע��)��δע���¸��ֶ�lUserRegisterDate��ʾ�״ο��ٵ�¼��ʱ��
		long                            lUserRegisterDate;      /// ע�����ڸ�2�ֽڱ�ʾ�꣬�ε��ֽڱ�ʾ�£����ֽڱ�ʾ��(XXXX-XX-XX)
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lResult;                /// ��¼�������(0 -�˺Ŵ���1 -��¼�ɹ�)
		mtQueueHall::UserInfo           kUserInfo;              /// �û���Ϣ
		mtQueueHall::DataSpace          pkSpace[2];   			/// ����Ϣ
	};

	struct DataInit
	{
		long							lStructBytes;
		char*                           pcServiceExeDir;
		mtQueueHall*                    pkQueueHall;   		/// ������Ϣ
		mtQueuePacket*                  pkQueuePacket;
		mtSQLEnv*						pkSQLEnv;
	};

public:
	mtServiceConnect();
	virtual ~mtServiceConnect();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);
	void*   getUserNodeAddr(long lUserId);
	static  void calcContinueLoginHall(mtQueueHall::UserInfo* pkUserInfo);
	void   initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);
	void   UpdateUserProp(long lUserId,long *plUserPropsCount);
public:
	mtQueuePacket *                   m_pkQueuePacket;
	mtQueueHall *                     m_pkQueueHall;   		/// ������Ϣ
	mtSQLEnv *		                  m_pkSQLEnv;
	long							  m_lBatchId;				/// ���κ�id
	long							  m_lVendorId;				/// ���̴���
};

#endif	///	__MT_SERVICE_CONNECT_H