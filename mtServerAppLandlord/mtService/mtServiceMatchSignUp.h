#ifndef 	__MT_SERVICE_MATCH_SIGN_UP_H
#define 	__MT_SERVICE_MATCH_SIGN_UP_H
#include "mtService.h"
#include "mtQueueHall.h"
//��������Э��
class 	mtServiceMatchSignUp : public mtService
{
public:
	struct DataRead
	{
		long 				lStructBytes;			/// ����С
		long                lServiceType;			/// ��������
		long 				plReservation[2];		/// �����ֶ�
		long                lUserId;                /// �û�id > 0
		long				lRoomId;                /// ����id
		long				lFee;                   /// ����
		long				lType;                  /// ���0�����,1�����ѣ�
	};

	struct DataWrite
	{
		long 				lStructBytes;			/// ����С
		long                lServiceType;			/// ��������
		long 				plReservation[2];		/// �����ֶ�				
		long				chip;   			    /// ����
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
	mtServiceMatchSignUp();
	virtual ~mtServiceMatchSignUp();

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

#endif 	/// __MT_SERVICE_MATCH_SIGN_UP_H
