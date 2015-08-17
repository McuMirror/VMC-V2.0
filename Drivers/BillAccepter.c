/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           BILLACCEPTER
** Last modified Date:  2013-03-04
** Last Version:         
** Descriptions:        ֽ��������������غ�������                     
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
#include "BillAccepter.h"


//��ǰֽ�����豸������
//uint8_t NowBillDev = 0;
//ֽ����ͨ����ֵ
uint32_t nBillvalue[16]={0};

//����ֽ����ͨ��Ӧ�����ݰ���ʱ����
//extern unsigned int MDBCONVERSATIONWAITACKTIMEOUT;
MDBBILLERROR MdbBillErr;

extern unsigned char BillRecyclerPayoutNumExpanse(unsigned int RecyPayoutMoney,unsigned char RecyPayoutNum);
extern unsigned char BillRecyclerPayoutValueExpanse(unsigned int RecyPayoutMoney,unsigned int *RecyPayoutMoneyBack);
extern void BillRecyclerTubeExpanse(void);


/*********************************************************************************************************
** Function name:       BillDevSellect
** Descriptions:       	ֽ�����豸ѡ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void BillDevSellect()
{
	unsigned char BillRdBuff[36],BillRdLen,ComStatus;
	unsigned char i;

	//NowBillDev = SystemPara.BillValidatorType;
	if(SystemPara.BillValidatorType == 0)
	{
		//NowBillDev = SystemPara.BillValidatorType;
	}
	else
	{
		/*//����Ƿ���GBAֽ����
		for(i=0;i<3;i++)
		{
		
			ComStatus = GbaControl_BillAccept();
			if(ComStatus != 2)
			{
				NowBillDev = BILLDEV_RS232;
				return;
			}
			
		}*/
		for(i=0;i<10;i++)
		{
			//Trace("1\r\n");
			//����Ƿ���MDBֽ����
			//Reset
			ComStatus = MdbConversation(0x30,NULL,0x00,&BillRdBuff[0],&BillRdLen);
			//Trace("1=%d\r\n",ComStatus);
			if(ComStatus == 1)
			{
				//Setup
				//ComStatus = MdbConversation(0x31,NULL,0x00,&BillRdBuff[0],&BillRdLen);
				//Trace("2=%d\r\n",ComStatus);		
				//if(ComStatus == 1)
				//{
					//Trace("%d,%d,%d,%d,%d,%d,%d,%d\r\n",BillRdBuff[11],BillRdBuff[12],BillRdBuff[13],BillRdBuff[14],BillRdBuff[15],BillRdBuff[16],BillRdBuff[17],BillRdBuff[18]);
				//NowBillDev = BILLDEV_MDB;
				OSTimeDly(OS_TICKS_PER_SEC/2);
				return;
				//}
			}
			OSTimeDly(OS_TICKS_PER_SEC);
		}	
	}
}


/*********************************************************************************************************
** Function name:       BillDevInit
** Descriptions:        ֽ�����豸��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      1ʹ��ʧ��,0ʹ�ܳɹ�
*********************************************************************************************************/
uint8_t BillDevInit()
{
	unsigned char BillRdBuff[36],BillRdLen,ComStatus,i;	
	unsigned char j;
	memset(BillRdBuff,0,sizeof(BillRdBuff));
	for(j = 0; j < 16; j++) 
	{
		 stDevValue.BillValue[j]=0;
		 //TraceBill("\r\nqqDrvChangebuf[%d] = %d,%d", j,ChangerRdBuff[j+2],stDevValue.CoinNum[j]);
	}
	//Reset
	for(ComStatus = 0, j = 0; j < 10 && ComStatus != 1; j++)
	{
		ComStatus =MdbConversation(0x30,NULL,0x00,&BillRdBuff[0],&BillRdLen);
		OSTimeDly(OS_TICKS_PER_SEC);
	}
	//OSTimeDly(OS_TICKS_PER_SEC*3);
	TraceBill("\r\n DrvBILLRESET=%d",ComStatus);
	LCDNumberFontPrintf(40,LINE15,2,"BillAccepter-1");	
	for(ComStatus = 0, j = 0; j < 10 && ComStatus != 1; j++)
	{
		ComStatus = MdbConversation(0x33,NULL,0x00,&BillRdBuff[0],&BillRdLen);
	}
	TraceBill("\r\n DrvBILLPOLL=%d",ComStatus);
	LCDNumberFontPrintf(40,LINE15,2,"BillAccepter-2");	
	//setup
	for(ComStatus = 0, j = 0; j < 10 && ComStatus != 1; j++)
	{
		ComStatus = MdbConversation(0x31,NULL,0x00,&BillRdBuff[0],&BillRdLen);
	}
	TraceBill("\r\n DrvBILLSETUP=%d",ComStatus);	
	LCDNumberFontPrintf(40,LINE15,2,"BillAccepter-3");	
	if(ComStatus == 1)
	{
		stDevValue.BillLevel = BillRdBuff[0];
		stDevValue.BillCode = ((((uint16_t)BillRdBuff[1]) << 8) | BillRdBuff[2]);
		stDevValue.BillDecimal = 100;
	    for(i = 0; i < BillRdBuff[5]; i++) 
	    {
		   stDevValue.BillDecimal /= 10;
	    }
		TraceBill("\r\nDrvBILLDec=%d,%d,%d",BillRdBuff[3],BillRdBuff[4],BillRdBuff[5]);
		stDevValue.BillScale = (uint32_t)((((uint16_t)BillRdBuff[3]) << 8) | BillRdBuff[4]) * stDevValue.BillDecimal;
		TraceBill("\r\nDrvBILLDec2=%ld,%ld,%ld", ((((uint16_t)BillRdBuff[3]) << 8) | BillRdBuff[4]),stDevValue.BillDecimal,stDevValue.BillScale);
		//stDevValue.Decimal = BillRdBuff[5];
		stDevValue.BillStkCapacity = ((((uint16_t)BillRdBuff[6]) << 8) | BillRdBuff[7]);
		stDevValue.BillSecurity = ((((uint16_t)BillRdBuff[8]) << 8) | BillRdBuff[9]);
		if(BillRdBuff[10] == 0) 
	    {
	  	  stDevValue.BillEscrowFun = 0;//���ݴ�
	    }
	    else 
	    {
	  	  stDevValue.BillEscrowFun = 1; //���ݴ�
	    }			
		for(i=0;i<BillRdLen-11;i++)
		{
			if(BillRdBuff[11+i] == 0xFF) 
			{
			     break;
			}
			
			nBillvalue[i] = (uint32_t)BillRdBuff[i+11] * stDevValue.BillScale;	
			stDevValue.BillValue[i] = nBillvalue[i];			
		}	
		for(i = 0; i < 16; i++) 
		{
			 TraceBill("\r\n%dDrvBill[%d] = %d", BillRdLen,i,stDevValue.BillValue[i]);
		}
	}
	
	//��ѯֽ����
	for(ComStatus = 0, j = 0; j < 10 && ComStatus != 1; j++)
	{
		ComStatus = MdbConversation(0x33,NULL,0x00,&BillRdBuff[0],&BillRdLen);
	}
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	//Identification
	ComStatus = MdbConversation(0x37,0x00,1,&BillRdBuff[0],&BillRdLen);
	for(i=0;i<BillRdLen;i++)
	{
		stDevValue.billIDENTITYBuf[i] = BillRdBuff[i];
	}
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	//Stacker
	//ComStatus = MdbConversation(0x36,NULL,0x00,&BillRdBuff[0],&BillRdLen);
	//OSTimeDly(OS_TICKS_PER_SEC / 100);
	//OSTimeDly(40);
	TraceBill("\r\nDrv4.1=%d,%ld,%ld,%ld,%d",stDevValue.BillLevel,stDevValue.BillScale,stDevValue.BillDecimal,stDevValue.BillStkCapacity,stDevValue.BillEscrowFun);
	TraceBill("\r\nDrv4.2=%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld",nBillvalue[0],nBillvalue[1],nBillvalue[2],nBillvalue[3],nBillvalue[4],nBillvalue[5],nBillvalue[6],nBillvalue[7]);
	TraceBill("\r\nIden=");
	for(i=0;i<BillRdLen;i++)
	{
		TraceBill(" [%02x]",stDevValue.billIDENTITYBuf[i]);
	}
	//OSTimeDly(OS_TICKS_PER_SEC*3);
	BillDevEnable();
	LCDNumberFontPrintf(40,LINE15,2,"BillAccepter-4");	
	ComStatus = MdbConversation(0x33,NULL,0x00,&BillRdBuff[0],&BillRdLen);
	return MdbBillErr.Communicate;
	/*
	else
	if(NowBillDev == BILLDEV_RS232)
	{
		for(i=0;i<8;i++)
			nBillvalue[i] = GetMemoryData(ADD_BILLVALUE1+i*4);
		BillDevEnable();	
	}*/
	//���ڴ��ж�ȡӲ����ͨ����ֵ
	//for(i=0;i<8;i++)
	//	nCoinvalue[i] = GetMemoryData(ADD_COINVALUE1+i*4);
}


/*********************************************************************************************************
** Function name:       BillDevDisable
** Descriptions:        ����ֽ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void BillDevDisable(void)
{
	uint8_t BillValue[4]={0},BillRdBuff[36],BillRdLen;
	uint8_t ComStatus;

	/*if(NowBillDev == BILLDEV_RS232)
	{
		GbaControl_BillDisable();
	}
	else*/
	
	BillValue[0] = 0x00;
	BillValue[1] = 0x00;
	BillValue[2] = 0xff;
	BillValue[3] = 0xff;
   	ComStatus = MdbConversation(0x34,BillValue,4,&BillRdBuff[0],&BillRdLen);
	if(ComStatus == 1)
		MdbBillErr.Communicate = 0;
	else
		MdbBillErr.Communicate = 1;
	
}


void BillDevReject(void)
{
	uint8_t BillValue[4]={0},BillRdBuff[36],BillRdLen;

	/*if(NowBillDev == BILLDEV_RS232)
	{
		GbaControl_BillReject();
	}
	else*/
	
	BillValue[0] = 0x00;
   	MdbConversation(0x35,BillValue,1,&BillRdBuff[0],&BillRdLen);	
		
}



/*********************************************************************************************************
** Function name:       BillDevEnable
** Descriptions:        ʹ��ֽ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void BillDevEnable(void)
{
	uint8_t BillWrBuff[4]={0},BillRdBuff[36],BillRdLen;
	uint8_t ComStatus;
	unsigned char j;
	
	/*if(NowBillDev == BILLDEV_RS232)
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
	//Stacker
	ComStatus = MdbConversation(0x36,NULL,0x00,&BillRdBuff[0],&BillRdLen);
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	
	if(nBillvalue[8]!=0)
		BillWrBuff[0] |= 0x01;
	if(nBillvalue[9]!=0)
		BillWrBuff[0] |= 0x02;
	if(nBillvalue[10]!=0)
		BillWrBuff[0] |= 0x04;
	if(nBillvalue[11]!=0)
		BillWrBuff[0] |= 0x08;
	if(nBillvalue[12]!=0)
		BillWrBuff[0] |= 0x10;
	if(nBillvalue[13]!=0)
		BillWrBuff[0] |= 0x20;
	if(nBillvalue[14]!=0)
		BillWrBuff[0] |= 0x40;
	if(nBillvalue[15]!=0)
		BillWrBuff[0] |= 0x80;

	
	if(nBillvalue[0]!=0)
		BillWrBuff[1] |= 0x01;
	if(nBillvalue[1]!=0)
		BillWrBuff[1] |= 0x02;
	if(nBillvalue[2]!=0)
		BillWrBuff[1] |= 0x04;
	if(nBillvalue[3]!=0)
		BillWrBuff[1] |= 0x08;
	if(nBillvalue[4]!=0)
		BillWrBuff[1] |= 0x10;
	if(nBillvalue[5]!=0)
		BillWrBuff[1] |= 0x20;
	if(nBillvalue[6]!=0)
		BillWrBuff[1] |= 0x40;
	if(nBillvalue[7]!=0)
		BillWrBuff[1] |= 0x80;
	BillWrBuff[2] = 0xFF;
	BillWrBuff[3] = 0xFF;
	TraceBill("\r\nDrvBillEnable>>%#02x,%#02x,%#02x,%#02x,%#02x\r\n",0x34,BillWrBuff[0],BillWrBuff[1],BillWrBuff[2],BillWrBuff[3]);
	//Enable
	for(ComStatus = 0, j = 0; j < 2 && ComStatus != 1; j++)
	{
		ComStatus = MdbConversation(0x34,BillWrBuff,4,&BillRdBuff[0],&BillRdLen);
		OSTimeDly(3);
	}
	TraceBill("\r\n DrvBILLENABLE=%d",ComStatus);
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	if(ComStatus == 1)
		MdbBillErr.Communicate = 0;
	else
		MdbBillErr.Communicate = 1;
	
}




/*********************************************************************************************************
** Function name:       BillDevProcess
** Descriptions:        ֽ�����ձ�ѹ������
** input parameters:    ��
** output parameters:   RecvMoney���������ֽ�ҽ��
						BillType���������ֽ��ͨ��
						billOpt--ֽ�������Ʋ���
						billOptBack--ֽ�������Ʒ��ؽ��
** Returned value:      ��ֽ�����뷵��1���޷���0
*********************************************************************************************************/
uint8_t BillDevProcess(uint32_t *RecvMoney,unsigned char *BillType,unsigned char billOpt,unsigned char *billOptBack,uint32_t RecyPayoutMoney,uint8_t RecyPayoutNum,unsigned int *RecyPayoutMoneyBack)
{
	unsigned char BillRdBuff[36],BillRdLen,ComStatus,BillWrBuff[1];
	uint8_t type=0,i=0;
	static uint8_t billrec=0;


	switch(billOpt)
	{
		case MBOX_BILLENABLEDEV:
			//Trace("enable bill\r\n");
			BillDevEnable();
			break;
		case MBOX_BILLDISABLEDEV:
			//Trace("disable bill\r\n");
			TraceBill("\r\n DrvBILLDISABLEDEV opt");
			BillDevDisable();
			break;			
		case MBOX_BILLESCROW:
			//Trace("\r\nescrow bill");
			if(BillDevEscrow())
				*billOptBack = 2;	
			else
				*billOptBack = 1;	
			break;	
		case MBOX_BILLRETURN:
			//Trace("\r\nescrow bill");
			if(BillDevReturn())
				*billOptBack = 4;	
			else
				*billOptBack = 3;	
			break;	
		case MBOX_BILLRECYPAYOUTNUM:
			//TraceBill("\r\n DrvRECPay=%ld,Num=%d",RecyPayoutMoney,RecyPayoutNum);
			if(BillRecyclerPayoutNumExpanse(RecyPayoutMoney,RecyPayoutNum))
				*billOptBack = 6;	
			else
				*billOptBack = 5;		
			break;
		case MBOX_BILLRECYPAYOUTVALUE:
			//TraceBill("\r\n DrvRECPay=%ld,Num=%d",RecyPayoutMoney,RecyPayoutNum);
			if(BillRecyclerPayoutValueExpanse(RecyPayoutMoney,RecyPayoutMoneyBack))
				*billOptBack = 6;	
			else
				*billOptBack = 5;		
			break;	
	}
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	//Trace("6\r\n");
	BillWrBuff[0] = 0x01;
	
	//��ѯֽ�����Ƿ����յ�ֽ�ң��з���1
	ComStatus = MdbConversation(0x33,NULL,0x00,&BillRdBuff[0],&BillRdLen);
	if(ComStatus == 1)
	{
		MdbBillErr.Communicate = 0;
		billrec=0;
		TraceBill("\r\nDrvBill= %02d-",BillRdLen);
		for(i=0;i<16;i++)
		{
			TraceBill(" %02x ",BillRdBuff[i]);
		}
		TraceBill("\r\n");
		OSTimeDly(OS_TICKS_PER_SEC / 100);
		if(BillRdLen==0)
		{
			TraceBill("\r\n Drvbill default");	
			OSTimeDly(OS_TICKS_PER_SEC / 100);
			memset(&MdbBillErr,0,sizeof(MdbBillErr));
		}
		for(i = 0; i < BillRdLen; i++) 
		{
			if((BillRdBuff[i]&0xf0)==0x90)
			{
				type = BillRdBuff[i]&0x0f;
				if(stDevValue.BillEscrowFun == 0)//��ֽ�������ݴ湦��
				{
					//����ѹ��ָ��
					MdbConversation(0x35,BillWrBuff,0x01,&BillRdBuff[0],&BillRdLen);
					Timer.EscrowTimer = 40;
					while(Timer.EscrowTimer)
					{
						//��ѯ����Ƿ�ѹ�ֳɹ�
						ComStatus = MdbConversation(0x33,NULL,0x00,&BillRdBuff[0],&BillRdLen);
						if(ComStatus == 1)
						{
							for(i = 0; i < BillRdLen; i++) 
							{
								if((BillRdBuff[i]&0xf0)==0x80)
								{
									*RecvMoney = nBillvalue[type];
									*BillType = type;						
									return 1;	
								}
							}
						}	
					}
				}
				else
				{
					*RecvMoney = nBillvalue[type];
					*BillType = type;
					TraceBill("Drvbill=%ld,%d\r\n",*RecvMoney,*BillType);
					OSTimeDly(OS_TICKS_PER_SEC / 100);
					return 1;
				}
			}
			else if((BillRdBuff[i] & 0xF0) == 0) 
			{   
				//validator status
			    switch(BillRdBuff[i]) 
				{
		            case 0x01:			                 //defective motor    
		            	TraceBill("\r\n Drvbill motor");
				        MdbBillErr.moto = 1;
				        break;
			
			        case 0x02:			                 //sensor problem
			        	TraceBill("\r\n Drvbill sensor");
					    MdbBillErr.sensor = 1;
				        break;
			
			        case 0x03:			                 //validator busy
			        	TraceBill("\r\n Drvbil busy");
			        	break;
			
			        case 0x04:			                 //rom chksum err
			        	TraceBill("\r\n Drvbill chksum");
				    	MdbBillErr.romchk = 1;
			        break;
			
			        case 0x05:			                 //validator jammed
			        	TraceBill("\r\n Drvbill jammed");
					    MdbBillErr.jam = 1;			       
				        break;
			
			        case 0x06:			                 //validator was reset
			        	TraceBill("\r\n Drvbil reset");
						break;
				 
			        case 0x07:			                 //bill removed	
			        	TraceBill("\r\n Drvbil removed");
			        	break;
			 
			        case 0x08:			                 //cash box out of position
			        	TraceBill("\r\n Drvbill removeCash");
					    MdbBillErr.removeCash = 1;	
				        break;
			
			        case 0x09:			                 //validator disabled	
			        	TraceBill("\r\n Drvbill disabled");
			        	MdbBillErr.disable = 1;
						break;
			
			        case 0x0A:			                 //invalid escrow request
			        	TraceBill("\r\n Drvbil invalid");
			       		break;
			
			        case 0x0B:			                 //bill rejected
			        	TraceBill("\r\n Drvbil rejected");
			        	break;
			
			        case 0x0C:			                 //possible credited bill removal
			        	TraceBill("\r\n Drvbill cashErr");
					    MdbBillErr.cashErr = 1;	
				        break;
			
			        default:
						TraceBill("\r\n Drvbill default");	
						memset(&MdbBillErr,0,sizeof(MdbBillErr));
					    break;
		         }
		    }
		}
	}
	else
	{
		billrec++;
		TraceBill("\r\n Drvbill commuFail=%d,billrec=%d",ComStatus,billrec);
		if(billrec>=30)
		{
			TraceBill("\r\n Drvbill commReject");
			BillDevReject();
			billrec=0;
		}
		MdbBillErr.Communicate = 1;
	}
	
	/*
	else
	if(NowBillDev == BILLDEV_RS232)
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
** Function name:       BillDevEscrow
** Descriptions:        �ݴ�ֽ��ѹ������
** input parameters:    ��
** output parameters:   
** Returned value:      ѹ���ɹ�����1��ʧ�ܷ���0
*********************************************************************************************************/
uint8_t BillDevEscrow()
{
	unsigned char BillRdBuff[36],BillRdLen,ComStatus,BillWrBuff[1],j;
	uint8_t i=0;
	BillWrBuff[0] = 0x01;

	//����ѹ��ָ��
	for(ComStatus = 0, j = 0; j < 20 && ComStatus != 1; j++)
	{
		ComStatus =MdbConversation(0x35,BillWrBuff,0x01,&BillRdBuff[0],&BillRdLen);
	}
	TraceBill("\r\n DrvEscrowsend=%d",ComStatus);
	TraceBill("\r\nDrvEscrowsend>>%#02x,%#02x\r\n",0x35,BillWrBuff[0]);
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	Timer.EscrowTimer = 13;
	while(Timer.EscrowTimer)
	{
		//��ѯ����Ƿ�ѹ�ֳɹ�
		ComStatus = MdbConversation(0x33,NULL,0x00,&BillRdBuff[0],&BillRdLen);
		TraceBill("\r\nDrvEscrowsend>>%#02x\r\n",0x33);
		if(ComStatus == 1)
		{
			TraceBill("\r\nDrvBill= %02d-",BillRdLen);
			for(i=0;i<16;i++)
			{
				TraceBill(" %02x ",BillRdBuff[i]);
			}
			TraceBill("\r\n");
			for(i = 0; i < BillRdLen; i++) 
			{
				//0x80������,0xb0��ѭ����
				if( ((BillRdBuff[i]&0xf0)==0x80)||((BillRdBuff[i]&0xf0)==0xb0) )
				{			
					TraceBill("\r\n Drvescrow1");
					OSTimeDly(OS_TICKS_PER_SEC / 100);
					if((BillRdBuff[i]&0xf0)==0xb0)
					{
						BillRecyclerTubeExpanse();
					}
					return 1;	
				}
				//û��ѹ���ɹ�����;�˳�
				else if((BillRdBuff[i]&0xf0)==0xa0)
				{
					TraceBill("\r\n DrvescrowReturn");
					return 0;
				}
			}			
			TraceBill("\r\n Drvescrow2");
			OSTimeDly(OS_TICKS_PER_SEC / 100);
		}
		TraceBill("\r\n Drvescrow3");
		OSTimeDly(7);
	}	
	return 0;
}

/*********************************************************************************************************
** Function name:       BillDevReturn
** Descriptions:        �ݴ�ֽ���˱Ҳ���
** input parameters:    ��
** output parameters:   
** Returned value:      �˱ҳɹ�����1��ʧ�ܷ���0
*********************************************************************************************************/
uint8_t BillDevReturn()
{
	unsigned char BillRdBuff[36],BillRdLen,ComStatus,BillWrBuff[1],j;
	BillWrBuff[0] = 0x00;

	//�����˱�ָ��
	for(ComStatus = 0, j = 0; j < 20 && ComStatus != 1; j++)
	{
		ComStatus =MdbConversation(0x35,BillWrBuff,0x01,&BillRdBuff[0],&BillRdLen);
	}
	TraceBill("\r\n DrvReturnsend=%d",ComStatus);
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	Timer.EscrowTimer = 10;
	while(Timer.EscrowTimer)
	{
		//��ѯ����Ƿ��˱ҳɹ�
		ComStatus = MdbConversation(0x33,NULL,0x00,&BillRdBuff[0],&BillRdLen);
		if(ComStatus == 1)
		{
			//if((BillRdBuff[0]&0xf0)==0x80)
			{			
				TraceBill("\r\n Drvreturn1");
				OSTimeDly(OS_TICKS_PER_SEC / 100);
				return 1;	
			}
			//TraceBill("\r\n escrow2");
		}
		TraceBill("\r\n Drvsend3");
		OSTimeDly(7);
	}	
	return 0;
}


//==============================
//====ֽ��ѭ��������
//==============================

/*********************************************************************************************************
** Function name:       BillRecyclerIdExpanse
** Descriptions:        �õ�ֽ�����Ƿ�֧��ѭ���������Ϣ,��z30-z33�ֽ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void BillRecyclerIdExpanse(void)
{
    uint8_t BillRdBuff[36],BillRdLen,ComStatus,VMCdata[1]={0x02},i;
    //uint8_t coinscale,dispenseValue;

	ComStatus = MdbConversation(0x37,VMCdata,1,&BillRdBuff[0],&BillRdLen);
	TraceBill("\r\n 6DrvRecyclerIDLevel2>>%#02x,%#02x",0x37,VMCdata[0]);
	if(ComStatus == 1)	
      {  	
	    	TraceBill("\r\nDrvRecyclerRec<< [%02d]-",BillRdLen);
		for(i=0;i<BillRdLen;i++)
		{
			TraceBill(" %#02x ",BillRdBuff[i]);
		}
		TraceBill("\r\n");	    
     } 	  
}


/*********************************************************************************************************
** Function name:       BillRecyclerFeatureEnableExpanse
** Descriptions:        ʹ����չָ�������ֽ�������ܽ�����չָ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void BillRecyclerFeatureEnableExpanse(void)
{
    uint8_t BillRdBuff[36],BillRdLen,ComStatus,VMCdata[5]={0x01,0x00,0x00,0x00,0x02};
    //uint8_t coinscale,dispenseValue;

	ComStatus = MdbConversation(0x37,VMCdata,5,&BillRdBuff[0],&BillRdLen);
	TraceBill("\r\n 7DrvRecyclerFeatureEnable>>%#02x,%#02x,%#02x,%#02x,%#02x,%#02x",0x37,VMCdata[0],VMCdata[1],VMCdata[2],VMCdata[3],VMCdata[4]);
	if(ComStatus == 1)	
    {  	
    		TraceBill("\r\nDrvRecEnable-7");    
    } 	  
}

/*********************************************************************************************************
** Function name:       BillRecyclerSetupExpanse
** Descriptions:        ѭ��ֽ����setupָ��,�õ���Щֽ��ֵ���Էŵ�ѭ������
** input parameters:    ��
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void BillRecyclerSetupExpanse(void)
{
    uint8_t BillRdBuff[36],BillRdLen,ComStatus,VMCdata[1]={0x03},i;
    
	
    ComStatus = MdbConversation(0x37,VMCdata,1,&BillRdBuff[0],&BillRdLen);
	TraceBill("\r\n 11DrvRecyclerSetup>>%#02x,%#02x",0x37,VMCdata[0]);
	
	if(ComStatus == 1)	
    {  	
    	TraceBill("\r\nDrvRecyclerRec<< [%02d]-",BillRdLen);
		for(i=0;i<BillRdLen;i++)
		{
			TraceBill(" %#02x ",BillRdBuff[i]);
		}
		TraceBill("\r\n");	
		for(i=0;i<7;i++)
		{
			if((BillRdBuff[1]>>i)&0x01)
				stDevValue.RecyclerValue[i]=stDevValue.BillValue[i];
		}
		TraceBill("\r\nDrvRecValue=%ld,%ld,%ld,%ld,%ld,%ld,%ld\r\n",stDevValue.RecyclerValue[0],stDevValue.RecyclerValue[1],stDevValue.RecyclerValue[2],stDevValue.RecyclerValue[3],stDevValue.RecyclerValue[4],stDevValue.RecyclerValue[5],stDevValue.RecyclerValue[6]);
    } 	  
}



/*********************************************************************************************************
** Function name:       BillRecyclerEnableExpanse
** Descriptions:        ʹ���Ƿ����ѭ����,��3�ֽ�-18�ֽڣ�����ͨ��0-15�Ƿ����ѭ����
**                      ȫ������ѭ����VMCdata[19]=  {0x04,0xff,0xff,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03};
**                      ȫ��������ѭ����VMCdata[19]={0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
** input parameters:    enable=1��ѭ��������,0�ر�ѭ��������
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void BillRecyclerEnableExpanse(uint8_t enable)
{
    uint8_t BillRdBuff[36],BillRdLen,ComStatus,i;
	//uint8_t VMCdata[19]={0x04,0xff,0xff,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03};
    uint8_t VMCdata[19]={0x04,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	
	//����ѭ����
	if(enable==1)
	{
		for(i=0;i<7;i++)
		{
			if(stDevValue.RecyclerValue[i]>0)
			{
				VMCdata[3+i]=0x03;
			}
		}		
	}
	//�ر�ѭ����
	else if(enable==0)
	{		
		for(i=0;i<7;i++)
		{
			VMCdata[3+i]=0x00;				
		}				
	}	
	
	TraceBill("\r\n12DrvRecEnable>> [%02d]- %#02x,",20,0x37);
	for(i=0;i<19;i++)
	{
		TraceBill(" %#02x ",VMCdata[i]);
	}
	TraceBill("\r\n");	
	
    ComStatus = MdbConversation(0x37,VMCdata,19,&BillRdBuff[0],&BillRdLen);
	if(ComStatus == 1)	
    {  	 
    	TraceBill("\r\nDrvRecEnable-9");
    }
	 	
}


/*********************************************************************************************************
** Function name:       BillRecyclerTubeExpanse
** Descriptions:        ֽ��������z1-z2�����ֵ�Ƿ��Ѿ�����,z3-z34ÿ����ֵĿǰ�洢������,2���ֽڱ�ʾһ����ֵ
** input parameters:    ��
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void BillRecyclerTubeExpanse(void)
{
    uint8_t BillRdBuff[36],BillRdLen,ComStatus,VMCdata[1]={0x05},i,j;
	static uint8_t RecAble=1;//1�Ѿ���,0�Ѿ��ر�
    
	
    ComStatus = MdbConversation(0x37,VMCdata,1,&BillRdBuff[0],&BillRdLen);
	TraceBill("\r\n 13DrvRecyclerTube>>%#02x,%#02x",0x37,VMCdata[0]);
	if(ComStatus == 1)	
    {  	
    	TraceBill("\r\nDrvRecyclerRec<< [%02d]-",BillRdLen);
		for(i=0;i<BillRdLen;i++)
		{
			TraceBill(" %#02x ",BillRdBuff[i]);
		}
		TraceBill("\r\n");
		OSTimeDly(OS_TICKS_PER_SEC / 100);
		//������κ�һ����ֵ�洢�����͹ر�ѭ�������ܡ������ѭ��������
		if(INTEG16(BillRdBuff[0],BillRdBuff[1]))
		{
			if(RecAble==1)
			{
				BillRecyclerEnableExpanse(0);
				RecAble=0;
			}
		}
		else
		{
			if(RecAble==0)
			{			
				BillRecyclerEnableExpanse(1);
				RecAble=1;
			}
		}
		//����ÿ����ֵĿǰ�洢������
	    for( i=0,j=2; i<7; i++, j+=2 )
		{
			stDevValue.RecyclerNum[i]  = INTEG16(BillRdBuff[j],BillRdBuff[j+1]);
		}
		TraceBill("\r\nDrvRecNum=%ld,%ld,%ld,%ld,%ld,%ld,%ld\r\n",stDevValue.RecyclerNum[0],stDevValue.RecyclerNum[1],stDevValue.RecyclerNum[2],stDevValue.RecyclerNum[3],stDevValue.RecyclerNum[4],stDevValue.RecyclerNum[5],stDevValue.RecyclerNum[6]);
    } 	  
}




/*********************************************************************************************************
** Function name:       BillDevInit
** Descriptions:        ֽ�����豸��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      1ʹ��ʧ��,0ʹ�ܳɹ�
*********************************************************************************************************/
uint8_t RecyclerDevInit()
{
	unsigned char BillRdBuff[36],BillRdLen,ComStatus,i;	
	unsigned char j;
	uint8_t VMCdata[2]={0x00,0x00};
	memset(BillRdBuff,0,sizeof(BillRdBuff));	
	for(j = 0; j < 16; j++) 
	{
		 stDevValue.BillValue[j]=0;
		 //TraceBill("\r\nqqDrvChangebuf[%d] = %d,%d", j,ChangerRdBuff[j+2],stDevValue.CoinNum[j]);
	}
	//1Reset
	for(ComStatus = 0, j = 0; j < 10 && ComStatus != 1; j++)
	{
		ComStatus =MdbConversation(0x30,NULL,0x00,&BillRdBuff[0],&BillRdLen);
		OSTimeDly(OS_TICKS_PER_SEC/100);
	}
	//OSTimeDly(OS_TICKS_PER_SEC*3);
	TraceBill("\r\n 1DrvRecyclerRESET>>%#02x",0x30);	
	LCDNumberFontPrintf(40,LINE15,2,"Recycler-1");	
	//2Poll
	for(ComStatus = 0, j = 0; j < 10 && ComStatus != 1; j++)
	{
		ComStatus = MdbConversation(0x33,NULL,0x00,&BillRdBuff[0],&BillRdLen);
	}
	TraceBill("\r\n 2DrvRecyclerPOLL>>%#02x",0x33);
	TraceBill("\r\nDrvRecyclerRec<< [%02d]-",BillRdLen);
	for(i=0;i<BillRdLen;i++)
	{
		TraceBill(" %#02x ",BillRdBuff[i]);
	}
	TraceBill("\r\n");
	LCDNumberFontPrintf(40,LINE15,2,"Recycler-2");
	
	//3setup
	for(ComStatus = 0, j = 0; j < 10 && ComStatus != 1; j++)
	{
		ComStatus = MdbConversation(0x31,NULL,0x00,&BillRdBuff[0],&BillRdLen);
	}
	TraceBill("\r\n 3DrvRecyclerSETUP>>%#02x",0x31);	
	TraceBill("\r\nDrvRecyclerRec<< [%02d]-",BillRdLen);
	for(i=0;i<BillRdLen;i++)
	{
		TraceBill(" %#02x ",BillRdBuff[i]);
	}
	TraceBill("\r\n");
	if(ComStatus == 1)
	{
		stDevValue.BillLevel = BillRdBuff[0];
		stDevValue.BillCode = ((((uint16_t)BillRdBuff[1]) << 8) | BillRdBuff[2]);
		stDevValue.BillDecimal = 100;
	    for(i = 0; i < BillRdBuff[5]; i++) 
	    {
		   stDevValue.BillDecimal /= 10;
	    }
		stDevValue.BillScale = ((((uint16_t)BillRdBuff[3]) << 8) | BillRdBuff[4]) * stDevValue.BillDecimal;
		//stDevValue.Decimal = BillRdBuff[5];
		stDevValue.BillStkCapacity = ((((uint16_t)BillRdBuff[6]) << 8) | BillRdBuff[7]);
		stDevValue.BillSecurity = ((((uint16_t)BillRdBuff[8]) << 8) | BillRdBuff[9]);
		TraceBill("\r\n DrvRecyclerSecu=%#02x,%#02x,%ld",BillRdBuff[8],BillRdBuff[9],stDevValue.BillSecurity);
		VMCdata[0]=BillRdBuff[8];
		VMCdata[1]=BillRdBuff[9];
		if(BillRdBuff[10] == 0) 
	    {
	  	  stDevValue.BillEscrowFun = 0;//���ݴ�
	    }
	    else 
	    {
	  	  stDevValue.BillEscrowFun = 1; //���ݴ�
	    }			
		for(i=0;i<BillRdLen-11;i++)
		{
			if(BillRdBuff[11+i] == 0xFF) 
			{
			     break;
			}
			
			nBillvalue[i] = (uint32_t)BillRdBuff[i+11] * stDevValue.BillScale;	
			stDevValue.BillValue[i] = nBillvalue[i];			
		}
		for(i = 0; i < 16; i++) 
		{
			 TraceBill("\r\n%dDrvRec[%d] = %d", BillRdLen,i,stDevValue.BillValue[i]);
		}
	}
	
	//4security
	for(ComStatus = 0, j = 0; j < 10 && ComStatus != 1; j++)
	{
		ComStatus = MdbConversation(0x32,VMCdata,2,&BillRdBuff[0],&BillRdLen);
	}
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	TraceBill("\r\n 4DrvRecyclerSecurity>>%#02x,%#02x,%#02x",0x32,VMCdata[0],VMCdata[1]);
	LCDNumberFontPrintf(40,LINE15,2,"Recycler-3");	
	
	//5IDLevel1
	ComStatus = MdbConversation(0x37,0x00,1,&BillRdBuff[0],&BillRdLen);
	for(i=0;i<BillRdLen;i++)
	{
		stDevValue.billIDENTITYBuf[i] = BillRdBuff[i];
	}	
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	TraceBill("\r\n 5DrvRecyclerIDLevel1>>%#02x,%#02x",0x37,0x00);
	TraceBill("\r\nDrvRecyclerRec<< [%02d]-",BillRdLen);
	for(i=0;i<BillRdLen;i++)
	{
		TraceBill(" %#02x ",BillRdBuff[i]);
	}
	TraceBill("\r\n");
	//6IDLevel2
	BillRecyclerIdExpanse();
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	//7FeatureEnable
	BillRecyclerFeatureEnableExpanse();
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	//8Stacker
	ComStatus = MdbConversation(0x36,NULL,0x00,&BillRdBuff[0],&BillRdLen);
	TraceBill("\r\n 8DrvRecyclerStacker>>%#02x",0x36);
	TraceBill("\r\nDrvRecyclerRec<< [%02d]-",BillRdLen);
	for(i=0;i<BillRdLen;i++)
	{
		TraceBill(" %#02x ",BillRdBuff[i]);
	}
	TraceBill("\r\n");
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	//OSTimeDly(40);
	TraceBill("\r\nDrv4.1=%d,%ld,%ld,%ld,%d",stDevValue.BillLevel,stDevValue.BillScale,stDevValue.BillDecimal,stDevValue.BillStkCapacity,stDevValue.BillEscrowFun);
	TraceBill("\r\nDrv4.2=%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld",nBillvalue[0],nBillvalue[1],nBillvalue[2],nBillvalue[3],nBillvalue[4],nBillvalue[5],nBillvalue[6],nBillvalue[7]);
	//OSTimeDly(OS_TICKS_PER_SEC*3);
	//9Enable
	BillDevEnable();
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	LCDNumberFontPrintf(40,LINE15,2,"RecyclerAccepter-4");
	//10Poll
	ComStatus = MdbConversation(0x33,NULL,0x00,&BillRdBuff[0],&BillRdLen);
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	TraceBill("\r\n 10DrvRecyclerPOLL>>%#02x",0x33);
	TraceBill("\r\nDrvRecyclerRec<< [%02d]-",BillRdLen);
	for(i=0;i<BillRdLen;i++)
	{
		TraceBill(" %#02x ",BillRdBuff[i]);
	}
	TraceBill("\r\n");
	for(j = 0; j < 130; j++)
	{
		//12Poll
		ComStatus = MdbConversation(0x33,NULL,0x00,&BillRdBuff[0],&BillRdLen);
		OSTimeDly(OS_TICKS_PER_SEC / 10);
		TraceBill("\r\n 10DrvRecyclerPOLL>>%#02x",0x33);
		TraceBill("\r\nDrvRecyclerRec<< [%02d]-",BillRdLen);
		for(i=0;i<BillRdLen;i++)
		{
			TraceBill(" %#02x ",BillRdBuff[i]);
		}
		TraceBill("\r\n");
	}
	
	//11RecyclerSetup
	BillRecyclerSetupExpanse();
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	//12RecyclerEnable
	BillRecyclerEnableExpanse(1);
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	LCDNumberFontPrintf(40,LINE15,2,"RecyclerAccepter-5");
	//13RecyclerTube
	BillRecyclerTubeExpanse();
	OSTimeDly(OS_TICKS_PER_SEC / 100);
	return MdbBillErr.Communicate;	
}


/*********************************************************************************************************
** Function name:       BillRecyclerPayoutNumExpanse
** Descriptions:        �������ұ�,0-15����ֵ��0x00-0x0f����ʾ,0x00,0x01��ʾ��������
                        ����10Ԫ��ֵ��һ��VMCdata[4]={0x06,0x02,0x00,0x01}
** input parameters:    PayoutType��Ҫ�ҵ���ֵ,PayoutNum��Ҫ�ҵ�����
** output parameters:   ��
** Returned value:      1����ɹ�,0����ʧ��
*********************************************************************************************************/
unsigned char BillRecyclerPayoutNumExpanse(unsigned int RecyPayoutMoney,unsigned char RecyPayoutNum)
{
    uint8_t BillRdBuff[36],BillRdLen,ComStatus,VMCdata[4]={0x06,0x00,0x00,0x00},VMCPoll[1]={0x09},i,payout=1;
	uint8_t payStatus=0;

	for(i=0;i<7;i++)
	{
		if(stDevValue.RecyclerValue[i]==RecyPayoutMoney)
		{
			VMCdata[1]=i;
		}
	}	
	VMCdata[3]=RecyPayoutNum;
	TraceBill("\r\nDrvRecPay= %02d,Num=%02d",VMCdata[1],VMCdata[3]);
	//1�����ұ�����
    ComStatus = MdbConversation(0x37,VMCdata,4,&BillRdBuff[0],&BillRdLen);
	if(ComStatus == 1)	
    {  	
    	TraceBill("\r\nDrvBillRecycler8= %02d-",BillRdLen);
		for(i=0;i<BillRdLen;i++)
		{
			TraceBill(" %#02x ",BillRdBuff[i]);
		}
		TraceBill("\r\n");
		OSTimeDly(OS_TICKS_PER_SEC / 100);
		while(payout)
		{
			//2����pollָ��
			ComStatus = MdbConversation(0x33,NULL,0x00,&BillRdBuff[0],&BillRdLen);
			TraceBill("\r\nDrvBillPoll= %02d-",BillRdLen);
			for(i=0;i<BillRdLen;i++)
			{
				TraceBill(" %#02x ",BillRdBuff[i]);
			}
			TraceBill("\r\n");
			OSTimeDly(7);
			
			//3������չpayout value pollָ�����ұ����
			ComStatus = MdbConversation(0x37,VMCPoll,1,&BillRdBuff[0],&BillRdLen);
			TraceBill("\r\nDrvRecVPoll= %02d-",BillRdLen);
			for(i=0;i<BillRdLen;i++)
			{
				TraceBill(" %#02x ",BillRdBuff[i]);
			}
			TraceBill("\r\n");
			if(BillRdLen==0)
			{
				VMCPoll[0]=0x08;
				//4������չpayout statusָ�����ұ�����
				ComStatus = MdbConversation(0x37,VMCPoll,1,&BillRdBuff[0],&BillRdLen);
				TraceBill("\r\nDrvRecStatus= %02d-",BillRdLen);
				for(i=0;i<BillRdLen;i++)
				{
					TraceBill(" %#02x ",BillRdBuff[i]);
				}
				TraceBill("\r\n");
				OSTimeDly(7);
				//5����ָ����±Ҷ�ʣ��ֽ������
				BillRecyclerTubeExpanse();
				payout=0;
				payStatus=1;
			}
			OSTimeDly(7);
			
		}
    } 
	return payStatus;
}


/*********************************************************************************************************
** Function name:       BillRecyclerPayoutValueExpanse
** Descriptions:        ���Ž���ұ�                        
** input parameters:    RecyPayoutMoney��Ҫ�ҵĽ��,�Է�Ϊ��λ
** output parameters:   ��
** Returned value:      1����ɹ�,0����ʧ��
*********************************************************************************************************/
unsigned char BillRecyclerPayoutValueExpanse(unsigned int RecyPayoutMoney,unsigned int *RecyPayoutMoneyBack)
{
	uint8_t BillRdBuff[36],BillRdLen,ComStatus,VMCdata[3]={0x07,0x00,0x00},VMCPoll[1]={0x09},i,payout=1;
	uint8_t billscale,dispenseValue;	
	uint8_t payStatus=0;
	uint32_t RecyclerValue=0;

	*RecyPayoutMoneyBack=0;
	billscale = stDevValue.BillScale;
	dispenseValue = RecyPayoutMoney / billscale;//���������׼���� 
	VMCdata[1]=HUINT16(dispenseValue);
	VMCdata[2]=LUINT16(dispenseValue);
	TraceBill("\r\nDrvBillRecyclerMoney= %#02x-%#02x",VMCdata[1],VMCdata[2]);
	ComStatus = MdbConversation(0x37,VMCdata,3,&BillRdBuff[0],&BillRdLen);
	if(ComStatus == 1)	
	{	
		TraceBill("\r\nDrvBillRecycler8= %02d-",BillRdLen);
		for(i=0;i<BillRdLen;i++)
		{
			TraceBill(" %#02x ",BillRdBuff[i]);
		}
		TraceBill("\r\n");
		OSTimeDly(OS_TICKS_PER_SEC / 100);
		while(payout)
		{
			//2����pollָ��
			ComStatus = MdbConversation(0x33,NULL,0x00,&BillRdBuff[0],&BillRdLen);
			TraceBill("\r\nDrvBillPoll= %02d-",BillRdLen);
			for(i=0;i<BillRdLen;i++)
			{
				TraceBill(" %#02x ",BillRdBuff[i]);
			}
			TraceBill("\r\n");
			OSTimeDly(7);
			
			//3������չpayout value pollָ�����ұ����
			ComStatus = MdbConversation(0x37,VMCPoll,1,&BillRdBuff[0],&BillRdLen);
			TraceBill("\r\nDrvRecVPoll= %02d-",BillRdLen);
			for(i=0;i<BillRdLen;i++)
			{
				TraceBill(" %#02x ",BillRdBuff[i]);
			}
			TraceBill("\r\n");
			if(BillRdLen==0)
			{
				VMCPoll[0]=0x08;
				//4������չpayout statusָ�����ұ�����
				ComStatus = MdbConversation(0x37,VMCPoll,1,&BillRdBuff[0],&BillRdLen);
				TraceBill("\r\nDrvRecStatus= %02d-",BillRdLen);
				for(i=0;i<BillRdLen;i++)
				{
					TraceBill(" %#02x ",BillRdBuff[i]);
				}
				TraceBill("\r\n");
				//5�õ��������Ǯ
				for(i=0;i<7;i++)
				{
					if(stDevValue.RecyclerValue[i])
					{
						RecyclerValue=stDevValue.RecyclerValue[i];//�õ������ݴ浽ѭ��������ֵ
						break;
					}
				}
				for(i=0;i<BillRdLen;i++)
				{
					if(BillRdBuff[i]>0)
					{
						*RecyPayoutMoneyBack=RecyclerValue*BillRdBuff[i];
						break;
					}
				}
				OSTimeDly(7);
				//5����ָ����±Ҷ�ʣ��ֽ������
				BillRecyclerTubeExpanse();
				payout=0;
				payStatus=1;
			}
			OSTimeDly(7);
			
		}
	}	
	return payStatus;
}



