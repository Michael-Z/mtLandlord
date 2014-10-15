#ifndef 	__MT_SERVICE_SPIT_GOLD_H
#define 	__MT_SERVICE_SPIT_GOLD_H
#include "mtService.h"
#include "mtQueueHall.h"
/// �½��Э��
class mtServiceSpitGold : public mtService
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
		long		lgold;			    // �³��Ľ��
	};
	struct DataInit
	{
		long							lStructBytes;
		char*                           pcServiceExeDir;
		mtQueueHall*                    pkQueueHall;   		// ������Ϣ
		mtQueuePacket*                  pkQueuePacket;
		mtSQLEnv*						pkSQLEnv;
	};
	enum Result
	{
		E_RESULT_SUCESS                 = 1,
		E_RESULT_HAVED                  = -1,
		E_RESULT_ERROR                  = 0,
	};
public:
	mtServiceSpitGold();
	virtual ~mtServiceSpitGold();

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

#endif 	/// __MT_SERVICE_SPIT_GOLD_H

