#ifndef		__MT_QUEUE_HALL_H
#define		__MT_QUEUE_HALL_H
#include "mtPlatform.h"
#include "mtDSQueue.h"
class  mtSQLEnv;
class mtQueueHall
{
public:
	enum
	{
		E_HALL_USER_STATUS_BEG           = 0,
		E_HALL_USER_STATUS_OFFLINE       = E_HALL_USER_STATUS_BEG, /// ����
		E_HALL_USER_STATUS_ONLINE,                                 /// ����(�ڴ������߷���)
		E_HALL_USER_STATUS_HALL,                                   /// ��ǰ�ڴ���(����)
		E_HALL_USER_STATUS_ROOM,                                   /// ��ǰ��ĳ����������(����)
		E_HALL_USER_STATUS_ROOM_FORCE_EXIT,                        /// ��ǰ��ĳ����������(����,�û�����Ϸ�Ѿ���ʼʱ��ǿ���˳�)
		E_HALL_USER_STATUS_ROOM_NETWORK_EXIT,                      /// ��ǰ��ĳ����������(����,�û�����Ϸ�Ѿ���ʼʱ����������ԭ���˳�)

		E_HALL_USER_STATUS_END
	};
	enum MatchRoomStatus
	{
		OPEN_SOON   = 0,
		IS_OPEN     = 1,
		HAS_ENDED   = -1,
	};
	enum MatchRoomCurrentStatus
	{
		OPEN   = 1,
		CLOSE  = 0
	};
	enum UserRankStatus
	{
		NOT_END     = -1,
		NO_WINNERS  = 0,
		NO_AWARD    = 1,
		HAS_AWARD   = -2
	};
	struct PKRoomInfo	//��������������
	{

		long                            lRoomId;                /// ����id
		char                            pcRoomName[32];         /// ��������
		char							pcRoomlP[15];			/// �����������IP
		long 							lRoomPort;				/// ����������Ķ˿�
		long                            lante;					/// ����׷�(����)
		long                            llowestchip;			/// ����׼���(����)
		char							pcRoomImg[80];			//����ͼƬ
		std::tm							lStartTime;				//��ʼʱ��
		long							lActiveTime;			//�ʱ��	
		std::tm							lEndTime;				//����ʱ��
		long							lFee;					//��������
		long							lFeeType;				//��������һ��������
		long							lInitChip;				//��ʼ����
		char							pcPrizeName[20];		//��Ʒ
		long							lAwardNum;				//��������
		char							pcAwardcontent[30];		//��������
		//long							lActiveStatus;			//(�������š������С������)
		//long							lCurStatus;				//�Ƿ���
	};

	struct UserPKInfo		//�û���������
	{
		long							lUserId;			//�û�ID
		long							lRoomId;			//����������ID
		char							pcNickName[12];     //�ǳ�
		long							lChip;				//����
		long							lScore;				//����
		char							pcMobile[12];		//��ϵ�绰
		long							lStatus;			//�ý�״̬��δ�ܽ�,δ�ý�,δ�콱,���콱
	};

	struct MatchRoom
	{
		long 				lStructBytes;		/// ����С
		long                lRoomId;            /// ����id
		char                pcRoomName[32];     /// ��������
		unsigned long		ulRoomlP;		    /// �����������IP
		long 				lRoomPort;		    /// ����������Ķ˿�
		long                lPersonNumber;      /// ���䵱ǰ����
		long                lGoldBase;          /// ����׷�(���)
		long                lGoldAdmittance;    /// ����׼���(���)
		long                lGoldDeduct;        /// 0ÿ�ֿ۳���(���)��
		char				pcRoomImg[80];		/// ����ͼƬ
		long				lActiveTime[3];		/// �ʱ��						
		long				lFee;			    /// ��������
		long				lFeeType;		    /// ��������һ��������
		long                lIntiChip;          //��ʼ����
		char				pcPrizeName[20];	/// ��Ʒ
		char				pcAwardcontent[30];	/// ��������
		long				lcurstatus;		    /// �Ƿ���(1������0�ر�)
		long				lstatus;		    /// (0�������š�1�����С�-1�����)
	};


	struct DataRoom
	{
		long 							lStructBytes;			/// ����С
		long                            lSpaceId;               /// ��id
		long                            lRoomId;                /// ����id
		char                            pcRoomName[32];         /// ��������
		unsigned long 					ulRoomlP;			    /// �����������IP
		long 					        lRoomPort;		        /// ����������Ķ˿�
		long                            lPersonNumber;          /// ���䵱ǰ����
		long                            lGoldBase;              /// ����׷�(���)
		long                            lGoldAdmittance;        /// ����׼���(���)
		long                            lGoldDeduct;            /// ÿ�ֿ۳���(���)��
	};

	struct DataSpace
	{
		long 							lStructBytes;			/// ����С
		long                            lSpaceId;               /// ��id
		char                            pcSpaceName[32];        /// ������
		DataRoom					    pkRoom[6];
	};

	struct UserInfo
	{
		long                            lUserId;                /// �û�id	0 ��ʾʧ��
		//long							lBatchId;				/// ���κ�id
		//long							lVendorId;				/// ���̴���
		long                            lUserIconId;            /// �û�ͷ��id
		long                            lUserSceneId;               ///����ID
		char                            pcUserAccountNum[12];   /// �û��˺�
		char                            pcUserName[12];         /// �û�����
		char                            pcUserNickName[12];     /// �û��ǳ�
		char                            pcUserPwd[12];          /// �û�����
		char                            pcUserLove[32];         /// ��Ȥ����
		char                            pcUserRemark[64];       /// ��ע˵��
		char                            pcUserPhone[12];        /// �ֻ�����
		char                            pcUserQQ[12];           /// QQ����
		long                            lUserSex;               /// �Ա� (0--�У�1--Ů)
		long                            lUserAge;               /// ����
		long                            lUserGold;              /// �û�ӵ�н����
		long 							lUserScore;				/// �û�����
		long                            lUserLevel;             /// �û��ȼ�
		long							lUserPayCount;				/// �û��ܳ�ֵ
		long							lUserFirstCharge;			/// �û��׳佱�����	
		long							lUserDayChess;				/// ���վ���
		long                            lUserAllChess;          /// �ܾ���
		long                            lUserWinChess;          /// ʤ����
		long                            lUserWinRate;           /// ʤ��
		long                            lUserOffLineCount;      /// ���ߴ���
		//long                            lIRegister;             /// �Ƿ��Ѿ�ע��(0 -δע�ᣬ1 -��ע��)��δע���¸��ֶ�lUserRegisterDate��ʾ�״ο��ٵ�¼��ʱ��
		long                            lUserRegisterDate;      /// ע�����ڸ�2�ֽڱ�ʾ�꣬�ε��ֽڱ�ʾ�£����ֽڱ�ʾ��(XXXX-XX-XX)
		long                            lUserLoginHallDate;     /// �û����һ�ε�¼������(XXXX-XX-XX)
		long                            lUserContinueLoginDays; /// �û�������¼������
		long                            lUserLoginFirstToday;   /// �Ƿ��ǽ���ĵ�һ�ε�¼(0 -���ǽ����һ�Σ� 1 -�ǽ����һ��)
		long                            plPropsCount[16];       /// �û�������Ŀ
		long                            plUserChessPalyed[6];   /// �û�������ÿ������������Ϸ�ľ���
		long                            lUserGiveGoldFrequence; /// ÿ�����͸��û������Ĵ���
	};

	struct ProjectInfo
	{
		long            lVendorId;				/// ���̴���
		long            lBatchId;				/// ���κ�id
		char			pcRoomServer[15];			/// �����ַ
		long			lRoomPort;					/// ����˿�
	};

	struct Message
	{
		char		content[80];
	};

	struct MessageInfo			//ϵͳ��Ϣ
	{
		Message		pkMessage[10];
	};

	struct TaskInfo
	{
		char							pcTaskName[20];		//��������
		long							ldemand;			//Ҫ��
		long							awardgold;			//�������
		char							cType;				//���� 1Ϊ���ն�ս����,2Ϊ�ܶ�ս����,3Ϊ��ʤ����,4Ϊ�ܳ�ֵ��
		char							cIsUse;				//�Ƿ�����ȡ1����ȡ��0δ��ȡ
	};

	struct UseTimeInfo
	{
		long							lId;					///	ID
		std::tm							tUseTime;				/// ʱ��
	};

	struct PigInfo		//������
	{
		long                            lUserId; 
		long							lLevel;					/// �ȼ�
		long							lexperience;			/// ����
		long							lMash;					/// ʣ������
		std::tm							tUseTime;				/// �½��ʱ��
	};

	struct PrizeInfo			//��Ʒ��Ϣ
	{
		long                            lNeedGold;				/// �齱������
		long                            lMaxTimes;				///	���齱����
		long							lVipTimes;				/// Vip���ĳ齱����
		long                            lLuckPrize;				/// ����ֵ����Ľ�Ʒ
		long                            lProbability;			/// �齱��ֵ
		long                            lPrize;					/// ��Ʒ���
	};

	struct LotteryInfo			//�û��齱��Ϣ
	{
		long                            lUserId; 
		long							lSumTimes;				/// �齱�ܴ���
		long							lSumGold;				/// �黨���ܽ��
		long							lUseTimes;				/// ���Գ齱����
		long							lLuck;					/// ����ֵ
		long                            lFeeCard;               ///����
		std::tm							tUseTime;				/// �齱��ʱ��
	};

	struct AwardInfo			//�ý���Ϣ
	{
		long                            lUserid;
		char                            pcPhone[MT_BYTES_OF_PHONE]; 
		long                            lPrize;					///	�鵽�Ľ�Ʒ
		long                            iIsUse;					/// �Ƿ���ȡ
	};

	struct UserDataNode
	{
		long							lStructBytes;
		long                            lIsOnLine;              /// �û�����״̬
		long                            lSpaceId;               /// ��id
		long                            lRoomId;                /// ����id
		long                            lUserGoldBuy;           /// �û�����Ľ����
		long                            lUserGoldBuyStatus;     /// �û������ҵĽ��״̬(0 -Ϊ���£�����û�յ��շѷ�������״̬���£�1 -�Ѹ��£����յ��շѷ�������״̬����)
		UserInfo                        kUserInfo;              /// �û���Ϣ
	};

	struct DataRank
	{
		long                            lUserId;                /// �û�id
		long                            lUserLevel;             /// �û��ȼ�
		long 							lUserScore;				/// �û�����
		char                            pcUserNickName[12];     /// �û��ǳ�
	};

	struct DataHall
	{
		long 							lStructBytes;			     /// ����С
		DataSpace*                      pkSpace;   			         /// ����Ϣ
		CRITICAL_SECTION	            kCriticalSectionDataRank;    /// ���а��ڴ�����ٽ���
		DataRank*                       pkDataRank;                  /// ���а�ǰ10����Ϣ
		UserDataNode*                   pkUserDataNodeList;          /// �û���Ϣ
	};

	struct DataInit
	{
		long		lStructBytes;
		int			iSpinCount;
		char*       pcServiceExeDir;
		mtSQLEnv   *pkSQLEnv;
	};

public:
	mtQueueHall();

	~mtQueueHall();

	int    init(void* pData);
	int    exit(void);

	int    initSpaceRoom();
	int				             m_iSpinCount;
	mtQueueHall::DataHall        m_kDataHall;
	char*                        m_pcServiceExeDir;
	mtSQLEnv               *     m_pkSQLEnv;
	
};

#endif	///	__MT_QUEUE_HALL_H