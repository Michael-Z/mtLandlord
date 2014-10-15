#ifndef		__MT_SERVICE_HALL_UPDATE_ROOM_H
#define		__MT_SERVICE_HALL_UPDATE_ROOM_H
#include "mtQueueWork.h"
#include "mtService.h"
#include "mtQueueList.h"
#include "mtQueueMgr.h"

class mtServiceHallUpdateRoom	: public mtService
{

public:
	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lUserId;                /// �û�id
		long							lBatchId;				/// ���κ�id
		long							lVendorId;				/// ���̴���
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		mtQueueHall::DataSpace          pkSpace[2];   			/// ����Ϣ
		long				            llength;                /// ϵͳ��Ϣ�����鳤��
		char				            pcSystemNotice[256];	/// ϵͳ����
		mtQueueHall::MessageInfo    	pkMessageInfo; 		    /// ϵͳ��Ϣ

	};

	struct DataInit
	{
		long							lStructBytes;
		mtQueueHall *                   pkQueueHall;   			         /// ����Ϣ
		mtSQLEnv*						pkSQLEnv;
	};

public:
	mtServiceHallUpdateRoom();
	virtual ~mtServiceHallUpdateRoom();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);
	void*   getUserNodeAddr(long lUserId);
	void mtServiceHallUpdateRoom::parseMessage(char * message);
public:
	mtQueueHall   * m_pkQueueHall;   	    /// ������Ϣ
	mtSQLEnv      * m_pkSQLEnv;

};

#endif	///	__MT_SERVICE_HALL_UPDATE_ROOM_H