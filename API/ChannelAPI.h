#ifndef _CHANNELACCEPTERAPI_H
#define _CHANNELACCEPTERAPI_H

#include "..\Uart3Driver\UBProtocol.h"


//========================================================================
//������������ ��ṹ��
//by yoc  2014.04.01
//=============================================================================

#define PRINT_HUODAO_ENABLE  0
#if PRINT_HUODAO_ENABLE == 1
#define  print_hd(...)       Trace(__VA_ARGS__)
#else
#define  print_hd(...)       do {} while(0)
#endif

#define GOODS_SOLD_ERR			(1 << 0)   //bit0�ܹ���λ
#define GOODS_SOLDOUT_BIT		(1 << 1)   //bit1�������
#define MOTO_MISPLACE_BIT		(1 << 2)   //bit2�����ת֮ǰ�Ͳ�����ȷ��λ����(Ҳ��������)
#define MOTO_NOTMOVE_BIT		(1 << 3)   //bit3�������ת(Ҳ��������)
#define MOTO_NOTRETURN_BIT		(1 << 4)   //bit4���ûת����ȷλ��(Ҳ��������)
#define GOODS_NOTPASS_BIT		(1 << 5)   //bit5��Ʒû��(����ȷ��û��⵽)
#define DRV_CMDERR_BIT			(1 << 6)   //bit6�������(ֻ�з�������Ͳ�ѯ������������������������������ͱ���)
#define DRV_GOCERR_BIT			(1 << 7)   //bit7�������ģ��״̬(GOC����)


//�������д��FLASHҳ�ź�
#define HD_CABINET1_FLASH_1    4000
#define HD_CABINET1_FLASH_2    4002
#define HD_CABINET1_FLASH_3    4004
#define HD_CABINET1_FLASH_4    4006

#define HD_CABINET2_FLASH_1    4008
#define HD_CABINET2_FLASH_2    4010
#define HD_CABINET2_FLASH_3    4012
#define HD_CABINET2_FLASH_4    4014

//�������д��FLASH����ҳ�ź�
#define HD_CABINET1_FLASH_1_BAK    4020
#define HD_CABINET1_FLASH_2_BAK    4022
#define HD_CABINET1_FLASH_3_BAK    4024
#define HD_CABINET1_FLASH_4_BAK    4026

#define HD_CABINET2_FLASH_1_BAK    4028
#define HD_CABINET2_FLASH_2_BAK    4030
#define HD_CABINET2_FLASH_3_BAK    4032
#define HD_CABINET2_FLASH_4_BAK    4034





	  	  
//���������Ϣ ��д����
#define HUODAO_TYPE_NOT					(0)//�ղ���
#define HUODAO_TYPE_PRICE				(1) //1������������
#define HUODAO_TYPE_COUNT				(2)//2������������
#define HUODAO_TYPE_STATE				(3)// 3��������״̬
#define HUODAO_TYPE_MAX_COUNT			(4)// 4���������������
#define HUODAO_TYPE_SELECTNO			(5)//5����ѡ������ֵ
#define HUODAO_TYPE_SUC_COUNT			(6)// 6�����ɹ����״���
#define HUODAO_TYPE_ID					(7)// 7������Ʒ��� 
#define HUODAO_TYPE_PHYSICNO			(8)//8����������  Ŀǰ������
#define HUODAO_TYPE_OPEN			    (9)//9������������״̬



//����8 *10���� ��
#define HUODAO_MAX_COLUMN		10//������������
#define HUODAO_MAX_ROW			8//�ۻ���������


#define HUODAO_STATE_NORMAL		1//����
#define HUODAO_STATE_FAULT      2//����
#define HUODAO_STATE_EMPTY      3//ȱ��
#define HUODAO_STATE_PC_CLOSE   4//�ݲ�����
#define HUODAO_STATE_NOT      	0//δ֪

//�»������
typedef struct _st_huodao_{

	unsigned char physicNo;//���������Ź̶����� ʮ���ƹ̶�8 * 10
	unsigned char physicAddr;//���������ַ��������������ʵ�Ľ�����ַ
	unsigned char logicNo;	//�߼�������Ÿ��ݻ���������־���� ʮ����
	unsigned char openFlag;//����������ر�־ 0�����ر� 1��������

	unsigned char  id;//��Ʒ���
	unsigned char  selectNo;//��Ӧѡ������
	unsigned char  state;//����״̬��1:����;2:����;3:�޻�;4:PC��λ������	
	unsigned char  count;//������Ʒ����
	unsigned char  maxCount;//�û������������
	unsigned int   price;//������Ʒ����
	unsigned int   sucCount;//�ɹ����״���
		
}ST_HUODAO;

//���Ͳ�������
typedef struct _st_level_huodao_{

	unsigned char openFlag;//�㿪����־0�ر�  1����
	ST_HUODAO     huodao[HUODAO_MAX_COLUMN];//ÿ��ʮ������

}ST_LEVEL_HUODAO;





//�������񸱹�����ṹ��
extern ST_LEVEL_HUODAO stHuodao[HUODAO_MAX_ROW];
extern ST_LEVEL_HUODAO stSubHuodao[HUODAO_MAX_ROW];


typedef struct _st_huodao_kaoji_{
	unsigned short times;//�ܿ�������
	unsigned short faultTimes[8][10]; //���ϴ��� 0��ʾ����

}ST_HUODAO_KAOJI;

extern ST_HUODAO_KAOJI stHuodaoKaoji[2];


unsigned char hd_add_goods(unsigned char cabinetNo,unsigned char no,unsigned type);
unsigned char hd_get_index(unsigned char cabinetNo,unsigned char no,unsigned char type,
		unsigned char *rIndex,unsigned char *cIndex);
unsigned char hd_all_check(void);
void hd_init(unsigned char cabinetNo);
unsigned char hd_set_by_logic(unsigned char cabinetNo,unsigned char logicNo,unsigned char type,unsigned int value);
unsigned int hd_get_by_logic(unsigned char cabinetNo,unsigned char logicNo,unsigned char type);
unsigned char hd_set_by_physic(unsigned char cabinetNo,unsigned char physicNo,unsigned char type,unsigned int value);
unsigned int hd_get_by_physic(unsigned char cabinetNo,unsigned char physicNo,unsigned char type);
unsigned char hd_get_all_physicNo(unsigned char cabinetNo,unsigned char *buf);
unsigned char hd_huodao_rpt_vp(unsigned char cabinetNo,unsigned char *buf);
unsigned char hd_huodao_id_rpt_vp(unsigned char cabinetNo,unsigned char *buf);

unsigned char hd_set_by_pc(unsigned char cabinetNo,unsigned char len,void * data,unsigned char type);
unsigned char hd_physicNo_by_logicNo(unsigned char cabinetNo,unsigned char logicNo);
unsigned char hd_save_param_bak(unsigned char cabinetNo);
unsigned char hd_read_param_bak(unsigned char cabinetNo);
unsigned char hd_save_param(unsigned char cabinetNo);
unsigned char hd_read_param(unsigned char cabinetNo);
unsigned char hd_id_by_logic(unsigned char cabinetNo,unsigned char logic);
unsigned char hd_nums_by_id(unsigned char cabinetNo);
unsigned char hd_state_by_id(unsigned char cabinetNo,unsigned char id);
unsigned int hd_get_by_id(unsigned char cabinetNo,unsigned char id,unsigned char type);
unsigned char hd_setNums_by_id(unsigned char cabinetNo,unsigned char id,unsigned char value);
unsigned char hd_ids_by_level(unsigned char cabinetNo,unsigned char level,unsigned char *startId,unsigned char *stopId);





//========================================================================
//�ڶ������������ĺ� ��ṹ��
//by yoc  2014.04.01
//=============================================================================

#define OPEN			(1)
#define CLOSE			(2)
#define COLUMNHANDLE	(1)
#define LEVELHANDLE		(2)

#define CHANNELPRICE				(11)
#define CHANNELCOUNT				(12)
#define CHANNELSTATE				(13)
#define CHANNELMAXCOUNT				(14)
#define CHANNELSELECTNUM			(15)
#define CHANNELSUCCESSCOUNT			(16)
#define CHANNELGOODSNUM				(17)
#define CHANNELPHYSICCLEAR			(18)


void ChannelResetinit(void);
void ChannelGetFlashData(void);
void ChannelExChannge(unsigned char logicnum,unsigned char LevelOrChnltype,unsigned char Handletype,unsigned char Binnum);
unsigned char ChannelGetLevelNum(unsigned char Binnum,unsigned char LogicNum);
unsigned char ChannelAPIHandle(unsigned char LogicNum,unsigned char HandleType,unsigned char GocType,unsigned char Binnum);
unsigned char ChannelGetSelectColumn(unsigned char Bin,unsigned char type,unsigned short SelectNum);
unsigned char ChannelSetParam(unsigned char Logicnum,unsigned char Binnum,unsigned char HandleType,unsigned short Value,unsigned int Price);
void ChannelSaveParam(void);
unsigned char ChannelGetCloseCount(unsigned char Binnum,unsigned char Type,unsigned char Level);
unsigned char ChannelGetPhysicNum(unsigned char Logicnum,unsigned char Binnum);
unsigned int ChannelCheckIsOk(unsigned char Logicnum,unsigned char Binnum);
unsigned int ChannelGetParamValue(unsigned char logicnum,unsigned char type,unsigned char Binnum);
unsigned char ChannelGetAddressLOWandRow(unsigned char Type,unsigned char Binnum,unsigned char Address,unsigned char *Row,unsigned char *Low);
unsigned char GetLevelOrColumnState(unsigned char Bin,unsigned char Type,unsigned char Num);
unsigned int GetColumnStateByPhysic(unsigned char Bin,unsigned char PhysicNum,unsigned char Type);
unsigned int SetColumnParamByPhysic(unsigned char Bin,unsigned char PhysicNum,unsigned char Type,unsigned int Value);
unsigned char AddColumnGoods(unsigned char Bin,unsigned Type,unsigned char LogicNum);
unsigned char FromPhysicGetLogicNum(unsigned char Bin,unsigned char PhysicNum);
unsigned char TestColumnPost(unsigned char HandleType,unsigned char ChkFlag,unsigned char ChannelNum,unsigned char Binnum);
unsigned char TestColumnPend(unsigned char ChkFlag);
unsigned char GetPhysicIndexByRowAndLow(unsigned char type,unsigned char Bin,unsigned char row,unsigned char low);
unsigned char pcSetHuodao(unsigned char setNums,struct GoodsAttribute *stData);
unsigned char ChannelGetAllOpenColumn(unsigned char Bin,unsigned char *IndexBuf);
unsigned char nt_get_level_state(unsigned char cabinetNo,unsigned char levelNo);
unsigned char nt_get_huodoa_ON_OFF(unsigned char cabinetNo,unsigned char huodaoNo);
unsigned char nt_huodao_exsit(unsigned char cabinetNo,unsigned char logicNo);
unsigned char special_cabinet_info(unsigned char *data);
unsigned char nt_huodao_exsit_by_physic(unsigned char cabinetNo,unsigned char physicNo);
void ChannelGetPCChange(unsigned char cabinetNo,unsigned char type,unsigned char *valuebuf);
#endif


