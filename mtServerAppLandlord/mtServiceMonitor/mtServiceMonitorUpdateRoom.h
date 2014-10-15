#ifndef		__MT_SERVICE_HALL_UPDATE_ROOM_H
#define		__MT_SERVICE_HALL_UPDATE_ROOM_H
#include "mtQueueWork.h"
#include "mtServiceMonitor.h"
#include "mtQueueHall.h"

class mtServiceMonitorUpdateRoom : public mtServiceMonitor
{

public:
	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            plSpaceRoom[2][6];      /// ��������������Ϣ
	};

	struct DataInit
	{
		long							lStructBytes;
		mtQueueHall::DataSpace*         pkSpace;   			         /// ����Ϣ
	};

public:
	mtServiceMonitorUpdateRoom();
	virtual ~mtServiceMonitorUpdateRoom();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);

	void	OutPutPersonNumber();

public:
	mtQueueHall::DataSpace*      m_pkSpace;   	    /// ����Ϣ

};

#endif	///	__MT_SERVICE_HALL_UPDATE_ROOM_H