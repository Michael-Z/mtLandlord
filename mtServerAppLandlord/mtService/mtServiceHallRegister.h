#ifndef 	__MT_SERVICE_HALL_REGISTER_H
#define 	__MT_SERVICE_HALL_REGISTER_H
#include "mtQueueWork.h"
#include "mtService.h"

class mtQueuePacket;
class mtQueueHall;
class 	mtServiceHallRegister : public mtService
{
public:
	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long 							lUserId;                /// �û�id(-1 -û��Idע��)
		long 							lUserSex;				/// �Ա�
		char 							pcUserAccountNum[12];	/// �˺�
		char							pcUserPwd[12];			/// ����
		char							pcUserPhoneNum[12];		/// �ֻ�����
		char							pcUserQQ[12];			/// QQ����
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long 							lUserId;				/// �û�id, -1 ��ʾע��ʧ�ܣ� �ɹ� >= MT_SERVER_WORK_USER_ID_MIN
	};

	struct DataInit
	{
		long							lStructBytes;
		char*                           pcServiceExeDir;
		mtQueuePacket*                  pkQueuePacket;
		mtQueueHall*                    pkQueueHall;   		/// ������Ϣ
	};
public:
	mtServiceHallRegister();
	virtual ~mtServiceHallRegister();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);

	void*   getUserNodeAddr(long lUserId);

	static  VOID CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);

	mtQueuePacket*                   m_pkQueuePacket;
	mtQueueHall*                     m_pkQueueHall;   		/// ������Ϣ

};

#endif 	/// __MT_SERVICE_HALL_REGISTER_H
