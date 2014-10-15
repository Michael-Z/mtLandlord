#ifndef		__MT_SERVICE_CONNECT_H
#define		__MT_SERVICE_CONNECT_H
#include "mtService.h"
#include "mtQueueMgr.h"

/// ���ӷ������
class mtServiceConnect : public mtService
{
public:
	struct DataInit
	{
		long				    lStructBytes;
		mtQueueMgr*             pkQueueMgr;
		mtQueueUser*			pkQueueUser;
		mtQueueHall*			pkQueueHall;
		mtQueuePacket*	        pkQueuePacket;
	};

	struct DataRead
	{
		long 					lStructBytes;			               /// ����С
		long                    lServiceType;			               /// ��������
		long 				    plReservation[2];		               /// �����ֶ�
		long 					lSpaceId;				               /// ��Id
		long 					lRoomId;				               /// ����Id
		long                    lRequestSameRoom;                      /// �Ƿ��������ͬһ����(�û���Ҫ����ķ����Ƿ�ʹ���ĳ�id����id��ͬ,0 -��ͬ��1 -��ͬ)
		DataUserInfo			kUserInfo;				               /// ��Ҹ�����Ϣ
	};

	struct DataWrite
	{
		long 					lStructBytes;			               /// ����С
		long                    lServiceType;			               /// ��������
		long 					plReservation[2];		               /// �����ֶ�
		long 				    lResult;                               /// ���ӷ�����������(0 -ʧ�ܣ�1 -�ɹ�)
		                                                                   /// 2 -���û���һ��ǿ�ƴ�ĳ�����˳�(��Ϸ���ڽ�����)
		                                                                   /// 3 -���û���һ��ǿ�ƴ�ĳ�����˳�(��Ϸ�Ѿ�����)
		                                                                   /// 4 -���û����������쳣��ĳ�����˳�(��Ϸ���ڽ�����)
		                                                                   /// 5 -���û����������쳣��ĳ�����˳�(��Ϸ�Ѿ�����)
		                                                                   /// 6 -���û��˺��Ѿ��������ط���½��
		                                                                   /// 7 -���û��˺��Ѿ����ڸ÷�����
		DataUserInfo            kUserInfo[2];                          /// ���������Ϣ(��lResultΪ3��5ʱ����һ��λ�ñ�ʾ�û����µ���Ϣ����������һ��)
		/// �����ֶ�ֻ����lResultΪ2��4ʱ�Ż���Ч
		char					pcCardsBase[MT_NODE_PLAYER];           /// ÿһ�ֵĵ�����Ϣ
		char					pcCardsTotal[MT_NODE_PLAYER][24];      /// ���û��ĵ�ǰ������Ϣ
		char					pcCardsShow[24];                       /// �����û��ĵ�ǰ������Ϣ
		char					pcCardsCur[24];                        /// �����ϵ�ǰ���������
		char					pcCardsOutTotal[MT_NODE_PLAYER][24];   /// ÿ���˵ĵ�ǰ�Ѿ�������Ϣ
		long                    plPlayerLots[MT_NODE_PLAYER];          /// ÿ����ҵĳ��Ƶ�����
		long				    plScore[MT_NODE_PLAYER];	           /// ��ҽеķ���(��ʼֵ��-1, 0 - ���з�, 0< - �еķ���  )
		long					plDoubleScore[MT_NODE_PLAYER];         /// ��Ҽӱ�����(��ʼֵ��-1, 0 - ���ӱ�, 1 - �ӱ�  )
		long                    plShowCard[MT_NODE_PLAYER];            /// �Ƿ�����(1 -���ƣ�0 -������)
		long					plBomb[MT_NODE_PLAYER];                /// ��ҳ���ը����(��ʼֵ��0, 0 < -)
		long				    lRoundOver;	                           /// ����Ƿ��ϴ�һ�ֽ������(��ʼֵ��0, 0 - ��û�ϴ�, 1 - �Ѿ��ϴ���  )
		long					lRocket;                               /// �������Ƿ��Ѿ������˻����(��ʼֵ��0, 0 -��û���������1 -�Ѿ������˻��)
		long					lCardType;                             /// �����ϵ�ǰ�����������
		long                    lUserNodeIdx;                          /// ���û��������ϵ�λ��
		long                    lUserDeskIdxCardOut;                   /// �����ϵ�ǰ����Ƶ��û��������ϵ�λ��(���Ʋ���)
		long                    lUserDeskIdxCardOutNext;               /// ���ӵ�ǰ��Ҫ���Ƶ��û��������ϵ�λ��
		long 					lQueueUserDefaultLandlord;             /// �����ϵ�ǰĬ�Ͽ�ʼ�зֵ��û��������ϵ�λ��(��ʼֵ��-1, 0< -)
	};

public:
	mtServiceConnect();
	virtual ~mtServiceConnect();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();

	int runRecoverScene(mtQueueUser::DataNode* pkQueueUserDataNode, mtQueueUser::DataNode* pkQueueUserDataNodeReplaceAi, DataWrite* pkDataWrite);
	int getUserInfo(mtQueueDesk::DataNode* pkQueueDeskDataNode, int iUserNodeIdx, DataUserInfo* pkUserInfo);
	int getQueueUserNextQueueUser(int iQueueUserCurIdx);
	int getQueueUserPreQueueUser(int iQueueUserCurIdx);
	int getQueueUserLandlord(mtQueueDesk::DataNode* pkQueueDeskDataNode);
	int getQueueUserLeftCards(char* pcCard);

	mtQueueMgr*             m_pkQueueMgr;
	mtQueueUser*			m_pkQueueUser;
	mtQueueHall*			m_pkQueueHall;
	mtQueuePacket*	        m_pkQueuePacket;
};

#endif ///	__MT_SERVICE_CONNECT_H