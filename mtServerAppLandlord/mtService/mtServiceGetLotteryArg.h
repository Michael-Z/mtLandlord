#ifndef 	__MT_SERVICE_LOTTERY_GETLOTTERYARG_H
#define 	__MT_SERVICE_LOTTERY_GETLOTTERYARG_H
#include "mtService.h"
#include "mtQueueHall.h"
/// �齱����Э��
class 	mtServiceGetLotteryArg : public mtService
{
public:
	struct DataRead
	{
		long		lStructBytes;			// ����С
		long        lServiceType;			// ��������
		long		plReservation[2];		// �����ֶ�
		long		lUserId;                // �û�id
	};

	struct DataWrite
	{
		long		lStructBytes;		// ����С
		long		lServiceType;		// ��������
		long		plReservation[2];	// �����ֶ�
		long        lResult;			// 0��ʧ�ܣ����������ô��� 1���ɹ�
		long		lNeedGold;	        // �齱���
		long		lUseTimes;			// ���ô���
		long		lMaxTimes;			// ���齱����
		long		lLuck;				// ����ֵ
		long        lFeeCard;           // ����
	};
	struct DataInit
	{
		long							lStructBytes;
		char*                           pcServiceExeDir;
		mtQueueHall*                    pkQueueHall;   		// ������Ϣ
		mtQueuePacket*                  pkQueuePacket;
		mtSQLEnv*						pkSQLEnv;
	};
public:
	mtServiceGetLotteryArg();
	virtual ~mtServiceGetLotteryArg();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);

	void * getUserNodeAddr(long lUserId);
	void   initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);
	void   calcSurplusLotteryFrequency(mtQueueHall::LotteryInfo * pkLotteryInfo,long lMaxTimes,long lVipTimes,bool bVip);
	bool   IsVip(long lUserId);

	mtQueuePacket *                   m_pkQueuePacket;
	mtQueueHall *                     m_pkQueueHall;   		/// ������Ϣ
	mtSQLEnv *		                  m_pkSQLEnv;
};

#endif 	/// __MT_SERVICE_LOTTERY_GETLOTTERYARG_H

