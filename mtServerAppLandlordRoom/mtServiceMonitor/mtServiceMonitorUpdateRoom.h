#ifndef 	__MT_SERVICE_MONITOR_UPDATE_ROOM_H
#define 	__MT_SERVICE_MONITOR_UPDATE_ROOM_H
#include "mtServiceMonitor.h"
#include "mtQueueMgr.h"

/// �����ϱ���Ϣ����������
class 	mtServiceMonitorUpdateRoom : public mtServiceMonitor
{
public:
	struct DataInit
	{
		long				    lStructBytes;
		mtQueueHall*            pkQueueHall;
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            plSpaceRoom[2][6];      /// ��������������Ϣ
	};
public:
	mtServiceMonitorUpdateRoom();
	virtual ~mtServiceMonitorUpdateRoom();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();

	int	runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);

    mtQueueHall*             m_pkQueueHall;
};

#endif 	/// __MT_SERVICE_MONITOR_UPDATE_ROOM_H

