/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           CompressorAndLightAPI.c
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        �¿��豸��չʾ�ƿ���API�ӿ�                     
**------------------------------------------------------------------------------------------------------
** Created by:          liya
** Created date:        2013-01-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#include "..\config.h"



/*****************************************************************************
** Function name:	acdc_handle_API	
**
** Descriptions:         ACDC���ƺ���												 			
** parameters:		cprFlag:�Ƿ���ѹ������־  1����  0 �ر�
					ledFlag:�Ƿ���չʾ�Ʊ�־  1����  0 �ر�
					Binnum:�����
** Returned value:	1:�ɹ�;0:ʧ��;
*****************************************************************************/
unsigned char acdc_handle_API(unsigned char binnum,unsigned char ledFlag,unsigned char cprFlag)
{
	unsigned short cmd ;// �����ֽڿ��� չʾ�� ����λ����ѹ����

	TraceCompress("acdc_handle:bin= %d,led=%d,cpr=%d\r\n",binnum,ledFlag,cprFlag);
	
	if(binnum == 1 && SystemPara.Channel >= 3)
	{
		return liftTableLight(binnum,ledFlag);
	}
	else if(binnum == 2 && SystemPara.SubBinOpen == 1 && SystemPara.SubChannel >= 3)
	{
		return liftTableLight(binnum,ledFlag);
	}
	else
	{
		cmd = INTEG16(cprFlag,ledFlag);
		ACDCHandle(binnum,cmd);
	}
	
	return 1;
}

/*****************************************************************************
** Function name:	acdc_cpr_API	
**
** Descriptions:         ACDCѹ�������ƺ���												 			
** parameters:		cprFlag:�Ƿ���ѹ������־  1����  0 �ر�
			
					Binnum:�����
** Returned value:	1:�ɹ�;0:ʧ��;
*****************************************************************************/
unsigned char acdc_cpr_API(unsigned char binnum,unsigned char cprFlag)
{
	unsigned short cmd ;// �����ֽڿ��� չʾ�� ����λ����ѹ����
	unsigned char temp = 0; 
	TraceCompress("acdc_handle:bin= %d,cpr=%d\r\n",binnum,cprFlag);
	if(binnum == 1)
		temp = UserPara.LEDCtr.flag;
	else
		temp = UserPara.LEDCtr2Bin.flag;
	cmd = INTEG16(temp,cprFlag);

	ACDCHandle(binnum,cmd);
	return 1;
}

/*****************************************************************************
** Function name:	acdc_led_API	
**
** Descriptions:         ACDC�������ƺ���												 			
** parameters:		ledFlag:�Ƿ���������־  1����  0 �ر�
			
					Binnum:�����
** Returned value:	1:�ɹ�;0:ʧ��;
*****************************************************************************/
unsigned char acdc_led_API(unsigned char binnum,unsigned char ledFlag)
{
	unsigned short cmd ;// �����ֽڿ��� չʾ�� ����λ����ѹ����
	unsigned char temp = 0; 
	TraceCompress("acdc_handle:bin= %d,led=%d\r\n",binnum,ledFlag);
	if(binnum == 1)
		temp = UserPara.CompressorCtr.flag;
	else
		temp = UserPara.CompressorCtr2Bin.flag;
	cmd = INTEG16(ledFlag,temp);
	if(binnum == 1 && SystemPara.Channel >= 3)
	{
		return liftTableLight(binnum,ledFlag);
	}
	else if(binnum == 2 && SystemPara.SubBinOpen == 1 && SystemPara.SubChannel >= 3)
	{
		return liftTableLight(binnum,ledFlag);
	}
	else
	{
		ACDCHandle(binnum,cmd);
	}
		
	return 1;
}



/*****************************************************************************
** Function name:	API_ACDCHandle	
**
** Descriptions:	���豸����������ACDC��������	
**					
**														 			
** parameters:		HandleType:��������
					ChkFlag:�Ƿ����������
					Binnum:�����
** Returned value:	0:�ɹ�;1:ʧ��;
** 
*****************************************************************************/
unsigned char API_ACDCHandle(unsigned char flag,unsigned char binnum,unsigned char HandleType)
{
	MessagePack *RecvBoxPack;
	uint8_t err;
	uint16_t ACDCData = 0x0000;

	if(HandleType == OPENCOMPRESSORANDLIGHT)
	{
		ACDCData = 0x0101;
		MsgAccepterPack.Binnum = binnum;
		MsgAccepterPack.ACDCHandleType = ACDCHANDLE;
		MsgAccepterPack.ACDCData = ACDCData;	
	}
	else
	if(HandleType == CLOSECOMPRESSORANDLIGHT)
	{
		ACDCData = 0x0000;
		MsgAccepterPack.Binnum = binnum;
		MsgAccepterPack.ACDCHandleType = ACDCHANDLE;
		MsgAccepterPack.ACDCData = ACDCData;
	}
	else
	if(HandleType == OPENCOMPRESSOR)
	{
		if(flag==1)
			ACDCData = 0x0101;
		else
		if(flag==0)
			ACDCData = 0x0100;
		MsgAccepterPack.Binnum = binnum;
		MsgAccepterPack.ACDCHandleType = ACDCHANDLE;
		MsgAccepterPack.ACDCData = ACDCData;
	}
	else
	if(HandleType == OPENLIGHT)
	{
		if(flag==1)
			ACDCData = 0x0101;
		else
		if(flag==0)
			ACDCData = 0x0001;
		MsgAccepterPack.Binnum = binnum;
		MsgAccepterPack.ACDCHandleType = ACDCHANDLE;
		MsgAccepterPack.ACDCData = ACDCData;
	}
	else
	if(HandleType == CLOSECOMPRESSOR)
	{
		if(flag==1)
			ACDCData = 0x0001;
		else
		if(flag==0)
			ACDCData = 0x0000;
		MsgAccepterPack.Binnum = binnum;
		MsgAccepterPack.ACDCHandleType = ACDCHANDLE;
		MsgAccepterPack.ACDCData = ACDCData;
	}
	else
	if(HandleType == CLOSELIGHT)
	{
		if(flag==1)
			ACDCData = 0x0100;
		else
		if(flag==0)
			ACDCData = 0x0000;
		MsgAccepterPack.Binnum = binnum;
		MsgAccepterPack.ACDCHandleType = ACDCHANDLE;
		MsgAccepterPack.ACDCData = ACDCData;
	}
	OSMboxPost(g_ACDCMail,&MsgAccepterPack);
	RecvBoxPack = OSMboxPend(g_ACDCBackMail,100,&err);
	if(err == OS_NO_ERR)
	{
		return RecvBoxPack->HandleResult;
	}
	return 0;
}



/*****************************************************************************
** Function name:	acdc_hot_API	
**
** Descriptions:         ACDC���ƺ���												 			
** parameters:		
					Binnum:�����
** Returned value:	1:�ɹ�;0:ʧ��;
*****************************************************************************/
unsigned char acdc_hot_API(unsigned char binnum,const unsigned short temp)
{	
	//unsigned char res = 0;
	unsigned short hotTemp;
	binnum = binnum;
	Trace("\r\n acdc_hot_API send=\r\n");
	hotTemp = (temp > ACDC_MAX_HOT_TEMP) ?  ACDC_MAX_HOT_TEMP : temp;
	return SetTemperatureAPI(hotTemp);
}






/*****************************************************************************
** Function name:	acdc_getTemp_API	
**
** Descriptions:         ACDC��ȡ�¶�										 			
** parameters:		
					Binnum:�����
** Returned value:	�����¶�ֵ
*****************************************************************************/
unsigned short acdc_getTemp_API(unsigned char binnum)
{	
	binnum = binnum;
	return sysXMTMission.recPVTemp;
}





/*****************************************************************************
** Function name:	acdc_chuchou_API	
**
** Descriptions:         ACDC���ƺ���												 			
** parameters:		flag:�Ƿ񿪱�־  1����  0 �ر�
					Binnum:�����
** Returned value:	1:�ɹ�;0:ʧ��;
*****************************************************************************/
unsigned char acdc_chuchou_API(unsigned char binnum,unsigned char flag)
{
	unsigned short cmd ,res = 0;
	TraceCompress("acdc_chuchou_API:bin= %d,flag =%d\r\n",binnum,flag);
	cmd = INTEG16(flag,1);
	if(binnum == 1 && SystemPara.Channel >= 3)
	{
		return liftTableChuchou(binnum,flag);
	}
	else if(binnum == 2 && SystemPara.SubBinOpen == 1 && SystemPara.SubChannel >= 3)
	{
		return liftTableChuchou(binnum,flag);
	}
	else
	{
		res = ACDCHandle(binnum,cmd);
	}
	
	TraceCompress("res = %d\r\n",res);
	return ((res == 1) ?  1 : 0);

}



/*****************************************************************************
** Function name:	acdc_chuchou_API	
**
** Descriptions:         ACDC���ƺ���												 			
** parameters:		type:1 ���� 2 ���� 3���� 4����
					cabinet:�����1����,2����
** Returned value:	0�ر�  1����  2����  3��ʱ 0xff��֧�ָ��豸
*****************************************************************************/
unsigned char acdc_status_API(unsigned char cabinet,unsigned char type)
{

	unsigned  char temp = 0; 
	switch(type)
	{
		case 1:
			if(cabinet == 1)
				temp = UserPara.LEDCtr.flag & 0x01;
			else
				temp = UserPara.LEDCtr2Bin.flag & 0x01;
			break;
		case 2:
			if(cabinet == 1)
				temp = UserPara.CompressorCtr.flag & 0x01;
			else
				temp = UserPara.CompressorCtr2Bin.flag & 0x01;
			break;
		case 3:
			if(cabinet == 1)
				temp = UserPara.box1Hot.flag & 0x01;
			else
				temp = UserPara.box2Hot.flag & 0x01;
		break;
		case 4:
			if(cabinet == 1)
				temp = UserPara.box1Chuchou.flag & 0x01;
			else
				temp = UserPara.box2Chuchou.flag & 0x01;
		break;
		default:break;
	}
	temp=0xff;
	return temp;

}











/*--------------------------------------------------------------------------------
										�޸ļ�¼
1.���ڣ�2013.11.6 �޸��ˣ�liya 
  ���ݣ����ļ��еĺ��������������
--------------------------------------------------------------------------------*/




