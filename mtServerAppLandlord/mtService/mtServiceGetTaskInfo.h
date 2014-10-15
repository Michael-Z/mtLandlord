#ifndef 	__MT_SERVICE_LOTTERY_GET_TASK_INFI_H
#define 	__MT_SERVICE_LOTTERY_GET_TASK_INFI_H

#include "mtService.h"
#include "mtQueueHall.h"
/// ��ȡ������ϢЭ��
class 	mtServiceGetTaskInfo : public mtService
{
public:
	struct DataRead
	{
		long		lStructBytes;		// ����С
		long        lServiceType;	    // ��������
		long		plReservation[2];	// �����ֶ�
		long		lUserId;            // �û�id
	};


	struct DataWrite
	{
		long		             lStructBytes;		// ����С
		long		             lServiceType;		// ��������
		long		             plReservation[2];	// �����ֶ�
		mtQueueHall::TaskInfo	 pkTaskInfo[54];
	};
	enum TaskType
	{
		E_TASK_TYPE_DAILY_TASK            = 1,
		E_TASK_TYPE_TOTAL_FIGHT            = 2,
		E_TASK_TYPE_TOTAL_WIN              = 3,
		E_TASK_TYPE_TOTAL_PAY              = 4,
	};
	struct DataInit
	{
		long							lStructBytes;
		char*                           pcServiceExeDir;
		mtQueueHall*                    pkQueueHall;   		// ������Ϣ
		mtQueuePacket*                  pkQueuePacket;
		mtSQLEnv*						pkSQLEnv;
	};
public:
	mtServiceGetTaskInfo();
	virtual ~mtServiceGetTaskInfo();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);
	void  * getUserNodeAddr(long lUserId);
	void   initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);



	mtQueuePacket *                   m_pkQueuePacket;
	mtQueueHall *                     m_pkQueueHall;   		/// ������Ϣ
	mtSQLEnv *		                  m_pkSQLEnv;
};

#endif 	/// __MT_SERVICE_LOTTERY_GET_TASK_INFI_H

