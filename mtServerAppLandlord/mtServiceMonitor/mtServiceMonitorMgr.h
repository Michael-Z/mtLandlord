#ifndef		__MT_SERVICE_MONITOR_MGR_H
#define		__MT_SERVICE_MONITOR_MGR_H
#include "mtPlatform.h"
#include "mtQueueHall.h"
#include "mtQueueWork.h"

class mtServiceMonitor;
class mtServiceMonitorMgr
{
public:
	enum EServiceId
	{
		E_SERVICE_MONITOR_BEG	              = 0,
		E_SERVICE_MONITOR_UPDATE_ROOM         = E_SERVICE_MONITOR_BEG,  /// ������Ϣ��ʱ����
		E_SERVICE_MONITOR_UPDATE_SYS_NOTICE,                            /// ���乫�涨ʱ����
		E_SERVICE_MONITOR_UPDATE_GOLD_BUY,                              /// �û���ֵ���շѷ�����֪ͨ����Ϸ������
		E_SERVICE_MONITOR_END
	};

	struct DataHead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
	};

	struct DataInit
	{
		long		             lStructBytes;
		mtQueueHall::DataHall*   pkDataHall;                    /// ����Ϣ
		///mtQueueHall::DataSpace*  pkSpace;   	               /// ����Ϣ
	};

	struct DataRun
	{
		long		     lStructBytes;
		mtQueueWorkNode* pQueueWorkNode;
	};

public:
	mtServiceMonitorMgr();
	virtual ~mtServiceMonitorMgr();

	int			init(DataInit* pkDataInit);
	virtual int	run(void* pData);
	virtual int exit();

	int	runReadDataHead(SOCKET socket, DataHead* pkDataHead);
private:
	int			initService();
	int			initServiceMonitorUpdateRoom();
	int			initServiceMonitorUpdateSysNotice();
	int			initServiceMonitorUpdateGoldBuy();

	mtQueueHall::DataHall*       m_pkDataHall;   	                       /// ����Ϣ
	mtServiceMonitor*		     m_pkServiceList[E_SERVICE_MONITOR_END];

};

#endif	///	__MT_SERVICE_MONITOR_MGR_H
