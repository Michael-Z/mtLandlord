#pragma once
#include "mtServiceConnect.h"
#include "mtServiceHallExit.h"
#include "mtServiceHallFeedBack.h"
#include "mtServiceHallGiveGold.h"
#include "mtServiceHallGoldBuy.h"
#include "mtServiceHallHall2Room.h"
#include "mtServiceHallPropBuy.h"
#include "mtServiceHallRank.h"
#include "mtServiceHallRegister.h"
#include "mtServiceHallRoom2Hall.h"
#include "mtServiceHallUpdatePlayInfo.h"
#include "mtServiceHallUpdateRoom.h"
#include "mtServiceHallUpdateSocialInfo.h"
#include "mtServiceGetLotteryArg.h"
#include "mtServiceLottery.h"
#include "mtServiceSystemMessage.h"

#include "mtServiceGetTaskInfo.h"
#include "mtServiceGetTaskAward.h"
#include "mtServiceGetPetInfo.h"
#include "mtServiceFeedPet.h"
#include "mtServiceSpitGold.h"
#include "mtServiceCashLottery.h"

#include "mtServiceMatchGetRoomInfo.h"
#include "mtServiceMatchGetUserInfo.h"
#include "mtServiceMatchSignUp.h"
#include "mtServiceMatchGetRoomStatus.h"
#include "mtServiceMatchGetRank.h"
#include "mtServiceMatchAward.h"

#include "mtServiceMgr.h"


extern int mtPrint(mtQueueHall::DataRoom* pkDataRoom,int iTab = 0);
extern int mtPrint(mtQueueHall::DataSpace* pkDataSpace,int iTab = 0);
extern int mtPrint(mtQueueHall::UserInfo* pkUserInfo,int iTab = 0);
extern int mtPrint(mtQueueHall::UserDataNode* pkUserDataNode,int iTab = 0);
extern int mtPrint(mtQueueHall* pkmtQueueHall,int iTab = 0);

extern int mtPrint(mtQueueHall::MatchRoom* pkDataRoom,int iTab = 0);

extern int mtPrint(mtQueueHall::DataRank* pkDataRank,int iTab = 0);
extern int mtPrint(mtQueueHall::DataHall* pkDataHall,int iTab = 0);
extern int mtPrint(mtQueueHall::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceConnect::DataRead* pkDataRead,int iTab = 0);  ///0���Ӵ���
extern int mtPrint(mtServiceConnect::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceConnect::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceHallRegister::DataRead* pkDataRead,int iTab = 0);
extern int mtPrint(mtServiceHallRegister::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceHallRegister::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceHallPropBuy::DataRead* pkDataRead,int iTab = 0);  /// 2�������
extern int mtPrint(mtServiceHallPropBuy::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceHallPropBuy::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceHallGoldBuy::DataRead* pkDataRead,int iTab = 0); /// 4������
extern int mtPrint(mtServiceHallGoldBuy::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceHallGoldBuy::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceHallRank::DataRead* pkDataRead,int iTab = 0); /// 6������а�
extern int mtPrint(mtServiceHallRank::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceHallRank::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceHallGiveGold::DataRead* pkDataRead,int iTab = 0); /// 9����
extern int mtPrint(mtServiceHallGiveGold::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceHallGiveGold::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceHallUpdatePlayInfo::DataRead* pkDataRead,int iTab = 0);/// 7�����û����˻���Ϣ(�������а�)
extern int mtPrint(mtServiceHallUpdatePlayInfo::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceHallUpdatePlayInfo::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceHallUpdateSocialInfo::DataRead* pkDataRead,int iTab = 0);/// 8�����û��Ļ���������Ϣ
extern int mtPrint(mtServiceHallUpdateSocialInfo::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceHallUpdateSocialInfo::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceHallUpdateRoom::DataRead* pkDataRead,int iTab = 0); /// 10������Ϣ��ʱ����
extern int mtPrint(mtServiceHallUpdateRoom::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceHallUpdateRoom::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceHallHall2Room::DataRead* pkDataRead,int iTab = 0);/// 11�û�����Ӵ���������
extern int mtPrint(mtServiceHallHall2Room::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceHallHall2Room::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceHallRoom2Hall::DataRead* pkDataRead,int iTab = 0); /// 12�û�����ӷ��䷵�ش���
extern int mtPrint(mtServiceHallRoom2Hall::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceHallRoom2Hall::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceHallFeedBack::DataRead* pkDataRead,int iTab = 0);  /// 13�û���Ϣ����
extern int mtPrint(mtServiceHallFeedBack::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceHallFeedBack::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceHallExit::DataRead* pkDataRead,int iTab = 0); /// 14�˳�����
extern int mtPrint(mtServiceHallExit::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceHallExit::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceGetLotteryArg::DataRead* pkDataRead,int iTab = 0);/// 15ȡ�齱����
extern int mtPrint(mtServiceGetLotteryArg::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceGetLotteryArg::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceLottery::DataRead* pkDataRead,int iTab = 0);/// 16�齱
extern int mtPrint(mtServiceLottery::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceLottery::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceSystemMessage::DataRead* pkDataRead,int iTab = 0); /// 17ϵͳ��Ϣ
extern int mtPrint(mtServiceSystemMessage::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceSystemMessage::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceGetTaskInfo::DataRead* pkDataRead,int iTab = 0);///18��ȡ������Ϣ
extern int mtPrint(mtServiceGetTaskInfo::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceGetTaskInfo::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceGetTaskAward::DataRead* pkDataRead,int iTab = 0);///19��ȡ������
extern int mtPrint(mtServiceGetTaskAward::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceGetTaskAward::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceGetPetInfo::DataRead* pkDataRead,int iTab = 0);  ///20��ȡ����Э��
extern int mtPrint(mtServiceGetPetInfo::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceGetPetInfo::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceFeedPet::DataRead* pkDataRead,int iTab = 0);  ///21ι������
extern int mtPrint(mtServiceFeedPet::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceFeedPet::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceSpitGold::DataRead* pkDataRead,int iTab = 0);  ///22�½��
extern int mtPrint(mtServiceSpitGold::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceSpitGold::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceCashLottery::DataRead* pkDataRead,int iTab = 0);  ///23�ҽ�
extern int mtPrint(mtServiceCashLottery::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceCashLottery::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceMatchGetRoomInfo::DataRead* pkDataRead,int iTab = 0);  ///24��ȡ����������ϢЭ��
extern int mtPrint(mtServiceMatchGetRoomInfo::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceMatchGetRoomInfo::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceMatchGetUserInfo::DataRead* pkDataRead,int iTab = 0);  /// 25��ȡ�û�����������Э��
extern int mtPrint(mtServiceMatchGetUserInfo::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceMatchGetUserInfo::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceMatchSignUp::DataRead* pkDataRead,int iTab = 0);  /// 26��������Э��.
extern int mtPrint(mtServiceMatchSignUp::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceMatchSignUp::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceMatchGetRoomStatus::DataRead* pkDataRead,int iTab = 0);  /// 27��ȡ����״̬Э��
extern int mtPrint(mtServiceMatchGetRoomStatus::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceMatchGetRoomStatus::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceMatchGetRank::DataRead* pkDataRead,int iTab = 0);  /// 28����������ȡЭ��
extern int mtPrint(mtServiceMatchGetRank::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceMatchGetRank::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceMatchAward::DataRead* pkDataRead,int iTab = 0);  /// 29�����콱Э��
extern int mtPrint(mtServiceMatchAward::DataWrite* pkDataWrite,int iTab = 0);
extern int mtPrint(mtServiceMatchAward::DataInit* pkDataInit,int iTab = 0);

extern int mtPrint(mtServiceMgr::DataHead* pkDataRead,int iTab = 0);   
extern int mtPrint(mtServiceMgr::DataInit* pkDataInit,int iTab = 0);
extern int mtPrint(mtServiceMgr::DataRun* pkDataRun,int iTab = 0);



