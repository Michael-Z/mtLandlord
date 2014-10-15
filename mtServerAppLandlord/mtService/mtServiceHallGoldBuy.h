#ifndef 	__MT_SERVICE_HALL_GOLD_BUY_H
#define 	__MT_SERVICE_HALL_GOLD_BUY_H
#include "mtService.h"
#include "mtQueueHall.h"
/// wap�շѣ������ҷ���
class 	mtServiceHallGoldBuy : public mtService
{
public:
	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lUserId;                /// �û�id
		long							lPoint;					/// 
		long 							lMoney;	                /// ֧������
		long 							lGold;	                /// ��ҹ���
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lResult;                /// ������߽��(>= 1�ɹ���0 -ʧ�ܣ�-1 -�û��˺Ŵ���-2 -�û�������)
		long 							lGold;	                /// ��ҹ���
		long                            lMash;                  ///���͵�����
	};

	struct DataInit
	{
		long							lStructBytes;
		char*                           pcServiceExeDir;
		mtQueueHall*                    pkQueueHall;   		/// ������Ϣ
		mtQueuePacket*                  pkQueuePacket;
		mtSQLEnv*						pkSQLEnv;
	};
public:
	mtServiceHallGoldBuy();
	virtual ~mtServiceHallGoldBuy();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);

	void*   getUserNodeAddr(long lUserId);
	void   initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);
	long    givingMash(long  lUserId,long lMoney);

	mtQueuePacket*                   m_pkQueuePacket;
	mtQueueHall*                     m_pkQueueHall;   		/// ������Ϣ
	mtSQLEnv*		                 m_pkSQLEnv;
};

#endif 	/// __MT_SERVICE_HALL_GOLD_BUY_H

