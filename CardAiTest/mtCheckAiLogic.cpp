//#include "mtCardAi.h"
//#include<iostream>
//using namespace std;
////#define DEBUG   //��������̶����� ר�Ÿ������˹̶����ƣ��Լ�ѡ��
//
//int   main()
//{  
//#if 0
//	mtCardInfo pkCardTakeout[21]; //�������ϸոճ�����
//	mtCardInfo pkCardDst[21];  //��ѡ�������� 
//	for (int i = 0; i<21; i++)
//	{
//		pkCardTakeout[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
//	}
//	for (int i = 0; i<21; i++)
//	{
//		pkCardDst[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
//	}
//	mtCardAI* pkCardAI = new mtCardAI;
//	if (!pkCardAI)
//	{
//		delete pkCardAI;
//		return 0;
//	}
//	mtCardAI* pkCardMan = new mtCardAI;  //  ���õ�  �õ�������� 
//	if (!pkCardMan)
//	{
//		delete pkCardMan;
//		delete pkCardAI;
//		return 0;
//	}
//#ifdef DEBUG
//	mtCardInfo kCardInfoAI[21] = {
//		{0,2},{1,3},{1,3},{3,4},{0,6},
//		{0,6},{1,7},{0,7},{2,8},{3,8},
//		{2,9},{1,9},{2,10},{3,10},{0,11},
//		{2,12},{3,13},
//		{0,MTFL_NUM_CARD_TERMINAL},
//		{0,MTFL_NUM_CARD_TERMINAL},
//		{0,MTFL_NUM_CARD_TERMINAL},
//		{0,MTFL_NUM_CARD_TERMINAL}
//	};                                            //�����ҳ�ʼ��
//#else
//	mtCardInfo kCardInfoAI[21];  //��������õ���
//
//#endif // DEBUG
//
//	while (1)
//	{
//		cout<<"ϴ�ƣ�1,������0"<<endl;
//		bool bFlag = false;
//		cin>>bFlag;
//		if (bFlag == false)
//		{
//			return 0;
//		}
//		else //ϴ��
//		{
//#ifndef DEBUG   //
//			for (int i = 0; i<21; i++)
//			{
//				kCardInfoAI[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
//			}
//			pkCardAI->shuffle();  //ϴ����
//			memcpy(kCardInfoAI,pkCardAI->m_pkASetOfCards,17*sizeof(mtCardInfo)); //����ϴ�õ��Ƶ�ǰ��17���� �������˴�������
//#endif // DEBUG			
//			pkCardAI->sortCard(kCardInfoAI); //����һ��
//			cout<<"�����˵��������£�";
//			pkCardAI->dispalyCardInfo(kCardInfoAI);
//			while (pkCardAI->getCardNum(kCardInfoAI)) //������û������
//			{
//				for (int i = 0; i<21; i++)
//				{
//					pkCardTakeout[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
//				}
//				for (int i = 0; i<21; i++)
//				{
//					pkCardDst[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
//				}
//				cout<<"��ѡ��˭�ȳ��ƣ�1 �㣬0 ������"<<endl;
//				int iWhoCardOut = -1;
//				cin>>iWhoCardOut;
//				switch (iWhoCardOut)
//				{
//				case 1:                    //�����
//					{
//sign1:				    cout<<"������������Ҫ���Ƶ��±�,�����100������"<<endl;
//						for (int i = 0; i<21; i++)
//						{
//							pkCardTakeout[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
//						}
//						int a= 0;
//						int i = 0;
//						while(cin>>a)
//						{
//							if (a == 100)
//							{
//								break;
//							}
//							memcpy(&pkCardTakeout[i],pkCardMan->m_pkASetOfCards+a,sizeof(mtCardInfo));
//							i++;
//						}
//						cout<<"��ѡ������ǣ�";
//						pkCardAI->sortCard(pkCardTakeout);
//						pkCardAI->dispalyCardInfo(pkCardTakeout);						
//						if (pkCardAI->judgeCardOut(pkCardTakeout,NULL,MT_FL_CARD_TYPE_BEG))  //ѡ������ܹ���Ϊ����
//						{
//							mteCardType eCardTypeTakeOut = pkCardAI->getTypeFromCard(pkCardTakeout); //�õ����Ƶ�����
//							cout<<"������Ϊ��";
//							cout<<eCardTypeTakeOut<<endl;
//							mteCardType eCardType = pkCardAI->getCardOut(kCardInfoAI,pkCardTakeout,eCardTypeTakeOut,pkCardDst);//�õ������˵���
//
//							if (eCardType == MT_FL_CARD_TYPE_PASS)
//							{
//								cout<<"�����˴���,���������"<<endl;
//								cout<<"������ʣ�������:";
//								pkCardAI->dispalyCardInfo(kCardInfoAI); //�ٴ�ӡ����
//								goto sign1;
//							}
//							else
//							{
//								cout<<"�����˳�������:";
//								pkCardAI->dispalyCardInfo(pkCardDst);
//								cout<<"������Ϊ��";
//								cout<<eCardType<<endl;
//								pkCardAI->takeAwayCard(kCardInfoAI,pkCardDst);//ɾ����������
//								if(!pkCardAI->getCardNum(kCardInfoAI))
//								{
//									continue;
//								}
//								pkCardAI->sortCard(kCardInfoAI); //�������к�
//								cout<<"������ʣ�������:";
//								pkCardAI->dispalyCardInfo(kCardInfoAI); //�ٴ�ӡ����
//
//							}
//						}
//						else
//						{
//							cout<<"��ѡ����Ʋ�����Ҫ��,������ѡ���±�"<<endl;
//							goto sign1;
//						}						
//					}
//					break;
//				case 0:                   //�����˳��� ����ѹ�� 
//					{
//sign2:					mteCardType eCardTypeTakeOut = MT_FL_CARD_TYPE_BEG;
//						for (int i = 0; i<21; i++)
//						{
//							pkCardDst[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
//						}
//						mteCardType eCardType = pkCardAI->getCardOut(kCardInfoAI,NULL,eCardTypeTakeOut,pkCardDst);//�õ������˵���
//						cout<<"�����˳������ǣ�";
//						pkCardAI->dispalyCardInfo(pkCardDst);
//						cout<<"�������ǣ�";
//						cout<<eCardType<<endl;
//						pkCardAI->takeAwayCard(kCardInfoAI,pkCardDst);
//						if(!pkCardAI->getCardNum(kCardInfoAI))
//						{
//							continue;
//						}
//						pkCardAI->sortCard(kCardInfoAI); //�������к�
//						cout<<"������ʣ������ǣ�";
//						pkCardAI->dispalyCardInfo(kCardInfoAI); //�ٴ�ӡ����
//
//						cout<<"ѡ���Ƿ�ѹ�ƣ�1:ѹ�� ��0:����"<<endl;
//						int iIsOutCard = -1;
//						cin>>iIsOutCard;
//						if (iIsOutCard) //ѹ��
//						{
//sign3:					    cout<<"������������Ҫ���Ƶ��±�,�����100������"<<endl;
//							for (int i = 0; i<21; i++)
//							{
//								pkCardTakeout[i].cCardValue = MTFL_NUM_CARD_TERMINAL;
//							}
//							int a= 0;
//							int i = 0;
//							while(cin>>a)
//							{
//								if (a == 100)
//								{
//									break;
//
//								}
//								memcpy(&pkCardTakeout[i],pkCardMan->m_pkASetOfCards+a,sizeof(mtCardInfo));
//								i++;
//							}
//							cout<<"��ѡ������ǣ�";
//							pkCardAI->sortCard(pkCardTakeout);
//							pkCardAI->dispalyCardInfo(pkCardTakeout);
//							if(pkCardAI->judgeCardOut(pkCardTakeout,pkCardDst,eCardType))  //����ѹ���ոմ����
//							{
//								cout<<"ѡ�Ʒ���Ҫ��,�ҳ������ǣ�"<<endl;
//								pkCardAI->dispalyCardInfo(pkCardTakeout);
//								cout<<"�������ǣ�";
//								cout<<pkCardAI->getTypeFromCard(pkCardTakeout)<<endl;
//
//							}
//							else
//							{
//								cout<<"��ѡ����Ʋ�����Ҫ��,������ѡ���±�"<<endl;
//								goto sign3;
//							}
//						}
//						else //����
//						{
//							cout<<"��ѡ�����,�����˼�������"<<endl;
//							goto sign2;
//						}						
//					}
//					break;
//				default:
//					break;
//				}				
//			}
//			cout<<"�����˳�������,�����¿�һ��"<<endl;
//		}
//	}	
//	delete pkCardAI;
//	if (pkCardAI != NULL)
//	{
//		pkCardAI = NULL;
//	}
//	delete pkCardMan;
//	if(pkCardMan != NULL)
//	{
//		pkCardMan = NULL;
//	}
//#endif
//	return 0;
//}
