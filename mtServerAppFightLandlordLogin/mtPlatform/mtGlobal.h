#ifndef		__MT_GLOBAL_H
#define		__MT_GLOBAL_H

//extern long g_plVersion[4];
extern FILE* g_pkMtLogFile;									//�ļ�ָ��
/// ��������������(�������ļ���ȡ)
// ��¼
extern unsigned long        g_ulLoginServerIP;
extern long                 g_lLoginServerPort;				//��¼�˿�
// ����
extern unsigned long        g_ulHallServerIP;				//����IP
extern long                 g_lHallServerPort;				//�����˿�
// ���
extern unsigned long        g_ulMonitorServerIP;
extern long                 g_lMonitorServerPort;
// �շ�
extern unsigned long        g_ulChargeServerIP;
extern long                 g_lChargeServerPort;
// ����
extern unsigned long        g_ulDownloadServerIP;
extern long                 g_lDownloadServerPort;
// �������url(��ʱδʹ��)
//extern char                 g_pcUpdateUrl;
//extern long                 g_lUpdatePort;
//extern long                 g_lUpdateUrlType;
// ���ݿ���ز���
extern char					g_DataBaseServer[MT_SERVER_DATABASE_PARA_LEN];
extern char					g_DataBaseUserName[MT_SERVER_DATABASE_PARA_LEN];
extern char					g_DataBasePassWord[MT_SERVER_DATABASE_PARA_LEN];
extern char					g_DataBaseName[MT_SERVER_DATABASE_PARA_LEN];
extern char					g_DataBaseCODE[MT_SERVER_DATABASE_PARA_LEN];
extern long					g_DataBasePoolSize;
/////////////////////////////////////////////////////////////////////////
// ���ݺ���־���Ŀ¼
extern char                 g_pcServerDataDir[MT_APP_FILE_PATH_LEN];			//����Ŀ¼
extern char                 g_pcServerLogDir[MT_APP_FILE_PATH_LEN];				//��־Ŀ¼
extern char                 g_pcServiceExeDir[MT_APP_FILE_PATH_LEN];			//��ǰexe���е�Ŀ¼

extern CRITICAL_SECTION	    g_kCriticalSection;				//�ٽ�������Ŀǰʹ�õĶ����У��ļ�����ָ�룩����			


extern int mtCreateLogFile();								///������־�ļ��������ڸ�ʽ+MT_APP_LOG_FILE_NAMEΪ�ļ���
extern void mtLog( char * pcformat, ... );					//д��־�ļ�
extern int mtGetRunExeDir(char* pDir, DWORD dwLen);		///��ȡ��ǰִ���ļ���Ŀ¼��ʧ�ܷ���0����
extern int mtCreateDirectory(const char* pcDir);			///����·��һ��һ��Ĵ���Ŀ¼
BOOL SetConsoleColor(WORD wAttributes);
#endif ///	__MT_GLOBAL_H