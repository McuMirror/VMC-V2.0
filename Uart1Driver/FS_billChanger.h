#ifndef _FS_BILL_CHANGER_H_
#define _FS_BILL_CHANGER_H_


typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef char int8;
typedef short int16;
typedef int int32;

#define FS_TIMER()	do{if(fs_timer){fs_timer--;}if(fs_reqTimer){fs_reqTimer--;}if(fs_taskTimer){fs_taskTimer--;}}while(0)





typedef struct{
	uint8 status;//״̬ 0���� 1��ʼ��ʧ��  2ͨ�Ź���  �������� ��ϸ������� ��һ�ֽ� errCode 
	uint16 errCode; // Ϊ������
	uint8 box[4]; // 0���� 1 ȱ��  2���䱻����  3����
}FS_STATUS;





extern volatile unsigned int fs_timer ;
extern volatile unsigned int fs_reqTimer ;
extern volatile unsigned int fs_taskTimer;


uint32 FS_dispense(uint32 payAmount);  //���� ����Ϊ���  ���� ��������  ��Ϊ��λ
FS_STATUS *FS_getStatus(void);	 //��ȡ״̬ 



extern unsigned char FS_poll(void);
extern void FS_init(void);
void FS_mainTask(void);
#endif
