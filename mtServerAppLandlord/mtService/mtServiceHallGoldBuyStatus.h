#ifndef 	__MT_SERVICE_HALL_GOLD_BUY_STATUS_H
#define 	__MT_SERVICE_HALL_GOLD_BUY_STATUS_H
#include "mtService.h"
#include "mtQueueHall.h"
/// �µ���Ϸ�����շѽṹ��ѯ����
class 	mtServiceHallGoldBuyStatus : public mtService
{
public:
	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lUserId;                /// �û�id
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lResult;                /// ��ѯ��ֵ״̬���(0 -ʧ�ܣ�1 -��ֵ״̬�Ѹ��£�-1 -�û��˺Ŵ���-2 -��ֵ״̬δ���£�-3 -�û����ߣ�-4 -�û���ֵʧ��)
		long                            lUserId;                /// �û�id
		long 							lGold;	                /// ��ֵ�Ľ��(ֻ����lResult����1ʱ����Ч)
	};

	struct DataInit
	{
		long							lStructBytes;
		char*                           pcServiceExeDir;
		mtQueueHall*                    pkQueueHall;   		/// ������Ϣ
		mtQueuePacket*                  pkQueuePacket;
	};
public:
	mtServiceHallGoldBuyStatus();
	virtual ~mtServiceHallGoldBuyStatus();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);
	void   initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);
	void*   getUserNodeAddr(long lUserId);

	mtQueuePacket*                   m_pkQueuePacket;
	mtQueueHall*                     m_pkQueueHall;   		/// ������Ϣ
};

#endif 	/// __MT_SERVICE_HALL_GOLD_BUY_STATUS_H

