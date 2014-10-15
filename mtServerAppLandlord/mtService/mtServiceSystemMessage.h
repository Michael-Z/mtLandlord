#ifndef 	__MT_SERVICE_SYSTEM_MESSAGE_H
#define 	__MT_SERVICE_SYSTEM_MESSAGE_H
#include "mtService.h"
#include "mtQueueHall.h"

class 	mtServiceSystemMessage : public mtService
{
public:
	struct DataRead
	{
		long				lStructBytes;		/// ����С
		long            	lServiceType;		/// ��������
		long				plReservation[2];	/// �����ֶ�
		long				lUserId;                /// �û�id
	};

	struct DataWrite
	{
		long				           lStructBytes;		/// ����С
		long				           lServiceType;		/// ��������
		long				           plReservation[2];	/// �����ֶ�
		long                           lResult;
		long				           llength;             /// ϵͳ��Ϣ�����鳤��
		char				           pcSystemNotice[256];	/// ϵͳ����
		mtQueueHall::MessageInfo       pkMessageInfo; 
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
	mtServiceSystemMessage();
	virtual ~mtServiceSystemMessage();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);

	void  * getUserNodeAddr(long lUserId);
	void parseMessage(char * message);

	mtQueuePacket *                   m_pkQueuePacket;
	mtQueueHall *                     m_pkQueueHall;   		/// ������Ϣ
	mtSQLEnv *		                  m_pkSQLEnv;
};

#endif 	/// __MT_SERVICE_SYSTEM_MESSAGE_H

