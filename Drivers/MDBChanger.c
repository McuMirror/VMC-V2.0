/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           CHANGERACCEPTER
** Last modified Date:  2013-03-04
** Last Version:         
** Descriptions:        ����������������غ�������                     
**------------------------------------------------------------------------------------------------------
** Created by:          gzz
** Version:             V2.0
** Descriptions:        The original version
**------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
********************************************************************************************************/
#include "..\config.h"	
#include "MDBChanger.h"



//��ǰֽ�����豸������
//uint8_t NowChangerDev = 0;
//ֽ����ͨ����ֵ
uint32_t nChangervalue[8]={0};
uint8_t NowChangerDevState = 0;

//����ֽ����ͨ��Ӧ�����ݰ���ʱ����
//extern unsigned int MDBCONVERSATIONWAITACKTIMEOUT;





/*********************************************************************************************************
** Function name:       ChangeGetTubes
** Descriptions:        MDBӲ���������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void ChangeGetTubes(void)
{
	uint8_t ChangerRdBuff[36],ChangerRdLen,ComStatus,j;
	
	//if(NowChangerDev==2)
	{		
	   	ComStatus = MdbConversation(0x0A,NULL,0x00,&ChangerRdBuff[0],&ChangerRdLen);
		if(ComStatus == 1)
		{
			 for(j = 0; j < 16; j++) 
			 {
				 stDevValue.CoinNum[j] =  ChangerRdBuff[2+j];				 
				 TraceChange("\r\nDrvChangebuf[%d] = %d", j,stDevValue.CoinNum[j]);
	         }
		}
	}
}

/*********************************************************************************************************
** Function name:       ChangePayoutProcessLevel3
** Descriptions:        level3�������
** input parameters:    PayMoney������Ҫ�ҳ���Ӳ�ҽ��
** output parameters:   
                        PayoutNum--��ͨ��ʵ�ʳ�������						
** Returned value:      ��Ӳ���ҳ�����1���޷���0
*********************************************************************************************************/
unsigned char ChangePayoutProcessLevel3(uint32_t PayMoney,unsigned char PayoutNum[16])
{
	unsigned char CoinRdBuff[36],CoinRdLen,ComStatus,VMCValue[2]={0},VMCPoll[1]={0};
	uint32_t coinscale,dispenseValue;
	uint8_t  i;
	

	//NowChangerDev = SystemPara.CoinChangerType;
	//Trace("6\r\n");	
	
	ChangeGetTubes();
	OSTimeDly(OS_TICKS_PER_SEC / 10);
	//stDevValue.CoinLevel = 2;
	/***************************************
	** level3����ֱ�ӷ����������
	***************************************/
	//if(stDevValue.CoinLevel >= 3)
	{
		coinscale = stDevValue.CoinScale;
		dispenseValue = PayMoney / coinscale;//���������׼����			
		TraceChange("\r\nDrvChangescale = %d,%d", coinscale,dispenseValue);
		VMCValue[0] = 0x02;
		VMCValue[1] = dispenseValue;
		//1�����ұ�ָ��
		ComStatus = MdbConversation(0x0F,VMCValue,2,&CoinRdBuff[0],&CoinRdLen);
		if(ComStatus == 1)
		{
			Timer.PayoutTimer = dispenseValue/2 + 20;
			while(Timer.PayoutTimer)
			{					
				//2������չpollָ�����ұ��Ƿ����
				VMCPoll[0] = 0x04;
				ComStatus = MdbConversation(0x0F,VMCPoll,1,&CoinRdBuff[0],&CoinRdLen);
				TraceChange("\r\nDrvChangesend = %d,%d,%d,%d\r\n",ComStatus,CoinRdBuff[0],CoinRdBuff[1],CoinRdLen);
				//�������ʱ��CoinRdLen=1 ������ɺ�CoinRdLen = 0
				if( CoinRdLen == 0 )
				{
					//CoinRdLen = 0;
					//3������չָ���Ȿ���ұҸ�ͨ���Ҷ���ö
					VMCPoll[0] = 0x03;
					ComStatus = MdbConversation(0x0F,VMCPoll,1,&CoinRdBuff[0],&CoinRdLen);
					TraceChange("\r\nDrvChange s = %d,%d,%d,%d\r\n",ComStatus,CoinRdBuff[0],CoinRdBuff[1],CoinRdLen);
					if( CoinRdLen > 0 )
					{
						TraceChange("\r\nDrvChangeOut=%d,%d,%d,%d",CoinRdBuff[0],CoinRdBuff[1],CoinRdBuff[2],CoinRdBuff[3]);
						for(i = 0;i < 16;i++)
						{
							PayoutNum[i] = CoinRdBuff[i];
						}
						break;
					}
				}
				OSTimeDly(OS_TICKS_PER_SEC / 100);
			}
		}	
		else
		{
			TraceChange("\r\nDrvChangesendFail");
		}
	}		
	return 0;
}


/*********************************************************************************************************
** Function name:       ChangePayoutProcessLevel2
** Descriptions:        level2�������
** input parameters:    PayType��������ͨ��
**                      PayNum ������������
** output parameters:   PayoutNum--��ͨ��ʵ�ʳ�������							
** Returned value:      ��Ӳ���ҳ�����1���޷���0
*********************************************************************************************************/
unsigned char ChangePayoutProcessLevel2(unsigned char PayType,unsigned char PayNum, unsigned char PayoutNum[8])
{
	unsigned char ComStatus,VMCPoll[1]={0},CoinRdBuff[36],CoinRdLen;
	
	//5����
	VMCPoll[0]  =  PayType&0x0f;
	VMCPoll[0] |= ((PayNum<<4)&0xf0);
	ComStatus = MdbConversation(0x0D,VMCPoll,1,&CoinRdBuff[0],&CoinRdLen);
	//6�ϴ�ͨ���ĳ���ö��
	if(ComStatus == 1)
	{
		PayoutNum[PayType] = PayNum;	
		TraceChange("\r\nDrvChange nu=%d",PayoutNum[PayType]);
	}	
	return 0;
}




