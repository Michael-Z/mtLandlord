#ifndef 	__MT_SERVICE_CASH_LOTTERY_H
#define 	__MT_SERVICE_CASH_LOTTERY_H

#include "mtService.h"
#include "mtQueueHall.h"
/// ��ȡ������ϢЭ��
class 	mtServiceCashLottery : public mtService
{
public:
	enum CashLotteryId
	{
		//��ҽ�Ʒ
		Seven_hundred_and_twenty_thousand_gold             = 0,
		One_million_three_hundred_thousand_gold            = 1,
		two_million_six_hundred_thoousand_gold             = 2,
		five_millon_two_hundred_thousand_gold              = 3,
        //ʵ�ｱƷ  
		thirty_rechargeable_card                     = 4,
		fifth_rechargeable_card                            = 5,
		one_hundred_rechargeable_card                      = 6,
		Ipad_mini                                          = 7,
		xiaomi_phone                                       = 8,
		yiqi_commemorate_silver                            = 9,

	};
	struct DataRead
	{
		long		lStructBytes;		// ����С
		long        lServiceType;	    // ��������
		long		plReservation[2];	// �����ֶ�
		long		lUserId;            // �û�id
		long        lPrizeId;           //��ƷID
		char        pcPhone[MT_BYTES_OF_PHONE];          
	};


	struct DataWrite
	{
		long		             lStructBytes;		// ����С
		long		             lServiceType;		// ��������
		long		             plReservation[2];	// �����ֶ�
		long                     lResult;
		long                     lPrizeId;          //��Ʒid
		long                     lFeeCard;           //Ŀǰ����		          
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
	mtServiceCashLottery();
	virtual ~mtServiceCashLottery();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);
	void  * getUserNodeAddr(long lUserId);
	void    initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);
	void    SaveAwardInfo(long lUserId,long lPrizeId,char *pcPhone);

	mtQueuePacket *                   m_pkQueuePacket;
	mtQueueHall *                     m_pkQueueHall;   		/// ������Ϣ
	mtSQLEnv *		                  m_pkSQLEnv;
};

#endif 	/// __MT_SERVICE_CASH_LOTTERY_H

