#ifndef 	__MT_SERVICE_HALL_RANK_H
#define 	__MT_SERVICE_HALL_RANK_H
#include "mtService.h"
#include "mtQueueHall.h"
/// ���а����
class 	mtServiceHallRank : public mtService
{
public:
	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		mtQueueHall::DataRank           kRank[10];              /// �û�������Ϣ
	};

	struct DataInit
	{
		long							lStructBytes;
		mtQueueHall*                    pkQueueHall;   		/// ������Ϣ
		mtQueuePacket*                  pkQueuePacket;
	};

public:
	mtServiceHallRank();
	virtual ~mtServiceHallRank();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);

	mtQueuePacket*                   m_pkQueuePacket;
	mtQueueHall*                     m_pkQueueHall;   		/// ������Ϣ
};

#endif 	/// __MT_SERVICE_HALL_RANK_H

