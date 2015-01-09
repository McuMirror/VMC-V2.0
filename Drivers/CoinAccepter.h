#ifndef _COINACCEPTER_H
//#define COINDEV_MDB		2				//��ǰϵͳʶ�𵽵�Ӳ��������ΪMDB

typedef struct	
{
	uint8_t Communicate;//1����Ӳ����ͨѶ����
	uint8_t sensor;//1������������
	uint8_t tubejam;//1������ҿڿ���
	uint8_t romchk;//1����rom����
	uint8_t routing;//1�������ͨ������
	uint8_t jam;//1����Ͷ�ҿ���	
	uint8_t removeTube;//1Ӳ�Ҷ��ƿ�
	//ʹ����չָ����
	uint8_t disable;//1Ӳ����Ϊ����ԭ��vmc������
	uint8_t unknowError;//��0Ӳ���������ֹ���
	uint8_t unknowErrorLow;//Ӳ���������ֹ��ϵ���״̬��
	
}MDBCOINERROR;
extern MDBCOINERROR MdbCoinErr;



extern uint8_t CoinDevInit(void);


extern void CoinDevDisable(void);

extern void CoinDevEnable(void);

extern unsigned char CoinDevProcess(uint32_t *RecvMoney,unsigned char *CoinType,unsigned char *coinOptBack);

extern void CoinDevProcessExpanse(void);
#endif
