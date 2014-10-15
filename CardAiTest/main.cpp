#include "mtInclude.h"
#define  LOG_FILE_FOLDER_NAME   "RunLog"           //��־�ļ���
#define  THREAD_NUMBER    50                    //�߳�����
#define  PORT             9099
#define  IP               "192.168.0.198"
#define  MAX_ROUND        10
#define  SLEEP_CARD_OUT_TIME       1000*2           //���Ƶȴ�ʱ��
#define  SLEEP_CALL_START_TIME     1000*5           //�ؽп�ʼ�ȴ�ʱ��
static long sl_UserId = 1073;
SOCKADDR_IN  serverAddress;  //ȫ�ֵ�ַ
CRITICAL_SECTION g_cs;

void  SaftyRelease(void *p); 

bool  initNetModule();

void  initThreadModule();

void  Log( FILE* pFiLE,char * pcformat, ... );            

bool  GetRunExeDir(char* pDir, DWORD dwLen);    //�õ����г����·��

int   WSDCreateDirectory(const char* pcDir);    //�½�Ŀ¼ ����Ϊȫ·�� ��ʵ�����½�һ���ļ���

int   ReadData(SOCKET socket, char *pcBuffer, long bufferSize);  //��ȡ��������

int   main()
{  
	//����
	if(!initNetModule())
	{
		cout<<"��ʼ������ʧ��"<<endl;
		return 0;
	}
	//�߳�
	InitializeCriticalSection(&g_cs);

	initThreadModule();
	
	Sleep(INFINITE);  //���߳�һֱ�ȴ�

	WSACleanup();
	return 0;
}

DWORD WINAPI ThreadWorkFun( LPVOID lpParam )  //�̺߳���
{
	EnterCriticalSection(&g_cs);
	sl_UserId++;
	long lUserAiId = sl_UserId; //�õ��Լ�id
	LeaveCriticalSection(&g_cs);
	long lLastAiId = 0; //�ϻ�id
	long lNextAiId = 0; //�»�id
	unsigned long lDeskId   = 0;  //����id
	int  iLastAiCardNum = 17;          //�ϻ����е���
	int  iNextAiCardNum = 17;          //�»����е���
	long lRealOutCardPlay = 0;  //�������Ƶ���� �����Ʋ��㣩
	char CardUserCard[24] = {0}; //�û����� �����
	char CardBaseCard[7]={0};    //����     �����
	mtCardInfo CardUserInfo[24];  //�û����� �ͻ���
	mtCardInfo CardBaseInfo[7];   //����     �ͻ���
	mtCardInfo CardSlectInfo[24];      //Ҫ������  �ͻ���
	char       CardSlect[24] = {0};    //Ҫ������  �����
	mtCardInfo CardInDeskInfo[24];     //�����ϸոճ����� �ͻ���
	int     iResult = -1;
	char    buffer[10*1024] ={0};         //�����������ݵĻ�����
	int     iCompleteRound = 0;            //��ͬһ������ ��ɵľ���
	int     iCompleteLunShu = 0;
	srand( (unsigned)time( NULL ) );       
	int iRoundTotal = 1 + rand()%10;        //��ͬһ������ Ҫ��ɵľ���
	bool bStartFlag = false;                //��ʼ��־
	SOCKET clientSocket = INVALID_SOCKET;   //ͨѶ�׽���

	FILE*      pLogFile = NULL;            //��־�ļ�ָ��
	char cExeDir[500] = {0};               //���浱ǰ�����·��
	char cLogDir[500] = {0};               //��־�ļ���·��

	GetRunExeDir(cExeDir,500);               //�õ�����·��
	strcpy(cExeDir + strlen(cExeDir), "\\");
	strcpy(cLogDir, cExeDir);
	strcpy(cLogDir + strlen(cLogDir), LOG_FILE_FOLDER_NAME);

	WSDCreateDirectory(cLogDir);
	strcpy(cLogDir+strlen(cLogDir),"\\AiId");
	char buf[500] = {0};
	sprintf(buf,"%s%d.log",cLogDir,lUserAiId);
	if (pLogFile)
	{
		fclose(pLogFile);
		pLogFile = NULL;
	}
	pLogFile = fopen(buf, "a+");
	if (NULL == pLogFile)
	{
		return 0;
	}
	mtCardAI *pkCardAi = new mtCardAI;
	if (!pkCardAi)
	{
		SaftyRelease(pkCardAi);
		Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
		return 0;
	}
	mtNetWork *pkNetWork = new mtNetWork();
	if (!pkNetWork)
	{
		SaftyRelease(pkCardAi);
		SaftyRelease(pkNetWork);
		Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
		return 0;
	}
	Log(pLogFile,"�û�һ����Ҫ���%d��\n",iRoundTotal);
	 //�����ѭ��
	while (1)   //�ظ�����                                                     
	{
step1:  clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //�����׽���
		if (clientSocket == INVALID_SOCKET)
		{
			cout<<"�׽��ִ���ʧ��"<<endl;
			cout<<GetLastError()<<endl;
			SaftyRelease(pkCardAi);
			SaftyRelease(pkNetWork);
			Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
			return 0;
		}

		if(!pkNetWork->init(clientSocket,&serverAddress)) //
		{
			SaftyRelease(pkCardAi);
			SaftyRelease(pkNetWork);
			if (clientSocket != SOCKET_ERROR)
			{
				closesocket(clientSocket);
			}
			return 0;
		}

	    if(!pkNetWork->ConnectServer())  //����������
		{
			SaftyRelease(pkNetWork);
			SaftyRelease(pkCardAi);
			if (clientSocket != SOCKET_ERROR)
			{
				closesocket(clientSocket);
			}
			Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
			return 0;
		}
		//��������
		iLastAiCardNum = 17;
		iNextAiCardNum = 17;
		iCompleteRound = 0;
		bStartFlag     = false;
		iCompleteLunShu++;                //������1
		for (int i = 0; i<24; i++)  //Ҫ����һ�����浱ǰ������Ϣ
		{
			CardInDeskInfo[i].cCardType = 0;
			CardInDeskInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
		}
		//��������Э������ ��һ�η�������
		mtServiceRoomConnect kServiceRoomConnect;
		kServiceRoomConnect.init(lUserAiId);
		if(!(pkNetWork->WriteData((char*)&kServiceRoomConnect.m_kConnectDataWrite,
			                       sizeof(kServiceRoomConnect.m_kConnectDataWrite))))  //������������ ������ʽ

		{
			SaftyRelease(pkNetWork);
			SaftyRelease(pkCardAi);
			if (clientSocket != SOCKET_ERROR)
			{
				closesocket(clientSocket);
			}
			Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
			return 0;
		}
		SYSTEMTIME curTime;
		GetLocalTime(&curTime);
		Log(pLogFile,"��������Э�� %d:%d:%d\n",curTime.wHour,curTime.wMinute,curTime.wSecond);
		while (1)                     //�����ظ��Ľ�������
		{
			//Log(pLogFile,"\n׼������Э��......\n");
			memset(buffer,0,10*1024*sizeof(char)); //����һ��������� ����յ�����
	
			int iReturn = ReadData(clientSocket,buffer,10*1024);  //selectģ��
			//int iReturn = recv(clientSocket,buffer,10*1024,0);      //��ͨ����
			if (iReturn > 0)  //��������û�д��� ��ʼ����
			{
				long *lpCurPos = (long*)buffer;  //ǿת
				//���봦��һ�µõ������� ���ܻ�����м���Э��  
				long lDataSize = lpCurPos[0];
				while (iReturn >= lDataSize)             // һ���Խ��˼���Э��
				{
					if (iReturn == lDataSize) //ֻʣ��һ��
					{
						switch (lpCurPos[1]) //�õ�Э�������
						{
						case E_SERVICE_ROOM_CONNECT:                         //����
							{
								Log(pLogFile,"��������Э��\n");
								mtServiceRoomConnect::DataRead *pkConnectDataRead = (mtServiceRoomConnect::DataRead*)(lpCurPos);
								iResult = pkConnectDataRead->lResult;
								if (iResult == 1)  //���ӳɹ���
								{
									Log(pLogFile,"���ӳɹ�\n");
									//���͵ȴ���ʼ��Э��
									/*mtServiceRoomWaitStart kServiceRoomWaitStart;
									kServiceRoomWaitStart.init();
									if(!(pkNetWork->WriteData((char*)(&kServiceRoomWaitStart.m_kWaitStartDataWrite),sizeof(kServiceRoomWaitStart.m_kWaitStartDataWrite))))
									{
									SaftyRelease(pkNetWork);
									SaftyRelease(pkCardAi);
									return 0;
									}*/
								}
								else 
								{
									Log(pLogFile,"���ӳ����쳣����%d\n",iResult);
									SaftyRelease(pkNetWork);
									SaftyRelease(pkCardAi);
									if (clientSocket != SOCKET_ERROR)
									{
										closesocket(clientSocket);
									}
									Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
									return 0;
									
									
								}														
							}
							break;
						case E_SERVICE_ROOM_DESK_TWO:          //�齨˫������
							{
								Log(pLogFile,"\n����˫�������齨��Э��\n");
								if(!bStartFlag)  //���û�нп�ʼ
								{
									mtServiceRoomWaitStart kServiceRoomWaitStart;
									kServiceRoomWaitStart.init();
									if(!(pkNetWork->WriteData((char*)(&kServiceRoomWaitStart.m_kWaitStartDataWrite),sizeof(kServiceRoomWaitStart.m_kWaitStartDataWrite))))
									{
										SaftyRelease(pkNetWork);
										SaftyRelease(pkCardAi);
										if (clientSocket != SOCKET_ERROR)
										{
											closesocket(clientSocket);
										}
										Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
										return 0;
									}
									Log(pLogFile,"����˫��������ÿ�ʼЭ��\n");
									bStartFlag = true;

								}
								
							}
							break;
						case E_SERVICE_ROOM_DESK_THREE:       //�齨�������� 
							{
								Log(pLogFile,"�������������齨��Э��\n");
								mtServiceRoomDeskThree::DataRead *pkDeskThreeDataRead = (mtServiceRoomDeskThree::DataRead*)(lpCurPos);
								lLastAiId = pkDeskThreeDataRead->kUserInfo[0].lUserId;  //�õ��ϼ�id
								lNextAiId = pkDeskThreeDataRead->kUserInfo[1].lUserId;  //�õ��¼�id
								lDeskId   = pkDeskThreeDataRead->kUserInfo[0].ulDeskId; //�õ�����id
								Log(pLogFile,"\n*******��%d�ֵ�%d�ֿ�ʼ,�յ��������ӽ���Э��,�ϼ�id:%d,�¼�id:%d,����id:%x*******\n",iCompleteLunShu,iCompleteRound + 1,lLastAiId,lNextAiId,lDeskId);							
								if (!bStartFlag)
								{
									mtServiceRoomWaitStart kServiceRoomWaitStart;
									kServiceRoomWaitStart.init();
									if(!(pkNetWork->WriteData((char*)(&kServiceRoomWaitStart.m_kWaitStartDataWrite),sizeof(kServiceRoomWaitStart.m_kWaitStartDataWrite))))
									{
										SaftyRelease(pkNetWork);
										SaftyRelease(pkCardAi);
										if (clientSocket != SOCKET_ERROR)
										{
											closesocket(clientSocket);
										}
										Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
										return 0;
									}
									Log(pLogFile,"�������������齨�õĿ�ʼЭ��\n");
									bStartFlag = true;
								}
							}
							break;
						case E_SERVICE_ROOM_WAIT_START:       //��ʼЭ��    
							{
								mtServiceRoomWaitStart::DataRead *pkWaitStartDataRead = (mtServiceRoomWaitStart::DataRead*)(lpCurPos);
								Log(pLogFile,"���տ�ʼЭ��%d\n",pkWaitStartDataRead->lUserId);
								 //Ҫ����һ�����浱ǰ������Ϣ  ��Ϊ���¿�ʼ �ᵽ������ ����������һ������
								iLastAiCardNum = 17;
								iNextAiCardNum = 17;
								lRealOutCardPlay = 0;
								for (int i = 0; i<24; i++) 
								{
									CardInDeskInfo[i].cCardType = 0;
									CardInDeskInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
								}
							}
							break;
						case E_SERVICE_ROOM_CARD_DEAL:         //�ƹ����� �ӵ��� 
							{
								Log(pLogFile,"�ӵ�ϴ�Ʒ���Э��\n");
								mtServiceRoomCardDealRecv::DataRead *pkCardDealRecvDataRead = (mtServiceRoomCardDealRecv::DataRead*)(lpCurPos);
								// �ȴ����
								for (int i = 0; i<24; i++)
								{
									CardUserInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
									CardUserInfo[i].cCardType = 0;
									CardUserCard[i] = MTFL_NUM_CARD_TERMINAL;
									if (i<7)
									{
										CardBaseCard[i] = MTFL_NUM_CARD_TERMINAL;
									}
								}
								memcpy(CardUserCard,pkCardDealRecvDataRead->pcCards,sizeof(char)*17);
								memcpy(CardBaseCard,pkCardDealRecvDataRead->pcBaseCards,sizeof(char)*7);
								pkCardAi->cardConvert(CardUserCard,CardUserInfo);
								pkCardAi->sortCard(CardUserInfo);
								
								Log(pLogFile,"�Լ����棺");
								pkCardAi->dispalyCardInfo(CardUserInfo,pLogFile);
								if( pkCardDealRecvDataRead->lDefaultLandlordUserId == lUserAiId )  // �Լ���ѡ��������
								{
									//����һ����������Э��
									Log(pLogFile,"�Լ���ѡ��������\n");
									mtServiceRoomGrabLandlord kServiceRoomGrabLandlord;
									kServiceRoomGrabLandlord.init();
									if (!(pkNetWork->WriteData((char*)&(kServiceRoomGrabLandlord.m_kGrablandlordDataWrite),sizeof(kServiceRoomGrabLandlord.m_kGrablandlordDataWrite))))
									{
										SaftyRelease(pkNetWork);
										SaftyRelease(pkCardAi);
										if (clientSocket != SOCKET_ERROR)
										{
											closesocket(clientSocket);
										}
										Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
										return 0;
									}
									Log(pLogFile,"����������Э�� 3��\n");
								}
								else                                                         //Ĭ���������Ĳ����Լ�
								{
									//Log(pLogFile,"�ȴ������������з�\n");
								}

							}
							break;
						case E_SERVICE_ROOM_GRAB_LANDLORD:                                   //������Э��
							{
								Log(pLogFile,"�ӵ�������Э��\n");
								mtServiceRoomGrabLandlord::DataRead *pkGrabLandLordDataRead = (mtServiceRoomGrabLandlord::DataRead*)(lpCurPos);
								if (pkGrabLandLordDataRead->lUserIdLandlord > 0)   //�Ѿ��е�����
								{
								
									if ( pkGrabLandLordDataRead->lUserIdLandlord == lUserAiId )     // �Լ������˵��� ����Ҫ�ӽ��Լ����������� ��Ҫ����һ��                                                    
									{
										//���ͳ���Э��
										Log(pLogFile,"�Լ��ǵ���\n");
										for (int i = 0; i<24; i++)
										{
											CardUserInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
											CardUserInfo[i].cCardType = 0;
											CardSlectInfo[i].cCardType = 0;
											CardSlectInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
											CardSlect[i] = MTFL_NUM_CARD_TERMINAL;
											if (i<7)
											{
												CardBaseInfo[i].cCardType = 0;
												CardBaseInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
											}
										}
										pkCardAi->cardConvert(CardUserCard,CardUserInfo);     //���������� ת��Ϊ�ͻ��˵���
										pkCardAi->cardConvert(CardBaseCard,CardBaseInfo);
										pkCardAi->addInCard(CardUserInfo,CardBaseInfo);      //��� ����  ���е�ֻҪ��֤�� �������� MTFL_NUM_CARD_TERMINAL ���� �Ͱ�ȫ��
										pkCardAi->sortCard(CardUserInfo);                    //����һ��
										//ִ�е�һ�γ��Ʋ��� ������Э���ȥ
										pkCardAi->getCardOut(CardUserInfo,NULL,MT_FL_CARD_TYPE_BEG,CardSlectInfo);//�õ����Ƶ�����
										mteCardType eCardType = pkCardAi->getTypeFromCard(CardSlectInfo);          //�õ����Ƶ�����
										pkCardAi->cardConvert(CardSlectInfo,CardSlect); //������ת��һ�� �������͵������
										mtServiceRoomCardOut kServiceRoomCardOut;
										kServiceRoomCardOut.init(eCardType,CardSlect);
										Sleep(SLEEP_CARD_OUT_TIME);
										if (!(pkNetWork->WriteData((char*)(&kServiceRoomCardOut.m_kCardOutDataWrite),sizeof(kServiceRoomCardOut.m_kCardOutDataWrite))))
										{
											SaftyRelease(pkNetWork);
											SaftyRelease(pkCardAi);
											if (clientSocket != SOCKET_ERROR)
											{
												closesocket(clientSocket);
											}
											Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
											return 0;
										}
										
										Log(pLogFile,"�Լ����ͳ���Э��,�����ݣ�");
										pkCardAi->dispalyCardInfo(CardSlectInfo,pLogFile);
										//����һ���Լ�������
										pkCardAi->takeAwayCard(CardUserInfo,CardSlectInfo);
										pkCardAi->sortCard(CardUserInfo);
										Log(pLogFile,"�Լ�����ʣ����ƣ�");
										pkCardAi->dispalyCardInfo(CardUserInfo,pLogFile);
									}
									else if(pkGrabLandLordDataRead->lUserIdLandlord == lLastAiId)
									{
										
										Log(pLogFile,"�������ϼ�:%d\n",pkGrabLandLordDataRead->lUserIdLandlord);
										iLastAiCardNum = 20;
									}
									else if(pkGrabLandLordDataRead->lUserIdLandlord == lNextAiId)
									{
										Log(pLogFile,"�������¼�:%d\n",pkGrabLandLordDataRead->lUserIdLandlord);
										iNextAiCardNum = 20;
									}
									else
									{
										cout<<"****"<<endl;
									}
								}
								if (pkGrabLandLordDataRead->lUserIdLandlord == -1)  //��û�в�������
								{
									Log(pLogFile,"��û�в�������\n");
									if (pkGrabLandLordDataRead->lUserId == lLastAiId) //����һ�ҽеĵ���
									{
										//�Լ�������
										
										Log(pLogFile,"�ϼ�����������:%d\n",pkGrabLandLordDataRead->lScore);
										mtServiceRoomGrabLandlord kServiceRoomGrabLandlord;
										kServiceRoomGrabLandlord.init();
										if (!(pkNetWork->WriteData((char*)&(kServiceRoomGrabLandlord.m_kGrablandlordDataWrite),sizeof(kServiceRoomGrabLandlord.m_kGrablandlordDataWrite))))
										{
											SaftyRelease(pkNetWork);
											SaftyRelease(pkCardAi);
											if (clientSocket != SOCKET_ERROR)
											{
												closesocket(clientSocket);
											}
											Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
											return 0;
										}
										Log(pLogFile,"����������Э�� ������%d\n",kServiceRoomGrabLandlord.m_kGrablandlordDataWrite.lScore);
									}
									else
									{
										Log(pLogFile,"�¼�������\n");
									}
						
								}
							}
							break;
						case E_SERVICE_ROOM_CARD_OUT:                                     //����Э�� 
							{
								mtServiceRoomCardOut::DataRead *pkCardOutDataRead = (mtServiceRoomCardOut::DataRead*)(lpCurPos);
								mteCardType eCardType = (mteCardType)pkCardOutDataRead->lCardType; //�õ����Ƶ�����
								long lUserId = pkCardOutDataRead->lUserId; //�õ������˵�id�����Ƶ�Ҳ�㣩
								int iCardOutNum = 0;                 //��ǰ���Ƶ�����
								for (int i = 0; i<24; i++)
								{
									CardSlectInfo[i].cCardType = 0;
									CardSlectInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
									CardSlect[i] = MTFL_NUM_CARD_TERMINAL;
								}
								if (eCardType != MT_FL_CARD_TYPE_PASS) //������ǹ���  ��Ҫ����һ�����ӵ�ǰ������Ϣ ������ʵ�ĳ����ˣ�����ʵ�Ƶ��ˣ�
								{
									char *pCardOut = pkCardOutDataRead->pcCard;  //�õ������ϸոճ�����
									for (int i = 0; i<24; i++)
									{
										CardInDeskInfo[i].cCardType = 0;
										CardInDeskInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;	
									}
									pkCardAi->cardConvert(pCardOut,CardInDeskInfo);//ת���ɿͻ��˵������� ��CardInDeskInfo��
									pkCardAi->sortCard(CardInDeskInfo);
									Log(pLogFile,"\n���ճ���Э��,������%d,�������ݣ�",lUserId);
									pkCardAi->dispalyCardInfo(CardInDeskInfo,pLogFile);
									lRealOutCardPlay = pkCardOutDataRead->lUserId;//����һ�� �����ĳ������ ���Ĳ��� 
									iCardOutNum = pkCardAi->getCardNum(CardInDeskInfo); //�õ����Ƶ�����
								}
								else                                                                //����
								{
									//��CardInDeskInfo ������һ���������
									Log(pLogFile,"\n���ճ���Э��,������%d,�������ݣ���\n",lUserId);

								}
								Log(pLogFile,"��ǰ����������:");
								pkCardAi->dispalyCardInfo(CardInDeskInfo,pLogFile);

								if (lUserId == lNextAiId)                                //�¼ҳ�����  
								{
									iNextAiCardNum -= iCardOutNum;
									if (iNextAiCardNum <= 0)                         //�¼��Ѿ���������  ���ͽ���һ��Э�� Ҳ���ܻᷢ�������˼�����Э�� �ٿ�
									{
										Log(pLogFile,"�¼�ʣ����Ϊ0\n");
										mtServiceRoomRoundOver kServiceRoomRoundOver;
										kServiceRoomRoundOver.init();
										if (!(pkNetWork->WriteData((char*)(&kServiceRoomRoundOver.m_kRoundOverDataWrite),sizeof(kServiceRoomRoundOver.m_kRoundOverDataWrite))))
										{
										SaftyRelease(pkNetWork);
										SaftyRelease(pkCardAi);
										if (clientSocket != SOCKET_ERROR)
										{
										closesocket(clientSocket);
										}
										Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
										return 0;
										}
										Log(pLogFile,"�Լ����ͽ���һ��Э��\n");
									}
									else
									{
										//Log(pLogFile,"�¼ҵĳ��ƣ���Ҫ\n");
									}
								}
								else if (lUserId == lLastAiId)                            //�ϼҳ����� ��Ҫѹ��
								{
									if (lUserAiId == lRealOutCardPlay)           //�ϴ��������Ƶ����Լ�
									{
										//�ж�һ���Լ����е���
										if (!pkCardAi->getCardNum(CardUserInfo))  //����û������
										{
											//���� ����һ�ֵ�Э���ȥ
										    Log(pLogFile,"�Լ�ʣ����Ϊ0\n");
											mtServiceRoomRoundOver kServiceRoomRoundOver;
											kServiceRoomRoundOver.init();
											if (!(pkNetWork->WriteData((char*)(&kServiceRoomRoundOver.m_kRoundOverDataWrite),sizeof(kServiceRoomRoundOver.m_kRoundOverDataWrite))))
											{
												SaftyRelease(pkNetWork);
												SaftyRelease(pkCardAi);
												if (clientSocket != SOCKET_ERROR)
												{
													closesocket(clientSocket);
												}
												Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
												return 0;
											}
											Log(pLogFile,"���ͽ���һ��Э��\n");

										}
										else //������ ��������
										{
											
											//ִ�е�һ�γ��Ʋ��� ������Э���ȥ
											pkCardAi->getCardOut(CardUserInfo,NULL,MT_FL_CARD_TYPE_BEG,CardSlectInfo);//�õ����Ƶ�����
											mteCardType eCardType = pkCardAi->getTypeFromCard(CardSlectInfo);          //�õ����Ƶ�����
											pkCardAi->cardConvert(CardSlectInfo,CardSlect); //������ת��һ�� �������͵������
											mtServiceRoomCardOut kServiceRoomCardOut;
											kServiceRoomCardOut.init(eCardType,CardSlect);
											Sleep(SLEEP_CARD_OUT_TIME); //��Ϣһ��
											if (!(pkNetWork->WriteData((char*)(&kServiceRoomCardOut.m_kCardOutDataWrite),sizeof(kServiceRoomCardOut.m_kCardOutDataWrite))))
											{
												SaftyRelease(pkNetWork);
												SaftyRelease(pkCardAi);
												if (clientSocket != SOCKET_ERROR)
												{
													closesocket(clientSocket);
												}
												Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
												return 0;
											}
											Log(pLogFile,"�Լ���������:");
											pkCardAi->dispalyCardInfo(CardSlectInfo,pLogFile);
											//����һ���Լ�������
											pkCardAi->takeAwayCard(CardUserInfo,CardSlectInfo);
											pkCardAi->sortCard(CardUserInfo);
											Log(pLogFile,"�Լ�����ʣ�����:");
											pkCardAi->dispalyCardInfo(CardUserInfo,pLogFile);
										}
									}
									else //�������ƵĲ����Լ� �п����� �ϼ� Ҳ�п������¼� ��������һ�� �Ҷ�Ҫѹ��
									{
										iLastAiCardNum -= iCardOutNum; //�õ��ϼ����е���
										if (iLastAiCardNum <= 0)                         //�ϼ��Ѿ���������  ���ͽ���һ��Э�� Ҳ���ܻᷢ�������˼�����Э�� �ٿ�
										{
										
											mtServiceRoomRoundOver kServiceRoomRoundOver;
											kServiceRoomRoundOver.init();
											if (!(pkNetWork->WriteData((char*)(&kServiceRoomRoundOver.m_kRoundOverDataWrite),sizeof(kServiceRoomRoundOver.m_kRoundOverDataWrite))))
											{
												SaftyRelease(pkNetWork);
												SaftyRelease(pkCardAi);
												if (clientSocket != SOCKET_ERROR)
												{
													closesocket(clientSocket);
												}
												Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
												return 0;
											}
											Log(pLogFile, "�ϼ�ʣ����Ϊ0���Լ����ͽ���һ��Э���ȥ\n");
										}
										else
										{
											//Log(pLogFile, "�ϼҳ�����\n");
											eCardType = pkCardAi->getCardOut(CardUserInfo,CardInDeskInfo,eCardType,CardSlectInfo); //ѹ��
											mtServiceRoomCardOut kServiceRoomCardOut;
											if (eCardType == MT_FL_CARD_TYPE_PASS) //����Ҫ���� �������� ֻ��ע��������  ������ƺ��� ������㴫һ������
											{
												char *pCardOut = pkCardOutDataRead->pcCard;  //�õ������ϸոճ�����
												Log(pLogFile, "�Լ����ͳ���Э�飬���ݣ���\n");
												kServiceRoomCardOut.init(MT_FL_CARD_TYPE_PASS,pCardOut); //*********
												if (!(pkNetWork->WriteData((char*)(&kServiceRoomCardOut.m_kCardOutDataWrite),sizeof(kServiceRoomCardOut.m_kCardOutDataWrite))))
												{
												SaftyRelease(pkNetWork);
												SaftyRelease(pkCardAi);
												if (clientSocket != SOCKET_ERROR)
												{
													closesocket(clientSocket);
												}
												Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
												return 0;
												}
											}
											else                                   //����ѹ���� ����Э���ȥ
											{
											
												pkCardAi->cardConvert(CardSlectInfo,CardSlect);//ת���ɷ���˵���
												kServiceRoomCardOut.init(eCardType,CardSlect); 
												Sleep(SLEEP_CARD_OUT_TIME); //��Ϣһ��
												if (!(pkNetWork->WriteData((char*)(&kServiceRoomCardOut.m_kCardOutDataWrite),sizeof(kServiceRoomCardOut.m_kCardOutDataWrite))))
												{
													SaftyRelease(pkNetWork);
													SaftyRelease(pkCardAi);
													if (clientSocket != SOCKET_ERROR)
													{
														closesocket(clientSocket);
													}
													Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
													return 0;
												}
												//����һ���Լ�������
												Log(pLogFile,"�Լ����ͳ���Э��,����:");
												pkCardAi->dispalyCardInfo(CardSlectInfo,pLogFile);
												pkCardAi->takeAwayCard(CardUserInfo,CardSlectInfo);
												pkCardAi->sortCard(CardUserInfo);
												Log(pLogFile, "�Լ�ʣ������:");
												pkCardAi->dispalyCardInfo(CardUserInfo,pLogFile);															
											}
										}							
									}						
								}
								else if (lUserId == lUserAiId)                            //�Լ�������
								{
									
									//�ж�һ�� �Լ����е���  
									if (!pkCardAi->getCardNum(CardUserInfo))  //����û������
									{
										//���� ����һ�ֵ�Э���ȥ
									    
										mtServiceRoomRoundOver kServiceRoomRoundOver;
										kServiceRoomRoundOver.init();
										if (!(pkNetWork->WriteData((char*)(&kServiceRoomRoundOver.m_kRoundOverDataWrite),sizeof(kServiceRoomRoundOver.m_kRoundOverDataWrite))))
										{
											SaftyRelease(pkNetWork);
											SaftyRelease(pkCardAi);
											if (clientSocket != SOCKET_ERROR)
											{
												closesocket(clientSocket);
											}
											Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
											return 0;
										}
										Log(pLogFile, "�Լ�����ʣ����Ϊ0�����ͽ���һ��Э���ȥ\n");
									}					
								}
							}
							break;
						case E_SERVICE_ROOM_ROUND_OVER:                                //�ӵ�������һ����Э��  �����˳�����Э��
							{
								Log(pLogFile, "���ս���һ��Э��\n");
								Log(pLogFile,"\n*******��%d�ֵ�%d�ֽ���,�յ�����һ��Э��,�ϼ�id:%d,�¼�id:%d,����id:%x*******\n",iCompleteLunShu,iCompleteRound + 1,lLastAiId,lNextAiId,lDeskId);							
								bStartFlag = false;                                      //��ʼ��־ �� false
								iCompleteRound++;                                       //��ɾ��� ��1
								if (iCompleteRound == iRoundTotal)                      //�ﵽ������Ҫ��  ���˳�����
								{
									//ֱ�ӷ����˳�����Э�� 
									mtServiceRoomExit kServiceRoomExit;
									kServiceRoomExit.init();
									if (!(pkNetWork->WriteData((char*)(&kServiceRoomExit.m_kExitDataWrite),sizeof(kServiceRoomExit.m_kExitDataWrite))))
									{
										SaftyRelease(pkNetWork);
										SaftyRelease(pkCardAi);
										if (clientSocket != SOCKET_ERROR)
										{
											closesocket(clientSocket);
										}
										Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
										return 0;
									}
									Log(pLogFile, "�����˳�Э��\n");
								}
								else
								{
									iLastAiCardNum = 17;
									iNextAiCardNum = 17;
									lRealOutCardPlay = 0;
									for (int i = 0; i<24; i++) 
									{
										CardInDeskInfo[i].cCardType = 0;
										CardInDeskInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
									}
									//���Ϳ�ʼЭ��
									mtServiceRoomWaitStart kServiceRoomWaitStart;
									kServiceRoomWaitStart.init();
									Sleep(SLEEP_CALL_START_TIME);                   //������
									if(!(pkNetWork->WriteData((char*)(&kServiceRoomWaitStart.m_kWaitStartDataWrite),sizeof(kServiceRoomWaitStart.m_kWaitStartDataWrite))))
									{
										SaftyRelease(pkNetWork);
										SaftyRelease(pkCardAi);
										if (clientSocket != SOCKET_ERROR)
										{
											closesocket(clientSocket);
										}
										Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
										return 0;
									}
									bStartFlag = true;
									Log(pLogFile, "���ͽ���һ�ֵ����¿�ʼЭ��\n");
								}					
							}
							break;
						case E_SERVICE_ROOM_EXIT:                                           //�˳�����Э��
							{
								SYSTEMTIME curTime;
								GetLocalTime(&curTime);

								mtServiceRoomExit::DataRead *pkExitDataRead = (mtServiceRoomExit::DataRead*)lpCurPos;
								if (lUserAiId == pkExitDataRead->lUserId)//���Լ����˳�Э��
								{
									Log(pLogFile, "�ӵ��Լ��˳�����Э�飬׼����������  %d:%d:%d\n",curTime.wHour,curTime.wMinute,curTime.wSecond);
									closesocket(clientSocket);
									Sleep(1000*3);//��ͣ���� ������        						
									goto step1;
						
								}
								else  //�����˳�������  
								{
								
									Log(pLogFile, "�ӵ���ң�%d�˳�����Э��,�Ҳ�������  %d:%d:%d\n",pkExitDataRead->lUserId,curTime.wHour,curTime.wMinute,curTime.wSecond);
								}
							}
							break;
						default:
							Log(pLogFile, "���Э��:%d\n",lpCurPos[1]);
							break;
						}

						break;
					}
					else
					{
						//���ý���Э�鴦����
						switch (lpCurPos[1]) //�õ�Э�������
						{
						case E_SERVICE_ROOM_CONNECT:                         //����
							{
								Log(pLogFile, "�ӵ�����Э��\n");
								mtServiceRoomConnect::DataRead *pkConnectDataRead = (mtServiceRoomConnect::DataRead*)(lpCurPos);
								iResult = pkConnectDataRead->lResult;
								if (iResult == 1)  //���ӳɹ���
								{
									Log(pLogFile,"���ӳɹ�\n");
								}
								else       
								{
									Log(pLogFile, "�����쳣����:%d\n",iResult);
									SaftyRelease(pkNetWork);
									SaftyRelease(pkCardAi);
									if (clientSocket != SOCKET_ERROR)
									{
										closesocket(clientSocket);
									}
									Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
									return 0;
								}					
							}
							break;
						case E_SERVICE_ROOM_DESK_TWO:          //�齨˫������
							{
								Log(pLogFile, "�ӵ��齨��˫������Э��\n");
								if (!bStartFlag)
								{
									mtServiceRoomWaitStart kServiceRoomWaitStart;
									kServiceRoomWaitStart.init();
									if(!(pkNetWork->WriteData((char*)(&kServiceRoomWaitStart.m_kWaitStartDataWrite),sizeof(kServiceRoomWaitStart.m_kWaitStartDataWrite))))
									{
										SaftyRelease(pkNetWork);
										SaftyRelease(pkCardAi);
										if (clientSocket != SOCKET_ERROR)
										{
											closesocket(clientSocket);
										}
										Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
										return 0;
									}
									Log(pLogFile, "�����齨��˫�����ӿ�ʼЭ��\n");
									bStartFlag = true;
								}								
							}
							break;
						case E_SERVICE_ROOM_DESK_THREE:       //�齨�������� 
							{
								Log(pLogFile,"�ӵ��齨����������Э��\n");
								mtServiceRoomDeskThree::DataRead *pkDeskThreeDataRead = (mtServiceRoomDeskThree::DataRead*)(lpCurPos);
								lLastAiId = pkDeskThreeDataRead->kUserInfo[0].lUserId;  //�õ��ϼ�id
								lNextAiId = pkDeskThreeDataRead->kUserInfo[1].lUserId;  //�õ��¼�id
								lDeskId   = pkDeskThreeDataRead->kUserInfo[0].ulDeskId; //�õ�����id
								Log(pLogFile,"\n*******��%d�ֵ�%d�ֿ�ʼ,�յ��������ӽ���Э��,�ϼ�id:%d,�¼�id:%d,����id:%x*******\n",iCompleteLunShu,iCompleteRound + 1,lLastAiId,lNextAiId,lDeskId);							
								if (!bStartFlag)
								{
									mtServiceRoomWaitStart kServiceRoomWaitStart;
									kServiceRoomWaitStart.init();
									if(!(pkNetWork->WriteData((char*)(&kServiceRoomWaitStart.m_kWaitStartDataWrite),sizeof(kServiceRoomWaitStart.m_kWaitStartDataWrite))))
									{
										SaftyRelease(pkNetWork);
										SaftyRelease(pkCardAi);
										if (clientSocket != SOCKET_ERROR)
										{
											closesocket(clientSocket);
										}
										Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
										return 0;
									}
									Log(pLogFile, "�����齨���������ӿ�ʼЭ��\n");
									bStartFlag = true;
								}
							}
							break;
						case E_SERVICE_ROOM_WAIT_START:       //��ʼЭ��    
							{
								mtServiceRoomWaitStart::DataRead *pkWaitStartDataRead = (mtServiceRoomWaitStart::DataRead*)(lpCurPos);
								Log(pLogFile, "�ӵ���ʼЭ��%d\n",pkWaitStartDataRead->lUserId);
								 //Ҫ����һ�����浱ǰ������Ϣ  ��Ϊ���¿�ʼ �ᵽ������ ����������һ������
								iLastAiCardNum = 17;
								iNextAiCardNum = 17;
								lRealOutCardPlay = 0;
								for (int i = 0; i<24; i++) 
								{
									CardInDeskInfo[i].cCardType = 0;
									CardInDeskInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
								}
							}
							break;
						case E_SERVICE_ROOM_CARD_DEAL:         //�ƹ����� �ӵ��� 
							{
								Log(pLogFile, "�ӵ�ϴ�Ʒ���Э��\n");
								mtServiceRoomCardDealRecv::DataRead *pkCardDealRecvDataRead = (mtServiceRoomCardDealRecv::DataRead*)(lpCurPos);
								// �ȴ����
								for (int i = 0; i<24; i++)
								{
									CardUserInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
									CardUserInfo[i].cCardType = 0;
									CardUserCard[i] = MTFL_NUM_CARD_TERMINAL;
									if (i<7)
									{
										CardBaseCard[i] = MTFL_NUM_CARD_TERMINAL;
									}
								}
								memcpy(CardUserCard,pkCardDealRecvDataRead->pcCards,sizeof(char)*17);
								memcpy(CardBaseCard,pkCardDealRecvDataRead->pcBaseCards,sizeof(char)*7);
								pkCardAi->cardConvert(CardUserCard,CardUserInfo);
								pkCardAi->sortCard(CardUserInfo);
								Log(pLogFile, "�Լ������ǣ�\n");
								pkCardAi->dispalyCardInfo(CardUserInfo,pLogFile);
								if( pkCardDealRecvDataRead->lDefaultLandlordUserId == lUserAiId )  // �Լ���ѡ��������
								{
									//����һ����������Э��
									
									Log(pLogFile, "�Լ���ѡ��������\n");
									mtServiceRoomGrabLandlord kServiceRoomGrabLandlord;
									kServiceRoomGrabLandlord.init();
									if (!(pkNetWork->WriteData((char*)&(kServiceRoomGrabLandlord.m_kGrablandlordDataWrite),sizeof(kServiceRoomGrabLandlord.m_kGrablandlordDataWrite))))
									{
										SaftyRelease(pkNetWork);
										SaftyRelease(pkCardAi);
										if (clientSocket != SOCKET_ERROR)
										{
											closesocket(clientSocket);
										}
										Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
										return 0;
									}
									Log(pLogFile, "����������Э�� 3��\n");
								}
								else                                                         //Ĭ���������Ĳ����Լ�
								{
									Log(pLogFile, "�ȴ�������\n");
								}

							}
							break;
						case E_SERVICE_ROOM_GRAB_LANDLORD:                                   //������Э��
							{
								Log(pLogFile,"�ӵ�������Э��\n");
								mtServiceRoomGrabLandlord::DataRead *pkGrabLandLordDataRead = (mtServiceRoomGrabLandlord::DataRead*)(lpCurPos);
								if (pkGrabLandLordDataRead->lUserIdLandlord > 0)   //�Ѿ��е�����
								{
									Log(pLogFile,"��������\n");
									
									if ( pkGrabLandLordDataRead->lUserIdLandlord == lUserAiId )     // �Լ������˵��� ����Ҫ�ӽ��Լ����������� ��Ҫ����һ��                                                    
									{
										//���ͳ���Э��
										Log(pLogFile,"�Լ��ǵ���\n");
										for (int i = 0; i<24; i++)
										{
											CardUserInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
											CardUserInfo[i].cCardType = 0;
											CardSlectInfo[i].cCardType = 0;
											CardSlectInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
											CardSlect[i] = MTFL_NUM_CARD_TERMINAL;
											if (i<7)
											{
												CardBaseInfo[i].cCardType = 0;
												CardBaseInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
											}
										}
										pkCardAi->cardConvert(CardUserCard,CardUserInfo);     //���������� ת��Ϊ�ͻ��˵���
										pkCardAi->cardConvert(CardBaseCard,CardBaseInfo);
										pkCardAi->addInCard(CardUserInfo,CardBaseInfo);      //��� ����  ���е�ֻҪ��֤�� �������� MTFL_NUM_CARD_TERMINAL ���� �Ͱ�ȫ��
										pkCardAi->sortCard(CardUserInfo);                    //����һ��
										//ִ�е�һ�γ��Ʋ��� ������Э���ȥ
										pkCardAi->getCardOut(CardUserInfo,NULL,MT_FL_CARD_TYPE_BEG,CardSlectInfo);//�õ����Ƶ�����
										mteCardType eCardType = pkCardAi->getTypeFromCard(CardSlectInfo);          //�õ����Ƶ�����
										pkCardAi->cardConvert(CardSlectInfo,CardSlect); //������ת��һ�� �������͵������
										mtServiceRoomCardOut kServiceRoomCardOut;
										kServiceRoomCardOut.init(eCardType,CardSlect);
										Sleep(SLEEP_CARD_OUT_TIME);
										if (!(pkNetWork->WriteData((char*)(&kServiceRoomCardOut.m_kCardOutDataWrite),sizeof(kServiceRoomCardOut.m_kCardOutDataWrite))))
										{
											SaftyRelease(pkNetWork);
											SaftyRelease(pkCardAi);
											if (clientSocket != SOCKET_ERROR)
											{
												closesocket(clientSocket);
											}
											Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
											return 0;
										}
										Log(pLogFile, "���ͳ���Э�飬���ݣ�");
										pkCardAi->dispalyCardInfo(CardSlectInfo,pLogFile);
										//����һ���Լ�������
										pkCardAi->takeAwayCard(CardUserInfo,CardSlectInfo);
										pkCardAi->sortCard(CardUserInfo);
										Log(pLogFile, "�Լ�ʣ����ƣ�");
										pkCardAi->dispalyCardInfo(CardUserInfo,pLogFile);
									}
									else if(pkGrabLandLordDataRead->lUserIdLandlord == lLastAiId)
									{
										Log(pLogFile, "�������ϼ�\n");
										iLastAiCardNum = 20;
									}
									else if(pkGrabLandLordDataRead->lUserIdLandlord == lNextAiId)
									{
										Log(pLogFile, "�������¼�\n");
										iNextAiCardNum = 20;
									}
									else
									{
										cout<<"****"<<endl;
									}
								}
								if (pkGrabLandLordDataRead->lUserIdLandlord == -1)  //��û�в�������
								{
									//Log(pLogFile, "������û�в���\n");
									if (pkGrabLandLordDataRead->lUserId == lLastAiId) //����һ�ҽеĵ���
									{
										//�Լ�������
										//Log(pLogFile,"�ϼ�������,����:%d\n",pkGrabLandLordDataRead->lScore);
										mtServiceRoomGrabLandlord kServiceRoomGrabLandlord;
										kServiceRoomGrabLandlord.init();
										if (!(pkNetWork->WriteData((char*)&(kServiceRoomGrabLandlord.m_kGrablandlordDataWrite),sizeof(kServiceRoomGrabLandlord.m_kGrablandlordDataWrite))))
										{
											SaftyRelease(pkNetWork);
											SaftyRelease(pkCardAi);
											if (clientSocket != SOCKET_ERROR)
											{
												closesocket(clientSocket);
											}
											Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
											return 0;
										}
										Log(pLogFile, "����������Э�� 3��\n");
									}
									else
									{
										//cout<<"�¼�������������"<<endl;
									}
						
								}
							}
							break;
						case E_SERVICE_ROOM_CARD_OUT:                                     //����Э�� 
							{
								
								mtServiceRoomCardOut::DataRead *pkCardOutDataRead = (mtServiceRoomCardOut::DataRead*)(lpCurPos);
								mteCardType eCardType = (mteCardType)pkCardOutDataRead->lCardType; //�õ����Ƶ�����
								long lUserId = pkCardOutDataRead->lUserId; //�õ������˵�id�����Ƶ�Ҳ�㣩
								int iCardOutNum = 0;                 //��ǰ���Ƶ�����
								for (int i = 0; i<24; i++)
								{
									CardSlectInfo[i].cCardType = 0;
									CardSlectInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
									CardSlect[i] = MTFL_NUM_CARD_TERMINAL;
								}
								if (eCardType != MT_FL_CARD_TYPE_PASS) //������ǹ���  ��Ҫ����һ�����ӵ�ǰ������Ϣ ������ʵ�ĳ����ˣ�����ʵ�Ƶ��ˣ�
								{
									char *pCardOut = pkCardOutDataRead->pcCard;  //�õ������ϸոճ�����
									for (int i = 0; i<24; i++)
									{
										CardInDeskInfo[i].cCardType = 0;
										CardInDeskInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;	
									}
									pkCardAi->cardConvert(pCardOut,CardInDeskInfo);//ת���ɿͻ��˵������� ��CardInDeskInfo��
									pkCardAi->sortCard(CardInDeskInfo);
									Log(pLogFile,"���ճ���Э�飬�����ˣ�%d,���ݣ�",lUserId);
									pkCardAi->dispalyCardInfo(CardInDeskInfo,pLogFile);
									lRealOutCardPlay = pkCardOutDataRead->lUserId;//����һ�� �����ĳ������ ���Ĳ��� 
									iCardOutNum = pkCardAi->getCardNum(CardInDeskInfo); //�õ����Ƶ�����
								}
								else                                                                //����
								{
									//��CardInDeskInfo ������һ���������
									Log(pLogFile,"���ճ���Э�飬�����ˣ�%d,����:��\n",lUserId);


								}
								Log(pLogFile,"���ӵ�ǰ���棺");
								pkCardAi->dispalyCardInfo(CardInDeskInfo,pLogFile);

								if (lUserId == lNextAiId)                                //�¼ҳ�����  �п���Ҫ�������һ��Э���ȥ
								{
									iNextAiCardNum -= iCardOutNum;
									if (iNextAiCardNum <= 0)                         //�¼��Ѿ���������  ���ͽ���һ��Э�� Ҳ���ܻᷢ�������˼�����Э�� �ٿ�
									{
										
										Log(pLogFile,"�¼����е����Ѿ�����\n");
										mtServiceRoomRoundOver kServiceRoomRoundOver;
										kServiceRoomRoundOver.init();
										if (!(pkNetWork->WriteData((char*)(&kServiceRoomRoundOver.m_kRoundOverDataWrite),sizeof(kServiceRoomRoundOver.m_kRoundOverDataWrite))))
										{
											SaftyRelease(pkNetWork);
											SaftyRelease(pkCardAi);
											if (clientSocket != SOCKET_ERROR)
											{
												closesocket(clientSocket);
											}
											Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
											return 0;
										}
										Log(pLogFile, "�Լ����ͽ���һ��Э��\n");
									}
									else
									{
										
										//Log(pLogFile, "���¼ҳ����ƣ���Ҫ\n");
									}
								}
								else if (lUserId == lLastAiId)                            //�ϼҳ����� ��Ҫѹ��
								{
									if (lUserAiId == lRealOutCardPlay)           //�ϴ��������Ƶ����Լ�
									{
										//�ж�һ���Լ����е���
										if (!pkCardAi->getCardNum(CardUserInfo))  //����û������
										{
											//���� ����һ�ֵ�Э���ȥ
											
											Log(pLogFile, ",�Լ����е��ƴ���\n");
											mtServiceRoomRoundOver kServiceRoomRoundOver;
											kServiceRoomRoundOver.init();
											if (!(pkNetWork->WriteData((char*)(&kServiceRoomRoundOver.m_kRoundOverDataWrite),sizeof(kServiceRoomRoundOver.m_kRoundOverDataWrite))))
											{
												SaftyRelease(pkNetWork);
												SaftyRelease(pkCardAi);
												if (clientSocket != SOCKET_ERROR)
												{
													closesocket(clientSocket);
												}
												Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
												return 0;
											}
											Log(pLogFile, "���ͽ���һ��Э��\n");

										}
										else //������ ��������
										{
											
											//ִ�е�һ�γ��Ʋ��� ������Э���ȥ
											pkCardAi->getCardOut(CardUserInfo,NULL,MT_FL_CARD_TYPE_BEG,CardSlectInfo);//�õ����Ƶ�����
											mteCardType eCardType = pkCardAi->getTypeFromCard(CardSlectInfo);          //�õ����Ƶ�����
											pkCardAi->cardConvert(CardSlectInfo,CardSlect); //������ת��һ�� �������͵������
											mtServiceRoomCardOut kServiceRoomCardOut;
											kServiceRoomCardOut.init(eCardType,CardSlect);
											Sleep(SLEEP_CARD_OUT_TIME); //��Ϣһ��
											if (!(pkNetWork->WriteData((char*)(&kServiceRoomCardOut.m_kCardOutDataWrite),sizeof(kServiceRoomCardOut.m_kCardOutDataWrite))))
											{
												SaftyRelease(pkNetWork);
												SaftyRelease(pkCardAi);
												if (clientSocket != SOCKET_ERROR)
												{
													closesocket(clientSocket);
												}
												Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
												return 0;
											}
											Log(pLogFile, "�Լ����ͳ���Э��,���ݣ�");
											pkCardAi->dispalyCardInfo(CardSlectInfo,pLogFile);
											//����һ���Լ�������
											pkCardAi->takeAwayCard(CardUserInfo,CardSlectInfo);
											pkCardAi->sortCard(CardUserInfo);
											Log(pLogFile, "�Լ����е���,���ݣ�");
											pkCardAi->dispalyCardInfo(CardUserInfo,pLogFile);
										}
									}
									else //�������ƵĲ����Լ� �п����� �ϼ� Ҳ�п������¼� ��������һ�� �Ҷ�Ҫѹ��
									{
										iLastAiCardNum -= iCardOutNum; //�õ��ϼ����е���
										if (iLastAiCardNum <= 0)                         //�ϼ��Ѿ���������  ���ͽ���һ��Э�� Ҳ���ܻᷢ�������˼�����Э�� �ٿ�
										{
											Log(pLogFile, "�ϼ����е����Ѿ�����\n");
											mtServiceRoomRoundOver kServiceRoomRoundOver;
											kServiceRoomRoundOver.init();
											if (!(pkNetWork->WriteData((char*)(&kServiceRoomRoundOver.m_kRoundOverDataWrite),sizeof(kServiceRoomRoundOver.m_kRoundOverDataWrite))))
											{
												SaftyRelease(pkNetWork);
												SaftyRelease(pkCardAi);
												if (clientSocket != SOCKET_ERROR)
												{
													closesocket(clientSocket);
												}
												Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
												return 0;
											}
											Log(pLogFile, "�Լ����ͽ���һ��Э���ȥ\n");
										}
										else
										{
											
											eCardType = pkCardAi->getCardOut(CardUserInfo,CardInDeskInfo,eCardType,CardSlectInfo); //ѹ��
											mtServiceRoomCardOut kServiceRoomCardOut;
											if (eCardType == MT_FL_CARD_TYPE_PASS) //����Ҫ���� �������� ֻ��ע��������  ������ƺ��� ������㴫һ������
											{
												char *pCardOut = pkCardOutDataRead->pcCard;  //�õ������ϸոճ�����
												
												Log(pLogFile, "�Լ����ͳ���Э��,���ݣ���\n");
												kServiceRoomCardOut.init(MT_FL_CARD_TYPE_PASS,pCardOut); //*********
												if (!(pkNetWork->WriteData((char*)(&kServiceRoomCardOut.m_kCardOutDataWrite),sizeof(kServiceRoomCardOut.m_kCardOutDataWrite))))
												{
												SaftyRelease(pkNetWork);
												SaftyRelease(pkCardAi);
												if (clientSocket != SOCKET_ERROR)
												{
													closesocket(clientSocket);
												}
												Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
												return 0;
												}
											}
											else                                   //����ѹ���� ����Э���ȥ
											{
												
												pkCardAi->cardConvert(CardSlectInfo,CardSlect);//ת���ɷ���˵���
												kServiceRoomCardOut.init(eCardType,CardSlect); 
												Sleep(SLEEP_CARD_OUT_TIME); //��Ϣһ��
												if (!(pkNetWork->WriteData((char*)(&kServiceRoomCardOut.m_kCardOutDataWrite),sizeof(kServiceRoomCardOut.m_kCardOutDataWrite))))
												{
													SaftyRelease(pkNetWork);
													SaftyRelease(pkCardAi);
													if (clientSocket != SOCKET_ERROR)
													{
														closesocket(clientSocket);
													}
													Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
													return 0;
												}
												Log(pLogFile, "�Լ����ͳ���Э�飬���ݣ�");
											
												
												pkCardAi->dispalyCardInfo(CardSlectInfo,pLogFile);
												pkCardAi->takeAwayCard(CardUserInfo,CardSlectInfo);
												pkCardAi->sortCard(CardUserInfo);
												Log(pLogFile, "�Լ�ʣ�������ǣ����ݣ�");
												pkCardAi->dispalyCardInfo(CardUserInfo,pLogFile);															
											}
										}							
									}						
								}
								else if (lUserId == lUserAiId)                            //�Լ�������
								{
									
									//�ж�һ�� �Լ����е���  
									if (!pkCardAi->getCardNum(CardUserInfo))  //����û������
									{
										//���� ����һ�ֵ�Э���ȥ
										mtServiceRoomRoundOver kServiceRoomRoundOver;
										kServiceRoomRoundOver.init();
										if (!(pkNetWork->WriteData((char*)(&kServiceRoomRoundOver.m_kRoundOverDataWrite),sizeof(kServiceRoomRoundOver.m_kRoundOverDataWrite))))
										{
											SaftyRelease(pkNetWork);
											SaftyRelease(pkCardAi);
											if (clientSocket != SOCKET_ERROR)
											{
												closesocket(clientSocket);
											}
											Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
											return 0;
										}
										Log(pLogFile, "�Լ������ƴ��꣬���ͽ���һ��Э��\n");
									}					
								}
							}
							break;
						case E_SERVICE_ROOM_ROUND_OVER:                                //�ӵ�������һ����Э��  �����˳�����Э��
							{
								Log(pLogFile, "���ս���һ��Э��\n");
								Log(pLogFile,"\n*******��%d�ֵ�%d�ֽ���,�յ�����һ��Э��,�ϼ�id:%d,�¼�id:%d,����id:%x*******\n",iCompleteLunShu,iCompleteRound + 1,lLastAiId,lNextAiId,lDeskId);							
								bStartFlag = false;                                      //��ʼ��־ �� false
								iCompleteRound++;                                       //��ɾ��� ��1
								if (iCompleteRound == iRoundTotal)                      //�ﵽ������Ҫ��  ���˳�����
								{
									//ֱ�ӷ����˳�����Э�� 
									mtServiceRoomExit kServiceRoomExit;
									kServiceRoomExit.init();
									if (!(pkNetWork->WriteData((char*)(&kServiceRoomExit.m_kExitDataWrite),sizeof(kServiceRoomExit.m_kExitDataWrite))))
									{
										SaftyRelease(pkNetWork);
										SaftyRelease(pkCardAi);
										if (clientSocket != SOCKET_ERROR)
										{
											closesocket(clientSocket);
										}
										Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
										return 0;
									}
									Log(pLogFile, "�����˳�����Э��\n");
								}
								else
								{
									iLastAiCardNum = 17;
									iNextAiCardNum = 17;
									lRealOutCardPlay = 0;
									for (int i = 0; i<24; i++) 
									{
										CardInDeskInfo[i].cCardType = 0;
										CardInDeskInfo[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
									}
									//���Ϳ�ʼЭ��
									mtServiceRoomWaitStart kServiceRoomWaitStart;
									kServiceRoomWaitStart.init();
									Sleep(SLEEP_CALL_START_TIME);
									if(!(pkNetWork->WriteData((char*)(&kServiceRoomWaitStart.m_kWaitStartDataWrite),sizeof(kServiceRoomWaitStart.m_kWaitStartDataWrite))))
									{
										SaftyRelease(pkNetWork);
										SaftyRelease(pkCardAi);
										if (clientSocket != SOCKET_ERROR)
										{
											closesocket(clientSocket);
										}
										Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
										return 0;
									}
									bStartFlag = true;
									Log(pLogFile,"���ͽ���һ�ֵ����¿�ʼ��Э��\n");
								}					
							}
							break;
						case E_SERVICE_ROOM_EXIT:                                           //�˳�����Э��
							{
								mtServiceRoomExit::DataRead *pkExitDataRead = (mtServiceRoomExit::DataRead*)lpCurPos;
								if (lUserAiId == pkExitDataRead->lUserId)//���Լ����˳�Э��
								{
									Log(pLogFile,"�ӵ��Լ��˳�����Э�飬׼����������\n");
									closesocket(clientSocket);
									Sleep(1000 * 3);//��ͣ���� ������        						
									goto step1;
						
								}
								else  //
								{
									Log(pLogFile,"�ӵ����:%d�˳�����Э��\n",pkExitDataRead->lUserId);
								}
							}
							break;
						default:
							Log(pLogFile, "���Э��%d\n",lpCurPos[1]);
							break;
						}
						lpCurPos = (long*)(buffer + lDataSize);
						lDataSize += lpCurPos[0];
					}
				}
			}
			else if (iReturn == E_ERROR_SELECT_NONE)
			{
				//����
				
			}
			else
			{
				Log(pLogFile, "�������ݴ���\n");
				SaftyRelease(pkNetWork);
				SaftyRelease(pkCardAi);
				if (clientSocket != SOCKET_ERROR)
				{
					closesocket(clientSocket);
				}
				Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
				return 0;
			}
	    }
	}
	SaftyRelease(pkNetWork);
	SaftyRelease(pkCardAi);
	if (clientSocket != SOCKET_ERROR)
	{
		closesocket(clientSocket);
	}
	Log(pLogFile,"�߳��˳� ����:%d\n",__LINE__);
	return 0;

}

void SaftyRelease(void *p)  //�ͷ�ָ�� 
{
	if (p == NULL)
	{
		return ;
	}
	else
	{
		delete p;
		if (p != NULL)
		{
			p = NULL;
		}
	}
}

bool initNetModule()
{
	//��ʼ������ģ��
	WSADATA  wsadata;
	if( WSAStartup(MAKEWORD(2,2),&wsadata) != 0 )
	{
		cout<<"Init Windows Socket Fail"<<endl;
		return false;
	}
	//����������ַ 
	memset(&serverAddress,0,sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(IP);   
	serverAddress.sin_port = htons(PORT);
	return true;
}

void  initThreadModule()
{
	HANDLE hThreadWork[THREAD_NUMBER];
	for(int i = 0; i<THREAD_NUMBER; i++)
	{
		hThreadWork[i] = CreateThread(0,0,ThreadWorkFun,NULL,0,0);
		Sleep(500);
		CloseHandle(hThreadWork[i]);
	}
}

void Log( FILE* pFiLE,char * pcformat, ... )  //��־����
{
	va_list kArgs;
	int     iLen;
	char    pcBuffer[1024];

	memset(pcBuffer, 0, sizeof(pcBuffer));
	va_start(kArgs, pcformat);
	iLen = _vscprintf(pcformat, kArgs) ;
	vsprintf(pcBuffer, pcformat, kArgs);

	if (NULL == pFiLE)
	{
		return ;

    }
	fwrite(pcBuffer, iLen, 1, pFiLE);
	fflush(pFiLE);
	va_end(kArgs);
	printf(pcBuffer);
}

bool GetRunExeDir(char* pDir, DWORD dwLen)  //�õ���ǰ���г����·�����浽����ָ������
{
	GetModuleFileName(NULL, pDir, dwLen);//���ص�ǰӦ�ó���ȫ·������Ϊapi����
	bool flag = NULL;
	int iLen = strlen(pDir);
	if (0 >= iLen)
	{
		return FALSE;
	}

	//ȡ���ļ�·��  
	for(int i = iLen; i >= 0; i--)
	{
		if(pDir[i] == '\\')
		{   
			pDir[i] = '\0';
			break;
		} 
	}

	return TRUE;
}

int WSDCreateDirectory(const char* pcDir)    //�½�Ŀ¼ ����Ϊȫ·�� ��ʵ�����½�һ���ļ���
{
	char pcDirTmp[256];
	strcpy(pcDirTmp, pcDir);

	char* pcOne = strchr(pcDirTmp, '\\');    
	while(pcOne)
	{
		*pcOne = '\0';
		_mkdir(pcDirTmp);
		*pcOne = '\\';
		pcOne = strchr(pcOne + 1, '\\');
	}

	_mkdir(pcDirTmp);

	return 1;
}
								
int ReadData(SOCKET socket, char *pcBuffer, long bufferSize)
{
	timeval		kTimeVal;  
	kTimeVal.tv_sec = 0;
	kTimeVal.tv_usec = 1000*10; //�ȴ�ʮ����
	fd_set		m_kFdSet;
	int 		iRetRead	= 0;
	FD_ZERO(&m_kFdSet);
	FD_SET(socket, &m_kFdSet);
	int 		iRetSelect		= select (socket+1, &m_kFdSet, NULL, NULL, &kTimeVal);
	if (1 == iRetSelect)   //
	{
		iRetRead	= recv(socket, pcBuffer, bufferSize,0);
		if (iRetRead > (sizeof(long)<<2))  //�ӵ������ݴ��� 16���ֽ�
		{
			return 	iRetRead;  //���ض������ֽ���
		}
		
	}
	if (0 == iRetSelect)   //û�пɶ����׽���
	{
		return 	E_ERROR_SELECT_NONE;
	}
	else if(1 == iRetSelect && 0 == iRetRead)  //��������Ϊ0
	{
		return E_ERROR_NETWORK_CLOSE;
	}
	else if (1 == iRetSelect && -1 == iRetRead)
	{
		return 	E_ERROR_READ;
	} 
	else if (1 == iRetSelect) 
	{
		return 	E_ERROR_READ_BYTES;
	} 
	else 
	{
		return 	E_ERROR_SELECT;
	}


}
