#ifndef 	__MT_SERVICE_GET_PET_INFI_H
#define 	__MT_SERVICE_GET_PET_INFI_H
#include "mtService.h"
#include "mtQueueHall.h"
/// ��ȡ����Э��
class 	mtServiceGetPetInfo : public mtService
{
public:
	struct DataRead
	{
		long		lStructBytes;		// ����С
		long        lServiceType;	// ��������
		long		plReservation[2];	// �����ֶ�
		long		lUserId;            // �û�id
	};
	struct DataWrite
	{
		long		lStructBytes;		// ����С
		long		lServiceType;		// ��������
		long		plReservation[2];	// �����ֶ�
		long		llevel;			    // �ȼ�
		long		lexperience;		// ����
		long		lnextexperience;	// ��һ�ȼ����辭��
		long		lmash;			    // ���� 
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
	mtServiceGetPetInfo();
	virtual ~mtServiceGetPetInfo();

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

#endif 	/// __MT_SERVICE_GET_PET_INFI_H

