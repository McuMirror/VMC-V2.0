/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           SelectKey.C
** Last modified Date:  2013-03-04
** Last Version:         
** Descriptions:        ѡ����������������غ�������                     
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
#include "SelectKey.h"


#define CMDHEAD 		0xED
#define SELECTKEYNUM			6	 
uint8_t SelectKeySN[SELECTKEYNUM],SelectKeyNo[SELECTKEYNUM];   //ͬ����ż����

#define SELECTKEY_QUERY		0x51
#define SELECTKEY_QUERYRUN	0x52//ѡ����������ʱʹ�������;by gzz 20121121


#define ID_SELECTION1	0x16//0xA1
#define ID_SELECTION2	0x17//0xA2
#define ID_SELECTION3	0x18//0xA3
#define ID_SELECTION4	0x19
#define ID_SELECTION5	0x1A
#define ID_SELECTION6	0x1B



uint8_t evbbuf2[20]={0};

#define KEY1_OK 1 << 0
#define KEY2_OK 1 << 1
#define KEY3_OK 1 << 2
#define KEY4_OK 1 << 3
#define KEY5_OK 1 << 4
#define KEY6_OK 1 << 5

#define KEY1_ER 1 << 8
#define KEY2_ER 1 << 9
#define KEY3_ER 1 << 10
#define KEY4_ER 1 << 11
#define KEY5_ER 1 << 12
#define KEY6_ER 1 << 13

typedef struct          //���������ʽ 
{				 
   uint8_t Head;  		//����ͷ
   uint8_t Lenth;  		//����
   uint8_t SN;	      	//ͬ�����
   uint8_t Com;	     	//������
   uint8_t DevNo;     	//�豸��
   uint8_t rsv1;	  	//����
   uint8_t rsv2;	  	//������⣨1:�� 0:�أ�
   uint8_t chksum;    	//У����
} EVBCommand;
EVBCommand SelectKeycom;

SELECTKEY selKey;

/*********************************************************************************************************
** Function name:     	SelectKey_Command
** Descriptions:	    EVBͨѶ����ѡ�������巢�ͺͽ��հ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      0�����ճɹ�������������ʧ��
*********************************************************************************************************/
static uint8_t SelectKey_Command(EVBCommand SelectKeycom)
{
	uint8_t *pcombuf;
	uint8_t checksum, i, j;
	uint8_t  status=0, lenth;
	//unsigned char retry = 3;
	//unsigned char recRes=0;

	//Uart1_ClrBuf();
	//Uart3x_ClrBuf();

	//Uart2_SetEvbMode();
	//Uart2_SetParity(PARITY_DIS);
	

	SelectKeycom.Head = CMDHEAD;//0xED;
	SelectKeycom.Lenth = 0x08;//param+2

	pcombuf = (uint8_t *)&SelectKeycom;

	checksum = 0;

	for(i = 0; i < SelectKeycom.Lenth - 1; i++) 
	{
	  checksum ^= pcombuf[i];
	}

	SelectKeycom.chksum = checksum;

		
	//1.����ָ��	
	for(i = 0; i < 3; i++) 
	{
		//Trace("\r\nW=%02x",SelectKeycom.Lenth);
		//Uart1_Write((uint8_t *)&SelectKeycom, SelectKeycom.Lenth);
		//ѡ������;by gzz 20121025
		//Uart3xPutStr((uint8_t *)&SelectKeycom, SelectKeycom.Lenth);
		//Trace("\r\n begin send");
		/*
		Uart1_SendByte(SelectKeycom.Head);
		Uart1_SendByte(SelectKeycom.Lenth);
		Uart1_SendByte(SelectKeycom.SN);
		Uart1_SendByte(SelectKeycom.Com);
		Uart1_SendByte(SelectKeycom.DevNo);
		Uart1_SendByte(SelectKeycom.rsv1);
		Uart1_SendByte(SelectKeycom.rsv2);
		Uart1_SendByte(SelectKeycom.chksum);
		*/
		//OSSchedLock();
		//��������
		//Uart3PutStr((uint8_t *)&SelectKeycom, SelectKeycom.Lenth);
		Uart1PutStr((uint8_t *)&SelectKeycom, SelectKeycom.Lenth);
		TraceSelection("\r\n drvSend=%x,%x,%x,%x,%x,%x,%x,%x",SelectKeycom.Head,SelectKeycom.Lenth,SelectKeycom.SN,SelectKeycom.Com,SelectKeycom.DevNo,SelectKeycom.rsv1,SelectKeycom.rsv2,SelectKeycom.chksum);
		OSTimeDly(3);
		//OSSchedUnlock();	
		//OSTimeDly(OS_TICKS_PER_SEC * 3);
		status = 0;
		checksum = 0;
		//recRes = 0;
		memset(evbbuf2,0,sizeof(evbbuf2));
		//��������
		lenth = Uart1_Read(evbbuf2,8);
		if( lenth )
		{	
			 //TraceSelection("\r\n drvRec=%x,%x,%x,%x,%x,%x,%x,%x",evbbuf2[0],evbbuf2[1],evbbuf2[2],evbbuf2[3],evbbuf2[4],evbbuf2[5],evbbuf2[6],evbbuf2[7]);
			 if(evbbuf2[0] != 0xFD) 
			 {
				TraceSelection("\r\nDrvSelERR_BEGIN");
				status = ERR_STX;
				//return status;
			 } 
			 else 
			 {
				for(j = 0; j < lenth - 1; j++) 
				{
				   checksum ^= evbbuf2[j];
				}

				if(checksum != evbbuf2[j]) 
				{			
					TraceSelection("\r\nDrvSelERR_CHKSUM");
					status = ERR_CHKSUM;
					//return status;
				}
				else 
				{
					//TraceSelection("\r\n drvRec=%x,%x,%x,%x,%x,%x,%x,%x",evbbuf2[0],evbbuf2[1],evbbuf2[2],evbbuf2[3],evbbuf2[4],evbbuf2[5],evbbuf2[6],evbbuf2[7]);
					status = ERR_NULL;
					//�д��ڰ���
					if( evbbuf2[5] != 0xff )
					{
						TraceSelection("\r\n drvRec=%x,%x,%x,%x,%x,%x,%x,%x,key=%d",evbbuf2[0],evbbuf2[1],evbbuf2[2],evbbuf2[3],evbbuf2[4],evbbuf2[5],evbbuf2[6],evbbuf2[7],evbbuf2[5]);
						Buzzer();//�������� chen
					}
					//return status;
				}
			 }			 
			 break;
		}
		OSTimeDly(3);
	}		
	if( i >= 3 )
	{
		//OSTimeDly(10);		
		TraceSelection("\r\nDrvSelERR_TIMEOUT");
		status = ERR_TIMEOUT;		
	}
	//Trace("\r\n 3res");
	//OSTimeDly(OS_TICKS_PER_SEC / 10);
	return status;
}

#if 0
{
	uint8_t *pcombuf;
	uint8_t checksum, i, j;
	uint8_t  status=0, lenth;
	unsigned char retry = 3;
	unsigned char recRes=0;

	//Uart1_ClrBuf();
	//Uart3x_ClrBuf();

	//Uart2_SetEvbMode();
	//Uart2_SetParity(PARITY_DIS);
	

	SelectKeycom.Head = CMDHEAD;//0xED;
	SelectKeycom.Lenth = 0x08;//param+2

	pcombuf = (uint8_t *)&SelectKeycom;

	checksum = 0;

	for(i = 0; i < SelectKeycom.Lenth - 1; i++) 
	{
	  checksum ^= pcombuf[i];
	}

	SelectKeycom.chksum = checksum;

	    
    //1.����ָ��    
	while( retry )
	{
		//Trace("\r\nW=%02x",SelectKeycom.Lenth);
		//Uart1_Write((uint8_t *)&SelectKeycom, SelectKeycom.Lenth);
		//ѡ������;by gzz 20121025
		//Uart3xPutStr((uint8_t *)&SelectKeycom, SelectKeycom.Lenth);
		//Trace("\r\n begin send");
		/*
		Uart1_SendByte(SelectKeycom.Head);
		Uart1_SendByte(SelectKeycom.Lenth);
		Uart1_SendByte(SelectKeycom.SN);
		Uart1_SendByte(SelectKeycom.Com);
		Uart1_SendByte(SelectKeycom.DevNo);
		Uart1_SendByte(SelectKeycom.rsv1);
		Uart1_SendByte(SelectKeycom.rsv2);
		Uart1_SendByte(SelectKeycom.chksum);
		*/
		//Uart3PutStr((uint8_t *)&SelectKeycom, SelectKeycom.Lenth);
		Uart1PutStr((uint8_t *)&SelectKeycom, SelectKeycom.Lenth);
		//TraceSelection("\r\n drvSend=%x,%x,%x,%x,%x,%x,%x,%x",SelectKeycom.Head,SelectKeycom.Lenth,SelectKeycom.SN,SelectKeycom.Com,SelectKeycom.DevNo,SelectKeycom.rsv1,SelectKeycom.rsv2,SelectKeycom.chksum);
		OSTimeDly(OS_TICKS_PER_SEC / 100);
		//OSTimeDly(OS_TICKS_PER_SEC * 3);
		Timer.selTimer = 100;
		status = 0;
		checksum = 0;
		recRes = 0;
		while( Timer.selTimer )
		{
			lenth = Uart1_Read(evbbuf2,8);
			if( lenth )
			{	
				 //TraceSelection("\r\n drvRec=%x,%x,%x,%x,%x,%x,%x,%x",evbbuf2[0],evbbuf2[1],evbbuf2[2],evbbuf2[3],evbbuf2[4],evbbuf2[5],evbbuf2[6],evbbuf2[7]);
				 if(evbbuf2[0] != 0xFD) 
				 {
					TraceSelection("\r\nDrvSelERR_BEGIN");
				    status = ERR_STX;
					//return status;
				 } 
				 else 
				 {
					for(j = 0; j < lenth - 1; j++) 
					{
				       checksum ^= evbbuf2[j];
				    }

					if(checksum != evbbuf2[j]) 
					{			
					 	TraceSelection("\r\nDrvSelERR_CHKSUM");
				        status = ERR_CHKSUM;
						//return status;
					}
					else 
					{
						//TraceSelection("\r\n drvRec=%x,%x,%x,%x,%x,%x,%x,%x",evbbuf2[0],evbbuf2[1],evbbuf2[2],evbbuf2[3],evbbuf2[4],evbbuf2[5],evbbuf2[6],evbbuf2[7]);
						status = ERR_NULL;
						//return status;
				    }
				 }
				 recRes = 1;
				 break;
			}
			OSTimeDly(OS_TICKS_PER_SEC / 100);
		}
		if(Timer.selTimer==0)
		{
			retry--;
			//TracePC("\r\n Drv failretry=%d",retry); 
		}	
		//Trace("\r\n 1res");
		if(recRes)
		{
			//Trace("\r\n 2res");
			break;
		}
		OSTimeDly(OS_TICKS_PER_SEC / 100);
		
	}
	if( retry== 0 )
	{
		OSTimeDly(10);		
		TraceSelection("\r\nDrvSelERR_TIMEOUT");
		status = ERR_TIMEOUT;	    
	}
    //Trace("\r\n 3res");
    //OSTimeDly(OS_TICKS_PER_SEC / 10);
    return status;
}
#endif


//120816 by cq SelectKey
//ѡ����������ʱʹ�������;by gzz 20121121
//
/*********************************************************************************************************
** Function name:     	SelectKeySet
** Descriptions:	    EVBͨѶ����ѡ�������巢�ͺͽ��հ�
** input parameters:   
						selectkeych����ѡ��������,0	- 5
						ReadyLed��ȷ�ĵ�,ErrLed����ĵ�
						ledMode:0����״̬;1����״̬
** output parameters:   ��
** Returned value:      0�����ճɹ�������������ʧ��
*********************************************************************************************************/
uint8_t SelectKeySet(uint8_t selectkeych, uint8_t ReadyLed, uint8_t ErrLed,uint8_t ledMode)
{
   EVBCommand SelectKeycom;
   uint8_t  status;

   
   SelectKeycom.SN = SelectKeySN[selectkeych];
   if(SystemPara.threeSelectKey==1)
   {
   		SelectKeycom.Com = SELECTKEY_QUERY;//HOPPER_PAYOUT;
   }
   else	
   {
	   //ѡ����������ʱʹ�������;by gzz 20121121
	   if(ledMode==0)
	   		SelectKeycom.Com = SELECTKEY_QUERY;//HOPPER_PAYOUT;
	   else	if(ledMode==1)
	   		SelectKeycom.Com = SELECTKEY_QUERYRUN;//HOPPER_PAYOUT;
   }		
   		
   SelectKeycom.DevNo = SelectKeyNo[selectkeych];

   
   SelectKeycom.rsv1 = ReadyLed;//0;//ok//
   SelectKeycom.rsv2 = ErrLed;//0xff;//err//

   status = SelectKey_Command(SelectKeycom);

   return status;
}

/*
uint8_t SelectKeySetled(uint8_t selectkeych, uint16_t amount)
{
   EVBCommand SelectKeycom;
   uint8_t  status;

   
   //SelectKeycom.Head = CMDHEAD;//0xED;
   //SelectKeycom.Lenth = 0x08;//param+2

   SelectKeycom.SN = SelectKeySN[selectkeych];
   SelectKeycom.Com = SELECTKEY_QUERY;//HOPPER_PAYOUT;
   SelectKeycom.DevNo = SelectKeyNo[selectkeych];

   
   SelectKeycom.rsv1 = (uint8_t)amount;//0;//ok//
   SelectKeycom.rsv2 = (uint8_t)(amount >> 8);//0xff;//err//

   status = SelectKey_Command(SelectKeycom);

   return status;
}
*/

/*********************************************************************************************************
** Function name:     	GetMiddleSelectKey
** Descriptions:	    ����ѡ�������巵�صĽ���ֵ,ת������Ӧ����ֵ���ϴ���Ӧ�ò�
** input parameters:   	keyDevѡ���������豸��,	keyValue����ֵ				 
						
** output parameters:   
** Returned value:      ת����İ���ֵ

*********************************************************************************************************/
uint8_t GetMiddleSelectKey(uint8_t keyDev,uint8_t keyValue)
{
   uint8_t  key=0,i;
   for(i = 0; i < SELECTKEYNUM;i++)
   {
	   if(keyDev==SelectKeyNo[i])
	   {
	   		key = keyValue + 10*(i+1)+1;
	   }
   }
   return key;
}

/*********************************************************************************************************
** Function name:     	GetSelectKey
** Descriptions:	    ѡ�������巢�ͺͽ���ֵ
** input parameters:   						 
						
** output parameters:   ��
** Returned value:      
�ް���ʱ����0xfd 0x8 00 00 0x16 0xff 00 0x1c,����0x16����ѡ������豸��,��һ���ֽڴ�������,���û�а��£�����0xff,
�а��£�������Ӧ�ĸ���ť���µ�

*********************************************************************************************************/
uint8_t GetSelectKey()
{
    uint8_t      pStatus,j;
	uint8_t key=0xff;
	uint8_t keyNum=0;
	if(SystemPara.threeSelectKey==1)
		keyNum=3;
	else
		keyNum=6;
	
	for(j = 0; j < keyNum; j++)	
	{		
		/*
		//led = 0x3f3f;
		//led = (sysVPMission.sel2ErrLed<<8)|(sysVPMission.sel2ReadyLed);
		//led = sysVPMission.sel2ErrLed;
		//led = (led<<8)|sysVPMission.sel2ReadyLed;
		led = 0x3f3f;
		pStatus = SelectKeySetled(j, led);
		*/


		
		switch(j)
		{
			case 0://ѡ����������ʱʹ�������;by gzz 20121121
				pStatus = SelectKeySet(j, selKey.sel1ReadyLed,selKey.sel1ErrLed,selKey.selectMode); 
				break;
			case 1:
				pStatus = SelectKeySet(j, selKey.sel2ReadyLed,selKey.sel2ErrLed,selKey.selectMode);
				break;
			case 2:
				pStatus = SelectKeySet(j, selKey.sel3ReadyLed,selKey.sel3ErrLed,selKey.selectMode);
				break;
			case 3:
				pStatus = SelectKeySet(j, selKey.sel4ReadyLed,selKey.sel4ErrLed,selKey.selectMode);
				break;
			case 4:
				pStatus = SelectKeySet(j, selKey.sel5ReadyLed,selKey.sel5ErrLed,selKey.selectMode);
				break;
			case 5:
				pStatus = SelectKeySet(j, selKey.sel6ReadyLed,selKey.sel6ErrLed,selKey.selectMode);
				break;	
		}
		
		
		
		
		//�ް���ʱ����0xfd 0x8 00 00 0x16 0xff 00 0x1c,����0x16����ѡ������豸��,��һ���ֽڴ�������,���û�а��£�����0xff,�а��£�������Ӧ�ĸ���ť���µ�
		if(pStatus == 0)
		{
			key = evbbuf2[5];
			
			if( key != 0xff )
			{
				//TraceSelection("\r\n drvRec2=%x,%x,%x,%x,%x,%x,%x,%x,key=%d",evbbuf2[0],evbbuf2[1],evbbuf2[2],evbbuf2[3],evbbuf2[4],evbbuf2[5],evbbuf2[6],evbbuf2[7],key);
			
				key = GetMiddleSelectKey(evbbuf2[4],key);
			
				TraceSelection("\r\nDrvSelkey = %d",key);

				
				//Buzzer();//�������� chen

				
				
				
				return key;
				
				
			}
			
		}
		OSTimeDly(OS_TICKS_PER_SEC / 100);
	}

	return key;

}



		



/*********************************************************************************************************
** Function name:     	SelectKey_InitProc
** Descriptions:	    ��ʼ��������
** input parameters:   						 
						
** output parameters:   ��
** Returned value: 
*********************************************************************************************************/
void SelectKey_InitProc(void)
{
    
	SelectKeyNo[0] = ID_SELECTION1;
	SelectKeyNo[1] = ID_SELECTION2;
	SelectKeyNo[2] = ID_SELECTION3;
	SelectKeyNo[3] = ID_SELECTION4;
	SelectKeyNo[4] = ID_SELECTION5;
	SelectKeyNo[5] = ID_SELECTION6;
}


/*********************************************************************************************************
** Function name:     	UpdateSelectionLed
** Descriptions:	    ����ѡ��������
** input parameters:   						 
						
** output parameters:   ��
** Returned value: 
*********************************************************************************************************/
void UpdateSelectionLed(MessageKEYPack *AccepterMsg)
{
	selKey.sel1ErrLed = AccepterMsg -> sel1ErrLed;
    selKey.sel1ReadyLed = AccepterMsg -> sel1ReadyLed;
	selKey.sel2ErrLed = AccepterMsg -> sel2ErrLed;
    selKey.sel2ReadyLed = AccepterMsg -> sel2ReadyLed;
	selKey.sel3ErrLed = AccepterMsg -> sel3ErrLed;
    selKey.sel3ReadyLed = AccepterMsg -> sel3ReadyLed;
	selKey.sel4ErrLed = AccepterMsg -> sel4ErrLed;
    selKey.sel4ReadyLed = AccepterMsg -> sel4ReadyLed;
	selKey.sel5ErrLed = AccepterMsg -> sel5ErrLed;
    selKey.sel5ReadyLed = AccepterMsg -> sel5ReadyLed;
	selKey.sel6ErrLed = AccepterMsg -> sel6ErrLed;
    selKey.sel6ReadyLed = AccepterMsg -> sel6ReadyLed;	
	selKey.selectMode = AccepterMsg -> selectMode;	
	TraceSelection("\r\nTaskLed=%d,%x,%x,%x,%x,%x,%x",selKey.selectMode,selKey.sel1ReadyLed,selKey.sel2ReadyLed,selKey.sel3ReadyLed,selKey.sel4ReadyLed,selKey.sel5ReadyLed,selKey.sel6ReadyLed);
}


/*
 

unsigned char UpdateSelectionLed( void )
{
	uint8_t i = 0;
	unsigned char x = 0;
	unsigned char y = 0;
	uint8_t   channel,ChannelX,ChannelY;

    

	if(g_Amount > 0)
	{
		//sysVPMission.sel1ErrLed = 0;
	    sysVPMission.sel1ReadyLed = 0;
		//sysVPMission.sel2ErrLed = 0;
	    sysVPMission.sel2ReadyLed = 0;
		//sysVPMission.sel3ErrLed = 0;
	    sysVPMission.sel3ReadyLed = 0;
		//sysVPMission.sel4ErrLed = 0;
	    sysVPMission.sel4ReadyLed = 0;
		//sysVPMission.sel5ErrLed = 0;
	    sysVPMission.sel5ReadyLed = 0;
		//sysVPMission.sel6ErrLed = 0;
	    sysVPMission.sel6ReadyLed = 0;
		for( i=1; i<=KEYEXTRAVAL; i++ )
	    {
	    
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{	if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1)&&(g_Amount >= Channels[ChannelX][ChannelY].GoodsPrice) )
						{
							sysVPMission.sel1ReadyLed |= 1<<(i-1);							
						}  
					} 
				}  		
			}
			//sysVPMission.sel1ReadyLed |= 1<<i;
	    }
	    for( i=KEYEXTRAVAL+1; i<=KEYEXTRAVAL*2; i++ )
	    {
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1)&&(g_Amount >= Channels[ChannelX][ChannelY].GoodsPrice) )
						{
							sysVPMission.sel2ReadyLed |= 1<<(i-KEYEXTRAVAL-1);						
						}  
					} 
				}  		
			}		    	
	        //sysVPMission.sel2ErrLed |= 1<<(i-KEYEXTRAVAL);
	    }
		for( i=KEYEXTRAVAL*2+1; i<=KEYEXTRAVAL*3; i++ )
	    {
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1)&&(g_Amount >= Channels[ChannelX][ChannelY].GoodsPrice) )
						{
							sysVPMission.sel3ReadyLed |= 1<<(i-KEYEXTRAVAL*2-1);						
						} 
					} 
				}  		
			}				
	    }
		for( i=KEYEXTRAVAL*3+1; i<=KEYEXTRAVAL*4; i++ )
	    {
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1)&&(g_Amount >= Channels[ChannelX][ChannelY].GoodsPrice) )
						{
							sysVPMission.sel4ReadyLed |= 1<<(i-KEYEXTRAVAL*3-1);						
						} 
					} 
				}  		
			}				
	    }
		for( i=KEYEXTRAVAL*4+1; i<=KEYEXTRAVAL*5; i++ )
	    {
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1)&&(g_Amount >= Channels[ChannelX][ChannelY].GoodsPrice) )
						{
							sysVPMission.sel5ReadyLed |= 1<<(i-KEYEXTRAVAL*4-1);						
						} 
					} 
				}  		
			}				
	    }
		for( i=KEYEXTRAVAL*5+1; i<=KEYEXTRAVAL*6; i++ )
	    {
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1)&&(g_Amount >= Channels[ChannelX][ChannelY].GoodsPrice) )
						{
							sysVPMission.sel6ReadyLed |= 1<<(i-KEYEXTRAVAL*5-1);						
						} 
					} 
				}  		
			}				
	    }
		
		sysVPMission.selectMode = 1;//ѡ����������ʱʹ�������;by gzz 20121121
	}	
	else
	{		
		sysVPMission.sel1ErrLed = 0xff;
	    sysVPMission.sel1ReadyLed = 0;
		sysVPMission.sel2ErrLed = 0xff;
	    sysVPMission.sel2ReadyLed = 0;
		sysVPMission.sel3ErrLed = 0xff;
	    sysVPMission.sel3ReadyLed = 0;
		sysVPMission.sel4ErrLed = 0xff;
	    sysVPMission.sel4ReadyLed = 0;
		sysVPMission.sel5ErrLed = 0xff;
	    sysVPMission.sel5ReadyLed = 0;
		sysVPMission.sel6ErrLed = 0xff;
	    sysVPMission.sel6ReadyLed = 0;
	    
		
		for( i=1; i<=KEYEXTRAVAL; i++ )
	    {
	    	
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1) )
						{
							sysVPMission.sel1ReadyLed |= 1<<(i-1);
							sysVPMission.sel1ErrLed &= ~(1<<(i-1));
						}  
					} 
				}  		
			}	
			
			//sysVPMission.sel1ErrLed |= 1<<i;
	    }
	    for( i=KEYEXTRAVAL+1; i<=KEYEXTRAVAL*2; i++ )
	    {
	    	
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1) )
						{
							sysVPMission.sel2ReadyLed |= 1<<(i-KEYEXTRAVAL-1);
							sysVPMission.sel2ErrLed &= ~(1<<(i-KEYEXTRAVAL-1));
						}  
					} 
				}  		
			}	
			
			//sysVPMission.sel2ErrLed |= 1<<(i-KEYEXTRAVAL);
	    }
		for( i=KEYEXTRAVAL*2+1; i<=KEYEXTRAVAL*3; i++ )
	    {
	    	
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1) )
						{
							sysVPMission.sel3ReadyLed |= 1<<(i-KEYEXTRAVAL*2-1);
							sysVPMission.sel3ErrLed &= ~(1<<(i-KEYEXTRAVAL*2-1));
						}  
					} 
				}  		
			}	
			
			//sysVPMission.sel3ErrLed |= 1<<(i-KEYEXTRAVAL*2);
	    }
		for( i=KEYEXTRAVAL*3+1; i<=KEYEXTRAVAL*4; i++ )
	    {
	    	
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1) )
						{
							sysVPMission.sel4ReadyLed |= 1<<(i-KEYEXTRAVAL*3-1);
							sysVPMission.sel4ErrLed &= ~(1<<(i-KEYEXTRAVAL*3-1));
						}  
					} 
				}  		
			}	
			
			//sysVPMission.sel3ErrLed |= 1<<(i-KEYEXTRAVAL*2);
	    }
		for( i=KEYEXTRAVAL*4+1; i<=KEYEXTRAVAL*5; i++ )
	    {
	    	
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1) )
						{
							sysVPMission.sel5ReadyLed |= 1<<(i-KEYEXTRAVAL*4-1);
							sysVPMission.sel5ErrLed &= ~(1<<(i-KEYEXTRAVAL*4-1));
						}  
					} 
				}  		
			}	
			
			//sysVPMission.sel3ErrLed |= 1<<(i-KEYEXTRAVAL*2);
	    }
		for( i=KEYEXTRAVAL*5+1; i<=KEYEXTRAVAL*6; i++ )
	    {
	    	
	    	for(x = 0; x<MAXLAYER;x++)	  
			{					
				for(y=0; y<MAXCHANNEL;y++) 
				{  
					channel = x*10+y+11;
					if(DRV_ChnChk(channel,&ChannelX,&ChannelY) == TRUE) 
					{
						if( (Channels[ChannelX][ChannelY].GoodsCode == i)&&(Channels[ChannelX][ChannelY].GoodsNum > 0)&&((Channels[ChannelX][ChannelY].ChannelStat & 0x03)==1) )
						{
							sysVPMission.sel6ReadyLed |= 1<<(i-KEYEXTRAVAL*5-1);
							sysVPMission.sel6ErrLed &= ~(1<<(i-KEYEXTRAVAL*5-1));
						}  
					} 
				}  		
			}	
			
			//sysVPMission.sel3ErrLed |= 1<<(i-KEYEXTRAVAL*2);
	    }
	    
		sysVPMission.selectMode = 0;//ѡ����������ʱʹ�������;by gzz 20121121
	}	

    return VP_ERR_NULL;
}



*/
