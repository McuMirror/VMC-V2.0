
#ifndef __ZHIHUIAPI_H__
#define __ZHIHUIAPI_H__


#define TRADE_PAGE_MAX			3990				//���׼�¼��flash�����ҳ��


#define print_pc(...)		TracePC(__VA_ARGS__)

//���Ժ�
#define  PRINT_ENABLE         0
#if PRINT_ENABLE == 1

#define  print_log(...)       Trace(__VA_ARGS__)
#define  print_err(errno)        Trace("ERROR: In %s at Line %u - errno = %d\n", __FUNCTION__, __LINE__, errno)
#define  print_warning(no)		 Trace("Warning: In %s at Line %u - no = %d\n", __FUNCTION__, __LINE__, no)

#else 
#define  print_log(...)       do {} while(0)
#define  print_err(errno)      do {} while(0)
#define  print_warning(no)	  do {} while(0)

#endif



//=============================================================================
//��ȡ��������
//=============================================================================
#define AISLE_PRICE		1
#define AISLE_COUNT		2
#define AISLE_STATE	  	3
#define AISLE_MAXCOUNT	4
#define AISLE_SELCET	5
#define AISLE_SELL_NUM	6
#define AISLE_ID	    7


//=============================================================================
//ZHIHUIPC
//=============================================================================

//#define ZH_DEBUG //��������ʽ��Ҫȡ��
#define RECV_SIZE			     256	    //���ذ�POLLָ��� PC��Ӧ��size
#define SEND_SIZE		         256     //  ���ʹ�������size 
#define VMC_ONLINE				 0x01
#define VMC_OFFLINE				 0x02


//=============================================================================
//����������Ϣ
//=============================================================================
#define MAC_T_F				 	 0x11	//ʳƷ��
#define MAC_T_G				 	 0x21//����
#define MAC_T_D				 	 0x01//���ϻ�
#define MAC_T_YT				 0x20 //һ�϶�

#define AISLE_SIZE			     48//Ĭ��80����
#define FOOD_ROW    		 	 (uint8_t)6     	//��������
#define FOOD_COLUMN 		 	 (uint8_t)8     	//��������

#define ID_BYTES			         1  //��ƷID��ID_BYTES �ֽ�


#define MBOX_ST_LEN				 0x08 //���ǻ��йص�����ͨ�Žṹ�峤��
#if 0
#define MAC_SN					 (uint32_t)11040070  //�����ͺ�
#define VMC_SN					 (uint16_t)0x1212	 //���ذ�汾��
#define MAC_T					 (uint8_t)MAC_T_D    //�������� 0X01���ϻ���0X11ʳƷ����0X21
#define AISLE_SUM				 48//�������� 8*3 
#define DRINK_SIZE				 48//���ϻ���������
#endif


//=============================================================================
//�ۻ���ͶǮ��ʽ
//=============================================================================

#define ZH_BILL					1
#define ZH_COIN				    2
#define ZH_CARD					3
#define ZH_ESCROWIN				4
#define ZH_ESCROWOUT			5


//=============================================================================
// ACK�źż�������
//=============================================================================

#define ZH_R_NAK						 0x00 
#define ZH_R_ACK						 0x01
#define ZH_R_MESG					 	 0x02
#define ZH_R_ERR_CRC					 0x03	//crcУ����� ���հ�����ȷ
#define ZH_R_TIMEOUT					 0x04	//��ʱ
#define ZH_R_ERR_U				 		 0x05
#define ZH_R_RTIMEOUT					 0x06   //�����ֽڽ��ճ�ʱ



#define ACK_TIMEOUT_SIZE		 15    //����ACK��ʱʱ��

//=============================================================================
// ������
//=============================================================================

#define ADDR_T					 0xEF	
#define CMD_T					 0xEE
#define DATA_T					 0xFE
#define ACK_T				     0x00	
#define NAK_T			         0x15

#define ACK_PC					 0x01
#define NAK_PC				     0x15

//=============================================================================
//type Э������� ������
//=============================================================================

#define PC_SHIPMENT_REQUEST 	 0x01   //��������
#define PC_SYS_TIME_SET			 0x02   //ϵͳʱ������
#define PC_DEDUCT_SHIP			 0x03   //�ۿ��������(������ʳƷ��֧�����Ϲ�)
#define PC_GOODS_SET		     0x04	//����VMCϵͳ��Ʒ��� ����
#define PC_ADD_GOODS 			 0x05   //����VMCϵͳ�ӻ��ϵ�
#define PC_PRICE_SET			 0x06	//����VMCϵͳ��Ʒ�۸�
#define PC_CLEAR_SET			 0x70  //��������
#define PC_AISLE_SET			 0x71  //���û���
#define ACK_OFF				     0     	//����Э���������ƫ��


#define VMC_T_POLL        			 0x76	//��ѯ����
#define VMC_T_REGISTER 	   			 0x78	//ǩ��
#define VMC_T_TRADE   			 	 0x7C	//����
#define VMC_T_AISLE_RUN 			 0x79	//�ϵ�������Ϣ
#define VMC_T_MAC_RUN   			 0x7D	//����������Ϣ
#define VMC_T_CONFIG		 		 0x7A	//ϵͳ������Ϣ
#define VMC_T_GOODS_SN	 			 0x7E	//�ϵ���Ʒ��Ϣ
#define VMC_T_GOODS_PRICE 		     0x7F   //�ϵ��۸���Ϣ
#define VMC_T_SUM 				     0x7B   //��Ʒ���ۻ�����Ϣ

#define VMC_L_POLL        			 (10 + ID_BYTES)	//��ѯ����
#define VMC_L_REGISTER 	   			 20	//ǩ��
#define VMC_L_TRADE   			 	 (28 + ID_BYTES)	//����
#define VMC_L_AISLE_RUN 			 (AISLE_SIZE + 7)  //55 - 48    	//�ϵ�������Ϣ
#define VMC_L_MAC_RUN   			 10	 	//����������Ϣ
#define VMC_L_CONFIG		 		 17	//ϵͳ������Ϣ
#define VMC_L_GOODS_SN	 			 (AISLE_SIZE * ID_BYTES + 1) //49	//�ϵ���Ʒ��Ϣ
#define VMC_L_GOODS_PRICE 		     (AISLE_SIZE + AISLE_SIZE + 1) //97//97  -48    //�ϵ��۸���Ϣ
#define VMC_L_SUM 				     76   //��Ʒ���ۻ�����Ϣ



#define ZH_L_TRADE					 21 //��PC�����������ݳ���
#define ZH_L_DATETIME				 8
#define ZH_L_AISLEINFO				 8
#define ZH_L_AISLECOUNT				 6


//�������
#define CHUHUO_OK  	        0x00   		//�����ɹ�
#define CHUHUO_PRICE_ERR 	0x01		//����Ϊ�����PC���ֽ�ۿʽ����ʱ ���͵Ŀۿ�������Ʒ���۲���
#define CHUHUO_QUEHUO		0x02		//ȱ��
#define CHUHUO_AISLE_FAULT  	0x03	//��������
#define CHUHUO_AISLE_NOEXIST	0x04 	//�޴˻���
#define CHUHUO_SYS_FAULT	0x05		//ϵͳ����
#define CHUHUO_UNABLE_COST	0x06		//���㣬������Ǯ
#define CHUHUO_UNABLE_PAYOUT	0x07	//��������
#define CHUHUO_UNABLE_PC	0x08		//��Ͷ���ֽ� ��ֹ���ֽ�(TRADE_CASH || TRADE_ONECARD)��ʽ���� 
#define CHUHUO_YACHAO_FAIL	0x09		//ѹ��ʧ��
#define CHUHUO_CABINET_ERR	0x10		//��Ŵ���
#define CHUHUO_TYPE_ERR		0x11		//�������Ͳ��ǻ����Ż���ID��
#define CHUHUO_ID_ERR		0x12		//��Ʒ��ID�Ŵ���
#define CHUHUO_NO_ERR		0x13		//�����Ŵ���
#define CHUHUO_BUTTON_ERR	0x14		//���û�����ѡ����60���ھܾ�PC����
#define CHUHUO_ERR		0x99		//δ֪����


//=============================================================================
//���﷽ʽ
//=============================================================================

#define TRADE_CASH				 0x01 //�ֽ�֧��
#define TRADE_ONE_CARD			 0x02 //һ��ͨ
#define TRADE_UNION_CARD 	 	 0x11//������
#define TRADE_PC_NORMAL			 0x21//PC ��������
#define TRADE_PC_ALIPAY			 0x41//֧����

#define BATCH_FLASH_PAGE		3997
#define TRADE_FLASH_PAGE		3999
#define BATCH_FLASH_SIZE		76

#define FLASH_WRITE_SIZE	    510 //����дflash һҳ ����ֽ���

#define TRADE_PAGE_SUM			(TRADE_PAGE_MAX + 1)//���׼�¼������
#define OFFLINE_REPORT_GPRS		0
#define OFFLINE_REPORT_PC		1


//=============================================================================
//������Ϣ
//=============================================================================
#define NONSUPPORT				 0x99 //��֧��
#define NONSUPPORT_BYBCD		 99 //��֧��


//=============================================================================
//�꺯�������ʽ
//=============================================================================

#define SN_NO(n)		(((n)/16-1)*COLUMN+((n)&0x0f))//�߼����ת1-48������				
#define ARR_NO(n)			    (((n)-1)/8)     //���ݻ�����Ų��������±��
#define ARR_BIT(n)				(((n)-1)%8)		//���ݻ�����Ų�������Ԫ��λ��bit0-7
#define BIT_MASK(n)				(0x01<<ARR_BIT(n))//��������

#define  HEC_to_ASC(n) ((n) + 48)//������
#define  ASC_to_HEC(n) ((n) - 48)
#define HUINT16(n)				(unsigned char)(((n) >> 8) & 0xff)//16λ��ֵ��8λ
#define LUINT16(n)				(unsigned char)((n) & 0xff)//16λ��ֵ��8λ	
#define H0UINT32(n)				(unsigned char)(((n) >> 24) & 0xff)//32λ��ֵ��һ����8λ
#define H1UINT32(n)				(unsigned char)(((n) >> 16) & 0xff)//32λ��ֵ�ڶ�����8λ
#define L0UINT32(n)				(unsigned char)(((n) >> 8) & 0xff)//32λ��ֵ��һ����8λ
#define L1UINT32(n)				(unsigned char)((n) & 0xff)//32λ��ֵ�ڶ�����8λ
#define yoffset_of(type,member) ({(INT32U & ((type*)(0)->member));})
//32λ������ptr�ṹ����member��ַ��type�ṹ������member�ṹ���Ա
#define ycontain_of(ptr,type,member) ({	\
		(type*)((INT32U*)ptr - yoffset_of(type,member)); \
		})
	//����unsigned char �������ϳ�unsigned short������
#define INTEG16(h,l)			(unsigned short)(((unsigned short)(h) << 8) | ((l) & 0xff))
	//�ĸ�unsigned char �������ϳ�unsigned int������
#define INTEG32(h0,l0,h1,l1)	(unsigned int)(((unsigned int)(h0) << 24) | \
									((unsigned int)(l0) << 16) | \
									((unsigned int)(h1) << 8) | \
									((l1) & 0xff))



//=============================================================================
//Э����ṹ������
//=============================================================================

typedef struct _zhihui_msg{
	//����ͷ
	unsigned short len;//���ĳ��� ���������ȱ�������ͷ - 4 + ������ 4λ ����
	unsigned int terminal;//8λ ����
	unsigned char  version;//2λ ����
	//unsigned char  reserve[6];//6λ���� �ַ���
	unsigned char  direction; //����1λ �ַ���
	unsigned char  contrl; // ������ 2λ ����
	//���Ͱ�
	unsigned char  send_msg[SEND_SIZE];
	unsigned short send_len;
	//Ӧ���
	unsigned char  recv_msg[RECV_SIZE];
	unsigned short recv_len;
	
	unsigned char  ship_cache_len;
	unsigned char  recv_err_num;	
}ZHIHHUI_MSG;


//=============================================================================
//��Ʒ��Ϣ�ṹ������
//=============================================================================

typedef struct _good_info{
	unsigned char	num;//��Ʒ���ڻ�����1-24
	unsigned char	logic_no;//��Ʒ�߼���� ʮ����

}GOOD_INFO;	

//=============================================================================
//�ۻ�����Ϣ�ṹ������
//=============================================================================

typedef struct _machine_info{
	unsigned char row;
	unsigned char column;
	unsigned char aisle_sum;//ʵ�ʻ�����
	unsigned char mac_t;//��������
	unsigned int mac_sn; //�������
	//unsigned char mac_sn[4];
	unsigned int vmc_sn;//���ذ汾��
	unsigned char  drink_size;	
	unsigned char c_no;//�����
}MACHINE_INFO;


//=============================================================================
//������Ϣ�ṹ������
//=============================================================================

typedef struct _data_st{

	//����������Ϣ
	MACHINE_INFO machine_info;
	//����״̬	
	unsigned char	vend_mac_state;//�ۻ���״̬ Bit7��������Ӧ��;Bit6����ģ�����;Bit5�����¶��쳣;Bit4�����¶��쳣;
						   //Bit3�����¶ȴ���������;Bit2�����¶ȴ���������;Bit1ϵͳʱ�Ӳ����������棩;Bit0��������Ӧ��(��ͣӪҵ);
	unsigned char	mdb_mac_state;//ֽ����״̬Bit7����;Bit6ֽ�Ҷ�����ֽ�����ڣ�;Bit5ֽ����Ǯ������Ǯ�䣩;Bit4ֽ����Ǯ�䱻ȡ�ߣ�Ǯ��;
    					  //Bit3ֽ����ROMУ���Bit2;ֽ��������������;Bit1ֽ��������������;Bit0ֽ������Ӧ��;
	unsigned char	coin_mac_state;//Ӳ����״̬Bit7Ӳ����1Ԫȱ�ң�Ǯ�ң�;Bit6Ӳ����5��ȱ�ң�Ǯ�ң�;Bit5Ӳ��֧��������Ӳ�ҿڣ�;
	//Bit4Ӳ�ҽ��ն�����Ӳ�ҿڣ�;Bit3Ӳ����ROMУ���;Bit2Ӳ��������������;Bit1Ӳ����������ѹ�ͣ����棩;Bit0Ӳ������Ӧ��
	unsigned char	light_state;//����״̬0X00���Զ���0X01�����գ�0X02������	   
	unsigned char	lamp_time[4];//�չ��ʱ��1-2������ʱ��HHMMѹ��3-4��ֹͣʱ��HHMMѹ��
	unsigned char	left_room_state;//����״̬
	unsigned char	right_room_state;//����״̬
	unsigned char	energy_save_time[4];//����ʱ��
	unsigned char	cool_temp[2];//�����¶� �������� bit7��ʾ����
	unsigned char	hot_temp[2];   //�����¶� �������� bit7��ʾ����
	unsigned char	run_state; //����״̬Bit1:0�Źأ�1�ſ�Bit0:0Ӫҵ��1��ͣӪҵ
	//�ͻ������������Ϣ
	unsigned short	bill_num;//ֽ������
	unsigned short	coin_num1;//Ӳ��0.5Ԫ����
	unsigned short	coin_num2;//Ӳ��1Ԫ����
	unsigned short	coin_amount;//Ӳ�ҽ��ս��
	unsigned short	bill_amount;//ֽ�ҽ��ս�
	unsigned short  card_amount;
	unsigned short  all_amount;
	unsigned short	change_amount;//����
	unsigned char	sel_num;//�ͻ�ѡ�������
	unsigned short	sel_id; //�ͻ�ѡ����Ʒ���
	unsigned short	sel_amount;//�ͻ�ѡ����Ʒ�۸�	
	unsigned char   escrow_flag;
	//PC   ������Ϣ
	unsigned char	trade_run_no;//���ذ��������ˮ�� 0-255
	unsigned char	trade_run_num[3];//PC��������λBCD����ˮ��
	unsigned char	trade_gprs_rum_num;
	unsigned char	trade_card_no[10];//10λΪ20λ���ű���ѹ�������Ų���20λǰ��0��
	unsigned char	trade_state;//������� 
	unsigned char	trade_pay_mode;//֧����ʽ
	unsigned short	trade_price;
	unsigned char	trade_time[7];//����ʱ��
	unsigned char   trade_num;
	unsigned char	trade_sn;
	
	unsigned char	sell_config;  //��������
	unsigned char	usr_trade;// 1��ʾ�û��������ۻ��� �Ự	  0����		
	unsigned char	driver_state;//�������� ��ͣӪҵ
	unsigned char	online;
	//unsigned char	err_report_pc;//�������PC����������
	unsigned char   isWeihu;//����ά��ģʽ
	//unsigned char   clear_state;//�����־ 1����
	unsigned char   door_state;//��״̬��־ 1 ��ʾ�ɿ�
	unsigned char   rt_state; //�����Ӧ 1��ʾ���˿��� 0 ��ʾ��

	//gprs
	unsigned short	read_index;
	unsigned char	read_sub_index;
	unsigned short	write_index;
	
}MACDATA_ST;

//=============================================================================
//�����ǻ�PC������ṹ��
//=============================================================================


//=============================================================================
//��������
//=============================================================================
extern MACDATA_ST MACData;
extern ZHIHHUI_MSG zhihui_msg;
extern GOOD_INFO vmc_goods[AISLE_SIZE];

//=============================================================================
//��������
//=============================================================================
unsigned char crc_check(unsigned char *msg, unsigned short len);
extern void zh_task3_post(unsigned char type);
extern unsigned char zh_task3_pend(uint32_t payAllMoney);
extern void zh_vmc_post(unsigned char type);
extern void vmcEndTrade(void);
void pollHuman(unsigned char flag);
void pollDoorAPI(unsigned char flag);

//�ǻ۽ӿ�
void zh_goodsID_set(void *pc_msg);

void zh_fault_update(void);
void zh_goods_update(unsigned char mode);
void zh_room_update(void);
void zh_run_update(void);
void zh_all_update(void);
void zh_init(void);
extern void vmcEndTrade(void);


unsigned int getAisleInfo(unsigned char logicNo,unsigned char type);
unsigned char setAisleInfo(unsigned char logicNo,unsigned char type,const unsigned int value);
unsigned char vmc_check_huodao(unsigned char logicNo,unsigned char cabinetNo);
unsigned char vmc_check_no(unsigned char no);
unsigned char vmc_batch_info_read(unsigned char *data);
unsigned char vmc_batch_info_write(void);
unsigned char vmc_trade_info_read(void);
unsigned char vmc_trade_info_write(void);

unsigned char vmc_logic_to_no(unsigned char logic_no,GOOD_INFO *good_info);
unsigned char vmc_sn_to_no(unsigned char sn);

unsigned char pc_trade_info_clear(unsigned char mode);

unsigned char vmc_data_clr(MACDATA_ST *mac_data);

void gprs_clear_index(void);








#endif
