#ifndef		__MT_SERVICE_HALL_EXIT_H
#define		__MT_SERVICE_HALL_EXIT_H
#include "mtQueueWork.h"
#include "mtService.h"
#include "mtQueueList.h"
#include "mtQueueMgr.h"
/// �˳���������
class mtServiceHallExit	: public mtService
{

public:
	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		mtQueueHall::UserInfo           kUserInfo;              /// �û���Ϣ
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lResult;                /// �û��˳����������0 -�û���Ϣ����ʧ�ܣ� 1 -�û���Ϣ���³ɹ���2 -�����û���Ϣ����3 -�����û���ǰ������
		long                            lUserId;                /// �û�id
	};

	struct DataInit
	{
		long							lStructBytes;
		char*                           pcServiceExeDir;
		mtQueuePacket*                  pkQueuePacket;
		mtQueueHall::UserDataNode*      pkUserDataNodeList;       /// �û���Ϣ

		mtSQLEnv*		pkSQLEnv;
	};

public:
	mtServiceHallExit();
	virtual ~mtServiceHallExit();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);

	int     runLoginHallNoId(DataWrite* pkDataWrite);
	int     runLoginHallOther(SOCKET iSocket, DataWrite* pkDataWrite);

	void*   getUserNodeAddr(long lUserId);
	void   initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);
public:
	mtQueuePacket*                   m_pkQueuePacket;
	mtQueueHall::UserDataNode*       m_pkUserDataNodeList;       /// �û���Ϣ

	mtSQLEnv*		m_pkSQLEnv;
};

#endif	///	__MT_SERVICE_HALL_EXIT_H