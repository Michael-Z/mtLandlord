#ifndef 	__MT_SERVICE_HALL_PROP_BUY_H
#define 	__MT_SERVICE_HALL_PROP_BUY_H
#include "mtService.h"
#include "mtQueueHall.h"
/// ������߷���
class 	mtServiceHallPropBuy : public mtService
{
public:
	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lUserId;                /// �û�id
		long                            lPropIdx;               /// ��������
		long							lPropPrice;				/// ���ߵļ۸�(�ɰ汾�����Ľ��)���°汾ΪԪ��
		long 							lPropBuyCount;	        /// �����������
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lResult;                /// ������߽��
		long                            lusetime;               ///ʣ��ʹ��Сʱ
		long                            lPropIdx;               /// ��������                           
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
	mtServiceHallPropBuy();
	virtual ~mtServiceHallPropBuy();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);
	void*   getUserNodeAddr(long lUserId);
	void   initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);
	bool  IsVip(long lUserId);

	mtQueuePacket*                   m_pkQueuePacket;
	mtQueueHall*                     m_pkQueueHall;   		/// ������Ϣ
	mtSQLEnv*		m_pkSQLEnv;
};

#endif 	/// __MT_SERVICE_HALL_PROP_BUY_H

