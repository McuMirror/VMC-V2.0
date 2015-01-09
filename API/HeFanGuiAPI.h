#ifndef _HEFANGUIAPI_H
#define _HEFANGUIAPI_H

//�з����Ų���
#define HEFANGUI_KAIMEN			(1)
//�з����ѯ����
#define HEFANGUI_CHAXUN			(2)
//�з�����ȿ�
#define HEFANGUI_JIAREKAI		(3)
//�з�����ȹ�				
#define HEFANGUI_JIAREGUAN		(4)
//�з������俪
#define HEFANGUI_ZHILENGKAI		(5)
//�з��������
#define HEFANGUI_ZHILENGGUAN	(6)
//�з�������������
#define HEFANGUI_ZHAOMINGKAI	(7)
//�з�������������
#define HEFANGUI_ZHAOMINGGUAN	(8)

//======================����з�������ṹ��=================================================
#define HE_FAN_MAX_CABINET    10
#define HE_FAN_MAX_HUODAO	   88


#define HEFAN_LIGHT    0
#define HEFAN_LIGHT_OPEN    1

#define HEFAN_HOT      2
#define HEFAN_HOT_OPEN      3
#define HEFAN_COOL     4
#define HEFAN_COOL_OPEN     5
#define HEFAN_EXIST 6






#define HEFAN_HUODAO_NORMAL   1
#define HEFAN_HUODAO_FAULT   2
#define HEFAN_HUODAO_EMPTY   3
#define HEFAN_HUODAO_CLOSE   4

/*
type��1������������
	  2������������
	  3��������״̬
	  4���������������
	  5����ѡ������ֵ
	  6�����ɹ����״���
	  7������Ʒ���

*/
#define HEFAN_HUODAO_PRICE  1
#define HEFAN_HUODAO_STATE  3
#define HEFAN_HUODAO_TRADE_NUM  6
#define HEFAN_HUODAO_ID     7

#define HEFAN_CABINET_NUMS   11


typedef struct _huodao_{

	unsigned short price;
	unsigned short tradeNum;//��������
	unsigned char  state;	//  1����  2����3 ȱ�� 4 �ر�
	unsigned char  id;
	

}HUODAO;


typedef struct _hefan_cabinet_{
	
	HUODAO huodao[HE_FAN_MAX_HUODAO];
	unsigned char state;// xx cc hh ll       xx ��ʾ���Ӵ������1���� 0������ cc��������״̬ hh���ڼ���״̬ ll��������װ��
	unsigned char huodaoNums;//��������

}HEFAN_CABINET;

extern HEFAN_CABINET stHefanCabinet[HE_FAN_MAX_CABINET];






extern unsigned char ChaXunRst[2];

extern unsigned char HeFanGuiSendMBox(unsigned char HandleType,unsigned char ChannelNum,unsigned char Binnum);

extern unsigned char HeFanGuiHandle(unsigned char LogicNum,unsigned char HandleType,unsigned char Binnum);
unsigned int hefanGetParaVlaue(unsigned char cabinetNo,unsigned char boxNo,unsigned char type);
unsigned char hefanSetParaVlaue(unsigned char cabinetNo,unsigned char boxNo,unsigned int value,unsigned char type);
unsigned char read_hefan_info_by_flash(void);
unsigned char hefanHuodaoCheck(unsigned char cabinetNo,unsigned char boxNo);
unsigned char setHefanCabinetState(unsigned char cabinetNo,unsigned char type,unsigned char state);
unsigned char getHefanCabinetState(unsigned char cabinetNo,unsigned char type);
unsigned char hefanOutGoodsHandle(unsigned char LogicNum,unsigned char HandleType,unsigned char Binnum);
void save_hefan_huodao_by_flash(void);
unsigned char save_hefan_huodao_info(unsigned char cabinetNo);

unsigned char hefanSetParaVlaue(unsigned char cabinetNo,unsigned char boxNo,unsigned int value,unsigned char type);
unsigned int hefanGetParaVlaue(unsigned char cabinetNo,unsigned char boxNo,unsigned char type);
unsigned char get_hefan_Index(unsigned char cabinetNo);
unsigned char getHefanHuodaotState(unsigned char cabinetNo,unsigned char boxNO);
void hefanMannage(void );

unsigned char hefanHuodaoInit(unsigned char cabinetNo);
unsigned char hefan_clear_trade(unsigned char cabinetNo);
unsigned char hefanAddgoodsHandle(unsigned char cabinetNo,unsigned  char type);


#endif
