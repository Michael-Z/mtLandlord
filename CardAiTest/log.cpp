//#include "log.h"
//#include <iostream>
//#include <direct.h>
//using namespace std;
//char     g_LogDir[FILE_PATH_LEN]={0};
//char     g_ExeDir[FILE_PATH_LEN]={0};
//
//bool GetRunExeDir(char* pDir, DWORD dwLen)  //�õ���ǰ���г����·�����浽����ָ������
//{
//	GetModuleFileName(NULL, pDir, dwLen);//���ص�ǰӦ�ó���ȫ·������Ϊapi����
//	bool flag = NULL;
//	int iLen = strlen(pDir);
//	if (0 >= iLen)
//	{
//		return FALSE;
//	}
//
//	//ȡ���ļ�·��  
//	for(int i = iLen; i >= 0; i--)
//	{
//		if(pDir[i] == '\\')
//		{   
//			pDir[i] = '\0';
//			break;
//		} 
//	}
//
//	return TRUE;
//}
//
//int  CreateLogFile(char *logFile,FILE* pLogFile)  //������־�ļ�����
//{
//	//SYSTEMTIME curTime;//�õ���ǰʱ��
//	//GetLocalTime(&curTime);
//
//	/// ������־�ļ�
//	char logFileName[FILE_PATH_LEN] = {0};
//	strcpy(logFileName, g_LogDir);
//	strcpy(logFileName + strlen(logFileName),"\\");
//	//sprintf(logFileName + strlen(logFileName), "%d%d%d%d%d%d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour,curTime.wMinute,curTime.wSecond);
//	strcpy(logFileName + strlen(logFileName),logFile);
//	strcpy(logFileName + strlen(logFileName), ".log");
//
//	if (pLogFile)
//	{
//		fclose(pLogFile);
//		pLogFile = NULL;
//	}
//
//	pLogFile = fopen(logFileName, "a+");
//
//	if (NULL == pLogFile)
//	{
//		return 0;
//	}
//
//	return 1;
//}
//
//
//int WSDCreateDirectory(const char* pcDir)    //�½�Ŀ¼ ����Ϊȫ·�� ��ʵ�����½�һ���ļ���
//{
//	char pcDirTmp[256];
//	strcpy(pcDirTmp, pcDir);
//
//	char* pcOne = strchr(pcDirTmp, '\\');    
//	while(pcOne)
//	{
//		*pcOne = '\0';
//		_mkdir(pcDirTmp);
//		*pcOne = '\\';
//		pcOne = strchr(pcOne + 1, '\\');
//	}
//
//	_mkdir(pcDirTmp);
//
//	return 1;
//}
//
//
//int   LogInit()
//{
//	if(!GetRunExeDir(g_ExeDir,FILE_PATH_LEN))  
//	{
//		cout<<"get dir fail"<<endl;
//		return 0;
//	}
//	strcpy(g_ExeDir + strlen(g_ExeDir), "\\");
//	strcpy(g_LogDir, g_ExeDir);
//	strcpy(g_LogDir + strlen(g_LogDir), "RunLog");
//	//strcpy(g_LogDir + strlen(g_LogDir), "MoniterOutNetTestlog");
//
//	WSDCreateDirectory(g_LogDir);//������־Ŀ¼ ��.log�ļ���
//	return 1;
//
//}