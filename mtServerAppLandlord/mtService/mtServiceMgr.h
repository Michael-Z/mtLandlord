#ifndef		__MT_SERVICE_MGR_H
#define		__MT_SERVICE_MGR_H

#include "mtService.h"
#include "mtPlatform.h"
#include "mtQueueWork.h"
#include "mtSQLEnv.h"
#include "mtServiceConnect.h"

class mtServiceMgr
{
public:
	enum EServiceId
	{
		E_SERVICE_HALL_BEG	               = 0,
		E_SERVICE_HALL_CONNECT             = E_SERVICE_HALL_BEG,
		E_SERVICE_HALL_REGISTER ,
		E_SERVICE_HALL_PROP_BUY,                                      /// 2�������
		E_SERVICE_HALL_PROP_USE,                                      /// 3ʹ�õ���
		E_SERVICE_HALL_GOLD_BUY,                                      /// 4������
		E_SERVICE_HALL_GOLD_BUY_STATUS,                               /// 5������״̬����
		E_SERVICE_HALL_RANKING,                                       /// 6������а�
		E_SERVICE_HALL_UPDATE_PALY_INFO,                              /// 7�����û����˻���Ϣ(�������а�)
		E_SERVICE_HALL_UPDATE_SOCIAL_INFO,                            /// 8�����û��Ļ���������Ϣ
		E_SERVICE_HALL_GIVE_GOLD,                                     /// 9����
		E_SERVICE_HALL_UPDATE_ROOM,                                   /// 10������Ϣ��ʱ����
		E_SERVICE_HALL_HALL_2_ROOM,                                   /// 11�û�����Ӵ���������
		E_SERVICE_HALL_ROOM_2_HALL,                                   /// 12�û�����ӷ��䷵�ش���
		E_SERVICE_HALL_FEED_BACK,                                     /// 13�û���Ϣ����
		E_SERVICE_HALL_EXIT,								          /// 14�˳�����
		E_SERVICE_HALL_GETLOTTERYARG,								  /// 15ȡ�齱����
		E_SERVICE_HALL_LOTTERY,										  /// 16�齱
		E_SERVICE_HALL_SYSTEMMESSAGE,								  /// 17ϵͳ��Ϣ
		//����Э��
		E_SERVICE_HALL_GET_TASK_INFO,                                  /// 18��ȡ������Ϣ
		E_SERVICE_HALL_GET_TASK_AWARD,                                 /// 19��ȡ������
		E_SERVICE_HALL_GET_PET_INFO,                                   /// 20��ȡ����Э��
		E_SERVICE_HALL_FEED_PET,                                       /// 21ι������
		E_SERVICE_HALL_SPIT_GOLD,                                      /// 22�½��
		E_SERVICE_HALL_CASH_LOTTERY,                                   /// 23�ҽ�
		//����Э��(������14-8-18)
		E_SERVICE_MATCH_HALL_GET_ROOM_INFO,                            /// 24��ȡ����������ϢЭ��
		E_SERVICE_MATCH_HALL_GET_USER_INFO,                            /// 25��ȡ�û�����������Э��
		E_SERVICE_MATCH_HALL_SIGN_UP,                                  /// 26��������Э��.
		E_SERVICE_MATCH_HALL_GET_ROOM_STATUS,                          /// 27��ȡ����״̬Э��
		E_SERVICE_MATCH_HALL_GET_RANK,                                 /// 28����������ȡЭ��
		E_SERVICE_MATCH_HALL_AWARD,                                    /// 29�����콱Э��

		E_SERVICE_HALL_END
	};

	struct DataHead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
	};

	struct DataInit
	{
		long		           lStructBytes;
		long                   lSpinCount;
		mtQueueMgr*            pkQueueMgr;
		char*                  pcServiceExeDir;
		mtSQLEnv*		       pkSQLEnv;
	};

	struct DataRun
	{
		long		     lStructBytes;
		mtQueueWorkNode* pQueueWorkNode;
	};

public:
	mtServiceMgr();
	virtual ~mtServiceMgr();

	int			init(DataInit* pkDataInit);
	virtual int	run(void* pData);
	virtual int exit();

	int	runReadDataHead(SOCKET socket, DataHead* pkDataHead);
private:
	int			initService();
	int			initServiceLoginConnect();
	int			initServiceHallRegister();
	int			initServiceHallPropBuy();
	int			initServiceHallGoldBuy();
	int			initServiceHallGoldBuyStatus();
	int			initServiceHallRank();
	int			initServiceHallUpdatePlayInfo();
	int			initServiceHallUpdateSocialInfo();
	int			initServiceHallGiveGold();
	int			initServiceHallUpdateRoom();
	int			initServiceHallHall2Room();
	int			initServiceHallRoom2Hall();
	int			initServiceHallFeedBack();
	int			initServiceHallExit();
	int         initServiceGetLottertArg();
	int         initServiceLottery();
	int         initServiceSystemMessage();

	int         initServiceGetTaskInfo();
	int         initServiceGetTaskAward();
	int         initServiceGetPetInfo();
	int         initServiceFeedPet();
	int         initServiceSpitGold();
	int         iniServiceCashLottery();

	int         initServiceMatchGetRoomInfo();
	int         initServiceMatchGetUserInfo();
	int         initServiceMatchSignUp();
	int         initServiceMatchGetRoomStatus();
	int         initServiceMatchGetRank();
	int         initServiceMatchAward();

	long                         m_lSpinCount;
	mtQueueMgr*                  m_pkQueueMgr;
	char*                        m_pcServiceExeDir;
	mtSQLEnv       *		     m_pkSQLEnv;
	mtService*		             m_pkServiceList[E_SERVICE_HALL_END];

};

#endif	///	__MT_SERVICE_MGR_H