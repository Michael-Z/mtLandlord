#include "mtSQLEnv.h"

mtSQLEnv::mtSQLEnv(void)
{
}

mtSQLEnv::~mtSQLEnv(void)
{
}

namespace soci{
	template<>
	struct type_conversion<mtQueueUser::UserInfo>
	{
		typedef values base_type;

		static void from_base(values const & v,indicator & ind,mtQueueUser::UserInfo & userInfo)
		{
			userInfo.ulId = v.get<int>("id");					/// �û�id	0 ��ʾʧ��
			sprintf(userInfo.pcVendorId,"%d",v.get<int>("vendorId"));					///����ID
			sprintf(userInfo.pcPatchId,"%d",v.get<int>("batchId"));						///����ID
			strcpy(userInfo.pcCompanyId,v.get<std::string>("companyCode","").c_str());		///��˾����
			//userInfo.lUserIconId = v.get<int>("iconId");			/// �û�ͷ��id
			strcpy(userInfo.pcAccount,v.get<std::string>("accountNum","").c_str());		/// �û��˺�
			strcpy(userInfo.pcPhone,v.get<std::string>("phone","").c_str());				/// �ֻ�����
			strcpy(userInfo.pcQQ,v.get<std::string>("qq","").c_str());						/// QQ����
			strcpy(userInfo.pcPwd,v.get<std::string>("pwd","").c_str());					/// �û�����
			//strcpy(userInfo.pcUserName,v.get<std::string>("name","").c_str());					/// �û�����
			//strcpy(userInfo.pcUserNickName,v.get<std::string>("nickName","").c_str());			/// �û��ǳ�
			//strcpy(userInfo.pcUserLove,v.get<std::string>("love","").c_str());					/// ��Ȥ����
			//strcpy(userInfo.pcUserRemark,v.get<std::string>("remark","").c_str());				/// ��ע˵��
			
			//userInfo.lUserSex = v.get<int>("sex");						/// �Ա� (0--�У�1--Ů)
			//userInfo.lUserAge = v.get<int>("age");						/// ����
			//userInfo.lUserGold = v.get<int>("gold");					/// �û�ӵ�н����
			//userInfo.lUserScore = v.get<int>("score");					/// �û�����
			//userInfo.lUserLevel = v.get<int>("level");					/// �û��ȼ�
			//userInfo.lUserAllChess = v.get<int>("allChess");			/// �ܾ���
			//userInfo.lUserWinChess = v.get<int>("winChess");			/// ʤ����
			//userInfo.lUserWinRate = v.get<int>("winRate");				/// ʤ��
			//userInfo.lUserOffLineCount = v.get<int>("offLineCount");	/// ���ߴ���
			//userInfo.lRegister = v.get<int>("iRegister");				/// �Ƿ��Ѿ�ע��(0 -δע�ᣬ1 -��ע��)��δע���¸��ֶ�lUserRegisterDate��ʾ�״ο��ٵ�¼��ʱ��

			std::tm kRegisterDate = v.get<std::tm>("registerDate");			/// ע�����ڸ�2�ֽڱ�ʾ�꣬�ε��ֽڱ�ʾ�£����ֽڱ�ʾ��(XXXX-XX-XX)
			long lregisterHallDate;
			unsigned short* pusYear = (unsigned short*)&lregisterHallDate;
			char* pcMonthDate       = (char*)&lregisterHallDate;
			pusYear[1]                          = kRegisterDate.tm_year;
			pcMonthDate[1]                      = kRegisterDate.tm_mon;
			pcMonthDate[0]                      = kRegisterDate.tm_mday;
			userInfo.lUserRegisterDate = lregisterHallDate;
			
			/*std::tm kLoginDate = v.get<std::tm>("loginHallDate");		/// �û����һ�ε�¼������(XXXX-XX-XX)
			long lLoginHallDate;
			pusYear = (unsigned short*)&lLoginHallDate;
			pcMonthDate       = (char*)&lLoginHallDate;
			pusYear[1]                          = kLoginDate.tm_year;
			pcMonthDate[1]                      = kLoginDate.tm_mon;
			pcMonthDate[0]                      = kLoginDate.tm_mday;
			userInfo.lUserLoginHallDate = lLoginHallDate;*/

			//userInfo.lUserContinueLoginDays = v.get<int>("continueLoginDays");		/// �û�������¼������
			//userInfo.lUserLoginFirstToday = v.get<int>("loginFirstToday");			/// �Ƿ��ǽ���ĵ�һ�ε�¼(0 -���ǽ����һ�Σ� 1 -�ǽ����һ��)
			//userInfo.lUserGiveGoldFrequence = v.get<int>("giveGoldFrequence");		/// ÿ�����͸��û������Ĵ���

			//char sPropsCount[50];
			//strcpy(sPropsCount,v.get<std::string>("propsCount","").c_str());		/// �û�������Ŀ
			//readNumSep(sPropsCount,",",userInfo.plPropsCount,10);

			//char sChessPalyed[30];
			//strcpy(sChessPalyed,v.get<std::string>("chessPalyed").c_str());		/// �û�������ÿ������������Ϸ�ľ���
			//readNumSep(sChessPalyed,",",userInfo.plUserChessPalyed,6);
			
			ind = i_ok;
		}
		static void to_base(const mtQueueUser::UserInfo & userInfo,values &v,indicator & ind)
		{
			v.set("id",(int)userInfo.ulId);							/// �û�id	0 ��ʾʧ��
			v.set("vendorId",atoi(userInfo.pcVendorId));						///����ID
			v.set("batchId",atoi(userInfo.pcPatchId));							///����ID
			v.set("companyCode",(std::string)userInfo.pcCompanyId);			///��˾����
			//v.set("iconId",(int)userInfo.lUserIconId);					/// �û�ͷ��id
			v.set("accountNum",(std::string)userInfo.pcAccount);		/// �û��˺�
			v.set("phone",(std::string)userInfo.pcPhone); 					/// �ֻ�����
			v.set("qq",(std::string)userInfo.pcQQ); 						/// QQ����
			v.set("pwd",(std::string)userInfo.pcPwd); 						/// �û�����
			//v.set("name",(std::string)userInfo.pcUserName); 					/// �û�����
			v.set("nickName",(std::string)userInfo.pcNickName); 			/// �û��ǳ�
			//v.set("love",(std::string)userInfo.pcUserLove); 					/// ��Ȥ����
			//v.set("remark",(std::string)userInfo.pcUserRemark); 				/// ��ע˵��
			//v.set("sex",(int)userInfo.lUserSex);  						/// �Ա� (0--�У�1--Ů)
			//v.set("age",(int)userInfo.lUserAge);  						/// ����
			//v.set("gold",(int)userInfo.lUserGold);  					/// �û�ӵ�н����
			//v.set("score",(int)userInfo.lUserScore);  					/// �û�����
			//v.set("level",(int)userInfo.lUserLevel);  					/// �û��ȼ�
			//v.set("allChess",(int)userInfo.lUserAllChess);  			/// �ܾ���
			//v.set("winChess",(int)userInfo.lUserWinChess);  			/// ʤ����
			//v.set("winRate",(int)userInfo.lUserWinRate);  				/// ʤ��

			//v.set("offLineCount",(int)userInfo.lUserOffLineCount);  	/// ���ߴ���
			//v.set("iRegister",(int)userInfo.lRegister);  				/// �Ƿ��Ѿ�ע��(0 -δע�ᣬ1 -��ע��)��δע���¸��ֶ�lUserRegisterDate��ʾ�״ο��ٵ�¼��ʱ��

			std::tm  kRegisterNow;
			__time64_t kRNowTime;
			time(&kRNowTime);
			_localtime64_s(&kRegisterNow, &kRNowTime);
			v.set("registerDate",kRegisterNow);			//userInfo.lUserRegisterDate		/// ע�����ڸ�2�ֽڱ�ʾ�꣬�ε��ֽڱ�ʾ�£����ֽڱ�ʾ��(XXXX-XX-XX)
			v.set("loginHallDate",kRegisterNow);		//userInfo.lUserLoginHallDate		/// �û����һ�ε�¼������(XXXX-XX-XX)

			//v.set("continueLoginDays",(int)userInfo.lUserContinueLoginDays);		/// �û�������¼������
			//v.set("loginFirstToday",(int)userInfo.lUserLoginFirstToday);			/// �Ƿ��ǽ���ĵ�һ�ε�¼(0 -���ǽ����һ�Σ� 1 -�ǽ����һ��)
			//v.set("giveGoldFrequence",(int)userInfo.lUserGiveGoldFrequence); 		/// ÿ�����͸��û������Ĵ���

			/*char pcValue[50];
			pcValue[49] = '\0';
			sprintf(pcValue,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",userInfo.plPropsCount[0],userInfo.plPropsCount[1],
				userInfo.plPropsCount[2],userInfo.plPropsCount[3],userInfo.plPropsCount[4],userInfo.plPropsCount[5],
				userInfo.plPropsCount[6],userInfo.plPropsCount[7],userInfo.plPropsCount[8],userInfo.plPropsCount[9]);
			pcValue[strlen(pcValue)] = '\0';
			v.set("propsCount",(std::string)pcValue);						/// �û�������Ŀ
			*/
			/*pcValue[49] = '\0';
			sprintf(pcValue,"%d,%d,%d,%d,%d,%d",userInfo.plUserChessPalyed[0],userInfo.plUserChessPalyed[1],
				userInfo.plUserChessPalyed[2],userInfo.plUserChessPalyed[3],userInfo.plUserChessPalyed[4],
				userInfo.plUserChessPalyed[5]);
			pcValue[strlen(pcValue)] = '\0';
			v.set("chessPalyed",(std::string)pcValue);						/// �û�������ÿ������������Ϸ�ľ���
			*/
			ind = i_ok;
		}

		//����ַ�������
		static void readNumSep(const char* pcDir,const char *delim,long* count,int size){
			char pcDirTmp[50];
			strcpy(pcDirTmp, pcDir);
			char *p;
			for(int i=0;i<size;i++){
				if(i==0)
					p = strtok(pcDirTmp,delim);
				else
					p = strtok(NULL,delim);
				if(p)
					count[i] = atoi(p);
			}
		}
	};

	template<>
	struct type_conversion<mtQueueUser::ProjectInfo>
	{
		typedef values base_type;
		static void from_base(values const & v,indicator & ind,mtQueueUser::ProjectInfo & projectInfo)
		{
			sprintf(projectInfo.pcVendorId,"%d", v.get<int>("vid"));
			sprintf(projectInfo.pcPatchId,"%d", v.get<int>("pid"));
			projectInfo.lUpdateType = v.get<int>("update_type");
			strcpy(projectInfo.pcUpdateUrl,v.get<std::string>("update_url").c_str());	
			projectInfo.lUpdatePort = v.get<int>("update_port");
			projectInfo.lVersion3 = v.get<int>("update_v3");
			projectInfo.lVersion2 = v.get<int>("update_v2");
			projectInfo.lVersion1 = v.get<int>("update_v1");
			projectInfo.lVersion0 = v.get<int>("update_v0");

			ind = i_ok;
		}
		static void to_base(const mtQueueUser::ProjectInfo & projectInfo,values &v,indicator & ind)
		{

		}

	};
}


int mtSQLEnv::init(/*DataInit* pkDataInit*/)
{
	iPoolSize = g_DataBasePoolSize;

	MT_EXCEPTION_DEBUG(NULL != (m_pkDBPool = new connection_pool(iPoolSize)), EXCEPTION_DB_POOL_NEW);

	int		iConnectIdx;
	char conStr[256]  = {0};
	sprintf(conStr,"mysql://host=%s db=%s user=%s password='%s' charset=%s",g_DataBaseServer,
		g_DataBaseName,g_DataBaseUserName,g_DataBasePassWord,g_DataBaseCODE);
	//MT_DEBUG(8,"\n%s",conStr);
	try
	{
		for(iConnectIdx = 0; iConnectIdx<iPoolSize; iConnectIdx++){
			session &sessionConn = m_pkDBPool->at(iConnectIdx);
			sessionConn.open(conStr);
			//MT_DEBUG(8,"\ninit connection :%d",sessionConn.get_backend());
		}
		MT_DEBUG(PTCL, "init DBPool [%d] success!\n",(iConnectIdx));
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_PTCL("\nSQLERROR: [%s] from Connect DB",e.what());
		errorMsg = e.what();
		MT_EXCEPTION_DEBUG(0, EXCEPTION_DB_CONNECT_NEW);
	}

	return 1;
}

/*int	mtSQLEnv::saveHallFeeBack(mtServiceHallFeedBack::DataRead* pkDataRead)
{
	try
	{
		session sql(*m_pkDBPool);
		sql << "insert into feedback(userid,content,create_at) values(:userid,:content,now())",
			use((pkDataRead->lUserId+MT_SERVER_WORK_USER_ID_BASE)),use((string)pkDataRead->pcContentFeedBack);
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_PTCL("\nSQLERROR: [%s] from saveHallFeeBack",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}*/

int	mtSQLEnv::getProject(char* vendorId,char* patchId,mtQueueUser::ProjectInfo* pkProjectInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "select * from project where vid = :vendorId and pid = :patchId limit 1",
			use(atoi(vendorId)),use(atoi(patchId)),into(*pkProjectInfo,ind);

		if(ind != i_ok)
			return DB_OPERATING_FAIL;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_PTCL("\nSQLERROR: [%s] from getProject",e.what());
		errorMsg = e.what();
		return DB_OPERATING_FAIL_ERROR;
	}

	return DB_OPERATING_SUCCESS;
}

//��ȡ�û���Ϣ
int	mtSQLEnv::getUserInfo(char* account,long type,mtQueueUser::UserInfo* pkUserInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		if(mtQueueUser::E_ACCOUNT_TYPE_ID == type)
		{
			sql << "select * from userinfo where id = :account limit 1",use((std::string)account),into(*pkUserInfo,ind);
		}
		else if(mtQueueUser::E_ACCOUNT_TYPE_ACCOUNT == type)
		{
			sql << "select * from userinfo where accountNum = :account limit 1",use((std::string)account),into(*pkUserInfo,ind);
		}
		else if(mtQueueUser::E_ACCOUNT_TYPE_PHONE == type)
		{
			sql << "select * from userinfo where phone = :account limit 1",use((std::string)account),into(*pkUserInfo,ind);
		}
		else if(mtQueueUser::E_ACCOUNT_TYPE_QQ == type)
		{
			sql << "select * from userinfo where qq = :account limit 1",use((std::string)account),into(*pkUserInfo,ind);
		}

		if(ind != i_ok)
			return DB_OPERATING_FAIL;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_PTCL("\nSQLERROR: [%s] from getUserInfo",e.what());
		errorMsg = e.what();
		return DB_OPERATING_FAIL_ERROR;
	}

	return DB_OPERATING_SUCCESS;
}

//�����û���Ϣ
int mtSQLEnv::saveUserInfo(mtQueueUser::UserInfo* pkUserInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "insert into userinfo(id,vendorId,batchId,companyCode,iconId,sceneId,accountNum,phone,qq,pwd,name,nickName,love,remark,sex,age,"\
			"gold,score,level,payCount,firstCharge,dayChess,allChess,winChess,winRate,offLineCount,registerDate,loginHallDate,continueLoginDays,loginFirstToday," \
			"giveGoldFrequence,chessPalyed) values(:id,:vendorId,:batchId,:companyCode,0,0,:accountNum,:phone,:qq,:pwd,'',:nickName,'',''," \
			"0,0,1000,0,0,0,0,0,0,0,0,0,now(),'1970-01-01 00:00:00',1,0,0,'0,0,0,0,0,0')",use(*pkUserInfo,ind);

		if(ind != i_ok)
			return DB_OPERATING_FAIL;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_PTCL("\nSQLERROR: [%s] from saveUserInfo",e.what());
		errorMsg = e.what();
		return DB_OPERATING_FAIL_ERROR;
	}

	return DB_OPERATING_SUCCESS;
}
//�������˺�
int mtSQLEnv::bindingUserInfo(char* id,char* phone,char* qq)
{
	try
	{
		session sql(*m_pkDBPool);
		//indicator ind = i_truncated;
		sql << "update userinfo set phone=:phone,qq=:qq where id=:id",
			use((std::string)phone),use((std::string)qq),use((std::string)id);

		//if(ind != i_ok)
			//return DB_OPERATING_FAIL;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_PTCL("\nSQLERROR: [%s] from bindingUserInfo",e.what());
		errorMsg = e.what();
		return DB_OPERATING_FAIL_ERROR;
	}

	return DB_OPERATING_SUCCESS;
}
int mtSQLEnv::isBingdingUserInfo(char* phone,char* qq)
{
	try
	{
		session sql(*m_pkDBPool);
		int count = 0;
		indicator ind = i_truncated;
		if(strcmp(phone,"") != 0 && strcmp(qq,"") !=0)
		{
			sql << "select count(*) from userinfo where phone=:phone or qq=:qq; ",
				use((std::string)phone),use((std::string)qq),into(count,ind);
			if(ind == i_ok){
				if(count > 0)
					return DB_OPERATING_FAIL_ALL_IS_EXIST;
			}
		}
		else if(strcmp(phone,"") != 0)
		{	
			sql << "select count(*) from userinfo where phone=:phone; ",use((std::string)phone),into(count,ind);

			if(ind == i_ok){
				if(count > 0)
					return DB_OPERATING_FAIL_PHONE_IS_EXIST;
			}
		}else if(strcmp(qq,"") !=0 )
		{
			sql << "select count(*) from userinfo where and qq=:qq;",use((std::string)qq),into(count,ind);
			if(ind == i_ok){
				if(count > 0)
					return DB_OPERATING_FAIL_QQ_IS_EXIST;
			}
		}
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_PTCL("\nSQLERROR: [%s] from isBingdingUserInfo",e.what());
		errorMsg = e.what();
		return DB_OPERATING_FAIL_ERROR;
	}

	return DB_OPERATING_SUCCESS;
}
//�޸��û�����
int mtSQLEnv::updatePassWord(char* id,char* userOldPwd,char* userNewPwd)
{
	try
	{
		session sql(*m_pkDBPool);
		//indicator ind = i_truncated;
		sql << "update userinfo set pwd=:userNewPwd where id=:id and pwd=:userOldPwd",
			use((std::string)userNewPwd),use((std::string)id),use((std::string)userOldPwd);

		//if(ind != i_ok)
			//return DB_OPERATING_FAIL;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_PTCL("\nSQLERROR: [%s] from updatePassWord",e.what());
		errorMsg = e.what();
		return DB_OPERATING_FAIL_ERROR;
	}

	return DB_OPERATING_SUCCESS;
}

int mtSQLEnv::getSuperName(char* sName)
{
	try
	{
		session sql(*m_pkDBPool);
		string supername;
		indicator ind = i_truncated;
		sql << "SELECT surname FROM surname ORDER BY rand() LIMIT 1; ",into(supername,ind);
		if(ind == i_ok){
			strcpy(sName ,supername.c_str());
			return DB_OPERATING_SUCCESS;
		}
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_PTCL("\nSQLERROR: [%s] from getSuperName",e.what());
		errorMsg = e.what();
		return DB_OPERATING_FAIL_ERROR;
	}

	return DB_OPERATING_FAIL;
}
int mtSQLEnv::getLastName(char* lastName)
{
	try
	{
		session sql(*m_pkDBPool);
		string name;
		indicator ind = i_truncated;
		sql << "SELECT name FROM lastname ORDER BY rand() LIMIT 1; ",into(name,ind);
		if(ind == i_ok){
			strcpy(lastName ,name.c_str());
			return DB_OPERATING_SUCCESS;
		}
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_PTCL("\nSQLERROR: [%s] from getLastName",e.what());
		errorMsg = e.what();
		return DB_OPERATING_FAIL_ERROR;
	}

	return DB_OPERATING_FAIL;
}

int mtSQLEnv::getName(char* name)
{
	char supername[8],lastname[8];
	getSuperName(supername);
	getLastName(lastname);
	sprintf(name,"%s%s",supername,lastname);
	return 1;
}

const char* mtSQLEnv::error()
{
	return this->errorMsg;
}

int mtSQLEnv::exit()
{
	if (NULL != m_pkDBPool)
	{
		int		iConnectIdx;
		for(iConnectIdx = 0; iConnectIdx<iPoolSize; iConnectIdx++){
			session &sessionConn = m_pkDBPool->at(iConnectIdx);
			sessionConn.close();
		}
		delete m_pkDBPool;
		m_pkDBPool = NULL;
	}
	return 1;
}

int mtSQLEnv::exception()
{
	switch (m_iExceptionId)
	{
		MT_EXCETPION_STRING(EXCEPTION_DB_POOL_NEW)
		MT_EXCETPION_STRING(EXCEPTION_DB_CONNECT_NEW)
		
	default:
		MT_DEBUG_PTCL( "���쳣û�б�����");
		return	0;
	}

	return	1;
}

int mtSQLEnv::run()
{
	return	1;
}