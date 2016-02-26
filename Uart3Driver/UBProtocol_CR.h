/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           UBProtocol.h
** Last modified Date:  2013-03-04
** Last Version:         
** Descriptions:        LCD����������غ�������                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-03-04
** Version:             V2.0
** Descriptions:        The original version
**------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
********************************************************************************************************/



#ifndef __UBPROTOCOL_CR_H__
#define __UBPROTOCOL_CR_H__

//==========
//Э�����Ϣ
//==========
#define VP_ERR_NULL     0
#define VP_ERR_PAR      1   //mission parameter error
#define VP_ERR_COM      2   //communication error

#define VP_SF             0xAA
#define VP_PROEASIV_SF    0xE5//����Э��
#define VP_PROTOCOL_VER   0x0//Э��汾
#define VP_PROEASIV_VER   0x40//����Э��
#define VP_PROTOCOL_ACK   0x80
#define VP_PROEASIV_ACK   0x01//����Э��
#define VP_PROTOCOL_NAK   0x00

//==========
//VMC-->PC
//==========
#define VP_ACK_RPT       1
#define VP_NAK_RPT       2
#define VP_POLL          3
#define VP_VMC_SETUP     4
#define VP_STATUS_RPT   5
#define VP_INFO_RPT      6
#define VP_PAYIN_RPT     8
#define VP_PAYOUT_RPT    9
#define VP_COST_RPT      10//�ۿ�󣬷��ؿۿ�ִ�н��;by gzz 20110823


#define VP_ACTION_RPT    11   //V2
#define VP_BUTTON_RPT    12


#define VP_MT_MIN_SEND  VP_TEXT_MSG
//#define VP_MT_MAX_SEND  VP_DEBT_RPT
#define VP_MT_MAX_SEND  VP_OFFLINEDATA_RPT//120419 by cq TotalSell 
//#define VP_MT_MAX_SEND  VP_STARTUP_RPT_1//120419 by cq TotalSell 

//==========
//PC-->VMC
//==========
#define VP_ACK             1
#define VP_NAK             2
#define VP_GET_SETUP_IND   4
#define VP_GET_STATUS      5
#define VP_GETINFO_IND     6
#define VP_CONTROL_IND     7
#define VP_PAYIN_IND     8
#define VP_PAYOUT_IND      9    //V2
#define VP_COST_IND        10//ִ�пۿ�����;by gzz 20110823

#define VP_RESET_IND       132
#define VP_HOUDAO_IND      135
#define VP_POSITION_IND    136


#define VP_MT_MIN_RECEIVE  VP_ACK
#define VP_MT_MAX_RECEIVE  VP_SET_HUODAO

//================
//Ӧ�ò��Э�鳣��
//================
#define VP_MAC_SET_H        0
#define VP_MAC_SET_L        1
#define COLUMN_NUM_SET      80
#define VP_DEV_COIN         0
#define VP_DEV_BILL         1
#define VP_DEV_ESCROWIN        2
#define VP_DEV_ESCROWOUT       3
#define VP_DEV_READER         2
#define VP_BUT_GAME           0
#define VP_BUT_NUMBER         1
#define VP_BUT_GOODS          2
#define VP_BUT_RETURN         4

#define VP_ACT_HEART          0
#define VP_ACT_ADMIN         1
#define VP_ACT_ADMINOUT    2



#define VP_CONTROL_BILLCOIN   0
#define VP_CONTROL_CABINETDEV 3
#define VP_CONTROL_PAYOUT     6
#define VP_CONTROL_GAMELED    16
#define VP_CONTROL_CLOCK      17
#define VP_CONTROL_SCALFACTOR 18
#define VP_CONTROL_INITOK     19
#define VP_CONTROL_HEFANGUI   21


#define VP_ADMIN_GOODSADDALL  1
#define VP_ADMIN_GOODSADDCOL  2
#define VP_ADMIN_CHANGEADD    3
#define VP_ADMIN_GOODSCOLUMN  4
#define VP_ADMIN_GOODSADDTRAY 8
#define VP_ADMIN_GETBILL      9
#define VP_ADMIN_GOODSBUHUO  10

#define VP_INFO_ERR           0
#define VP_INFO_TOTALVALUE    1
#define VP_INFO_VER           2
#define VP_INFO_HARD          3
#define VP_INFO_BILL          4
#define VP_INFO_COIN          5
#define VP_INFO_COINOUT    6
#define VP_INFO_BILLOUT    7


#define VP_TIME_OUT 150
#define VP_COM_RETRY 3



#define VPM_LEN_MAX   250
struct VMC_PC_MSGCR
{
    uint8_t   sf;                //fixed with 0xe7
	uint8_t   len;               //5~250, msg+5
    uint8_t   datLen;            //
	uint8_t   verFlag;           //0x01( none response ); 0x81( need response )
    uint8_t   sn;                //0~255
	uint8_t   msgType;           //
	uint8_t   msg[VPM_LEN_MAX];  //0~250
	uint16_t    chk;
};


struct VP_MissionCR
{
    struct  VMC_PC_MSG  send;
	struct  VMC_PC_MSG  receive;
	uint8_t  payInDev;
	uint16_t payInMoney;
	uint32_t payAllMoney;
	uint16_t payoutMoney;
	uint32_t changeMoney;
	uint32_t costMoney;
	uint8_t  payoutDev;
	uint8_t  type;
	uint8_t Column;
	uint8_t status;                     
	uint8_t ColumnSum;    
	uint8_t method; 
	uint8_t device; 
	uint8_t action;
	//��PC���·��豸״̬
	uint8_t bv_st;                      //ֽ����״̬��0=������1=�������ʱ���ã�2=���ϣ�3=�豸������
	uint8_t check_st;                   //ֽ��������״̬��0=������1=�������ʱ���ã�2=���ϣ�3=�豸������
	uint8_t cc_st;                      //Ӳ����״̬���������㹦�ܣ���0=������1=�������ʱ���ã�2=���ϣ�3=�豸������
	uint8_t vmc_st;                     //Ӳ��������״̬�����������ۻ�������0=������1= ����������Ʒȫ���ۿգ��������ʱ�䣬2=���ϣ�3=ά��ģʽ
	uint32_t change;                    //Ӳ���������ܽ��
	uint32_t recychange;                     //ֽ���������ܽ��	
};
//extern struct VP_Mission sysVPMission;


//unsigned char VPBusTxMsg( void );
extern void VPSerialInit_CR(void);
extern unsigned char VPMission_Poll_CR( void);
extern unsigned char VPMission_Payin_RPT_CR(uint8_t dev,uint16_t payInMoney,uint32_t payAllMoney);
extern unsigned char VPMission_Payout_RPT( uint8_t payoutDev,unsigned char Type, unsigned int payoutMoney, unsigned int payAllMoney );
extern unsigned char VPMission_Cost_RPT_CR( unsigned char Type, uint32_t costMoney, unsigned int payAllMoney );
extern unsigned char VPMission_Button_RPT_CR( void );
extern unsigned char VPMission_Status_RPT_CR(void);
extern unsigned char VPMission_Act_RPT_CR( unsigned char action);
extern unsigned char VPMission_Info_RPT_CR( uint8_t type );
#endif

