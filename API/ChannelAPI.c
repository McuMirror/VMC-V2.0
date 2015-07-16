/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           ChannelAccepterAPI.C
** Last modified Date:  2013-03-05
** Last Version:         
** Descriptions:        ��������API�ӿ�                     
**------------------------------------------------------------------------------------------------------
** Created by:          liya
** Created date:        2013-03-05
** Version:             V2.0
** Descriptions:        The original version
**------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
********************************************************************************************************/
#include "..\config.h"



//����ͨ������ĶԷ�������
extern OS_EVENT *g_CmdMsg;
extern OS_EVENT *g_CmdBackMsg;
//�����������ݽṹ
extern MessagePack MsgPack;



//8*8���������ַ�� 
const unsigned char huodao_addr_8_8[8][10]={
	{57,58,59,60,61,62,63,64, 0, 0}, //  ��һ�� 
	{49,50,51,52,53,54,55,56, 0, 0}, 
	{41,42,43,44,45,46,47,48, 0, 0},
	{33,34,35,36,37,38,39,40, 0, 0},
	{25,26,27,28,29,30,31,32, 0, 0},
	{17,18,19,20,21,22,23,24, 0, 0},
	{ 9,10,11,12,13,14,15,16, 0, 0},
	{ 1, 2, 3, 4, 5, 6, 7, 8, 0, 0}  //  �ڰ˲� 
};


//8*10���������ַ��
const unsigned char huodao_addr_8_10[8][10]={
	{71,72,73,74,75,76,77,78,79,80}, //  ��һ�� 
	{61,62,63,64,65,66,67,68,69,70}, 
	{51,52,53,54,55,56,57,58,59,60},
	{41,42,43,44,45,46,47,48,49,50},
	{31,32,33,34,35,36,37,38,39,40},
	{21,22,23,24,25,26,27,28,29,30},
	{11,12,13,14,15,16,17,18,19,20},
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9,10}   //  �ڰ˲� 
};

//����洢flash ҳ�� ����
const unsigned short cabinet1_page[4] = {
		HD_CABINET1_FLASH_1,
		HD_CABINET1_FLASH_2,
		HD_CABINET1_FLASH_3,
		HD_CABINET1_FLASH_4	
};
const unsigned short cabinet2_page[4] = {
		HD_CABINET2_FLASH_1,
		HD_CABINET2_FLASH_2,
		HD_CABINET2_FLASH_3,
		HD_CABINET2_FLASH_4	
};


//����洢flash ����ҳ�� ����
const unsigned short cabinet1_page_bak[4] = {
		HD_CABINET1_FLASH_1_BAK,
		HD_CABINET1_FLASH_2_BAK,
		HD_CABINET1_FLASH_3_BAK,
		HD_CABINET1_FLASH_4_BAK
};
const unsigned short cabinet2_page_bak[4] = {
		HD_CABINET2_FLASH_1_BAK,
		HD_CABINET2_FLASH_2_BAK,
		HD_CABINET2_FLASH_3_BAK,
		HD_CABINET2_FLASH_4_BAK
};






ST_HUODAO_KAOJI stHuodaoKaoji[2];


#pragma arm section zidata = "RAM2"

ST_LEVEL_HUODAO stHuodao[HUODAO_MAX_ROW];
ST_LEVEL_HUODAO stSubHuodao[HUODAO_MAX_ROW];


#pragma arm section zidata






//========================================================================
//�����������ӿں��� hd -- huodao ���к�������hd��ͷ
//by yoc  2014.04.01
//=============================================================================




/*****************************************************************************
** Function name:	hd_init	
** Descriptions:	      ���³�ʼ����������														 			
** parameters:		cabinetNo  ��� 1 ���� 2 ����					
** Returned value:	��
*******************************************************************************/
void hd_init(unsigned char cabinetNo)
{
	unsigned char i,j;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	unsigned char (*huodao_table)[8][10];
	
	if(cabinetNo == 1)
	{
		stHuodaoPtr = stHuodao;
		if(SystemPara.ChannelType == 1)//8 * 10
			huodao_table = (unsigned char (*)[8][10])huodao_addr_8_10;					
		else
			huodao_table = (unsigned char (*)[8][10])huodao_addr_8_8;
	}	
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
	{
		stHuodaoPtr = stSubHuodao;
		if(SystemPara.SubBinChannelType == 1)//8 * 10
			huodao_table = (unsigned char (*)[8][10])huodao_addr_8_10;						
		else
			huodao_table = (unsigned char (*)[8][10])huodao_addr_8_8;	
	}	
	else
	{
		hefanHuodaoInit(cabinetNo);
		return;
	}
	
	//��ʼ������
	for(i = 0;i < HUODAO_MAX_ROW;i++)
	{
		stHuodaoPtr[i].openFlag = 1;
		for(j = 0;j < HUODAO_MAX_COLUMN;j++)
		{
			//��ʼ������������
			if(j >= 9)
				stHuodaoPtr[i].huodao[j].physicNo = (i + 1) * 10;
			else
				stHuodaoPtr[i].huodao[j].physicNo = (i + 1) * 10 + j + 1;

			stHuodaoPtr[i].huodao[j].logicNo = stHuodaoPtr[i].huodao[j].physicNo;
			//��ʼ����ʵ���������ַ
			stHuodaoPtr[i].huodao[j].physicAddr = (*huodao_table)[i][j];
			//�������������ַ�������Ƿ����˻���
			stHuodaoPtr[i].huodao[j].openFlag = (stHuodaoPtr[i].huodao[j].physicAddr) ? 1 : 0;
			stHuodaoPtr[i].huodao[j].id = 0;
			stHuodaoPtr[i].huodao[j].count = 10;
			stHuodaoPtr[i].huodao[j].maxCount = 10;
			stHuodaoPtr[i].huodao[j].sucCount = 0;
			stHuodaoPtr[i].huodao[j].state = 1;
			stHuodaoPtr[i].huodao[j].selectNo = 0;
			stHuodaoPtr[i].huodao[j].price = 100;
			
		}
	}
	
	print_hd("cabinet[%d]:InitOver..\r\n",cabinetNo);
	
}


/*****************************************************************************
** Function name:	hd_logic_init	
** Descriptions:	      ���³�ʼ�������߼�����													 			
** parameters:		cabinetNo  ��� 1 ���� 2 ����	
** Returned value:	1 �ɹ� 0 ʧ��
*******************************************************************************/
unsigned char  hd_logic_init(unsigned char cabinetNo)
{
	unsigned char i,j,r = 0,c = 0;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		print_err(cabinetNo);
		return 0;
	}
	
	for(i = 0;i < HUODAO_MAX_ROW;i++)
	{
		for(j = 0,c = 0;j < HUODAO_MAX_COLUMN;j++)
		{
			if(stHuodaoPtr[i].huodao[j].openFlag && stHuodaoPtr[i].openFlag && 
				stHuodaoPtr[i].huodao[j].physicAddr)
			{
				if(c >= 9)
					stHuodaoPtr[i].huodao[j].logicNo = (r + 1) * 10;	
				else
					stHuodaoPtr[i].huodao[j].logicNo = (r + 1) * 10 + 1 + c;
				c++;
			}
			else
				stHuodaoPtr[i].huodao[j].logicNo = 0;	
		}
		
		if(stHuodaoPtr[i].openFlag && c)
			r++;
		
	}

	return 1;
	
}


/*****************************************************************************
** Function name:hd_save_param_bak	
** Descriptions:	�������������ݱ��浽FLASH������	
**				�����������ṹ�峤�ȷֳ�4�� ��4ҳ�洢
**Input parameters:  cabinetNo ���
** Returned value:	1 �ɹ�  0 ʧ��
*******************************************************************************/
unsigned char hd_save_param_bak(unsigned char cabinetNo)
{
	//��4ҳ�洢��������ǰ��ҳ�洢����Ϊlen_th,���һҳ����Ϊlen_lh
	unsigned short crc,len_th,len_lh;
	unsigned char buf[512]={0},*hdPtr,i;
	unsigned short *pageNo;
	print_hd("SaveChannel_Start..\r\n");

	if(cabinetNo == 1)
	{
		hdPtr = (unsigned char *)stHuodao;
		pageNo = (unsigned short *)cabinet1_page_bak;
	}
		
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
	{
		hdPtr = (unsigned char *)stSubHuodao;
		pageNo = (unsigned short *)cabinet2_page_bak;
	}	
	else
	{
		print_err(cabinetNo);
		return 0;
	}
	len_th = (sizeof(ST_LEVEL_HUODAO) * HUODAO_MAX_ROW) / 4;
	len_lh = (sizeof(ST_LEVEL_HUODAO) * HUODAO_MAX_ROW) - len_th * 3;

	//�洢ǰ��ҳ
	for(i = 0;i < 3;i++)
	{
		memset(buf,0,512);
		memcpy(buf,&hdPtr[len_th * i],len_th);
		crc = CrcCheck(buf,len_th);
		buf[len_th] = HUINT16(crc);
		buf[len_th + 1] = LUINT16(crc);	
		AT45DBWritePage(pageNo[i],buf);
	}	
	//�洢����ҳ
	memset(buf,0,512);
	memcpy(buf,&hdPtr[len_th * 3],len_lh);
	crc = CrcCheck(buf,len_lh);
	buf[len_lh] = HUINT16(crc);
	buf[len_lh + 1] = LUINT16(crc);	
	AT45DBWritePage(pageNo[3],buf);

	print_hd("SaveChannel_OK..\r\n");	
	return 1;
	
	
}





/*****************************************************************************
** Function name:hd_save_param	
** Descriptions:	�������������ݱ��浽FLASH��	
**				�����������ṹ�峤�ȷֳ�4�� ��4ҳ�洢
**Input parameters:  cabinetNo ���
** Returned value:	1 �ɹ�  0 ʧ��
*******************************************************************************/
unsigned char hd_save_param(unsigned char cabinetNo)
{
	//��4ҳ�洢��������ǰ��ҳ�洢����Ϊlen_th,���һҳ����Ϊlen_lh
	unsigned short crc,len_th,len_lh;
	unsigned char buf[512]={0},*hdPtr,i;
	unsigned short *pageNo;
	print_hd("SaveChannel_Start..\r\n");

	if(cabinetNo == 1)
	{
		hdPtr = (unsigned char *)stHuodao;
		pageNo = (unsigned short *)cabinet1_page;
	}
		
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
	{
		hdPtr = (unsigned char *)stSubHuodao;
		pageNo = (unsigned short *)cabinet2_page;
	}	
	else
	{
		print_err(cabinetNo);
		return 0;
	}
	len_th = (sizeof(ST_LEVEL_HUODAO) * HUODAO_MAX_ROW) / 4;
	len_lh = (sizeof(ST_LEVEL_HUODAO) * HUODAO_MAX_ROW) - len_th * 3;

	//�洢ǰ��ҳ
	for(i = 0;i < 3;i++)
	{
		memset(buf,0,512);
		memcpy(buf,&hdPtr[len_th * i],len_th);
		crc = CrcCheck(buf,len_th);
		buf[len_th] = HUINT16(crc);
		buf[len_th + 1] = LUINT16(crc);	
		AT45DBWritePage(pageNo[i],buf);
	}	
	//�洢����ҳ
	memset(buf,0,512);
	memcpy(buf,&hdPtr[len_th * 3],len_lh);
	crc = CrcCheck(buf,len_lh);
	buf[len_lh] = HUINT16(crc);
	buf[len_lh + 1] = LUINT16(crc);	
	AT45DBWritePage(pageNo[3],buf);

	print_hd("SaveChannel_OK..\r\n");	
	return 1;
	
	
}


/*****************************************************************************
** Function name:	hd_read_param	
** Descriptions:	��flash�ж�ȡ��������															 			
** parameters:		��					
** Returned value:	��
** 
*******************************************************************************/
unsigned char hd_read_param(unsigned char cabinetNo)
{

	unsigned short crc,len_th,len_lh;
	unsigned char buf[512]={0},*hdPtr,i,err;
	unsigned short *pageNo;
	print_hd("SaveChannel_Start..\r\n");

	if(cabinetNo == 1)
	{
		hdPtr = (unsigned char *)stHuodao;
		pageNo = (unsigned short *)cabinet1_page;
	}
		
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
	{
		hdPtr = (unsigned char *)stSubHuodao;
		pageNo = (unsigned short *)cabinet2_page;
	}	
	else
	{
		print_err(cabinetNo);
		return 0;
	}
	len_th = (sizeof(ST_LEVEL_HUODAO) * HUODAO_MAX_ROW) / 4;
	len_lh = (sizeof(ST_LEVEL_HUODAO) * HUODAO_MAX_ROW) - len_th * 3;


	//��ȡǰ��ҳ
	for(i = 0;i < 3;i++)
	{
		memset(buf,0,512);
		err = AT45DBReadPage(pageNo[i],buf);
		if(err)
		{
			crc = CrcCheck(buf,len_th);
			if(crc == INTEG16(buf[len_th],buf[len_th + 1]))
			{
				memcpy(&hdPtr[len_th * i],buf,len_th);	//�ɹ���ȡһҳ
				continue;
			}
		}
		//��ȡʧ��
		print_hd("read huodao faied! the pageNo is %d\r\n",pageNo[i]);
		return 0;		
	}	
	
	//��ȡ����ҳ
	memset(buf,0,512);
	err = AT45DBReadPage(pageNo[3],buf);
	if(err)
	{
		crc = CrcCheck(buf,len_lh);
		if(crc == INTEG16(buf[len_lh],buf[len_lh + 1]))
		{
			memcpy(&hdPtr[len_th * 3],buf,len_lh);	//�ɹ���ȡһҳ
			return 1;
		}
	}
	
	//��ȡʧ��
	print_hd("read huodao faied! the pageNo is %d\r\n",pageNo[3]);
	return 0;	
	
}



/*****************************************************************************
** Function name:	hd_read_param_bak	
** Descriptions:	��flash�����ж�ȡ��������															 			
** parameters:		��					
** Returned value:	��
** 
*******************************************************************************/
unsigned char hd_read_param_bak(unsigned char cabinetNo)
{

	unsigned short crc,len_th,len_lh;
	unsigned char buf[512]={0},*hdPtr,i,err;
	unsigned short *pageNo;
	print_hd("SaveChannel_Start..\r\n");

	if(cabinetNo == 1)
	{
		hdPtr = (unsigned char *)stHuodao;
		pageNo = (unsigned short *)cabinet1_page_bak;
	}
		
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
	{
		hdPtr = (unsigned char *)stSubHuodao;
		pageNo = (unsigned short *)cabinet2_page_bak;
	}	
	else
	{
		print_err(cabinetNo);
		return 0;
	}
	len_th = (sizeof(ST_LEVEL_HUODAO) * HUODAO_MAX_ROW) / 4;
	len_lh = (sizeof(ST_LEVEL_HUODAO) * HUODAO_MAX_ROW) - len_th * 3;


	//��ȡǰ��ҳ
	for(i = 0;i < 3;i++)
	{
		memset(buf,0,512);
		err = AT45DBReadPage(pageNo[i],buf);
		if(err)
		{
			crc = CrcCheck(buf,len_th);
			if(crc == INTEG16(buf[len_th],buf[len_th + 1]))
			{
				memcpy(&hdPtr[len_th * i],buf,len_th);	//�ɹ���ȡһҳ
				continue;
			}
		}
		//��ȡʧ��
		print_hd("read huodao faied! the pageNo is %d\r\n",pageNo[i]);
		return 0;		
	}	
	
	//��ȡ����ҳ
	memset(buf,0,512);
	err = AT45DBReadPage(pageNo[3],buf);
	if(err)
	{
		crc = CrcCheck(buf,len_lh);
		if(crc == INTEG16(buf[len_lh],buf[len_lh + 1]))
		{
			memcpy(&hdPtr[len_th * 3],buf,len_lh);	//�ɹ���ȡһҳ
			return 1;
		}
	}
	
	//��ȡʧ��
	print_hd("read huodao faied! the pageNo is %d\r\n",pageNo[3]);
	return 0;	
	
}



/*****************************************************************************
** Function name:	hd_get_index	
** Descriptions:		��ȡ�����ṹ������ ��														 			
** parameters:		type 1 ͨ���߼������Ż�ȡ������
						2 ͨ����������Ż�ȡ������
						3 ѡ��������
						4 ID��
						5 �����ַ�Ż�ȡ������
					rIndex ��������
					cIndex ����������
					no  �߼�������/���������/������/ID��
** Returned value:	1 �ɹ�  0 ʧ��
** 
*******************************************************************************/
unsigned char hd_get_index(unsigned char cabinetNo,unsigned char no,unsigned char type,
		unsigned char *rIndex,unsigned char *cIndex)
{
	unsigned char i,j,temp;
	ST_LEVEL_HUODAO *stHuodaoPtr;

	*rIndex = 0;
	*cIndex = 0;
	
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		print_err(cabinetNo);
		return 0;
	}

	if(!no)
	{
		print_err(no);
		return 0;
	}

	for(i = 0;i < HUODAO_MAX_ROW;i++)
	{
		for(j = 0;j < HUODAO_MAX_COLUMN;j++)
		{
			if(type == 1)
				temp = stHuodaoPtr[i].huodao[j].logicNo;
			else if(type == 2)
				temp = stHuodaoPtr[i].huodao[j].physicNo;
			else if(type == 3)
				temp = stHuodaoPtr[i].huodao[j].selectNo;
			else if(type == 4)
				temp = stHuodaoPtr[i].huodao[j].id;
			else if(type == 5)
				temp = stHuodaoPtr[i].huodao[j].physicAddr;
			else
				return 0;

			if(temp == no)
			{
				*rIndex = i;
				*cIndex = j;
				return 1;
			}
				
		}
	}

	return 0;
	
}



/*****************************************************************************
** Function name:	hd_open_handle	
** Descriptions:	      �������غ���
**                            ����������¶��������������߼��źţ�
					������������,���¶Ըò�Ļ��������߼��ź�
** parameters:		cabinetNo�������
					no:     ����������/��� 					
					type��1����ĳ����������2����ĳ�����
					flag:	1����������0�����ر�														
** Returned value:	1 �ɹ�  0ʧ��
*******************************************************************************/
unsigned char hd_open_handle(unsigned char cabinetNo,unsigned char no,unsigned char type,unsigned char flag)
{
	unsigned char i,j,err,temp;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	
	print_hd("no=%d type=%d flag=%d Bin=%d\r\n",no,type,flag,cabinetNo);
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		print_err(cabinetNo);
		return 0;
	}

	if(type == 2)//�����
	{
		if(no < 1 || no > 8)
			return 0;	
		temp = (flag == 1) ? 1 : 0;
		if(stHuodaoPtr[no - 1].openFlag == temp)//�˴β������ϼ��㿪���ַ������俪��ʲô�಻��ֱ�ӷ���1
			return 1;
		stHuodaoPtr[no - 1].openFlag = temp;
		for(j = 0;j < HUODAO_MAX_COLUMN;j++)//�ò�����л���ִ����Ӧ�Ŀ��ز���
		{
			if(!stHuodaoPtr[no - 1].huodao[j].physicAddr)
				stHuodaoPtr[no - 1].huodao[j].openFlag = 0;
			else
				stHuodaoPtr[no - 1].huodao[j].openFlag = temp;
		}
		hd_logic_init(cabinetNo);//���������߼���
		return 1;			
	}
	else//Ī��������
	{
		err = hd_get_index(cabinetNo,no,2,&i,&j);
		if(err)
		{
			if(!stHuodaoPtr[i].openFlag )//��ر��˲����������������
				return 0;
			if(!stHuodaoPtr[i].huodao[j].physicAddr)//�����ַ�����ڲ��������
			{
				stHuodaoPtr[i].huodao[j].openFlag = 0;
				return 0;
			}
			temp = (flag == 1) ? 1 : 0;
			if(stHuodaoPtr[i].huodao[j].openFlag == temp)
				return 1;
			
			stHuodaoPtr[i].huodao[j].openFlag = temp;
			hd_logic_init(cabinetNo);//���������߼���
			return 1;	
			
			
		}
	}
	
	return 0;
	
}

/*****************************************************************************
** Function name:	hd_logicNo_by_selectNo	
** Descriptions:	      ͨ��ѡ�������Ż�û����߼���												 			
** parameters:		cabinetNo:�����		
					selectNo :ѡ��������
** Returned value:	0:ʧ��;  ��0  ���ػ����߼���
*******************************************************************************/
unsigned char hd_logicNo_by_selectNo(unsigned char cabinetNo,unsigned char selectNo)
{
	unsigned char i,j,err;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		print_err(cabinetNo);
		return 0;
	}
	
	err = hd_get_index(cabinetNo,selectNo,3,&i,&j);
	if(err)
	{
		return stHuodaoPtr[i].huodao[j].logicNo;
	}
	return 0;

}


/*****************************************************************************
** Function name:	hd_logicNo_by_id	
** Descriptions:	      ͨ��ID�Ż�û����߼���												 			
** parameters:		cabinetNo:�����		
					selectNo :ѡ��������
** Returned value:	0:�޴˻���;  ��0  ���ػ����߼���  0xFF ��ʾȱ��
*******************************************************************************/
unsigned char hd_logicNo_by_id(unsigned char cabinetNo,unsigned char id)
{
	unsigned char i,j,count = 0,bestLogicNo = 0;
	ST_LEVEL_HUODAO *stHuodaoPtr;

	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		print_err(cabinetNo);
		return 0;
	}
	
	for(i = 0;i < HUODAO_MAX_ROW;i++)
	{
		for(j = 0;j < HUODAO_MAX_COLUMN;j++)
		{
			
			if(stHuodaoPtr[i].huodao[j].id != id)
			  	continue;
			
			if((stHuodaoPtr[i].huodao[j].count > count) && 
				(stHuodaoPtr[i].huodao[j].state == HUODAO_STATE_NORMAL))
			{		
				
				count =  stHuodaoPtr[i].huodao[j].count;
				bestLogicNo = stHuodaoPtr[i].huodao[j].logicNo;
			}
		}
	}
	
	return bestLogicNo ;

}

/*****************************************************************************
** Function name:	hd_logicNo_by_physic	
** Descriptions:	      ͨ����������Ż�û����߼���												 			
** parameters:		cabinetNo:�����		
					physicNo :
** Returned value:	0:ʧ��;  ��0  ���ػ����߼���
*******************************************************************************/
unsigned char hd_logicNo_by_physic(unsigned char cabinetNo,unsigned char physicNo)
{
	unsigned char i,j,err;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		print_err(cabinetNo);
		return 0;
	}
	
	err = hd_get_index(cabinetNo,physicNo,2,&i,&j);
	if(err)
	{
		return stHuodaoPtr[i].huodao[j].logicNo;
	}
	return 0;

}


/*****************************************************************************
** Function name:	hd_physicNo_by_logicNo	
** Descriptions:	      ͨ���߼������Ż�����������											 			
** parameters:		cabinetNo:�����		
					selectNo :ѡ��������
** Returned value:	0:ʧ��;  ��0  ���ػ������������
*******************************************************************************/
unsigned char hd_physicNo_by_logicNo(unsigned char cabinetNo,unsigned char logicNo)
{
	unsigned char i,j,err;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		print_err(cabinetNo);
		return 0;
	}
	
	err = hd_get_index(cabinetNo,logicNo,1,&i,&j);
	if(err)
	{
		return stHuodaoPtr[i].huodao[j].physicNo;
	}
	return 0;

}



/*****************************************************************************
** Function name:	hd_set_by_logic	
** Descriptions:	      ͨ���߼����������û�����Ϣ											 			
** parameters:		cabinetNo:�����		
					logicNo :�߼�������
** Returned value:	0:ʧ��; 1�ɹ�
*******************************************************************************/
unsigned char hd_set_by_logic(unsigned char cabinetNo,unsigned char logicNo,unsigned char type,unsigned int value)
{
	unsigned char  i,j,err,flag = 0;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		print_err(cabinetNo);
		return 0;
	}
	
	err = hd_get_index(cabinetNo,logicNo,1,&i,&j);
	if(err)
	{
		switch(type)
		{
			case HUODAO_TYPE_PRICE:
				stHuodaoPtr[i].huodao[j].price = value;
				flag = 1;
				break;
			case HUODAO_TYPE_COUNT:
				stHuodaoPtr[i].huodao[j].count= value;
				flag = 1;
				break;
			case HUODAO_TYPE_MAX_COUNT:
				stHuodaoPtr[i].huodao[j].maxCount= value;
				flag = 1;
				break;
			case HUODAO_TYPE_STATE:
				stHuodaoPtr[i].huodao[j].state= value;
				flag = 1;
				break;
			case HUODAO_TYPE_SELECTNO:
				stHuodaoPtr[i].huodao[j].selectNo= value;
				flag = 1;
				break;
			case HUODAO_TYPE_ID:
				stHuodaoPtr[i].huodao[j].id= value;
				flag = 1;
				break;
			case HUODAO_TYPE_SUC_COUNT:
				stHuodaoPtr[i].huodao[j].sucCount = value;
				flag = 1;
				break;
			case HUODAO_TYPE_OPEN:
				if(stHuodaoPtr[i].openFlag)
				{
					stHuodaoPtr[i].huodao[j].openFlag  = value;
					flag = 1;
				}					
				break;
			default:
				break;
		}
		
	}

	return flag;

}


/*****************************************************************************
** Function name:	hd_get_by_logic	
** Descriptions:	ͨ���߼������Ŷ�ȡ��������ֵ													 			
** parameters:		logicNo:�߼���ַ
					type��1������������
						  2������������
						  3��������״̬l
						  4���������������
						  5����ѡ������ֵ
						  6�����ɹ����״���
						  7������Ʒ���					
** Returned value:	0���ޣ�������Ӧֵ
*******************************************************************************/
unsigned int hd_get_by_logic(unsigned char cabinetNo,unsigned char logicNo,unsigned char type)
{
	unsigned char  i,j,err;
	unsigned int value = 0;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
		return hefanGetParaVlaue(get_hefan_Index(cabinetNo),logicNo,type);
	
	err = hd_get_index(cabinetNo,logicNo,1,&i,&j);
	if(err)
	{
		switch(type)
		{
			case HUODAO_TYPE_PRICE:
				value = stHuodaoPtr[i].huodao[j].price;
				break;
			case HUODAO_TYPE_COUNT:
				value = stHuodaoPtr[i].huodao[j].count;
				break;
			case HUODAO_TYPE_MAX_COUNT:
				value = stHuodaoPtr[i].huodao[j].maxCount;
				break;
			case HUODAO_TYPE_STATE:
				value = stHuodaoPtr[i].huodao[j].state;
				break;
			case HUODAO_TYPE_SELECTNO:
				value = stHuodaoPtr[i].huodao[j].selectNo;
				break;
			case HUODAO_TYPE_ID:
				value = stHuodaoPtr[i].huodao[j].id;
				break;
			case HUODAO_TYPE_SUC_COUNT:
				value = stHuodaoPtr[i].huodao[j].sucCount;
				break;
			case HUODAO_TYPE_OPEN:
				value = (stHuodaoPtr[i].openFlag) ? (stHuodaoPtr[i].huodao[j].openFlag) : 0;
				break;
			default:
				break;
		}
		
	}
	return value;
	
}

/*****************************************************************************
** Function name:	hd_set_by_physic	
** Descriptions:	      ͨ��������������û�����Ϣ											 			
** parameters:		cabinetNo:�����		
					physicNo :���������
** Returned value:	0:ʧ��; 1�ɹ�
*******************************************************************************/
unsigned char hd_set_by_physic(unsigned char cabinetNo,unsigned char physicNo,unsigned char type,unsigned int value)
{
	unsigned char  i,j,err,flag = 0;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		print_err(cabinetNo);
		return 0;
	}
	err = hd_get_index(cabinetNo,physicNo,2,&i,&j);
	if(err)
	{
		switch(type)
		{
			case HUODAO_TYPE_PRICE:
				stHuodaoPtr[i].huodao[j].price = value;
				flag = 1;
				break;
			case HUODAO_TYPE_COUNT:
				stHuodaoPtr[i].huodao[j].count= value;
				flag = 1;
				break;
			case HUODAO_TYPE_MAX_COUNT:
				stHuodaoPtr[i].huodao[j].maxCount= value;
				flag = 1;
				break;
			case HUODAO_TYPE_STATE:
				stHuodaoPtr[i].huodao[j].state= value;
				flag = 1;
				break;
			case HUODAO_TYPE_SELECTNO:
				stHuodaoPtr[i].huodao[j].selectNo= value;
				flag = 1;
				break;
			case HUODAO_TYPE_ID:
				stHuodaoPtr[i].huodao[j].id= value;
				flag = 1;
				break;
			case HUODAO_TYPE_SUC_COUNT:
				stHuodaoPtr[i].huodao[j].sucCount = value;
				flag = 1;
				break;
			case HUODAO_TYPE_OPEN:
				if(stHuodaoPtr[i].openFlag)
				{
					stHuodaoPtr[i].huodao[j].openFlag  = value;
					flag = 1;
				}					
				break;
			default:
				break;
		}
		
	}

	return flag;

}


/*****************************************************************************
** Function name:	hd_get_by_physic	
** Descriptions:	ͨ����������Ŷ�ȡ��������ֵ													 			
** parameters:		physicNo:���������
					type��1������������
						  2������������
						  3��������״̬
						  4���������������
						  5����ѡ������ֵ
						  6�����ɹ����״���
						  7������Ʒ���					
** Returned value:	0���ޣ�������Ӧֵ
*******************************************************************************/
unsigned int hd_get_by_physic(unsigned char cabinetNo,unsigned char physicNo,unsigned char type)
{
	unsigned char  i,j,err;
	unsigned int value = 0;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
		return hefanGetParaVlaue(get_hefan_Index(cabinetNo),physicNo,type);
	
	err = hd_get_index(cabinetNo,physicNo,2,&i,&j);
	if(err)
	{
		switch(type)
		{
			case HUODAO_TYPE_PRICE:
				value = stHuodaoPtr[i].huodao[j].price;
				break;
			case HUODAO_TYPE_COUNT:
				value = stHuodaoPtr[i].huodao[j].count;
				break;
			case HUODAO_TYPE_MAX_COUNT:
				value = stHuodaoPtr[i].huodao[j].maxCount;
				break;
			case HUODAO_TYPE_STATE:
				value = stHuodaoPtr[i].huodao[j].state;
				break;
			case HUODAO_TYPE_SELECTNO:
				value = stHuodaoPtr[i].huodao[j].selectNo;
				break;
			case HUODAO_TYPE_ID:
				value = stHuodaoPtr[i].huodao[j].id;
				break;
			case HUODAO_TYPE_SUC_COUNT:
				value = stHuodaoPtr[i].huodao[j].sucCount;
				break;
			case HUODAO_TYPE_OPEN:
				value = stHuodaoPtr[i].huodao[j].openFlag;
				break;
			default:
				break;
		}
		
	}
	return value;
	
}



/*****************************************************************************
** Function name:	hd_check_ok	
** Descriptions:	      �������Ƿ�ɷ���														 			
** parameters:		logicNo:�߼���ַ					
** Returned value:	0��������1������Ϊ0,3����������;4:ȱ����5���޴˻����� 6��ƷIDΪ0 ��7PC��λ������
*******************************************************************************/
unsigned char hd_check_ok(unsigned char cabinetNo,unsigned char logicNo)
{
	unsigned char  i,j,err;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
		return hefanHuodaoCheck(cabinetNo,logicNo);
	
	err = hd_get_index(cabinetNo,logicNo,1,&i,&j);
	if(!err)
		return 5;
	
	if((SystemPara.PcEnable)&&(stHuodaoPtr[i].huodao[j].id==0))
		return 6;
	if(stHuodaoPtr[i].huodao[j].price == 0)
		return 1;
	if(stHuodaoPtr[i].huodao[j].count== 0)
		return 4;
	if(stHuodaoPtr[i].huodao[j].state == 2)
		return 3;
	if(stHuodaoPtr[i].huodao[j].state== 3)
		return 4;
	if(stHuodaoPtr[i].huodao[j].state == 4)
		return 7;

	return 0;
		
}




/*****************************************************************************
** Function name:	hd_add_goods	
** Descriptions:	��ӻ����������													 			
** parameters:		cabinetNo��1--���� 2--����
					type����������	
						1�����������
						2��������߼���
						3��ȫ���������			
** Returned value:	0��ʧ�ܣ�1���ɹ�
*******************************************************************************/
unsigned char hd_add_goods(unsigned char cabinetNo,unsigned char no,unsigned type)
{
	unsigned char  i,j,err;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		return 0;
	}

	if(type == 1)//������
	{
		err = hd_get_index(cabinetNo,no,1,&i,&j);
		if(err)
		{
			if(stHuodaoPtr[i].huodao[j].state ==  HUODAO_STATE_EMPTY
				|| stHuodaoPtr[i].huodao[j].state ==  HUODAO_STATE_NORMAL)
			{
				stHuodaoPtr[i].huodao[j].state =  HUODAO_STATE_NORMAL;
				stHuodaoPtr[i].huodao[j].count = stHuodaoPtr[i].huodao[j].maxCount;
				return 1;	
			}
		}
	}
	else if(type == 2)//�����
	{
		if(no < 1 || no > HUODAO_MAX_ROW)
			return 0;
		if(!stHuodaoPtr[no - 1].openFlag)
			return 0;
		for(j = 0;j < HUODAO_MAX_COLUMN;j++)
		{
			if(stHuodaoPtr[no - 1].huodao[j].state ==  HUODAO_STATE_EMPTY
				|| stHuodaoPtr[no - 1].huodao[j].state ==  HUODAO_STATE_NORMAL)
			{
				stHuodaoPtr[no - 1].huodao[j].state =  HUODAO_STATE_NORMAL;
				stHuodaoPtr[no - 1].huodao[j].count = stHuodaoPtr[no - 1].huodao[j].maxCount;
			}
		}
		return 1;
	}
	else if(type == 3)
	{
		for(i = 0;i < HUODAO_MAX_ROW;i++)
		{
			if(!stHuodaoPtr[i].openFlag)
				continue;
			for(j = 0;j < HUODAO_MAX_COLUMN;j++)
			{
				if(stHuodaoPtr[i].huodao[j].state ==  HUODAO_STATE_EMPTY
					|| stHuodaoPtr[i].huodao[j].state ==  HUODAO_STATE_NORMAL)
				{
					stHuodaoPtr[i].huodao[j].state =  HUODAO_STATE_NORMAL;
					stHuodaoPtr[i].huodao[j].count = stHuodaoPtr[i].huodao[j].maxCount;
				}
			}
		}

		return 1;
	}
	
	return 0;
}

/*****************************************************************************
** Function name:	hd_all_check	
** Descriptions:	      ������л��� ����													 			
** parameters:							
** Returned value:	0��������1ȫ��������
*******************************************************************************/
unsigned char hd_all_check(void)
{
	unsigned char i,j;
	
	//�Ȳ鿴��1
	for(i = 0;i < HUODAO_MAX_ROW;i++)
	{
		if(!stHuodao[i].openFlag)
			continue;
		for(j = 0;j < HUODAO_MAX_COLUMN;j++)
		{
				if(!stHuodao[i].huodao[j].openFlag)
					continue;
				if(stHuodao[i].huodao[j].count && stHuodao[i].huodao[j].price
					&&  stHuodao[i].huodao[j].state == HUODAO_STATE_NORMAL)
				{
					print_hd("The first ok hd is[%d][%d]\r\n",i,j);
					return 0;
				}
					
		}
	}

	//���� ��ʾ��1 ȫ�������� ��������2 ��鿴��2
	if(SystemPara.SubBinOpen)
	{
		for(i = 0;i < HUODAO_MAX_ROW;i++)
		{
			if(!stSubHuodao[i].openFlag)
				continue;
			for(j = 0;j < HUODAO_MAX_COLUMN;j++)
			{
				if(!stSubHuodao[i].huodao[j].openFlag)
					continue;
				if(stSubHuodao[i].huodao[j].count && stSubHuodao[i].huodao[j].price
					&&  stSubHuodao[i].huodao[j].state == HUODAO_STATE_NORMAL)
				{
					print_hd("The sub first ok hd is[%d][%d]\r\n",i,j);
					return 0;
				}
					
			}
		}
	}
	print_hd("hd all empty!\r\n");
	return 1;

			
}


/*****************************************************************************
** Function name:	hd_set_by_pc	
** Descriptions:	     PC���û�����Ϣ
** parameters:		cabinetNo :���  1 ����ID ;  2:�������� ; 3 : ���õ���by ID  4:���õ��� by ���� 		
** Returned value:	1�ɹ� 0ʧ��
*******************************************************************************/
unsigned char hd_set_by_pc(unsigned char cabinetNo,unsigned char len,void * data,unsigned char type)
{	
	unsigned char  i,j,err = 0,r,c,logicNo,index = 0;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	struct GoodsMatrix (*stGoods)[];
	unsigned char *buf,value;
	unsigned int price;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		return 0;
	}
	
	if((len != HUODAO_MAX_ROW * HUODAO_MAX_COLUMN) && (type != 3))
	{
		return 0;
	}
	
	if(type == 3) //���û��� by ID
	{
		stGoods = (struct GoodsMatrix (*)[])data;
		for(i = 0;i < len;i++)
		{
			print_hd("id[%d] = %d,%d\r\n",i,(*stGoods)[i].GoodsType,(*stGoods)[i].Price);
			for(r = 0;r < HUODAO_MAX_ROW;r++)
			{
				for(c = 0;c < HUODAO_MAX_COLUMN;c++)
				{
					if(stHuodaoPtr[r].huodao[c].id == (*stGoods)[i].GoodsType && stHuodaoPtr[r].huodao[c].openFlag)
					{
						stHuodaoPtr[r].huodao[c].price = (*stGoods)[i].Price;
						print_hd("r= %d c= %d\r\n",r,c);
					}
				}
			}
		}
		return 1;
	}
	else if(type == 4)//���û��� by ����
	{
		stGoods = (struct GoodsMatrix (*)[])data;
		for(i = 0;i < HUODAO_MAX_ROW;i++)
		{
			for(j = 0;j < HUODAO_MAX_COLUMN;j++)
			{
				if(j >= 9 )
					logicNo = (i + 1) * 10;
				else
					logicNo = (i + 1) * 10 + (j + 1);
				price = (*stGoods)[index++].Price;
				if(hd_get_index(cabinetNo,logicNo,1,&r,&c))//��������
				{
					stHuodaoPtr[r].huodao[c].price = price;
				}		
			}
		}
	}
	else
	{
		//���� ID  ������
		buf = (unsigned char *)data;
		
		for(i = 0;i < HUODAO_MAX_ROW;i++)
		{
			for(j = 0;j < HUODAO_MAX_COLUMN;j++)
			{
				if(j >= 9 )
					logicNo = (i + 1) * 10;
				else
					logicNo = (i + 1) * 10 + (j + 1);
				
				value = buf[index++];
				//print_hd("type =%d value = %d\r\n",type,value);
				if(hd_get_index(cabinetNo,logicNo,1,&r,&c))//��������
				{
					if(type == 1)//����ID ��λ���̶�����80���ֽ�����
					{
						//��λ����ر�
						if(value == 0x00 || value == 0xFF)
						{
							stHuodaoPtr[r].huodao[c].id = 0;
							stHuodaoPtr[r].huodao[c].state = HUODAO_STATE_PC_CLOSE;			
						}
						else//���ϻ�����������ID
						{
							stHuodaoPtr[r].huodao[c].id = value;
							if(stHuodaoPtr[r].huodao[c].state != HUODAO_STATE_FAULT && 
								stHuodaoPtr[r].huodao[c].state != HUODAO_STATE_NOT)
							{
								if(stHuodaoPtr[r].huodao[c].count)
									stHuodaoPtr[r].huodao[c].state = HUODAO_STATE_NORMAL;
								else
									stHuodaoPtr[r].huodao[c].state = HUODAO_STATE_EMPTY;
							}
						
						}
					}
					else if(type == 2)
					{	
						if((stHuodaoPtr[r].huodao[c].state == HUODAO_STATE_EMPTY)||
							(stHuodaoPtr[r].huodao[c].state == HUODAO_STATE_NORMAL))
						{
							stHuodaoPtr[r].huodao[c].count = (value & 0x3F);
							if(stHuodaoPtr[r].huodao[c].count)
								stHuodaoPtr[r].huodao[c].state = HUODAO_STATE_NORMAL;
							else
								stHuodaoPtr[r].huodao[c].state = HUODAO_STATE_EMPTY;
						}
					}
				}		
			}
		}
	}


	


	return err;
	
}



/*****************************************************************************
** Function name:	hd_get_next_physicNo	
** Descriptions:	      ��ȡ����и���������ŵ���һ�����ڵ����������										 			
** parameters:		cabinetNo :	
** Returned value:	1�ɹ� 0ʧ��
*******************************************************************************/
unsigned char hd_get_next_physicNo(unsigned char cabinetNo,unsigned char physicNo)
{
#if 0
	unsigned char  row,col,i,j;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		return 0;
	}

	row = physicNo / 10;//��� 1 - 8
	col = (physicNo % 10) ? (physicNo % 10) : 10;//�к� 1- 10
	//��������һ������һ�������򷵻ص�һ���һ������

	
	for(i = 0;i < HUODAO_MAX_ROW;i++)
	{
		for(j = 0;j < HUODAO_MAX_COLUMN;j++)
		{
			if(stHuodaoPtr[i].huodao[j].physicAddr)
				  ;
		}
	}

#endif
	return 0;	
}


/*****************************************************************************
** Function name:	ChannelSendMBox	
**
** Descriptions:	���豸��������������	
**					
**														 			
** parameters:		HandleType:��������
					ChkFlag:�Ƿ����������
					Binnum:�����
** Returned value:	0:�ɹ�;1:ʧ��;
** 
*****************************************************************************/
unsigned char ChannelSendMBox(unsigned char HandleType,unsigned char ChkFlag,unsigned char ChannelNum,unsigned char Binnum)
{
	MessagePack *RecvBoxPack;
	uint8_t err;

	switch(HandleType)
	{
		case CHANNEL_OUTGOODS:
			MsgAccepterPack.ChnlHandleType = CHANNEL_OUTGOODS;
			MsgAccepterPack.ChannelNum = ChannelNum;
			MsgAccepterPack.GOCFlag = ChkFlag;
			MsgAccepterPack.Binnum = Binnum;
			//���������DevieTask�������Hopper�����Ҳ���
			OSMboxPost(g_ChannelMail,&MsgAccepterPack);
			//�ȴ����䷵��
			RecvBoxPack = OSMboxPend(g_ChannelBackMail,8000,&err);//8000 * 5 = 40s 
			if(err == OS_NO_ERR)
			{
				return RecvBoxPack->HandleResult;
			}
			break;
		case CHANNEL_CHECKSTATE:
			MsgAccepterPack.ChnlHandleType = CHANNEL_CHECKSTATE;
			MsgAccepterPack.ChannelNum = ChannelNum;
			MsgAccepterPack.GOCFlag = ChkFlag;
			MsgAccepterPack.Binnum = Binnum;
			//���������DevieTask�������Hopper�����Ҳ���
			OSMboxPost(g_ChannelMail,&MsgAccepterPack);
			//�ȴ����䷵��
			RecvBoxPack = OSMboxPend(g_ChannelBackMail,1000,&err);
			if(err == OS_NO_ERR)
			{
				return RecvBoxPack->HandleResult;
			}
			break;
		case CHANNEL_CHECKOUTRESULT:
			MsgAccepterPack.ChnlHandleType = CHANNEL_CHECKOUTRESULT;
			MsgAccepterPack.ChannelNum = ChannelNum;
			MsgAccepterPack.GOCFlag = ChkFlag;
			MsgAccepterPack.Binnum = Binnum;
			//���������DevieTask�������Hopper�����Ҳ���
			OSMboxPost(g_ChannelMail,&MsgAccepterPack);
			//�ȴ����䷵��
			RecvBoxPack = OSMboxPend(g_ChannelBackMail,1000,&err);
			if(err == OS_NO_ERR)
			{
				return RecvBoxPack->HandleResult;
			}
			break;
		default:
			break;
	}
	return 0xff;	
}

unsigned char TestColumnPost(unsigned char HandleType,unsigned char ChkFlag,unsigned char ChannelNum,unsigned char Binnum)
{
	switch(HandleType)
	{
		case CHANNEL_OUTGOODS:
			MsgAccepterPack.ChnlHandleType = CHANNEL_OUTGOODS;
			MsgAccepterPack.ChannelNum = ChannelNum;
			MsgAccepterPack.GOCFlag = ChkFlag;
			MsgAccepterPack.Binnum = Binnum;
			//���������DevieTask�������Hopper�����Ҳ���
			OSMboxPost(g_ChannelMail,&MsgAccepterPack);
			break;
		case CHANNEL_CHECKSTATE:
			MsgAccepterPack.ChnlHandleType = CHANNEL_CHECKSTATE;
			MsgAccepterPack.ChannelNum = ChannelNum;
			MsgAccepterPack.GOCFlag = ChkFlag;
			MsgAccepterPack.Binnum = Binnum;
			//���������DevieTask�������Hopper�����Ҳ���
			OSMboxPost(g_ChannelMail,&MsgAccepterPack);
			break;
		case CHANNEL_CHECKOUTRESULT:
			MsgAccepterPack.ChnlHandleType = CHANNEL_CHECKOUTRESULT;
			MsgAccepterPack.ChannelNum = ChannelNum;
			MsgAccepterPack.GOCFlag = ChkFlag;
			MsgAccepterPack.Binnum = Binnum;
			//���������DevieTask�������Hopper�����Ҳ���
			OSMboxPost(g_ChannelMail,&MsgAccepterPack);
			break;
		default:
			break;
	}
	
	return 0xff;
}



unsigned char TestColumnPend(unsigned char ChkFlag)
{
	MessagePack *RecvBoxPack;
	uint8_t err;

	//�ȴ����䷵��
	RecvBoxPack = OSMboxPend(g_ChannelBackMail,5,&err);
	if(err == OS_NO_ERR)
	{
		if(RecvBoxPack->HandleResult == 0x00)
		{
			return 0;	
		}
		else
		if(RecvBoxPack->HandleResult == 0xff)
		{
			return 5;
		}
		else
		if((ChkFlag==1)&&(RecvBoxPack->HandleResult&0x80))
			return 0;
		else
		if(RecvBoxPack->HandleResult&0x01)
		{
			//���δת��
			if((ChkFlag==1)&&(RecvBoxPack->HandleResult&0x08))
				return 1;
			//����ȷ�Ͽ���������ȷ��δ��⵽��Ʒ����
			if((ChkFlag==1)&&(RecvBoxPack->HandleResult&0x20))
				return 4;	
			else
			//����ȷ�Ͽ��������δת��λ��������ȷ�ϼ�⵽��Ʒ����
			if((ChkFlag==1)&&(RecvBoxPack->HandleResult==0x11))
				return 0;
			else
			//����ȷ��δ���������δת��λ
			if((ChkFlag==0)&&(RecvBoxPack->HandleResult&0x10))
				return 3;
			else
			//����ȷ��δ�������������ת��
			if((ChkFlag==0)&&(RecvBoxPack->HandleResult&0x08))
				return 1;
		}
		return 1;
	}
	else
		return 0xFF;
}



/*****************************************************************************
** Function name:	ChannelHandleProcess	
**
** Descriptions:	��������������	
**					
**														 			
** parameters:		HandleType:��������
					ChkFlag:�Ƿ����������
 GOODS_SOLD_ERR          (1 << 0)   //bit0�ܹ���λ
 GOODS_SOLDOUT_BIT       (1 << 1)   //bit1�������
 MOTO_MISPLACE_BIT       (1 << 2)   //bit2�����ת֮ǰ�Ͳ�����ȷ��λ����(Ҳ��������)
 MOTO_NOTMOVE_BIT        (1 << 3)   //bit3�������ת(Ҳ��������)
 MOTO_NOTRETURN_BIT      (1 << 4)   //bit4���ûת����ȷλ��(Ҳ��������)
 GOODS_NOTPASS_BIT       (1 << 5)   //bit5��Ʒû��(����ȷ��û��⵽)
 DRV_CMDERR_BIT          (1 << 6)   //bit6�������(ֻ�з�������Ͳ�ѯ������������������������������ͱ���)
 DRV_GOCERR_BIT          (1 << 7)   //bit7�������ģ��״̬(GOC����)
 
** Returned value:	0:�ɹ�;1:����;2:����������;3:���δ��λ;4:����ʧ�� 5:ͨ�Ź���
** 
*******************************************************************************/
unsigned char ChannelAPIHandle(unsigned char LogicNum,unsigned char HandleType,unsigned char GocType,unsigned char Binnum)
{
	unsigned char Rst=0;

	if(LogicNum == 0xff)
		return 2;
	Rst = ChannelSendMBox(HandleType,GocType,LogicNum,Binnum);
	TraceChannel("OutGoods_DeviceBack=%x bin = %d\r\n",Rst,Binnum);
	if(Rst == 0x00)
	{
		TraceChannel("\r\nOutGoods_OK");
		return 0;	
	}
	else if(Rst == 0xff)
	{
		TraceChannel("\r\nOutGoods_COMERR");
		return 5;
	}	
	//1.�ж�GOC�Ƿ����bit7:  GOC����->��Ǯ
	else if(Rst&DRV_GOCERR_BIT)
	{
		if(Rst&GOODS_SOLD_ERR)
		{
			//2.���жϵ����
			//ûת��λ������״��bit1,bit2,bit3,bit4->�����ù���  
			if( Rst & (GOODS_SOLDOUT_BIT|MOTO_MISPLACE_BIT|MOTO_NOTMOVE_BIT|MOTO_NOTRETURN_BIT)) 
			{
				TraceChannel("\r\nOutGoods_3");
				return 3;								
			}
			else
			{
				TraceChannel("\r\nOutGoods_4");
				return 4;	
			}
		}
		else
		{
			TraceChannel("\r\nOutGoods_0");
			return 0;
		}
	}
	else if(Rst&GOODS_SOLD_ERR)
	{
		/*
		//���δת��
		if((GocType==1)&&(Rst&0x08))
			return 1;
		//����ȷ�Ͽ���������ȷ��δ��⵽��Ʒ����
		if((GocType==1)&&(Rst&0x20))
			return 4;	
		else
		//����ȷ�Ͽ��������δת��λ��������ȷ�ϼ�⵽��Ʒ����
		if((GocType==1)&&(Rst==0x11))
			return 0;
		else
		//����ȷ��δ���������δת��λ
		if((GocType==0)&&(Rst&0x10))
			return 3;
		else
		//����ȷ��δ�������������ת��
		if((GocType==0)&&(Rst&0x08))
			return 1;
		*/
		/*********GOC�򿪵������********************/
		if(GocType==1)
		{			
			//1.���δת��,����Ǯ
			if(Rst&MOTO_NOTMOVE_BIT)
			{
				TraceChannel("\r\nOutGoods_1");
				return 1;
			}			
			//2.���ж�GOC�Ƿ��⵽bit5,  ==0˵������ȷ�ϼ�⵽��Ʒ���� 			
			else if((Rst & GOODS_NOTPASS_BIT) == 0) 
			{				
				//�м�⵽->�����ɹ���Ǯ  
				/*************************************************************************/
				TraceChannel("\r\nOutGoods_0");
				return 0;		
			}
			else
			{ 
				   //û��⵽->����ʧ�ܲ���Ǯ
				   //3.���жϵ����
				   //ûת��λ������״��bit1,bit2,bit3,bit4->�����ù���	
				   if( Rst & (GOODS_SOLDOUT_BIT|MOTO_MISPLACE_BIT|MOTO_NOTMOVE_BIT|MOTO_NOTRETURN_BIT) ) 
				   {
						TraceChannel("\r\nOutGoods_3");
						return 3;
				   }
				   else
				   {
						TraceChannel("\r\nOutGoods_4");
						return 4;
				   }				   
			}
			
		}	
		/*********GOC�رյ������********************/
		else
		{
			//2.���жϵ����
		   //ûת��λ������״��bit1,bit2,bit3,bit4->�����ù���	
		   if( Rst & (GOODS_SOLDOUT_BIT|MOTO_MISPLACE_BIT|MOTO_NOTMOVE_BIT|MOTO_NOTRETURN_BIT)) 
		   {
				TraceChannel("\r\nOutGoods_3");
				return 3;								
		   }
		   else
		   {
				TraceChannel("\r\nOutGoods_4");
				return 4;	
		   }
		}
	}
	return 1;
}




//========================================================================
//�ڶ����ϻ����ӿں���
//���� by yoc  2014.04.01
//=============================================================================


/*****************************************************************************
** Function name:	ChannelResetinit	
** Descriptions:	���³�ʼ����������														 			
** parameters:		��					
** Returned value:	��
*******************************************************************************/
void ChannelResetinit()
{
	hd_init(1);
	hd_init(2);
}


/*****************************************************************************
** Function name:	ChannelSaveParam	
**
** Descriptions:	�����������ݱ��浽FLASH��	
**					
**														 			
** parameters:		physicnum����������
					1:�߼��ṹ���ݱ���
					2:ǰ40������������ṹ���ݱ���
					3:��40���������߼��ṹ���ݱ���
					4:�������ݱ���
** Returned value:	��
** 
*******************************************************************************/
void ChannelSaveParam()
{
	hd_save_param(1);
	hd_save_param(2);
	ChannelGetFlashData();	
}


/*****************************************************************************
** Function name:	ChannelGetFlashData	
**
** Descriptions:	��flash�ж�ȡ��������	
**					
**														 			
** parameters:		��
					
** Returned value:	��
** 
*******************************************************************************/
void ChannelGetFlashData()
{
	hd_read_param(1);
	hd_read_param(2);
	read_hefan_info_by_flash();
}


/*****************************************************************************
** Function name:	ChannelExChannge	
**
** Descriptions:	�������ز������߼���ַ�������ַ֮���ƫ�ƽ���	
**					
**														 			
** parameters:		LevelOrChnltype��1����ĳ����������2����ĳ�����
					Handletype:1����������2�����ر�
					logicnum:�߼��������
					Binnum�������
					
** Returned value:	��
** 
*******************************************************************************/
void ChannelExChannge(unsigned char logicnum,unsigned char LevelOrChnltype,unsigned char Handletype,unsigned char Binnum)
{
	hd_open_handle(Binnum,logicnum,LevelOrChnltype,Handletype);
}




/*****************************************************************************
** Function name:	ChannelGetSelectColumn	
** Descriptions:	     	��ȡ�߼�������											 			
** parameters:		Bin:�����
					type://1-ѡ������ 2-��Ʒ���
					selectNo :ѡ��������/id
** Returned value:	0:ʧ��;  ��0  ���ػ����߼�
*******************************************************************************/

unsigned char ChannelGetSelectColumn(unsigned char Bin,unsigned char type,unsigned short SelectNum)
{
	if(type == 1)
		return hd_logicNo_by_selectNo(Bin,SelectNum);
	else
		return hd_logicNo_by_id(Bin,SelectNum);
}





/*****************************************************************************
** Function name:	ChannelGetGocState	
**
** Descriptions:	��ó���ȷ�ϵ�ǰ״̬	
**					
**														 			
** parameters:		Binnum:�����
					
** Returned value:	0:����;1:����;
** 
*******************************************************************************/
unsigned char ChannelGetGocState(unsigned char Binnum)
{
	return 0;	
}




/*****************************************************************************
** Function name:	ChannelSetParam	
**
** Descriptions:	���û�������														 			
** parameters:		physicnum:�����ַ
					HandleType:������������
					Value:�µ�����
					Price:����,�ɹ����״���					
** Returned value:	��
** 
*******************************************************************************/
unsigned char ChannelSetParam(unsigned char Logicnum,unsigned char Binnum,unsigned char HandleType,unsigned short Value,unsigned int Price)
{
	unsigned char type = HUODAO_TYPE_NOT;
	unsigned int tempValue = 0;
	switch(HandleType)
	{
		case CHANNELPRICE:
			type = HUODAO_TYPE_PRICE;
			tempValue = Price;
			break;
		case CHANNELCOUNT:
			type = HUODAO_TYPE_COUNT;
			tempValue = Value;
			break;
		case CHANNELSTATE:
			type = HUODAO_TYPE_STATE;
			tempValue = Value;
			break;
		case CHANNELMAXCOUNT:
			type = HUODAO_TYPE_MAX_COUNT;
			tempValue = Value;
			break;		
		case CHANNELSELECTNUM:
			type = HUODAO_TYPE_SELECTNO;
			tempValue = Value;
			break;
		case CHANNELSUCCESSCOUNT:
			type = HUODAO_TYPE_SUC_COUNT;
			tempValue = Price;
			break;
		case CHANNELGOODSNUM:
			type = HUODAO_TYPE_ID;
			tempValue = Value;
			break;
		default:
			type = HUODAO_TYPE_NOT;
			break;

	}

	return hd_set_by_logic(Binnum,Logicnum,type,tempValue);

}



/*****************************************************************************
** Function name:	ChannelGetPhysicNum	
** Descriptions:	ͨ��������߼���Ż��ʵ�ʵ�������													 			
** parameters:		Logicnum:�����ַ					
** Returned value:	�ɹ�������Ӧ�������ţ�ʧ�ܷ���0xff
*******************************************************************************/
unsigned char ChannelGetPhysicNum(unsigned char Logicnum,unsigned char Binnum)
{
	unsigned char physicAddr = 0xFF,i,j,err;
	err = hd_get_index(Binnum,Logicnum,1,&i,&j);
	if(err)
	{
		if(Binnum == 1)
			physicAddr = stHuodao[i].huodao[j].physicAddr;
		else if(Binnum == 2)
			physicAddr = stSubHuodao[i].huodao[j].physicAddr;
	}
	return (physicAddr) ? physicAddr: 0xFF;
}


/*****************************************************************************
** Function name:	ChannelCheckIsOk	
** Descriptions:	�������Ƿ�ɷ���														 			
** parameters:		Logicnum:�߼���ַ					
** Returned value:	0��������1������Ϊ0,3����������;4:ȱ����5���޴˻����� 6��ƷIDΪ0 ��7PC��λ������
*******************************************************************************/
unsigned int ChannelCheckIsOk(unsigned char Logicnum,unsigned char Binnum)
{
	return hd_check_ok(Binnum,Logicnum);	
}


/*****************************************************************************
** Function name:	ChannelGetParamValue	
** Descriptions:	��ȡ��������ֵ													 			
** parameters:		Logicnum:�߼���ַ
					type��1������������
						  2������������
						  3��������״̬
						  4���������������
						  5����ѡ������ֵ
						  6�����ɹ����״���
						  7������Ʒ���					
** Returned value:	0���ޣ�������Ӧֵ
*******************************************************************************/
unsigned int ChannelGetParamValue(unsigned char logicnum,unsigned char type,unsigned char Binnum)
{
	return hd_get_by_logic(Binnum,logicnum,type);
}


/*****************************************************************************
** Function name:	ChannelGetAddressLOWandRow	
** Descriptions:	ͨ������Ļ����߼��������ŵó������ھ����б��е�ROW��LOW													 			
** parameters:		Address:��ַ���
					type��1��������߼������е�λ��
						  2���������������е�λ��
					Binnum:�����					
** Returned value:	0���ޣ�
					1���ɹ���������Row�����꣬Low������
*******************************************************************************/
unsigned char ChannelGetAddressLOWandRow(unsigned char Type,unsigned char Binnum,
				unsigned char Address,unsigned char *Row,unsigned char *Low)
{	
	if(Type == 2)
		return hd_get_index(Binnum,Address,5,Row,Low);
	else if(Type == 1)
		return hd_get_index(Binnum,Address,1,Row,Low);
	return 0;
}


/*****************************************************************************
** Function name:	ChannelGetCloseCount	
** Descriptions:	���ݲ�ͬ�����ͣ������ܵĿ�����������ĳ��Ĺرջ�������														 			
** parameters:		Level:ĳ��
					type��1����ĳ��Ļ����ر�����
						  2�����رյĲ�����
					Binnum:�����				
** Returned value:	0���ޣ�
					1���ɹ���������Row�����꣬Low������
*******************************************************************************/
unsigned char ChannelGetCloseCount(unsigned char Binnum,unsigned char Type,unsigned char Level)
{
	
	return 0;
}






/*****************************************************************************
** Function name:	GetLevelOrColumnState	
** Descriptions:	���ĳ��Ļ�������״̬														 			
** parameters:		Num:  ĳ�����ĳ�������
					Bin��1��������	 2��������
					Type��1��������״̬ 2������״̬				
** Returned value:	0xff����ʧ��1��������0�����ر�
*******************************************************************************/
unsigned char GetLevelOrColumnState(unsigned char Bin,unsigned char Type,unsigned char Num)
{
	unsigned char i,j,flag=0;
	//���������
	if(Type==1)
	{
		flag = hd_get_index(Bin,Num,2,&i,&j);
		if(flag==0)
			return 0xFF;
		if(Bin==1)
			return stHuodao[i].huodao[j].openFlag;
		else if(Bin==2)
			return stSubHuodao[i].huodao[j].openFlag;
		else
			return 0xFF;
	}
	else if(Type==2)
	{
		if(Num < 1 || Num > HUODAO_MAX_ROW)
			return 0xFF;
		if(Bin==1)
			return stHuodao[Num - 1].openFlag;
		else if(Bin==2)
			return stSubHuodao[Num - 1].openFlag;
		else
			return 0xFF;
	}
	return 0xFF;	
}


/*****************************************************************************
** Function name:	GetColumnStateByPhysic	
** Descriptions:	      ͨ�������Ż�û�������													 			
** parameters:		PhysicNum:����������
					Bin��1--����2--����
					Type����������	
						1������
						2����������
						3��״̬
						4����󴢻�����
						5���ɹ����״���
						6��ѡ������
						7����Ʒ���				
** Returned value:	Type��Ӧ��ֵ
*******************************************************************************/
unsigned int GetColumnStateByPhysic(unsigned char Bin,unsigned char PhysicNum,unsigned char Type)
{
	unsigned char tempType = HUODAO_TYPE_NOT;
	switch(Type)
	{
		case 1:
			tempType = HUODAO_TYPE_PRICE;
			break;
		case 2:
			tempType = HUODAO_TYPE_COUNT;
			break;
		case 3:
			tempType = HUODAO_TYPE_STATE;
			break;
		case 4:
			tempType = HUODAO_TYPE_MAX_COUNT;
			break;
		case 5:
			tempType = HUODAO_TYPE_SUC_COUNT;
			break;
		case 6:
			tempType = HUODAO_TYPE_SELECTNO;
			break;
		case 7:
			tempType = HUODAO_TYPE_ID;
			break;
		default:
			break;

	}
	
	return hd_get_by_physic(Bin,PhysicNum,tempType);
}


/*****************************************************************************
** Function name:	SetColumnParamByPhysic	
** Descriptions:	ͨ�������Ż�û�������														 			
** parameters:		PhysicNum:����������
					Bin��1--����2--����
					Type����������	
						1������
						2����������
						3��״̬
						4����󴢻�����
						5���ɹ����״���
						6��ѡ������
						7����Ʒ���				
** Returned value:	Type��Ӧ��ֵ
*******************************************************************************/
unsigned int SetColumnParamByPhysic(unsigned char Bin,unsigned char PhysicNum,unsigned char Type,unsigned int Value)
{
	unsigned char tempType = HUODAO_TYPE_NOT;
	switch(Type)
	{
		case 1:
			tempType = HUODAO_TYPE_PRICE;
			break;
		case 2:
			tempType = HUODAO_TYPE_COUNT;
			break;
		case 3:
			tempType = HUODAO_TYPE_STATE;
			break;
		case 4:
			tempType = HUODAO_TYPE_MAX_COUNT;
			break;
		case 5:
			tempType = HUODAO_TYPE_SUC_COUNT;
			break;
		case 6:
			tempType = HUODAO_TYPE_SELECTNO;
			break;
		case 7:
			tempType = HUODAO_TYPE_ID;
			break;
		default:
			break;

	}
	return hd_set_by_physic(Bin,PhysicNum,tempType,Value);
	
}


/*****************************************************************************
** Function name:	AddColumnGoods	
** Descriptions:	��ӻ����������													 			
** parameters:		
					Bin��1--���� 2--����
					Type����������	
						1�����������
						2�������
						3��ȫ���������			
** Returned value:	0��ʧ�ܣ�1���ɹ�
*******************************************************************************/
unsigned char AddColumnGoods(unsigned char Bin,unsigned Type,unsigned char LogicNum)
{
	return hd_add_goods(Bin,LogicNum,Type);
}

/*****************************************************************************
** Function name:	FromPhysicGetLogicNum	
** Descriptions:	ͨ�������������߼��������													 			
** parameters:		Bin��1��������2��������
					PhysicNum������������			
** Returned value:	0��ʧ�ܣ��������߼��������
*******************************************************************************/
unsigned char FromPhysicGetLogicNum(unsigned char Bin,unsigned char PhysicNum)
{
	return hd_logicNo_by_physic(Bin,PhysicNum);	
}


/*****************************************************************************
** Function name:	GetPhysicIndexByRowAndLow	
** Descriptions:	ͨ��row��low������ַ����ø��߼�����ӳ���������������													 			
** parameters:		Bin��1��������2��������
					row������������
					low������������	
					type:1-�����õ��߼��������
						 2-�����õ��߼�����ӳ���������������ֵ	
						 3-�����õ��߼�������״̬
						 4-�����õ��߼������Ĵ�������	
** Returned value:	0��ʧ�ܣ����������ֵ
** 
*******************************************************************************/
unsigned char GetPhysicIndexByRowAndLow(unsigned char type,unsigned char Bin,unsigned char row,unsigned char low)
{
	ST_LEVEL_HUODAO *stHuodaoPtr;
	unsigned int value;
	if(Bin == 1)
		stHuodaoPtr = stHuodao;
	else if(Bin == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		return 0;
	}
	switch(type)
	{
		case 1:
			value = stHuodaoPtr[row].huodao[low].logicNo;
			break;
		case 2:
			value = stHuodaoPtr[row].huodao[low].physicNo;
			break;
		case 3:
			value = stHuodaoPtr[row].huodao[low].state;
			break;
		case 4:
			value = stHuodaoPtr[row].huodao[low].count;
			break;
		case 5:
			value = stHuodaoPtr[row].huodao[low].openFlag;
			break;

		default:
			break;
	}
	
	return value;
	
}



//�������п��õ����л�����ͳ�Ʊ���
unsigned char ChannelGetAllOpenColumn(unsigned char Bin,unsigned char *IndexBuf)
{
	unsigned char  i,j,index = 0;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(Bin == 1)
		stHuodaoPtr = stHuodao;
	else if(Bin == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		return 0;
	}
	for(i = 0;i < HUODAO_MAX_ROW;i++)
	{
		if(!stHuodaoPtr[i].openFlag)
			continue;
		for(j = 0;j < HUODAO_MAX_COLUMN;j++)
		{
			if(stHuodaoPtr[i].huodao[j].openFlag && stHuodaoPtr[i].huodao[j].logicNo)
			{
				IndexBuf[index++] = stHuodaoPtr[i].huodao[j].logicNo;
			}
		}
	}
	return index;

	
}

/*****************************************************************************
** Function name:	pcSetHuodao													 			
** parameters:		setNums ���û������� 	stData �����ṹ������ָ��	
** Returned value:	0��ʧ�ܣ�1 �ɹ�
** 
*******************************************************************************/
unsigned char pcSetHuodao(unsigned char setNums,struct GoodsAttribute *stData)
{
	unsigned char i,cabinetNo,rst = 0;
	
	for(i = 0;i < setNums && setNums;i++)
	{
		cabinetNo = stData[i].Cabinet;
		if(cabinetNo < 3)
		{
			if(ChannelGetPhysicNum(stData[i].hd_no,cabinetNo) == 0xFF)//����������
				continue;
				
			if(stData[i].hd_id)//�������
			{
				ChannelSetParam(stData[i].hd_no,cabinetNo,CHANNELGOODSNUM,stData[i].hd_id,0);		
				ChannelSetParam(stData[i].hd_no,cabinetNo,CHANNELCOUNT,stData[i].hd_count,0);
				ChannelSetParam(stData[i].hd_no,cabinetNo,CHANNELPRICE,0,stData[i].hd_Price);	
				if(ChannelGetParamValue(stData[i].hd_no,CHANNELSTATE,cabinetNo) == 3 ||
					ChannelGetParamValue(stData[i].hd_no,CHANNELSTATE,cabinetNo) == 4)
					
					ChannelSetParam(stData[i].hd_no,cabinetNo,CHANNELSTATE,1,0);		
			}
			else //����ر�
			{		
				ChannelSetParam(stData[i].hd_no,cabinetNo,CHANNELSTATE,4,0);
			}

			rst = 1;
			
		}
		else//�з���
		{
			cabinetNo = get_hefan_Index(cabinetNo);
			if(getHefanHuodaotState(cabinetNo,stData[i].hd_no) ==  0)//����������
				continue;
			if(stData[i].hd_id)//�������
			{
				hefanSetParaVlaue(cabinetNo,stData[i].hd_no,stData[i].hd_id,HEFAN_HUODAO_ID);
				hefanSetParaVlaue(cabinetNo,stData[i].hd_no,stData[i].hd_Price,HEFAN_HUODAO_PRICE);
				if(hefanGetParaVlaue(cabinetNo,stData[i].hd_no,HEFAN_HUODAO_STATE) == HEFAN_HUODAO_CLOSE)
					hefanSetParaVlaue(cabinetNo,stData[i].hd_no,HEFAN_HUODAO_NORMAL,HEFAN_HUODAO_EMPTY);		
				if(stData[i].hd_count )
				{
					hefanSetParaVlaue(cabinetNo,stData[i].hd_no,HEFAN_HUODAO_NORMAL,HEFAN_HUODAO_STATE);	
				}
				else		
					hefanSetParaVlaue(cabinetNo,stData[i].hd_no,HEFAN_HUODAO_EMPTY,HEFAN_HUODAO_STATE);	
				
			}
			else //����ر�
			{
				hefanSetParaVlaue(cabinetNo,stData[i].hd_no,HEFAN_HUODAO_CLOSE,HEFAN_HUODAO_STATE);	
						
			}
			
			rst = 1;
			
		}
	}

	return rst;
	
}


/*****************************************************************************
** Function name:	ChannelGetLevelNum													 			
** parameters:		
** Returned value:	0��ʧ�ܣ�1 �ɹ�
** 
*******************************************************************************/

unsigned char ChannelGetLevelNum(unsigned char Binnum,unsigned char LogicNum)
{	
	unsigned char i,j;
	if(hd_get_index(Binnum,LogicNum,1,&i,&j))
		return i + 1;
	return 0;
		
}



/*****************************************************************************
** Function name:	nt_get_level_state	yoc
** Descriptions:														 			
** Returned value:	0:�ر�;1:����; 0xFF �����ڵĲ��
** 
*****************************************************************************/

unsigned char nt_get_level_state(unsigned char cabinetNo,unsigned char levelNo)
{	
	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		return 0xFF;
	}

	if(levelNo < 1 || levelNo > HUODAO_MAX_ROW)
		return 0xFF;

	return stHuodaoPtr[levelNo - 1].openFlag;
		
}

/*****************************************************************************
** Function name:	nt_get_huodoa_ON_OFF	yoc
** Descriptions:	   ��ȡ��������״̬														 			
** Returned value:	0:�ر�;1:����; 0xFF �����ڵĻ�����
** 
*****************************************************************************/

unsigned char nt_get_huodoa_ON_OFF(unsigned char cabinetNo,unsigned char huodaoNo)
{	
	unsigned char  i,j;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		return 0xFF;
	}
	if(hd_get_index(cabinetNo,huodaoNo,2,&i,&j))
	{
		return (stHuodaoPtr[i].openFlag) ? stHuodaoPtr[i].huodao[j].openFlag : 0;
	}
	return 0xFF;	
	
}

/*****************************************************************************
** Function name:	nt_huodao_exsit	yoc
** Descriptions:	   ͨ���߼������ż������Ƿ����														 			
** Returned value:	0:���ܴ���;1:����;
** 
*****************************************************************************/
unsigned char nt_huodao_exsit(unsigned char cabinetNo,unsigned char logicNo)
{	
	unsigned char i,j;
	return hd_get_index(cabinetNo,logicNo,1,&i,&j);
	
}

/*****************************************************************************
** Function name:	nt_huodao_exsit_by_physic	yoc
** Descriptions:	   ͨ����������ż������Ƿ����														 			
** Returned value:	0:���ܴ���;1:����;
** 
*****************************************************************************/
unsigned char nt_huodao_exsit_by_physic(unsigned char cabinetNo,unsigned char physicNo)
{	
	unsigned char row,col;
	row = physicNo / 10;
	col = (physicNo % 10) ? (physicNo % 10) : 10;
	if(cabinetNo == 1)
	{
		if(SystemPara.ChannelType == 1)//10��
			return (row >= 1 && row <= 8 && col >= 1 && col <= 10);
		else
			return (row >= 1 && row <= 8 && col >= 1 && col <= 8);			
	}
	else if(cabinetNo == 2)
	{
		if(SystemPara.SubBinChannelType == 1)//10��
			return (row >= 1 && row <= 8 && col >= 1 && col <= 10);
		else
			return (row >= 1 && row <= 8 && col >= 1 && col <= 8);
	}
	return 0;
	
}


/*****************************************************************************
** Function name:	special_cabinet_info													 			
** parameters:		
** Returned value:	���ط���ͨ������
** 
*******************************************************************************/

unsigned char special_cabinet_info(unsigned char *data)
{
	unsigned char i,cabinetNums = 0,index = 0;
	if(data == NULL)
	{
		print_log("data == NULL\r\n");
		return 0;
	}
	for(i = 0; i < 10;i ++)
	{

#if 0
			data[index++] = (i + 1);
			data[index++] = (i + 1) * 10;
			data[index++] =  0x10;
			cabinetNums++;
#else
		if(getHefanCabinetState(i + 1,HEFAN_EXIST))
		{
			data[index++] = (i + 1);
			data[index++] = (hefanGetParaVlaue(i + 1,0,HEFAN_CABINET_NUMS));
			data[index++] = (getHefanCabinetState(i + 1,HEFAN_HOT)) |
							(getHefanCabinetState(i + 1,HEFAN_HOT) << 1) |
							(getHefanCabinetState(i + 1,HEFAN_HOT) << 2);
			cabinetNums++;
		}
#endif
	}
	
	return cabinetNums;
	
}

/*****************************************************************************
** Function name:	ChannelGetPCChange	
** Descriptions:	���ݽ���PC��ָ����Ĳ���ֵ														 			
** parameters:		type=1:������Ʒ���
					type=2��������Ʒ����
					type=3��������Ʒ������� 
					valuebuf����PC�����յ�������					
** Returned value:	��
** 
*******************************************************************************/
void ChannelGetPCChange(unsigned char cabinetNo,unsigned char type,unsigned char *valuebuf)
{
	unsigned char  i,j,index = 0,logicNo,r,c;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	unsigned int value;

	if(!valuebuf)
		return;
	
	cabinetNo = valuebuf[index++];//����
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		print_err(cabinetNo);
		return;
	}

	for(i = 0;i < HUODAO_MAX_ROW;i++)
	{
		for(j = 0;j < HUODAO_MAX_COLUMN;j++)
		{
			if(j >= 9)
				logicNo = (i + 1) * 10;
			else
				logicNo = (i + 1) * 10 + j + 1;
			
			value = valuebuf[index++];//��ȡ���ݰ�
			if(hd_get_index(cabinetNo,logicNo,1,&r,&c))//��������
			{
				if(type == 1)//����ID ��λ���̶�����80���ֽ�����
				{
					//��λ����ر�
					if(value == 0x00 || value == 0xFF)
					{
						stHuodaoPtr[r].huodao[c].id = 0;
						stHuodaoPtr[r].huodao[c].state = HUODAO_STATE_PC_CLOSE;			
					}
					else//���ϻ�����������ID
					{
						stHuodaoPtr[r].huodao[c].id = value;
						if(stHuodaoPtr[r].huodao[c].state != HUODAO_STATE_FAULT && 
							stHuodaoPtr[r].huodao[c].state != HUODAO_STATE_NOT)
						{
							if(stHuodaoPtr[r].huodao[c].count)
								stHuodaoPtr[r].huodao[c].state = HUODAO_STATE_NORMAL;
							else
								stHuodaoPtr[r].huodao[c].state = HUODAO_STATE_EMPTY;
						}
					
					}
				}
				else if(type == 2)//������Ʒ����
				{
					// ��Ʒ�������� ��ʱ��������ӿ�
					value = value;
				}
				else if(type == 3)
				{	
					if((stHuodaoPtr[r].huodao[c].state == HUODAO_STATE_EMPTY)||
						(stHuodaoPtr[r].huodao[c].state == HUODAO_STATE_NORMAL))
					{
						stHuodaoPtr[r].huodao[c].count = value;
						stHuodaoPtr[r].huodao[c].state = HUODAO_STATE_NORMAL;
					}
				}
			}
			
		}
	}
	

}


unsigned char hd_huodao_id_rpt_vp(unsigned char cabinetNo,unsigned char *buf)
{
	unsigned char i,j,index = 0,logicNo,r,c;
	
	cabinetNo = cabinetNo;
	
	for(i = 0;i < HUODAO_MAX_ROW;i++)
	{
		for(j = 0;j < HUODAO_MAX_COLUMN;j++,index++)
		{
			if(j >= 9)
				logicNo = (i + 1) * 10;
			else
				logicNo = (i + 1) * 10 + j + 1;
			
			if(hd_get_index(1,logicNo,1,&r,&c))
				buf[index] = stHuodao[r].huodao[c].id;
			else
				buf[index] = 0x00;
			
		}
		
	}
	return 1;
}

/*****************************************************************************
** Function name:	hd_huodao_rpt_vp	
** Descriptions:		�ϱ� HUODAO_RPT 							
** Returned value:	��
** 
*******************************************************************************/

unsigned char hd_huodao_rpt_vp(unsigned char cabinetNo,unsigned char *buf)
{
	unsigned char i,j,index = 0,logicNo,r,c;

	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
	{
		return 0;
	}

	
	for(i = 0;i < HUODAO_MAX_ROW;i++)
	{
		for(j = 0;j < HUODAO_MAX_COLUMN;j++,index++)
		{
			if(j >= 9)
				logicNo = (i + 1) * 10;
			else
				logicNo = (i + 1) * 10 + j + 1;
			buf[index] = 0;
			if(hd_get_index(cabinetNo,logicNo,1,&r,&c))
			{
				if(stHuodaoPtr[r].huodao[c].state == HUODAO_STATE_PC_CLOSE)
				{
					buf[index] |= 0xC0;
				}
				else if(stHuodaoPtr[r].huodao[c].state == HUODAO_STATE_NORMAL ||
					stHuodaoPtr[r].huodao[c].state == HUODAO_STATE_EMPTY)
				{
					buf[index] = 0;
				}
				else
					buf[index] |= 0x40;

				buf[index] |= ((stHuodaoPtr[r].huodao[c].count) & 0x3F);
			}
			else
				buf[index] = 0x80;
			
		}
		
	}
	return 1;

}

/*****************************************************************************
** Function name:	hd_id_by_logic	
** Descriptions:	ͨ���߼�������ȡ�û�����ID��									 			
** parameters:		cabinetNo:�����		logic �߼�����
** Returned value:	����ID��  ʧ�ܷ���0
*******************************************************************************/
unsigned char hd_id_by_logic(unsigned char cabinetNo,unsigned char logic)
{
	unsigned char i,j,id;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	//Trace("hd_id_by_logic:bin=%d,logic=%d\r\n",cabinetNo,logic);
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else{
		print_err(cabinetNo);
		return 0;
	}

	for(i = 0;i < HUODAO_MAX_ROW;i++){
		for(j = 0;j < HUODAO_MAX_COLUMN;j++){
			if(stHuodaoPtr[i].huodao[j].logicNo == logic){
				id = stHuodaoPtr[i].huodao[j].id;
				//Trace("stHuodaoPtr[%d][%d].id = %dr\n",i,j,id);
				return id;
			}		  	
		}
	}
	
	return 0 ;
	
}


/*****************************************************************************
** Function name:	hd_nums_by_id	
** Descriptions:	ͨ����Ż�û�������									 			
** parameters:		cabinetNo:�����		
** Returned value:	���ػ�������
*******************************************************************************/
unsigned char hd_nums_by_id(unsigned char cabinetNo)
{
	unsigned char i,j,nums = 0;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else{
		print_err(cabinetNo);
		return 0;
	}

	for(i = 0;i < HUODAO_MAX_ROW;i++){
		for(j = 0;j < HUODAO_MAX_COLUMN;j++){
			if((stHuodaoPtr[i].huodao[j].openFlag != 0) &&
				(stHuodaoPtr[i].huodao[j].id != 0)){
				nums++;
			}			
		}
	}

	return nums;
	
}

/*****************************************************************************
** Function name:	hd_state_by_id	
** Descriptions:	      ͨ��ID�Ż�û���״̬											 			
** parameters:		cabinetNo:�����		
					id :id��
** Returned value:	1:����  0������
*******************************************************************************/
unsigned char hd_state_by_id(unsigned char cabinetNo,unsigned char id)
{
	unsigned char i,j;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else{
		print_err(cabinetNo);
		return 0;
	}

	for(i = 0;i < HUODAO_MAX_ROW;i++){
		for(j = 0;j < HUODAO_MAX_COLUMN;j++){
			if(stHuodaoPtr[i].huodao[j].id == id){
				if((stHuodaoPtr[i].huodao[j].state == 1) && 
					(stHuodaoPtr[i].huodao[j].count != 0)){
					return 1;
				}
			}			
		}
	}

	return 0;
}

/*****************************************************************************
** Function name:	hd_get_by_id	
** Descriptions:	ͨ��ID�Ŷ�ȡ��������ֵ													 			
** parameters:		logicNo:�߼���ַ
					type��1������������
						  2������������
						  3��������״̬  ����״̬��1:����;2:����;3:�޻�;4:PC��λ������	
						  4���������������
						  5����ѡ������ֵ
						  6�����ɹ����״���
						  7������Ʒ���					
** Returned value:	0���ޣ�������Ӧֵ
*******************************************************************************/
unsigned int hd_get_by_id(unsigned char cabinetNo,unsigned char id,unsigned char type)
{
	unsigned char  i,j,err;
	unsigned int value = 0;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	if(cabinetNo == 1)
		stHuodaoPtr = stHuodao;
	else if(cabinetNo == 2 && SystemPara.SubBinOpen)
		stHuodaoPtr = stSubHuodao;
	else
		return 0;
	
	err = hd_get_index(cabinetNo,id,4,&i,&j);
	if(err)
	{
		switch(type)
		{
			case HUODAO_TYPE_PRICE:
				value = stHuodaoPtr[i].huodao[j].price;
				break;
			case HUODAO_TYPE_COUNT:
				value = stHuodaoPtr[i].huodao[j].count;
				break;
			case HUODAO_TYPE_MAX_COUNT:
				value = stHuodaoPtr[i].huodao[j].maxCount;
				break;
			case HUODAO_TYPE_STATE:
				value = stHuodaoPtr[i].huodao[j].state;
				break;
			case HUODAO_TYPE_SELECTNO:
				value = stHuodaoPtr[i].huodao[j].selectNo;
				break;
			case HUODAO_TYPE_ID:
				value = stHuodaoPtr[i].huodao[j].id;
				break;
			case HUODAO_TYPE_SUC_COUNT:
				value = stHuodaoPtr[i].huodao[j].sucCount;
				break;
			case HUODAO_TYPE_OPEN:
				value = (stHuodaoPtr[i].openFlag) ? (stHuodaoPtr[i].huodao[j].openFlag) : 0;
				break;
			default:
				break;
		}
		
	}
	return value;
	
}


/*****************************************************************************
** Function name:	hd_setNums_by_id	
** Descriptions:	      ͨ��ID�����û�������											 			
** parameters:		cabinetNo:�����		
					id :ID��   value ����ֵ
** Returned value:	0:�޴˻���;  ��0  ���ػ����߼���  0xFF ��ʾȱ��
*******************************************************************************/
unsigned char hd_setNums_by_id(unsigned char cabinetNo,unsigned char id,unsigned char value)
{
	unsigned char i,j;
	ST_LEVEL_HUODAO *stHuodaoPtr;

	if(cabinetNo == 1){
		stHuodaoPtr = stHuodao;
	}	
	else if(cabinetNo == 2 && SystemPara.SubBinOpen){
		stHuodaoPtr = stSubHuodao;
	}	
	else{
		print_err(cabinetNo);
		return 0;
	}
	
	for(i = 0;i < HUODAO_MAX_ROW;i++){
		for(j = 0;j < HUODAO_MAX_COLUMN;j++){
			if(stHuodaoPtr[i].huodao[j].id == id){
				if(value == 0){
					stHuodaoPtr[i].huodao[j].count = 0;
					if(stHuodaoPtr[i].huodao[j].state != HUODAO_STATE_FAULT){
						stHuodaoPtr[i].huodao[j].state = HUODAO_STATE_EMPTY;
					}
				}
				else{
					stHuodaoPtr[i].huodao[j].count = value & 0x3F;
					if(stHuodaoPtr[i].huodao[j].state != HUODAO_STATE_FAULT){
						stHuodaoPtr[i].huodao[j].state = HUODAO_STATE_NORMAL;
					}
				}
			}
		}
	}
	
	return 1;
}


/*****************************************************************************
** Function name:	hd_ids_by_level	
** Descriptions:	ͨ����Ż�ȡ�ò����ʼID�ͽ���ID											 			
** parameters:		cabinetNo:�����		
					level :���   
					startId ��ʼID stopid ����ID
** Returned value:	0:ʧ��; 1�ɹ�
*******************************************************************************/
unsigned char hd_ids_by_level(unsigned char cabinetNo,unsigned char level,unsigned char *startId,unsigned char *stopId)
{
	unsigned char i,j,temp = 0;
	ST_LEVEL_HUODAO *stHuodaoPtr;
	*startId = 0;
	*stopId = 0;
	if(cabinetNo == 1){
		stHuodaoPtr = stHuodao;
	}	
	else if(cabinetNo == 2 && SystemPara.SubBinOpen){
		stHuodaoPtr = stSubHuodao;
	}	
	else{
		print_err(cabinetNo);
		return 0;
	}
	
	for(i = 0;i < HUODAO_MAX_ROW;i++){
		if(stHuodaoPtr[i].openFlag != 1)
			continue;
		temp++;
		if(level != temp)
			continue;
		for(j = 0;j < HUODAO_MAX_COLUMN;j++){
			if(stHuodaoPtr[i].huodao[j].openFlag == 1 &&
				stHuodaoPtr[i].huodao[j].id != 0){
				if(*startId == 0){
					*startId = stHuodaoPtr[i].huodao[j].id;
				}
				else{
					*stopId = stHuodaoPtr[i].huodao[j].id;
				}
			}
		}
	}

	
	return (*startId != 0 && *stopId != 0);
}



/*--------------------------------------------------------------------------------
										�޸ļ�¼
1.���ڣ�2013.11.6 �޸��ˣ�liya 
  ���ݣ����ļ��еĺ��������������
--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
										�޸ļ�¼
1.���ڣ�2014.04.01 �޸��ˣ�yoc
  ���ݣ�������ͻ����ṹ�弰 �ӿں���
--------------------------------------------------------------------------------*/





