#ifndef			__MT_SQLENV_H_
#define			__MT_SQLENV_H_

#include "soci.h"
#include "soci-mysql.h"

#include "mtException.h"
#include "mtPlatform.h"

#include "mtServiceHallFeedBack.h"
#include "mtQueueHall.h"

using namespace soci;
using namespace std;

class mtSQLEnv : public mtException
{
public:
	enum  
	{
		EXCEPTION_BEG	= 0,
		EXCEPTION_DB_POOL_NEW,
		EXCEPTION_DB_CONNECT_NEW,
		EXCEPTION_END
	};

	enum DBResult
	{
		DB_OPERATING_BEG,
		DB_OPERATING_SUCCESS					= 1,
		DB_OPERATING_FAIL						= 0,
		DB_OPERATING_FAIL_PHONE_IS_EXIST		= -1,
		DB_OPERATING_FAIL_QQ_IS_EXIST			= -2,
		DB_OPERATING_FAIL_ALL_IS_EXIST			= -3,
		DB_OPERATING_FAIL_ERROR					= -4
	};

public:
	mtSQLEnv(void);
	~mtSQLEnv(void);

	int		init(/*DataInit* pkDataInit*/);
	int		exit();
	int		run();

	virtual int exception();

	int		saveHallFeeBack(mtServiceHallFeedBack::DataRead* pkDataRead);		//������־�洢
	int		saveOnlinePerson(mtQueueHall::DataHall* m_kDataHall);
	int		savePayInfo(long userId,long point,long fee);						//�����ֵ���

	int		getPrizeInfo(mtQueueHall::PrizeInfo* m_kPrizeInfo);						//��ȡ�齱����
	int		getPrizeInfoList(mtQueueHall::PrizeInfo* m_kPrizeInfoList);				//��ȡ�齱�б�

	int		getLotteryinfo(long userId,mtQueueHall::LotteryInfo* m_kLotteryInfo);		//�û��齱��Ϣ��ȡ
	int		saveLotteryinfo(mtQueueHall::LotteryInfo* m_kLotteryInfo);				//�û��齱��Ϣ�洢
	int		updateLotteryinfo(mtQueueHall::LotteryInfo* m_kLotteryInfo);				//�û��齱��Ϣ����

	int		getPigInfo(long userId,mtQueueHall::PigInfo* m_kPigInfo);					//��ȡ������Ϣ
	int		savePigInfo(mtQueueHall::PigInfo* m_kPigInfo);
	int		updatePigInfo(mtQueueHall::PigInfo* m_kPigInfo);

	int		getUserPropInfoList(long userId,mtQueueHall::UseTimeInfo* m_kPropInfoList,long* size);		//��ȡ�û�������Ϣ
	int		getUserPropInfo(long userId,long propId,mtQueueHall::UseTimeInfo* pkUserTimeInfo);
	int		saveUserPropInfo(long userId,long propId,std::tm stopTime);
	int		updateUserPropInfo(long userId,long propId,std::tm stopTime);

	int		getTaskInfo(long taskId,mtQueueHall::TaskInfo* m_kTaskInfo);
	int		getTaskInfoList(mtQueueHall::TaskInfo* m_kTaskInfoList);												//��ȡ�����б�
	int		getUserTaskPrizeInfoList(long userId,mtQueueHall::UseTimeInfo* m_kTaskPrizeInfoList,long* size);	//��ȡ�����콱��Ϣ										//��ȡ�û������콱��Ϣ
	int		getUserTaskPrizeInfo(long userId,long taskId,mtQueueHall::UseTimeInfo* m_kTaskPrizeInfo);		//��ȡ�����콱��Ϣ
	int		saveUserTaskPrizeInfo(long userId,long taskId);					//���������콱��Ϣ
	int		updateUserTaskPrizeInfo(long userId,long taskId);					//���������콱��Ϣ

	int		saveAwardInfo(mtQueueHall::AwardInfo* m_kAwardInfo);					//�ý���Ϣ����

	int		getLastNoticeInfo(char* content);										//��ȡ���µĹ���
	int		saveSystemMessage(char* content);
	int		getSystemMessageList(mtQueueHall::MessageInfo* m_kMessageInfo);

	int		getUserInfo(long userId,mtQueueHall::UserInfo* pkUserInfo);		//�û���Ϣ��ȡ
	int		saveUserInfo(mtQueueHall::UserInfo* pkUserInfo);					//�û���Ϣ�洢
	int		updateUserInfo(mtQueueHall::UserInfo* pkUserInfo);					//�û���Ϣ����
	
	int		getProject(long vendorId,long patchId,mtQueueHall::ProjectInfo* pkProjectInfo);
	
	int		getPKRoomList(mtQueueHall::PKRoomInfo* pkPKRoomInfoList,long size);				//��ȡ����������
	int		getPKRoom(long roomId,mtQueueHall::PKRoomInfo* pkPKRoomInfo);

	int		getUserPKInfo(long userId,long roomId,mtQueueHall::UserPKInfo* pkUserPKInfo);
	int		saveUserPKInfo(mtQueueHall::UserPKInfo* pkUserPKInfo);
	int		updateUserPKInfo(mtQueueHall::UserPKInfo* pkUserPKInfo);

	int		getPKRoomRankList(long roomId,mtQueueHall::DataRank* pkPKRoomRankList,long* size);
	int		getPKUserRankInfo(long userId,long roomId,mtQueueHall::DataRank* pkPKRoomRank);

	const char*	error();

private:
	connection_pool*	m_pkDBPool;						//�̳߳�
	const char*			errorMsg;						//������Ϣ
	int					iPoolSize;
};

#endif