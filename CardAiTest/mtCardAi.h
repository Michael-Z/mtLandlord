#pragma once
#include <iostream>
using namespace std;

typedef struct 
{
	char cCardType;
	char cCardValue;
	
}mtCardInfo;

#if 0
typedef  char	Card[2];
#endif
#define			MTFL_NUM_CARD_TERMINAL						100
#define			MTFL_NUM_CARD_TWO							13
#define			MTFL_NUM_CARD								20


 enum mteCardType
 {
 	MT_FL_CARD_TYPE_BEG		= 0,
 	MT_FL_CARD_TYPE_PASS	= MT_FL_CARD_TYPE_BEG,	///����
 	MT_FL_CARD_TYPE_SINGLE,							///��֧
 	MT_FL_CARD_TYPE_PAIR,							///����
 	MT_FL_CARD_TYPE_ROCKET,							///���
 	MT_FL_CARD_TYPE_BOMB,							///ը��
 	MT_FL_CARD_TYPE_TRIPLE,							///����
 	MT_FL_CARD_TYPE_TRIPLE_ONE,						///����һ
 	MT_FL_CARD_TYPE_TRIPLE_TWO,						///������
 	MT_FL_CARD_TYPE_SINGLE_PROGRESSION,				///��˳
 	MT_FL_CARD_TYPE_PAIR_PROGRESSION,				///˫˳
 	MT_FL_CARD_TYPE_TRIPLE_PROGRESSION,				///��˳
 	MT_FL_CARD_TYPE_AIRPLANE,						///�ɻ������
 	MT_FL_CARD_TYPE_FOUR_TWO,						///�Ĵ���
 
 	MT_FL_CARD_TYPE_END
 };

class mtCardAI
{
public:
	mtCardAI();
	~mtCardAI();
	/// ��Ҫ�ӿں���
	//	����˵����
	//***************************************************
    //����ֵ:ѡ���Ƶ�����
	//pcHoldCard����ǰ������е���
	//pcCardOut: �����ϵ�ǰ�ĳ��ƣ��������Ҫѹ���ƣ�ΪNULLʱ��������ϴγ���û����ѹ��ס
	//pcSelectedCard�����������ѡ������
	//eCardType����ǰ�����ϳ����Ƶ�����
	//****************************************************
	mteCardType getCardOut(mtCardInfo *pkHoldCard,mtCardInfo *pkCardOut,mteCardType eCardType,mtCardInfo *pkSelectedCard);

	//	����˵����
	//***************************************************
    //����ֵ:�ܷ�ѹ�������ϵ���
	//pcSelectedCard��ѡ�е���
	//pcCardOut: �����ϵ�ǰ�ĳ��ƣ��������Ҫѹ���ƣ�ΪNULLʱ��������ϴγ���û����ѹ��ס
	//eCardType����ǰ�����ϳ����Ƶ�����
	//****************************************************
	bool judgeCardOut(mtCardInfo *pkSelectedCard,mtCardInfo *pkCardOut,mteCardType  eCardType);








	/// ���ƴ�С��������
	int sortCard(mtCardInfo* pkCardSrc);
	/// ��src��ɾ��takeaway����
	int takeAwayCard(mtCardInfo* pkCardSrc, mtCardInfo* pkCardTakeAway);
	/// ��������Ƶķ���, >=7:���� >=5:���� >=3:һ��
	int getCardScore(mtCardInfo* pkCardSrc);
	/// ����Ƶ�����
	int getCardNum(mtCardInfo* pkCardSrc);
	/// ��addin���Ƽ��뵽dst��
	int addInCard(mtCardInfo* pkCardDst, mtCardInfo* pkCardAddIn);
	/// ������(������һ,������,�ɻ������,�Ĵ������е���Ч�Ʒ���ǰ�沢����)
	int tidyCard(mtCardInfo* pkCardSrc);
	/// ����src�е����Ƿ���cardtype����
	int	testCardType(mtCardInfo* pkCardSrc, mteCardType eCardType);
	/// �����������Ƿ���testcards(��������)
	int testIncludeCardsInHand(mtCardInfo* pkHandCards, mtCardInfo* pkTestCards);
	/// ϴ��(��������)
	mtCardInfo* shuffle(void);  //ϴ�õ��ƴ��ҷ�����m_pkASetOfCards[54]����
	/// ���Դ�������Ƿ����������,�������ܷ���ָ����cardType����С��minCardTakeout;
	mteCardType testTakeoutCard(mtCardInfo* pkCardSrc, mtCardInfo kMinCardTakeout, mteCardType eCardType = MT_FL_CARD_TYPE_PASS);
	/// ��������:������ѡ����С����(dst:ѡ������,src:���е���)
	mteCardType getCardToTakeout(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc);
	/// ����:������ѡ����С���ܴ�ס�ϼҵ���(dst:ѡ������,src:���е���,cardType:�ϴδ��������,cardTakeout:�ϴδ������,unglue:�Ƿ����:1��,0����)
	mteCardType getCardFromType(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc, mteCardType eCardType, mtCardInfo* pkCardTakeout, int iUnglue);
	/// ����������Ϣ ���ڹ۲�
	void dispalyCardInfo(mtCardInfo* pkCardSrc,FILE* pLogFile,int logFlag = 0);   //logFlag = 0,��д����־��ֻ��ӡ������̨��logFlag = 1��д��־���ļ�

	static void cardConvert(char *cCardSrc,mtCardInfo *pkCardDst); //���������� ת��Ϊ �ͻ�����

	static void cardConvert(mtCardInfo *pkCardSrc,char *cCardDst); //�ͻ��˵��� ת��Ϊ����������



//private:
public:
	mteCardType	getTypeFromCard(mtCardInfo* pkCardSrc);
	mteCardType getCardFromSpecifyType(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc, mteCardType eCardType, mtCardInfo* pkCardTakeout);
	mteCardType getCardDirectFromType(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc, mteCardType eCardType, mtCardInfo* pkCardTakeout);
	mteCardType getCardSmallest(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc);
	int findASpecifyCardOfValueType(mtCardInfo* pkCardDst, mtCardInfo* pkCardFind);
	int calcBombTripleSinglePair(mtCardInfo* pkCardSrc);
	int	getTripleNum(mtCardInfo* pkCardSrc);
	int copyCard(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc);
	int findAGivenSingle(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc, char cCardFindValue);
	int findAGivenPair(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc, char cCardFindValue);
	int findAGivenTriple(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc, char cCardFindValue);
	int findAllSingleProgression(mtCardInfo* pkCardSrc);
	int findAllSimpleProgression(mtCardInfo* pkCardSrc);
	int findPairProgressionInSingleProgression(int iSingleProgressionNum);
	int delASingleProgression(int iSingleProgressionIdx, int iSingleProgressionNum);
	int mergeSingleProgression(int iSingleProgressionNum);

	int findRocket(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc);
	int findABombBiger(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc, mtCardInfo cCardTakeout);
	int findASingleBiger(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc, mtCardInfo cCardTakeout);
	int findAPairBiger(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc, mtCardInfo cCardTakeout);
	int	findATripleBiger(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc, mtCardInfo cCardTakeout);
	int findATripleProgression(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc);
	int	findATripleProgressionBiger(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc, mtCardInfo cMaxCardTakeOut, int iTripleNumTakeOut);
	int findASingleProgression(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc);
	int findASingleProgressionBiger(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc, mtCardInfo cMinCardTakeOut, int iCardNumTakeout);
	int findAPairProgression(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc);
	int	findAPairProgressionBiger(mtCardInfo* pkCardDst, mtCardInfo* pkCardSrc, mtCardInfo cMinCardTakeOut, int iPairNumTakeOut);
	void findTheMostSomeCard(mtCardInfo* pkCardSrc,char *pcCardFindValue);//�ҳ�pkCardSrc����������ͬ���Ƶ�ֵ �����cCardFindValue����  ���� 33344 ��ô����3
	void findTheMaxCard(mtCardInfo* pkCardSrc,char *pcCardFindValue);
	void outPutSingleCard(mtCardInfo *pkcard,FILE* pLogFile,int logFlag);

//private:
public:
	mtCardInfo		m_pkCardGet[MTFL_NUM_CARD + 1];
	mtCardInfo		m_pkCardTmpPrivate[MTFL_NUM_CARD + 1];	/// findAllSingleProgression, getTripleNum
	mtCardInfo		m_pkCardTmp[MTFL_NUM_CARD + 1];
	mtCardInfo		m_pkCardTmpPublic[MTFL_NUM_CARD + 1];	/// getCardToTakeout, getCardFromType
	mtCardInfo		m_pkSingleProgression[4][13];
	mtCardInfo		m_pkASetOfCards[54];
	int				m_iBombNum;
	int				m_iTripleNum;
	int				m_iPairNum;
	int				m_iSingleNum;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	a) ��ȷ��������ж��Ƿ��д�С����
// 	b) ��ȷ��ը���������Ƿ�����ͷ��
// 	c) ��ȷ���������ڳ���ը������������ж��Ƿ����������
// 	d) ��ȷ����˳�����Ѿ�ȷ�����������ж��Ƿ�������ڵ�����������У����������˳��ע�⣬Ӧ��ʹ��˳�����������ܴ󡣼������
//	   444555666������ϳ�һ����˳�������Ƿֳ�444555һ����˳��666һ��������
// 	e) ��ȷ����˳���жϵ�˳ʱ����ȥ���ĸ�2���������ը���������ж��Ƿ���ڳ��������ƣ������������ָ���е���������������ƣ����
//	   �У�������ȡ��������Σ���ʣ�������ÿһ����������������˳����������ϣ�����ܹ�������ɵ�˳�Ͷ��ӣ���ԭ�е�����ȡ����
//	   ������ϳ����ƺ͵�˳������4566678�������45678��66������󣬽���֪�����ơ���������������˳����ʣ�µ�����������ϣ�������
//	   ��45678����֪�����ƣ�999��������10J�����ƣ���������ϳ�45678910J��99��ͨ�����ϵķ��������ܽ����ƺ������ܺõ�������ϡ�
// 	f) ��ȷ��˫˳�����ȣ��������˳����ȫ�غϣ�����������ϳ�˫˳����Σ��ڳ�ը������˳����������˳��������м���Ƿ����˫˳��
// 	   ����У�������ȡ������
// 	g) ��ȷ�����ӣ���ը������˳�����������ơ�˫˳��������м���Ƿ���ڶ��ӣ�������ڽ�����ȡ������
// 	h) ��ȷ�����ƣ�����ը������˳�����������ơ�˫˳������������������Ŷ��ǵ��ơ�
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////