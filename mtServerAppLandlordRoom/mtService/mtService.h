#ifndef		__MT_SERVICE_H
#define		__MT_SERVICE_H
#include "mtQueueUser.h"
#include "mtQueuePacket.h"

class mtService
{
public:
	struct DataUserInfo
	{
		long 							lUserId;				/// �û�Id
		long                            lUserIconId;            /// �û�ͷ��id
		long                            lUserSex;               /// �Ա� (0--�У�1--Ů)
		long                            lUserGold;              /// �û�ӵ�н����
		long                            lUserLevel;             /// �û��ȼ�
		long 							lUserScore;				/// �û�����
		long                            lUserAllChess;          /// �ܾ���
		long                            lUserWinChess;          /// ʤ����
		long                            lUserWinRate;           /// ʤ��
		long                            lUserOffLineCount;      /// ���ߴ���
		long 							lIsWaitStart;			/// ����Ƿ�ʼ
		long							lPlayerType;	        /// ÿ���˵�����(��ʼֵ��1�� 0 - landlord, 1 - farmer)
		unsigned long					ulDeskId;               /// ���ӱ�ʶ
		long                            plPropsCount[16];       /// �û�������Ŀ
		char                            pcUserAccountNum[12];   /// �û��˺�
		char                            pcUserName[12];         /// �û�����
		char                            pcUserNickName[12];     /// �û��ǳ�
	};

	struct DataRun
	{
		long						lStructBytes;
		mtQueueUser::DataNode*		pkQueueUserDataNode;
		mtQueuePacket::DataNode*	pkQueuePacketDataNode;
	};

	mtService();
	virtual ~mtService();

	virtual int init(void* pData) = 0;
	virtual int	run(DataRun* pkDataRun) = 0;
	virtual int exit() = 0;

};

#endif	///	__MT_SERVICE_H