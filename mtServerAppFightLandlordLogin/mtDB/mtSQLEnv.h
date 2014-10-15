#ifndef			__MT_SQLENV_H_
#define			__MT_SQLENV_H_

#include "soci.h"
#include "soci-mysql.h"

#include "mtException.h"
#include "mtDefine.h"
#include "mtGlobal.h"

#include "mtQueueUser.h"

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

	/*struct UserInfo
	{
		long                            lUserId;                /// �û�id	0 ��ʾʧ��
		char							pcVendorId[6];			/// ����id
		char							pcPatchId[4];			/// ���κ�id
		char							pcCompanyId[22];		/// ��˾����
		//long                            lUserIconId;            /// �û�ͷ��id
		char                            pcUserAccountNum[12];   /// �û��˺�
		char                            pcUserPhone[12];        /// �ֻ�����
		char                            pcUserQQ[12];           /// QQ����
		char                            pcUserNickName[12];     /// �û��ǳ�
		//char                            pcUserName[12];         /// �û�����
		char                            pcUserPwd[12];          /// �û�����
		//char                            pcUserLove[32];         /// ��Ȥ����
		//char                            pcUserRemark[64];       /// ��ע˵��
		//long                            lUserSex;               /// �Ա� (0--�У�1--Ů)
		//long                            lUserAge;               /// ����
		//long                            lUserGold;              /// �û�ӵ�н����
		//long 							  lUserScore;				/// �û�����
		//long                            lUserLevel;             /// �û��ȼ�
		//long                            lUserAllChess;          /// �ܾ���
		//long                            lUserWinChess;          /// ʤ����
		//long                            lUserWinRate;           /// ʤ��
		//long                            lUserOffLineCount;      /// ���ߴ���
		long                            lIRegister;             /// �Ƿ��Ѿ�ע��(0 -δע�ᣬ1 -��ע��)��δע���¸��ֶ�lUserRegisterDate��ʾ�״ο��ٵ�¼��ʱ��
		long                            lUserRegisterDate;      /// ע�����ڸ�2�ֽڱ�ʾ�꣬�ε��ֽڱ�ʾ�£����ֽڱ�ʾ��(XXXX-XX-XX)
		long                            lUserLoginHallDate;     /// �û����һ�ε�¼������(XXXX-XX-XX)
		//long                            lUserContinueLoginDays; /// �û�������¼������
		//long                            lUserLoginFirstToday;   /// �Ƿ��ǽ���ĵ�һ�ε�¼(0 -���ǽ����һ�Σ� 1 -�ǽ����һ��)
		//long                            plPropsCount[10];       /// �û�������Ŀ
		//long                            plUserChessPalyed[6];   /// �û�������ÿ������������Ϸ�ľ���
		//long                            lUserGiveGoldFrequence; /// ÿ�����͸��û������Ĵ���
	};*/

public:
	mtSQLEnv(void);
	~mtSQLEnv(void);

	int		init(/*DataInit* pkDataInit*/);
	int		exit();
	int		run();

	virtual int exception();

	//int		saveHallFeeBack(DataRead* pkDataRead);					//������־�洢

	int		getProject(char* vendorId,char* patchId,mtQueueUser::ProjectInfo* pkProjectInfo);

	int		getUserInfo(char* account,long type,mtQueueUser::UserInfo* pkUserInfo);		//�û���Ϣ��ȡ
	int		saveUserInfo(mtQueueUser::UserInfo* pkUserInfo);					//�û���Ϣ�洢

	int		bindingUserInfo(char* id,char* phone,char* qq);						//�������˺�
	int		isBingdingUserInfo(char* phone,char* qq);							//phone��qq�Ƿ��
	int		updatePassWord(char* id,char* userOldPwd,char* userNewPwd);		//�޸�����

	int		getSuperName(char* superName);						//�����ȡ��
	int		getLastName(char* lastName);						//�����ȡ��
	int     getName(char* name);

	const char*	error();

private:
	connection_pool*	m_pkDBPool;						//�̳߳�
	const char*			errorMsg;						//������Ϣ
	int					iPoolSize;
	
};

#endif