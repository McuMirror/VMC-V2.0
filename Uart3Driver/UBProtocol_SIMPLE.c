/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           UBProtocol.C
** Last modified Date:  2013-03-04
** Last Version:         
** Descriptions:        �ѱ�����������غ�������                     
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
#include "UBProtocol_SIMPLE.h"
#include "..\APP\CHANNEL.h"


//uint8_t  decimalPlaces=1;//������λ1��,2��,0Ԫ
/******************************
//������Ľṹ���ŵ�RAM2�ķ���:
��һ��:
#pragma arm section zidata = "RAM2"
xxxx
#pragma arm section zidata
������һ�ΰ��������ж�������ŵ�RAM2�У�����RAM2�������Լ������
����ȥ�Ķ������ǷŻص�ԭ����RAM��


�ڶ���:�������Ĺ�����->Linker->.\Firmware\SRAM_SCT.sct->Edit ���ļ�
ROM_LOAD 0x00000000
{
    VECTOR 0x00000000//ԭ�ȶ��壬��ʾRO���뵽��ַ0x00000000��
    {
        vector_table.o (+RO)
    }
    ROM_EXEC +0
    {
        *.o (+RO)
    }

    SRAM 0x10000000//ԭ�ȶ��壬��ʾRW,ZI���뵽��ַ0x10000000��
    {
        * (+RW,+ZI)
    }

    //�����Լ������Ķ��壬��ʾUBProtocol.o���ļ����뵽��ַ0x2007c000��,
    //оƬ������16k�ģ�0x2007c000��16k����������
    //0x20080000��16k������usb
    //�������ǰ����ŵ������RAM��
    //�����ڵ�һ���ж��壬���ֻ��һС��ŵ�RAM2��
    //RW_RAM2�������Լ��������,(RAM2)�ǵ�һ���������
	RW_RAM2 0x2007c000 0x00004000
	{
		*.o (RAM2)
	}

    HEAP +0 UNINIT
    {
        rt_sys_s.o (Heap)
    }
}

������:�������Ĺ�����->Target
IRAM1:0x10000000     0x8000
IRAM1:0x2007C000     0x8000
��

�Ѿ������
���Ĳ�:���������������ṹ���Ƿ�ŵ�RAM2��
.Map�ļ��п��Կ���
sysVPMissionSIMPLE                             0x2007c000   Data         560  ubprotocol.o(RAM2)
sysGoodsMatrix                           0x2007c230   Data         496  ubprotocol.o(RAM2)

********************************/
#pragma arm section zidata = "RAM2"

unsigned char SIMPLEg_msg[255]={0};
unsigned char SIMPLEVPMsgBuf[255]={0};

struct VP_MissionSIMPLE sysVPMissionSIMPLE;

#pragma arm section zidata

/*********************************************************************************************************
** Function name:     	SIMPLESIMPLEVPSerialInit
** Descriptions:	    ��ʼ��
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void SIMPLESIMPLEVPSerialInit()
{
	uart3_clr_buf();
	memset( &sysVPMissionSIMPLE.send, 0, sizeof(sysVPMissionSIMPLE.send) );
	memset( &sysVPMissionSIMPLE.receive, 0, sizeof(sysVPMissionSIMPLE.receive) );
}




/*********************************************************************************************************
** Function name:     	crc_check
** Descriptions:	    CRCУ���
** input parameters:    msg��Ҫ���������;len���ݳ���
** output parameters:   ��
** Returned value:      CRC������
*********************************************************************************************************/
unsigned char SIMPLESIMPLEVPBusTxMsg( void )
{
	unsigned char  i   = 0;
//	//unsigned char  data len = 0;
//	//unsigned char  data sum = 0;
//	//unsigned char xdata DataString[255];
//
	
//	//1.caculate the CHK
    memset(SIMPLEg_msg,0x00,sizeof(SIMPLEg_msg));	

	SIMPLEg_msg[0] = sysVPMissionSIMPLE.send.sf;
	SIMPLEg_msg[1] = sysVPMissionSIMPLE.send.msgType;
	SIMPLEg_msg[2] = sysVPMissionSIMPLE.send.datLen;
	for( i = 0; i < sysVPMissionSIMPLE.send.datLen; i++ )
	{
		SIMPLEg_msg[3+i] += sysVPMissionSIMPLE.send.msg[i]; 
	}
	
	sysVPMissionSIMPLE.send.chk = CrcCheck(SIMPLEg_msg,sysVPMissionSIMPLE.send.len);
	//2.send the message
	Uart3PutChar( sysVPMissionSIMPLE.send.sf );
	Uart3PutChar( sysVPMissionSIMPLE.send.msgType );
	Uart3PutChar( sysVPMissionSIMPLE.send.datLen );
    for( i=0; i<sysVPMissionSIMPLE.send.datLen; i++  )
	{
		Uart3PutChar( sysVPMissionSIMPLE.send.msg[i] );
	}
	Uart3PutChar( sysVPMissionSIMPLE.send.chk/256 );
    Uart3PutChar( sysVPMissionSIMPLE.send.chk%256 );
	//Trace("\r\n send = %02x,%02x,%02x,%02x,%02x,%02x",sysVPMissionSIMPLE.send.sf,sysVPMissionSIMPLE.send.len,sysVPMissionSIMPLE.send.verFlag,sysVPMissionSIMPLE.send.msgType,sysVPMissionSIMPLE.send.sn,sysVPMissionSIMPLE.send.datLen); 

	return 1;
}


/*********************************************************************************************************
** Function name:     	changeFailedBeep
** Descriptions:	    ʧ�ܵķ���������
** input parameters:    
** output parameters:   ��
** Returned value:      CRC������
*********************************************************************************************************/
void changeSIMPLEFailedBeep()
{
	Buzzer();
	OSTimeDly(OS_TICKS_PER_SEC/5);
	Buzzer();
}


/*********************************************************************************************************
** Function name:     	SIMPLESIMPLEVPBusFrameUnPack
** Descriptions:	    
** input parameters:    msg��Ҫ���������;len���ݳ���
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
unsigned char SIMPLESIMPLEVPBusFrameUnPack( void )
{
	unsigned char i=0, k=0, m=0;
	unsigned char len = 0;
	unsigned int  sum = 0;
	unsigned char reclen=0;
	
	while(Uart3BuffIsNotEmpty() == 1)
	{		
		for( i=0; i<sizeof( SIMPLEVPMsgBuf )-1; i++ )
		{
			SIMPLEVPMsgBuf[i] = SIMPLEVPMsgBuf[i+1];
		}
		SIMPLEVPMsgBuf[sizeof(SIMPLEVPMsgBuf)-1] = Uart3GetCh();	
		reclen=1;
	}
	
		
		
	
	if(reclen)
	{		
		for( i=0; i<=sizeof(SIMPLEVPMsgBuf); i++ )
		{  	
			//Check the SF
			if( SIMPLEVPMsgBuf[i] != VPSIMPLE_SF ) 
			{
				continue;
			}
			//Check the len
			len = SIMPLEVPMsgBuf[i+2]+3;
			TracePC("\r\n Drv rec=%d",len); 
			//Check the CHK
			sum = 0;
			sum = CrcCheck(SIMPLEVPMsgBuf + i, len);
			if( (SIMPLEVPMsgBuf[i+len] != sum/256)||(SIMPLEVPMsgBuf[i+len+1] != sum%256)	)
			{
				break;
		    }
			TracePC("\r\n Drv rec=chk"); 
			OSTimeDly(20);
			//Check the message type
	        if( !((SIMPLEVPMsgBuf[i+1]>=VP_MT_MIN_RECEIVESIMPLE) && (SIMPLEVPMsgBuf[i+1]<=VP_MT_MAX_RECEIVESIMPLE)) )   
	            continue;  
			TracePC("\r\n Drv rec=type"); 
			OSTimeDly(20);
			//Save the message
			sysVPMissionSIMPLE.receive.sf      = SIMPLEVPMsgBuf[i];
			sysVPMissionSIMPLE.receive.msgType = SIMPLEVPMsgBuf[i+1];
			sysVPMissionSIMPLE.receive.datLen  = SIMPLEVPMsgBuf[i+2];
			sysVPMissionSIMPLE.receive.len	   = sysVPMissionSIMPLE.receive.datLen+5;
	 	    

	        
			TracePC("\r\n Drv rec1=%02x,%02x,%02x,",sysVPMissionSIMPLE.receive.sf,sysVPMissionSIMPLE.receive.msgType,sysVPMissionSIMPLE.receive.datLen); 
			OSTimeDly(20);
			for( m=0,k=i+3; k<i+3+sysVPMissionSIMPLE.receive.datLen; m++,k++)
			{			
				sysVPMissionSIMPLE.receive.msg[m] = SIMPLEVPMsgBuf[k];	
				TracePC("\r\n Drv k=%d,m=%d,k<%d,msg=%02x",k,m,(3+sysVPMissionSIMPLE.receive.datLen),sysVPMissionSIMPLE.receive.msg[m]);
			}
			//TracePC("\r\n Drv rec2=%02x,%02x,%02x,%02x,%02x",sysVPMissionSIMPLE.receive.msg[0],sysVPMissionSIMPLE.receive.msg[1],sysVPMissionSIMPLE.receive.msg[2],sysVPMissionSIMPLE.receive.msg[3],sysVPMissionSIMPLE.receive.msg[4]); 
			OSTimeDly(20);
			TracePC("\r\n Drv rec2=%02x,%02x",SIMPLEVPMsgBuf[k],SIMPLEVPMsgBuf[k+1]);
			sysVPMissionSIMPLE.receive.chk = sysVPMissionSIMPLE.receive.msg[k]*256 + sysVPMissionSIMPLE.receive.msg[k+1];
			memset( SIMPLEVPMsgBuf, 0, sizeof(SIMPLEVPMsgBuf) );
			//TracePC("\r\n Drv rec3=%d",sysVPMissionSIMPLE.receive.chk); //OSTimeDly(20);
			return 1;
		}	
	}
	memset( SIMPLEVPMsgBuf, 0, sizeof(SIMPLEVPMsgBuf) );
	return 0;
}

/*********************************************************************************************************
** Function name:     	VPMsgPackSend
** Descriptions:	    ��װ���ķ�������
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
unsigned char SIMPLESIMPLEVPMsgPackSend( unsigned char msgType )
{
//    
    uint8_t i=0,j=0,hdnum=0,ceng=0,hdstate=0;     
//	uint16_t tempMoney;
//	uint8_t tempSend=0;
//	uint32_t tradeMoney=0;
//	uint8_t cabinet=0,cabno=0;
//	//u_char xdata len = 0;
//
	if( msgType>VP_MT_MAX_SENDSIMPLE )
		return VP_ERRSIMPLE_PAR;

//
	switch( msgType )
	{
		case SIMPLE_INFO_RPT:
		    {
				i = 0;
				sysVPMissionSIMPLE.send.msgType = SIMPLE_INFO_RPT;
				sysVPMissionSIMPLE.send.msg[i++] = sysVPMissionSIMPLE.receive.msg[0]; 
				switch(sysVPMissionSIMPLE.receive.msg[0])
				{
					//��ȡ�ۻ������Ա���
					case 0:
						sysVPMissionSIMPLE.send.msg[i++] = 0x16; //flag1
						sysVPMissionSIMPLE.send.msg[i++] = 0;    //flag2
						sysVPMissionSIMPLE.send.msg[i++] = hd_nums_by_id(1);    //��������
						sysVPMissionSIMPLE.send.msg[i++] = 0;    //չʾλ����
						sysVPMissionSIMPLE.send.msg[i++] = 51;    //����汾�ַ�������
						sysVPMissionSIMPLE.send.msg[i++]  = 'e';
						sysVPMissionSIMPLE.send.msg[i++]  = 'a';
						sysVPMissionSIMPLE.send.msg[i++]  = 's';
						sysVPMissionSIMPLE.send.msg[i++]  = 'i';
						sysVPMissionSIMPLE.send.msg[i++]  = 'v';
						sysVPMissionSIMPLE.send.msg[i++]  = 'e';
						sysVPMissionSIMPLE.send.msg[i++]  = 'n';
						sysVPMissionSIMPLE.send.msg[i++]  = 'd';
						sysVPMissionSIMPLE.send.msg[i++]  = '_';
						sysVPMissionSIMPLE.send.msg[i++]  = '8';
						sysVPMissionSIMPLE.send.msg[i++]  = 'X';
						sysVPMissionSIMPLE.send.msg[i++]  = '6';
						sysVPMissionSIMPLE.send.msg[i++]  = ':';
						sysVPMissionSIMPLE.send.msg[i++]  = 'T';
						sysVPMissionSIMPLE.send.msg[i++]  = 'a';
						sysVPMissionSIMPLE.send.msg[i++]  = 'n';
						sysVPMissionSIMPLE.send.msg[i++]  = 'H';
						sysVPMissionSIMPLE.send.msg[i++]  = 'u';
						sysVPMissionSIMPLE.send.msg[i++]  = 'a';
						sysVPMissionSIMPLE.send.msg[i++]  = 'n';
						sysVPMissionSIMPLE.send.msg[i++]  = 'g';
						sysVPMissionSIMPLE.send.msg[i++]  = ':';
						sysVPMissionSIMPLE.send.msg[i++]  = 'T';
						sysVPMissionSIMPLE.send.msg[i++]  = 'o';
						sysVPMissionSIMPLE.send.msg[i++]  = 'u';
						sysVPMissionSIMPLE.send.msg[i++]  = 'M';
						sysVPMissionSIMPLE.send.msg[i++]  = 'i';
						sysVPMissionSIMPLE.send.msg[i++]  = 'n';
						sysVPMissionSIMPLE.send.msg[i++]  = 'g';
						sysVPMissionSIMPLE.send.msg[i++]  = '_';
						sysVPMissionSIMPLE.send.msg[i++]  = 'Z';
						sysVPMissionSIMPLE.send.msg[i++]  = 'h';
						sysVPMissionSIMPLE.send.msg[i++]  = 'i';
						sysVPMissionSIMPLE.send.msg[i++]  = 'L';
						sysVPMissionSIMPLE.send.msg[i++]  = 'e';
						sysVPMissionSIMPLE.send.msg[i++]  = 'n';
						sysVPMissionSIMPLE.send.msg[i++]  = 'g';
						sysVPMissionSIMPLE.send.msg[i++]  = '_';
						sysVPMissionSIMPLE.send.msg[i++]  = 'A';
						sysVPMissionSIMPLE.send.msg[i++]  = 'p';
						sysVPMissionSIMPLE.send.msg[i++]  = 'r';
						sysVPMissionSIMPLE.send.msg[i++]  = 'i';
						sysVPMissionSIMPLE.send.msg[i++]  = 'l';
						sysVPMissionSIMPLE.send.msg[i++]  = ' ';
						sysVPMissionSIMPLE.send.msg[i++]  = '1';
						sysVPMissionSIMPLE.send.msg[i++]  = '0';
						sysVPMissionSIMPLE.send.msg[i++]  = ' ';
						sysVPMissionSIMPLE.send.msg[i++]  = '2';
						sysVPMissionSIMPLE.send.msg[i++]  = '0';
						sysVPMissionSIMPLE.send.msg[i++]  = '1';
						sysVPMissionSIMPLE.send.msg[i++]  = '5';
						break;
					case 1:
						sysVPMissionSIMPLE.send.msg[i++]  = 0;//flag	
						hdnum=hd_nums_by_id(1);    //��������
						if(hdnum%8==0)
							ceng  = hdnum/8;//�������channel���ֽ���	
						else
							ceng  = hdnum/8+1;//�������channel���ֽ���	
						sysVPMissionSIMPLE.send.msg[i++]  = ceng;//�������channel���ֽ���	

						for(j=0;j<ceng;j++)
						{
							hdstate=0;
							if((j*8+1)<=hdnum)
							{
								if(hd_state_by_id(1,(j*8+1)))
								{
									hdstate|=0x01;
								}
							}
							if((j*8+2)<=hdnum)
							{
								if(hd_state_by_id(1,(j*8+2)))
								{
									hdstate|=0x02;
								}
							}
							if((j*8+3)<=hdnum)
							{
								if(hd_state_by_id(1,(j*8+3)))
								{
									hdstate|=0x04;
								}
							}
							if((j*8+4)<=hdnum)
							{
								if(hd_state_by_id(1,(j*8+4)))
								{
									hdstate|=0x08;
								}
							}
							if((j*8+5)<=hdnum)
							{
								if(hd_state_by_id(1,(j*8+5)))
								{
									hdstate|=0x10;
								}
							}
							if((j*8+6)<=hdnum)
							{
								if(hd_state_by_id(1,(j*8+6)))
								{
									hdstate|=0x20;
								}
							}
							if((j*8+7)<=hdnum)
							{
								if(hd_state_by_id(1,(j*8+7)))
								{
									hdstate|=0x40;
								}
							}
							if((j*8+8)<=hdnum)
							{
								if(hd_state_by_id(1,(j*8+8)))
								{
									hdstate|=0x80;
								}
							}
							sysVPMissionSIMPLE.send.msg[i++]  = hdstate;//�����ۿ���Ϣ
						}						
						break;
				}
				sysVPMissionSIMPLE.send.datLen = i; 
			}
			break;
		case SIMPLE_BUTTON_RPT:	
			{
				i = 0;
				sysVPMissionSIMPLE.send.msgType = SIMPLE_BUTTON_RPT;
				sysVPMissionSIMPLE.send.msg[i++] = 1;
				sysVPMissionSIMPLE.send.msg[i++] = sysVPMissionSIMPLE.btnchannel_id;
				sysVPMissionSIMPLE.send.datLen = i; 
			}	
			break;	
		case SIMPLE_VEND_RPT:
		    {
				i = 0;
				sysVPMissionSIMPLE.send.msgType = SIMPLE_VEND_RPT;
				sysVPMissionSIMPLE.send.msg[i++] = sysVPMissionSIMPLE.channel_id;
				sysVPMissionSIMPLE.send.msg[i++] = sysVPMissionSIMPLE.channel_result;
				sysVPMissionSIMPLE.send.msg[i++] = sysVPMissionSIMPLE.SN;
				sysVPMissionSIMPLE.send.datLen = i; 
			}
			break;
		case SIMPLE_ADMIN_RPT:
		    {
				i = 0;
			    sysVPMissionSIMPLE.send.msgType =  SIMPLE_ADMIN_RPT;
				sysVPMissionSIMPLE.send.msg[i++] = sysVPMissionSIMPLE.admintype;	
				//1����ȫ��������2������ܻ�����3������������4�����ʱ�䣬5�Զ��˱�ʱ��
				switch(sysVPMissionSIMPLE.admintype)
				{
					case 1:
						sysVPMissionSIMPLE.send.msg[i++] = 0;
						sysVPMissionSIMPLE.send.msg[i++] = 0;						
						break;
					case 2:
						sysVPMissionSIMPLE.send.msg[i++] = sysVPMissionSIMPLE.admincolumn;
						sysVPMissionSIMPLE.send.msg[i++] = 0;						
						break;
					case 3:
						sysVPMissionSIMPLE.send.msg[i++] = sysVPMissionSIMPLE.admincolumn;
						sysVPMissionSIMPLE.send.msg[i++] = sysVPMissionSIMPLE.admincolumnsum;						
						break;
					case 4:
					case 5:	
						sysVPMissionSIMPLE.send.msg[i++] = sysVPMissionSIMPLE.admincolumnsum;	
						sysVPMissionSIMPLE.send.msg[i++] = 0;	
						break;		
				}
				sysVPMissionSIMPLE.send.datLen = i; 
				
			}
			break; 
		case SIMPLE_GET_ADMIN:
			{
				i = 0;
			    sysVPMissionSIMPLE.send.msgType =  SIMPLE_GET_ADMIN;
				sysVPMissionSIMPLE.send.msg[i++] = sysVPMissionSIMPLE.getadmintype;	
				//3�����ĳһ��������ŵ���Ʒʣ����,4��ȡ��ǰ�����ʱ��,5��ȡ��ǰ�Զ��˱�ʱ��
				switch(sysVPMissionSIMPLE.getadmintype)
				{
					case 3:
						sysVPMissionSIMPLE.send.msg[i++] = sysVPMissionSIMPLE.getadmincolumn;										
						break;
					case 4:
						sysVPMissionSIMPLE.send.msg[i++] = 0;										
						break;
					case 5:
						sysVPMissionSIMPLE.send.msg[i++] = 0;										
						break;	
				}
				sysVPMissionSIMPLE.send.datLen = i; 
			}
			break;
		default: break;

	}
	sysVPMissionSIMPLE.send.sf  = VPSIMPLE_SF;	 
    sysVPMissionSIMPLE.send.len = sysVPMissionSIMPLE.send.datLen + 3;		   	
	
    SIMPLESIMPLEVPBusTxMsg();    
	return VP_ERRSIMPLE_NULL;
}

/*********************************************************************************************************
** Function name:     	VPMissionSIMPLE_Setup_RPT
** Descriptions:	    ����ʱ�ϱ�������Ϣ
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
unsigned char VPMissionSIMPLE_Setup_RPT( void )
{
	SIMPLESIMPLEVPMsgPackSend( SIMPLE_INFO_RPT);
	return VP_ERRSIMPLE_NULL;
}

/*********************************************************************************************************
** Function name:     	VP_Vendout_Ind
** Descriptions:	    PCָʾVMC��������
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
unsigned char VPSIMPLE_Vendout_Ind( void )
{
	//MessageSIMPLEUboxPCPack *AccepterSIMPLEUboxMsg;
	//unsigned char ComStatus;


	//SN��ͬʱ�������³���
	if((sysVPMissionSIMPLE.SN  == sysVPMissionSIMPLE.receive.msg[1])&&(sysVPMissionSIMPLE.SN  > 0))
	{
		VPMissionSIMPLE_Vendout_RPT(sysVPMissionSIMPLE.channel_result);
	}
	else
	{
		//1.�õ���������	
		sysVPMissionSIMPLE.channel_id = sysVPMissionSIMPLE.receive.msg[0];	
	    sysVPMissionSIMPLE.SN  = sysVPMissionSIMPLE.receive.msg[1];
		TracePC("\r\n Drv vendind=%d,%d",sysVPMissionSIMPLE.channel_id,sysVPMissionSIMPLE.SN); 
		
		//���������vmc
		MsgSIMPLEUboxPack[g_SIMPLEUbox_Index].PCTOVMCCmd = MBOX_SIMPLEPCTOVMC_VENDIND;	
		MsgSIMPLEUboxPack[g_SIMPLEUbox_Index].channel_id = sysVPMissionSIMPLE.channel_id;
		OSQPost(g_SIMPLEUbox_PCTOVMCQ,&MsgSIMPLEUboxPack[g_SIMPLEUbox_Index]);	
		UpdateSIMPLEIndex();
		OSTimeSet(0);
		TracePC("\r\n Drv %dVendout_Ind",OSTimeGet()); 
	}
    return VP_ERRSIMPLE_NULL;	
}

/*********************************************************************************************************
** Function name:     	VPSIMPLE_Display_Ind
** Descriptions:	    PCָʾVMC��ʾ������ʱ���ָ��
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
unsigned char VPSIMPLE_Display_Ind( void )
{
	//MessageSIMPLEUboxPCPack *AccepterSIMPLEUboxMsg;
	unsigned char distype=0;	
	unsigned char bai,shi,ge,mao,hourh,hourl,minh,minl;
	//1.�õ���ʾ����
	distype=(sysVPMissionSIMPLE.receive.msg[0]>>5)&0x07;
	sysVPMissionSIMPLE.distype=distype;		
	TracePC("\r\n Drv distype=%d",sysVPMissionSIMPLE.distype);
	//0�û���1ʱ�䣬2�ı���3��Ʒ���� 
	switch(sysVPMissionSIMPLE.distype)
	{
		case 0:
		case 3:	
			bai=(sysVPMissionSIMPLE.receive.msg[1]>0x30)?(sysVPMissionSIMPLE.receive.msg[1]-0x30):0;
			shi=(sysVPMissionSIMPLE.receive.msg[2]>0x30)?(sysVPMissionSIMPLE.receive.msg[2]-0x30):0;
			ge=(sysVPMissionSIMPLE.receive.msg[3]>0x30)?(sysVPMissionSIMPLE.receive.msg[3]-0x30):0;
			mao=(sysVPMissionSIMPLE.receive.msg[5]>0x30)?(sysVPMissionSIMPLE.receive.msg[5]-0x30):0;
			sysVPMissionSIMPLE.payInMoney=bai*10000+shi*1000+ge*100+mao*10;
			TracePC("\r\n Drv dismoney=%ld",sysVPMissionSIMPLE.payInMoney);
			//���������vmc
			MsgSIMPLEUboxPack[g_SIMPLEUbox_Index].PCTOVMCCmd = MBOX_SIMPLEPCTOVMC_DISPLAYIND;	
			MsgSIMPLEUboxPack[g_SIMPLEUbox_Index].distype = sysVPMissionSIMPLE.distype;
			MsgSIMPLEUboxPack[g_SIMPLEUbox_Index].payInMoney = sysVPMissionSIMPLE.payInMoney;
			OSQPost(g_SIMPLEUbox_PCTOVMCQ,&MsgSIMPLEUboxPack[g_SIMPLEUbox_Index]);	
			UpdateSIMPLEIndex();
			break;
		case 1:
			hourh=(sysVPMissionSIMPLE.receive.msg[1]>0x30)?(sysVPMissionSIMPLE.receive.msg[1]-0x30):0;
			hourl=(sysVPMissionSIMPLE.receive.msg[2]>0x30)?(sysVPMissionSIMPLE.receive.msg[2]-0x30):0;
			minh=(sysVPMissionSIMPLE.receive.msg[4]>0x30)?(sysVPMissionSIMPLE.receive.msg[4]-0x30):0;
			minl=(sysVPMissionSIMPLE.receive.msg[5]>0x30)?(sysVPMissionSIMPLE.receive.msg[5]-0x30):0;
			RTCData.hour=hourh*10+hourl;
			RTCData.minute=minh*10+minl;	
			TracePC("\r\n Drv distime=%d:%d",RTCData.hour,RTCData.minute);
			RTCSetTime(&RTCData);
			break;
	}
	
	//���������vmc
	//MsgSIMPLEUboxPack[g_SIMPLEUbox_Index].PCTOVMCCmd = MBOX_SIMPLEPCTOVMC_VENDIND;	
	//MsgSIMPLEUboxPack[g_SIMPLEUbox_Index].channel_id = sysVPMissionSIMPLE.channel_id;
	//OSQPost(g_SIMPLEUbox_PCTOVMCQ,&MsgSIMPLEUboxPack[g_SIMPLEUbox_Index]);	
	//UpdateSIMPLEIndex();
    return VP_ERRSIMPLE_NULL;	
}



/*********************************************************************************************************
** Function name:     	VPMissionSIMPLE_Poll
** Descriptions:	    ��ѯPC��
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
unsigned char VPMissionSIMPLESIMPLE_Poll()
{
    //unsigned char recRes=0;
		
	TracePC("\r\n Drv Pollsystem1"); 
	if( SIMPLESIMPLEVPBusFrameUnPack() )
	{ 	
		TracePC("\r\n Drv rec=ok"); 
		//recRes = 1;	 		
	}			

    switch( sysVPMissionSIMPLE.receive.msgType )
	{
		case SIMPLE_GET_INFO: 
			VPMissionSIMPLE_Setup_RPT();			
			break;
		case SIMPLE_VEND_IND:
		    VPSIMPLE_Vendout_Ind();
		    break;
		case SIMPLE_DISPLAY_IND: 
			VPSIMPLE_Display_Ind();			
			break;	
	}
	memset( &sysVPMissionSIMPLE.receive, 0, sizeof(sysVPMissionSIMPLE.receive) );
	return VP_ERRSIMPLE_NULL;
}

/*********************************************************************************************************
** Function name:     	VPMissionSIMPLE_Button_RPT
** Descriptions:	    �ϱ�������Ϣ
** input parameters:    type=1С����ѡ�����,4�˱�,0��Ϸ����
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
unsigned char VPMissionSIMPLE_Button_RPT( uint8_t btnchannel_id )
{
    //unsigned char flag = 0; 	
    //-------------------------------------------
    sysVPMissionSIMPLE.btnchannel_id = btnchannel_id;
	SIMPLESIMPLEVPMsgPackSend( SIMPLE_BUTTON_RPT);
	return VP_ERRSIMPLE_NULL;
}



/*********************************************************************************************************
** Function name:       VPMissionSIMPLE_Vendout_RPT
** Descriptions:        ��PC�ϱ�������Ϣ
** input parameters:    channel_result=0�����ɹ�,1������ʧ��                        
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
unsigned char VPMissionSIMPLE_Vendout_RPT( uint8_t channel_result )
{					     	
    //-------------------------------------------
    sysVPMissionSIMPLE.channel_result = channel_result;
	//������������
	if(channel_result==1)
		changeSIMPLEFailedBeep();
	SIMPLESIMPLEVPMsgPackSend( SIMPLE_VEND_RPT);
	return VP_ERRSIMPLE_NULL;
}

/*********************************************************************************************************
** Function name:       VPMissionSIMPLE_Get_Admin
** Descriptions:        ��PC���������Ϣ
** input parameters:    type
                        
						
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
unsigned char VPMissionSIMPLE_Get_Admin( unsigned char admintype,unsigned char hdstart)
{
	unsigned char recRes=0;
	unsigned char recAck=VP_ERRSIMPLE_PAR;	
	//-------------------------------------------
	sysVPMissionSIMPLE.getadmintype = admintype;
	
	//TracePC("\r\n Drv Admin=%d,%d,%d",sysVPMissionSIMPLE.admintype,sysVPMissionSIMPLE.admincolumn,sysVPMissionSIMPLE.admincolumnsum);	
	//3�����ĳһ��������ŵ���Ʒʣ����,4��ȡ��ǰ�����ʱ��,5��ȡ��ǰ�Զ��˱�ʱ��
	switch(sysVPMissionSIMPLE.getadmintype)
	{
		case 3:		
			sysVPMissionSIMPLE.getadmincolumn = hdstart;
			SIMPLESIMPLEVPMsgPackSend( SIMPLE_GET_ADMIN);		
			Timer.PCRecTimer = VP_TIMSIMPLEE_OUT;
			while( Timer.PCRecTimer )
			{
				if( SIMPLESIMPLEVPBusFrameUnPack() )
				{ 	
					TracePC("\r\n Drv rec=ok"); 
					recRes = 1;	
					break;
				}		
			}
			if( recRes== 0 )
			{

				TracePC("\r\n Drv UboxGetAdminTimeFail");
		        recAck= VP_ERRSIMPLE_COM;
			}
			else
			{
				if( sysVPMissionSIMPLE.receive.msgType==SIMPLE_ADMIN_IND )
				{						
					TracePC("\r\n Drv UboxGetAdminSuchd=%d,sum=%d",sysVPMissionSIMPLE.receive.msg[1],sysVPMissionSIMPLE.receive.msg[2]);
					hd_setNums_by_id(1,sysVPMissionSIMPLE.receive.msg[1],sysVPMissionSIMPLE.receive.msg[2]);
					recAck= VP_ERRSIMPLE_NULL;
				}
				
			}
			//memset( &sysVPMissionSIMPLE.receive, 0, sizeof(sysVPMissionSIMPLE.receive) );			
			break;
		case 4:		
			SIMPLESIMPLEVPMsgPackSend( SIMPLE_GET_ADMIN);		
			Timer.PCRecTimer = VP_TIMSIMPLEE_OUT;
			while( Timer.PCRecTimer )
			{
				if( SIMPLESIMPLEVPBusFrameUnPack() )
				{ 	
					TracePC("\r\n Drv rec=ok"); 
					recRes = 1;	
					break;
				}		
			}
			if( recRes== 0 )
			{

				TracePC("\r\n Drv UboxGetAdminTimeFail");
		        recAck= VP_ERRSIMPLE_COM;
			}
			else
			{
				if( sysVPMissionSIMPLE.receive.msgType==SIMPLE_ADMIN_IND )
				{						
					TracePC("\r\n Drv UboxGetAdminSuchd=%d",sysVPMissionSIMPLE.receive.msg[1]);
					SystemPara.ColumnTime=sysVPMissionSIMPLE.receive.msg[1];
					recAck= VP_ERRSIMPLE_NULL;
				}
				
			}
			//memset( &sysVPMissionSIMPLE.receive, 0, sizeof(sysVPMissionSIMPLE.receive) );			
			break;	
		case 5:		
			SIMPLESIMPLEVPMsgPackSend( SIMPLE_GET_ADMIN);		
			Timer.PCRecTimer = VP_TIMSIMPLEE_OUT;
			while( Timer.PCRecTimer )
			{
				if( SIMPLESIMPLEVPBusFrameUnPack() )
				{ 	
					TracePC("\r\n Drv rec=ok"); 
					recRes = 1;	
					break;
				}		
			}
			if( recRes== 0 )
			{

				TracePC("\r\n Drv UboxGetAdminTimeFail");
		        recAck= VP_ERRSIMPLE_COM;
			}
			else
			{
				if( sysVPMissionSIMPLE.receive.msgType==SIMPLE_ADMIN_IND )
				{						
					TracePC("\r\n Drv UboxGetAdminSuchd=%d",sysVPMissionSIMPLE.receive.msg[1]);
					SystemPara.SaleTime=sysVPMissionSIMPLE.receive.msg[1];
					recAck= VP_ERRSIMPLE_NULL;
				}
				
			}
			//memset( &sysVPMissionSIMPLE.receive, 0, sizeof(sysVPMissionSIMPLE.receive) );			
			break;		
	}
	//���������vmc
	MsgSIMPLEUboxPack[g_SIMPLEUbox_Index].PCTOVMCCmd = MBOX_SIMPLEVMCTOPC_RESULTIND;	
	MsgSIMPLEUboxPack[g_SIMPLEUbox_Index].adminresult = recAck;
	OSQPost(g_SIMPLEUbox_PCTOVMCQ,&MsgSIMPLEUboxPack[g_SIMPLEUbox_Index]);	
	UpdateSIMPLEIndex();
	memset( &sysVPMissionSIMPLE.receive, 0, sizeof(sysVPMissionSIMPLE.receive) );
	return recAck;

}

/*********************************************************************************************************
** Function name:       VPMissionSIMPLE_Set_Admin
** Descriptions:        ��PC���������Ϣ,���ұ��������vmc
** input parameters:    type
                        
						
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
unsigned char VPMissionSIMPLE_Set_Admin( unsigned char admintype,unsigned char hdstart,unsigned char hdend)
{
	unsigned char recRes=0;
	unsigned char recAck=VP_ERRSIMPLE_PAR;
	unsigned char hdnum=0,i;
	//-------------------------------------------
	sysVPMissionSIMPLE.getadmintype = admintype;
	//TracePC("\r\n Drv Admin=%d,%d,%d",sysVPMissionSIMPLE.admintype,sysVPMissionSIMPLE.admincolumn,sysVPMissionSIMPLE.admincolumnsum);	
	//3�����ĳһ��������ŵ���Ʒʣ����,4���õ�ǰ�����ʱ��,5���õ�ǰ�Զ��˱�ʱ��
	switch(sysVPMissionSIMPLE.getadmintype)
	{
		case 3:			
			hdnum=hdend;
			for(i=hdstart;i<=hdnum;i++)
			{
				recRes = 0;	
				sysVPMissionSIMPLE.getadmincolumn = i;
				SIMPLESIMPLEVPMsgPackSend( SIMPLE_GET_ADMIN);		
				Timer.PCRecTimer = VP_TIMSIMPLEE_OUT;
				while( Timer.PCRecTimer )
				{
					if( SIMPLESIMPLEVPBusFrameUnPack() )
					{ 	
						TracePC("\r\n Drv rec=ok"); 
						recRes = 1;	
						break;
					}		
				}
				if( recRes== 0 )
				{

					TracePC("\r\n Drv UboxGetAdminTimeFail");
			        recAck= VP_ERRSIMPLE_COM;
				}
				else
				{
					if( sysVPMissionSIMPLE.receive.msgType==SIMPLE_ADMIN_IND )
					{						
						TracePC("\r\n Drv UboxGetAdminSuchd=%d,sum=%d",sysVPMissionSIMPLE.receive.msg[1],sysVPMissionSIMPLE.receive.msg[2]);
						hd_setNums_by_id(1,sysVPMissionSIMPLE.receive.msg[1],sysVPMissionSIMPLE.receive.msg[2]);
						recAck= VP_ERRSIMPLE_NULL;
					}
					
				}
				memset( &sysVPMissionSIMPLE.receive, 0, sizeof(sysVPMissionSIMPLE.receive) );
			}
			break;
	}
	return recAck;

}

/*********************************************************************************************************
** Function name:       VPMissionSIMPLE_Admin_RPT
** Descriptions:        ��PC�ϱ�Admin��Ϣ
** input parameters:    admintype:1����ȫ��������2������ܻ�����3������������4�����ʱ�䣬5�Զ��˱�ʱ��
                        
						
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
unsigned char VPMissionSIMPLE_Admin_RPT( unsigned char admintype,uint8_t admincolumn,uint8_t admincolumnsum)
{
	unsigned char recRes=0;
	unsigned char recAck=VP_ERRSIMPLE_PAR;
	unsigned char hdstart=0, hdend=0;
	//-------------------------------------------
	sysVPMissionSIMPLE.admintype = admintype;
	//vmc�ϱ���PC����admin������1����ȫ��������2������ܻ�����3������������4�����ʱ�䣬5�Զ��˱�ʱ��
	switch(sysVPMissionSIMPLE.admintype)
	{
		case 1:
			sysVPMissionSIMPLE.admincolumn = admincolumn;
			sysVPMissionSIMPLE.admincolumnsum = admincolumnsum;
			//TracePC("\r\n Drv Admin=%d,%d,%d",sysVPMissionSIMPLE.admintype,sysVPMissionSIMPLE.admincolumn,sysVPMissionSIMPLE.admincolumnsum);	
			SIMPLESIMPLEVPMsgPackSend( SIMPLE_ADMIN_RPT);



			Timer.PCRecTimer = VP_TIMSIMPLEE_OUT;
			while( Timer.PCRecTimer )
			{
				if( SIMPLESIMPLEVPBusFrameUnPack() )
				{ 	
					TracePC("\r\n Drv rec=ok"); 					
					recRes = 1;	
					break;
				}		
			}
			if( recRes== 0 )
			{

				TracePC("\r\n Drv UboxAdminTimeFail");
		        recAck= VP_ERRSIMPLE_COM;
			}
			else
			{
				if( sysVPMissionSIMPLE.receive.msgType==SIMPLE_RESULT_IND )
				{
					if(sysVPMissionSIMPLE.receive.msg[0]>0)
					{
						TracePC("\r\n Drv UboxAdminAckFail");
						 recAck= VP_ERRSIMPLE_PAR;
					}
					else
					{				
						TracePC("\r\n Drv UboxAdminSuc");
						VPMissionSIMPLE_Set_Admin(3,1,hd_nums_by_id(1));
						recAck= VP_ERRSIMPLE_NULL;
					}
				}
				
			}	
			break;
		case 2:
			sysVPMissionSIMPLE.admincolumn = admincolumn;
			sysVPMissionSIMPLE.admincolumnsum = admincolumnsum;
			//TracePC("\r\n Drv Admin=%d,%d,%d",sysVPMissionSIMPLE.admintype,sysVPMissionSIMPLE.admincolumn,sysVPMissionSIMPLE.admincolumnsum);	
			SIMPLESIMPLEVPMsgPackSend( SIMPLE_ADMIN_RPT);



			Timer.PCRecTimer = VP_TIMSIMPLEE_OUT;
			while( Timer.PCRecTimer )
			{
				if( SIMPLESIMPLEVPBusFrameUnPack() )
				{ 	
					TracePC("\r\n Drv rec=ok"); 					
					recRes = 1;	
					break;
				}		
			}
			if( recRes== 0 )
			{

				TracePC("\r\n Drv UboxAdminTimeFail");
		        recAck= VP_ERRSIMPLE_COM;
			}
			else
			{
				if( sysVPMissionSIMPLE.receive.msgType==SIMPLE_RESULT_IND )
				{
					if(sysVPMissionSIMPLE.receive.msg[0]>0)
					{
						TracePC("\r\n Drv UboxAdminAckFail");
						 recAck= VP_ERRSIMPLE_PAR;
					}
					else
					{				
						TracePC("\r\n Drv UboxAdminSuc");
						hd_ids_by_level(1,sysVPMissionSIMPLE.admincolumn,&hdstart,&hdend);
						VPMissionSIMPLE_Set_Admin(3,hdstart,hdend);
						recAck= VP_ERRSIMPLE_NULL;
					}
				}
				
			}	
			break;	
		case 3:
			sysVPMissionSIMPLE.admincolumn = admincolumn;
			sysVPMissionSIMPLE.admincolumnsum = admincolumnsum;
			//TracePC("\r\n Drv Admin=%d,%d,%d",sysVPMissionSIMPLE.admintype,sysVPMissionSIMPLE.admincolumn,sysVPMissionSIMPLE.admincolumnsum);	
			SIMPLESIMPLEVPMsgPackSend( SIMPLE_ADMIN_RPT);



			Timer.PCRecTimer = VP_TIMSIMPLEE_OUT;
			while( Timer.PCRecTimer )
			{
				if( SIMPLESIMPLEVPBusFrameUnPack() )
				{ 	
					TracePC("\r\n Drv rec=ok"); 					
					recRes = 1;	
					break;
				}		
			}
			if( recRes== 0 )
			{

				TracePC("\r\n Drv UboxAdminTimeFail");
		        recAck= VP_ERRSIMPLE_COM;
			}
			else
			{
				if( sysVPMissionSIMPLE.receive.msgType==SIMPLE_RESULT_IND )
				{
					if(sysVPMissionSIMPLE.receive.msg[0]>0)
					{
						TracePC("\r\n Drv UboxAdminAckFail");
						 recAck= VP_ERRSIMPLE_PAR;
					}
					else
					{				
						TracePC("\r\n Drv UboxAdminSuc");						
						recAck= VP_ERRSIMPLE_NULL;
					}
				}
				
			}	
			break;	
		case 4:
		case 5:	
			sysVPMissionSIMPLE.admincolumn = admincolumn;
			sysVPMissionSIMPLE.admincolumnsum = admincolumnsum;
			//TracePC("\r\n Drv Admin=%d,%d,%d",sysVPMissionSIMPLE.admintype,sysVPMissionSIMPLE.admincolumn,sysVPMissionSIMPLE.admincolumnsum);	
			SIMPLESIMPLEVPMsgPackSend( SIMPLE_ADMIN_RPT);



			Timer.PCRecTimer = VP_TIMSIMPLEE_OUT;
			while( Timer.PCRecTimer )
			{
				if( SIMPLESIMPLEVPBusFrameUnPack() )
				{ 	
					TracePC("\r\n Drv rec=ok"); 					
					recRes = 1;	
					break;
				}		
			}
			if( recRes== 0 )
			{

				TracePC("\r\n Drv UboxAdminTimeFail");
		        recAck= VP_ERRSIMPLE_COM;
			}
			else
			{
				if( sysVPMissionSIMPLE.receive.msgType==SIMPLE_RESULT_IND )
				{
					if(sysVPMissionSIMPLE.receive.msg[0]>0)
					{
						TracePC("\r\n Drv UboxAdminAckFail");
						 recAck= VP_ERRSIMPLE_PAR;
					}
					else
					{				
						TracePC("\r\n Drv UboxAdminSuc");						
						recAck= VP_ERRSIMPLE_NULL;
					}
				}
				
			}	
			break;	
	}
	//���������vmc
	MsgSIMPLEUboxPack[g_SIMPLEUbox_Index].PCTOVMCCmd = MBOX_SIMPLEVMCTOPC_RESULTIND;	
	MsgSIMPLEUboxPack[g_SIMPLEUbox_Index].adminresult = recAck;
	OSQPost(g_SIMPLEUbox_PCTOVMCQ,&MsgSIMPLEUboxPack[g_SIMPLEUbox_Index]);	
	UpdateSIMPLEIndex();
	memset( &sysVPMissionSIMPLE.receive, 0, sizeof(sysVPMissionSIMPLE.receive) );
	
	return recAck;

}



