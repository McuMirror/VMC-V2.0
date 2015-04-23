/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           READERACCEPTERAPI
** Last modified Date:  2013-03-04
** Last Version:         
** Descriptions:        ˢ�����м������غ�������                     
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
** Function name:       ReaderDevInit
** Descriptions:        �������豸��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void ReaderDevInitAPI()
{
	switch(SystemPara.CashlessDeviceType)
	{		
		case MDB_READERACCEPTER:
			//1.����ֽ����
			MsgAccepterPack.ReaderBack = MBOX_READERINITDEV;			
			OSMboxPost(g_ReaderBackMoneyMail,&MsgAccepterPack);
			break;		
	}
	return;
}

/*********************************************************************************************************
** Function name:       GetReaderDevMoneyInAPI
** Descriptions:        ��ö������豸���ձ�״̬
** input parameters:    readerType=1�в忨,2û�忨
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint32_t GetReaderDevMoneyInAPI(uint8_t *readerType)
{
	unsigned char ComStatus;
	MessagePack *ReaderMsg;
	uint32_t money,ReturnBack;

	switch(SystemPara.CashlessDeviceType)
	{		
		case MDB_READERACCEPTER:
			ReaderMsg = OSMboxPend(g_ReaderMoneyMail,10,&ComStatus);
			//TraceReader("\r\nMiddReadermdb=%d\r\n",ComStatus);
			if(ComStatus == OS_NO_ERR)
			{
				if((ReaderMsg->ReaderCmd)==MBOX_READERMONEY)
				{
					money = ReaderMsg->ReaderMoney;				
					TraceReader("\r\nMiddReadermiddlevalue=%ld",money);
					ReturnBack = money;
					*readerType = 1;					
				}
				else if((ReaderMsg->ReaderCmd)==MBOX_READERMONEYOUT)
				{
					money = ReaderMsg->ReaderMoney;				
					TraceReader("\r\nMiddReadervalue=%ld\r\n",money);
					ReturnBack = money;
					*readerType = 2;
				}
			}
			else
				ReturnBack = 0;	
			break;					
	}
	return ReturnBack;
	
}




/*********************************************************************************************************
** Function name:       ReaderDevDisable
** Descriptions:        ���ܶ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void ReaderDevDisableAPI(void)
{
	switch(SystemPara.CashlessDeviceType)
	{		
		case MDB_READERACCEPTER:			
			MsgAccepterPack.ReaderBack = MBOX_READERDISABLEDEV;			
			OSMboxPost(g_ReaderBackMoneyMail,&MsgAccepterPack);
			break;					
	}
	return;	
}






/*********************************************************************************************************
** Function name:       ReaderDevEnable
** Descriptions:        ʹ�ܶ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void ReaderDevEnableAPI(void)
{
	switch(SystemPara.CashlessDeviceType)
	{		
		case MDB_READERACCEPTER:			
			MsgAccepterPack.ReaderBack = MBOX_READERENABLEDEV;			
			OSMboxPost(g_ReaderBackMoneyMail,&MsgAccepterPack);
			break;					
	}
	return;
}


/*********************************************************************************************************
** Function name:       ReaderDevVendoutAPI
** Descriptions:        ˢ��������
** input parameters:    money���׽��(��Ϊ��λ),vendrst=0�ۿ���׳ɹ�,1�ۿ����ʧ��
** output parameters:   ��
** Returned value:      ���׳ɹ�1������ʧ��0
*********************************************************************************************************/
uint8_t ReaderDevVendoutTestAPI(uint16_t money,uint8_t vendrst)
{		
	MessagePack *AccepterMsg;
	uint8_t err,i,returnvalue;
	//uint32_t backmoney;
	//unsigned char ComStatus;

	switch(SystemPara.CashlessDeviceType)
	{		
		case MDB_READERACCEPTER:
			//2.�����������Ϳ�Ǯ����
			OSMboxAccept(g_ReaderBackMoneyMail);
			MsgAccepterPack.ReaderBack = MBOX_READERVENDREQUEST;	
			MsgAccepterPack.ReaderPrice = money;
			OSMboxPost(g_ReaderBackMoneyMail,&MsgAccepterPack);	
			//OSTimeDly(OS_TICKS_PER_SEC*2);
			
			for(i=0;i<10;i++)
			{
				//3.�ȴ��Ƿ������Ǯ
				AccepterMsg = OSMboxPend(g_ReaderBackCmdMail,OS_TICKS_PER_SEC*5,&err);//�����Ƿ�������				
				if(err == OS_NO_ERR) 
				{
					//4.�����Ǯ
					if(AccepterMsg->ReaderBackCmd == MBOX_READERVENDOUT)
					{
						TraceReader("\r\nMiddReader 10.VendSuccess");
						OSTimeDly(OS_TICKS_PER_SEC*2);
						OSMboxAccept(g_ReaderBackCmdMail);
						OSMboxAccept(g_ReaderBackMoneyMail);
						//���ͽ��׳ɹ���������
						if(vendrst==0)
						{
							TraceReader("\r\nMiddReader 10.VendSUCC");
							MsgAccepterPack.ReaderBack = MBOX_READERVENDSUCCESS;
							//MsgAccepterPack.ReaderPrice = money;
							OSMboxPost(g_ReaderBackMoneyMail,&MsgAccepterPack);
							returnvalue = 1;
						}
						//���ͽ���ʧ�ܸ�������
						else if(vendrst==1)
						{
							TraceReader("\r\nMiddReader 10.VendFail");
							MsgAccepterPack.ReaderBack = MBOX_READERVENDFAIL;
							OSMboxPost(g_ReaderBackMoneyMail,&MsgAccepterPack);
							
							TraceReader("\r\nMiddReader 10.VendFialComp");
							OSTimeDly(OS_TICKS_PER_SEC*2);
							OSMboxAccept(g_ReaderBackCmdMail);
							OSMboxAccept(g_ReaderBackMoneyMail);
							MsgAccepterPack.ReaderBack = MBOX_READERVENDCOMP;	
							OSMboxPost(g_ReaderBackMoneyMail,&MsgAccepterPack);
							returnvalue = 0;
						}						
						break;
					}
					//5.�������Ǯ�����ͽ���ʧ�ܸ�������
					else if(AccepterMsg->ReaderBackCmd == MBOX_READERVENDDENY)
					{
						TraceReader("\r\nMiddReader 10.VendFialComp");
						OSTimeDly(OS_TICKS_PER_SEC*2);
						OSMboxAccept(g_ReaderBackCmdMail);
						OSMboxAccept(g_ReaderBackMoneyMail);
						MsgAccepterPack.ReaderBack = MBOX_READERVENDCOMP;	
						//MsgAccepterPack.ReaderPrice = money;
						OSMboxPost(g_ReaderBackMoneyMail,&MsgAccepterPack);
						returnvalue = 0;
						break;
					}
				}
			}
			//5.û���յ���������Ϣ�����ͽ���ʧ�ܸ�������
			if(i >= 10)
			{
				TraceReader("\r\nMiddReader 10.VendFialComp");
				OSMboxAccept(g_ReaderBackCmdMail);
				OSMboxAccept(g_ReaderBackMoneyMail);
				MsgAccepterPack.ReaderBack = MBOX_READERVENDCOMP;	
				//MsgAccepterPack.ReaderPrice = money;
				OSMboxPost(g_ReaderBackMoneyMail,&MsgAccepterPack);	
				returnvalue = 0;
			}	
									
			break;					
	}
	return returnvalue;	
}


/*********************************************************************************************************
** Function name:       ReaderDevVendoutAPI
** Descriptions:        vmcֻ���ͽ��������������
** input parameters:    money���׽��
** output parameters:   ��
** Returned value:      ���ͳɹ�1������ʧ��0
*********************************************************************************************************/
uint8_t ReaderDevVendoutRPTAPI(uint16_t money)
{		
	MessagePack *AccepterMsg;
	uint8_t err,i,returnvalue;
	//uint32_t backmoney;
	//unsigned char ComStatus;

	switch(SystemPara.CashlessDeviceType)
	{		
		case MDB_READERACCEPTER:
			OSMboxAccept(g_ReaderBackMoneyMail);
			MsgAccepterPack.ReaderBack = MBOX_READERVENDREQUEST;	
			MsgAccepterPack.ReaderPrice = money;
			OSMboxPost(g_ReaderBackMoneyMail,&MsgAccepterPack);	
			//OSTimeDly(OS_TICKS_PER_SEC*2);
			
			for(i=0;i<3;i++)
			{
				//3.�ȴ��Ƿ������Ǯ
				AccepterMsg = OSMboxPend(g_ReaderBackCmdMail,OS_TICKS_PER_SEC*4,&err);//�����Ƿ�������				
				if(err == OS_NO_ERR) 
				{
					//4.�����Ǯ����������ɹ�
					if(AccepterMsg->ReaderBackCmd == MBOX_READERVENDOUT)
					{
						TraceReader("\r\nMiddReader 10.VendRptSuccess");						
						returnvalue = 1;
						break;
					}
					//5.�������Ǯ����������ʧ��
					else if(AccepterMsg->ReaderBackCmd == MBOX_READERVENDDENY)
					{
						TraceReader("\r\nMiddReader 10.VendFialComp");
						OSTimeDly(OS_TICKS_PER_SEC*2);
						OSMboxAccept(g_ReaderBackCmdMail);
						OSMboxAccept(g_ReaderBackMoneyMail);
						MsgAccepterPack.ReaderBack = MBOX_READERVENDCOMP;	
						//MsgAccepterPack.ReaderPrice = money;
						OSMboxPost(g_ReaderBackMoneyMail,&MsgAccepterPack);
						returnvalue = 0;
						break;
					}
				}
			}
			//5.û���յ���������Ϣ�����ͽ���ʧ�ܸ�������
			if(i >= 3)
			{
				TraceReader("\r\nMiddReader 10.VendFialComp");
				OSMboxAccept(g_ReaderBackCmdMail);
				OSMboxAccept(g_ReaderBackMoneyMail);
				MsgAccepterPack.ReaderBack = MBOX_READERVENDCOMP;	
				//MsgAccepterPack.ReaderPrice = money;
				OSMboxPost(g_ReaderBackMoneyMail,&MsgAccepterPack);					
				returnvalue = 0;
			}	
									
			break;					
	}
	return returnvalue;	
}

/*********************************************************************************************************
** Function name:       ReaderDevVendoutResult
** Descriptions:        vmc���ͽ��׽����������
** input parameters:    result���׽��,1�ɹ�,2ʧ��
** output parameters:   ��
** Returned value:      ���׳ɹ�1������ʧ��0
*********************************************************************************************************/
void ReaderDevVendoutResultAPI(uint8_t result)
{		
		
	switch(SystemPara.CashlessDeviceType)
	{		
		case MDB_READERACCEPTER:
			OSMboxAccept(g_ReaderBackCmdMail);
			OSMboxAccept(g_ReaderBackMoneyMail);
			//���ͽ��׳ɹ���������
			if(result == 1)
			{
				
				TraceReader("\r\nMiddReader 10.VendSUCC");				
				MsgAccepterPack.ReaderBack = MBOX_READERVENDSUCCESS;	
				//MsgAccepterPack.ReaderPrice = money;
				OSMboxPost(g_ReaderBackMoneyMail,&MsgAccepterPack);				
			}
			//���ͽ���ʧ�ܸ�������
			else if(result == 2)
			{
				TraceReader("\r\nMiddReader 10.VendFail");
				MsgAccepterPack.ReaderBack = MBOX_READERVENDFAIL;	
				//MsgAccepterPack.ReaderPrice = money;
				OSMboxPost(g_ReaderBackMoneyMail,&MsgAccepterPack);

				TraceReader("\r\nMiddReader 10.VendFialComp");
				OSTimeDly(OS_TICKS_PER_SEC*2);
				OSMboxAccept(g_ReaderBackCmdMail);
				OSMboxAccept(g_ReaderBackMoneyMail);
				MsgAccepterPack.ReaderBack = MBOX_READERVENDCOMP;	
				OSMboxPost(g_ReaderBackMoneyMail,&MsgAccepterPack);
			}
			break;
	}
}


/*********************************************************************************************************
** Function name:       GetReaderDevState
** Descriptions:        ��ö������豸�ĵ�ǰ״̬
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void GetReaderDevStateAPI()
{
	return;	
}


/*********************************************************************************************************
** Function name:       ReaderDevCashSaleAPI
** Descriptions:        vmc������ϸ�ֽ��׼�¼�������������ڱ�����ϸ��Ϣ
** input parameters:    money���׽��
** output parameters:   ��
** Returned value:      ���ͳɹ�1������ʧ��0
*********************************************************************************************************/
void ReaderDevCashSaleAPI(uint16_t money)
{		
	//MessagePack *AccepterMsg;
	//uint8_t err,i,returnvalue;
	//uint32_t backmoney;
	//unsigned char ComStatus;

	switch(SystemPara.CashlessDeviceType)
	{		
		case MDB_READERACCEPTER:
			if(stDevValue.ReaderCashSale==1)
			{
				TraceReader("\r\nMiddReader CashSale=%ld",money);
				OSMboxAccept(g_ReaderBackMoneyMail);
				MsgAccepterPack.ReaderBack = MBOX_READERCASHSALE;	
				MsgAccepterPack.ReaderPrice = money;
				OSMboxPost(g_ReaderBackMoneyMail,&MsgAccepterPack); 
			}
			break;					
	}
}



