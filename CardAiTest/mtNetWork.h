#pragma once 
#include <windows.h>
class mtNetWork
{
public:
	mtNetWork();
	~mtNetWork();
public:
	bool   init(SOCKET socketClient,SOCKADDR_IN* pkServerAddress);  //ʵ�ִ����׽��֣����շ�������ַ
	bool   ConnectServer();   //���ӷ�����
	bool   WriteData(char *buffer,int iBufferBytes);        //��������
	bool   ReadData(char *buffer,int iBufferBytes);         //��������
public:
	SOCKET m_SocketClient;       //�ͻ��˵��׽���  �ڸ�������ʼ����ʱ�� ����
	SOCKADDR_IN*     m_pkServerAddress; //��������ַ�ṹ��ָ��

};