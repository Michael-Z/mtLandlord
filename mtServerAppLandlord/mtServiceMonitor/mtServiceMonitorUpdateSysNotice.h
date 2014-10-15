#ifndef		__MT_SERVICE_HALL_UPDATE_SYS_NOTICE_H
#define		__MT_SERVICE_HALL_UPDATE_SYS_NOTICE_H
#include "mtPlatform.h"
#include "mtServiceMonitor.h"

class mtServiceMonitorUpdateSysNotice : public mtServiceMonitor
{

public:
	struct DataRead
	{
		long 							lStructBytes;			              /// ����С
		long                        	lServiceType;			              /// ��������
		long 							plReservation[2];		              /// �����ֶ�
		long                            lEffectStyle;                         /// �������Ч��ʽ(0 -�ڶ�����Ч��1 -����)
		char                            pcSystemNotice[MT_SYSTEM_NOTICE_LEN];         
	};

	struct DataInit
	{
		long							lStructBytes;
	};

public:
	mtServiceMonitorUpdateSysNotice();
	virtual ~mtServiceMonitorUpdateSysNotice();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);
};

#endif	///	__MT_SERVICE_HALL_UPDATE_SYS_NOTICE_H