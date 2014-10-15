#ifndef		__MT_SERVICE_MGR_H
#define		__MT_SERVICE_MGR_H

#include "mtService.h"
#include "mtPlatform.h"
#include "mtQueueUser.h"
#include "mtServiceConnect.h"
#include "mtQueueMgr.h"

class mtServiceMgr
{
public:
	enum EServiceId
	{
		E_SERVICE_ROOM_BEG	= 0,
		E_SERVICE_ROOM_CONNECT		= E_SERVICE_ROOM_BEG,	    /// 0���ӷ��������
		E_SERVICE_ROOM_DESK_TWO,							    /// 1˫����
		E_SERVICE_ROOM_DESK_THREE,							    /// 2������
		E_SERVICE_ROOM_WAIT_START,							    /// 3�ȴ���ʼ
		E_SERVICE_ROOM_CARD_DEAL,							    /// 4����
		E_SERVICE_ROOM_GRAD_LANDLORD,						    /// 5�з�
		E_SERVICE_ROOM_DOUBLE_SCORE,						    /// 6�ӱ�
		E_SERVICE_ROOM_SHOW_CARD,						        /// 7����
		E_SERVICE_ROOM_CARD_OUT,						        /// 8����
		E_SERVICE_ROOM_REQUEST_CARDS_LEFT,                      /// 9�����������������������Ϣ
		E_SERVICE_ROOM_ROUND_OVER,                              /// 10һ�ֽ���(����Ҫ�ϴ�����)
		E_SERVICE_ROOM_CHANGE_DESK,                             /// 11������
		E_SERVICE_ROOM_CHAT_SHORTCUT,						    /// 12�����
		E_SERVICE_ROOM_CHAT_ICON,							    /// 13����
		E_SERVICE_ROOM_CHAT_CONTEXT,						    /// 14������������
		E_SERVICE_ROOM_HEART_BEAT,                              /// 15��������������
		E_SERVICE_ROOM_EXIT,								    /// 16�˳�����
		E_SERVICE_ROOM_INCREMENT_GOLD,                          /// 17�������ӽ�ҷ���
		E_SERVICE_ROOM_END
	};

	struct	DataHead
	{
		long	lStructBytes;
		long	lServiceId;
		long	plReservation[2];
	};

	struct DataInit
	{
		long		            lStructBytes;
		int		                iSpinCount;
		HANDLE	                hIOCP;
		HANDLE					hEventManagerAi;
        mtQueueMgr*             pkQueueMgr;
	};

public:
	mtServiceMgr();
	virtual ~mtServiceMgr();

	int				init(DataInit* pkDataInit);
	virtual int		run(mtService::DataRun* pkDataRun);
	virtual int		exit();

private:
	int			initService();
	int			initServiceConnect();
	int			initServiceRoomDeskTwo();
	int			initServiceRoomDeskThree();
	int			initServiceRoomWaitStartRecv();
	int			initServiceRoomCardDeal();
	int         initServiceRoomGrabLandlord();
	int         initServiceRoomCardDoubleScore();
	int         initServiceRoomShowCard();
	int         initServiceRoomCardOut();
	int         initServiceRoomRequestCardsLeft();
	int         initServiceRoomRoundOver();
	int         initServiceRoomChangeDesk();
	int         initServiceRoomChatShortCut();
	int         initServiceRoomChatIcon();
	int         initServiceRoomChatContext();
	int         initServiceRoomHeartBeat();
	int         initServiceRoomIncrementGold();
	int         initServiceRoomExit();

private:
	int		                     m_iSpinCount;
	mtService*		             m_pkServiceList[E_SERVICE_ROOM_END];
	HANDLE	                     m_hIOCP;
	HANDLE					     m_hEventManagerAi;
	mtQueueMgr*                  m_pkQueueMgr;
};

#endif	///	__MT_SERVICE_MGR_H