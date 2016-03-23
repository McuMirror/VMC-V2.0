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



#ifndef __UBPROTOCOL_SIMPLE_H__
#define __UBPROTOCOL_SIMPLE_H__

////==========
////Э�����Ϣ
////==========
#define VP_ERRSIMPLE_NULL     0
#define VP_ERRSIMPLE_PAR      1   //mission parameter error
#define VP_ERRSIMPLE_COM      2   //communication error
#define VP_TIMSIMPLEE_OUT     200

#define VPSIMPLE_SF             0xAA


//==========
//VMC-->PC
//==========
#define SIMPLE_INFO_RPT   0x01
#define SIMPLE_VEND_RPT   0x02
#define SIMPLE_BUTTON_RPT 0x03
#define SIMPLE_GET_ADMIN  0x04
#define SIMPLE_ADMIN_RPT  0x05

#define VP_MT_MIN_SENDSIMPLE  SIMPLE_INFO_RPT
#define VP_MT_MAX_SENDSIMPLE  SIMPLE_ADMIN_RPT

//==========
//PC-->VMC
//==========
#define SIMPLE_GET_INFO           0x01
#define SIMPLE_VEND_IND           0x02
#define SIMPLE_DISPLAY_IND        0x03
#define SIMPLE_RESULT_IND         0x05
#define SIMPLE_ADMIN_IND          0x06

#define VP_MT_MIN_RECEIVESIMPLE  SIMPLE_GET_INFO
#define VP_MT_MAX_RECEIVESIMPLE  SIMPLE_ADMIN_IND
#define SIMPLE_TIME_OUT 150


#define VPM_LEN_MAXSIMPLE   250
struct VMC_PC_MSGSIMPLE
{
    uint8_t   sf;                //fixed with 0xe7
	uint8_t   msgType;           //	  	
    uint8_t   datLen;            //
	uint8_t   len;               //5~250, msg+5
	uint8_t   msg[VPM_LEN_MAXSIMPLE];  //0~250
	uint16_t  chk;
};


struct VP_MissionSIMPLE
{
    struct  VMC_PC_MSGSIMPLE  send;
	struct  VMC_PC_MSGSIMPLE  receive;	
	//����
	uint8_t channel_id;
	uint8_t  SN;
	uint8_t channel_result;
    //display��ʾ
	uint8_t distype;   //��ʾ���ͣ�0�û���1ʱ�䣬2�ı���3��Ʒ����                  
	uint16_t payInMoney;//Ͷ����	
	//�ϱ�������Ӧ��Ʒid
	uint8_t btnchannel_id;                  //vmc�ϱ���PC������Ӧ��Ʒid
	//admin
	uint8_t admintype;                      //vmc�ϱ���PC����admin������1����ȫ��������2������ܻ�����3������������4�����ʱ�䣬5�Զ��˱�ʱ��
	uint8_t admincolumn;                    //vmc�ϱ���PC���Ĳ����Ĳ�Ż��߻�����
	uint8_t admincolumnsum;                 //vmc�ϱ���PC���Ĳ���������
	//��PC����vmc������Ϣ
	uint8_t getadmintype;                  //3�����ĳһ��������ŵ���Ʒʣ����,4��ȡ��ǰ�����ʱ��,5��ȡ��ǰ�Զ��˱�ʱ��
	uint8_t getadmincolumn;                //�������
	uint8_t getadmincolumnsum;             //��������
};
extern struct VP_MissionSIMPLE sysVPMissionSIMPLE;


//
//
////unsigned char VPBusTxMsg( void );
extern void VPSIMPLE_Sethdquery(uint8_t num);
extern void SIMPLESIMPLEVPSerialInit(void);
extern unsigned char VPMissionSIMPLESIMPLE_Poll( void );
extern unsigned char VPMissionSIMPLE_Button_RPT( uint8_t btnchannel_id );
extern unsigned char VPMissionSIMPLE_Vendout_RPT( uint8_t channel_result );
extern unsigned char VPMissionSIMPLE_Get_Admin2( unsigned char admintype);
extern void VPSIMPLE_Admin_Ind(void);
extern unsigned char VPMissionSIMPLE_Admin_RPT( unsigned char admintype,uint8_t admincolumn,uint8_t admincolumnsum);
extern unsigned char VPSIMPLE_Result_Ind(void);
#endif

