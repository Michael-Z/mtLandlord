#ifndef 	__MT_SERVICE_HALL_UPDATE_SOCIAL_INFO_H
#define 	__MT_SERVICE_HALL_UPDATE_SOCIAL_INFO_H
#include "mtService.h"
#include "mtQueueHall.h"

class 	mtServiceHallUpdateSocialInfo : public mtService
{
public:
	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lUserId;                /// �û�id
		long                            lUserIconId;            /// �û�ͷ��id
		long                            lUserSceneId;               ///����ID
		long                            lUserSex;               /// �Ա� (0--�У�1--Ů)
		long                            lUserAge;               /// ����
		char                            pcUserName[12];         /// �û�����
		char                            pcUserNickName[12];     /// �û��ǳ�
		char                            pcUserPwd[12];          /// �û�����
		char                            pcUserLove[32];         /// ��Ȥ����
		char                            pcUserRemark[64];       /// ��ע˵��
		char                            pcUserPhone[12];        /// �ֻ�����
		char                            pcUserQQ[12];           /// QQ����
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long							lResult;                /// �����û���Ϣ���(1 -�ɹ���0 -ʧ�ܣ� -1 -�û��˺Ŵ��� -2 -�û�������)
	};

	struct DataInit
	{
		long							lStructBytes;
		char*                           pcServiceExeDir;
		mtQueueHall*                    pkQueueHall;   		/// ������Ϣ
		mtQueuePacket*                  pkQueuePacket;

		mtSQLEnv*		pkSQLEnv;
	};

public:
	mtServiceHallUpdateSocialInfo();
	virtual ~mtServiceHallUpdateSocialInfo();

	virtual int	init(void* pData);
	virtual int	run(void* pData);
	virtual int exit();
	int		runRead(SOCKET socket, DataRead* pkDataRead);
	int		runWrite(SOCKET socket, const char* pcBuffer, int iBytes,  int flags, int iTimes);

	void*   getUserNodeAddr(long lUserId);
    void   initDataWrite(DataRead &kDataRead,DataWrite &kDataWrite);

	int     updateDataRank(mtQueueHall::DataRank* pkDataRank);
	void    UpdateUserInfo(mtQueuePacket::DataNode* pkQueuePacketDataNode);

	mtQueuePacket*                   m_pkQueuePacket;
	mtQueueHall*                     m_pkQueueHall;   		/// ������Ϣ

	mtSQLEnv*		m_pkSQLEnv;





};

#endif 	/// __MT_SERVICE_HALL_UPDATE_SOCIAL_INFO_H

