#ifndef _CHANNELACCEPTER_H
#define _CHANNELACCEPTER_H


//��������
#define CHANNEL_OUTGOODS		(1)
//��������ǰ״̬			
#define CHANNEL_CHECKSTATE		(2)
//�������ϴεĳ������			
#define CHANNEL_CHECKOUTRESULT	(3)	
//���sn
#define CHANNEL_CLEARSN			(4)


//����
#define MACHINE_RESET			(5)
//����������
#define MACHINE_LIGHTOPEN		(6)
//�ر�������
#define MACHINE_LIGHTCLOSE		(7)
//����
#define MACHINE_OPENDOOR		(8)
//����
#define MACHINE_CLOSEDOOR		(9)
//��û�������
#define MACHINE_INFORMATION		(10)	
	
//����
#define OUTGOODSCMD				(0x70)
//��ѯ��ǰ״̬			
#define CHECKCHANNEL			(0x71)
//���SN
#define CLEARSN					(0x72)
//��ѯ�������				
#define CHECKOUTRESULT			(0x73)	


struct ChannelErrForm
{
	//����ȷ���豸����״̬λ
	unsigned char GOCErrFlag[2];
	//���������״̬λ
	unsigned char HuoDaoBanFlag[2];
};
//���������б�
extern struct ChannelErrForm stChannelErrForm;		


unsigned char ChannelHandle(unsigned char Handletype,unsigned char Add,unsigned char CheckFlag,unsigned char Binnum);





#endif
