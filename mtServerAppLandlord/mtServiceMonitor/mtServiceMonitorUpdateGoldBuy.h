#ifndef		__MT_SERVICE_HALL_UPDATE_GOLD_BUY_H
#define		__MT_SERVICE_HALL_UPDATE_GOLD_BUY_H
#include "mtQueueWork.h"
#include "mtServiceMonitor.h"
#include "mtQueueHall.h"

class mtServiceMonitorUpdateGoldBuy : public mtServiceMonitor
{

public:
	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lResult;                /// �û��û������ҽ��(0 -ʧ�ܣ�1 -�ɹ�)
		char                            pcKey[24];              /// keyֵ
		long                            lUserId;                /// �û�id
		long                            lGoldBuy;               /// ����Ľ����(>0,����ֵ���Ǵ���)
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lResult;                /// ֪ͨ�����ҽ��(0 -����1 -�ɹ��� -1 -�û�id���� -2, -������������)
		char                            pcKey[24];              /// keyֵ
		long                            lUserId;                /// �û�id
		long                            lGoldBuy;               /// ����Ľ����(>0,����ֵ���Ǵ���)
		
	};

	struct DataInit
	{
		long							lStructBytes;
		mtQueueHall::DataHall*          pkDataHall;   	        /// ����Ϣ
		//mtQueueHall::DataSpace*         pkSpace;   			    /// ����Ϣ
	};

public:
	mtServiceMonitorUpdateGoldBuy();
	virtual ~mtServiceMonitorUpdateGoldBuy();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);

	void* getUserNodeAddr(long lUserId);
public:
	mtQueueHall::DataHall*       m_pkDataHall;   	            /// ����Ϣ

};

#endif	///	__MT_SERVICE_HALL_UPDATE_GOLD_BUY_H