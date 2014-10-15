#include "mtPlatform.h"
#include <stdarg.h>

//long                 g_plVersion[4]            = {0,0,0,1};
FILE*                g_pkMtLogFile             = NULL;

/// ��������������(�������ļ���ȡ)
// ��¼
unsigned long        g_ulLoginServerIP         = 0;
long                 g_lLoginServerPort        = 0;
// ����
unsigned long        g_ulHallServerIP          = 0;
long                 g_lHallServerPort         = 0;
// ���
unsigned long        g_ulMonitorServerIP       = 0;
long                 g_lMonitorServerPort      = 0;
// �շ�
unsigned long        g_ulChargeServerIP        = 0;
long                 g_lChargeServerPort       = 0;
// ����
unsigned long        g_ulDownloadServerIP      = 0;
long                 g_lDownloadServerPort     = 0;
// �������url(��ʱδʹ��)
//char                 g_pcUpdateUrl             = 0;
//long                 g_lUpdatePort             = 0;
//long                 g_lUpdateUrlType          = 0;
// ���ݿ���ز���
char				g_DataBaseServer[MT_SERVER_DATABASE_PARA_LEN] = {0};
char				g_DataBaseUserName[MT_SERVER_DATABASE_PARA_LEN] = {0};
char				g_DataBasePassWord[MT_SERVER_DATABASE_PARA_LEN] = {0};
char				g_DataBaseName[MT_SERVER_DATABASE_PARA_LEN] = {0};
char				g_DataBaseCODE[MT_SERVER_DATABASE_PARA_LEN] = {0};
long				g_DataBasePoolSize;
//////////////////////////////////////////////////////////////////
// ���ݺ���־���Ŀ¼
char                 g_pcServerDataDir[MT_APP_FILE_PATH_LEN] = {0};
char                 g_pcServerLogDir[MT_APP_FILE_PATH_LEN]  = {0};
char                 g_pcServiceExeDir[MT_APP_FILE_PATH_LEN] = {0};

CRITICAL_SECTION	 g_kCriticalSection;

//��ȡ��ǰִ���ļ���Ŀ¼��ʧ�ܷ���0����
int mtGetRunExeDir(char* pDir, DWORD dwLen)
{	//��ȡ��ǰִ���ļ���ȫ·��
	GetModuleFileName(NULL, pDir, dwLen);		
	int iRet = -1;
	int iLen = strlen(pDir);
	if (0 >= iLen){
		return iRet;
	}

	//ȡ���ļ�·����ɾ���ļ���������·����
	for(int i = iLen; i >= 0; i--) {
		if(pDir[i] == '\\') {   
			pDir[i] = '\0';
			iRet = 1;
			break;
		} 
	}

	return iRet;
}
//����·��һ��һ��Ĵ���Ŀ¼
int mtCreateDirectory(const char* pcDir)
{
	char pcDirTmp[256];
	strcpy(pcDirTmp, pcDir);

	//�����״γ��ָ��ַ���λ�õ�ָ��
	char* pcOne = strchr(pcDirTmp, '\\');		//�ҵ���һ��Ŀ¼
	while(pcOne)
	{
		*pcOne = '\0';							//��ָ�����ó�ĩβ
		_mkdir(pcDirTmp);						//����Ŀ¼
		*pcOne = '\\';							//��ָ��λ�ø�λ
		pcOne = strchr(pcOne + 1, '\\');		//�ӵ�ǰλ�õĺ�һ��λ�ü�������
	}

	_mkdir(pcDirTmp);

	return 1;
}
//����־д��log�ļ������ж��ļ���С�Ƿ񳬳�ָ��ֵ�����������´����ļ�
void mtLog( char * pcformat, ... )
{
	va_list kArgs;
	int     iLen;
	char    pcBuffer[10240];

	memset(pcBuffer, 0, sizeof(pcBuffer));
	va_start(kArgs, pcformat);
	//iLen = _vscprintf(pcformat, kArgs) + 1;		//��ȡpcBuffer���ĳ���
	vsprintf(pcBuffer, pcformat, kArgs);
	EnterCriticalSection(&g_kCriticalSection);
	if (NULL == g_pkMtLogFile)
	{
		LeaveCriticalSection(&g_kCriticalSection);
		mtCreateLogFile();
		printf(pcBuffer);
		return;
	}

	/*
	��1��buffer����һ��ָ�룬��fwrite��˵����Ҫ��ȡ���ݵĵ�ַ��
	��2��size��Ҫд�����ݵĵ��ֽ�����
	��3��count:Ҫ����д��size�ֽڵ�������ĸ�����
	��4��stream:Ŀ���ļ�ָ�룻
	��5������ʵ��д������������count��
	*/
	//fwrite(pcBuffer, iLen, 1, g_pkMtLogFile);
	fwrite(pcBuffer, strlen(pcBuffer)+1, 1, g_pkMtLogFile);
	fflush(g_pkMtLogFile);
	va_end(kArgs);

	/// �ж��ļ���С
	long lFileSize = ftell(g_pkMtLogFile);
	if (MT_APP_LOG_FILE_MAX_SIZE <= lFileSize)
	{
		LeaveCriticalSection(&g_kCriticalSection);
		mtCreateLogFile();
		printf(pcBuffer);
		return;
	}

	LeaveCriticalSection(&g_kCriticalSection);
	printf(pcBuffer);
}
//������־�ļ��������ڸ�ʽ+MT_APP_LOG_FILE_NAMEΪ�ļ���
int mtCreateLogFile()
{
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);

	/// ������־�ļ�
	char pcAppLogFileName[MT_APP_FILE_PATH_LEN] = {0};
	strcpy(pcAppLogFileName, g_pcServerLogDir);
	sprintf(pcAppLogFileName + strlen(pcAppLogFileName), "%d%d%d%d%d%d",curTime.wYear, 
		curTime.wMonth, curTime.wDay, curTime.wHour,curTime.wMinute,curTime.wSecond);
	strcpy(pcAppLogFileName + strlen(pcAppLogFileName), MT_APP_LOG_FILE_NAME);
	EnterCriticalSection(&g_kCriticalSection);
	if (g_pkMtLogFile){
		//�ر�һ������ע�⣺ʹ��fclose()�����Ϳ��԰ѻ����������ʣ�����������������ļ��У����ͷ��ļ�ָ����йصĻ�����
		fclose(g_pkMtLogFile);
		g_pkMtLogFile = NULL;
	}
	//a+ �Ը��ӷ�ʽ�򿪿ɶ�д���ļ�����ԭ����EOF����������
	g_pkMtLogFile = fopen(pcAppLogFileName, "a+");
	LeaveCriticalSection(&g_kCriticalSection);
	if (NULL == g_pkMtLogFile)
	{
		return 0;
	}

	return 1;
}

BOOL SetConsoleColor(WORD wAttributes)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
		return FALSE;

	BOOL bResult = SetConsoleTextAttribute(hConsole, wAttributes);

//	CloseHandle(hConsole);
	return bResult;
}