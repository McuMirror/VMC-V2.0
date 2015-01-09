#ifndef _BILLACCEPTER_H
//#define BILLDEV_MDB		2				//��ǰϵͳʶ�𵽵�ֽ��������ΪMDB

typedef struct	
{
	uint8_t Communicate;//1����ֽ����ͨѶ����
	uint8_t moto;//1����������
	uint8_t sensor;//1������������
	uint8_t romchk;//1����rom����
	uint8_t jam;//1����Ͷ�ҿ���
	uint8_t removeCash;//1�Ƴ�ֽ�ҳ���
	uint8_t cashErr;//1ֽ�ҳ������
	uint8_t disable;//1ֽ����Ϊ����ԭ�򱻽�����
}MDBBILLERROR;
extern MDBBILLERROR MdbBillErr;


extern void BillDevSellect(void);

extern uint8_t BillDevInit(void);

extern void BillDevDisable(void);

extern void BillDevEnable(void);

extern unsigned char BillDevProcess(uint32_t *RecvMoney,unsigned char *BillType,unsigned char billOpt,unsigned char *billOptBack,uint32_t RecyPayoutMoney,uint8_t RecyPayoutNum);

extern uint8_t BillDevEscrow(void);

extern uint8_t BillDevReturn(void);

extern uint8_t RecyclerDevInit(void);


#endif
