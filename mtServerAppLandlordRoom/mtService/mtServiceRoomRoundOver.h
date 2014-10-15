#ifndef		__MT_SERVICE_ROOM_ROUND_OVER_H
#define		__MT_SERVICE_ROOM_ROUND_OVER_H
#include "mtService.h"
#include "mtQueueHall.h"
#include "mtQueueUser.h"
#include "mtQueueMgr.h"

class mtServiceMgr;

/// һ����Ϸ��������(�����ϱ�����)
class mtServiceRoomRoundOver : public mtService
{
public:
	struct DataInit
	{
		long				    lStructBytes;
		mtQueueMgr*             pkQueueMgr;
		mtServiceMgr*	        pkServiceMgr;
	};

	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lGold;                  /// �� ��Ӯȡ�Ľ����(��ֵ��ʾ��Ľ��)

	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		DataUserInfo             		kUserInfo;              /// ���һ�ָ��º����Ϣ
	};

public:
	mtServiceRoomRoundOver();
	virtual ~mtServiceRoomRoundOver();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();

	int runWaitStartRecvAi(mtQueueDesk::DataNode* pkQueueDeskDataNode);
	int playWaitStartRecvAi(mtQueueUser::DataNode* pkQueueUserDataNode);
	int getQueueUserNextQueueUser(int iQueueUserCurIdx);
	int playUpdateQueueUserInfoAi(mtQueueDesk::DataNode* pkQueueDeskDataNode, long lServiceType, int iQueueUserIdx, long lGold, int iQueueUserNextAiIdx);
	int playUpdateQueueDesk(mtQueueDesk::DataNode* pkQueueDeskDataNode);

	int playReplaceAiExitRoom(mtQueueDesk::DataNode* pkQueueDeskDataNode);
	int playUpdateQueueUserInfo(mtQueueUser::DataNode* pkQueueUserDataNode, long lGold);
	int calculatePlayerLevel(mtQueueUser::DataNode* pkQueueUserDataNode);
	int resetWaitStartStatus(mtQueueDesk::DataNode* pkQueueDeskDataNode);
	int playRoundOver(mtQueueDesk::DataNode* pkQueueDeskDataNode, long lServiceType);
	int calcDoubleScore(mtQueueUser::DataNode*      pkQueueUserDataNode);

	mtQueueMgr*             m_pkQueueMgr;
	mtServiceMgr*	        m_pkServiceMgr;
};

#endif ///	__MT_SERVICE_ROOM_ROUND_OVER_H