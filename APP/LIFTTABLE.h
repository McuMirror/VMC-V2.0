#ifndef _LIFTTABLE_H
#define _LIFTTABLE_H

//��������
#define LIFTTABLE_OUTHANDLE				(1)
//��ѯ��ǰ״̬
#define LIFTTABLE_CHECKHANDLE			(2)
//���SN
#define LIFTTABLE_OUTGOODSRST			(3)
//��ѯ�����Ƿ�ȡ��
#define LIFTTABLE_TAKEGOODS				(4)



void LiftTableTest(unsigned char binnum);

unsigned char LiftTable1Process(unsigned char Binnum,unsigned char ChannelNum,unsigned char HandleType);
unsigned char LiftTable2Process(unsigned char Binnum,const unsigned char ChannelNum,unsigned char HandleType);

unsigned char LiftTableProcess(unsigned char Binnum,unsigned char ChannelNum,unsigned char HandleType);
unsigned char LiftTableTestByPcPost(unsigned char Bin,unsigned char LogicNum,unsigned char HandleType);
unsigned char LiftTableTestByPcPend(unsigned char HandleType);
#endif
