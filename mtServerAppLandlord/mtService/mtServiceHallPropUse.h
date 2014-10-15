#ifndef 	__MTFL_SERVICE_HALL_PROP_USE_H
#define 	__MTFL_SERVICE_HALL_PROP_USE_H

class 	mtFLServiceHallPropUse
{
public:
	struct DataWrite
		{
			long 							lStructBytes;			/// ����С
			long                        	lServiceType;			/// ��������
			long 							plReservation[2];		/// �����ֶ�
			long                            lUserId;                /// �û�id
			long 							plPropsUseCount[10];	/// ����ʹ�����
		};

		struct DataRead
		{
			long 							lStructBytes;			/// ����С
			long                        	lServiceType;			/// ��������
			long 							plReservation[2];		/// �����ֶ�
			long                            lResult;                /// ʹ�õ��߽��
		};
};

#endif 	/// __MTFL_SERVICE_HALL_PROP_USE_H
