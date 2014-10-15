
#ifndef __MT_SERVICE_ROOM_GRAB_LANDLORD_H
#define __MT_SERVICE_ROOM_GRAB_LANDLORD_H
#include "mtService.h"
#include "mtQueueMgr.h"

/// �û��з�����������
class mtServiceMgr;
class mtServiceRoomGrabLandlord : public mtService
{
public:
	struct DataInit
	{
		long				    lStructBytes;
		HANDLE	                hIOCP;
		mtQueueMgr*             pkQueueMgr;
		mtServiceMgr*	        pkServiceMgr;
		HANDLE					hEventManagerAi;
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long							lUserId;				/// ���ID 0,1,2
		long							lScore;	     			/// ��ҽеķ���
		long                            lUserIdLandlord;        /// �����û�id(-1 ��ǰ��û�е���, 0< ��ʶ����û�id)
	};

	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long							lScore;                 /// ��ҽеķ���
	};

public:
	mtServiceRoomGrabLandlord();
	virtual ~mtServiceRoomGrabLandlord();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();

	int getQueueUserNextQueueUser(int iQueueUserCurIdx);
	int getQueueDeskLandlordUserId(mtQueueDesk::DataNode* pkQueueDeskDataNode, mtQueueUser::DataNode* pkQueueUserDataNode, int* piQueueDeskUserIdx);
	/// ai������������Ӧ
	int playGrabLandlordAi(mtQueueDesk::DataNode* pkQueueDeskDataNode, long lServiceType, int iQueueUserCurAiIdx, int iQueueUserCurIdx, ULONGLONG* pullStartTime);
	int playRestartCardDeal(mtQueueUser::DataNode* pkQueueUserDataNode);
	int playDoubleScoreAi(mtQueueUser::DataNode* pkQueueUserDataNode);
	int playAiLandlordCardOut(mtQueueDesk::DataNode* pkQueueDeskDataNode, mtQueueUser::DataNode* pkQueueUserDataNodeAi, ULONGLONG* pullStartTime);
	int playQueueUserSort(mtQueueDesk::DataNode* pkQueueDeskDataNode);
	int getGrabLandlordMaxScore(mtQueueDesk::DataNode* pkQueueDeskDataNode);

	void arraggeCard(char pcCard[], long lNum);
	void insertCard(char pcCard[], char cCard, long lCardSortNum);


	/// ai
	int getPlayOutCardFirst(char pcSelfCard[],char pcOutCard[]);
	int	getAirPlane(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum);
	int	getTripleProgression(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum);
	int	getPairProgression(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum);
	int getSingleProgression(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum);
	int	getTripleWithTwo(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum);
	int	getTripleWithOne(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum);
	int getSingleCard(char* pcSelfCards, char* cOutCard,int iCardPos);
	int getPairCard(char* pcSelfCards,char* pcOutCard ,int iCardBegPos);
	int getTripleCard(char* pcSelfCards,char* pcOutCard,int iCardBegPos);

	HANDLE	                m_hIOCP;
	mtQueueMgr*             m_pkQueueMgr;
	mtServiceMgr*	        m_pkServiceMgr;
	HANDLE	                m_hEventManagerAi;
};


#endif	///__MT_SERVICE_ROOM_GRAB_LANDLORD_H
