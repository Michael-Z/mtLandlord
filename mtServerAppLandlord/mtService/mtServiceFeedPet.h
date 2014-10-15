#ifndef 	__MT_SERVICE_LOTTERY_FEED_PET_H
#define 	__MT_SERVICE_LOTTERY_FEED_PET_H
#include "mtService.h"
#include "mtQueueHall.h"
/// ι������Э��
class 	mtServiceFeedPet : public mtService
{
public:
	struct DataRead
	{
		long		lStructBytes;		// ����С
		long        lServiceType;	    // ��������
		long		plReservation[2];	// �����ֶ�
		long		lUserId;            // �û�id
	};
	struct DataWrite
	{
		long		lStructBytes;		// ����С
		long		lServiceType;		// ��������
		long		plReservation[2];	// �����ֶ�
		long		lResult; 
		long		llevel;			    // �ȼ�
		long		lexperience;		// ����
		long		lmash;			    // ���� 
		long		lnextexperience;	// ��һ�ȼ����辭��
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
	mtServiceFeedPet();
	virtual ~mtServiceFeedPet();

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

#endif 	/// __MT_SERVICE_LOTTERY_FEED_PET_H

