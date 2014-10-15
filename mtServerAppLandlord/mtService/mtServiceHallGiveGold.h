#ifndef 	__MT_SERVICE_HALL_GIVE_GOLD_H
#define 	__MT_SERVICE_HALL_GIVE_GOLD_H
#include "mtService.h"
#include "mtQueueHall.h"
/// ÿ�������ͽ�ҷ���(һ�����ͼ��Σ�����ϵ���¼�������������Ͳ�ͬ)
class 	mtServiceHallGiveGold : public mtService
{
public:
	struct DataRead
	{
		long 							lStructBytes;			    /// ����С
		long                        	lServiceType;			    /// ��������
		long 							plReservation[2];		    /// �����ֶ�
		long                            lUserId;                    /// �û�id
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long							lGiveGoldFrequence;     /// ���ͷִ���(>= 1�ɹ���0 -ʧ�ܣ�-1 -�û��˺Ŵ���-2 -�û�������)
	};

	struct DataInit
	{
		long							lStructBytes;
		mtQueueHall*                    pkQueueHall;   		/// ������Ϣ
		mtQueuePacket*                  pkQueuePacket;
		mtSQLEnv*						pkSQLEnv;
	};

public:
	mtServiceHallGiveGold();
	virtual ~mtServiceHallGiveGold();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);

	void*   getUserNodeAddr(long lUserId);
	void   initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);

	mtQueuePacket*                   m_pkQueuePacket;
	mtQueueHall*                     m_pkQueueHall;   		/// ������Ϣ
	mtSQLEnv*		                 m_pkSQLEnv;
};

#endif 	/// __MT_SERVICE_HALL_GIVE_GOLD_H

