#ifndef 	__MT_SERVICE_MATCH_GET_ROOM_INFO_H
#define 	__MT_SERVICE_MATCH_GET_ROOM_INFO_H
#include "mtService.h"
#include "mtQueueHall.h"
/// 24��ȡ����������ϢЭ��
class 	mtServiceMatchGetRoomInfo : public mtService
{
public:
	struct DataRead
	{
		long 				lStructBytes;		/// ����С
		long                lServiceType;		/// ��������
		long 				plReservation[2];	/// �����ֶ�
		long                lUserId;            /// �û�id > 0
	};

	struct DataWrite
	{
		long 				      lStructBytes;		/// ����С
		long                      lServiceType;		/// ��������
		long 				      plReservation[2];	/// �����ֶ�				
		mtQueueHall::MatchRoom    pkRoom[4];   		/// ������Ϣ
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
	mtServiceMatchGetRoomInfo();
	virtual ~mtServiceMatchGetRoomInfo();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);
	void  * getUserNodeAddr(long lUserId);
	void   initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);
	void   getActiveTime(long *lActiveTime,tm &tmStart,tm &tmEndTime,long ldayTime);
	bool   bMatchOpen(long ldayTime);
	long   getMatchStatus(tm tmStart,tm tmEnd,long ldayTime);
	long   getBeginHour(long ldayTime);
	long   getEndHour(long ldayTime);

	mtQueuePacket *                   m_pkQueuePacket;
	mtQueueHall *                     m_pkQueueHall;   		/// ������Ϣ
	mtSQLEnv *		                  m_pkSQLEnv;
};

#endif 	/// __MT_SERVICE_MATCH_GET_ROOM_INFO_H

