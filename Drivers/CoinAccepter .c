/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           COINACCEPTER
** Last modified Date:  2013-03-04
** Last Version:         
** Descriptions:        Ӳ��������������غ�������                     
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
#include "CoinAccepter.h"


//��ǰֽ�����豸������
//uint8_t NowCoinDev = 0;
//Ӳ�ұ���ͨ����ֵ
uint32_t nCoinvalue[16]={0};
uint8_t NowCoinDevState = 0;

//����ֽ����ͨ��Ӧ�����ݰ���ʱ����
//extern unsigned int MDBCONVERSATIONWAITACKTIMEOUT;

MDBCOINERROR MdbCoinErr;




/*********************************************************************************************************
** Function name:       CoinDevInit
** Descriptions:        Ӳ�����豸��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      1ʹ��ʧ��,0ʹ�ܳɹ�
*********************************************************************************************************/
uint8_t CoinDevInit()
{
	unsigned char CoinRdBuff[36],CoinRdLen,ComStatus,i;	
	unsigned char j;
	uint8_t VMCdata[5]={0x01,0xff,0xff,0xff,0xff};
	memset(CoinRdBuff,0,sizeof(CoinRdBuff));
	for(j = 0; j < 16; j++) 
	{
		 stDevValue.CoinValue[j]=0;
		 //TraceCoin("\r\nqqDrvChangebuf[%d] = %d,%d", j,ChangerRdBuff[j+2],stDevValue.CoinNum[j]);
	}
	//Reset
	for(ComStatus = 0, j = 0; j < 10 && ComStatus != 1; j++)
	{
		ComStatus = MdbConversation(0x08,NULL,0x00,&CoinRdBuff[0],&CoinRdLen);
	}	
	//OSTimeDly(OS_TICKS_PER_SEC/2);
	LCDNumberFontPrintf(40,LINE15,2,"CoinAccepter-1");	
	//Setup
	for(ComStatus = 0, j = 0; j < 10 && ComStatus != 1; j++)
	{
		ComStatus = MdbConversation(0x09,NULL,0x00,&CoinRdBuff[0],&CoinRdLen);
	}	
	if(ComStatus == 1)
	{
		TraceCoin("\r\nDrvCoinAccepter");
		LCDNumberFontPrintf(40,LINE15,2,"CoinAccepter-2");
		
		stDevValue.CoinLevel = CoinRdBuff[0];
		stDevValue.CoinCode = ((((uint16_t)CoinRdBuff[1]) << 8) | CoinRdBuff[2]);
		stDevValue.CoinDecimal = 100;
	    for(i = 0; i < CoinRdBuff[4]; i++) 
	    {
		   stDevValue.CoinDecimal /= 10;
	    }
		TraceCoin("\r\nDrvCOINDec=%d,%d",CoinRdBuff[3],CoinRdBuff[4]);
		stDevValue.CoinScale = CoinRdBuff[3] * stDevValue.CoinDecimal;
		stDevValue.CoinRouting	= (((uint16_t)CoinRdBuff[5]) << 8) | CoinRdBuff[6];
		TraceBill("\r\nDrvCOINDec2=%ld,%ld,%ld", CoinRdBuff[3],stDevValue.CoinDecimal,stDevValue.CoinScale);
		
				
		for(i=0;i<CoinRdLen-7;i++)
		{
			if(CoinRdBuff[7+i] == 0xFF)
			{
			     break;
			}
			
			nCoinvalue[i] = ((uint32_t)CoinRdBuff[7+i] * stDevValue.CoinScale);	
			stDevValue.CoinValue[i] = nCoinvalue[i];
			TraceCoin("\r\nDrv[%ld,%ld]",(uint32_t)CoinRdBuff[7+i],stDevValue.CoinValue[i]);
		}
		for(i = 0; i < 16; i++) 
		{
			 TraceCoin("\r\n%dDrvcoin[%d] = %d", CoinRdLen,i,stDevValue.CoinValue[i]);
		}
		//OSTimeDly(OS_TICKS_PER_SEC*5);
	}
	//��ѯ
	for(ComStatus = 0, j = 0; j < 10 && ComStatus != 1; j++)
	{
		ComStatus = MdbConversation(0x0B,NULL,0x00,&CoinRdBuff[0],&CoinRdLen);
	}
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	LCDNumberFontPrintf(40,LINE15,2,"CoinAccepter-3");
	TraceCoin("\r\nDrv4.1=%d,%ld,%d,%ld",stDevValue.CoinLevel,stDevValue.CoinDecimal,stDevValue.CoinScale,stDevValue.CoinRouting);
	TraceCoin("\r\nDrv4.2=%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld",nCoinvalue[0],nCoinvalue[1],nCoinvalue[2],nCoinvalue[3],nCoinvalue[4],nCoinvalue[5],nCoinvalue[6],nCoinvalue[7]);
	//Identification
	ComStatus = MdbConversation(0x0f,0x00,1,&CoinRdBuff[0],&CoinRdLen);
	for(i=0;i<CoinRdLen;i++)
	{
		stDevValue.coinIDENTITYBuf[i] = CoinRdBuff[i];
	}
	TraceCoin("\r\nDrvCoinIdRec<< [%02d]-",CoinRdLen);
	for(i=0;i<CoinRdLen;i++)
	{
		TraceReader(" %#02x ",CoinRdBuff[i]);
	}
	TraceReader("\r\n");	
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	ComStatus = MdbConversation(0x0f,VMCdata,5,&CoinRdBuff[0],&CoinRdLen);
	//for(i=0;i<CoinRdLen;i++)
	//{
	//	stDevValue.coinIDENTITYBuf[i] = CoinRdBuff[i];
	//}
	TraceCoin("\r\nDrvCoinFEARec<< [%02d]-",CoinRdLen);
	for(i=0;i<CoinRdLen;i++)
	{
		TraceReader(" %#02x ",CoinRdBuff[i]);
	}
	TraceReader("\r\n");
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	ComStatus = MdbConversation(0x0A,NULL,0x00,&CoinRdBuff[0],&CoinRdLen);
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	CoinDevEnable();
	LCDNumberFontPrintf(40,LINE15,2,"CoinAccepter-4");	
	return MdbCoinErr.Communicate;
	/*
	else
	if(NowCoinDev == BILLDEV_RS232)
	{
		for(i=0;i<8;i++)
			nCoinvalue[i] = GetMemoryData(ADD_BILLVALUE1+i*4);
		CoinDevEnable();	
	}*/
	//���ڴ��ж�ȡӲ����ͨ����ֵ
	//for(i=0;i<8;i++)
	//	nCoinvalue[i] = GetMemoryData(ADD_COINVALUE1+i*4);
}


/*********************************************************************************************************
** Function name:       CoinDevDisable
** Descriptions:        ����Ӳ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void CoinDevDisable(void)
{
	uint8_t CoinValue[4]={0},CoinRdBuff[36],CoinRdLen,ComStatus;

	/*if(NowCoinDev == BILLDEV_RS232)
	{
		GbaControl_BillDisable();
	}
	else*/
	
	CoinValue[0] = 0x00;
	CoinValue[1] = 0x00;
	CoinValue[2] = 0xff;
	CoinValue[3] = 0xff;
   	ComStatus = MdbConversation(0x0C,CoinValue,4,&CoinRdBuff[0],&CoinRdLen);	
	if(ComStatus == 1)
		MdbCoinErr.Communicate = 0;
	else
		MdbCoinErr.Communicate = 1;
}






/*********************************************************************************************************
** Function name:       CoinDevEnable
** Descriptions:        ʹ��Ӳ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void CoinDevEnable(void)
{
	uint8_t CoinWrBuff[4]={0},CoinRdBuff[36],CoinRdLen;
	uint8_t ComStatus;
	/*if(NowCoinDev == BILLDEV_RS232)
	{
		if(nBillvalue[0]!=0)
			BillColumnSet |= 0x01;
		if(nBillvalue[1]!=0)
			BillColumnSet |= 0x02;
		if(nBillvalue[2]!=0)
			BillColumnSet |= 0x04;
		if(nBillvalue[3]!=0)
			BillColumnSet |= 0x08;
		if(nBillvalue[4]!=0)
			BillColumnSet |= 0x10;
		if(nBillvalue[5]!=0)
			BillColumnSet |= 0x20;
		if(nBillvalue[6]!=0)
			BillColumnSet |= 0x40;
		if(nBillvalue[7]!=0)
			BillColumnSet |= 0x80;
	}
	else*/
	
	if(nCoinvalue[8]!=0)
		CoinWrBuff[0] |= 0x01;
	if(nCoinvalue[9]!=0)
		CoinWrBuff[0] |= 0x02;
	if(nCoinvalue[10]!=0)
		CoinWrBuff[0] |= 0x04;
	if(nCoinvalue[11]!=0)
		CoinWrBuff[0] |= 0x08;
	if(nCoinvalue[12]!=0)
		CoinWrBuff[0] |= 0x10;
	if(nCoinvalue[13]!=0)
		CoinWrBuff[0] |= 0x20;
	if(nCoinvalue[14]!=0)
		CoinWrBuff[0] |= 0x40;
	if(nCoinvalue[15]!=0)
		CoinWrBuff[0] |= 0x80;
	
	if(nCoinvalue[0]!=0)
		CoinWrBuff[1] |= 0x01;
	if(nCoinvalue[1]!=0)
		CoinWrBuff[1] |= 0x02;
	if(nCoinvalue[2]!=0)
		CoinWrBuff[1] |= 0x04;
	if(nCoinvalue[3]!=0)
		CoinWrBuff[1] |= 0x08;
	if(nCoinvalue[4]!=0)
		CoinWrBuff[1] |= 0x10;
	if(nCoinvalue[5]!=0)
		CoinWrBuff[1] |= 0x20;
	if(nCoinvalue[6]!=0)
		CoinWrBuff[1] |= 0x40;
	if(nCoinvalue[7]!=0)
		CoinWrBuff[1] |= 0x80;
	
	CoinWrBuff[2] = 0xFF;
	CoinWrBuff[3] = 0xFF;
	TraceCoin("\r\nDrvCoinEnable=%x,%x,%x,%x",CoinWrBuff[0],CoinWrBuff[1],CoinWrBuff[2],CoinWrBuff[3]);
	//Enable
	ComStatus = MdbConversation(0x0C,CoinWrBuff,4,&CoinRdBuff[0],&CoinRdLen);
	if(ComStatus == 1)
		MdbCoinErr.Communicate = 0;
	else
		MdbCoinErr.Communicate = 1;
	
}


/*********************************************************************************************************
** Function name:       GetCoinDevState
** Descriptions:        ���Ӳ�����豸�ĵ�ǰ״̬
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void GetCoinDevState()
{
	unsigned char CoinRdBuff[36],CoinRdLen,ComStatus,i;

	for(i=0;i<8;i++)
	{
		if(nCoinvalue[i]!=0)
			break;
	}
	if(i>=8)
	{
		NowCoinDevState = 1;
	}
	/*if(NowCoinDev == BILLDEV_RS232)
	{
		ComStatus = GbaControl_BillAccept();
		if(ComStatus != 2)
		{
			NowCoinDevState = 0;		
		}	
		else
			NowCoinDevState = 1;
	}
	else*/
	
	ComStatus = MdbConversation(0x33,NULL,0x00,&CoinRdBuff[0],&CoinRdLen);
	if(ComStatus == 1)
	{			
		NowCoinDevState = 0;		
	}	
	else
	{			
		NowCoinDevState = 1;
	}
	
}

/*********************************************************************************************************
** Function name:       CoinDevProcess
** Descriptions:        Ӳ�����ձҲ���
** input parameters:    ��
** output parameters:   RecvMoney���������Ӳ�ҽ��
						CoinType���������Ӳ��ͨ��
** Returned value:      ��Ӳ�����뷵��1���޷���0
*********************************************************************************************************/
uint8_t CoinDevProcess(uint32_t *RecvMoney,unsigned char *CoinType,unsigned char *coinOptBack)
{
	unsigned char CoinRdBuff[36],CoinRdLen,ComStatus;
	uint8_t type=0;
	uint8_t  i;
	static uint8_t payoutFlag = 0;

	//Trace("6\r\n");	
	
	//��ѯӲ�����Ƿ����յ�Ӳ�ң��з���1
	ComStatus = MdbConversation(0x0B,NULL,0x00,&CoinRdBuff[0],&CoinRdLen);
	if(ComStatus == 1)
	{
		NowCoinDevState = 0;	
		MdbCoinErr.Communicate = 0;
		TraceCoin("\r\nDrvCoin= %02d-",CoinRdLen);
		for(i=0;i<16;i++)
		{
			TraceCoin(" %02x ",CoinRdBuff[i]);
		}
		TraceCoin("\r\n");
		if(CoinRdLen==0)
		{
			memset(&MdbCoinErr,0,sizeof(MdbCoinErr));	
		}
		for(i = 0; i < CoinRdLen; i++) 
		{    
			//report changer activity
			//coins dispensed manually�ֶ��˱�
			if(CoinRdBuff[i] & 0x80) 
			{        	      
				i++; //���Ե�2�ֽ�
			} 
			else if((CoinRdBuff[i] & 0xC0) == 0x40) 
			{	 
				//coins deposited��Ӳ��Ͷ��
				if(((CoinRdBuff[i] & 0x30) == 0x10) || ((CoinRdBuff[i] & 0x30) == 0)) //in tubes
				{	 
					//in cashbox
				    //CoinAcceptor_UpDateDeposit(COINACCEPTOR_STACKED,CoinRdBuff[i] & 0x0F);
					type = CoinRdBuff[i] & 0x0F;
					*RecvMoney = nCoinvalue[type];
					*CoinType = type;
					return 1;
				}
				i++; //���Ե�2�ֽ�
			} 
			else if((CoinRdBuff[i] & 0xE0) == 0x20) 
			{	 //slug
			}
			else 
			{	
				//status
				switch(CoinRdBuff[i]) 
				{						
				   case 0x01:			                 //escrow request	
				   	   payoutFlag ++;
					   if(payoutFlag >= 1)
					   {
					   		*coinOptBack = 1;
							payoutFlag = 0;
					   }
					   break;

				   case 0x02:			                 //changer pay out busy
					   break;

				   case 0x03:			                 //no credit
					   //���ܽ����Ӧͨ��
					   break;

				   case 0x04:			                 //defective tube sensor				   	   	
					   //����������
					   MdbCoinErr.sensor = 1;
					   break;

				   case 0x05:			                 //double arriver
				   	   break;

				   case 0x06:			                 //acceptor unplugged						   
					   break;

				   case 0x07:			                 //tube jam
					   //���ҿ���
					   MdbCoinErr.tubejam = 1;
					   break;

				   case 0x08:			                 //rom chksum err
					   //ROM����
					   MdbCoinErr.romchk = 1;
					   break;

				   case 0x09:			                 //coin routing err
					   //����ͨ������
					   MdbCoinErr.routing = 1;
					   break;

				   case 0x0A:			                 //changer busy
				   	   break;

				   case 0x0B:			                 //changer was reset
					   //�ո�λ
					   break;

				   case 0x0C:			                 //coin jam
					   //Ͷ�ҿ���
					    MdbCoinErr.jam = 1;
					   break;

				   case 0x0D:		              	     //possible credited coin removal
					   //��ͼ�Ƴ�Ӳ��
					   MdbCoinErr.removeTube = 1;
					   break;

				   default:	
				   	   memset(&MdbCoinErr,0,sizeof(MdbCoinErr));	
					   break;
				}
		    }
	    }
		
	}
	else
	{
		TraceCoin("\r\n Drvcoin commuFail");
		NowCoinDevState = 1;
		MdbCoinErr.Communicate = 1;
	}
	
	/*
	else
	if(NowCoinDev == BILLDEV_RS232)
	{
		ComStatus = GbaControl_BillAccept();	
		if(ComStatus == 1)
		{
			ComStatus = GbaControl_BillStack();
			MDBCONVERSATIONWAITACKTIMEOUT1 = 800;
			while(MDBCONVERSATIONWAITACKTIMEOUT1)
			{
				ComStatus = GbaControl_BillStacked();
				if(ComStatus == 1)
				{
					type = RecvBillType/8-1;
					*RecvMoney = nBillvalue[type];
					*BillType = type;
					MsgPack.BackType = TASKCMD_BILLIN;
					MsgPack.SubBackType = type;
					MsgPack.moneyback = nBillvalue[type];
					//���������DevieTask�������Hopper�����Ҳ���
					OSMboxPost(g_MoneyIN,&MsgPack);
					return 1;	
				}
				OSTimeDly(5);	
			}
		}
		//BillDevReject();
	}*/
	return 0;
}




/*********************************************************************************************************
** Function name:       CoinDevProcessExpanse
** Descriptions:        ��չָ����Ӳ����״̬
** input parameters:    ��
** output parameters:   
** Returned value:      �д򿪷���2�������������1,��������0
*********************************************************************************************************/
void CoinDevProcessExpanse(void)
{
    uint8_t CoinRdBuff[36],CoinRdLen,ComStatus,VMCdata[1]={0x05},i;
    
	
    ComStatus = MdbConversation(0x0F,VMCdata,1,&CoinRdBuff[0],&CoinRdLen);
	TraceCoin("\r\nDrvcoinExp>>%#02x,%#02x",0x0F,VMCdata[0]);
	if(ComStatus == 1)	
    {  	
	    if(CoinRdLen <= 16) 
		{				      
			for(i = 0; i < CoinRdLen-1; i=i+2) 
			{
				//TraceCoin("\r\nDrvCoin=%#02x,%#02x",CoinRdBuff[i],CoinRdBuff[i+1]);
				TraceCoin("\r\nDrvcoinRec<< [%02d]-",CoinRdLen);
				for(i=0;i<CoinRdLen;i++)
				{
					TraceCoin(" %#02x ",CoinRdBuff[i]);
				}
				TraceCoin("\r\n");
				
				if(
					(CoinRdBuff[i] <= 0x05)//����
					||((CoinRdBuff[i] == 0x11)&&(CoinRdBuff[i+1] == 0x10))
				)
				{
					TraceCoin("\r\nDrvCoin ok");
					MdbCoinErr.disable = 0;
					MdbCoinErr.unknowError = 0;
				}
				else if(CoinRdBuff[i] == 0x06)//��vmc����
				{
					TraceCoin("\r\nDrvCoin disable");
					MdbCoinErr.disable = 1;
					MdbCoinErr.unknowError = 0;
				}
				else if(CoinRdBuff[i] <= 0x15)//�������ֹ���
				{
					TraceCoin("\r\nDrvCoin unknow=%#02x",CoinRdBuff[i]);
					MdbCoinErr.unknowError = CoinRdBuff[i];
					MdbCoinErr.unknowErrorLow = CoinRdBuff[i+1];
				}
				//�����������
				else
				{
					TraceCoin("\r\nDrvCoin ok");
					MdbCoinErr.disable = 0;
					MdbCoinErr.unknowError = 0;
				}
			}
	    }
    } 	  
}




