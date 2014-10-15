#ifndef		__MT_SERVICE_SHOW_CARD_H
#define		__MT_SERVICE_SHOW_CARD_H
#include "mtService.h"
#include "mtQueueDesk.h"

/// �û����Ʒ���
class mtServiceShowCard : public mtService
{
public:
	struct DataInit
	{
		long		            lStructBytes;
		mtQueueUser*	        pkQueueUser;
		mtQueuePacket*	        pkQueuePacket;
	};

	struct DataRead
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lShowCard;              /// �Ƿ�����(1 -���ƣ�0 -������)
		char 							pcCard[24];             /// ������
	};

	struct DataWrite
	{
		long 							lStructBytes;			/// ����С
		long                        	lServiceType;			/// ��������
		long 							plReservation[2];		/// �����ֶ�
		long                            lUserId;                /// �����û�id
		long                            lShowCard;              /// �Ƿ�����(1 -���ƣ�0 -������)
		char 							pcCard[24];             /// ������
	};

public:
	mtServiceShowCard();
	~mtServiceShowCard();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();

	mtQueueUser*	m_pkQueueUser;
	mtQueuePacket*	m_pkQueuePacket;
};


#endif ///	__MT_SERVICE_SHOW_CARD_H