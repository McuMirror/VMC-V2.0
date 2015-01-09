/***************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           VMC_GPRS_PC.C
** Last modified Date:  2013-07-10
** Last Version:         
** Descriptions:          ���ذ�GPRSЭ��                   
**------------------------------------------------------------------------------------------------------
** Created by:          yanbo
** Created date:        2013-07-10
** Version:             V2.0
** Descriptions:        The original version
**------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
*******************************************************************************************************/


#include "VMC_GPRS_PC.h"
#include "..\config.h"
volatile unsigned char vmc_gprs_pc_state; //gprs����״̬ 2��ʾ��PC�Ͽ����� 1���ӳ�ʱ
TRADE_CACHE_NODE trade_cache_head;

/***********************************************************************************************************
*Function name :    vmc_gprs_send
*Descriptions:		�����ͨ��GPRS����ͨ��Э�鵽������
*input parameters: zhihui_msg �������ģ�  				
*output parameters:   �ޣ�
*Returned value: 	 1��ʾ�ɹ� 0 ��ʾʧ�� 2��ʱ �Ͽ����� ��������̫��
*************************************************************************************************************/
unsigned char vmc_gprs_send(INT8U *send_msg,INT16U len)
{
	unsigned char err;
	Trace("vmc_gprs_send..........\r\n");
	err = GPRS_SendReport(send_msg,len);
	Trace("vmc_gprs_send len=%d err=%d\r\n",len,err);
	if(err == 2)
	{
		//vmc_gprs_pc_state = 1;
		trade_cache_head.com_err_num = 20;
		return 2;
	}
	else if(err == 1)
	{
		return 1;
	}
	trade_cache_head.com_err_num++;
	return 0;
}
/***********************************************************************************************************
*Function name :    vmc_gprs_recv
*Descriptions:		�����ͨ��GPRS���շ�������Ӧ��
*input parameters:  recv_msg ��Ӧ���� len ���ճ���				
*output parameters:   �ޣ�
*Returned value: 	 1��ʾ�ɹ� 0 ��ʾʧ�� 2��ʱ ������Ϣ���ͳɹ� ���������ճɹ�������Ӧ���� GPRSû���յ�
*************************************************************************************************************/
unsigned char vmc_gprs_recv(INT8U *recv_msg,INT16U *len,INT8U times)
{	
	unsigned char i,Len=0;
	memset(recv_msg,0,512);
	if(times == 1)
	{
		OSTimeDly(50);
		if(GPRS_GetAckReport(recv_msg,&Len) == 0)
		{
			if(zhihui_msg.recv_err_num++ > 3)
			{
				zhihui_msg.recv_err_num = 0;
				//change by liya
				GPRS_QICLOSE();	
				return GPRS_A_RESET;	
			}
			Trace("Time_Out:%c%c\r\n",zhihui_msg.send_msg[GPRS_HEAD -2],zhihui_msg.send_msg[GPRS_HEAD -1]);
			return GPRS_A_TIMEOUT;
		}
		Trace("\r\nRecv msg sus!!\r\nsend_msg:%s\r\nrecv_msg:%s\r\n",zhihui_msg.send_msg,recv_msg);		
	}
	else
	{
		//OSTimeDly(200);
		
		for(i =0;i < times;i++)
		{	
			OSTimeDly(3000/5);
		  	if(GPRS_GetAckReport(recv_msg,&Len) == 1)
				break;				
		}
		if(i >= times)
		{
			Trace("Time_Out:%c%c\r\n",zhihui_msg.send_msg[GPRS_HEAD -2],zhihui_msg.send_msg[GPRS_HEAD -1]);
			//change by liya
			GPRS_QICLOSE();
			//��ʱ
			return GPRS_A_TIMEOUT;
		}
		Trace("GetAck...Len=%d\r\n",Len);
		Trace("\r\nRecv msg sus!!\r\nsend_msg:%s\r\nrecv_msg:%s\r\n",zhihui_msg.send_msg,recv_msg);		
	}
	*len = Len - 4;
	//��֤�������Ƿ���ȷ
	if((ASC_to_HEC(recv_msg[0]) != *len/1000) ||
		(ASC_to_HEC(recv_msg[1]) != *len/100%10) ||
		(ASC_to_HEC(recv_msg[2]) != *len%100/10) ||
		(ASC_to_HEC(recv_msg[3]) != *len%10))
	{
		
		Trace("GPRS_A_ERR1:%c%c\r\n",zhihui_msg.send_msg[GPRS_HEAD -2],zhihui_msg.send_msg[GPRS_HEAD -1]);
		Trace("recv_msg HEADLEN=%d%d%d%d\r\n",ASC_to_HEC(recv_msg[0]),
			ASC_to_HEC(recv_msg[1]),
			ASC_to_HEC(recv_msg[2]),
			ASC_to_HEC(recv_msg[3]));
		return GPRS_A_ERR;
	}
	//��֤��ͷʱ����Ͱ汾��  �Ƿ���ȷ 
	if(strncmp((char *)&zhihui_msg.send_msg[4],(char *)&recv_msg[4],14) != 0 ||
		strncmp((char *)&zhihui_msg.send_msg[26],(char *)&recv_msg[26],8) != 0)
	{
	 	 Trace("GPRS_A_ERR2:%c%c\r\n",zhihui_msg.send_msg[GPRS_HEAD -2],zhihui_msg.send_msg[GPRS_HEAD -1]);
		 
		 return GPRS_A_ERR;
	}
	//��֤�ն˺��Ƿ���ȷ
	if(strncmp((char *)&zhihui_msg.send_msg[18],(char *)&recv_msg[18],8) != 0)
	{
		Trace("GPRS_A_ERR3:%c%c\r\n",zhihui_msg.send_msg[GPRS_HEAD -2],zhihui_msg.send_msg[GPRS_HEAD -1]);
		return 	GPRS_A_TERMINAL;
	}		
	//��֤ ���ݷ����Ƿ�Ϊ��Ӧ��  �������Ƿ�һ��
	if((recv_msg[GPRS_HEAD - 3] != ASK) || 
		recv_msg[GPRS_HEAD -2] != zhihui_msg.send_msg[GPRS_HEAD -2] ||
		recv_msg[GPRS_HEAD -1] != zhihui_msg.send_msg[GPRS_HEAD -1])
	{
		Trace("GPRS_A_ERR4:%c%c\r\n",zhihui_msg.send_msg[GPRS_HEAD -2],zhihui_msg.send_msg[GPRS_HEAD -1]);
		return GPRS_A_ERR;
	}
	if(recv_msg[GPRS_HEAD] == '0' && recv_msg[GPRS_HEAD + 1] == '0')
	{
		Trace("GPRS_A_SUCCEE:%c%c\r\n",zhihui_msg.send_msg[GPRS_HEAD -2],zhihui_msg.send_msg[GPRS_HEAD -1]);
		return GPRS_A_SUCCEE;
	}	
	if(recv_msg[GPRS_HEAD] == '9' && recv_msg[GPRS_HEAD + 1] == '9')
	{
		Trace("GPRS_A_GEN_ERR:%c%c\r\n",zhihui_msg.send_msg[GPRS_HEAD -2],zhihui_msg.send_msg[GPRS_HEAD -1]);
		return GPRS_A_GEN_ERR;
	}			
	else
	Trace("GPRS_A_ERR5:%c%c\r\n",zhihui_msg.send_msg[GPRS_HEAD -2],zhihui_msg.send_msg[GPRS_HEAD -1]);	
	return GPRS_A_FAIL;
}

/***********************************************************************************************************
*Function name :    ship_cache_empty
*Descriptions:		�������Ƿ�Ϊ��
*input parameters:  head ������ ָ��				
*output parameters:   �ޣ�
*Returned value: 	 1��ʾ�� 0 ��ʾ�ǿ�
*************************************************************************************************************/

char ship_cache_empty(void)
{
	//Trace("\r\n Print:read_index is %d write_index is %d\r\n",head->read_index,head->write_index);
	if(MACData.read_index == MACData.write_index)
		return 1;
	else
		return 0;
}
/***********************************************************************************************************
*Function name :    vmc_gprs_trade_send
*Descriptions:		��ȡflash�ϵĽ��׼�¼����
*input parameters:  ��			
*output parameters:   �ޣ�
*Returned value: 	 1��ʾ�ɹ�0 ��ʾʧ��
*************************************************************************************************************/
unsigned char vmc_gprs_trade_send(void)
{
	unsigned char sub_index,err,num,i,logic_no;
	unsigned short index;
	

	index = MACData.read_index;
	sub_index = MACData.read_sub_index;
	if((index > TRADE_PAGE_MAX) || (sub_index > 8))
	{
		Trace("err index is %d,sub_index is %d\r\n",index,sub_index);
		
		return 0;
	}
	if(ReadLogDetailAPI(index) != 1)//flash ���������׼������һҳ
	{
		Trace("read flash ReadLogDetailAPI err\r\n");
		memset((void *)&LogParaDetail,0,sizeof(LogParaDetail));
		return 2;
	}
	Trace("GPRS read LogParaDetail\r\n");
	Trace("logic_no[1]=%c%c\r\n",LogParaDetail.ColumnNo[0],LogParaDetail.ColumnNo[1]);
	Trace("logic_no[2]=%c%c\r\n",LogParaDetail.ColumnNo[3],LogParaDetail.ColumnNo[4]);
	Trace("logic_no[3]=%c%c\r\n",LogParaDetail.ColumnNo[6],LogParaDetail.ColumnNo[7]);
	Trace("logic_no[4]=%c%c\r\n",LogParaDetail.ColumnNo[9],LogParaDetail.ColumnNo[10]);
	Trace("logic_no[5]=%c%c\r\n",LogParaDetail.ColumnNo[12],LogParaDetail.ColumnNo[13]);
	Trace("logic_no[6]=%c%c\r\n",LogParaDetail.ColumnNo[15],LogParaDetail.ColumnNo[16]);
	Trace("logic_no[7]=%c%c\r\n",LogParaDetail.ColumnNo[18],LogParaDetail.ColumnNo[19]);
	Trace("logic_no[8]=%c%c\r\n",LogParaDetail.ColumnNo[21],LogParaDetail.ColumnNo[22]);
//	if(LogParaDetail.TradeNum == sub_index)//���ױ���������
//	{
//		Trace("gprs trade report over!!\r\n");
//		return 2;
//	}

	logic_no = ASC_to_HEC(LogParaDetail.ColumnNo[sub_index*3]) * 10 +
				ASC_to_HEC(LogParaDetail.ColumnNo[sub_index*3 + 1]);	
	Trace("index=%d,sub_index=%d logic_no=%d\r\n",index,sub_index,logic_no);
	if((logic_no < 11) || (logic_no >= 90))//�ж��Ƿ��н��� û����׼��ת��һҳ
	{
		Trace("gprs trade logic_no is not right!\r\n");
		memset((void *)&LogParaDetail,0,sizeof(LogParaDetail));
		return 2;
	}
	num = vmc_logic_to_no(logic_no,vmc_goods);//�߼�����ת������	
	MACData.trade_state = (LogParaDetail.TransSucc[sub_index] == 1) ?
			0 : 1;
	MACData.trade_pay_mode = (LogParaDetail.IncomeCard) ?
			0x02 : 0x01;
	/*
	zhihui_msg.date_ship.year = LogParaDetail.BeginYear;
	zhihui_msg.date_ship.month = LogParaDetail.BeginMonth;
	zhihui_msg.date_ship.day = LogParaDetail.BeginDay;
	zhihui_msg.date_ship.hour = LogParaDetail.BeginHour;
	zhihui_msg.date_ship.min = LogParaDetail.BeginMinute;
	zhihui_msg.date_ship.sec = sub_index;
	*/
	if(MACData.trade_pay_mode == 0x02)
	{
		for(i = 0;i < 10;i++)
			MACData.trade_card_no[i] = MACData.trade_card_no[i];
	}
	else
		memset(MACData.trade_card_no,0,10);
	Trace("Read detail:num=%d,logic=%d,price=%d,pay_mode=%d,shipment_state=%d\r\n",
		num,logic_no,getAisleInfo(logic_no,AISLE_PRICE),MACData.trade_pay_mode,MACData.trade_state);



	
	err = vmc_gprs_goods(num,1);
	if(err != 1)
	{
		Trace("vmc_gprs_goods send fail\r\n");
		memset((void *)&LogParaDetail,0,sizeof(LogParaDetail));
		return 0;
	}
	
	memset((void *)&LogParaDetail,0,sizeof(LogParaDetail));
	Trace("GPRS memset LogParaDetail\r\n");
	Trace("logic_no[1]=%c%c\r\n",LogParaDetail.ColumnNo[0],LogParaDetail.ColumnNo[1]);
	Trace("logic_no[2]=%c%c\r\n",LogParaDetail.ColumnNo[3],LogParaDetail.ColumnNo[4]);
	Trace("logic_no[3]=%c%c\r\n",LogParaDetail.ColumnNo[6],LogParaDetail.ColumnNo[7]);
	Trace("logic_no[4]=%c%c\r\n",LogParaDetail.ColumnNo[9],LogParaDetail.ColumnNo[10]);
	Trace("logic_no[5]=%c%c\r\n",LogParaDetail.ColumnNo[12],LogParaDetail.ColumnNo[13]);
	Trace("logic_no[6]=%c%c\r\n",LogParaDetail.ColumnNo[15],LogParaDetail.ColumnNo[16]);
	Trace("logic_no[7]=%c%c\r\n",LogParaDetail.ColumnNo[18],LogParaDetail.ColumnNo[19]);
	Trace("logic_no[8]=%c%c\r\n",LogParaDetail.ColumnNo[21],LogParaDetail.ColumnNo[22]);
	return 1;
}

/***********************************************************************************************************
*Function name :    trade_cache_init
*Descriptions:		��ʼ��GPRS������
*input parameters:  head ������ ָ��				
*output parameters:   �ޣ�
*Returned value: 	 1��ʾ�ɹ� 0 ��ʾʧ��
*************************************************************************************************************/

unsigned char trade_cache_init(void)
{
	Trace("trade_cache_init ....\r\n");
	memset((void *)&trade_cache_head,0,sizeof(TRADE_CACHE_NODE));
	trade_cache_head.buf_empty = TRUE;//��ʼ�� ������λ��
	trade_cache_head.recv_data = TRUE;//��ʼ�� �ɽ�������
	return 1;
}


/***********************************************************************************************************
*Function name :    ship_cache_poll
*Descriptions:		gprs ��ѯ�ϱ�����
*input parameters:  ��			
*output parameters:   �ޣ�
*Returned value: 	 1��ʾ�ɹ�0 ��ʾʧ��
*************************************************************************************************************/
unsigned char ship_cache_poll(void)
{
	unsigned char err,err1;
	//�������ݺ�鿴GPRS��Ӧ��
	if(trade_cache_head.recv_data == FALSE)
	{
		Trace("recv data........\r\n");
		err = vmc_gprs_recv(zhihui_msg.recv_msg,&zhihui_msg.recv_len,1);
		if(err != GPRS_A_TIMEOUT)
		{
			//Trace("");
			trade_cache_head.recv_data = TRUE;//���л�Ӧ��������ȷ�����1 ��ʾ���Լ�������
		}
		if((err == GPRS_A_SUCCEE) || (err == GPRS_A_GEN_ERR))//ͨ�ô���99
		{
			Trace("recv suc!!!\r\n");
			switch (trade_cache_head.type)
			{
				case GPRS_T_FAULT:
					trade_cache_head.fault_req = FALSE;
					break;
				case GPRS_T_RUN:
					trade_cache_head.run_req = FALSE;
					break;
				case GPRS_T_CONFIG:
					trade_cache_head.config_req = FALSE;
					break;
				case GPRS_T_AISLE:
					trade_cache_head.aisle_req = FALSE;
					break;
				case GPRS_T_TRADE:
					Trace("trade index changed...\r\n");
					if(MACData.read_sub_index >= 7)
					{	
						MACData.read_sub_index = 0;
						if(MACData.read_index >= TRADE_PAGE_MAX)
							MACData.read_index = 0;
						else
							MACData.read_index++;					
					}
					else
						MACData.read_sub_index++;
					
					if(vmc_trade_info_write() == 1)//д
					{
						Trace("write trade_index flash suc");
					}
					break;
				case GPRS_T_POLL:
					trade_cache_head.poll_req = FALSE;
					break;
				default:
					break;
			}

			trade_cache_head.com_err_num = 0;
			trade_cache_head.buf_empty = TRUE;//�� �������� ��־	
		//	memset(trade_cache_head.node,0,sizeof(trade_cache_head.node));
			trade_cache_head.len = 0;
			trade_cache_head.type = 0;
			return 1;		
		}
		else
		{
			Trace("recv fail!!!\r\n");			
			trade_cache_head.com_err_num++;			
		}	
		return 0;		
	}
    else if(trade_cache_head.recv_data == TRUE)//���յ��˻�Ӧ�����Է���
	{
		
		if(!trade_cache_head.buf_empty)//�������а� �ȷ�������
		{
				Trace("buffer is not empty\r\nlen = %d,type=%d\r\n",
					trade_cache_head.len,trade_cache_head.type);
		
				//memcpy(zhihui_msg.send_msg,&trade_cache_head.node,trade_cache_head.len);
				err = vmc_gprs_send(zhihui_msg.send_msg,trade_cache_head.len);
				if(err == 1)
				{
					Trace("send the buffer data!\r\n buf is %s\r\n",zhihui_msg.send_msg);
					trade_cache_head.recv_data = FALSE;
					return 1;
				}
				return 0;
					
		}
		//�鿴�Ƿ���fault run config aisle��Ҫ����
		if(trade_cache_head.fault_req)
		{
			Trace("fault_req send....\r\n");

			vmc_gprs_fualt(1);
			trade_cache_head.buf_empty = FALSE;//�޸Ļ�������־ �ǿ�	
			//memcpy(zhihui_msg.send_msg,trade_cache_head.node,trade_cache_head.len);
			if(vmc_gprs_send(zhihui_msg.send_msg,trade_cache_head.len) == 1)
			{
				Trace("send the data fault flash!\r\n buf is %s\r\n",zhihui_msg.send_msg);
				trade_cache_head.recv_data = FALSE;
				return 1;
			}
			return 0;
		
		}
		if(trade_cache_head.run_req)
		{

			Trace("run_req send....\r\n");
				//��flash �滺����
			vmc_gprs_run(1);
			trade_cache_head.buf_empty = FALSE;//�޸Ļ�������־ �ǿ�	
			//memcpy(zhihui_msg.send_msg,trade_cache_head.node,trade_cache_head.len);
			if(vmc_gprs_send(zhihui_msg.send_msg,trade_cache_head.len) == 1)
			{
				Trace("send the  run flash!\r\n buf is %s\r\n",zhihui_msg.send_msg);
				trade_cache_head.recv_data = FALSE;
				return 1;
			}	
		
			return 0;			
		}
		if(trade_cache_head.config_req)
		{
			Trace("config_req send....\r\n");	
			vmc_gprs_config(1);
			trade_cache_head.buf_empty = FALSE;//�޸Ļ�������־ �ǿ�	
			//memcpy(zhihui_msg.send_msg,trade_cache_head.node,trade_cache_head.len);
			if(vmc_gprs_send(zhihui_msg.send_msg,trade_cache_head.len) == 1)
			{
				Trace("send the buffer data!\r\n buf is %s\r\n",zhihui_msg.send_msg);
				trade_cache_head.recv_data = FALSE;
				return 1;
			}	

			return 0;
			
		}
		if(trade_cache_head.aisle_req)
		{
			Trace("aisle req send....\r\n");
			vmc_gprs_aisle(1);
			trade_cache_head.buf_empty = FALSE;//�޸Ļ�������־ �ǿ�	
			//memcpy(zhihui_msg.send_msg,trade_cache_head.node,trade_cache_head.len);
			if(vmc_gprs_send(zhihui_msg.send_msg,trade_cache_head.len) == 1)
			{
				Trace("send the buffer data!\r\n buf is %s\r\n",zhihui_msg.send_msg);
				trade_cache_head.recv_data = FALSE;
				return 1;
			}	
			return 0;
			
		}
		err = ship_cache_empty();
		//�鿴�Ƿ���δ�ϴ��ĳ���ָ��
		if(!err)
		{	
			Trace("trade is not empty send....\r\n");
			err1 = vmc_gprs_trade_send();//
			Trace("vmc_gprs_trade_send err1=%d\r\n",err1);
			if(err1 == 1)
			{
				trade_cache_head.len = zhihui_msg.send_len;
				trade_cache_head.type = GPRS_T_TRADE;
				Trace("len = %d,type= GPRS_T_TRADE\r\n",trade_cache_head.len);
				//memcpy(&trade_cache_head.node,zhihui_msg.send_msg,trade_cache_head.len);
				trade_cache_head.buf_empty = FALSE;//�޸Ļ�������־ �ǿ�	
				if(vmc_gprs_send(zhihui_msg.send_msg,trade_cache_head.len) == 1)
				{
					Trace("send the buffer data!\r\n buf is %s\r\n",zhihui_msg.send_msg);
					trade_cache_head.recv_data = FALSE;
					return 1;
				}
			}
			else if(err1 == 2)//ֱ��ת��һҳ
			{
					Trace("vmc_gprs_trade_send return 2\r\n");
					if(MACData.read_index <= TRADE_PAGE_MAX)
						MACData.read_index++;
					else
						MACData.read_index = 0;	
					MACData.read_sub_index = 0;
				return 1;
			}
			

		}
		//��ѯ�� ������ʱ�ŷ��� һ�������������ϱ� ��������
		
		if(trade_cache_head.poll_req)
		{
			Trace("poll_req send....\r\n");
			vmc_gprs_poll(1);
			trade_cache_head.buf_empty = FALSE;//�޸Ļ�������־ �ǿ�	
			//memcpy(zhihui_msg.send_msg,trade_cache_head.node,trade_cache_head.len);
			if(vmc_gprs_send(zhihui_msg.send_msg,trade_cache_head.len) == 1)
			{
				Trace("send the buffer data!\r\n buf is %s\r\n",zhihui_msg.send_msg);
				trade_cache_head.recv_data = FALSE;
				return 1;
			}		
			return 0;
		}
	
	}
	
	return 0;
}



/***********************************************************************************************************
*Function name :    msg_head_config
*Descriptions:		��ͷ����
*input parameters:  mode ����ģʽ				
*output parameters:   �ޣ�
*Returned value: 	 1��ʾ�ɹ� 0 ��ʾʧ��  
*************************************************************************************************************/
unsigned char msg_head_config(unsigned char mode)
{
	if(mode == GPRS_H_INIT)
	{
		vmc_gprs_pc_state = 0;//��ʼ������״̬
		memset((void *)&zhihui_msg,0,sizeof(zhihui_msg));
		zhihui_msg.terminal = MACData.machine_info.mac_sn;//MAC_SN; //�汾��
		zhihui_msg.version = 1;
		//memset(zhihui_msg.reserve,'0',6);
		//zhihui_msg.reserve[5] = '1';
	}
	/*  
	zhihui_msg.date.year = RTCData.year;
	zhihui_msg.date.month = RTCData.month;
	zhihui_msg.date.day = RTCData.day;
	zhihui_msg.date.hour = RTCData.hour;
	zhihui_msg.date.min =  RTCData.minute;
	zhihui_msg.date.sec =  RTCData.second;
	   */
   return 1;
}
/***********************************************************************************************************
*Function name :    vmc_integ_str
*Descriptions:		���Ͱ�����
*input parameters:  zhihui_msg ��len���ͳ���				
*output parameters:   �ޣ�
*Returned value: 	 ���ͳ��� 
*************************************************************************************************************/
static unsigned short vmc_integ_str(char *buf,unsigned char len)
{
	unsigned short len_s;
	memset((void*)zhihui_msg.send_msg,0,sizeof(zhihui_msg.send_msg));
	len_s = sprintf((char *)zhihui_msg.send_msg,
		"%04.4d%04.4d%02.2d%02.2d%02.2d%02.2d%02.2d%08d%02d%06.6s%01c%02d%-0*.*s",
		zhihui_msg.len,RTCData.year,RTCData.month,RTCData.day,RTCData.hour,
		RTCData.minute,RTCData.second,zhihui_msg.terminal,zhihui_msg.version,"000001",
		zhihui_msg.direction,zhihui_msg.contrl,len,len,buf);
	
	return len_s;
}
/***********************************************************************************************************
*Function name :    vmc_gprs_register
*Descriptions:		�����ͨ��GPRS����ǩ����������
*input parameters: zhihui_msg �������ģ�  				
*output parameters:   �ޣ�
*Returned value:   1 �ɹ�  0 ʧ��
*************************************************************************************************************/
unsigned char vmc_gprs_register(unsigned char mode)
{

	unsigned char len,err,err_no = 0;
	char buf[L_REGISTER] = {'0'};
	msg_head_config(GPRS_H_INIT);
	zhihui_msg.len = GPRS_HEAD - 4 +L_REGISTER;
	if(MACData.machine_info.mac_t == MAC_T_F)//�ͺ� 1ʳƷ�� 2���ϻ� 3����
		buf[0] = '1';
	else if(MACData.machine_info.mac_t == MAC_T_D)
		buf[0] = '2';
	else
		buf[0] = '3';
	sprintf(&buf[1],"%-20.*s%-14.*s",20,"PC21PC",14,"20120618");	
	zhihui_msg.direction = 'R';
	zhihui_msg.contrl = G_REGISTER;	
	len = vmc_integ_str(buf,L_REGISTER);
retry:
	if(err_no > 20)
	{
		err_no = 0;
		while(mvc_gprs_init() != 1)
		{
			Trace("GPRS_Init.RETRY..\r\n");
			OSTimeDly(3000/5);
		}
	}
	err = vmc_gprs_send(zhihui_msg.send_msg,len);
	if(err != 1)
	{
		err = 0;
		err_no++;
		OSTimeDly(2000/5);
		if(mode == 1)
			goto retry;
		else 
			return 0;
	}
	err = vmc_gprs_recv(zhihui_msg.recv_msg,&zhihui_msg.recv_len,TIMES);		
	if(err == GPRS_A_SUCCEE)//��ʾ�յ���������Ӧ�ɹ�
	{			
	   return 1;
	}
	else if(err == GPRS_A_TIMEOUT )//��ʱ
	{
		if(mode == 1)
		{
			OSTimeDly(2000/5);
			goto retry;	
		}		
		else
			return 0;	
	}
	else 
	{
		if(mode == 1)
		{
			OSTimeDly(2000/5);
			goto retry;	
		}		
		else
			return 0;	
	}
}
/********************************************************************************************************
** Function name:     	vmc_gprs_poll
** Descriptions:	    ��ѯ
** input parameters:    ��
** output parameters:   ��
** Returned value:      1,�ɹ�  0ʧ��
*********************************************************************************************************/
unsigned char vmc_gprs_poll(unsigned char mode)
{
	unsigned char len,err;
	char buf[L_POLL+1] = {'0'};
	msg_head_config(GPRS_H_NOINIT);
	zhihui_msg.len = GPRS_HEAD - 4  + L_POLL;
	zhihui_msg.direction= 'R';
	zhihui_msg.contrl = G_POLL;
	len = vmc_integ_str(buf,L_POLL);
	if(mode == 1)
	{	
		if(trade_cache_head.buf_empty)
		{
			trade_cache_head.len = len;
			trade_cache_head.type = GPRS_T_POLL;
			//memcpy(trade_cache_head.node,zhihui_msg.send_msg,len);
			trade_cache_head.buf_empty = FALSE;	//�޸Ļ�������־ �ǿ�	
		}
		return 0;
	}

	err = vmc_gprs_send(zhihui_msg.send_msg,len);
	Trace("vmc_gprs_poll:send err =%d\r\n",err);
	err = vmc_gprs_recv(zhihui_msg.recv_msg,&zhihui_msg.recv_len,TIMES);
	Trace("vmc_gprs_poll:recv err =%d\r\n",err);
	if(err == GPRS_A_SUCCEE)
	{					
		return 1;	
	}
	return 0;
	
}
/********************************************************************************************************
** Function name:     	vmc_gprs_fualt
** Descriptions:	   	����
** input parameters:    ��
** output parameters:   ��
** Returned value:      1,�ɹ�  0ʧ��
*********************************************************************************************************/
unsigned char vmc_gprs_fualt(unsigned char mode)
{
	unsigned char len,i,rcx,err,state;
	char buf[L_FAULT] = {'0'};
	msg_head_config(GPRS_H_NOINIT);
	zhihui_msg.direction = 'R';
	zhihui_msg.contrl = G_FUALT;
	zhihui_msg.len = GPRS_HEAD - 4  + L_FAULT;
	buf[0] = MACData.driver_state ;
	
	for(i = 0;i < 8;i++)
	{
		buf[i+1] = (MACData.vend_mac_state >> (7-i)) & 0x01;
		buf[i+9] = (MACData.mdb_mac_state >> (7-i)) & 0x01;
		buf[i+17] = (MACData.coin_mac_state >> (7-i)) & 0x01;
	}
	for(i = 0;i < 24;i++)
	{
		state = getAisleInfo(vmc_goods[i].logic_no,AISLE_STATE);
		if(state == 1)
		{
			buf[i+25] = 0;
		}
		else if(state == 2)
			buf[i+25] = 1;
		else if(state == 3)
			buf[i+25] = 1;
		else;
		//zhihui_msg.msg[i+25] = (MACData.aisle_without[i / 8] >> (i % 8)) & 0x01;
	}
	for(i = 0;i < L_FAULT;i++)
	{
		//zhihui_msg.msg[i] = i % 10;
		sprintf((char *)&zhihui_msg.send_msg[i],"%d",buf[i]);	
		//printf("zhihui_msg.send_msg[%d] = %d\n",i,zhihui_msg.send_msg[i]);
	}
	sprintf(buf,"%*.*s",L_FAULT,L_FAULT,zhihui_msg.send_msg);
	//printf("G_FUALT\n");
	len = vmc_integ_str(buf,L_FAULT);
	Trace("fault integ_str len=%d",len);
	if(mode == 1)//��flash��������
	{						 
		//trade_cache_head.fault_req = TRUE;
		trade_cache_head.len = len;
		trade_cache_head.type = GPRS_T_FAULT;
		//memcpy(trade_cache_head.node,zhihui_msg.send_msg,len);
		return 1; 					
	}

	rcx = 2;
retry:
	Trace("Gprs_Send_fualt\r\n");
	err = vmc_gprs_send(zhihui_msg.send_msg,len);
	if(err != 1)
	{
		Trace("Send_fualt fail\r\n");
	 	return 0;
	}

	err = vmc_gprs_recv(zhihui_msg.recv_msg,&zhihui_msg.recv_len,TIMES);
	if(err == GPRS_A_SUCCEE)
	{				
		return 1;	
	}
	else if(err == GPRS_A_ERR)
	{
		if(rcx--)
			goto retry;
	}
	return 0;
	
}
/********************************************************************************************************
** Function name:     	vmc_gprs_run
** Descriptions:	   	����
** input parameters:    ��
** output parameters:   ��
** Returned value:      1,�ɹ�  0ʧ��
*********************************************************************************************************/
unsigned char vmc_gprs_run(unsigned char mode)
{
	unsigned char i,len,rcx,err;
	char buf[L_RUN] = {'0'};
	msg_head_config(GPRS_H_NOINIT);
	zhihui_msg.direction = 'R';
	zhihui_msg.contrl = G_RUN;
	zhihui_msg.len = GPRS_HEAD - 4  + L_RUN;
	memset(zhihui_msg.send_msg,0,8);
	//memset(buf,'0',L_RUN);

	zhihui_msg.send_msg[6] = (MACData.run_state >> 1) & 0x01; //��������״̬ 1�Ŵ�
	zhihui_msg.send_msg[7] = (MACData.run_state ) & 0x01;    //Ӫҵ״̬ 1��ͣӪҵ
	for(i = 0;i < 8;i++)
	{
		sprintf((char *)&buf[i],"%d",zhihui_msg.send_msg[i]);
	}
	sprintf((char *)&buf[8],"%04.4d",MACData.coin_num1);
	sprintf((char *)&buf[12],"%04.4d",MACData.coin_num2);
	sprintf((char *)&buf[16],"%04.4d",MACData.bill_num);
	for(i = 0;i < 24;i++)
	{
		sprintf((char *)&buf[20+i*2],"%02.2d",0);
	}	
	//printf("G_RUN\n");
	len = vmc_integ_str(buf,L_RUN);
	if(mode == 1)
	{
	
		trade_cache_head.len = len;
		trade_cache_head.type = GPRS_T_RUN;
		//memcpy(trade_cache_head.node,zhihui_msg.send_msg,len);
		return 1; 
	
	}

	rcx = 2;
retry:
	Trace("Gprs_Send_run\r\n");
	err = vmc_gprs_send(zhihui_msg.send_msg,len);
	 if(err != 1)
	{
		Trace("Send_run fail\r\n");
	 	return 0;
	}
	err = vmc_gprs_recv(zhihui_msg.recv_msg,&zhihui_msg.recv_len,TIMES);
	if(err == GPRS_A_SUCCEE)
	{		
		return 1;	
	}
	else if(err == GPRS_A_ERR)
	{
		if(rcx--)
			goto retry;
	}
	return 0;

	
}
/********************************************************************************************************
** Function name:     	vmc_gprs_config
** Descriptions:	   	����
** input parameters:    ��
** output parameters:   ��
** Returned value:      1,�ɹ�  0ʧ��
*********************************************************************************************************/
unsigned char vmc_gprs_config(unsigned char mode)
{
	unsigned char i,len,rcx,err;
	char buf[L_CONFIG] = {'0'};
	msg_head_config(GPRS_H_NOINIT);
	zhihui_msg.direction = 'R';
	zhihui_msg.contrl = G_CONFIG;
	zhihui_msg.len = GPRS_HEAD - 4  + L_CONFIG;
//	memset(buf,'0',L_CONFIG);

	buf[6] = '0';//MACData.sell_config
	buf[7] = '0';//�����Ա�0-��  1-ǿ��
	//����
	sprintf((char *)&buf[8],"%02.2x",MACData.light_state);
	for(i = 0;i < 4;i++)
	{
		sprintf((char *)&buf[10+i*2],"%02.2x",MACData.lamp_time[i]);
		
	}	
	sprintf((char *)&buf[18],"%02.2x",MACData.left_room_state);
	sprintf((char *)&buf[20],"%02.2x",MACData.right_room_state);
	for(i = 0;i < 4;i++)
		sprintf((char *)&buf[22+i*2],"%02.2x",MACData.energy_save_time[i]);
	buf[30] = ((MACData.cool_temp[0] & 0x80) == 0) ? '+' : '-' ;
	sprintf((char *)&buf[31],"%02.2x",MACData.cool_temp[0] & 0x7F);
	buf[33] = ((MACData.cool_temp[1] & 0x80) == 0) ? '+' : '-' ;
	sprintf((char *)&buf[34],"%02.2x",(MACData.cool_temp[1] & 0x7F));
	buf[36] = ((MACData.hot_temp[0] & 0x80) == 0) ? '+' : '-' ;
	sprintf((char *)&buf[37],"%02.2x",(MACData.hot_temp[0] & 0x7F));
	buf[39] = ((MACData.hot_temp[1] & 0x80) == 0) ? '+' : '-' ;
	sprintf((char *)&buf[40],"%02.2x",MACData.hot_temp[1] & 0x7F);
	//printf("%c\n",zhihui_msg.msg[22]);
	//printf("%-042s\n",zhihui_msg.msg);
	//printf("G_CONFIG\n");
	len = vmc_integ_str(buf,L_CONFIG);
	if(mode == 1)
	{		
		//trade_cache_head.config_req = TRUE;
		trade_cache_head.len = len;
		trade_cache_head.type = GPRS_T_CONFIG;		
		//memcpy(trade_cache_head.node,zhihui_msg.send_msg,len);
		return 1; 

	}

	rcx = 2;
retry:
	Trace("Gprs_Send_config\r\n");
	err = vmc_gprs_send(zhihui_msg.send_msg,len);
	if(err != 1)
	{
		Trace("Send_config fail\r\n");
	 	return 0;
	}
	err = vmc_gprs_recv(zhihui_msg.recv_msg,&zhihui_msg.recv_len,TIMES);
	if(err == GPRS_A_SUCCEE)
	{					
		return 1;	
	}
	else if(err == GPRS_A_ERR)
	{
		if(rcx--)
			goto retry;
	}
	return 0;
	
}
/********************************************************************************************************
** Function name:     	vmc_gprs_aisle
** Descriptions:	   	�������
** input parameters:    ��
** output parameters:   ��
** Returned value:      1,�ɹ�  0ʧ��
*********************************************************************************************************/
unsigned char vmc_gprs_aisle(unsigned char mode)
{
	unsigned char i,len,rcx,err;
	char buf[L_AISLE] = {'0'};
	msg_head_config(GPRS_H_NOINIT);
	zhihui_msg.direction = 'R';
	zhihui_msg.contrl = G_AISLE;
	zhihui_msg.len = GPRS_HEAD - 4  + L_AISLE;
	//memset(buf,'0',L_AISLE);
	
	for(i = 0;i < 24;i++)
	{
		sprintf((char *)&buf[i*2],"%02.2x",
		getAisleInfo(vmc_goods[i].logic_no,AISLE_ID));
	}
	for(i = 0;i < 24;i++)
	{
		sprintf((char *)&buf[48+i*5],"%05.5d",
		getAisleInfo(vmc_goods[i].logic_no,AISLE_PRICE));
	}
	//printf("%s\n",zhihui_msg.msg);
	//printf("G_AISLE\n");
	len = vmc_integ_str(buf,L_AISLE);
	if(mode == 1)
	{
	
		trade_cache_head.len = len;
		trade_cache_head.type = GPRS_T_AISLE;
		//memcpy(trade_cache_head.node,zhihui_msg.send_msg,len);
		return 1;
			
	}

	rcx = 2;
retry:
	Trace("Gprs_Send_aisle\r\n");
	err = vmc_gprs_send(zhihui_msg.send_msg,len);
	if(err != 1)
	{
		Trace("send aisle fail \r\n");
		return 0;
	}
	err = vmc_gprs_recv(zhihui_msg.recv_msg,&zhihui_msg.recv_len,TIMES);
	if(err == GPRS_A_SUCCEE)
	{					
		return 1;	
	}
	else if(err == GPRS_A_ERR)
	{
		if(rcx--)
			goto retry;
	}
	return 0;
}


/********************************************************************************************************
** Function name:     	vmc_gprs_goods
** Descriptions:	   	���������Ϊʵʱ�ϱ������� �������
** input parameters:    ��
** output parameters:   ��
** Returned value:      1,�ɹ�  0ʧ��
*********************************************************************************************************/
unsigned char vmc_gprs_goods(unsigned char num,unsigned char mode)
{
	unsigned char i,len,rcx,err,off;
	char buf[L_GOODS] = {'0'};
	msg_head_config(GPRS_H_NOINIT);
	zhihui_msg.direction = 'R';
	zhihui_msg.contrl = G_GOODS;
	zhihui_msg.len = GPRS_HEAD - 4  + L_GOODS;



	sprintf((char *)&buf[0],"%04.4d",RTCData.year);
	sprintf((char *)&buf[4],"%02.2d",RTCData.month);
	sprintf((char *)&buf[6],"%02.2d",RTCData.day);
	sprintf((char *)&buf[8],"%02.2d",RTCData.hour);
	sprintf((char *)&buf[10],"%02.2d",RTCData.minute);
	sprintf((char *)&buf[12],"%02.2d",RTCData.second);
	off = 14;
	sprintf((char *)&buf[off + 0],"%02.2d",num);
	sprintf((char *)&buf[off + 2],"%02.2x",getAisleInfo(vmc_goods[num-1].logic_no,AISLE_ID));
	sprintf((char *)&buf[off + 4],"%05.5d",getAisleInfo(vmc_goods[num-1].logic_no,AISLE_PRICE));
	
	sprintf((char *)&buf[off + 9],"%03.3d",MACData.trade_run_no++);//�ۻ������к�1-255
	
	sprintf((char *)&buf[off + 12],"%02.2d",MACData.trade_pay_mode);
	sprintf((char *)&buf[off + 14],"%01.1d",MACData.trade_state);
	for(i = 0;i < 10;i++)
	{		
		sprintf((char *)&buf[off + 15 + i*2],"%02.2x",MACData.trade_card_no[0]);
	}

	sprintf((char *)&buf[off + 35],"%06.6d",MACData.trade_gprs_rum_num++);//GPRS ���к�1-255


	sprintf((char *)&buf[off + 41],"%08.8d",LogPara.vpSuccessNumTotal);
	sprintf((char *)&buf[off + 49],"%010.10d",LogPara.vpSuccessMoneyTotal);
	#if 1
	sprintf((char *)&buf[off + 59],"%04.4d",LogParaDetail.BeginYear);
	sprintf((char *)&buf[off + 63],"%02.2d",LogParaDetail.BeginMonth);
	sprintf((char *)&buf[off + 65],"%02.2d",LogParaDetail.BeginDay);
	sprintf((char *)&buf[off + 67],"%02.2d",LogParaDetail.BeginHour);
	sprintf((char *)&buf[off + 69],"%02.2d",LogParaDetail.GoodsMin[MACData.read_sub_index]);
	sprintf((char *)&buf[off + 71],"%02.2d",LogParaDetail.GoodsSec[MACData.read_sub_index]);
	#endif
	#if 0
	sprintf((char *)&zhihui_msg.msg[off + 59],"%04.4d",RTCData.year);
	sprintf((char *)&zhihui_msg.msg[off + 63],"%02.2d",RTCData.month);
	sprintf((char *)&zhihui_msg.msg[off + 65],"%02.2d",RTCData.day);
	sprintf((char *)&zhihui_msg.msg[off + 67],"%02.2d",RTCData.hour);
	sprintf((char *)&zhihui_msg.msg[off + 69],"%02.2d",RTCData.minute);
	sprintf((char *)&zhihui_msg.msg[off + 71],"%02.2d",RTCData.second);
	#endif	
	len = vmc_integ_str(buf,L_GOODS);	
	if(mode == 1)
	{
		zhihui_msg.send_len = len;
		return 1;	
	}	
	rcx = 2;
retry:
	err = vmc_gprs_send(zhihui_msg.send_msg,len);
	if(err != 1)
	{
		Trace("send goods fail \r\n");
		return 0;
	}
	err = vmc_gprs_recv(zhihui_msg.recv_msg,&zhihui_msg.recv_len,TIMES);
	if(err == GPRS_A_SUCCEE)
	{	
		
		return 1;	
	}
	else if(err == GPRS_A_ERR)
	{
		if(rcx--)
			goto retry;
	}
	return 0;
}

/********************************************************************************************************
** Function name:     	vmc_gprs_total
** Descriptions:	   	����
** input parameters:    ��
** output parameters:   ��
** Returned value:      1,�ɹ�  0ʧ��
*********************************************************************************************************/
unsigned char vmc_gprs_total(void)
{
	unsigned char len,i,rcx,err,index = 0;
	char buf[L_TOTAL]= {'0'},data[BATCH_FLASH_SIZE] = {0};
	unsigned int temp;
	msg_head_config(GPRS_H_NOINIT);
	zhihui_msg.direction = 'R';
	zhihui_msg.contrl = G_TOTAL;
	zhihui_msg.len = GPRS_HEAD - 4  + L_TOTAL;

	sprintf((char *)&buf[0],"%04.4d",RTCData.year);
	sprintf((char *)&buf[4],"%02.2d",RTCData.month);
	sprintf((char *)&buf[6],"%02.2d",RTCData.day);
	sprintf((char *)&buf[8],"%02.2d",RTCData.hour);
	sprintf((char *)&buf[10],"%02.2d",RTCData.minute);
	sprintf((char *)&buf[12],"%02.2d",RTCData.second);
	index += 14;

	if(vmc_batch_info_read((unsigned char *)data) != 1)
		return 0;
	//�ܱ���
	temp = INTEG32(0,data[0],data[1],data[2]);
	sprintf((char *)&buf[index],"%08.8d",temp);
	index += 8;

	//�ܽ��
	temp = INTEG32(data[3],data[4],data[5],data[6]);
	sprintf((char *)&buf[index],"%010.10d",temp);
	index += 10;
	
	//ˢ���ܱ���
	temp = INTEG32(0,data[14],data[15],data[16]);
	sprintf((char *)&buf[index],"%08.8d",temp);
	index += 8;
	//ˢ���ܽ��
	temp = INTEG32(data[17],data[18],data[19],data[20]);
	sprintf((char *)&buf[index],"%010.10d",temp);
	index += 10;
	//���α���
	temp = INTEG32(0,0,data[49],data[50]);
	sprintf((char *)&buf[index],"%04.4d",temp);
	index += 4;
	//���ν��
	temp = 	INTEG32(0,data[51],data[52],data[53]);
	sprintf((char *)&buf[index],"%08.8d",temp);
	index += 8;
	//�����ֽ����
	temp = 	INTEG32(0,0,data[54],data[55]);
	sprintf((char *)&buf[index],"%04.4d",temp);
	index += 4;
	//�����ֽ���
	temp = 	INTEG32(0,data[56],data[57],data[58]);
	sprintf((char *)&buf[index],"%08.8d",temp);
	index += 8;
	//���ο�����
	temp = 	INTEG32(0,0,data[59],data[60]);
	sprintf((char *)&buf[index],"%04.4d",temp);
	index += 4;
	
    temp = 	INTEG32(0,data[61],data[62],data[63]);
	sprintf((char *)&buf[index],"%08.8d",temp);
	index += 8;

	temp = 	INTEG32(0,0,data[64],data[65]);
	sprintf((char *)&buf[index],"%04.4d",temp);
	index += 4;
	
	temp = 	INTEG32(0,data[66],data[67],data[68]);
	sprintf((char *)&buf[index],"%08.8d",temp);
	index += 8;

	for(i = 0;i < 24;i++)
	{
		
		sprintf((char *)&buf[index+i*2],"%02.2d",
		(unsigned char)getAisleInfo(vmc_goods[i].logic_no,AISLE_SELL_NUM));
	}
	index += 48;
	
	sprintf((char *)&buf[index],"%02.2d",BCD_to_HEC(data[69]));
	index += 2;
	sprintf((char *)&buf[index],"%02.2d",BCD_to_HEC(data[70]));
	index += 2;
	sprintf((char *)&buf[index],"%02.2d",BCD_to_HEC(data[71]));
	index += 2;
	sprintf((char *)&buf[index],"%02.2d",BCD_to_HEC(data[72]));
	index += 2;
	sprintf((char *)&buf[index],"%02.2d",BCD_to_HEC(data[73]));
	index += 2;
	sprintf((char *)&buf[index],"%02.2d",BCD_to_HEC(data[74]));
	index += 2;
	sprintf((char *)&buf[index],"%02.2d",BCD_to_HEC(data[75]));
	index += 2;
	
	len = vmc_integ_str(buf,L_TOTAL);

	//Trace("send total:%s\r\n",zhihui_msg.send_msg);
	rcx = 2;
retry:
	err = vmc_gprs_send(zhihui_msg.send_msg,len);
	if(err != 1)
	{
		Trace("send sum fail \r\n");
		return 0;
	}
	err = vmc_gprs_recv(zhihui_msg.recv_msg,&zhihui_msg.recv_len,TIMES);
	if((err == GPRS_A_SUCCEE) || (err == GPRS_A_GEN_ERR))
	{				
		return 1;	
	}
	else if(err == GPRS_A_ERR)
	{
		if(rcx--)
			goto retry;
	}
	return 0;
}

unsigned char vmc_gprs_trade_rpt(unsigned short index)
{

	MACData.write_index = index;
	trade_cache_head.run_req = TRUE;
	return 1;
}



/********************************************************************************************************
** Function name:     	gprs_weihu_rpt
** Descriptions:	   	GPRSά��״̬�ϱ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void gprs_weihu_rpt(unsigned char type)
{

}

/********************************************************************************************************
** Function name:     	vmc_gprs_setup
** Descriptions:	   	GPRSЭ���ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
unsigned char vmc_gprs_setup(void)
{
	unsigned char err,i;

retry:
	vmc_gprs_register(1);//ѭ��ע��
	zh_all_update();
#if 1
	for(i = 0;i < 10;i++)
	{
	   	OSTimeDly(2000/5);
		err = vmc_gprs_total();
		if(err == 1) //���ָ�1111
			break;
	}
	if(i >= 10)
		goto retry;
	for(i = 0;i < 10;i++)
	{
		OSTimeDly(2000/5);
		err = vmc_gprs_fualt(0);
		if(err == 1)
			break;		
	}
	if(i >= 10)
		goto retry;
	
	for(i = 0;i < 10;i++)
	{
		OSTimeDly(2000/5);
		err = vmc_gprs_run(0);
		if(err == 1)
			break;	
	}
	if(i >= 10)
		goto retry;
	
	for(i = 0;i < 10;i++)
	{
		OSTimeDly(2000/5);
		err = vmc_gprs_config(0);
		if(err == 1)
			break;	
	}
	if(i >= 10)
		goto retry;
	
	for(i = 0;i < 10;i++)
	{
		OSTimeDly(2000/5);
		err = vmc_gprs_aisle(0);
		if(err == 1)
			break;
	}
	if(i >= 10)
		goto retry;
	

#endif
	return 1;


}



/********************************************************************************************************
** Function name:     	task3_gprs_poll
** Descriptions:	 gprs��ѯ
** input parameters:    ��
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void task3_gprs_poll(void)
{
	unsigned char err;
	MESSAGE_ZHIHUI *accepter_msg;
	static unsigned char gprs_tick,gprs_sub_tick; 
	if(trade_cache_head.com_err_num > 10)
	{	
		Trace("\r\ngprs offline........\r\n");
		//֪ͨ���ذ��������ģʽ
		zh_vmc_post(MBOX_ZH_VMC_OFFLINE);
		while(mvc_gprs_init() != 1)
		{
			Trace("GPRS_Init...\r\n");
			OSTimeDly(3000/5);
		}
		Trace("GPRS_Init suc!!!\r\n");
		trade_cache_head.com_err_num = 0;
		trade_cache_head.recv_data = TRUE;	
		vmc_gprs_setup();
		//֪ͨ���ذ��������ģʽ
		Trace("\r\n gprs online....\r\n");
		zh_vmc_post(MBOX_ZH_VMC_ONLINE);

		//accepter_msg = OSMboxPend(g_PCMail,5000,&err);
		accepter_msg = OSQPend(g_PCMail,5000,&err);
		if(err == OS_NO_ERR)
		{
			if(accepter_msg->vmc_type == MBOX_VMC_ZH_OFFLINE_REPORT)
			{	
				Trace("gprs recv mbox...\r\n"); 
				MACData.write_index = accepter_msg->trade_index;
			
			}
		}
	}	

	if(!MACData.usr_trade && !MACData.isWeihu)
	{	
		if(!(gprs_tick % 5))
		{							
			ship_cache_poll();//�����û�ϱ��ĳ���ָ�����ϱ�	
			Trace("\r\npoll:buf_empty=%d,ask=%d,read_index=%d,read_sub_index=%d,write_index=%d,fault=%d,run=%d,config=%d,aisle=%d,poll=%d,com_err=%d\r\n",
				trade_cache_head.buf_empty,trade_cache_head.recv_data,MACData.read_index,
				MACData.read_sub_index,MACData.write_index,trade_cache_head.fault_req,
				trade_cache_head.run_req,trade_cache_head.config_req,trade_cache_head.aisle_req,
				trade_cache_head.poll_req,trade_cache_head.com_err_num);
			
		}		
		if(gprs_tick >= 250) //599 ÿ3������ѯ������ 99�����
		{
			gprs_tick = 0;
			gprs_sub_tick++;
			trade_cache_head.poll_req = TRUE;
		}
		if(gprs_sub_tick>=20) //20 ÿ60���� �ϱ�����״̬��������
		{
			gprs_sub_tick = 0;
			zh_all_update();
			trade_cache_head.fault_req = TRUE;
			trade_cache_head.run_req = TRUE;
			trade_cache_head.config_req = TRUE;
			trade_cache_head.aisle_req = TRUE;
		}
	}
	gprs_tick++;

	
}

