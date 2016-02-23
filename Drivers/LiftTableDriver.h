#ifndef _LIFTTABLEDRIVER_H
#define _LIFTTABLEDRIVER_H

//VMC֪ͨGCC��λ��ʼ��
#define VMC_RESET_REQ				(0x80)
//VMC֪ͨGCC����GCC״̬
#define VMC_STATUS_REQ				(0x81)
//VMC֪ͨGCC����
#define VMC_VENDING_REQ				(0x82)
//VMC֪ͨGCC����������
#define VMC_VENDINGRESULT_REQ		(0x83)
//VMC֪ͨGCC����
#define VMC_OPENDOOR_REQ			(0x84)
//VMC֪ͨGCC����
#define VMC_CLOSEDOOR_REQ			(0x85)
//VMC֪ͨGCC��������
#define VMC_OPENLIGHT_REQ			(0x86)
//VMC֪ͨGCC�ر�������
#define VMC_CLOSELIGHT_REQ			(0x87)
//VMC֪ͨGCC�������ò���
#define VMC_GETINFO_REQ				(0x88)

#define VMC_CHUCHOU_REQ				(0x8A)

#define GCC_STATUS_ACK				(0x01)
//GCC֪ͨVMC����
#define GCC_VENDING_ACK				(0x02)
//GCC֪ͨVMC����������
#define GCC_VENDINGRESULT_ACK		(0x03)
//GCC֪ͨVMC����
#define GCC_OPENDOOR_ACK			(0x04)
//GCC֪ͨVMC����
#define GCC_CLOSEDOOR_ACK			(0x05)
//GCC֪ͨVMC��������
#define GCC_OPENLIGHT_ACK			(0x06)
//GCC֪ͨVMC�ر�������
#define GCC_CLOSELIGHT_ACK			(0x07)
//GCC֪ͨVMC�������ò���
#define GCC_GETINFO_ACK				(0x08)

#define GCC_CHUCHOU_ACK				(0x0A)

#define GCC_RESET_ACK               (0x00)


typedef struct 
{
	//��������
	unsigned char Magic;
	//ȡ�����
	unsigned char Error_FMD;
	//���µ��
	unsigned char Error_FUM;
	//����ȷ�϶����
	unsigned char Error_GOC;
	//��ܼ�⴫����
	unsigned char Error_FLD;
	//��ʼλ�ô�����
	unsigned char Error_FOD;
	//�û�ȡ���Ź���
	unsigned char Error_UDS;
	//���������
	unsigned char Error_GCD;
	//��������æ
	unsigned char Error_SOVB;
	//�������Ź���
	unsigned char Error_SOMD1;
	//�յ��������ݰ�
	unsigned char Error_SOEC;
	//����̨��ȷȡ��
	unsigned char Error_SFHG;
	//�������Ƿ��
	unsigned char Error_SOFP;
	//ȡ����
	unsigned char Error_SOMD2;
	//����״̬ 0������   1������
	unsigned char Error_OVERALLUINT;
	//������״̬ 
	unsigned char Error_LIFTER;
	//ȡ�����״̬
	unsigned char Error_TAKEMOTTOR;
	//�û��綯��״̬
	unsigned char Error_USERDOOR;
	//���һ���״̬
	unsigned char Error_DISPENCE;
	//����ģ��״̬
	unsigned char Error_FANGDAO;
	//����������״̬
	unsigned char Error_COLUMN;
	//�ۻ��������豸����
	//�Ƿ�֧���ƻ�	0:��֧�� 1��֧��
	unsigned char SupportSpring;
	//�Ƿ�֧��������
	unsigned char Light;
	//�Ƿ�֧������
	unsigned char Cold;
	//�Ƿ�֧�ּ���
	unsigned char Hot;
	//�Ƿ�֧������
	unsigned char Lifter;
	//�Ƿ�֧��ƽ��
	unsigned char Flat;
	//�Ƿ�֧���Ĵ�
	unsigned char ApronWheel;
}STLIFTTABLE;

extern STLIFTTABLE stLiftTable;


unsigned short CrcCheck(unsigned char *msg, unsigned short len) ;
unsigned char LiftTableDriver(unsigned char Binnum,unsigned char Cmd,unsigned char Add,unsigned char Level,unsigned char *Result);

unsigned char LIFT_vmcStatusReq(unsigned char bin);
#endif
