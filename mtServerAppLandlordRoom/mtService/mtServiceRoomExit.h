#ifndef		__MT_SERVICE_ROOM_EXIT_H
#define		__MT_SERVICE_ROOM_EXIT_H
#include "mtService.h"
#include "mtQueueHall.h"
#include "mtQueueUser.h"
#include "mtQueueMgr.h"

class mtServiceMgr;
/// �˳��������
class mtServiceRoomExit : public mtService
{
public:
	enum EExitStatus
	{
		E_STATUS_EXIT_FAIL,
		E_STATUS_EXIT_NORMAL = 2,
		E_STATUS_EXIT_FORCE = 4,
		E_STATUS_EXIT_NETWORK_ANOMALY
	};
	struct DataInit
	{
		long				    lStructBytes;
		mtQueueMgr*             pkQueueMgr;
		mtServiceMgr*	        pkServiceMgr;
	};

	struct DataRead
	{
		long 							lStructBytes;	/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lStatusExit;            /// ��ʵ�û��˳�����ʱ��״̬(1 -�����˳���-1 -���ҽз֣�-2 -���Ҽӱ���
		                                                        /// -3 -�������ƣ�-4 -���ҳ��ƣ����������ֶα�ʾ������Ϣ)
		long							lCardType;              /// ��������
		char 							pcCard[24];             /// ��������
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lUserId;                /// �û�id
		long                            lStatusExit;            /// �û��ӷ����˵�����ʱ��״̬
		                                                        /// 0 -���û��˳�����ʧ��
		                                                        /// 2 -���û������˳�(һ�ֽ������߻�δ��ʼ��Ϸʱִ���˳��������)
		                                                        /// 4 -���û�ǿ�ƴ�ĳ�����˳�(��Ϸ�Ѿ���ʼ�һ�û����)
		                                                        /// 5 -���û����������쳣��ĳ�����˳�(��Ϸ�Ѿ���ʼ�һ�û����)
	};

public:
	mtServiceRoomExit();
	virtual ~mtServiceRoomExit();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();
	int getQueueUserLeftCards(char* pcCard);

	int runRoomExitQueueDeskTwo(mtQueueDesk::DataNode* pkQueueDeskDataNode, mtQueueUser::DataNode* pkQueueUserDataNode
		                        , mtQueueHall::DataRoom* pkQueueHallDataRoom);
// 	int runRoomExitQueueDeskThree(mtQueueDesk::DataNode* pkQueueDeskDataNode, mtQueueUser::DataNode* pkQueueUserDataNode
// 		                        , mtQueueHall::DataRoom* pkQueueHallDataRoom, DataRead* pkDataRead);

	int runRoomExitQueueDeskThree(mtQueueDesk::DataNode* pkQueueDeskDataNode, mtQueueUser::DataNode* pkQueueUserDataNode
		, mtQueueHall::DataRoom* pkQueueHallDataRoom, DataRead* pkDataRead
		, mtQueueUser::DataNode** pkQueueUserNodeReplaceAi, int* iSeriveType, int* iCardsOutStyle);

	int playReplaceAiGrabLandlord(mtQueueUser::DataNode* pkQueueUserDataNodeAi);
	int playReplaceAiDoubleScore(mtQueueUser::DataNode* pkQueueUserDataNode);
	int playReplaceAiRoundOver(mtQueueDesk::DataNode* pkQueueDeskDataNode, mtQueueUser::DataNode* pkQueueUserDataNodeAi);
	int playReplaceAiShowCard(mtQueueUser::DataNode* pkQueueUserDataNodeAi);
	int playReplaceAiCardOut(mtQueueUser::DataNode* pkQueueUserDataNodeAi, DataRead* pkDataRead);
	int getQueueUserNextQueueUser(int iQueueUserCurIdx);
	int getQueueUserPreQueueUser(int iQueueUserCurIdx);
	int initQueueUserDataNodeAiReplace(mtQueueDesk::DataNode* pkQueueDeskDataNode, mtQueueUser::DataNode* pkQueueUserDataNode
		                               , int iQueueUserDeskIdx, mtQueueUser::DataNode* pkQueueUserDataNodeAi);
	int calcRoundOver(mtQueueDesk::DataNode* pkQueueDeskDataNode, mtQueueUser::DataNode* pkQueueUserDataNode, long* plGold);

	/// ai
	int getPlayOutCardFirst(mtQueueDesk::DataNode* pkQueueDeskDataNode, int iQueueUserIdx, DataRead* pkDataRead);
	int getPlayOutCardFirst(char pcSelfCard[],char pcOutCard[]);
	int getPlayOutCard(mtQueueDesk::DataNode* pkQueueDeskDataNode, int iQueueUserIdx, DataRead* pkDataRead);
	int getPlayOutCard(int iCardType,char pcRecvCard[],char pcSelfCard[],char pcOutCard[]);

	int IsSeriesCard(char pcSelfCard[],char pcOutCard[]);
	int getCardsNum(char pcCard[]);
	int playOutCardsNotByType(char pcSelfCard[],char pcOutCard[] ,int iCardType, int iSelfCardsNum);
	int getSingleCard(char* pcSelfCards, char* cOutCard,int iCardPos);
	int getPairCard(char* pcSelfCards,char* pcOutCard ,int iCardBegPos);
	int getTripleCard(char* pcSelfCards,char* pcOutCard,int iCardBegPos);
	int	getTripleWithOne(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum);
	int	getAirPlane(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum);
	int	getTripleProgression(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum);
	int	getPairProgression(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum);
	int getSingleProgression(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum);
	int	getTripleWithTwo(char* pcSelfCards,char* pcOutCards,int iCardBegPos,int iCardsNum);
	int playOutMaxCardsByType(char pcSelfCard[],char pcOutCard[] ,int iCardType, int iSelfCardsNum);
	int getMainCardFromRecv(int iCardType,char* pcCardsRecv,int iCardsRecvNum);
	int getPlayOutCardType(char pcOutCard[], long* plCardType);
	int getBombCard(char* pcSelfCards,char* pcOutCard,int iCardBegPos);
	int getRocketCard(char* pcSelfCards,char* pcOutCard);
	int judgeTripleWithOne(char* pcCards,int iCardsNum);
	int judgeTripleWithTwo(char* pcCards,int iCardsNum);
	int judgeAirPlane(char* pcCards,int iCardsNum);
	int judgeFourWithTwo(char* pcCards,int iCardsNum);
	int judgeTripleProgression(char* pcCards,int iCardsNum);

	mtQueueMgr*             m_pkQueueMgr;
	mtServiceMgr*	        m_pkServiceMgr;
};

#endif ///	__MT_SERVICE_ROOM_EXIT_H