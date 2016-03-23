/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           BILLACCEPTER
** Last modified Date:  2013-03-04
** Last Version:         
** Descriptions:        ֽ�����м������غ�������                     
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
** Function name:       BillDevInit
** Descriptions:        ֽ�����豸��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void BillDevInitAPI()
{
	switch(SystemPara.BillValidatorType)
	{		
		case MDB_BILLACCEPTER:
			//ֽ��ѭ��������
			if(SystemPara.BillRecyclerType==MDB_BILLRECYCLER)
			{
				TraceBill("\r\n Mddpost recyclerinit");
				//1.����ֽ��ѭ��������
				MsgAccepterPack.BillBack = MBOX_BILLRECYCLERDEV;
			}
			//ֽ����
			else
			{
				TraceBill("\r\n Mddpost billinit");
				//1.����ֽ����
				MsgAccepterPack.BillBack = MBOX_BILLINITDEV;
			}
			OSMboxPost(g_BillMoneyMail,&MsgAccepterPack);
			
			break;	
		default:break;	
	}
	return;
}

/*********************************************************************************************************
** Function name:       GetBillDevMoneyInAPI
** Descriptions:        ���ֽ�����豸���ձ�״̬(�յ����Զ�ѹ��)
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint32_t GetBillDevMoneyInAPI(void)
{
	unsigned char ComStatus;
	MessagePack *BillMsg;
	uint32_t money,ReturnBack;
	uint8_t err,i;

	switch(SystemPara.BillValidatorType)
	{		
		case MDB_BILLACCEPTER:
			BillMsg = OSQPend(g_BillMoneyQ,10,&ComStatus);
			//Trace("mdb=%d\r\n",ComStatus);
			if(ComStatus == OS_NO_ERR)
			{
				money = BillMsg->BillMoney;				
				//ReturnBack = money;
				
				//ѹ��ֽ����
				MsgAccepterPack.BillBack = MBOX_BILLESCROW;			
				OSMboxPost(g_BillMoneyMail,&MsgAccepterPack);
				for(i=0;i<10;i++)
				{
					//Trace("\r\n 1.");
					//3.�ȴ��Ƿ�ѹ���ɹ�
					BillMsg = OSMboxPend(g_BillMoneyBackMail,OS_TICKS_PER_SEC*4,&err);//�����Ƿ�ѹ���ɹ�			
					if(err == OS_NO_ERR) 
					{					
						//4.ѹ���ɹ�������ѹ���ɹ���ֽ����
						if(BillMsg->BillBackCmd == MBOX_BILLESCROWSUCC)
						{
							ReturnBack = money;
						}
						//5.ѹ��ʧ�ܣ�����ѹ��ʧ�ܸ�ֽ����
						else if(BillMsg->BillBackCmd == MBOX_BILLESCROWFAIL)
						{
							ReturnBack = 0;
						}
						//Trace("\r\n 2.=%d",ReturnBack);
						break;
					}
					OSTimeDly(7);
				}
				if(i >= 10)
				{
					ReturnBack = 0;
				}
			}
			else
			{
				ReturnBack = 0;
			}
			break;	
		default:break;	
	}
	return ReturnBack;
	
}

/*********************************************************************************************************
** Function name:       GetBillDevMoneyUnStackInAPI
** Descriptions:        ���ֽ�����豸���ձ�״̬(�յ����Զ�ѹ��)
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint32_t GetBillDevMoneyUnStackInAPI(void)
{
	unsigned char ComStatus;
	MessagePack *BillMsg;
	uint32_t money,ReturnBack;
	

	switch(SystemPara.BillValidatorType)
	{		
		case MDB_BILLACCEPTER:
			BillMsg = OSQPend(g_BillMoneyQ,10,&ComStatus);
			//Trace("mdb=%d\r\n",ComStatus);
			if(ComStatus == OS_NO_ERR)
			{
				money = BillMsg->BillMoney;				
				ReturnBack = money;								
			}
			else
			{
				ReturnBack = 0;
			}
			break;
		default:break;	
	}
	return ReturnBack;
	
}

/*********************************************************************************************************
** Function name:       StackedBillDevMoneyInAPI
** Descriptions:        ѹ�������ֽ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      �ɹ�-1,ʧ��-0
*********************************************************************************************************/
uint8_t StackedBillDevMoneyInAPI(void)
{
	MessagePack *BillMsg;
	uint32_t ReturnBack;
	uint8_t err,i;

	switch(SystemPara.BillValidatorType)
	{		
		case MDB_BILLACCEPTER:
			if(stDevValue.BillEscrowFun == 0)//��ֽ�������ݴ湦��
			{
				ReturnBack = 1;
			}
			//��ֽ�������ݴ湦��
			else
			{
				//ѹ��ֽ����
				MsgAccepterPack.BillBack = MBOX_BILLESCROW;			
				OSMboxPost(g_BillMoneyMail,&MsgAccepterPack);
				for(i=0;i<10;i++)
				{
					//Trace("\r\n 1.");
					//3.�ȴ��Ƿ�ѹ���ɹ�
					BillMsg = OSMboxPend(g_BillMoneyBackMail,OS_TICKS_PER_SEC*4,&err);//�����Ƿ�ѹ���ɹ�			
					if(err == OS_NO_ERR) 
					{					
						//4.ѹ���ɹ�������ѹ���ɹ���ֽ����
						if(BillMsg->BillBackCmd == MBOX_BILLESCROWSUCC)
						{
							ReturnBack = 1;
						}
						//5.ѹ��ʧ�ܣ�����ѹ��ʧ�ܸ�ֽ����
						else if(BillMsg->BillBackCmd == MBOX_BILLESCROWFAIL)
						{
							ReturnBack = 0;
						}
						//Trace("\r\n 2.=%d",ReturnBack);
						break;
					}
					OSTimeDly(7);
				}
				if(i >= 10)
				{
					ReturnBack = 0;
				}	
			}
			break;	
		default:break;	
	}
	return ReturnBack;
	
}

/*********************************************************************************************************
** Function name:       ReturnBillDevMoneyInAPI
** Descriptions:        �˳��ݴ��ֽ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint8_t ReturnBillDevMoneyInAPI(void)
{
	MessagePack *BillMsg;
	uint32_t ReturnBack;
	uint8_t err,i;

	switch(SystemPara.BillValidatorType)
	{		
		case MDB_BILLACCEPTER:
			if(stDevValue.BillEscrowFun == 0)//��ֽ�������ݴ湦��
			{
				ReturnBack = 1;
			}
			//��ֽ�������ݴ湦��
			else
			{
				//�˱�ֽ����
				MsgAccepterPack.BillBack = MBOX_BILLRETURN;			
				OSMboxPost(g_BillMoneyMail,&MsgAccepterPack);
				for(i=0;i<10;i++)
				{
					//Trace("\r\n 1.");
					//3.�ȴ��Ƿ��˱ҳɹ�
					BillMsg = OSMboxPend(g_BillMoneyBackMail,OS_TICKS_PER_SEC*5,&err);//�����Ƿ��˱ҳɹ�			
					if(err == OS_NO_ERR) 
					{					
						//4.�˱ҳɹ��������˱ҳɹ���ֽ����
						if(BillMsg->BillBackCmd == MBOX_BILLRETURNSUCC)
						{
							ReturnBack = 1;
						}
						//5.�˱�ʧ�ܣ������˱�ʧ�ܸ�ֽ����
						else if(BillMsg->BillBackCmd == MBOX_BILLRETURNFAIL)
						{
							ReturnBack = 0;
						}
						//Trace("\r\n 2.=%d",ReturnBack);
						break;
					}
				}
				if(i >= 10)
				{
					ReturnBack = 0;
				}	
			}
			break;	
		default:break;	
	}
	return ReturnBack;
	
}



/*********************************************************************************************************
** Function name:       BillDevDisable
** Descriptions:        ����ֽ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void BillDevDisableAPI(void)
{
	switch(SystemPara.BillValidatorType)
	{		
		case MDB_BILLACCEPTER:		
			TraceBill("\r\n MddBILLDISABLEDEV post");
			MsgAccepterPack.BillBack = MBOX_BILLDISABLEDEV;			
			OSMboxPost(g_BillMoneyMail,&MsgAccepterPack);
			break;
		default:break;	
	}
	return;	
}






/*********************************************************************************************************
** Function name:       BillDevEnable
** Descriptions:        ʹ��ֽ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void BillDevEnableAPI(void)
{
	switch(SystemPara.BillValidatorType)
	{		
		case MDB_BILLACCEPTER:			
			MsgAccepterPack.BillBack = MBOX_BILLENABLEDEV;			
			OSMboxPost(g_BillMoneyMail,&MsgAccepterPack);
			break;
		default:break;	
	}
	return;
}

/*********************************************************************************************************
** Function name:       BillDevResetAPI
** Descriptions:        ����ֽ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void BillDevResetAPI(void)
{
	switch(SystemPara.BillValidatorType)
	{		
		case MDB_BILLACCEPTER:			
			MsgAccepterPack.BillBack = MBOX_BILLRESETDEV;			
			OSMboxPost(g_BillMoneyMail,&MsgAccepterPack);
			break;	
		default:break;	
	}
	return;
}


/*********************************************************************************************************
** Function name:       GetBillDevState
** Descriptions:        ���Ӳ�����豸�ĵ�ǰ״̬
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void GetBillDevStateAPI()
{
	return;	
}

/*********************************************************************************************************
** Function name:       BillRecyclerPayoutNumExpanseAPI
** Descriptions:        �������ұ�,0-15����ֵ��0x00-0x0f����ʾ,0x00,0x01��ʾ��������
** input parameters:    RecyPayoutMoney��Ҫ�ҵ���ֵ,RecyPayoutNum��Ҫ�ҵ�����
** output parameters:   ��
** Returned value:      1����ɹ�,0����ʧ��
*********************************************************************************************************/
unsigned char BillRecyclerPayoutNumExpanseAPI(unsigned int RecyPayoutMoney,unsigned char RecyPayoutNum)
{
	MessagePack *BillMsg;
	//MessageFSBillRecyclerPack *FSBillRecyclerMsg;
	uint8_t err,ReturnBack = 1;	
	uint32_t PayoutMoney=0,PayouBacktMoney=0;//�����ʵ��������
	
	switch(SystemPara.BillValidatorType)
	{		
		case MDB_BILLACCEPTER:	
			//ֽ��ѭ��������
			if(SystemPara.BillRecyclerType==MDB_BILLRECYCLER)
			{
				MsgAccepterPack.BillBack = MBOX_BILLRECYPAYOUTNUM;	
				MsgAccepterPack.RecyPayoutMoney = RecyPayoutMoney;	
				MsgAccepterPack.RecyPayoutNum = RecyPayoutNum;	
				OSMboxPost(g_BillMoneyMail,&MsgAccepterPack);
				TraceBill("\r\n MddRECPay=%ld,Num=%d",RecyPayoutMoney,RecyPayoutNum);
				OSMboxAccept(g_BillMoneyBackMail);
				//3.�ȴ��Ƿ�����ɹ�
				BillMsg = OSMboxPend(g_BillMoneyBackMail,OS_TICKS_PER_SEC*15*RecyPayoutNum,&err);//�����Ƿ�����ɹ�			
				if(err == OS_NO_ERR) 
				{					
					//4.�ɹ�������ѹ���ɹ���ֽ����
					if(BillMsg->BillBackCmd == MBOX_BILLRECYPAYOUTSUCC)
					{
						ReturnBack = 1;
					}
					//5.ʧ�ܣ�����ѹ��ʧ�ܸ�ֽ����
					else if(BillMsg->BillBackCmd == MBOX_BILLRECYPAYOUTFAIL)
					{
						ReturnBack = 0;
					}
					//Trace("\r\n 2.=%d",ReturnBack);					
				}				
					
			}
			else if(SystemPara.BillRecyclerType == FS_BILLRECYCLER)
			{
				/*MsgFSBillRecyclerPack.BillBack = MBOX_FSBILLRECYPAYOUTNUM;	
				MsgFSBillRecyclerPack.RecyPayoutMoney = RecyPayoutMoney;	
				MsgFSBillRecyclerPack.RecyPayoutNum = RecyPayoutNum;	
				OSMboxPost(g_FSBillRecyclerMail,&MsgFSBillRecyclerPack);
				print_fs("\r\n MddFSPay=%ld,Num=%d",RecyPayoutMoney,RecyPayoutNum);
				OSMboxAccept(g_FSBillRecyclerBackMail);
				//3.�ȴ��Ƿ�����ɹ�
				FSBillRecyclerMsg = OSMboxPend(g_FSBillRecyclerBackMail,OS_TICKS_PER_SEC*15*RecyPayoutNum,&err);//�����Ƿ�����ɹ�			
				if(err == OS_NO_ERR) 
				{					
					//4.�ɹ�������ѹ���ɹ���ֽ����
					if(FSBillRecyclerMsg->BillBackCmd == MBOX_FSBILLRECYPAYOUTSUCC)
					{
						print_fs("\r\n MddFSPayREC=SUCC");
						ReturnBack = 1;
					}
					//5.ʧ�ܣ�����ѹ��ʧ�ܸ�ֽ����
					else if(FSBillRecyclerMsg->BillBackCmd == MBOX_FSBILLRECYPAYOUTFAIL)
					{
						print_fs("\r\n MddFSPayREC=FAIL");
						ReturnBack = 0;
					}
					//Trace("\r\n 2.=%d",ReturnBack);					
				}
				*/
				PayoutMoney=RecyPayoutMoney*RecyPayoutNum;
				PayouBacktMoney=FS_dispense(PayoutMoney);		
				print_fs("\r\n MddFSPay=%ld,MddFSBackPay=%ld",PayoutMoney,PayouBacktMoney);
			}
			break;	
		default:break;	
	}	
	TraceBill("\r\n MddRECPaySuc=%d",ReturnBack);
	return ReturnBack;
}


/*********************************************************************************************************
** Function name:       BillRecyclerPayoutValueExpanse
** Descriptions:        ������ұ�                        
** input parameters:    RecyPayoutMoney��Ҫ�ҵĽ��,�Է�Ϊ��λ
** output parameters:   ��
** Returned value:      1����ɹ�,0����ʧ��
*********************************************************************************************************/
unsigned char BillRecyclerPayoutValueExpanseAPI(unsigned int RecyPayoutMoney,uint32_t  *RecyPayoutMoneyBack)
{
	MessagePack *BillMsg;
	uint8_t err,ReturnBack = 1;
	
	switch(SystemPara.BillValidatorType)
	{		
		case MDB_BILLACCEPTER:	
			//ֽ��ѭ��������
			if(SystemPara.BillRecyclerType==MDB_BILLRECYCLER)
			{
				if(RecyPayoutMoney)
				{
					MsgAccepterPack.BillBack = MBOX_BILLRECYPAYOUTVALUE;	
					MsgAccepterPack.RecyPayoutMoney = RecyPayoutMoney;	
					OSMboxPost(g_BillMoneyMail,&MsgAccepterPack);
					TraceBill("\r\n MddRECPay=%ld",RecyPayoutMoney);
					OSMboxAccept(g_BillMoneyBackMail);
					//3.�ȴ��Ƿ�����ɹ�
					BillMsg = OSMboxPend(g_BillMoneyBackMail,OS_TICKS_PER_SEC*60*5,&err);//�����Ƿ�����ɹ� 		
					if(err == OS_NO_ERR) 
					{					
						//4.����ɹ�����������ɹ���ֽ����
						if(BillMsg->BillBackCmd == MBOX_BILLRECYPAYOUTSUCC)
						{
							*RecyPayoutMoneyBack=BillMsg->RecyPayoutMoneyBack;
							ReturnBack = 1;
						}
						//5.����ʧ�ܣ���������ʧ�ܸ�ֽ����
						else if(BillMsg->BillBackCmd == MBOX_BILLRECYPAYOUTFAIL)
						{
							*RecyPayoutMoneyBack=0;
							ReturnBack = 0;
						}
						//Trace("\r\n 2.=%d",ReturnBack);					
					}	
					else
					{
						ReturnBack = 1;
						*RecyPayoutMoneyBack=RecyPayoutMoney;
					}	
				}
					
			}
			else if(SystemPara.BillRecyclerType == FS_BILLRECYCLER)
			{
				if(RecyPayoutMoney)
				{
					/*MsgFSBillRecyclerPack.BillBack = MBOX_FSBILLRECYPAYOUTNUM;	
					MsgFSBillRecyclerPack.RecyPayoutMoney = RecyPayoutMoney;	
					MsgFSBillRecyclerPack.RecyPayoutNum = RecyPayoutNum;	
					OSMboxPost(g_FSBillRecyclerMail,&MsgFSBillRecyclerPack);
					print_fs("\r\n MddFSPay=%ld,Num=%d",RecyPayoutMoney,RecyPayoutNum);
					OSMboxAccept(g_FSBillRecyclerBackMail);
					//3.�ȴ��Ƿ�����ɹ�
					FSBillRecyclerMsg = OSMboxPend(g_FSBillRecyclerBackMail,OS_TICKS_PER_SEC*15*RecyPayoutNum,&err);//�����Ƿ�����ɹ�			
					if(err == OS_NO_ERR) 
					{					
						//4.�ɹ�������ѹ���ɹ���ֽ����
						if(FSBillRecyclerMsg->BillBackCmd == MBOX_FSBILLRECYPAYOUTSUCC)
						{
							print_fs("\r\n MddFSPayREC=SUCC");
							ReturnBack = 1;
						}
						//5.ʧ�ܣ�����ѹ��ʧ�ܸ�ֽ����
						else if(FSBillRecyclerMsg->BillBackCmd == MBOX_FSBILLRECYPAYOUTFAIL)
						{
							print_fs("\r\n MddFSPayREC=FAIL");
							ReturnBack = 0;
						}
						//Trace("\r\n 2.=%d",ReturnBack);					
					}
					*/
					*RecyPayoutMoneyBack=FS_dispense(RecyPayoutMoney);		
					print_fs("\r\n MddFSPay=%ld,MddFSBackPay=%ld",RecyPayoutMoney,*RecyPayoutMoneyBack);
					ReturnBack = 1;
				}
			}
			break;
		default:break;	
	}	
	TraceBill("\r\n MddRECPaySuc=%d",ReturnBack);
	return ReturnBack;
}



