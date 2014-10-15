#ifndef		__MT_SERVICE_HALL_HALL_2_ROOM_H
#define		__MT_SERVICE_HALL_HALL_2_ROOM_H
#include "mtQueueWork.h"
#include "mtService.h"
#include "mtQueueList.h"
#include "mtQueueMgr.h"
/// �Ӵ���Ҫ���뷿��ķ���
class mtServiceHallHall2Room	: public mtService
{

public:
	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lSpaceId;               /// ��id
		long                            lRoomId;                /// ����id
		long                            lUserId;                /// �û�id
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lResult;                /// �û��Ӵ������뷿����(0 -ʧ�ܣ�1 -�ɹ���-1 -�û�id����
		                                                            /// 3 -�û��˺��Ѿ�������ĳ������)
		                                                            /// 4 -���û�֮ǰ��ĳ����ǿ���˳�(��Ϸ�Ѿ���ʼ�һ�û����)
		                                                            /// 5 -���û�֮ǰ��������ԭ���ĳ�����쳣�˳�(��Ϸ�Ѿ���ʼ�һ�û����)
		long                            lSpaceId;               /// ��id
		long                            lRoomId;                /// ����id
	};

	struct DataInit
	{
		long							lStructBytes;
		mtQueueHall*                    pkQueueHall;   		    /// ������Ϣ
	};

public:
	mtServiceHallHall2Room();
	virtual ~mtServiceHallHall2Room();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);
	void*   getUserNodeAddr(long lUserId);
	void   initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);
public:
	mtQueueHall*                     m_pkQueueHall;   		/// ������Ϣ
};

#endif	///	__MT_SERVICE_HALL_HALL_2_ROOM_H