#include "mtSQLEnv.h"


mtSQLEnv::mtSQLEnv(void)
{
}

mtSQLEnv::~mtSQLEnv(void)
{
}

namespace soci{
	template<>
	struct type_conversion<mtQueueHall::UserInfo>
	{
		typedef values base_type;

		static void from_base(values const & v,indicator & ind,mtQueueHall::UserInfo & userInfo)
		{
			userInfo.lUserId = v.get<int>("id");					/// �û�id	0 ��ʾʧ��
			//userInfo.lBatchId = v.get<int>("batchId");
			//userInfo.lVendorId = v.get<int>("vendorId");
			userInfo.lUserIconId = v.get<int>("iconId");			/// �û�ͷ��id
			userInfo.lUserSceneId   = v.get<int>("sceneId");
			strcpy(userInfo.pcUserAccountNum,v.get<std::string>("accountNum","").c_str());		/// �û��˺�
			strcpy(userInfo.pcUserName,v.get<std::string>("name","").c_str());					/// �û�����
			strcpy(userInfo.pcUserNickName,v.get<std::string>("nickName","").c_str());			/// �û��ǳ�
			strcpy(userInfo.pcUserPwd,v.get<std::string>("pwd","").c_str());					/// �û�����
			strcpy(userInfo.pcUserLove,v.get<std::string>("love","").c_str());					/// ��Ȥ����
			strcpy(userInfo.pcUserRemark,v.get<std::string>("remark","").c_str());				/// ��ע˵��
			strcpy(userInfo.pcUserPhone,v.get<std::string>("phone","").c_str());				/// �ֻ�����
			strcpy(userInfo.pcUserQQ,v.get<std::string>("qq","").c_str());						/// QQ����
			userInfo.lUserSex = v.get<int>("sex");						/// �Ա� (0--�У�1--Ů)
			userInfo.lUserAge = v.get<int>("age");						/// ����
			userInfo.lUserGold = v.get<int>("gold");					/// �û�ӵ�н����
			userInfo.lUserScore = v.get<int>("score");					/// �û�����
			userInfo.lUserLevel = v.get<int>("level");					/// �û��ȼ�
			userInfo.lUserPayCount = v.get<int>("payCount");
			userInfo.lUserFirstCharge = v.get<int>("firstCharge");
			userInfo.lUserDayChess = v.get<int>("dayChess");

			userInfo.lUserAllChess = v.get<int>("allChess");			/// �ܾ���
			userInfo.lUserWinChess = v.get<int>("winChess");			/// ʤ����
			userInfo.lUserWinRate = v.get<int>("winRate");				/// ʤ��
			userInfo.lUserOffLineCount = v.get<int>("offLineCount");	/// ���ߴ���

			std::tm kRegisterDate = v.get<std::tm>("registerDate");			/// ע�����ڸ�2�ֽڱ�ʾ�꣬�ε��ֽڱ�ʾ�£����ֽڱ�ʾ��(XXXX-XX-XX)
			long lregisterHallDate;
			unsigned short* pusYear = (unsigned short*)&lregisterHallDate;
			char* pcMonthDate       = (char*)&lregisterHallDate;
			pusYear[1]                          = kRegisterDate.tm_year;
			pcMonthDate[1]                      = kRegisterDate.tm_mon;
			pcMonthDate[0]                      = kRegisterDate.tm_mday;
			userInfo.lUserRegisterDate = lregisterHallDate;
			
			std::tm kLoginDate = v.get<std::tm>("loginHallDate");		/// �û����һ�ε�¼������(XXXX-XX-XX)
			long lLoginHallDate;
			pusYear = (unsigned short*)&lLoginHallDate;
			pcMonthDate       = (char*)&lLoginHallDate;
			pusYear[1]                          = kLoginDate.tm_year;
			pcMonthDate[1]                      = kLoginDate.tm_mon;
			pcMonthDate[0]                      = kLoginDate.tm_mday;
			userInfo.lUserLoginHallDate = lLoginHallDate;

			userInfo.lUserContinueLoginDays = v.get<int>("continueLoginDays");		/// �û�������¼������
			userInfo.lUserLoginFirstToday = v.get<int>("loginFirstToday");			/// �Ƿ��ǽ���ĵ�һ�ε�¼(0 -���ǽ����һ�Σ� 1 -�ǽ����һ��)
			userInfo.lUserGiveGoldFrequence = v.get<int>("giveGoldFrequence");		/// ÿ�����͸��û������Ĵ���
			/*
			char sPropsCount[50];
			strcpy(sPropsCount,v.get<std::string>("propsCount","").c_str());		/// �û�������Ŀ
			readNumSep(sPropsCount,",",userInfo.plPropsCount,10);
			*/
			char sChessPalyed[30];
			strcpy(sChessPalyed,v.get<std::string>("chessPalyed").c_str());		/// �û�������ÿ������������Ϸ�ľ���
			readNumSep(sChessPalyed,",",userInfo.plUserChessPalyed,6);
			
			ind = i_ok;
		}
		static void to_base(const mtQueueHall::UserInfo & userInfo,values &v,indicator & ind)
		{
			v.set("id",(int)userInfo.lUserId);							/// �û�id	0 ��ʾʧ��
			v.set("iconId",(int)userInfo.lUserIconId);					/// �û�ͷ��id
			v.set("sceneId",(int)userInfo.lUserSceneId);
			v.set("accountNum",(std::string)userInfo.pcUserAccountNum);		/// �û��˺�
			v.set("name",(std::string)userInfo.pcUserName); 					/// �û�����
			v.set("nickName",(std::string)userInfo.pcUserNickName); 			/// �û��ǳ�
			v.set("pwd",(std::string)userInfo.pcUserPwd); 						/// �û�����
			v.set("love",(std::string)userInfo.pcUserLove); 					/// ��Ȥ����
			v.set("remark",(std::string)userInfo.pcUserRemark); 				/// ��ע˵��
			v.set("phone",(std::string)userInfo.pcUserPhone); 					/// �ֻ�����
			v.set("qq",(std::string)userInfo.pcUserQQ); 						/// QQ����
			v.set("sex",(int)userInfo.lUserSex);  						/// �Ա� (0--�У�1--Ů)
			v.set("age",(int)userInfo.lUserAge);  						/// ����
			v.set("gold",(int)userInfo.lUserGold);  					/// �û�ӵ�н����
			v.set("score",(int)userInfo.lUserScore);  					/// �û�����
			v.set("level",(int)userInfo.lUserLevel);  					/// �û��ȼ�
			v.set("payCount",(int)userInfo.lUserPayCount);
			v.set("firstCharge",(int)userInfo.lUserFirstCharge);
			v.set("dayChess",(int)userInfo.lUserDayChess);
			v.set("allChess",(int)userInfo.lUserAllChess);  			/// �ܾ���
			v.set("winChess",(int)userInfo.lUserWinChess);  			/// ʤ����
			v.set("winRate",(int)userInfo.lUserWinRate);  				/// ʤ��

			v.set("offLineCount",(int)userInfo.lUserOffLineCount);  	/// ���ߴ���

			std::tm  kloginHallNow;
			__time64_t kNowTime;
			time(&kNowTime);
			_localtime64_s(&kloginHallNow, &kNowTime);
			//v.set("registerDate",userInfo.lUserRegisterDate);			//userInfo.lUserRegisterDate		/// ע�����ڸ�2�ֽڱ�ʾ�꣬�ε��ֽڱ�ʾ�£����ֽڱ�ʾ��(XXXX-XX-XX)
			v.set("loginHallDate",kloginHallNow);		//userInfo.lUserLoginHallDate		/// �û����һ�ε�¼������(XXXX-XX-XX)

			std::tm  kRegisterDate;
			memset(&kRegisterDate,0,sizeof(kRegisterDate));
			kRegisterDate.tm_year   = (userInfo.lUserRegisterDate >> 16) & 0xFFFF;
			kRegisterDate.tm_mon   = (userInfo.lUserRegisterDate >> 8) & 0xFF;
			kRegisterDate.tm_mday   = userInfo.lUserRegisterDate & 0xFF;
			v.set("registerDate",kRegisterDate);

			v.set("continueLoginDays",(int)userInfo.lUserContinueLoginDays);		/// �û�������¼������
			v.set("loginFirstToday",(int)userInfo.lUserLoginFirstToday);			/// �Ƿ��ǽ���ĵ�һ�ε�¼(0 -���ǽ����һ�Σ� 1 -�ǽ����һ��)
			v.set("giveGoldFrequence",(int)userInfo.lUserGiveGoldFrequence); 		/// ÿ�����͸��û������Ĵ���
			/*
			char pcValue[50];
			pcValue[49] = '\0';
			sprintf(pcValue,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",userInfo.plPropsCount[0],userInfo.plPropsCount[1],
				userInfo.plPropsCount[2],userInfo.plPropsCount[3],userInfo.plPropsCount[4],userInfo.plPropsCount[5],
				userInfo.plPropsCount[6],userInfo.plPropsCount[7],userInfo.plPropsCount[8],userInfo.plPropsCount[9]);
			pcValue[strlen(pcValue)] = '\0';
			v.set("propsCount",(std::string)pcValue);						/// �û�������Ŀ
			*/
			char pcValue[50]; 
			pcValue[49] = '\0';
			sprintf(pcValue,"%d,%d,%d,%d,%d,%d",userInfo.plUserChessPalyed[0],userInfo.plUserChessPalyed[1],
				userInfo.plUserChessPalyed[2],userInfo.plUserChessPalyed[3],userInfo.plUserChessPalyed[4],
				userInfo.plUserChessPalyed[5]);
			pcValue[strlen(pcValue)] = '\0';
			v.set("chessPalyed",(std::string)pcValue);						/// �û�������ÿ������������Ϸ�ľ���

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
	struct type_conversion<mtQueueHall::ProjectInfo>
	{
		typedef values base_type;
		static void from_base(values const & v,indicator & ind,mtQueueHall::ProjectInfo & projectInfo)
		{
			projectInfo.lVendorId = v.get<int>("vid");
			projectInfo.lBatchId = v.get<int>("pid");
			strcpy(projectInfo.pcRoomServer,v.get<std::string>("room_server").c_str());	
			projectInfo.lRoomPort = v.get<int>("room_port");

			ind = i_ok;
		}
		static void to_base(const mtQueueHall::ProjectInfo & projectInfo,values &v,indicator & ind)
		{

		}
	};

	template<>
	struct type_conversion<mtQueueHall::LotteryInfo>
	{
		typedef values base_type;
		static void from_base(values const & v,indicator & ind,mtQueueHall::LotteryInfo & lotteryInfo)
		{
			lotteryInfo.lUserId		 = v.get<int>("userid");
			lotteryInfo.lSumTimes		 = v.get<int>("sumtimes");		/// �齱�ܴ���
			lotteryInfo.lSumGold		 = v.get<int>("sumgold");		/// �黨���ܽ��
			lotteryInfo.lUseTimes		 = v.get<int>("usetimes");		/// ���Գ齱����
			lotteryInfo.lLuck			 = v.get<int>("luck");			/// ����ֵ
			lotteryInfo.lFeeCard         = v.get<int>("feecard");     
			lotteryInfo.tUseTime		 = v.get<std::tm>("usetime");
			//parseDate(lotteryInfo.lUseTime	, v.get<std::tm>("usetime"));		/// �齱��ʱ��

			ind = i_ok;
		}
		static void to_base(const mtQueueHall::LotteryInfo & lotteryInfo,values &v,indicator & ind)
		{
			v.set("userid",(int)lotteryInfo.lUserId);	
			v.set("sumtimes",(int)lotteryInfo.lSumTimes);	
			v.set("sumgold",(int)lotteryInfo.lSumGold);	
			v.set("usetimes",(int)lotteryInfo.lUseTimes);	
			v.set("luck",(int)lotteryInfo.lLuck);
			v.set("feecard",(int)lotteryInfo.lFeeCard); 
			v.set("usetime",lotteryInfo.tUseTime);	
			//std:tm tDate;
			//toDate(lotteryInfo.lUseTime,tDate);
			//v.set("usetime",tDate);	
			
			ind = i_ok;
		}

		static void parseDate(long &lDate, std::tm tDate)
		{
			unsigned short* pusYear = (unsigned short*)&lDate;
			char* pcMonthDate       = (char*)&lDate;
			pusYear[1]                          = tDate.tm_year;
			pcMonthDate[1]                      = tDate.tm_mon;
			pcMonthDate[0]                      = tDate.tm_mday;
		}
		static void toDate(long lDate, std::tm &tDate)
		{
			unsigned short* pusYear = (unsigned short*)&lDate;
			char* pcMonthDate       = (char*)&lDate;
			tDate.tm_year						=	pusYear[1];
			tDate.tm_mon						=	pcMonthDate[1];
			tDate.tm_mday						=   pcMonthDate[0];
		}
	};

	template<>
	struct type_conversion<mtQueueHall::PrizeInfo>
	{
		typedef values base_type;
		static void from_base(values const & v,indicator & ind,mtQueueHall::PrizeInfo & prizeInfo)
		{
			prizeInfo.lNeedGold		= v.get<int>("needgold");
			prizeInfo.lMaxTimes		= v.get<int>("maxtimes");		/// �齱�ܴ���
			prizeInfo.lVipTimes		= v.get<int>("viptimes");
			prizeInfo.lLuckPrize		= v.get<int>("luckprize");		/// �黨���ܽ��
			prizeInfo.lProbability		= v.get<int>("probability");	/// ���Գ齱����
			prizeInfo.lPrize			= v.get<int>("prize");			/// ����ֵ

			ind = i_ok;
		}
		static void to_base(const mtQueueHall::LotteryInfo & lotteryInfo,values &v,indicator & ind)
		{	
		}

	};

	template<>
	struct type_conversion<mtQueueHall::AwardInfo >
	{
		typedef values base_type;
		static void from_base(values const & v,indicator & ind,mtQueueHall::AwardInfo  & awardInfo)
		{
			awardInfo.lUserid		= v.get<int>("userid");
			strcpy(awardInfo.pcPhone,v.get<std::string>("phone","").c_str());        
			awardInfo.lPrize		 = v.get<int>("prize");		
			awardInfo.iIsUse		 = v.get<int>("isuse");		

			ind = i_ok;
		}
		static void to_base(const mtQueueHall::AwardInfo  & awardInfo,values &v,indicator & ind)
		{
			v.set("userid",(int)awardInfo.lUserid);
			v.set("phone",(std::string)awardInfo.pcPhone);
			v.set("prize",(int)awardInfo.lPrize);		
			v.set("isuse",(int)awardInfo.iIsUse);	

			ind = i_ok;
		}

	};
	
	template<>
	struct type_conversion<mtQueueHall::PigInfo>
	{
		typedef values base_type;
		static void from_base(values const & v,indicator & ind,mtQueueHall::PigInfo & pigInfo)
		{
			pigInfo.lUserId		 = v.get<int>("userid");
			pigInfo.lLevel			 = v.get<int>("level");		
			pigInfo.lexperience	 = v.get<int>("experience");
			pigInfo.lMash			 = v.get<int>("mash");		
			pigInfo.tUseTime		 = v.get<std::tm>("usetime");

			ind = i_ok;
		}
		static void to_base(const mtQueueHall::PigInfo & pigInfo,values &v,indicator & ind)
		{
			v.set("userid",(int)pigInfo.lUserId);	
			v.set("level",(int)pigInfo.lLevel);	
			v.set("experience",(int)pigInfo.lexperience);	
			v.set("mash",(int)pigInfo.lMash);	
			v.set("usetime",pigInfo.tUseTime);	

			ind = i_ok;
		}
	};

	template<>
	struct type_conversion<mtQueueHall::UserPKInfo>
	{
		typedef values base_type;
		static void from_base(values const & v,indicator & ind,mtQueueHall::UserPKInfo & userPKInfo)
		{
			userPKInfo.lUserId			 = v.get<int>("userid");
			userPKInfo.lRoomId			 = v.get<int>("roomid");
			strcpy(userPKInfo.pcNickName,v.get<std::string>("nickname").c_str());
			userPKInfo.lChip			 = v.get<int>("chip");		
			userPKInfo.lScore			 = v.get<int>("score");
			strcpy(userPKInfo.pcMobile,v.get<std::string>("mobile").c_str());
			userPKInfo.lStatus			 = v.get<int>("status");

			ind = i_ok;
		}
		static void to_base(const mtQueueHall::UserPKInfo & userPKInfo,values &v,indicator & ind)
		{
			v.set("userid",(int)userPKInfo.lUserId);	
			v.set("roomid",(int)userPKInfo.lRoomId);	
			v.set("nickname",(std::string)userPKInfo.pcNickName);
			v.set("chip",(int)userPKInfo.lChip);	
			v.set("score",(int)userPKInfo.lScore);	
			v.set("mobile",(std::string)userPKInfo.pcMobile);	
			v.set("status",(int)userPKInfo.lStatus);	

			ind = i_ok;
		}
	};

	/*
	template<>
	struct type_conversion<mtQueueHall::TaskInfo >
	{
		typedef values base_type;
		static void from_base(values const & v,indicator & ind,mtQueueHall::TaskInfo  & taskInfo)
		{
			strcpy(taskInfo.pcTaskName,v.get<std::string>("taskname").c_str());	
			taskInfo.ldemand		 = v.get<int>("demand");		
			taskInfo.awardgold		 = v.get<int>("awardgold");		
			taskInfo.cType			 = v.get<int>("type");	

			ind = i_ok;
		}
		static void to_base(const mtQueueHall::TaskInfo  & taskInfo,values &v,indicator & ind)
		{	
		}

	};*/
}

int mtSQLEnv::init(/*DataInit* pkDataInit*/)
{
	iPoolSize = g_DataBasePoolSize;

	MT_EXCEPTION_DEBUG(NULL == (m_pkDBPool = new connection_pool(iPoolSize)), EXCEPTION_DB_POOL_NEW);

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
		MT_DEBUG_COLOR(PTCL,FOREGROUND_GREEN, "init DBPool [%d] success!\n",(iConnectIdx));
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from Connect DB",e.what());
		errorMsg = e.what();
		MT_EXCEPTION_DEBUG(1, EXCEPTION_DB_CONNECT_NEW);
	}

	return 1;
}

int	mtSQLEnv::saveHallFeeBack(mtServiceHallFeedBack::DataRead* pkDataRead)
{
	try
	{
		session sql(*m_pkDBPool);
		sql << "insert into feedback(userid,content,create_at) values(:userid,:content,now())",
			use((pkDataRead->lUserId+MT_SERVER_WORK_USER_ID_BASE)),use((string)pkDataRead->pcContentFeedBack);
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from saveHallFeeBack",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

int	mtSQLEnv::saveOnlinePerson(mtQueueHall::DataHall* m_kDataHall)
{
	try
	{
		session sql(*m_pkDBPool);
		sql << "insert into onlineperson(roomId,online,create_at) values(1,:online1,now()),"\
			"(2,:online2,now()),(3,:online3,now()),(4,:online4,now()),"\
			"(5,:online5,now()),(6,:online6,now());",use(m_kDataHall->pkSpace->pkRoom[0].lPersonNumber),
			use(m_kDataHall->pkSpace->pkRoom[1].lPersonNumber),use(m_kDataHall->pkSpace->pkRoom[2].lPersonNumber),
			use(m_kDataHall->pkSpace->pkRoom[3].lPersonNumber),use(m_kDataHall->pkSpace->pkRoom[4].lPersonNumber),
			use(m_kDataHall->pkSpace->pkRoom[5].lPersonNumber);
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from saveOnlinePerson",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

int	mtSQLEnv::savePayInfo(long userId,long point,long fee)
{
	try
	{
		session sql(*m_pkDBPool);
		sql << "insert into payinfo(userid,point,fee,create_at) values(:userid,:point,:fee,now())",
			use(userId),use(point),use(fee);
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_PTCL("\nSQLERROR: [%s] from savePayInfo [userid:%d][point:%d][fee:%d]",e.what(),
			userId, point, fee);
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

//��ȡ�齱����
int	mtSQLEnv::getPrizeInfo(mtQueueHall::PrizeInfo* m_kPrizeInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "select * from prizeinfo where id = 1 limit 1",into(*m_kPrizeInfo,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getPrizeInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}
//��ȡ�齱�б�
int	mtSQLEnv::getPrizeInfoList(mtQueueHall::PrizeInfo* m_kPrizeInfoList)
{
	try
	{
		session sql(*m_pkDBPool);
		rowset<row> rs = (sql.prepare << "select needgold,maxtimes,viptimes,luckprize,probability,prize from prizeinfo");
		int prizeInfoIndex = 0;
		for(rowset<row>::const_iterator it=rs.begin();it!=rs.end();++it)
		{
			mtQueueHall::PrizeInfo* m_kPrizeInfo = m_kPrizeInfoList+prizeInfoIndex;
			row const& row = *it;
			m_kPrizeInfo->lNeedGold = row.get<int>("needgold");
			m_kPrizeInfo->lMaxTimes = row.get<int>("maxtimes");
			m_kPrizeInfo->lVipTimes = row.get<int>("viptimes");
			m_kPrizeInfo->lLuckPrize = row.get<int>("luckprize");
			m_kPrizeInfo->lProbability = row.get<int>("probability");
			m_kPrizeInfo->lPrize = row.get<int>("prize");
			prizeInfoIndex++;
		}
		if(prizeInfoIndex == 0)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getPrizeInfoList",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

//��ȡ�û�������Ϣ
int	mtSQLEnv::getUserPropInfoList(long userId,mtQueueHall::UseTimeInfo* m_kUserPropInfoList,long* size)
{
	try
	{
		session sql(*m_pkDBPool);
		rowset<row> rs = (sql.prepare << "select propid,stoptime from packageinfo where userid = :userid",use(userId));
		int userPropInfoIndex = 0;
		for(rowset<row>::const_iterator it=rs.begin();it!=rs.end();++it)
		{
			mtQueueHall::UseTimeInfo* m_kUserPropInfo = m_kUserPropInfoList + userPropInfoIndex;
			row const& row = *it;
			m_kUserPropInfo->lId = row.get<int>("propid");
			m_kUserPropInfo->tUseTime = row.get<std::tm>("stoptime");
			userPropInfoIndex++;
		}
		*size = userPropInfoIndex;
		if(userPropInfoIndex == 0)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getUserPropInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

int	mtSQLEnv::getUserPropInfo(long userId,long propId,mtQueueHall::UseTimeInfo* pkUserTimeInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		std::string content_s;
		sql << "select propid,stoptime from packageinfo where userid = :userid and propid=:propid",use(userId),use(propId)
			,into(pkUserTimeInfo->lId),into(pkUserTimeInfo->tUseTime,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getUserPropInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}
int	mtSQLEnv::saveUserPropInfo(long userId,long propId,std::tm stopTime)
{
	try
	{
		session sql(*m_pkDBPool);

		sql << "insert into packageinfo(userid,propid,stoptime,create_at) values(:userid,:propid,:stoptime,now())",
				use(userId),use(propId),use(stopTime);
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from saveUserPropInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}
int	mtSQLEnv::updateUserPropInfo(long userId,long propId,std::tm stopTime)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "update packageinfo set stoptime=:stoptime where userid = :userid and propid=:propid",
			use(stopTime),use(userId),use(propId,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from updateUserPropInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

//��ȡָ����������Ϣ
int	mtSQLEnv::getTaskInfo(long taskId,mtQueueHall::TaskInfo* m_kTaskInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		rowset<row> rs = (sql.prepare << "select taskname,demand,awardgold,type from taskinfo where id=:id",use(taskId));
		int taskInfoIndex = 0;
		for(rowset<row>::const_iterator it=rs.begin();it!=rs.end();++it)
		{
			row const& row = *it;
			strcpy(m_kTaskInfo->pcTaskName,row.get<std::string>("taskname").c_str());	
			m_kTaskInfo->ldemand = row.get<int>(1);
			m_kTaskInfo->awardgold = row.get<int>(2);
			m_kTaskInfo->cType = row.get<int>(3);
			taskInfoIndex++;
		}
		if(taskInfoIndex == 0)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getTaskInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}
//��ȡ�����б�
int	mtSQLEnv::getTaskInfoList(mtQueueHall::TaskInfo* m_kTaskInfoList)
{
	try
	{
		session sql(*m_pkDBPool);
		rowset<row> rs = (sql.prepare << "select taskname,demand,awardgold,type from taskinfo");
		int taskInfoIndex = 0;
		for(rowset<row>::const_iterator it=rs.begin();it!=rs.end();++it)
		{
			mtQueueHall::TaskInfo* m_kTaskInfo = m_kTaskInfoList+taskInfoIndex;
			row const& row = *it;
			strcpy(m_kTaskInfo->pcTaskName,row.get<std::string>("taskname").c_str());	
			m_kTaskInfo->ldemand = row.get<int>(1);
			m_kTaskInfo->awardgold = row.get<int>(2);
			m_kTaskInfo->cType = row.get<int>(3);
			taskInfoIndex++;
		}
		if(taskInfoIndex == 0)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getTaskInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

int	mtSQLEnv::getUserTaskPrizeInfoList(long userId,mtQueueHall::UseTimeInfo* m_kTaskPrizeInfoList,long* size)
{
	try
	{
		session sql(*m_pkDBPool);
		rowset<row> rs = (sql.prepare << "select taskid,usertime from taskprizeinfo where userid = :userid",use(userId));
		int taskPrizeInfoIndex = 0;
		for(rowset<row>::const_iterator it=rs.begin();it!=rs.end();++it)
		{
			mtQueueHall::UseTimeInfo* m_kTaskPrizeInfo = m_kTaskPrizeInfoList + taskPrizeInfoIndex;
			row const& row = *it;
			m_kTaskPrizeInfo->lId = row.get<int>("taskid");
			m_kTaskPrizeInfo->tUseTime = row.get<std::tm>("usertime");
			taskPrizeInfoIndex++;
		}
		*size = taskPrizeInfoIndex;
		if(taskPrizeInfoIndex == 0)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getUserTaskPrizeInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

//��ȡ�����콱��Ϣ
int	mtSQLEnv::getUserTaskPrizeInfo(long userId,long taskId,mtQueueHall::UseTimeInfo* m_kTaskPrizeInfo)	
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "select taskid,usertime from taskprizeinfo where userid = :userid and taskid=:taskid",use(userId),use(taskId)
			,into(m_kTaskPrizeInfo->lId),into(m_kTaskPrizeInfo->tUseTime,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getUserTaskPrizeInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

//���������콱��Ϣ
int	mtSQLEnv::saveUserTaskPrizeInfo(long userId,long taskId)
{
	try
	{
		session sql(*m_pkDBPool);
		sql << "insert into taskprizeinfo(userid,taskid,usertime,create_at) values(:userid,:taskid,now(),now())",
			use(userId),use(taskId);
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from saveUserTaskPrizeInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

//���������콱��Ϣ
int	mtSQLEnv::updateUserTaskPrizeInfo(long userId,long taskId)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "update taskprizeinfo set usertime=now() where userid = :userid and taskid=:taskid",use(userId),use(taskId,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from updateUserTaskPrizeInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

//��������Ϣ��ȡ
int mtSQLEnv::getPigInfo(long userId,mtQueueHall::PigInfo* m_kPigInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "select * from piginfo where userid = :userid limit 1",use(userId),into(*m_kPigInfo,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getPigInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}
//��������Ϣ�洢
int	mtSQLEnv::savePigInfo(mtQueueHall::PigInfo* m_kPigInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "insert into piginfo(userid,level,experience,mash,usetime,create_at) " \
			"values(:userid,:level,:experience,:mash,:usetime,now())",use(*m_kPigInfo,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from savePigInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}
//��������Ϣ����
int	mtSQLEnv::updatePigInfo(mtQueueHall::PigInfo* m_kPigInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "update piginfo set level=:level,experience=:experience,mash=:mash," \
			"usetime=:usetime where userid = :userid",use(*m_kPigInfo,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from updatePigInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}


//�û��齱��Ϣ��ȡ
int	mtSQLEnv::getLotteryinfo(long userId,mtQueueHall::LotteryInfo* m_kLotteryInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "select * from lotteryinfo where userid = :userid limit 1",use(userId),into(*m_kLotteryInfo,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getLotteryinfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}
//�û��齱��Ϣ�洢
int	mtSQLEnv::saveLotteryinfo(mtQueueHall::LotteryInfo* m_kLotteryInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "insert into lotteryinfo(userid,sumtimes,sumgold,usetimes,luck,feecard,usetime,create_at) " \
			"values(:userid,:sumtimes,:sumgold,:usetimes,:luck,:feecard,:usetime,now())",use(*m_kLotteryInfo,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from saveLotteryinfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}
//�û��齱��Ϣ����
int	mtSQLEnv::updateLotteryinfo(mtQueueHall::LotteryInfo* m_kLotteryInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "update lotteryinfo set sumtimes=:sumtimes,sumgold=:sumgold,usetimes=:usetimes," \
			"luck=:luck,feecard=:feecard,usetime=:usetime where userid = :userid",use(*m_kLotteryInfo,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from updateLotteryinfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

//�ý���Ϣ����
int	mtSQLEnv::saveAwardInfo(mtQueueHall::AwardInfo* m_kAwardInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "insert into awardinfo(userid,phone,prize,isuse,create_at) " \
			"values(:userid,:phone,:prize,:isuse,now())",use(*m_kAwardInfo,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from saveAwardInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

//��ȡϵͳ��Ϣ
int	mtSQLEnv::getLastNoticeInfo(char* content)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		std::string content_s;
		sql << "select content from noticeinfo where status = 1 group by id desc limit 1",into(content_s,ind);
		strcpy(content , content_s.c_str());

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getLastNoticeInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

int	mtSQLEnv::saveSystemMessage(char* content)
{
	try
	{
		session sql(*m_pkDBPool);
		sql << "insert into messageinfo(content,status,create_at) values(:content,1,now())",
			use((string)content);
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from saveSystemMessage",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}
//��ȡϵͳ��Ϣ
int	mtSQLEnv::getSystemMessageList(mtQueueHall::MessageInfo* m_kMessageInfo)
{
	int messageInfoIndex = 0;
	try
	{
		session sql(*m_pkDBPool);
		rowset<row> rs = (sql.prepare << "select content from messageinfo where status = 1 group by id desc limit 10");
		
		for(rowset<row>::const_iterator it=rs.begin();it!=rs.end();++it)
		{
			row const& row = *it;
			strcpy(m_kMessageInfo->pkMessage[messageInfoIndex].content , row.get<string>(0).c_str());
			messageInfoIndex++;
		}
		if(messageInfoIndex == 0)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getSystemMessage",e.what());
		errorMsg = e.what();
		return 0;
	}

	return messageInfoIndex;
}

//��ȡ�û���Ϣ
int	mtSQLEnv::getUserInfo(long userId,mtQueueHall::UserInfo* pkUserInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "select * from userinfo where id = :id limit 1",use(userId),into(*pkUserInfo,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getUserInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}
//�����û���Ϣ
int mtSQLEnv::saveUserInfo(mtQueueHall::UserInfo* pkUserInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "insert into userinfo(id,iSceneId,iconId,accountNum,phone,qq,pwd,name,nickName,love,remark,sex,age,gold,score,level," \
			"payCount,firstCharge,dayChess,allChess,winChess,winRate,offLineCount,registerDate,loginHallDate,continueLoginDays,loginFirstToday," \
			"giveGoldFrequence,chessPalyed) values(:id,:iSceneId,:iconId,:accountNum,:phone,:qq,:pwd,:name,:nickName,:love,"  \
			":remark,:sex,:age,:gold,:score,:level,:payCount,:firstCharge,:dayChess,:allChess,:winChess,:winRate,:offLineCount,now(),now()," \
			":continueLoginDays,:loginFirstToday,:giveGoldFrequence,:chessPalyed)",use(*pkUserInfo,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from saveUserInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}
//�����û���Ϣ
int mtSQLEnv::updateUserInfo(mtQueueHall::UserInfo* pkUserInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "update userinfo set sceneId =:sceneId,iconId=:iconId,accountNum=:accountNum,phone=:phone,qq=:qq,name=:name," \
			"nickName=:nickName,love=:love,remark=:remark,sex=:sex,age=:age,gold=:gold,score=:score,level=:level,"\
			"payCount=:payCount,firstCharge=:firstCharge,dayChess=:dayChess,allChess=:allChess," \
			"winChess=:winChess,winRate=:winRate,offLineCount=:offLineCount,loginHallDate=:loginHallDate," \
			"continueLoginDays=:continueLoginDays,loginFirstToday=:loginFirstToday," \
			"chessPalyed=:chessPalyed,giveGoldFrequence=:giveGoldFrequence where id = :id",use(*pkUserInfo,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from updateUserInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

int	mtSQLEnv::getProject(long vendorId,long patchId,mtQueueHall::ProjectInfo* pkProjectInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "select * from project where vid = :vendorId and pid = :patchId limit 1",
			use(vendorId),use(patchId),into(*pkProjectInfo,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getProject",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

int	mtSQLEnv::getPKRoomList(mtQueueHall::PKRoomInfo* pkPKRoomInfoList,long size)
{
	try
	{
		session sql(*m_pkDBPool);
		rowset<row> rs = (sql.prepare << "select * from matchinfo where status = 1 limit :size",use(size));
		int pkRoomInfoIndex = 0;
		for(rowset<row>::const_iterator it=rs.begin();it!=rs.end();++it)
		{
			mtQueueHall::PKRoomInfo* pkPKRoomInfo = pkPKRoomInfoList + pkRoomInfoIndex;
			row const& row = *it;
			pkPKRoomInfo->lRoomId = row.get<int>("id");
			strcpy(pkPKRoomInfo->pcRoomName, row.get<string>("name").c_str());
			strcpy(pkPKRoomInfo->pcRoomlP, row.get<string>("server").c_str());
			pkPKRoomInfo->lRoomPort = row.get<int>("port");
			pkPKRoomInfo->lante = row.get<int>("ante");
			pkPKRoomInfo->llowestchip  = row.get<int>("lowestchip");
			strcpy(pkPKRoomInfo->pcRoomImg, row.get<string>("img").c_str());
			pkPKRoomInfo->lStartTime = row.get<std::tm>("starttime");
			pkPKRoomInfo->lActiveTime = row.get<int>("activetime");
			pkPKRoomInfo->lEndTime = row.get<std::tm>("endtime");
			pkPKRoomInfo->lFee = row.get<int>("fee");
			pkPKRoomInfo->lFeeType = row.get<int>("feetype");
			pkPKRoomInfo->lInitChip = row.get<int>("initchip");
			strcpy(pkPKRoomInfo->pcPrizeName, row.get<string>("prizename").c_str());
			pkPKRoomInfo->lAwardNum = row.get<int>("awardnum");
			strcpy(pkPKRoomInfo->pcAwardcontent, row.get<string>("awardcontent").c_str());
			pkRoomInfoIndex++;
		}
		if(pkRoomInfoIndex == 0)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getPKRoom",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

int	mtSQLEnv::getPKRoom(long roomId,mtQueueHall::PKRoomInfo* pkPKRoomInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		rowset<row> rs = (sql.prepare << "select * from matchinfo where id = :roomId limit 1",use(roomId));
		int pkRoomInfoIndex = 0;
		for(rowset<row>::const_iterator it=rs.begin();it!=rs.end();++it)
		{
			row const& row = *it;
			pkPKRoomInfo->lRoomId = row.get<int>("id");
			strcpy(pkPKRoomInfo->pcRoomName, row.get<string>("name").c_str());
			strcpy(pkPKRoomInfo->pcRoomlP, row.get<string>("server").c_str());
			pkPKRoomInfo->lRoomPort = row.get<int>("port");
			pkPKRoomInfo->lante = row.get<int>("ante");
			pkPKRoomInfo->llowestchip  = row.get<int>("lowestchip");
			strcpy(pkPKRoomInfo->pcRoomImg, row.get<string>("img").c_str());
			pkPKRoomInfo->lStartTime = row.get<std::tm>("starttime");
			pkPKRoomInfo->lActiveTime = row.get<int>("activetime");
			pkPKRoomInfo->lEndTime = row.get<std::tm>("endtime");
			pkPKRoomInfo->lFee = row.get<int>("fee");
			pkPKRoomInfo->lFeeType = row.get<int>("feetype");
			pkPKRoomInfo->lInitChip = row.get<int>("initchip");
			strcpy(pkPKRoomInfo->pcPrizeName, row.get<string>("prizename").c_str());
			pkPKRoomInfo->lAwardNum = row.get<int>("awardnum");
			strcpy(pkPKRoomInfo->pcAwardcontent, row.get<string>("awardcontent").c_str());
			pkRoomInfoIndex++;
		}
		if(pkRoomInfoIndex == 0)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getPKRoom",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

int	mtSQLEnv::getUserPKInfo(long userId,long roomId,mtQueueHall::UserPKInfo* pkUserPKInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "select * from userpkinfo where userid=:userid and roomid=:roomid limit 1",
			use(userId),use(roomId),into(*pkUserPKInfo,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getUserPKInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

int	mtSQLEnv::saveUserPKInfo(mtQueueHall::UserPKInfo* pkUserPKInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "insert into userpkinfo(userid,roomid,nickname,chip,score,mobile,status,create_at) "\
			"values(:userid,:roomid,:nickname,:chip,:score,:mobile,:status,now())",
			use(*pkUserPKInfo,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from saveUserPKInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

int	mtSQLEnv::updateUserPKInfo(mtQueueHall::UserPKInfo* pkUserPKInfo)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		sql << "update userpkinfo set chip=:chip,score=:score,mobile=:mobile,status=:status where userid =:userid and roomid=:roomid",
			use(*pkUserPKInfo,ind);

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from updateUserPKInfo",e.what());
		errorMsg = e.what();
		return 0;
	}
}

int	mtSQLEnv::getPKRoomRankList(long roomId,mtQueueHall::DataRank* pkPKRoomRankList,long* size)
{
	try
	{
		session sql(*m_pkDBPool);
		rowset<row> rs = (sql.prepare << "select userid,nickname,score from userpkinfo where roomid = :roomid order by score desc limit 10",use(roomId));
		int rankInfoIndex = 0;
		for(rowset<row>::const_iterator it=rs.begin();it!=rs.end();++it)
		{
			mtQueueHall::DataRank* pkPKRoomRankInfo = pkPKRoomRankList + rankInfoIndex;
			row const& row = *it;
			pkPKRoomRankInfo->lUserId = row.get<int>("userid");
			strcpy(pkPKRoomRankInfo->pcUserNickName,row.get<std::string>("nickname").c_str());
			pkPKRoomRankInfo->lUserScore = row.get<int>("score");
			rankInfoIndex++;
		}
		*size = rankInfoIndex;
		if(rankInfoIndex == 0)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getPKRoomRankList",e.what());
		errorMsg = e.what();
		return 0;
	}

	return 1;
}

int	mtSQLEnv::getPKUserRankInfo(long userId,long roomId,mtQueueHall::DataRank* pkPKRoomRank)
{
	try
	{
		session sql(*m_pkDBPool);
		indicator ind = i_truncated;
		std::string content_s;
		sql << "SELECT b.userid,b.nickname,count(*)+1 as rank,b.score as score FROM userpkinfo a,"				\
			"(select userid,nickname,score from userpkinfo where userid=:userid and roomid=:roomid) b where a.roomid=:roomid and a.score>b.score"
			,use(userId),use(roomId),use(roomId)
			,into(pkPKRoomRank->lUserId),into(content_s),into(pkPKRoomRank->lUserLevel),into(pkPKRoomRank->lUserScore,ind);
		strcpy(pkPKRoomRank->pcUserNickName,content_s.c_str());

		if(ind != i_ok)
			return 0;
	}
	catch (std::exception const &e)
	{
		MT_DEBUG_ERROR(PTCL,"\nSQLERROR: [%s] from getPKUserRankInfo",e.what());
		errorMsg = e.what();
		return 0;
	}

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
		MT_DEBUG_ERROR(PTCL,"���쳣û�б�����");
		return	0;
	}

	return	1;
}

int mtSQLEnv::run()
{
	return	1;
}