#ifndef 	__MT_SERVICE_LOTTERY_H
#define 	__MT_SERVICE_LOTTERY_H
#include "mtService.h"
#include "mtQueueHall.h"
/// �齱����Э��
class 	mtServiceLottery : public mtService
{
public:
	enum EPrizeId
	{
		E_PRIZE_FIVE_HUNDRED_GOLD = 1,      //500���
		E_PRIZE_ONE_THOUSAND_GOLD   ,      //1000���
		E_PRIZE_TWO_THOUSAND_GOLD ,        //2000���
		E_PRIZE_FIFTY_THOUSAND_GOLD,       //5����                            
		E_PRIZE_ONE_FEE_CARD,              //1Ԫ����ȯ                            
		E_PRIZE_TWO_FEE_CARD,              //2Ԫ����ȯ                            
		E_PRIZE_THREE_FEE_CARD,            //3Ԫ����ȯ                  
		E_PRIZE_FOUR_FEE_CARD,             //5Ԫ����ȯ                           
		E_PRIZE_TEN_FEE_CARD,              //10Ԫ����ȯ          
        E_PRIZE_THIRTY_FEE_CARD,           //30Ԫ����ȯ        
	};
	enum ELotteryResult
	{
		E_RESULT_SUCCESS          = 1,    //�ɹ�
		E_RESULT_FAILURE          = 0,   //ʧ��
		E_RESULT_ACCOUNT_ERROR    = -1,  //�ʺŴ���
		E_RESULT_OFFLINE          = -2,  //�û�������
		E_RESULT_GOLD_INADEQUATE  = -3,  // ��Ҳ��� 
		E_RESULT_NUMBER_FULL      = -4,  //��������
	};
	struct DataRead
	{
		long		lStructBytes;		// ����С
		long        lServiceType;		// ��������
		long		plReservation[2];	// �����ֶ�
		long		lUserId;            // �û�id
		long		lGold;			    // �û��ܽ��
	};


	struct DataWrite
	{
		long		lStructBytes;		// ����С
		long		lServiceType;		// ��������
		long		plReservation[2];	// �����ֶ�
		long		lResult; 
		long		lPrize;			    // �鵽�Ľ�Ʒ
		long		lGold;			    // Ŀǰ��� 
		long        lLuck;
		long        lFeeCard;           //Ŀǰ����
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
	mtServiceLottery();
	virtual ~mtServiceLottery();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);

	void  * getUserNodeAddr(long lUserId);
	void   initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);

	long Lottery(mtQueueHall::PrizeInfo* pkPrizeInfoList);
	void SaveAwardInfo(long lPrize,mtQueueHall::UserInfo*   pkUserInfo,mtQueueHall::LotteryInfo * pkLotteryInfo);
	void GetMessage(char * message,char *name ,long PrepaidCard);

	mtQueuePacket           *                   m_pkQueuePacket;
	mtQueueHall             *                   m_pkQueueHall;   		/// ������Ϣ
	mtSQLEnv                *		            m_pkSQLEnv;
	
};

#endif 	/// __MT_SERVICE_LOTTERY_H

