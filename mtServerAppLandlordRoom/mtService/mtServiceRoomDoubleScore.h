#ifndef		__MT_SERVICE_ROOM_DOUBLE_SCORE_H
#define		__MT_SERVICE_ROOM_DOUBLE_SCORE_H
#include "mtService.h"
#include "mtQueueDesk.h"
#include "mtQueueMgr.h"

/// �û��ӱ�����(�ߡ�����������ը���ӱ�)
class mtServiceRoomDoubleScore : public mtService
{
public:
	struct DataInit
	{
		long		            lStructBytes;
		mtQueueMgr*             pkQueueMgr;
	};

	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long							lDoubleFlag;			/// �Ƿ�ӱ���ʼֵΪ-1, 1:�ӱ� 0:���ӱ�
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lUserId;                /// �����û�id
		long							lDoubleFlag;			/// �Ƿ�ӱ� 1:�ӱ� 0:���ӱ�
	};

public:
	mtServiceRoomDoubleScore();
	~mtServiceRoomDoubleScore();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();
	int getQueueUserNextQueueUser(int iQueueUserCurIdx);
	/// ai�ӱ�������Ӧ
	int playDoubleScoreAi(mtQueueDesk::DataNode* pkQueueDeskDataNode, long lServiceType, int iQueueUserIdx);

	mtQueueMgr*             m_pkQueueMgr;
};


#endif ///	__MT_SERVICE_ROOM_DOUBLE_SCORE_H