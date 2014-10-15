#ifndef 	__MT_SERVICE_MATCH_GET_GET_RANK_H
#define 	__MT_SERVICE_MATCH_GET_GET_RANK_H
#include "mtService.h"
#include "mtQueueHall.h"
/// ����������ȡЭ��
class 	mtServiceMatchGetRank : public mtService
{
public:
	struct DataRead
	{
		long 				lStructBytes;			/// ����С
		long                lServiceType;			/// ��������
		long 				plReservation[2];		/// �����ֶ�
		long                lUserId;                	/// �û�id > 0
		long				lRoomId;
	};

	struct Rank
	{
		char                            pcUserName[32];       	 /// �ǳ�
		long                            lScore;   		 /// ����	
	};

	struct DataWrite
	{
		long 				lStructBytes;			/// ����С
		long                lServiceType;			/// ��������
		long 				plReservation[2];		/// �����ֶ�				
		long				lUserRank;              /// �û�����
		long				lUserScore;             /// �û�����
		long				lStatus;                /// -1: δ������0:δ�ý���1:δ�콱��-2:���콱
		Rank				pkRank[10];   			/// ǰʮ����
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
	mtServiceMatchGetRank();
	virtual ~mtServiceMatchGetRank();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);
	void  * getUserNodeAddr(long lUserId);
	void    initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);
	long    getUserMatchStatus(long lUserid,long lRoomid,long lUserRank,long lawardNum,tm tmEnd,long ldayTime);
	bool    bMatchEnd(tm tmEnd,long ldayTime);
	time_t  getEndTime(tm tmEnd,long ldayTime);

	mtQueuePacket *                   m_pkQueuePacket;
	mtQueueHall *                     m_pkQueueHall;   		/// ������Ϣ
	mtSQLEnv *		                  m_pkSQLEnv;
};

#endif 	/// __MT_SERVICE_MATCH_GET_GET_RANK_H

