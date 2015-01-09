/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           LogAPI
** Last modified Date:  2013-03-04
** Last Version:         
** Descriptions:        ��־������غ�������                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
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
#include "..\Uart3Driver\ZHIHUIPC.h"


//static unsigned char run_no = 0,err_no = 0;



/*********************************************************************************************************
** Function name:       LogGetMoneyAPI
** Descriptions:        ��ʼ����ʱ������־��¼
** input parameters:    
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void LogBeginTransAPI(void)
{
	//������ϸ���׼�¼
	LogParaDetail.BeginYear = RTCData.year;
	LogParaDetail.BeginMonth = RTCData.month;
	LogParaDetail.BeginDay = RTCData.day;
	LogParaDetail.BeginHour = RTCData.hour;
	LogParaDetail.BeginMinute = RTCData.minute;	

	LogParaDetail.BeginSec = RTCData.second;//add by yoc 2013.9.11
	
	//Trace(("\r\n 4.=%ld,%ld,%ld",LogParaDetail.IncomeBill,LogParaDetail.IncomeCoin,LogParaDetail.IncomeCard);
}



/*********************************************************************************************************
** Function name:       LogGetMoneyAPI
** Descriptions:        �ձ�ʱ������־
** input parameters:    InValueͶ�ҵĽ��,MoneyType=1Ӳ����,2ֽ����,3������
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void LogGetMoneyAPI(uint32_t InValue,uint8_t MoneyType)
{
	if((MoneyType == 1)||(MoneyType == 2))
	{
		//���������׼�¼
		LogPara.Income += InValue;
		if(MoneyType == 1)
			LogPara.CoinsIncome += InValue;
		else if(MoneyType == 2)
			LogPara.NoteIncome += InValue;

		//�����ܽ��׼�¼
		LogPara.IncomeTotal += InValue;
		if(MoneyType == 1)
			LogPara.CoinsIncomeTotal += InValue;
		else if(MoneyType == 2)
			LogPara.NoteIncomeTotal += InValue;
	}

	//������ϸ���׼�¼
	if(MoneyType == 1)
		LogParaDetail.IncomeCoin += InValue;
	else if(MoneyType == 2)
		LogParaDetail.IncomeBill += InValue;
	else if(MoneyType == 3)
		LogParaDetail.IncomeCard = InValue;
	
}

/*********************************************************************************************************
** Function name:       LogTransactionAPI
** Descriptions:        ���׳���ʱ������־
** input parameters:    InValue�����Ľ��,transMul�ڼ��ν���,ChannelNum��������,payMode��������
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void LogTransactionAPI(uint32_t InValue,uint8_t transMul,char *ChannelNum,uint8_t payMode)
{
	/******************add by yoc 2013.9.11**********************************/

	//���¶��������ʽ
	// 0 �ֽ�  1 һ��ͨ  2������ 3 PC���� 4֧���� 5��Ϸ���� 6�ֻ����� ������ 
	unsigned char logicNo,pay_mode;
	//���ݽ������͸��½��״����ͽ��
	if(SystemPara.PcEnable == UBOX_PC)//�ѱ�Э��
	{
		if(payMode == 0)//�ֽ���
		{
			LogPara.vpCashNum ++;
			LogPara.vpCashMoney += InValue;
			LogPara.vpCashNumTotal ++;
			LogPara.vpCashMoneyTotal += InValue;
			pay_mode = PAYMODE_CASH;
		}
		else if(payMode == 1)//��Ϸ����
		{
			LogPara.vpGameNum ++;
			LogPara.vpGameMoney += InValue;
			LogPara.vpGameNumTotal ++;
			LogPara.vpGameMoneyTotal += InValue;
			pay_mode = PAYMODE_GAME;
		}
		else if((payMode == 5) || ((payMode >= 101) && (payMode <= 255)))//ˢ������
		{
			LogPara.vpCardNum ++;
			LogPara.vpCardMoney += InValue;
			LogPara.vpCardNumTotal ++;
			LogPara.vpCardMoneyTotal += InValue;
			pay_mode = PAYMODE_UNIONPAY;
		}
		else if(((payMode >= 2) && (payMode <= 4)) || ((payMode >= 6) && (payMode <= 100)))//���߳���
		{
			LogPara.vpOnlineNum ++;
			LogPara.vpOnlineMoney += InValue;
			LogPara.vpOnlineNumTotal ++;
			LogPara.vpOnlineMoneyTotal += InValue;
			pay_mode = PAYMODE_PC;
		}
	}
	else if((SystemPara.PcEnable == ZHIHUI_PC) || (SystemPara.PcEnable == GPRS_PC))//һ���ǻ� GPRS add by yoc zhihui
	{
		if(payMode == 0) payMode = 1;
		if(payMode == 5) payMode = 2;
		if(payMode == 0x01)//�ֽ�
		{
			LogPara.vpCashNum ++;
			LogPara.vpCashMoney += InValue;
			LogPara.vpCashNumTotal ++;
			LogPara.vpCashMoneyTotal += InValue;
			pay_mode = PAYMODE_CASH;
		}
		else if(payMode == 0x02)//һ��ͨ
		{
			LogPara.vpCardNum++;
			LogPara.vpCardMoney += InValue;
			
			LogPara.vpCardNumTotal ++;
			LogPara.vpCardMoneyTotal += InValue;
			LogPara.vpOneCardNumTotal++;
			LogPara.vpOneCardMoneyTotal += InValue;
			pay_mode = PAYMODE_ONECARD;
		}
		else if(payMode == 0x11)//������
		{
			LogPara.vpCardNum++;
			LogPara.vpCardMoney += InValue;

			LogPara.vpCardNumTotal ++;
			LogPara.vpCardMoneyTotal += InValue;
			LogPara.vpUCardNumTotal ++;
			LogPara.vpUCardMoneyTotal += InValue;
			pay_mode = PAYMODE_UNIONPAY;
		}
		else if(payMode == 0x21)//PC ��������
		{
			LogPara.vpOnlineNum++;
			LogPara.vpOnlineMoney += InValue;
			LogPara.vpOnlineNumTotal ++;
			LogPara.vpOnlineMoneyTotal += InValue;
			pay_mode = PAYMODE_PC;
		}
		else if(payMode == 0x41)//֧����
		{
			LogPara.vpOnlineNum++;
			LogPara.vpOnlineMoney += InValue;
			LogPara.vpPC2NumTotal ++;
			LogPara.vpPC2MoneyTotal += InValue;
			pay_mode = PAYMODE_ALIPAY;
		}
		else
		{
			LogPara.vpOnlineNum++;
			LogPara.vpOnlineMoney += InValue;
			LogPara.vpPC3NumTotal++;
			LogPara.vpPC3MoneyTotal += InValue;
			pay_mode = PAYMODE_OTHER;
		}
	}
	else
	{
		if(payMode == 0)//�ֽ���
		{
			LogPara.vpCashNum ++;
			LogPara.vpCashMoney += InValue;
			LogPara.vpCashNumTotal ++;
			LogPara.vpCashMoneyTotal += InValue;
			pay_mode = PAYMODE_CASH;
		}
		else if(payMode == 0x02)//һ��ͨ
		{
			LogPara.vpCardNum++;
			LogPara.vpCardMoney += InValue;
			LogPara.vpCardNumTotal ++;
			LogPara.vpCardMoneyTotal += InValue;
			pay_mode = PAYMODE_ONECARD;
		}
	}
	//�����ܽ��״������
	LogPara.vpSuccessNum++;
	LogPara.vpSuccessMoney += InValue;
	LogPara.vpSuccessNumTotal++;
	LogPara.vpSuccessMoneyTotal += InValue;

	/******************end add**********************************/
	LogPara.TotalTrans += InValue;
//	LogPara.SuccessNum ++;
	//�����ܽ��׼�¼
	LogPara.TotalTransTotal += InValue; 
	
	//������ϸ���׼�¼
	if(transMul < 8 )
	{
		
		
		if((ChannelNum[0]=='A')||(ChannelNum[0]=='B'))//��������
		{
			LogParaDetail.ColumnNo[transMul*3] = ChannelNum[0];
			LogParaDetail.ColumnNo[transMul*3+1] = ChannelNum[1];
			LogParaDetail.ColumnNo[transMul*3+2] = ChannelNum[2];
			logicNo = (ChannelNum[1] - '0') * 10 + (ChannelNum[2] - '0');
		}
		else
		{
			LogParaDetail.ColumnNo[transMul*3] = 'A';
			LogParaDetail.ColumnNo[transMul*3+1] = ChannelNum[0];
			LogParaDetail.ColumnNo[transMul*3+2] = ChannelNum[1];	
			logicNo = (ChannelNum[0] - '0') * 10 + (ChannelNum[1] - '0');
		}
		
		//changed by yoc 2013.12.03
		LogParaDetail.GoodsNo[transMul] = (unsigned char)(getAisleInfo(logicNo,AISLE_ID) & 0xFF);
		LogParaDetail.PriceNo[transMul] = InValue;
		LogParaDetail.TransSucc[transMul] = 1;

		//add by yoc 2013.9.11
		LogParaDetail.PayMode[transMul] = pay_mode;
		LogParaDetail.TradeNum = transMul + 1;
		LogParaDetail.GoodsMin[transMul] = RTCData.minute;
		LogParaDetail.GoodsSec[transMul] = RTCData.second;
		LogParaDetail.run_no[transMul] = LogPara.run_no;
		
	}

	print_log("logTransaction tradeNum = %d\r\n",transMul);
	
	LogPara.run_no++;
	
}


/*********************************************************************************************************
** Function name:       LogCostAPI
** Descriptions:        PC�ۿ�ʱ������־
** input parameters:    InValue�ۿ�Ľ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void LogCostAPI(uint32_t InValue)
{
	//����PC�ۿ�������		
	LogPara.vpCashMoney += InValue;			
	LogPara.vpCashMoneyTotal += InValue;
		
	//����PC�ۿ��ܽ��״������
	LogPara.vpSuccessMoney += InValue;
	LogPara.vpSuccessMoneyTotal += InValue;

	/******************end add**********************************/
	//���������׼�¼
	LogPara.TotalTrans += InValue;
	//�����ܽ��׼�¼
	LogPara.TotalTransTotal += InValue;	
}


/*********************************************************************************************************
** Function name:       LogTransactionAPI
** Descriptions:        �����ѽ�������־
** input parameters:    InValue�����Ľ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void LogCardIncomeAPI(uint32_t InValue)
{
	
	//changed by yoc 2013.9.11
	LogPara.vpCardNum ++ ;
	LogPara.vpCardMoney +=InValue;//old code
	LogPara.vpCardNumTotal ++;
	LogPara.vpCardMoneyTotal += InValue;
}



/*********************************************************************************************************
** Function name:       LogChangeAPI
** Descriptions:        ����ʱ������־
** input parameters:    InValue����Ľ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void LogChangeAPI(uint32_t InValue,uint32_t debtMoney)
{
	
	//���������׼�¼
	LogPara.TotalChange += InValue;
	LogPara.Iou += debtMoney;

	//�����ܽ��׼�¼	
	LogPara.TotalChangeTotal += InValue;

	//������ϸ���׼�¼
	LogParaDetail.Change = InValue;	
}


/*********************************************************************************************************
** Function name:       LogEndTransAPI
** Descriptions:        ��������ʱ������־��¼
** input parameters:    
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void LogEndTransAPI(void)
{	
	//������ϸ���׼�¼	
	LogParaDetail.EndMinute = RTCData.minute;
	//��ϸ���׼�¼д��FLASH��1-TRADE_PAGE_MAX,ҳ��0������LogPara.LogDetailPage��ʾĿǰ�Ѿ�д���flashҳ��
	if(LogPara.LogDetailPage < TRADE_PAGE_MAX)
		LogPara.LogDetailPage++;//������ϸ���׼�¼ҳ����	
	else
		LogPara.LogDetailPage = 1;
	WriteLogDetailAPI(LogPara.LogDetailPage);//������ϸ���׼�¼��flash��		
	//���������׼�¼,�ܽ��׼�¼��flash��
	WriteLogAPI();
	Trace("\r\n page=%d",LogPara.LogDetailPage);
	OSTimeDly(OS_TICKS_PER_SEC/100);	
}



/*********************************************************************************************************
** Function name:       LogClearAPI
** Descriptions:        ɾ��������־��¼
** input parameters:    
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void LogClearAPI(void)
{

	vmc_batch_info_write();	//add by yoc 2013.11.13
	LogPara.Income = 0;
	LogPara.NoteIncome = 0;
	LogPara.CoinsIncome = 0;
	LogPara.TotalTrans = 0;
	LogPara.TotalChange = 0;
	LogPara.CoinsChange[0] = 0;
	LogPara.CoinsChange[1] = 0;
	LogPara.CoinsChange[2] = 0;
	LogPara.SuccessNum = 0;
	LogPara.DoubtNum = 0;
	LogPara.Iou = 0;
//	LogPara.CardIncome = 0;
	
	//LogPara.LogDetailPage = 0;
	//���������׼�¼,�ܽ��׼�¼��flash��
	LogPara.vpSuccessNum = 0;
	LogPara.vpSuccessMoney = 0;
	LogPara.vpCashNum = 0;
	LogPara.vpCashMoney = 0;
	LogPara.vpGameNum = 0;
	LogPara.vpGameMoney = 0;
	LogPara.vpCardNum = 0;
	LogPara.vpCardMoney = 0;
	LogPara.vpOnlineNum = 0;
	LogPara.vpOnlineMoney = 0;
	
	WriteLogAPI();
	
}


/*********************************************************************************************************
** Function name:       WriteLogAPI
** Descriptions:        �������׼�¼,�ܽ��׼�¼д��flash��
** input parameters:    
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void WriteLogAPI(void)
{
	uint8_t *tp2,buf[512];
	uint16_t crc,len;

	tp2 = (uint8_t *) &LogPara;//�������ݼ�¼	
	/*
	crc = CrcCheck(tp2,sizeof(LogPara)-2);
	LogPara.CrcCheck[0] = crc/256;
	LogPara.CrcCheck[1] = crc%256; */
	//Trace("\r\n logcrc=%d,%x,%x",sizeof(LogPara)-2,LogPara.CrcCheck[0],LogPara.CrcCheck[1]);
	//changed by yoc 2013.9.11
	if(sizeof(LogPara) > 500)
	{
		Trace("sizeof(LogPara) is %d  > 500\r\n",sizeof(LogPara));
		return ;
	}
	len = sizeof(LogPara);
	memcpy(buf,tp2,len);
	crc = CrcCheck(buf,len);
	buf[len] = HUINT16(crc);
	buf[len + 1] = LUINT16(crc);
	buf[len + 2] = 0xE0;//�����׼�¼д��flash��־
	AT45DBWritePage(4040,buf);
	//Trace("LogPara write ...\r\n");
}

/*********************************************************************************************************
** Function name:       ReadLogAPI
** Descriptions:        ��flash�������׼�¼,�ܽ��׼�¼
** input parameters:    
** output parameters:   ��
** Returned value:      1��ȡ�ɹ�,0��ȡʧ��
*********************************************************************************************************/
uint8_t ReadLogAPI(void)
{
	uint8_t *tp2,buf[512];
	uint16_t crc,len;
	tp2 = (uint8_t *) &LogPara;//�������ݼ�¼	

	AT45DBReadPage(4040,buf);

	len = sizeof(LogPara);
	if(len > 500) return 0;
	crc = CrcCheck(buf,len);
	//Trace("LogPara:\r\n crc =%d,buf[crc]=%d type = %d\r\n",crc,INTEG16(buf[len],buf[len + 1]),buf[len + 2]);
	if((crc == INTEG16(buf[len],buf[len + 1])) && (buf[len + 2] == 0xE0))
	{
		Trace("\r\nLogPara read from flash suc....\r\n");
		memcpy(tp2,buf,len);
		return 1;
	}
	//Trace("LogPara read from flash fail....\r\n");
	return 0;
/*
	crc = CrcCheck(tp2,sizeof(LogPara)-2);
	//Trace("\r\n logcrc=%d,%x,%x",sizeof(LogPara)-2,crc/256,crc%256);
	if( (crc/256 == LogPara.CrcCheck[0])&&(crc%256 == LogPara.CrcCheck[1]) )
	{
		return 1;
	}
	else
	{
		return 0;
	} */
}



/*********************************************************************************************************
** Function name:       WriteLogDetailAPI
** Descriptions:        ����ϸ��־д��flash��
** input parameters:    
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void WriteLogDetailAPI(uint16_t	LogDetailPage)
{
	uint8_t *tp1,i,buf[512];
	uint16_t crc,len;

	tp1 = (uint8_t *) &LogParaDetail;//�������ݼ�¼	
	
	if(sizeof(LogParaDetail) > 500)
	{
		Trace("sizeof(LogPara) is %d  > 500\r\n",sizeof(LogPara));
		return ;
	}
	len = sizeof(LogParaDetail);
	memcpy(buf,tp1,len);
	crc = CrcCheck(buf,len);
	buf[len] = HUINT16(crc);
	buf[len + 1] = LUINT16(crc);
	buf[len + 2] = 0xE1;//�ܽ��׼�¼д��flash ��־
	AT45DBWritePage(LogDetailPage,buf);
	Trace("TradeNum=%d\r\n",LogParaDetail.TradeNum);
	for(i = 0;i < 24 ; )
	{
		Trace("ColumnNo=%c%c%c\r\n ",LogParaDetail.ColumnNo[i],LogParaDetail.ColumnNo[i+1],LogParaDetail.ColumnNo[i+2]);
		i+=3;
		OSTimeDly(20);
	}
	//PrintLogDetailAPI();
	OSTimeDly(OS_TICKS_PER_SEC/100);
	
	memset(&LogParaDetail,0,sizeof(LogParaDetail));
}

/*********************************************************************************************************
** Function name:       WriteLogDetailAPI
** Descriptions:        ��flash��ÿ����ϸ��־
** input parameters:    
** output parameters:   ��
** Returned value:      1��ȡ�ɹ�,0��ȡʧ��
*********************************************************************************************************/
uint8_t ReadLogDetailAPI(uint16_t	LogDetailPage)
{
	uint8_t *tp1,buf[512];
	uint16_t crc,len;
	
	tp1 = (uint8_t *) &LogParaDetail;//�������ݼ�¼		
	AT45DBReadPage(LogDetailPage,buf);

	len = sizeof(LogParaDetail);
	if(len > 500) return 0;
	crc = CrcCheck(buf,len);
	//Trace("LogDetailPage:\r\n crc =%d,buf[crc]=%d,typr = %d\r\n",crc,INTEG16(buf[len],buf[len + 1]),buf[len + 2]);
	if((buf[len + 2] == 0xE1) && (crc == INTEG16(buf[len],buf[len + 1])))
	{
		//Trace("LogDetailPage read from flash suc....\r\n");
		memcpy(tp1,buf,len);
		return 1;
	}
	//Trace("LogDetailPage read from flash fail...\r\n");
	return 0;
/*
	crc = CrcCheck(tp1,sizeof(LogParaDetail)-2);
	//Trace("\r\n detaillogcrc=%d,%x,%x",sizeof(LogParaDetail)-2,crc/256,crc%256);
	if( (LogParaDetail.CrcCheck[0]==0)&&(LogParaDetail.CrcCheck[1]==0) )
	{
		return 0;
	}
	else
	{
		if( (crc/256 == LogParaDetail.CrcCheck[0])&&(crc%256 == LogParaDetail.CrcCheck[1]) )
		{
			return 1;
		}
		else
		{
			return 0;
		} 
	}
	*/
}


/*********************************************************************************************************
** Function name:       ReadDetailTraceAPI
** Descriptions:        ��flash��ÿ����ϸ��־ ����ȫ�ֱ���
** input parameters:    
** output parameters:   ��
** Returned value:      1��ȡ�ɹ�,0��ȡʧ��
*********************************************************************************************************/
uint8_t ReadDetailTraceAPI(char * detailBuff,uint16_t	LogDetailPage)
{
	uint8_t buf[512];
	uint16_t crc,len;
		
	AT45DBReadPage(LogDetailPage,buf);

	len = sizeof(LogParaDetail);
	if(len > 500) return 0;
	crc = CrcCheck(buf,len);
	if((buf[len + 2] == 0xE1) && (crc == INTEG16(buf[len],buf[len + 1])))
	{
		//Trace("LogDetailPage read from flash suc....\r\n");
		if(detailBuff != NULL)
			memcpy(detailBuff,buf,len);
		return 1;
	}
	return 0;
}


