#ifndef _LOGAPI_H

//add by yoc newAisle
#define PAYMODE_CASH	0   //�ֽ����
#define PAYMODE_ONECARD	1	//һ��ͨ
#define PAYMODE_UNIONPAY 2  //������
#define PAYMODE_PC		3  //PCͨ�ó���
#define PAYMODE_ALIPAY  4  //֧����
#define PAYMODE_PHONE	5  //�ֻ�����
#define PAYMODE_GAME	6  //��Ϸ����
#define PAYMODE_CARD	7 //ͨ�ÿ�

#define PAYMODE_OTHER	0xAA  //������ʽ



extern  void LogBeginTransAPI(void);

extern	void LogGetMoneyAPI(uint32_t InValue,uint8_t MoneyType);

extern  void LogTransactionAPI(uint32_t InValue,uint8_t transMul,char *ChannelNum,uint8_t payMode);

extern  void LogCostAPI(uint32_t InValue);

extern  void LogCardIncomeAPI(uint32_t InValue);

extern  void LogChangeAPI(uint32_t InValue,uint32_t debtMoney);

extern  void LogEndTransAPI(void);

extern  void LogClearAPI(void);

extern  void WriteLogAPI(void);

extern  uint8_t ReadLogAPI(void);

extern  void WriteLogDetailAPI(uint16_t	LogDetailPage);

extern  uint8_t ReadLogDetailAPI(uint16_t	LogDetailPage);

extern unsigned char PrintLogDetailAPI(uint16_t no) ;

extern uint8_t ReadDetailTraceAPI(char * detailBuff,uint16_t	LogDetailPage);



#endif
