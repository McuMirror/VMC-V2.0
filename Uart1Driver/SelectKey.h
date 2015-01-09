/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           SelectKey.h
** Last modified Date:  2013-03-04
** Last Version:         
** Descriptions:        LCD����������غ�������                     
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



#ifndef _SELECTKEY_H

typedef struct	
{
	//ѡ������;by gzz 20121025
	uint8_t  sel1ReadyLed;               //������1�ġ̵�
	uint8_t  sel1ErrLed;                 //������1�ġ���
	uint8_t  sel2ReadyLed;               //������2�ġ̵�
	uint8_t  sel2ErrLed;                 //������2�ġ���
	uint8_t  sel3ReadyLed;               //������3�ġ̵�
	uint8_t  sel3ErrLed;                 //������3�ġ���
	uint8_t  sel4ReadyLed;               //������4�ġ̵�
	uint8_t  sel4ErrLed;                 //������4�ġ���
	uint8_t  sel5ReadyLed;               //������5�ġ̵�
	uint8_t  sel5ErrLed;                 //������5�ġ���
	uint8_t  sel6ReadyLed;               //������6�ġ̵�
	uint8_t  sel6ErrLed;                 //������6�ġ���
	uint8_t  selectMode;                 //ģʽ,0����״̬,1����״̬
}SELECTKEY;
extern SELECTKEY selKey;

uint8_t GetSelectKey(void);
void SelectKey_InitProc(void);
void UpdateSelectionLed(MessageKEYPack *AccepterMsg);


#define _SELECTKEY_H

#endif
