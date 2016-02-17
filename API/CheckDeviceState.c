/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           CheckDeviceState
** Last modified Date:  2013-03-04
** Last Version:         
** Descriptions:        ����豸����״̬��غ�������                     
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
uint8_t	 	billEnable = 1;//��ǰֽ����״̬,1��,0�ر�
uint8_t	 	coinEnable = 1;//��ǰӲ����״̬,1��,0�ر�
uint8_t	 	scaleError = 0;//��ǰ������ݱ���ϵ����ϵ����Ĵ���,1����,0��ȷ
uint8_t	 	weihuMode = 0;//��ǰ��ά��״̬1,����״̬0


extern void BillCoinCtr(uint8_t billCtr,uint8_t coinCtr,uint8_t readerCtr);

/*********************************************************************************************************
** Function name:       CheckDeviceState
** Descriptions:        ����ֽ������Ӳ�������������豸����״̬
** input parameters:    ��
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void CheckDeviceState()
{ 
	uint8_t err;
	DEVICESTATEPACK *DeviceMsg;
	
	//��ѯ���豸״̬
	DeviceStatePack.DevStateCmd = MBOX_GETDEVSTATE;						
	OSMboxPost(g_DeviceStateMail,&DeviceStatePack);
	//OSTimeDly(OS_TICKS_PER_SEC/2);
	//���ظ��豸״̬
	DeviceMsg = OSMboxPend(g_DeviceStateBackMail,OS_TICKS_PER_SEC*30,&err);		
	if(err == OS_NO_ERR) 
	{
		TraceChange("\r\n Middle1Change");
		if(DeviceMsg->DevStateCmd == MBOX_RETURNDEVSTATE)
		{
			TraceChange("\r\n Middle2Change");
			memcpy(&DeviceStateBusiness,DeviceMsg,sizeof(DeviceStateBusiness));
			DeviceStateBusiness.ErrorInit = 1;
			TraceBill("\r\n Middlebill=%d,%d,%d,%d,%d,%d,%d,%d",DeviceStateBusiness.BillCommunicate,DeviceStateBusiness.Billmoto,DeviceStateBusiness.Billsensor,DeviceStateBusiness.Billromchk,
				DeviceStateBusiness.Billjam,DeviceStateBusiness.BillremoveCash,DeviceStateBusiness.BillcashErr,DeviceStateBusiness.Billdisable);
			TraceCoin("\r\n Middlecoin=%d,%d,%d,%d,%d,%d,%d,%d,%d",DeviceStateBusiness.CoinCommunicate,DeviceStateBusiness.Coinsensor,DeviceStateBusiness.Cointubejam,DeviceStateBusiness.Coinromchk,
				DeviceStateBusiness.Coinrouting,DeviceStateBusiness.Coinjam,DeviceStateBusiness.CoinremoveTube,DeviceStateBusiness.Coindisable,DeviceStateBusiness.CoinunknowError);
			TraceChange("\r\n MiddleHopper2=%d,%d,%d",DeviceStateBusiness.Hopper1State,DeviceStateBusiness.Hopper2State,DeviceStateBusiness.Hopper3State);
			TraceChannel("\r\n MiddleChannel=%d,%d,%d,%d",DeviceStateBusiness.GOCError,DeviceStateBusiness.GOCErrorSub,DeviceStateBusiness.ColBoardError,DeviceStateBusiness.ColBoardErrorSub);
			TraceChannel("\r\n MiddleChannelSJT1=%d,%d,%d,%d,%d",DeviceStateBusiness.Error_FMD,DeviceStateBusiness.Error_FUM,DeviceStateBusiness.Error_GOC,DeviceStateBusiness.Error_FLD,DeviceStateBusiness.Error_FOD);
			TraceChannel("\r\n MiddleChannelSJT2=%d,%d,%d,%d,%d",DeviceStateBusiness.Error_UDS,DeviceStateBusiness.Error_GCD,DeviceStateBusiness.Error_SOVB,DeviceStateBusiness.Error_SOMD1,DeviceStateBusiness.Error_SOEC);
			TraceChannel("\r\n MiddleChannelSJT3=%d,%d,%d,%d",DeviceStateBusiness.Error_SFHG,DeviceStateBusiness.Error_SOFP,DeviceStateBusiness.Error_SOMD2,DeviceStateBusiness.Emp_Gol);
		}
	}
}

/*********************************************************************************************************
** Function name:       SetScaleError
** Descriptions:        ���ý�����ݱ���ϵ����ϵ�Ƿ����
** input parameters:    errorValue = 1����,0��ȷ
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void SetScaleError(uint8_t errorValue)
{
	scaleError = errorValue;
}

/*********************************************************************************************************
** Function name:       GetScaleError
** Descriptions:        �õ�������ݱ���ϵ����ϵ�Ƿ����
** input parameters:     1����,0��ȷ
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
uint8_t GetScaleError()
{
	return scaleError;
}



/*********************************************************************************************************
** Function name:       IsErrorState
** Descriptions:        �Ƿ�������״̬
** input parameters:    ��
** output parameters:   ��
** Returned value:      1���ϣ�0���� 
*********************************************************************************************************/
uint8_t IsErrorState()
{ 
	uint8_t coinError = 0,hopperError = 0,GOCError = 0,ColBoardError = 0,PcErr=0;
	static uint8_t billError = 0;
	//ֽ����	
	if(SystemPara.BillValidatorType==MDB_BILLACCEPTER)
	{
		if(
			(DeviceStateBusiness.BillCommunicate)||(DeviceStateBusiness.Billmoto)||(DeviceStateBusiness.Billsensor)||(DeviceStateBusiness.Billromchk)
			||(DeviceStateBusiness.Billjam)||(DeviceStateBusiness.BillremoveCash)||(DeviceStateBusiness.BillcashErr)
		  )
		{
			billError = 1;
		}
		else if(billError == 1)
		{
			billError = 2;
		}
		TraceBill("\r\n MiddBillState=%d",billError);
	}
	
	//Ӳ����
	if(SystemPara.CoinAcceptorType==MDB_COINACCEPTER)
	{
		if(
			(DeviceStateBusiness.CoinCommunicate)||(DeviceStateBusiness.Coinsensor)||(DeviceStateBusiness.Cointubejam)||(DeviceStateBusiness.Coinromchk)
			||(DeviceStateBusiness.Coinjam)||(DeviceStateBusiness.CoinremoveTube)||(DeviceStateBusiness.CoinunknowError)
		  )
		{
			coinError = 1;
		}
	}
	//Hopper������
	if(SystemPara.CoinChangerType == HOPPER_CHANGER)
	{
		if(DeviceStateBusiness.Hopper1State == 2)
		{
			hopperError = 1;
		}
	}
	if(SystemPara.GeziDeviceType==0)
	{
		//GOC
		if(SystemPara.GOCIsOpen == 1)
		{
			if(SystemPara.SubBinOpen==1)
			{
				if( (DeviceStateBusiness.GOCError != 0)&&(DeviceStateBusiness.GOCErrorSub != 0) )
				{
					GOCError = 1;
				}
			}
			else
			{
				if(DeviceStateBusiness.GOCError != 0)
				{
					GOCError = 1;
				}
			}
		}
		//������
		if(SystemPara.SubBinOpen==1)
		{
			if( (DeviceStateBusiness.ColBoardError != 0)&&(DeviceStateBusiness.ColBoardErrorSub != 0) )
			{
				ColBoardError = 1;
			}
		}
		else
		{
			if(DeviceStateBusiness.ColBoardError != 0)
			{
				ColBoardError = 1;
			}
		}
		//����̨
		if(SystemPara.ChannelType == 2)
		{
			if(
				(DeviceStateBusiness.Error_FMD)||(DeviceStateBusiness.Error_FUM)||(DeviceStateBusiness.Error_GOC)||(DeviceStateBusiness.Error_FLD)||(DeviceStateBusiness.Error_FOD)
				||(DeviceStateBusiness.Error_UDS)||(DeviceStateBusiness.Error_GCD)||(DeviceStateBusiness.Error_SOVB)||(DeviceStateBusiness.Error_SOMD1)||(DeviceStateBusiness.Error_SOEC)
				||(DeviceStateBusiness.Error_SFHG)||(DeviceStateBusiness.Error_SOFP)||(DeviceStateBusiness.Error_SOMD2)
			  )
			{
				ColBoardError = 1;
			}
		}
		//if(DeviceStateBusiness.Emp_Gol != 0)
		//{
		//	ColEmpErr = 1;
		//}
	}

	if(SystemPara.PcEnable == UBOX_PC)
	{
		if(LogPara.offLineFlag == 1)
			PcErr = 1;
	}

	if(coinError||hopperError||GOCError||ColBoardError||scaleError||PcErr)
	{
		return 1;
	}
	else
	{
		if(billError == 2)
		{
			billError = 0;
			ResetBill();
			TraceBill("\r\n MiddBillState2=%d",billError);
		}
		return 0;
	}
}

/*********************************************************************************************************
** Function name:       HopperIsEmpty
** Descriptions:        Hopper�������Ƿ�Ϊ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      1ȱ��,0�㹻
*********************************************************************************************************/
uint8_t HopperIsEmpty()
{ 
	if(SystemPara.CoinChangerType == HOPPER_CHANGER)
	{
		//if((DeviceStateBusiness.Hopper1State == 1)&&(DeviceStateBusiness.Hopper2State != 0)&&(DeviceStateBusiness.Hopper3State != 0))
        if(DeviceStateBusiness.Hopper1State == 1)
		{
			return 1;
		}
	}
	return 0;
}



/*********************************************************************************************************
** Function name:       ChangerIsErr
** Descriptions:        �������Ƿ񲻿���״̬
** input parameters:    1����,0����
** output parameters:   ��
** Returned value:      1��,0����
*********************************************************************************************************/
uint8_t ChangerIsErr()
{ 
	if(SystemPara.CoinChangerType==OFF_CHANGER)
	{
		return 1;
	}
	else if(SystemPara.CoinChangerType==MDB_CHANGER)
	{
		if(
			(DeviceStateBusiness.CoinCommunicate)||(DeviceStateBusiness.Coinsensor)||(DeviceStateBusiness.Cointubejam)||(DeviceStateBusiness.Coinromchk)
			||(DeviceStateBusiness.Coinjam)||(DeviceStateBusiness.CoinremoveTube)||(DeviceStateBusiness.CoinunknowError)
		  )
		{
			return 1;
		}
	}
	else if(SystemPara.CoinChangerType == HOPPER_CHANGER)
	{
		if((DeviceStateBusiness.Hopper1State != 0)&&(DeviceStateBusiness.Hopper2State != 0)&&(DeviceStateBusiness.Hopper3State != 0))
		{
			return 1;
		}
	}
	return 0;
}

/*********************************************************************************************************
** Function name:       BillIsErr
** Descriptions:        ֽ�����Ƿ񲻿���״̬
** input parameters:    1����,0����
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
uint8_t BillIsErr()
{ 
	if(SystemPara.BillValidatorType==MDB_BILLACCEPTER)
	{
		if(
			(DeviceStateBusiness.BillCommunicate)||(DeviceStateBusiness.Billmoto)||(DeviceStateBusiness.Billsensor)||(DeviceStateBusiness.Billromchk)
			||(DeviceStateBusiness.Billjam)||(DeviceStateBusiness.BillremoveCash)||(DeviceStateBusiness.BillcashErr)
		  )
		{
			return 1;
		}		
		//TraceBill("\r\n MiddBillState=%d",billError);
	}
	return 0;
}


/*********************************************************************************************************
** Function name:       SetBillCoinStatus
** Descriptions:        ����ֽ����,Ӳ������ǰ״̬
** input parameters:    type=1ֽ����,2Ӳ����;enable = 1����,0�ر�
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void SetBillCoinStatus(uint8_t type,uint8_t enable)
{
	if(type == 1)
		billEnable = enable;
	else if(type == 2)
		coinEnable = enable;
}

/*********************************************************************************************************
** Function name:       SetBillCoinStatus
** Descriptions:        �õ�ֽ����,Ӳ������ǰ״̬
** input parameters:    type=1ֽ����,2Ӳ����;
** output parameters:   ��
** Returned value:      1��,0�ر�
*********************************************************************************************************/
uint8_t GetBillCoinStatus(uint8_t type)
{
	if(type == 1)
		return billEnable;
	else if(type == 2)
		return coinEnable;
	return 0;
}


/*********************************************************************************************************
** Function name:       SetWeihuStatus
** Descriptions:        ����ά��״̬
** input parameters:    enable=1ά��״̬;0����״̬
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void SetWeihuStatus(uint8_t enable)
{
	weihuMode = enable;	
}

/*********************************************************************************************************
** Function name:       GetWeihuStatus
** Descriptions:        ����ά��״̬
** input parameters:    enable=1ά��״̬;0����״̬
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
uint8_t GetWeihuStatus()
{
	return weihuMode;	
}


/*********************************************************************************************************
** Function name:       ErrorStatus
** Descriptions:        
** input parameters:    ��
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
uint8_t ErrorStatus(uint8_t type)
{ 
	//������״̬��0=������1=�������ʱ���ã�2=���ϣ�3=�豸������
	if(type == 1)
	{
		//MDB������
		if(SystemPara.CoinChangerType==MDB_CHANGER)
		{
			if(
				(DeviceStateBusiness.CoinCommunicate)||(DeviceStateBusiness.Coinsensor)||(DeviceStateBusiness.Cointubejam)||(DeviceStateBusiness.Coinromchk)
				||(DeviceStateBusiness.Coinjam)||(DeviceStateBusiness.CoinremoveTube)||(DeviceStateBusiness.CoinunknowError)
			  )
			{
				return 2;
			}			
			
		}
		//Hopper������
		else if(SystemPara.CoinChangerType == HOPPER_CHANGER)
		{
			if(DeviceStateBusiness.Hopper1State == 2)
			{
				return 2;
			}
		}
		else
			return 3;
		
		return 0;
	}
	//ֽ����	0=������1=�������ʱ���ã�2=���ϣ�3=�豸������
	if(type == 2)
	{
		if(SystemPara.BillValidatorType==MDB_BILLACCEPTER)
		{
			if(
				(DeviceStateBusiness.BillCommunicate)||(DeviceStateBusiness.Billmoto)||(DeviceStateBusiness.Billsensor)||(DeviceStateBusiness.Billromchk)
				||(DeviceStateBusiness.Billjam)||(DeviceStateBusiness.BillremoveCash)||(DeviceStateBusiness.BillcashErr)
			  )
			{
				return 2;
			}	
			if(GetBillCoinStatus(1)==0)
				return 1;
			
			return 0;
		}
		else
			return 3;
	}
	
	//Ӳ����   Ӳ����״̬��0=������1=�������ʱ���ã�2=���ϣ�3=�豸������
	if(type == 3)
	{
		if(SystemPara.CoinAcceptorType==MDB_COINACCEPTER)
		{
			if(
				(DeviceStateBusiness.CoinCommunicate)||(DeviceStateBusiness.Coinsensor)||(DeviceStateBusiness.Cointubejam)||(DeviceStateBusiness.Coinromchk)
				||(DeviceStateBusiness.Coinjam)||(DeviceStateBusiness.CoinremoveTube)||(DeviceStateBusiness.CoinunknowError)
			  )
			{
				return 2;
			}
			if(GetBillCoinStatus(2)==0)
				return 1;
			
		}
		//����Ӳ����
		else if((SystemPara.CoinAcceptorType == PARALLEL_COINACCEPTER)||(SystemPara.CoinAcceptorType == SERIAL_COINACCEPTER))
		{
			if(GetBillCoinStatus(2)==0)
				return 1;			
		}
		else
			return 3;
		
		return 0;
	}
	//vmc 0=������1= ����������Ʒȫ���ۿգ��������ʱ�䣬2=���ϣ�3=ά��ģʽ
	if(type == 4)
	{
		//if((DeviceStateBusiness.Emp_Gol != 0)&&(SystemPara.GeziDeviceType==0))
		//	return 1;
		//else 
		if(IsErrorState())
			return 2;
		else if(weihuMode==1)
			return 3;
		else
			return 0;
	}
	return 0;
}

/*********************************************************************************************************
** Function name:       SIMPLEErrorStatus
** Descriptions:        ΪSIMPLEUBox��������״̬���ӿ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
uint8_t SIMPLEErrorStatus(uint8_t type)
{ 
	//����������Ʒȫ���ۿգ��������ʱ��
	if(type == 1)
	{
		if((DeviceStateBusiness.Emp_Gol != 0)&&(SystemPara.GeziDeviceType==0))
			return 1;
		else
			return 0;
	}
	//�Ƿ����
	if(type == 2)
	{
		if(IsErrorState())
			return 1;
		else	
			return 0;		
	}
	
	//ά��ģʽ
	if(type == 3)
	{
		if(weihuMode==1)
			return 1;
		else
			return 0;
	}
	return 0;	
}



