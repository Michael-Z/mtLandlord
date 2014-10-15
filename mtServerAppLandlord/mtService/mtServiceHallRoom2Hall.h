#ifndef		__MT_SERVICE_HALL_ROOM_2_HALL_H
#define		__MT_SERVICE_HALL_ROOM_2_HALL_H
#include "mtQueueWork.h"
#include "mtService.h"
#include "mtQueueList.h"
#include "mtQueueMgr.h"
/// �ӷ����˳�����������
class mtServiceHallRoom2Hall	: public mtService
{

public:
	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lUserId;                /// �û�id
		long                            lSpaceId;               /// ��id
		long                            lRoomId;                /// ����id
		long                            lStatusExit;            /// �û��ӷ����˵�����ʱ��״̬
		                                                        /// 0 -���û�����״̬
		                                                        /// 2 -���û������˳�(һ�ֽ������߻�δ��ʼ��Ϸʱִ���˳��������)
		                                                        /// 4 -���û�ǿ�ƴ�ĳ�����˳�(��Ϸ�Ѿ���ʼ�һ�û����)
		                                                        /// 5 -���û����������쳣��ĳ�����˳�(��Ϸ�Ѿ���ʼ�һ�û����
		long                            lUserGold;              /// �û�ӵ�н����
		long 				            lUserScore;		        /// �û�����
		long                            lUserLevel;             /// �û��ȼ�
		long				            lUserDayChess;		    /// ���վ��� 
		long                            lUserAllChess;          /// �ܾ���
		long                            lUserWinChess;          /// ʤ����
		long                            lUserWinRate;           /// ʤ��
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lResult;                /// �û��˳��������(0 -ʧ�ܣ� 1 -�ɹ���-1 -�˺Ŵ���)
	};

	struct DataInit
	{
		long							lStructBytes;
		mtQueueHall*                    pkQueueHall;   		    /// ������Ϣ
		mtSQLEnv*		                pkSQLEnv;
	};

public:
	mtServiceHallRoom2Hall();
	virtual ~mtServiceHallRoom2Hall();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);

	void   initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);
	void*   getUserNodeAddr(long lUserId);
	void    UpdateUserInfo(DataRead  * pkDataRead);
public:
	mtQueueHall*            m_pkQueueHall;   		    /// ������Ϣ
	mtSQLEnv*		        m_pkSQLEnv;
};

#endif	///	__MT_SERVICE_HALL_ROOM_2_HALL_H