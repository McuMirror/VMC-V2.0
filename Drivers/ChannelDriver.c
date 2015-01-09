/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           ChannelAccepter.c
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        ���ڻ����ײ���������                     
**------------------------------------------------------------------------------------------------------
** Created by:          liya
** Created date:        2013-01-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/

#include "..\config.h"

//ͨ�Ű����к�
uint8_t chSn = 1;

struct ChannelErrForm stChannelErrForm;


/*********************************************************************************************
**��   ��   ��:	ChannelHandle
**��������˵����������������
**�������������Handletype����������;Add���ܿػ�����ַ;CheckFlag:��������Ƿ�����1������0�ر�
**����������������
**�� �� ����ֵ��1���ɹ���0������ʧ�ܣ�
**********************************************************************************************/
uint8_t ChannelHandle(unsigned char Handletype,unsigned char Add,unsigned char CheckFlag,unsigned char Binnum)
{
	uint8_t ackdata[36];
	uint8_t res = 0,channel,index = 0;
	uint16_t data;
	
	channel = (Binnum == 1) ? 2 : 4;
	index = (Binnum == 1) ? 0 : 1;
	data = (CheckFlag == 1) ? 0x0100 : 0x0000;
	if(Handletype == CHANNEL_OUTGOODS)
	{
		res = EvbConversation(channel,0xED,chSn,OUTGOODSCMD,Add,data,ackdata);
		chSn = (chSn < 255) ? chSn + 1 : 1;
		if(res == 1)
		{
			stChannelErrForm.HuoDaoBanFlag[index] = 0;
			if(ackdata[5]==0x80)
				stChannelErrForm.GOCErrFlag[index]= 1;
			else
				stChannelErrForm.GOCErrFlag[index]=0;
			return ackdata[5];//�����ɹ�
		}
		else
		{
			stChannelErrForm.HuoDaoBanFlag[index] = 1;
			return 0xff;
		}		
	}
	else if(Handletype == CHANNEL_CHECKOUTRESULT)
	{
		res = EvbConversation(channel,0xED,chSn,CHECKOUTRESULT,Add,data,ackdata);
		if(res == 1)
		{
			stChannelErrForm.HuoDaoBanFlag[index]=0;
			chSn = (chSn < 255) ? chSn + 1 : 1;
			if(ackdata[5]==0x80)
				stChannelErrForm.GOCErrFlag[index]=1;	
			else
				stChannelErrForm.GOCErrFlag[index]=0;
			return ackdata[5];//�����ɹ�
		}
		else
		{
			stChannelErrForm.HuoDaoBanFlag[index]=1;
			return 0xff;
		}
	}
	else if(Handletype == CHANNEL_CHECKSTATE)
	{
		res = EvbConversation(channel,0xED,chSn,CHECKCHANNEL,Add,data,ackdata);	
		if(res == 1)
		{
			stChannelErrForm.HuoDaoBanFlag[index]=0;
			chSn = (chSn < 255) ? chSn + 1 : 1;
			if(ackdata[5]==0x80)
				stChannelErrForm.GOCErrFlag[index]=1;	
			else
				stChannelErrForm.GOCErrFlag[index]=0;
			return ackdata[5];//�����ɹ�
		}
		else
		{
			stChannelErrForm.HuoDaoBanFlag[index]=1;
			return 0xff;
		}
	}

	return 0xff;
	
}

/*--------------------------------------------------------------------------------
										�޸ļ�¼
1.���ڣ�2013.11.6 �޸��ˣ�liya 
  ���ݣ����ļ��еĺ��������������
--------------------------------------------------------------------------------*/

