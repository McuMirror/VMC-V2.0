

#ifndef   ___VMC_GPRS_PC_H__
#define   ___VMC_GPRS_PC_H__

#include "ZHIHUIPC.h"


//��������
#define  GPRS_CON_FLASH	  1		//����Flash ��Ϊ�����ϱ����� ������Ϊ0 ��ʹ���ڴ�
#define  GPRS_FALSH_OFF	  3900 //	
//������
#define  G_REGISTER  	  51    //ǩ������
#define  G_POLL		      52	//
#define  G_FUALT		  61	//
#define  G_RUN 			  62	//
#define  G_CONFIG		  63	//ϵͳ������Ϣ
#define  G_AISLE		  64	//����������Ϣ	
#define  G_GOODS		  71	//��Ʒ������Ϣ
#define  G_TOTAL		  72	//��Ʒ���ۻ�����Ϣ

//��������
#define  L_REGISTER  	  35    //ǩ�����󳤶���ͬ
#define  L_POLL		      0		//
#define  L_FAULT		  49	//
#define  L_RUN 			  68	//
#define  L_CONFIG		  42	//ϵͳ������Ϣ
#define  L_AISLE		  168	//����������Ϣ	
#define  L_GOODS		  87	//��Ʒ������Ϣ
#define  L_TOTAL		  160	//��Ʒ���ۻ�����Ϣ old 146
//��Ӧ��	  	
#define	 GPRS_A_ERR		  0	    
#define  GPRS_A_SUCCEE	  1		//�ɹ� ��00��
#define  GPRS_A_TIMEOUT	  2		//��ʱ
#define  GPRS_A_TERMINAL  3		//ǩ�����ն˱�Ų�����
#define  GPRS_A_FAIL 	  4     //��Ӧ�벻�ǡ�00��
#define  GPRS_A_GEN_ERR	  5		//ͨ��ʧ�� ���ء�99��
#define  GPRS_A_RESET	  6



#define  REQ	         'R'
#define  ASK		     'A'

#define  TIMES	 		   3 
#define  GPRS_HEAD	   	  37	
#define  GPRS_H_INIT   	  1
#define  GPRS_H_NOINIT    0

#define  GPRS_F_WRTIE	  0 //falshд
#define  GPRS_F_READ	  1//flash��
#define  GPRS_F_FAULT	  3940
#define  GPRS_F_RUN	  	  3941
#define  GPRS_F_AISLE	  3942
#define  GPRS_F_CONFIG	  3943
#define  GPRS_F_TRADE	  3944
#define  GPRS_F_POLL	  3945


#define  GPRS_T_FAULT	  '1'
#define  GPRS_T_RUN	  	  '2'
#define  GPRS_T_AISLE	  '3'
#define  GPRS_T_CONFIG	  '4'
#define  GPRS_T_TRADE	  '5'
#define  GPRS_T_POLL	  '6'

typedef struct _trade_cache_node
{
	//unsigned char	node[360];
	unsigned char	len;
	unsigned char	type;
	unsigned char buf_empty;
	unsigned char recv_data;
	unsigned char	poll_req;
	unsigned char fault_req;
	unsigned char aisle_req;
	unsigned char run_req;
	unsigned char config_req;
	unsigned char com_err_num;
	
}TRADE_CACHE_NODE;
extern volatile unsigned char vmc_gprs_pc_state;
extern TRADE_CACHE_NODE trade_cache_head;

unsigned char vmc_gprs_recv(unsigned char *recv_msg,unsigned short *len,unsigned char times);
unsigned char vmc_gprs_register(unsigned char mode);
unsigned char vmc_gprs_poll(unsigned char mode);
unsigned char vmc_gprs_fualt(unsigned char mode);
unsigned char vmc_gprs_run(unsigned char mode);
unsigned char vmc_gprs_config(unsigned char mode);
unsigned char vmc_gprs_aisle(unsigned char mode);
unsigned char vmc_gprs_goods(unsigned char num,unsigned char mode);
unsigned char vmc_gprs_total(void);
unsigned char vmc_gprs_setup(void);
char ship_cache_empty(void);
unsigned char ship_cache_send(unsigned char len);
unsigned char ship_cache_poll(void);
unsigned char trade_cache_init(void);
void vmc_poll_req(void);
unsigned char vmc_gprs_trade_rpt(unsigned short index);


void task3_gprs_poll(void);

#endif
