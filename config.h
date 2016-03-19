/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           config.h
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        ͷ�ļ����ã�λ�ڹ�����Ŀ¼                     
**------------------------------------------------------------------------------------------------------
** Created by:          chensunwei
** Created date:        2013-01-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#ifndef __CONFIG_H 
#define __CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************
  ͨ�ÿ����ͷ�ļ�
*********************************************************************************************************/
#include <stdint.h> 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <setjmp.h>
#include <rt_misc.h>
#include <stdarg.h>
/*********************************************************************************************************
  �̼������ͷ�ļ�
*********************************************************************************************************/
#include ".\firmware\firmware.h"
#include ".\firmware\zy_if.h"
#include ".\firmware\LPC1700.h"
#include ".\firmware\target_cfg.h"
#include ".\firmware\LPC1700PinCfg.h"
/*********************************************************************************************************
  ���������ײ�ͷ�ļ�
*********************************************************************************************************/
#include ".\Drivers\uart0.h"
#include ".\Drivers\uart1.h"
#include ".\Drivers\UART2.h"
#include ".\Drivers\uart3.h" 
#include ".\Drivers\buzzer.h"
#include ".\Drivers\lcd.h"
#include ".\Drivers\RTC.H"
#include ".\Drivers\i2c.h"
#include ".\Drivers\timer.h"
#include ".\Drivers\AT45DB161D.H"
#include ".\Drivers\WDTDog.h"
/*********************************************************************************************************
  ���������߲��ͷ�ļ�-����2����
*********************************************************************************************************/
#include ".\Drivers\MDB_UART2.h"
#include ".\Drivers\EVB_UART2.h"
//#include ".\Drivers\BillAccepter.h"
//#include ".\Drivers\CoinAccepter.h"
//#include ".\Drivers\ChangerAccepter.h"
//#include ".\Drivers\MDBReader.h"
#include ".\Drivers\HopperDriver.h"
#include ".\Drivers\pcoin.h" //����Ӳ����
#include ".\Drivers\key.h"//����
#include ".\Drivers\ChannelDriver.h"//����������
#include ".\Drivers\TASKDEVICE.h"//����1�߲�����
#include ".\Drivers\ACDCDriver.h"
#include ".\Drivers\LiftTableDriver.h"
#include ".\Drivers\GPRS_UART.h"
/*********************************************************************************************************
  ���������߲��ͷ�ļ�-����3����
*********************************************************************************************************/
//#include ".\Uart3Driver\ZHIHUIPC.H" //changed by yoc 2013.12.16
#include ".\Uart3Driver\TASKUART3DEVICE.H"
/*********************************************************************************************************
  ���������߲��ͷ�ļ�-����1����
*********************************************************************************************************/
//#include ".\Uart1Driver\SelectKey.H"
#include ".\Uart1Driver\TASKUART1DEVICE.H"
#include ".\Uart1Driver\Xmt.h"
#include ".\Uart1Driver\HeFanGui.h"
#include ".\Uart1Driver\FS_billChanger.h"

/*********************************************************************************************************
  ���������߲��ͷ�ļ�-����0����  by yoc newAisle
*********************************************************************************************************/
#include ".\Uart0Driver\taskUart0Device.h"


/*********************************************************************************************************
****************************** /\ ********************************************************
****************************** || ********************************************************
****************************[MAILBOX]*******************************************************
****************************** || ********************************************************
****************************** \/ ********************************************************
**********************************************************************************************************/
/*********************************************************************************************************
  �м��ͷ�ļ�
*********************************************************************************************************/
#include ".\API\BillAccepterAPI.h"
#include ".\API\CoinAccepterAPI.h"
#include ".\API\ChangerAPI.h"
#include ".\API\ReaderAPI.h"
#include ".\API\TuibiAPI.h"
#include ".\API\CheckDeviceState.h"
#include ".\API\ChannelAPI.h"
#include ".\API\ACDCAPI.h"
#include ".\API\LogAPI.h"
#include ".\API\TraceAPI.h"
#include ".\API\PCComAPI.h"
#include ".\API\LiftTableAPI.h"
#include ".\API\SelectKeyAPI.h"
#include ".\API\HeFanGuiAPI.h"
#include ".\API\colApi.h"
#include ".\API\ZhiHuiAPI.h" //add by yoc 2013.12.16
#include ".\API\GprsAPI.h"
/*********************************************************************************************************
  Ӧ�ò�ͷ�ļ�
*********************************************************************************************************/
#include ".\APP\common.h"
#include ".\APP\LANGUAGE.h"
#include ".\APP\MAINTAIN.H"
#include ".\APP\BUSINESS.H"

#ifdef __cplusplus
	}
#endif 

#endif

/**************************************End Of File*******************************************************/
