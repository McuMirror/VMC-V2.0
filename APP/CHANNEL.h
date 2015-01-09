#ifndef _CHANNEL_H
#define _CHANNEL_H


#define CHECKCOUNT			(1)
#define CHECKPRICE			(2)


//��UI������������ṹ��  create by yoc
typedef struct _ui_aisle_admin_{

	char *cabinetAdminMenu[MAXLANGUAGETYPE];//"������������"
	char *enternCabinetNo[MAXLANGUAGETYPE];//" ���������: "
	char *A_Bin[MAXLANGUAGETYPE];//" A ��"
	char *B_Bin[MAXLANGUAGETYPE];//" B ��"
	char *hefan[MAXLANGUAGETYPE];//"�з���"
	char *columnInit[MAXLANGUAGETYPE];//" ������ʼ��"
	char *columnInitOk[MAXLANGUAGETYPE];//" ��ʼ���ɹ�"
	char *levelManage[MAXLANGUAGETYPE];//" ��ܹ���"
	char *enterLevelNo[MAXLANGUAGETYPE];//" ���������:"
	char *columnmanage[MAXLANGUAGETYPE];//" ��������"
	char *enterColumNo[MAXLANGUAGETYPE];//" �����������: "
	char *noColumn[MAXLANGUAGETYPE];//" ����������! "
	char *columnStatus[MAXLANGUAGETYPE];//" ����״̬ :"
	char *maxCount[MAXLANGUAGETYPE];//" �������:"
	char *levelState[MAXLANGUAGETYPE];//"���״̬: "
	char *open[MAXLANGUAGETYPE];//" ����"
	char *close[MAXLANGUAGETYPE];//" �ر�"
	char *yes[MAXLANGUAGETYPE];//" ȷ��"
	char *cancel[MAXLANGUAGETYPE];//" ȡ��"
	char *no_1[MAXLANGUAGETYPE];//"��"
	char *no_2[MAXLANGUAGETYPE];
	char *no_3[MAXLANGUAGETYPE];
	char *no_4[MAXLANGUAGETYPE];
	char *no_5[MAXLANGUAGETYPE];


	//===�����û�����
	char *columnUserManage[MAXLANGUAGETYPE];//"������������"
	char *columnAtrib[MAXLANGUAGETYPE];//"��������"
	char *columnAdd[MAXLANGUAGETYPE];//"�������"
	char *columnLook[MAXLANGUAGETYPE];//"�����鿴"
	char *columnTest[MAXLANGUAGETYPE];//"��������"
	char *columnKaoji[MAXLANGUAGETYPE];//"������ѯ"
	char *columnPrice[MAXLANGUAGETYPE];//" ������Ʒ���� :"
	char *columnCount[MAXLANGUAGETYPE];//" ������������ : "
	char *columnSuc[MAXLANGUAGETYPE];//" �ɹ����״���: " 
	char *columnSelectNo[MAXLANGUAGETYPE];//"ѡ��������"
	char *columnID[MAXLANGUAGETYPE];//"��Ʒ���"
	char *columnOneAdd[MAXLANGUAGETYPE];//"���������",
	char *columnLevelAdd[MAXLANGUAGETYPE];//"��������",
	char *columnAllAdd[MAXLANGUAGETYPE];//"ȫ�������",
	char *columnSelect[MAXLANGUAGETYPE];//"��ѡ�����:",
	char *sureAddLevel[MAXLANGUAGETYPE];//"ȷ��Ҫ��Ӹò�?",
	char *columnCheck[MAXLANGUAGETYPE];
	char *columnBad[MAXLANGUAGETYPE];
	char *columnEmpty[MAXLANGUAGETYPE];
	char *columnPriceZero[MAXLANGUAGETYPE];
	char *columnlOK[MAXLANGUAGETYPE];


	char *column99Test[MAXLANGUAGETYPE];//"ȫ��������ҳ��"
	char *column98Test[MAXLANGUAGETYPE];//"����������ҳ��"
	char *column97Test[MAXLANGUAGETYPE];//"�����������ҳ��"
	char *columnOneTest[MAXLANGUAGETYPE];//"����������ҳ��"
	
	char *columnCheckWait[MAXLANGUAGETYPE];
	char *columnTestEmpty[MAXLANGUAGETYPE];
	char *columnTestOK[MAXLANGUAGETYPE];
	char *columnTestBad[MAXLANGUAGETYPE];
	char *columnTestMOTO[MAXLANGUAGETYPE];
	char *columnTestNA[MAXLANGUAGETYPE];
	char *columnCommunFail[MAXLANGUAGETYPE];
	
	
}UI_AISLE_ADMIN;

extern const UI_AISLE_ADMIN ui_aisle_admin;


void ChannelTest(unsigned char cabinetNo);

void ChannelManage(void);

void ChannelParamSet(unsigned char cabinetNo);

void ChannelAddGoods(unsigned char cabinetNo);

void ChannelLeaveFactory(void);

void ChannelAttributeManage(void);

void ChannelClearSuccesCount(void);

void ChannelLookUpSuccessCount(void);

unsigned int ChannelGetMaxGoodsPrice(unsigned char Binnum);

unsigned char ChannelReadChannelParam(unsigned char Bin,unsigned char logicnum);

unsigned char ChannelCheckCountOrPrice(unsigned char Logicnum,unsigned char Binnum,unsigned char Type,unsigned int price);

unsigned char ChannelAPIProcess(unsigned char LogicNum,unsigned char HandleType,unsigned char Binnum);
unsigned char TestColumnByPC(unsigned char Bin,unsigned LogicNum);
unsigned char ColumnTestByPcPost(unsigned char Bin,unsigned LogicNum);
unsigned char ColumnTestByPcPend(void);
void ChannelSomeTest(unsigned char cabinetNo,unsigned char *columnBuf,unsigned char columnNum,unsigned char titleType);
void ChannelKaojiClear(unsigned char cabinet);
#endif
