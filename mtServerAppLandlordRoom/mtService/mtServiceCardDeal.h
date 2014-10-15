#ifndef 	__MT_SERVICE_CARD_DEAL_H
#define 	__MT_SERVICE_CARD_DEAL_H
#include "mtService.h"
#include "mtQueueMgr.h"

/// ϴ�� ���Ʒ���
class mtServiceMgr;
class 	mtServiceCardDeal : public mtService
{
public:
	struct DataInit
	{
		long				    lStructBytes;
		mtQueueMgr*             pkQueueMgr;
		mtQueueUser*	        pkQueueUser;
		mtQueuePacket*	        pkQueuePacket;
		mtServiceMgr*	        pkServiceMgr;
		HANDLE					hEventManagerAi;
	};

	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long							lResult;
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long 							lDefaultLandlordUserId;	/// Ĭ�Ͽ�ʼ�зֵ����
		char               				pcCards[17];            /// ÿ����17����
		char                			pcBaseCards[7];         /// ����3��
	};
public:
	mtServiceCardDeal();
	virtual ~mtServiceCardDeal();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();

	int shuffleCard(char* pcCards);
	int shuffleCardEveryOne(char* pcCardsDest);
	/// ai���շ��Ʒ�����Ӧ
	int playDealCardAi(mtQueueDesk::DataNode* pkQueueDeskDataNode, int iQueueUserAiIdx);
	int playAiGrabLandlord(mtQueueUser::DataNode* pkQueueUserDataNodeAi, int iLandlordScore);
	int getQueueUserNextQueueUser(int iQueueUserCurIdx);
	int shuffleCardEx(char *pcCards,long *PlayerMode);
	int shuffleCardEx(char *pcCards);

	mtQueueMgr*     m_pkQueueMgr;
	mtQueueUser*	m_pkQueueUser;
	mtQueuePacket*	m_pkQueuePacket;
	mtServiceMgr*	m_pkServiceMgr;
	HANDLE	        m_hEventManagerAi;
};

#endif 	/// __MT_SERVICE_CARD_DEAL_H

