#ifndef		__MT_COMMON_H
#define		__MT_COMMON_H
//��س�������

#define		MT_GET_NAME(t)		#t

//�ɱ������__VA_ARGS__
#define		MT_DEBUG_0(...)			printf(__VA_ARGS__)
#define		MT_DEBUG_1(...)			printf(__VA_ARGS__)
#define		MT_DEBUG_2(...)			printf(__VA_ARGS__)
#define		MT_DEBUG_3(...)			printf("%d\t%s\n", __LINE__, __FILE__)
#define		MT_DEBUG_4(...)			printf(__VA_ARGS__)			/// �����߳�
#define		MT_DEBUG_5(...)			printf(__VA_ARGS__)			/// �����߳�
#define		MT_DEBUG_6(...)			printf(__VA_ARGS__)			/// �����߳�
#define		MT_DEBUG_7(...)			printf(__VA_ARGS__)			/// �ֲ�������
#define		MT_DEBUG_8(...)			printf(__VA_ARGS__)			/// ��ȷ�к�
#define		MT_DEBUG_9(...)			printf(__VA_ARGS__)			/// �����к�

#define		MT_DEBUG_PTCL(...)		    mtLog(__VA_ARGS__)			// printf(__VA_ARGS__)			///���log��־
#define		MT_DEBUG(iLevel, ...)   	MT_DEBUG_##iLevel(__VA_ARGS__); 
#define     MT_DEBUG_ERROR(iLevel,...)  SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);\
                                        MT_DEBUG_##iLevel(__VA_ARGS__);\
                                        SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
///#define		MT_DEBUG(iLevel, ...)	MT_DEBUG_##iLevel("%d ", iLevel);MT_DEBUG_##iLevel(__VA_ARGS__); 

#define		MT_NODE_USER			                        60000	                          /// 16384 2^14 32768 2^15

#define		MT_TIME_WAIT_LIMIT_MILLISECONDS			        5000 
#define		MT_TIME_CONNECT_LIMIT_SECONDS			        30
#define		MT_SERVER_WORK_USER_TOTAL		                60000							//Ŀǰ�޵ط�ʹ��
#define     MT_SERVER_WORK_USER_ID_BASE                     524288
#define     MT_SERVER_WORK_USER_ID_MIN                      1024
#define     MT_SERVER_WORK_USER_ID_MAX                      999999
#define		MT_SERVER_WORK_TIME_WAIT_MILLISECONDS			3000
#define		MT_SERVER_WORK_TIME_CONNECT_LIMIT_SECONDS		30
#define		MT_SERVER_MONITOR_USER_TOTAL	                5
#define		MT_SERVER_MONITOR_TIME_WAIT_MILLISECONDS		5000
#define		MT_SERVER_MONITOR_TIME_CONNECT_LIMIT_SECONDS	300

#define		MT_SERVER_SOCKET_ACCPET_TIMEOUT					5								//�쳣���ӳ�ʱ����
#define		MT_SERVER_SOCKET_DISCONNECT_TIMEOUT				1000 * 2						//DisconnectEx��ʱ��������
#define		MT_SERVER_WORK_SOCKET_TOTAL						100								//Ͷ�ݶ��ٸ�socket

#define		MT_APP_LIST_TOTAL				                2
#define		MT_APP_TOTAL					                20
#define		MT_APP_PACKET_TOTAL				                10
#define		MT_APP_PACKET_BYTES				                256
#define		MT_APP_FILE_PATH_LEN                            512							//file�ļ�·������
#define		MT_APP_UPDATE_URL_LEN                           512
#define		MT_APP_LOG_FILE_MAX_SIZE                        (1024 * 1024 * 8)				//����log�ļ�����󳤶�
#define		MT_APP_DATA_FILE_DIR_NAME                       "LoginServerData\\"				//�����ļ�Ŀ¼
#define		MT_APP_LOG_FILE_DIR_NAME                        "LoginServerLog\\"				//��־�ļ�Ŀ¼
#define		MT_APP_SET_FILE_NAME                            "mtLoginServerSet.ini"			//�����ļ�
#define		MT_APP_ID_FILE_NAME                             "MaxId"
#define     MT_APP_LOG_FILE_NAME                            "mtLoginLog.data"				//��־�ļ�����

#define		MT_SERVER_DATABASE_PARA_LEN		20			//DB��������

#define		MT_BYTES_ALIGNED(_iValue, _iAligned)	\
	((( (_iValue)+(_iAligned)-1) / (_iAligned)) * (_iAligned))
#define		MT_BYTES_OF_SECTOR				                512

#define		MT_BYTES_OF_ACCOUNT				                12
#define		MT_BYTES_OF_QQ					                12
#define		MT_BYTES_OF_PHONE				                12
#define		MT_BYTES_OF_PWD					                16
#define		MT_BYTES_OF_COMPANY_ID					        22
#define		MT_BYTES_OF_PATCH_ID					        4
#define		MT_BYTES_OF_VENDOR_ID					        6
#define		MT_BYTES_OF_RESERVATION					        8
#define     MT_DEFAULT_PWD_RAPID_REGISTER                   "000000"

#endif	///	__MT_COMMON_H