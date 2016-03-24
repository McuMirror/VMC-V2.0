/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           TuibiAPI
** Last modified Date:  2013-03-04
** Last Version:         
** Descriptions:        �˱ҳ�����غ�������                     
**------------------------------------------------------------------------------------------------------
** Created by:          gzz
** Created date:        2013-03-04
** Version:             V2.0
** Descriptions:        The original version
**------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
********************************************************************************************************/
#include "..\config.h"


/*********************************************************************************************************
** Function name:       IsTuibiAPI
** Descriptions:        �Ƿ��а���������
** input parameters:    ��
** output parameters:   ��
** Returned value:      1���£�0û����
*********************************************************************************************************/
uint8_t IsTuibiAPI()
{
	uint8_t err,returnvalue = 0;
	MessagePack *TuibiMsg;
	
	//��������˱Ұ���
	if(ReadPayBackKeyValue())
		returnvalue = 1;
	//���MDB�˱Ұ���
	TuibiMsg = OSMboxPend(g_CoinMoneyBackMail,5,&err);//�����Ƿ��˱�		
	if(err == OS_NO_ERR) 
	{
		if(TuibiMsg->CoinBackCmd == MBOX_COINRETURN)
		{
			returnvalue = 2;
			OSMboxAccept(g_CoinMoneyBackMail);
		}
	}
	if((returnvalue == 1)||(returnvalue == 2))
	{
		Trace("\r\n MiddleCoinReturn=%d",returnvalue);
		Buzzer();	
		ButtonRPTAPI(4,0,0);//�ϱ�pc������Ϣ
	}	
	return returnvalue;
}



/*********************************************************************************************************
** Function name:       ClrTuibiAPI
** Descriptions:        ����˱ұ�ʶ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void ClrTuibiAPI()
{
	uint8_t err;
	MessagePack *TuibiMsg;
	
	//��������˱Ұ���
	ReadPayBackKeyValue();
	//���MDB�˱Ұ���
	TuibiMsg = OSMboxPend(g_CoinMoneyBackMail,10,&err);//�����Ƿ��˱�		
	if(err == OS_NO_ERR) 
	{
		if(TuibiMsg->CoinBackCmd == MBOX_COINRETURN)
		{
			OSMboxAccept(g_CoinMoneyBackMail);
		}
	}	
}


