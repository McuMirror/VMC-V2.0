#ifndef _CHANGERACCEPTER_H

#define CHANGERDEV_HOPPER	1				//��ǰϵͳʶ�𵽵�Ӳ��������ΪHopper
#define CHANGERDEV_MDB		2				//��ǰϵͳʶ�𵽵�Ӳ��������ΪMDB



extern unsigned char ChangePayoutProcessLevel3(uint32_t PayMoney,unsigned char PayoutNum[16]);
extern unsigned char ChangePayoutProcessLevel2(unsigned char PayType,unsigned char PayNum, unsigned char PayoutNum[8]);
extern void ChangeGetTubes(void);

#endif
