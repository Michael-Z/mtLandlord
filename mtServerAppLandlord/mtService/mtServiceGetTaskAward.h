#ifndef 	__MT_SERVICE_LOTTERY_GET_TASK_AWARD_H
#define 	__MT_SERVICE_LOTTERY_GET_TASK_AWARD_H
#include "mtService.h"
#include "mtQueueHall.h"
/// ��ȡ������Э��
class 	mtServiceGetTaskAward : public mtService
{
public:
	struct DataRead
	{
		long		lStructBytes;		// ����С
		long        lServiceType;	    // ��������
		long		plReservation[2];	// �����ֶ�
		long		lUserId;            // �û�id
		long		ltaskId;		    // ��ȡ����������ID
	};


	struct DataWrite
	{
		long		lStructBytes;		// ����С
		long		lServiceType;		// ��������
		long		plReservation[2];	// �����ֶ�
		long		lResult; 
		long        ltaskId;             //����id
		long		lGold;			    // �������(Ŀǰ���) 
	};

	struct DataInit
	{
		long							lStructBytes;
		char*                           pcServiceExeDir;
		mtQueueHall*                    pkQueueHall;   		// ������Ϣ
		mtQueuePacket*                  pkQueuePacket;
		mtSQLEnv*						pkSQLEnv;
	};
	enum TaskType
	{
		E_TASK_TYPE_DAILY_TASK            = 1,
		E_TASK_TYPE_TOTAL_FIGHT            = 2,
		E_TASK_TYPE_TOTAL_WIN              = 3,
		E_TASK_TYPE_TOTAL_PAY              = 4,
	};
	enum Result
	{
		E_RESULT_SUCCESS                    = 1,//�ɹ�
		E_RESULT_ERROR                      = 0,//����
		E_RESULT_MISS_DEMAND                = -1,//δ�ﵽҪ��
		E_RESULT_HAVE_GET                   = -2,//�Ѿ���ȡ
	};
public:
	mtServiceGetTaskAward();
	virtual ~mtServiceGetTaskAward();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);
	void  * getUserNodeAddr(long lUserId);
	void   initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);
	bool  IsVip(long lUserId);

	mtQueuePacket *                   m_pkQueuePacket;
	mtQueueHall *                     m_pkQueueHall; // ������Ϣ
	mtSQLEnv *		                  m_pkSQLEnv;
};

#endif 	/// __MT_SERVICE_LOTTERY_GET_TASK_AWARD_H

