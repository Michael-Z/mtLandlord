#ifndef 	__MT_SERVICE_HALL_UPDATE_PLAY_INFO_H
#define 	__MT_SERVICE_HALL_UPDATE_PLAY_INFO_H
#include "mtService.h"
#include "mtQueueHall.h"

class 	mtServiceHallUpdatePlayInfo : public mtService
{
public:
	struct DataRead
	{
		long 							lStructBytes;			    /// ����С
		long                        	lServiceType;			    /// ��������
		long 							plReservation[2];		    /// �����ֶ�
		long                            lUserId;                    /// �û�id
		long                            lUserGold;                  /// �û�ӵ�н����
		long 							lUserScore;				    /// �û�����
		long                            lUserLevel;                 /// �û��ȼ�
		long                            lUserAllChess;              /// �ܾ���
		long                            lUserWinChess;              /// ʤ����
		long                            lUserWinRate;               /// ʤ��
		long                            lUserOffLineCount;          /// ���ߴ���
		long                            plPropsCount[16];           /// �û�������Ŀ
		long							plUserChessPlayedToday[6];	/// ��Ϸ����6������Ľ�����Ĵ���
		char                            pcUserNickName[12];         /// �û��ǳ�
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long							lResult;                /// �����û���Ϣ���(1 -�ɹ���0 -ʧ�ܣ� -1 -�û��˺Ŵ��� -2 -�û�������)
	};

	struct DataInit
	{
		long							lStructBytes;
		char*                           pcServiceExeDir;
		mtQueueHall*                    pkQueueHall;   		/// ������Ϣ
		mtQueuePacket*                  pkQueuePacket;
		mtSQLEnv*		pkSQLEnv;
	};

public:
	mtServiceHallUpdatePlayInfo();

	virtual ~mtServiceHallUpdatePlayInfo();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();

	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);

	void*   getUserNodeAddr(long lUserId);
	void   initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);

	int     updateDataRank(mtQueueHall::DataRank* pkDataRank);


	mtQueuePacket*                   m_pkQueuePacket;
	mtQueueHall*                     m_pkQueueHall;   		/// ������Ϣ
	mtSQLEnv*				         m_pkSQLEnv;
};

#endif 	/// __MT_SERVICE_HALL_UPDATE_PLAY_INFO_H

