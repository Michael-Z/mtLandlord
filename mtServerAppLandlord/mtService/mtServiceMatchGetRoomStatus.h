#ifndef 	__MT_SERVICE_MATCH_GET_ROOM_STATUS_H
#define 	__MT_SERVICE_MATCH_GET_ROOM_STATUS_H
#include "mtService.h"
#include "mtQueueHall.h"
///��ȡ����״̬Э��
class 	mtServiceMatchGetRoomStatus : public mtService
{
public:
	struct DataRead
	{
		long 				lStructBytes;		/// ����С
		long                lServiceType;		/// ��������
		long 				plReservation[2];	/// �����ֶ�
		long                lUserId;            /// �û�id > 0
		long				lRoomId;            /// ����id
	};
	struct DataWrite
	{
		long 				lStructBytes;		/// ����С
		long                lServiceType;		/// ��������
		long 				plReservation[2];	/// �����ֶ�				
		long				lStatus;            /// 0���ѽ�����1��������
	};
	struct DataInit
	{
		long							lStructBytes;
		char*                           pcServiceExeDir;
		mtQueueHall*                    pkQueueHall;   		// ������Ϣ
		mtQueuePacket*                  pkQueuePacket;
		mtSQLEnv*						pkSQLEnv;
	};
public:
	mtServiceMatchGetRoomStatus();
	virtual ~mtServiceMatchGetRoomStatus();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);
	void  * getUserNodeAddr(long lUserId);
	void   initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);

	mtQueuePacket *                   m_pkQueuePacket;
	mtQueueHall *                     m_pkQueueHall;   		/// ������Ϣ
	mtSQLEnv *		                  m_pkSQLEnv;
};

#endif 	/// __MT_SERVICE_MATCH_GET_ROOM_STATUS_H

