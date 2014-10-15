#pragma once
#include <WinSock2.h>
#include "mtCardAI.h"
#include "mtNetWork.h"
#include "mtServiceRoomCardOut.h"
#include "mtServiceRoomConnect.h"
#include "mtServiceRoomGrabLandlord.h"
#include "mtServiceRoomMgr.h"
#include "mtServiceRoomWaitStart.h"
#include "mtServiceRoomRoundOver.h"
#include "mtServiceRoomCardDealRecv.h"
#include "mtServiceRoomDeskThree.h"
#include "mtServiceRoomExit.h"
#include "time.h"
#include "log.h"
#include <direct.h>
#pragma comment(lib, "ws2_32.lib")
#include<iostream>
using namespace std;

enum
{
	E_ERROR_SELECT		= -3,			/// selectʧ��
	E_ERROR_SELECT_NONE	= -4,			/// û��socket����Ϣ
	E_ERROR_READ		= -5,			/// readʧ��
	E_ERROR_READ_BYTES	= -6,			/// read���ֽ�������
	E_ERROR_WRITE		= -7,			/// дʧ��
	E_ERROR_WRITE_BYTES	= -8,			/// д���ֽ�������
	E_ERROR_CLOSE		= -9,			/// close socketʧ��
	E_ERROR_NETWORK_CLOSE = -10,		/// ����ϵ�
};

