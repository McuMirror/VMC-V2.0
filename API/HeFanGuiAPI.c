/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           HeFanGuiAPI.c
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        �з����API�ӿ�                     
**------------------------------------------------------------------------------------------------------
** Created by:          liya
** Created date:        2014-01-20
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/

#include "..\config.h"

unsigned char ChaXunRst[2]={0};

#pragma arm section zidata = "RAM2" rwdata = "RAM2"
HEFAN_CABINET stHefanCabinet[HE_FAN_MAX_CABINET];
#pragma arm section



const static unsigned short hefan_flash_page_th[HE_FAN_MAX_CABINET] = {

	HEFAN1_HUODAO_PAGE_TH,
	HEFAN2_HUODAO_PAGE_TH,
	HEFAN3_HUODAO_PAGE_TH,
	HEFAN4_HUODAO_PAGE_TH,
	HEFAN5_HUODAO_PAGE_TH,
	HEFAN6_HUODAO_PAGE_TH,
	HEFAN7_HUODAO_PAGE_TH,
	HEFAN8_HUODAO_PAGE_TH//,
//	HEFAN9_HUODAO_PAGE_TH,
//	HEFAN10_HUODAO_PAGE_TH
	
};
const static unsigned short hefan_flash_page_lh[HE_FAN_MAX_CABINET] = {

	HEFAN1_HUODAO_PAGE_LH,
	HEFAN2_HUODAO_PAGE_LH,
	HEFAN3_HUODAO_PAGE_LH,
	HEFAN4_HUODAO_PAGE_LH,
	HEFAN5_HUODAO_PAGE_LH,
	HEFAN6_HUODAO_PAGE_LH,
	HEFAN7_HUODAO_PAGE_LH,
	HEFAN8_HUODAO_PAGE_LH//,
//	HEFAN9_HUODAO_PAGE_LH,
//	HEFAN10_HUODAO_PAGE_LH
	
};



//=========================���� �з��� ҳ��ṹ��============================================
typedef struct _hefan_cabinet_lcd_
{
	char *hefanManage[MAXLANGUAGETYPE];//����  �з������
	char *hefanNOManage[MAXLANGUAGETYPE];//����  �з���1����
	char *hefanCabAndHuodaoManage[MAXLANGUAGETYPE];//����  �з�1���13����
	char *huodaoInfo[MAXLANGUAGETYPE];//��������
	char *huodaoTest[MAXLANGUAGETYPE];//��������
	char *hefanControl[MAXLANGUAGETYPE];//���ڿ���
	char *huodaoAddGoods[MAXLANGUAGETYPE];//�������
	char *huodaoAddGoodsALl[MAXLANGUAGETYPE];//ȫ�������
	char *huodaoAddGoodsOne[MAXLANGUAGETYPE];//���������
	char *huodaoAddGoodsOK[MAXLANGUAGETYPE];//����ɹ�
	char *huodaoAddGoodsFail[MAXLANGUAGETYPE];//���ʧ��
	char *hefanlight[MAXLANGUAGETYPE];//�з�����
	char *hefanhot[MAXLANGUAGETYPE];//�з�����
	char *hefancool[MAXLANGUAGETYPE];//�з�����
	char *InputCabinetNo[MAXLANGUAGETYPE];//��������
	char *InputHuodaoNo[MAXLANGUAGETYPE];//��������Ӻ�
	char *huodaoPrice[MAXLANGUAGETYPE];//��������
	char *huodaoOpenState[MAXLANGUAGETYPE];//��������״̬
	char *huodaoTradeNum[MAXLANGUAGETYPE];//�����ɹ����״���
	char *huodaoTestting[MAXLANGUAGETYPE];//�������ڲ���
	char *huodaoOK[MAXLANGUAGETYPE];//��������
	char *huodaoFail[MAXLANGUAGETYPE];//��������
	char *open[MAXLANGUAGETYPE];//����
	char *close[MAXLANGUAGETYPE];//�ر�
	char *indexOpen[MAXLANGUAGETYPE];//����
	char *indexClose[MAXLANGUAGETYPE];//�ر�
	char *noSurport[MAXLANGUAGETYPE];//��֧��
	char *cabinetNonExist[MAXLANGUAGETYPE];//��Ų�����
	char *cabinetInit[MAXLANGUAGETYPE];//�����ʼ��
	char *cabinetInitOK[MAXLANGUAGETYPE];//�����ʼ�����
	char *cabinetInitting[MAXLANGUAGETYPE];//�������ڳ�ʼ��
	char *cabinetInitFail[MAXLANGUAGETYPE];//��ʼ��ʧ�ܻ��񲻴���
	char *cabinetConfirm[MAXLANGUAGETYPE];//ȷ��
	char *cabinetCancel[MAXLANGUAGETYPE];//ȡ��
	
}HEFAN_CABINET_LCD;




const HEFAN_CABINET_LCD st_hefan_cabinet = 
{
	{
		"�з������",
		"Cabinet box",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"�з���%d:����",
		"Cabinet %d:box",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"�з�%d��,��%d����",
		"The %d cabinet %d box",
		"Column",
		"SUTUN",
		"BANDEJA"
	}
	,
	{
		"�� ��������",
		"1.Box info",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"�� ��������",
		"2.Box test",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"�� ���ڿ���",
		"3.Cabinet device control",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"�� �������",
		"4.Add goods ",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"�� ȫ�������",
		"1.Load for all boxes",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"�� ���������",
		"2.Load for box",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"����ɹ�!",
		"Add the goods succeed!",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"���ʧ��!",
		"Add the goods failed!",
		"Column",
		"SUTUN",
		"BANDEJA"
	}
		,

	{
		"�� ��������:",
		"1.Cabinet lighting:",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"�� ���ڼ���:",
		"2.Cabinet heating:",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"�� ��������:",
		"3.Cabinet cooling:",
		"Column",
		"SUTUN",
		"BANDEJA"
	}
	,
	{
		"��������:",
		"Input cabinet code: ",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"��������Ӻ�:",
		"Input box code: ",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"�� ��������:",
		"1.Unit price:",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"�� ��������:",
		"2.Box status:",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"�� �ɹ����״���:",
		"3.Success deal qty:",
		"Column",
		"SUTUN",
		"BANDEJA"
	}
	,
	{
		"���ڲ��Ի���%d...",
		"Self-testing %d,pls wait...",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"�������: ����!",
		"Box: Normal!",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"�������: ����!",
		"Box: Error!",
		"Column",	
		"SUTUN",
		"BANDEJA"
	}
		,
	{
		"����",
		"ON",
		"Column",
		"SUTUN",
		"BANDEJA"
	}
		,
	{
		"�ر�",
		"OFF",
		"Column",
		"SUTUN",
		"BANDEJA"
	}	,
	{
		"�� ����",
		"1.ON",
		"Column",
		"SUTUN",
		"BANDEJA"
	}
		,
	{
		"�� �ر�",
		"2.OFF",
		"Column",
		"SUTUN",
		"BANDEJA"
	}
			,
	{
		"��֧��",
		"N/A",
		"Column",
		"SUTUN",
		"BANDEJA"
	}		,
	{
		"��Ų�����!",
		"Cabinet: N/A!",
		"Column",
		"SUTUN",
		"BANDEJA"
	}	,
	{
		"�� ������ʼ��",
		"5.Box initialize",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"��ʼ���ɹ�",
		"Initialize succeed!",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"���ڳ�ʼ��...",
		"Initializing...",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"��ʼ��ʧ��,���񲻴���!",
		"Initialize Failed!",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"�� ȷ��",
		"1.Enter",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"�� ȡ��",
		"2.Cancel",
		"Column",
		"SUTUN",
		"BANDEJA"
	}
};




/*****************************************************************************
** Function name:	save_hefan_huodao_info	
** Descriptions:	�з���������溯��			
**														 			
** parameters:		cabinetNo: �з����  
					
** Returned value:	1:�ɹ�;0:ʧ��
** 
*******************************************************************************/
unsigned char save_hefan_huodao_info(unsigned char cabinetNo)
{
	unsigned char buf[512] = {0},rst,*st_ptr,i;
	unsigned short crc,len_th,len_lh;
	len_th = sizeof(HEFAN_CABINET) / 2;
	len_lh = sizeof(HEFAN_CABINET) - len_th;
	st_ptr = (unsigned char *)&stHefanCabinet[cabinetNo - 1];

	//�洢�ϰ벿	
	memcpy(buf,(void *)st_ptr,len_th);
	crc = CrcCheck(buf,len_th);
	buf[len_th] = HUINT16(crc);
	buf[len_th + 1] = LUINT16(crc);
	rst = AT45DBWritePage(hefan_flash_page_th[cabinetNo - 1],buf);

	//�洢�°벿
	memcpy(buf,(void *)&st_ptr[len_th],len_lh);
	crc = CrcCheck(buf,len_lh);
	buf[len_lh] = HUINT16(crc);
	buf[len_lh + 1] = LUINT16(crc);
	rst = AT45DBWritePage(hefan_flash_page_lh[cabinetNo - 1],buf);

	#if 0
	print_hefan("save hefan[%d] state =%d nums= %d\r\n",
		cabinetNo - 1,stHefanCabinet[(cabinetNo - 1)].state,
		stHefanCabinet[(cabinetNo - 1)].huodaoNums);
	for(i = 0; i < 88;i++)
	{
		print_hefan("huodao[%d] price = %d state = %d tradeNums =%d id = %d\r\n",
			i,
			stHefanCabinet[(cabinetNo - 1)].huodao[i].price,
			stHefanCabinet[(cabinetNo - 1)].huodao[i].state,
			stHefanCabinet[(cabinetNo - 1)].huodao[i].tradeNum,
			stHefanCabinet[(cabinetNo - 1)].huodao[i].id
			);
	}
	#endif
	return rst;
}

/*****************************************************************************
** Function name:	save_hefan_huodao_by_flash	
** Descriptions:	�з���������溯��			
**														 			
** parameters:		cabinetNo: �з����  
					
** Returned value:	1:�ɹ�;0:ʧ��
** 
*******************************************************************************/

void save_hefan_huodao_by_flash(void)
{
	unsigned char i;
	for(i = 0; i < HE_FAN_MAX_CABINET;i++)
	{
		save_hefan_huodao_info(i + 1);	
	}
}


/*****************************************************************************
** Function name:	read_hefan_huodao_info	
** Descriptions:	�з��������ȡ����			
**														 			
** parameters:		cabinetNo: �з����  
					
** Returned value:	1:�ɹ�;0:ʧ��
** 
*******************************************************************************/
unsigned char read_hefan_huodao_info(unsigned char cabinetNo)
{
	unsigned char buf[512] = {0},*st_ptr,i;
	unsigned short len_th = 0,len_lh = 0,crc;
	
	len_th = sizeof(HEFAN_CABINET) / 2;
	len_lh = sizeof(HEFAN_CABINET) - len_th;
	st_ptr = (unsigned char *)&stHefanCabinet[cabinetNo - 1];

	AT45DBReadPage(hefan_flash_page_th[(cabinetNo - 1)],buf);
	crc = CrcCheck(buf,len_th);
	print_hefan("read:len_th=%d,crc = %d bufCrc = %d\r\n",
		len_th,crc,INTEG16(buf[len_th],buf[len_th+1]));
	if(buf[len_th] != HUINT16(crc) || buf[len_th + 1] != LUINT16(crc))
		return 0;
	//���ƻ����ϰ벿��Ϣ
	memcpy((void *)&st_ptr[0],buf,len_th);

	AT45DBReadPage(hefan_flash_page_lh[(cabinetNo - 1)],buf);
	crc = CrcCheck(buf,len_lh);
	print_hefan("read:len_lh=%d,crc = %d bufCrc = %d\r\n",
		len_lh,crc,INTEG16(buf[len_lh],buf[len_lh+1]));
	if(buf[len_lh] != HUINT16(crc) || buf[len_lh + 1] != LUINT16(crc))
		return 0;
	//���ƻ����ϰ벿��Ϣ
	memcpy((void *)&st_ptr[len_th],buf,len_lh);
	#if 0
	print_hefan("read hefan[%d] state =%d nums= %d\r\n",
	cabinetNo - 1,stHefanCabinet[(cabinetNo - 1)].state,
	stHefanCabinet[(cabinetNo - 1)].huodaoNums);
	for(i = 0; i < 88;i++)
	{
		print_hefan("huodao[%d] price = %d state = %d tradeNums =%d id = %d\r\n",
			i,
			stHefanCabinet[(cabinetNo - 1)].huodao[i].price,
			stHefanCabinet[(cabinetNo - 1)].huodao[i].state,
			stHefanCabinet[(cabinetNo - 1)].huodao[i].tradeNum,
			stHefanCabinet[(cabinetNo - 1)].huodao[i].id
			);
	}
	#endif
	return 1;
	

	
	
	
	
	

}



/*****************************************************************************
** Function name:	read_hefan_huodao_info	
** Descriptions:	�з��������ȡ����			
**														 			
** parameters:		cabinetNo: �з����  
					
** Returned value:	1:�ɹ�;0:ʧ��
** 
*******************************************************************************/
unsigned char read_hefan_info_by_flash(void)
{
	unsigned char i;
	for(i = 0; i < HE_FAN_MAX_CABINET;i++)
	{
		read_hefan_huodao_info(i + 1);
		
	}
	return 1;
}

/*****************************************************************************
** Function name:	hefanHuodaoInit	
** Descriptions:	�з����ʼ������	
**														 			
** parameters:		cabinetNo: �з����  
					
** Returned value:	1:�ɹ�;0:ʧ��
** 
*******************************************************************************/
unsigned char hefanHuodaoInit(unsigned char cabinetNo)
{
	unsigned char err = 0,j,temp;
	memset((void *)&stHefanCabinet[cabinetNo - 1],0,sizeof(HEFAN_CABINET));
	print_hefan("memset sizeof(HEFAN_CABINET) =%d\r\n",sizeof(HEFAN_CABINET));
	err = HeFanGuiHandle(0,HEFANGUI_CHAXUN,cabinetNo - 1);
	if(err == 1)
	{
		stHefanCabinet[cabinetNo - 1].huodaoNums = ChaXunRst[0];
		temp = ChaXunRst[1];
		setHefanCabinetState(cabinetNo,HEFAN_EXIST,1);
		if((temp >> 2) & 0x01)//����
		{
			setHefanCabinetState(cabinetNo,HEFAN_LIGHT,1);
			if(HeFanGuiHandle(0,HEFANGUI_ZHAOMINGKAI,cabinetNo - 1))
				setHefanCabinetState(cabinetNo,HEFAN_LIGHT_OPEN,1);
			else
				setHefanCabinetState(cabinetNo,HEFAN_LIGHT_OPEN,0);	
		}
		else
			setHefanCabinetState(cabinetNo,HEFAN_LIGHT,0);
		if((temp >> 0) & 0x01)//����
		{
			setHefanCabinetState(cabinetNo,HEFAN_HOT,1);
			if(HeFanGuiHandle(0,HEFANGUI_JIAREKAI,cabinetNo - 1))
				setHefanCabinetState(cabinetNo,HEFAN_HOT_OPEN,1);
			else
				setHefanCabinetState(cabinetNo,HEFAN_HOT_OPEN,0);	
		}		
		else
			setHefanCabinetState(cabinetNo,HEFAN_HOT,0);	

		if((temp >> 1) & 0x01)//����
		{
			setHefanCabinetState(cabinetNo,HEFAN_COOL,1);
			if(HeFanGuiHandle(0,HEFANGUI_ZHILENGKAI,cabinetNo - 1))
				setHefanCabinetState(cabinetNo,HEFAN_COOL_OPEN,1);
			else
				setHefanCabinetState(cabinetNo,HEFAN_COOL_OPEN,0);	
		}		
		else
			setHefanCabinetState(cabinetNo,HEFAN_COOL,0);
		print_hefan("cabinet = %d,nums =%d\r\n",cabinetNo,stHefanCabinet[cabinetNo - 1].huodaoNums);
		for(j = 0;j < stHefanCabinet[cabinetNo - 1].huodaoNums;j++)
		{
			stHefanCabinet[cabinetNo - 1].huodao[j].state = HEFAN_HUODAO_NORMAL;
			stHefanCabinet[cabinetNo - 1].huodao[j].price = 100;
		}	
	}
	else
	{
		setHefanCabinetState(cabinetNo,HEFAN_EXIST,0);	
		
	}
	save_hefan_huodao_info(cabinetNo);

	return err;
		
	
}


/*****************************************************************************
** Function name:	HeFanGuiSendMBox	
**
** Descriptions:	���豸��������������	
**					
**														 			
** parameters:		HandleType:��������
					ChkFlag:�Ƿ����������
					Binnum:�����
** Returned value:	1:�ɹ�;0:ʧ��;
** 
*****************************************************************************/
unsigned char HeFanGuiSendMBox(unsigned char HandleType,unsigned char ChannelNum,unsigned char Binnum)
{
	MessagePack *RecvBoxPack;
	uint8_t err,i;

	MsgAccepterPack.HeFanGuiHandle = HandleType;
	MsgAccepterPack.HeFanGuiNum = ChannelNum;
	MsgAccepterPack.Binnum = Binnum;
	//���������DevieTask�������Hopper�����Ҳ���
	OSMboxPost(g_HeFanGuiMail,&MsgAccepterPack);
	//�ȴ����䷵��
	RecvBoxPack = OSMboxPend(g_HeFanGuiBackMail,5000,&err);
	if(err == OS_NO_ERR)
	{
		print_hefan("HeFanGuiBuf=");
		for(i=0;i<RecvBoxPack->HeFanGuiBuf[1];i++)
			print_hefan("%x ",RecvBoxPack->HeFanGuiBuf[i]);
		print_hefan("\r\n");
		if(HandleType==HEFANGUI_CHAXUN)
		{
			ChaXunRst[0] = RecvBoxPack->HeFanGuiBuf[6];
			ChaXunRst[1] = RecvBoxPack->HeFanGuiBuf[8];
		}
		return RecvBoxPack->HeFanGuiRst;
	}
	return 0xff;	
}

/*****************************************************************************
** Function name:	hefanOutGoodsHandle	
** Descriptions:	  �з����������				
**														 			
** parameters:		HandleType:��������
					ChkFlag:�Ƿ����������
** Returned value:	1:�ɹ�;0:ʧ��
** 
*******************************************************************************/
unsigned char hefanOutGoodsHandle(unsigned char LogicNum,unsigned char HandleType,unsigned char Binnum)
{
	unsigned char Rst=0;
	if(LogicNum == 0xff)
		return 2;
	Rst = HeFanGuiSendMBox(HandleType,LogicNum,Binnum);
	if(Rst==1)
	{
		if(SystemPara.hefangGuiKou && HandleType == HEFANGUI_KAIMEN && stHefanCabinet[Binnum].huodao[LogicNum - 1].state == HEFAN_HUODAO_NORMAL)
			stHefanCabinet[Binnum].huodao[LogicNum - 1].state = HEFAN_HUODAO_EMPTY;
		stHefanCabinet[Binnum].huodao[LogicNum - 1].tradeNum++;
		return 1;
	}
	return 0;
}



/*****************************************************************************
** Function name:	HeFanGuiHandle	
** Descriptions:	�з������������					
**														 			
** parameters:		HandleType:��������
					ChkFlag:�Ƿ����������
** Returned value:	1:�ɹ�;0:ʧ��
** 
*******************************************************************************/
unsigned char HeFanGuiHandle(unsigned char LogicNum,unsigned char HandleType,unsigned char Binnum)
{
	unsigned char Rst=0;
	print_hefan("hefan.. logicNO = %d,handleType = %d,cabinetNo = %d\r\n",
		LogicNum,HandleType,Binnum);
	if(LogicNum == 0xff)
		return 2;
	Rst = HeFanGuiSendMBox(HandleType,LogicNum,Binnum);
	TraceChannel("OutGoods_DeviceBack=%x\r\n",Rst);
	if(Rst==1)
	{
		Trace("GUINum=%d Feature=%x\r\n",ChaXunRst[0],ChaXunRst[1]);
		return 1;
	}
	return 0;
}


/*****************************************************************************
** Function name:	getHefanHuodaotState	
** Descriptions:	��úз������״̬			
**														 			
** parameters:		cabinetNo  ��� 
					
** Returned value:	1 ���� 2 ȱ�� 3���� 4�ر�
** 
*******************************************************************************/
unsigned char getHefanHuodaotState(unsigned char cabinetNo,unsigned char boxNO)
{
	return stHefanCabinet[cabinetNo - 1].huodao[boxNO - 1].state;
}


/*****************************************************************************
** Function name:	setHefanHuodaotState	
** Descriptions:	��úз������״̬			
**														 			
** parameters:		cabinetNo  ��� value  1 ���� 2 ȱ�� 3���� 4�ر�
					
** Returned value:	
** 
*******************************************************************************/
unsigned char setHefanHuodaotState(unsigned char cabinetNo,unsigned char boxNO,unsigned char value)
{
	stHefanCabinet[cabinetNo - 1].huodao[boxNO - 1].state = value;
	return 1;
}


/*****************************************************************************
** Function name:	getHefanCabinetState	
** Descriptions:	��úз���״̬			
**														 			
** parameters:		cabinetNo
					
** Returned value:	
** 
*******************************************************************************/
unsigned char getHefanCabinetState(unsigned char cabinetNo,unsigned char type)
{
	unsigned char state;
	state = stHefanCabinet[cabinetNo - 1].state;
	return ((state >> type) & 0x01);	
}




/*****************************************************************************
** Function name:	setHefanCabinetState	
** Descriptions:	���úз���״̬			
**														 			
** parameters:		cabinetNo
					
** Returned value:	
** 
*******************************************************************************/
unsigned char setHefanCabinetState(unsigned char cabinetNo,unsigned char type,unsigned char state)
{

	if(state)
		stHefanCabinet[cabinetNo - 1].state |= (0x01) << type;
	else
		stHefanCabinet[cabinetNo - 1].state &= ~((0x01) << type);
	
	return 1;
}



/*****************************************************************************
** Function name:	get_hefan_Index	
** Descriptions:	�з���Ż�ýṹ��������																		 			
** parameters:		cabinetNo				
** Returned value:	 
*******************************************************************************/
unsigned char get_hefan_Index(unsigned char cabinetNo)
{
	
	return ((cabinetNo >= 3)  && (cabinetNo <= (HE_FAN_MAX_CABINET + 2))) ? (cabinetNo - 2) : 0;
}

/*****************************************************************************
** Function name:	hefan_clear_trade	
** Descriptions:	�з���������׼�¼																		 			
** parameters:		cabinetNo				
** Returned value:	 
*******************************************************************************/
unsigned char hefan_clear_trade(unsigned char cabinetNo)
{
	unsigned char i = 0;
	if(!cabinetNo || cabinetNo > HE_FAN_MAX_CABINET)
		return 0;
	for(i = 0;i < HE_FAN_MAX_HUODAO;i++)
		stHefanCabinet[cabinetNo - 1].huodao[i].tradeNum = 0;
	save_hefan_huodao_info(cabinetNo);
	return 1;
}



/*****************************************************************************
** Function name:	hefanGetParaVlaue	
** Descriptions:	��ȡ�з������			
**														 			
** parameters:		type 1 ����
**					boxNo 1- 88  
					
** Returned value:	
** 
*******************************************************************************/
unsigned int hefanGetParaVlaue(unsigned char cabinetNo,unsigned char boxNo,unsigned char type)
{
	unsigned int value = 0;
	if(!cabinetNo || (cabinetNo > (HE_FAN_MAX_CABINET)) || !boxNo || boxNo > HE_FAN_MAX_HUODAO)
		return 0;
	
	switch(type)
	{
		case HEFAN_HUODAO_PRICE://����
			value = stHefanCabinet[cabinetNo - 1].huodao[boxNo - 1].price;
			break;
		case HEFAN_HUODAO_STATE:
			value = stHefanCabinet[cabinetNo - 1].huodao[boxNo - 1].state;
			break;
		case HEFAN_HUODAO_TRADE_NUM:
			value = stHefanCabinet[cabinetNo - 1].huodao[boxNo - 1].tradeNum;
			break;
		case HEFAN_HUODAO_ID:
			value = stHefanCabinet[cabinetNo - 1].huodao[boxNo - 1].id;
			break;
		case HEFAN_CABINET_NUMS:
			value = stHefanCabinet[cabinetNo - 1].huodaoNums;
			break;
		default:
			break;
	}
	
	return (value)  ? value : 0;
}

/*****************************************************************************
** Function name:	hefanSetParaVlaue	
** Descriptions:	���úз������			
**														 			
** parameters:		type 1 ����boxNo 1- 88
					
** Returned value:	
** 
*******************************************************************************/
unsigned char hefanSetParaVlaue(unsigned char cabinetNo,unsigned char boxNo,unsigned int value,unsigned char type)
{

	unsigned char flag = 0;
	if((!cabinetNo) || (cabinetNo > (HE_FAN_MAX_CABINET)) || !boxNo || boxNo > HE_FAN_MAX_HUODAO)
		return 0;
	
	switch(type)
	{
		case HEFAN_HUODAO_PRICE://����
			stHefanCabinet[cabinetNo - 1].huodao[boxNo - 1].price = value;
			flag = 1;
			break;
		case HEFAN_HUODAO_STATE:
			stHefanCabinet[cabinetNo - 1].huodao[boxNo - 1].state = value;
			flag = 1;
			break;
		case HEFAN_HUODAO_TRADE_NUM:
			stHefanCabinet[cabinetNo - 1].huodao[boxNo - 1].tradeNum = value;
			flag = 1;
			break;
		case HEFAN_HUODAO_ID:
			stHefanCabinet[cabinetNo - 1].huodao[boxNo - 1].id = value;
			flag = 1;
			break;
		case HEFAN_CABINET_NUMS:
			stHefanCabinet[cabinetNo - 1].huodaoNums = value;
			flag = 1;
			break;
		default:
			break;
	}
	
	return (flag)  ? 1 : 0;
}

/*****************************************************************************
** Function name:	hefanSetParaVlaue	
** Descriptions:	���úз������			
**														 			
** parameters:		cabinetNo ��� boxNo 1- 88
					
** Returned value:	0��������1������Ϊ0,2������Ϊ0��3����������;4:ȱ����5���޴˻���
** 
*******************************************************************************/
unsigned char hefanHuodaoCheck(unsigned char cabinetNo,unsigned char boxNo)
{
	
	if( !get_hefan_Index(cabinetNo) || !boxNo || boxNo > stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodaoNums)
	{
		print_hefan("1 cabinetNo=%d,boxNo =%d\r\n",cabinetNo,boxNo);
		return 5;	
	}

	if(!getHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_EXIST))
		return 5;
	
	if(stHefanCabinet[get_hefan_Index(cabinetNo) -1].huodao[boxNo - 1].price == 0)
	{
		print_hefan("2 cabinetNo=%d,boxNo =%d\r\n",cabinetNo,boxNo);
		return 1;
	}
	if(stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodao[boxNo - 1].state != HEFAN_HUODAO_NORMAL)
	{
		if(stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodao[boxNo - 1].state == HEFAN_HUODAO_FAULT)
			return 3;
		else if(stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodao[boxNo - 1].state == HEFAN_HUODAO_EMPTY)
			return 2;
		
		return 5;
	}
		
	
		
	return 0;

	
}



/*****************************************************************************
** Function name:	hefanAddgoodsHandle	
** Descriptions:	�з����������			
**														 			
** parameters:		cabinetNo ���type 0 ȫ��������������ʾ �����������Ϊ������
					
** Returned value:	
** 
*******************************************************************************/
unsigned char hefanAddgoodsHandle(unsigned char cabinetNo,unsigned  char type)
{
	unsigned char i;
	if(cabinetNo == 0 || cabinetNo > HE_FAN_MAX_CABINET)
		return 0;

	if(type == 0)
	{	
		for(i = 0; i < HE_FAN_MAX_HUODAO;i++)
		{
			if(stHefanCabinet[cabinetNo - 1].huodao[i].state == HEFAN_HUODAO_NORMAL)
				continue;
			else if(stHefanCabinet[cabinetNo - 1].huodao[i].state == HEFAN_HUODAO_EMPTY)
				stHefanCabinet[cabinetNo - 1].huodao[i].state = HEFAN_HUODAO_NORMAL;
		}
	}
	else 
	{
		if(type > HE_FAN_MAX_HUODAO)
			return 0;
		if(stHefanCabinet[cabinetNo - 1].huodao[type - 1].state == HEFAN_HUODAO_EMPTY || 
			stHefanCabinet[cabinetNo - 1].huodao[i].state == HEFAN_HUODAO_NORMAL)
		{
			stHefanCabinet[cabinetNo - 1].huodao[type- 1].state = HEFAN_HUODAO_NORMAL;
			return 1;
		}
		else
			return 0;
				
	}

	return 1;
}





//=========================����Ϊ�з��� ҳ���������============================================






/*****************************************************************************
** Function name:	hefanHuodaoInfo	
** Descriptions:	�з����������ҳ��				
**														 			
** parameters:		
					
** Returned value:	
** 
*******************************************************************************/

void hefanHuodaoInfo(unsigned char cabinetNo)
{
	unsigned char  key =0xff,returnFlag  = 0,enterSub = 0,turnFlush = 0;
	unsigned short huodaoNo;
	unsigned char editMenu = 0,openState = 0;

	unsigned short price;
	char *stateStr;
	LCDClrScreen();
	LCDDrawRectangle(0,0,239,15);
	LCDDrawRectangle(0,0,239,3);
	LCDClrArea(1,4,238,14);
	LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
	LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.InputHuodaoNo[SystemPara.Language]);
	
	huodaoNo = 0;
	while(1)
	{
		key = ReadKeyValue();
		switch(key)
		{
			case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case '0':
				huodaoNo = (huodaoNo * 10 + key - '0');				
				LCDPrintf(8,5,0,SystemPara.Language,"%s%d",st_hefan_cabinet.InputHuodaoNo[SystemPara.Language],huodaoNo);
				break;
			case 'C':
				if(huodaoNo)
					huodaoNo = 0;
				else
					returnFlag = 1;
				LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.InputHuodaoNo[SystemPara.Language]);
				break;
			case 'E':
				if(huodaoNo && huodaoNo <= stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodaoNums)
				{
					price = stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodao[huodaoNo - 1].price;
					openState = (stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodao[huodaoNo - 1].state == HEFAN_HUODAO_CLOSE) ? 0 : 1;
					
					enterSub = 1;
					LCDClrArea(1,4,238,14);
					LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanCabAndHuodaoManage[SystemPara.Language],cabinetNo,huodaoNo);
					LCDPrintf(8,5,0,SystemPara.Language,"%s%2d.%02d",st_hefan_cabinet.huodaoPrice[SystemPara.Language],price/100,price%100);					

					if(openState)
						stateStr = st_hefan_cabinet.open[SystemPara.Language];
					else
						stateStr = st_hefan_cabinet.close[SystemPara.Language];
					LCDPrintf(8,7,0,SystemPara.Language,"%s%s",st_hefan_cabinet.huodaoOpenState[SystemPara.Language],stateStr);
					LCDPrintf(8,9,0,SystemPara.Language,"%s%d",
						st_hefan_cabinet.huodaoTradeNum[SystemPara.Language],
						hefanGetParaVlaue(get_hefan_Index(cabinetNo),huodaoNo,HEFAN_HUODAO_TRADE_NUM));
	
				}
				else
				{
					huodaoNo = 0;
					LCDPrintf(8,5,0,SystemPara.Language,"%s%d",st_hefan_cabinet.InputHuodaoNo[SystemPara.Language],huodaoNo);
				}
				break;
			default:
				break;
		}
		if(returnFlag)
		{
			save_hefan_huodao_info(get_hefan_Index(cabinetNo));
			return;
		}	
		OSTimeDly(5);

		//���������Ϣ
		
		while(enterSub)
		{
			key = ReadKeyValue();
			switch(key)
			{
				case '1'://����༭״̬
					price = 0;
					editMenu  = 1;
					LCDPrintf(8,5,1,SystemPara.Language,"%s%2d.%02d",st_hefan_cabinet.huodaoPrice[SystemPara.Language],price/100,price%100);
					break;
				case '2':
					
					editMenu  = 2;
					LCDPrintf(8,7,1,SystemPara.Language,"%s %s %s",
						st_hefan_cabinet.huodaoOpenState[SystemPara.Language],
						st_hefan_cabinet.indexOpen[SystemPara.Language],st_hefan_cabinet.indexClose[SystemPara.Language]);
	
					break;


				case 'C':
					if(!editMenu)
					{
						enterSub = 0;
						huodaoNo = 0;
					}
					price = stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodao[huodaoNo - 1].price;
					LCDPrintf(8,5,0,SystemPara.Language,"%s%2d.%02d",st_hefan_cabinet.huodaoPrice[SystemPara.Language],price/100,price%100);
					break;
				case '<'://�Ϸ�
					if(huodaoNo > 1)
						huodaoNo--;
					else
						huodaoNo = stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodaoNums;
					turnFlush = 1;
					
					break;
				case '>':
					if(huodaoNo < stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodaoNums)
						huodaoNo++;
					else
						huodaoNo = 1;
					turnFlush = 1;
					break;
				default:
					break;
					
			}
			OSTimeDly(5);
			if(turnFlush)
			{
				price = stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodao[huodaoNo - 1].price;
				openState = (stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodao[huodaoNo - 1].state == HEFAN_HUODAO_CLOSE) ? 0 : 1;
						
				LCDClrArea(1,4,238,14);
				LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanCabAndHuodaoManage[SystemPara.Language],cabinetNo,huodaoNo);
				LCDPrintf(8,5,0,SystemPara.Language,"%s%2d.%02d",st_hefan_cabinet.huodaoPrice[SystemPara.Language],price/100,price%100);					
				if(openState)
						stateStr = st_hefan_cabinet.open[SystemPara.Language];
				else
						stateStr = st_hefan_cabinet.close[SystemPara.Language];
				LCDPrintf(8,7,0,SystemPara.Language,"%s%s",st_hefan_cabinet.huodaoOpenState[SystemPara.Language],stateStr);
				LCDPrintf(8,9,0,SystemPara.Language,"%s%d",
						st_hefan_cabinet.huodaoTradeNum[SystemPara.Language],
						hefanGetParaVlaue(get_hefan_Index(cabinetNo),huodaoNo,HEFAN_HUODAO_TRADE_NUM));
	
				turnFlush = 0;
			}
			if(enterSub == 0)//�����ϲ� ˢ��ҳ��
			{
				LCDClrArea(1,4,238,14);
				LCDDrawRectangle(0,0,239,15);
				LCDDrawRectangle(0,0,239,3);
				LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
				LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.InputHuodaoNo[SystemPara.Language]);

			}

			//����༭�˵�
			while(editMenu)
			{
				key = ReadKeyValue();
				if(editMenu == 1)//�༭����
				{
					switch(key)
					{
						case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case '0':
							price = price * 10 + key - '0';
							LCDPrintf(8,5,1,SystemPara.Language,"%s%2d.%02d",st_hefan_cabinet.huodaoPrice[SystemPara.Language],price/100,price%100);
							break;
						case 'C':
							if(price)
							{
								price = 0;
								LCDPrintf(8,5,1,SystemPara.Language,"%s%2d.%02d",st_hefan_cabinet.huodaoPrice[SystemPara.Language],price/100,price%100);
							}	
							else
							{
								editMenu = 0;	
							}	
							break;
						case 'E':
							stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodao[huodaoNo - 1].price = price;
							LCDPrintf(8,5,0,SystemPara.Language,"%s%d",st_hefan_cabinet.huodaoPrice[SystemPara.Language],price);
							//save_hefan_huodao_info(get_hefan_Index(cabinetNo));
							editMenu = 0;
							break;
						default:
							break;	
					}
				}
				else if(editMenu == 2)//�༭��������
				{
					switch(key)
					{
						case '1':
							stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodao[huodaoNo - 1].state = HEFAN_HUODAO_NORMAL;
							editMenu = 0;
							break;
						case '2':
							stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodao[huodaoNo - 1].state = HEFAN_HUODAO_CLOSE;		
							editMenu = 0;
							break;
						case 'C':
							editMenu = 0;
							break;
						case 'E':
							editMenu = 0;
							//save_hefan_huodao_info(get_hefan_Index(cabinetNo));
							break;
					}
				}
				
				

				OSTimeDly(5);
				if(editMenu == 0)//ˢ��ҳ��
				{
					price = stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodao[huodaoNo - 1].price;
					openState = (stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodao[huodaoNo - 1].state == HEFAN_HUODAO_CLOSE) ? 0 : 1;
							
					LCDClrArea(1,4,238,14);
					LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanCabAndHuodaoManage[SystemPara.Language],cabinetNo,huodaoNo);
					LCDPrintf(8,5,0,SystemPara.Language,"%s%2d.%02d",st_hefan_cabinet.huodaoPrice[SystemPara.Language],price/100,price%100);					
					if(openState)
							stateStr = st_hefan_cabinet.open[SystemPara.Language];
					else
							stateStr = st_hefan_cabinet.close[SystemPara.Language];
					LCDPrintf(8,7,0,SystemPara.Language,"%s%s",st_hefan_cabinet.huodaoOpenState[SystemPara.Language],stateStr);
					LCDPrintf(8,9,0,SystemPara.Language,"%s%d",
						st_hefan_cabinet.huodaoTradeNum[SystemPara.Language],
						hefanGetParaVlaue(get_hefan_Index(cabinetNo),huodaoNo,HEFAN_HUODAO_TRADE_NUM));

		
				}
			}
			
			
		}
	}



}



/*****************************************************************************
** Function name:	hefanHuodaoTest	
** Descriptions:	�з����������ҳ��				
**														 			
** parameters:		
					
** Returned value:	
** 
*******************************************************************************/

void hefanHuodaoTest(unsigned char cabinetNo)
{
	unsigned char rst,huodaoNo,returnFlag = 0,key;
	LCDClrScreen();
	LCDDrawRectangle(0,0,239,15);
	LCDDrawRectangle(0,0,239,3);
	LCDClrArea(1,4,238,14);
	LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
	LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.InputHuodaoNo[SystemPara.Language]);

	huodaoNo = 0;
	while(1)
	{
		key = ReadKeyValue();
		switch(key)
		{
			case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case '0':
				huodaoNo = (huodaoNo * 10 + key - '0');
				
				LCDPrintf(8,5,0,SystemPara.Language,"%s%d",st_hefan_cabinet.InputHuodaoNo[SystemPara.Language],huodaoNo);
				break;
			case 'C':
				if(huodaoNo)
					huodaoNo = 0;
				else
					returnFlag = 1;
				LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.InputHuodaoNo[SystemPara.Language]);
				break;
			case 'E':				
				if(huodaoNo == 99)//ȫ��������
				{
					huodaoNo = 1;
					while(1)
					{
						key = ReadKeyValue();
						if(key == 'C')
						{
							huodaoNo = 0;
							break;
						}
						LCDClrArea(1,4,238,14);
						LCDDrawRectangle(0,0,239,15);
						LCDDrawRectangle(0,0,239,3);
						LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
						LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.huodaoTestting[SystemPara.Language],huodaoNo);
						
						rst = HeFanGuiHandle(huodaoNo,HEFANGUI_KAIMEN,get_hefan_Index(cabinetNo) - 1);
						LCDClearLineDraw(8,5,1);
						if(rst)
						{
							LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.huodaoOK[SystemPara.Language]);
						}
						else
							LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.huodaoFail[SystemPara.Language]);
						OSTimeDly(1000/5);
						if(huodaoNo < stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodaoNums)
							huodaoNo++;
						else
							huodaoNo = 1;
						
					}
				}
				else
				{
					if(huodaoNo <= stHefanCabinet[get_hefan_Index(cabinetNo) - 1].huodaoNums)
					{
						LCDClrArea(1,4,238,14);
						LCDDrawRectangle(0,0,239,15);
						LCDDrawRectangle(0,0,239,3);
						LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanCabAndHuodaoManage[SystemPara.Language],cabinetNo,huodaoNo);
						LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.huodaoTestting[SystemPara.Language],huodaoNo);

						rst = HeFanGuiHandle(huodaoNo,HEFANGUI_KAIMEN,get_hefan_Index(cabinetNo) - 1);
						LCDClearLineDraw(8,5,1);
						if(rst)
						{
							LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.huodaoOK[SystemPara.Language]);
						}
						else
							LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.huodaoFail[SystemPara.Language]);
						OSTimeDly(1000/5);
					}
				}	
				huodaoNo = 0;
				LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.InputHuodaoNo[SystemPara.Language]);
				break;

			default:
				break;
		}

		
		if(returnFlag)
			return;
		OSTimeDly(5);
	}
	
	
}




/*****************************************************************************
** Function name:	hefanCabinetControl	
** Descriptions:	�з������ҳ��				
**														 			
** parameters:		type 1 ���� 2 ���� 3����
					
** Returned value:	
** 
*******************************************************************************/

void hefanCabinetControl(unsigned char cabinetNo,unsigned char type)
{

	unsigned char key,flushFlag = 0,returnFlag  = 0,err;
	
	if(type == 1)//����
	{	
		if(getHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_LIGHT))
		{
			flushFlag = 1;
			while(1)
			{
				key = ReadKeyValue();
				switch(key)
				{
					case '1':
						err = HeFanGuiHandle(0,HEFANGUI_ZHAOMINGKAI,get_hefan_Index(cabinetNo) - 1);
						if(err == 1)
							setHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_LIGHT_OPEN,1);
			
						flushFlag = 1;
						break;
					case '2':
						err = HeFanGuiHandle(0,HEFANGUI_ZHAOMINGGUAN,get_hefan_Index(cabinetNo) - 1);	
						if(err == 1)
							setHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_LIGHT_OPEN,0);
						
						flushFlag = 1;
						break;
					case 'C':
						returnFlag = 1;
						break;
					case 'E':
						break;
					default:
						break;
				}

				OSTimeDly(5);
				if(flushFlag)
				{
					flushFlag = 0;	
					LCDClrScreen();
					LCDDrawRectangle(0,0,239,15);
					LCDDrawRectangle(0,0,239,3);
					LCDClrArea(1,4,238,14);
					LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
					
					if(getHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_LIGHT_OPEN))
						LCDPrintf(8,5,0,SystemPara.Language,"%s%s",st_hefan_cabinet.hefanlight[SystemPara.Language]
						,st_hefan_cabinet.open[SystemPara.Language]);
					else
						LCDPrintf(8,5,0,SystemPara.Language,"%s%s",st_hefan_cabinet.hefanlight[SystemPara.Language]
						,st_hefan_cabinet.close[SystemPara.Language]);
					
					LCDPrintf(8,9,0,SystemPara.Language,st_hefan_cabinet.indexOpen[SystemPara.Language]);
					LCDPrintf(8,11,0,SystemPara.Language,st_hefan_cabinet.indexClose[SystemPara.Language]);
					save_hefan_huodao_info(get_hefan_Index(cabinetNo));

					
				}

				if(returnFlag)
				{
					return;
				}
				
			}
		}
		else
		{
			LCDClrScreen();
			LCDDrawRectangle(0,0,239,15);
			LCDDrawRectangle(0,0,239,3);
			LCDClrArea(1,4,238,14);
			LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
			LCDPrintf(8,5,0,SystemPara.Language,"%s%s",
				st_hefan_cabinet.hefanlight[SystemPara.Language]
				,st_hefan_cabinet.noSurport[SystemPara.Language]);
			while(1)
			{
				key = ReadKeyValue();
				if(key == 'C')
					return;
				OSTimeDly(5);
			}
		}
		

	}
	else if(type == 2)//����
	{
		if(getHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_HOT))
		{
			flushFlag = 1;
			while(1)
			{
				key = ReadKeyValue();
				switch(key)
				{
					case '1':
						err = HeFanGuiHandle(0,HEFANGUI_JIAREKAI,get_hefan_Index(cabinetNo) - 1);
						if(err == 1)
							setHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_HOT_OPEN,1);
			
						flushFlag = 1;
						break;
					case '2':
						err = HeFanGuiHandle(0,HEFANGUI_JIAREGUAN,get_hefan_Index(cabinetNo) - 1);	
						if(err == 1)
							setHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_HOT_OPEN,0);
						
						flushFlag = 1;
						break;
					case 'C':
						returnFlag = 1;
						break;
					case 'E':
						break;
					default:
						break;
				}

				OSTimeDly(5);
				if(flushFlag)
				{
					flushFlag = 0;	
					LCDClrScreen();
					LCDDrawRectangle(0,0,239,15);
					LCDDrawRectangle(0,0,239,3);
					LCDClrArea(1,4,238,14);
					LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
					
					if(getHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_HOT_OPEN))
						LCDPrintf(8,5,0,SystemPara.Language,"%s%s",st_hefan_cabinet.hefanhot[SystemPara.Language]
						,st_hefan_cabinet.open[SystemPara.Language]);
					else
						LCDPrintf(8,5,0,SystemPara.Language,"%s%s",st_hefan_cabinet.hefanhot[SystemPara.Language]
						,st_hefan_cabinet.close[SystemPara.Language]);
					
					LCDPrintf(8,9,0,SystemPara.Language,st_hefan_cabinet.indexOpen[SystemPara.Language]);
					LCDPrintf(8,11,0,SystemPara.Language,st_hefan_cabinet.indexClose[SystemPara.Language]);
					save_hefan_huodao_info(get_hefan_Index(cabinetNo));

					
				}

				if(returnFlag)
				{
					return;
				}
				
			}
		}
		else
		{
			LCDClrScreen();
			LCDDrawRectangle(0,0,239,15);
			LCDDrawRectangle(0,0,239,3);
			LCDClrArea(1,4,238,14);
			LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
			LCDPrintf(8,5,0,SystemPara.Language,"%s%s",
				st_hefan_cabinet.hefanhot[SystemPara.Language]
				,st_hefan_cabinet.noSurport[SystemPara.Language]);
			while(1)
			{
				key = ReadKeyValue();
				if(key == 'C')
					return;
				OSTimeDly(5);
			}
		}
	}
	else if(type == 3)//����
	{
		if(getHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_COOL))
		{
			flushFlag = 1;
			while(1)
			{
				key = ReadKeyValue();
				switch(key)
				{
					case '1':
						err = HeFanGuiHandle(0,HEFANGUI_ZHILENGKAI,get_hefan_Index(cabinetNo) - 1);
						if(err == 1)
							setHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_COOL_OPEN,1);
			
						flushFlag = 1;
						break;
					case '2':
						err = HeFanGuiHandle(0,HEFANGUI_ZHILENGGUAN,get_hefan_Index(cabinetNo) - 1);	
						if(err == 1)
							setHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_COOL_OPEN,0);
						
						flushFlag = 1;
						break;
					case 'C':
						returnFlag = 1;
						break;
					case 'E':
						break;
					default:
						break;
				}

				OSTimeDly(5);
				if(flushFlag)
				{
					flushFlag = 0;	
					LCDClrScreen();
					LCDDrawRectangle(0,0,239,15);
					LCDDrawRectangle(0,0,239,3);
					LCDClrArea(1,4,238,14);
					LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
					
					if(getHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_COOL_OPEN))
						LCDPrintf(8,5,0,SystemPara.Language,"%s%s",st_hefan_cabinet.hefancool[SystemPara.Language]
						,st_hefan_cabinet.open[SystemPara.Language]);
					else
						LCDPrintf(8,5,0,SystemPara.Language,"%s%s",st_hefan_cabinet.hefancool[SystemPara.Language]
						,st_hefan_cabinet.close[SystemPara.Language]);
					
					LCDPrintf(8,9,0,SystemPara.Language,st_hefan_cabinet.indexOpen[SystemPara.Language]);
					LCDPrintf(8,11,0,SystemPara.Language,st_hefan_cabinet.indexClose[SystemPara.Language]);
					save_hefan_huodao_info(get_hefan_Index(cabinetNo));

					
				}

				if(returnFlag)
				{
					return;
				}
				
			}
		}
		else
		{
			LCDClrScreen();
			LCDDrawRectangle(0,0,239,15);
			LCDDrawRectangle(0,0,239,3);
			LCDClrArea(1,4,238,14);
			LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
			LCDPrintf(8,5,0,SystemPara.Language,"%s%s",
				st_hefan_cabinet.hefancool[SystemPara.Language]
				,st_hefan_cabinet.noSurport[SystemPara.Language]);
			while(1)
			{
				key = ReadKeyValue();
				if(key == 'C')
					return;
				OSTimeDly(5);
			}
		}
	}	
	
}


/*****************************************************************************
** Function name:	hefanControl	
** Descriptions:	�з������ҳ��				
**														 			
** parameters:		
					
** Returned value:	
** 
*******************************************************************************/

void hefanControl(unsigned char cabinetNo)
{

	unsigned char key,returnFlag = 0,flush = 0;
	flush = 1;
	while(1)
	{	
		key = ReadKeyValue();
		switch(key)
		{
			case '1':case '2':case '3':
				flush = 1;
				hefanCabinetControl(cabinetNo,key - '0');
				break;
			case 'C':
				returnFlag = 1;
				break;
			default:
				break;		
		}

		OSTimeDly(5);

		if(returnFlag)
		{
			return;
		}
		if(flush)
		{	
			
			LCDClrScreen();
			LCDDrawRectangle(0,0,239,15);
			LCDDrawRectangle(0,0,239,3);
			LCDClrArea(1,4,238,14);
			LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
			if(getHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_LIGHT))
			{
				if(getHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_LIGHT_OPEN))	
						LCDPrintf(8,5,0,SystemPara.Language,"%s%s",
						st_hefan_cabinet.hefanlight[SystemPara.Language],st_hefan_cabinet.open[SystemPara.Language]);
				else
					LCDPrintf(8,5,0,SystemPara.Language,"%s%s",
						st_hefan_cabinet.hefanlight[SystemPara.Language],st_hefan_cabinet.close[SystemPara.Language]);
				
			}
			else
				LCDPrintf(8,5,0,SystemPara.Language,"%s%s",
						st_hefan_cabinet.hefanlight[SystemPara.Language],st_hefan_cabinet.noSurport[SystemPara.Language]);
			
			if(getHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_HOT))
			{
				if(getHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_HOT_OPEN)) 
						LCDPrintf(8,7,0,SystemPara.Language,"%s%s",
						st_hefan_cabinet.hefanhot[SystemPara.Language],st_hefan_cabinet.open[SystemPara.Language]);
				else
					LCDPrintf(8,7,0,SystemPara.Language,"%s%s",
						st_hefan_cabinet.hefanhot[SystemPara.Language],st_hefan_cabinet.close[SystemPara.Language]);
				
			}
			else
				LCDPrintf(8,7,0,SystemPara.Language,"%s%s",
						st_hefan_cabinet.hefanhot[SystemPara.Language],st_hefan_cabinet.noSurport[SystemPara.Language]);
			if(getHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_COOL))
			{
				if(getHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_COOL_OPEN))	
						LCDPrintf(8,9,0,SystemPara.Language,"%s%s",
						st_hefan_cabinet.hefancool[SystemPara.Language],st_hefan_cabinet.open[SystemPara.Language]);
				else
					LCDPrintf(8,9,0,SystemPara.Language,"%s%s",
						st_hefan_cabinet.hefancool[SystemPara.Language],st_hefan_cabinet.close[SystemPara.Language]);	
			}
			else
				LCDPrintf(8,9,0,SystemPara.Language,"%s%s",
						st_hefan_cabinet.hefancool[SystemPara.Language],st_hefan_cabinet.noSurport[SystemPara.Language]);
			
			flush = 0;

			
		}
		
	}

}






/*****************************************************************************
** Function name:	hefanAddgoods	
** Descriptions:	�з������ҳ��			
**														 			
** parameters:		
					
** Returned value:	
** 
*******************************************************************************/

void hefanAddgoods(unsigned char cabinetNo)
{
	unsigned char key,returnFlag = 0,flush = 0,huodaoNo = 0,addRetrunFlag = 0;
	LCDClrScreen();
	LCDDrawRectangle(0,0,239,15);
	LCDDrawRectangle(0,0,239,3);
	LCDClrArea(1,4,238,14);
	LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
	LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.huodaoAddGoodsALl[SystemPara.Language],cabinetNo);
	LCDPrintf(8,7,0,SystemPara.Language,st_hefan_cabinet.huodaoAddGoodsOne[SystemPara.Language],cabinetNo);
	while(1)
	{
		key = ReadKeyValue();
		switch(key)
		{
			case '1'://ȫ�������
				flush = 1;
				LCDClrScreen();
				LCDDrawRectangle(0,0,239,15);
				LCDDrawRectangle(0,0,239,3);
				LCDClrArea(1,4,238,14);
				LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
				hefanAddgoodsHandle(get_hefan_Index(cabinetNo),0);
				LCDPrintf(20,7,0,SystemPara.Language,st_hefan_cabinet.huodaoAddGoodsOK[SystemPara.Language]);
				OSTimeDly(1000/5);
				break;
			case '2':
				LCDClrScreen();
				LCDDrawRectangle(0,0,239,15);
				LCDDrawRectangle(0,0,239,3);
				LCDClrArea(1,4,238,14);
				LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
				LCDPrintf(8,5,0,SystemPara.Language,"%s",st_hefan_cabinet.InputHuodaoNo[SystemPara.Language]);
				huodaoNo = 0;		
				while(1)
				{
					key = ReadKeyValue();
					switch(key)
					{
						case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case '0':				
						huodaoNo = huodaoNo * 10 + key - '0';
						LCDPrintf(8,5,0,SystemPara.Language,"%s%d",st_hefan_cabinet.InputHuodaoNo[SystemPara.Language],huodaoNo);
						break;

						case 'E':
							
							if(huodaoNo && hefanAddgoodsHandle(get_hefan_Index(cabinetNo),huodaoNo))	
								LCDPrintf(20,7,0,SystemPara.Language,st_hefan_cabinet.huodaoAddGoodsOK[SystemPara.Language]);
							else
								LCDPrintf(20,7,0,SystemPara.Language,st_hefan_cabinet.huodaoAddGoodsFail[SystemPara.Language]);
							
							OSTimeDly(800/5);
							huodaoNo = 0;
							addRetrunFlag = 1;
							break;
						case 'C':
							huodaoNo = 0;
							addRetrunFlag = 1;
							break;
						default:
							break;
					}
					
					OSTimeDly(5);

					if(addRetrunFlag)
					{
						addRetrunFlag = 0;
						break;
					}
						
				}
				flush = 1;
				break;
			case 'C':
				returnFlag = 1;
				break;
			default:
				break;
				
		}

		OSTimeDly(5);

		if(returnFlag)
		{
			return;
		}
		if(flush)
		{	
			LCDClrScreen();
			LCDDrawRectangle(0,0,239,15);
			LCDDrawRectangle(0,0,239,3);
			LCDClrArea(1,4,238,14);
			LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
			LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.huodaoAddGoodsALl[SystemPara.Language],cabinetNo);
			LCDPrintf(8,7,0,SystemPara.Language,st_hefan_cabinet.huodaoAddGoodsOne[SystemPara.Language],cabinetNo);
			flush = 0;
		}
		
	}
}


void hefanInitPage(unsigned char cabinetNo)
{
	unsigned char err,key;
	LCDClrScreen();
	LCDDrawRectangle(0,0,239,15);
	LCDDrawRectangle(0,0,239,3);
	LCDClrArea(1,4,238,14);
	LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
	LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.cabinetInit[SystemPara.Language]);		
	LCDPrintf(8,7,0,SystemPara.Language,st_hefan_cabinet.cabinetConfirm[SystemPara.Language]);
	LCDPrintf(8,9,0,SystemPara.Language,st_hefan_cabinet.cabinetCancel[SystemPara.Language]);
		
	while(1)
	{
		key= ReadKeyValue();
		if(key == '2' || key == 'C')
			return;
		else if(key == '1' || key == 'E')
		{
			LCDClrScreen();
			LCDDrawRectangle(0,0,239,15);
			LCDDrawRectangle(0,0,239,3);
			LCDClrArea(1,4,238,14);
			LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
			LCDPrintf(8,7,0,SystemPara.Language,st_hefan_cabinet.cabinetInitting[SystemPara.Language]);
			OSTimeDly(500/5);
			err = hefanHuodaoInit(get_hefan_Index(cabinetNo));
			LCDClearLineDraw(8,7,1);
			if(err == 1)
				LCDPrintf(8,7,0,SystemPara.Language,st_hefan_cabinet.cabinetInitOK[SystemPara.Language]);
			else
				LCDPrintf(8,7,0,SystemPara.Language,st_hefan_cabinet.cabinetInitFail[SystemPara.Language]);
			OSTimeDly(1000/5);
			return;
		}
		OSTimeDly(5);
	}

	
}

/*****************************************************************************
** Function name:	hefanMannage	
** Descriptions:	�з��������				
**														 			
** parameters:		
					
** Returned value:	
** 
*******************************************************************************/

void hefanMannage(void )
{
	unsigned char  key =0xff,returnFlag  = 0,enterSub = 0,err,topFlush = 1;
	unsigned char cabinetNo = 0,subFlush = 1;
	while(1)
	{
		if(topFlush)
		{
			//��ѡ��з�����
			LCDClrScreen();
			LCDDrawRectangle(0,0,239,15);
			LCDDrawRectangle(0,0,239,3);
			LCDClrArea(1,4,238,14);
			LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanManage[SystemPara.Language]);
			LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.InputCabinetNo[SystemPara.Language]);
			topFlush = 0;
		}
		key = ReadKeyValue();
		switch(key)
		{
			case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case '0':				
				cabinetNo = cabinetNo * 10 + key - '0';
				LCDPrintf(8,5,0,SystemPara.Language,"%s%d",st_hefan_cabinet.InputCabinetNo[SystemPara.Language],cabinetNo);
				break;
			case 'C':
				if(cabinetNo)
				{
					cabinetNo = 0;
					topFlush = 1;
				}	
				else
					returnFlag = 1;
				
				break;
			case 'E':
				err = 0;
				if((cabinetNo > 2) && cabinetNo <= (HE_FAN_MAX_CABINET + 2))
				{
					//��ѯ�˹�ĸ����� ������ֵ
					err = HeFanGuiHandle(0,HEFANGUI_CHAXUN,get_hefan_Index(cabinetNo) - 1);
				}
				if(err == 1)
				{
					subFlush = 1;
					enterSub = 1;
				}	
				else
				{
					LCDPrintf(8,9,0,SystemPara.Language,st_hefan_cabinet.cabinetNonExist[SystemPara.Language]);
					OSTimeDly(1000/5);
					if(get_hefan_Index(cabinetNo))
						setHefanCabinetState(get_hefan_Index(cabinetNo),HEFAN_EXIST,0);
					cabinetNo = 0;
					topFlush = 1;
				}	
				break;
			default:
				break;
		}
		OSTimeDly(5);
		
		if(returnFlag)
		{
			save_hefan_huodao_by_flash();
			return;
		}
			

		while(enterSub)
		{
			if(subFlush)
			{
				subFlush = 0;
				LCDClrScreen();
				LCDDrawRectangle(0,0,239,15);
				LCDDrawRectangle(0,0,239,3);
				LCDClrArea(1,4,238,14);
				LCDPrintf(8,1,0,SystemPara.Language,st_hefan_cabinet.hefanNOManage[SystemPara.Language],cabinetNo);
				LCDPrintf(8,5,0,SystemPara.Language,st_hefan_cabinet.huodaoInfo[SystemPara.Language]);
				LCDPrintf(8,7,0,SystemPara.Language,st_hefan_cabinet.huodaoTest[SystemPara.Language]);
				LCDPrintf(8,9,0,SystemPara.Language,st_hefan_cabinet.hefanControl[SystemPara.Language]);
				LCDPrintf(8,11,0,SystemPara.Language,st_hefan_cabinet.huodaoAddGoods[SystemPara.Language]);
				LCDPrintf(8,13,0,SystemPara.Language,st_hefan_cabinet.cabinetInit[SystemPara.Language]);		
			}
			
			key = ReadKeyValue();
			switch(key)
			{
				case 'C':
					topFlush = 1;
					enterSub  = 0;
					cabinetNo = 0;
					break;
				case '1':
					subFlush = 1;
					hefanHuodaoInfo(cabinetNo);
					break;
				case '2':
					subFlush = 1;
					hefanHuodaoTest(cabinetNo);
					break;

				case '3'://���ڿ���
					subFlush = 1;
					hefanControl(cabinetNo);
					break;
				case '4'://�������
					subFlush = 1;
					hefanAddgoods(cabinetNo);
					break;
				case '5'://������ʼ��
					subFlush = 1;
					hefanInitPage(cabinetNo);
					print_hefan("hefanInit over....\r\n");
					break;
				default:
					break;
			}		
			OSTimeDly(5);
		}
			
	}


}


