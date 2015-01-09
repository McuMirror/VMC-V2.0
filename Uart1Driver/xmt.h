#ifndef _XMT_H
#define _XMT_H


#define ITL_ERR_NULL     0
#define ITL_ERR_PAR      1   //mission parameter error
#define ITL_ERR_COM      2   //communication error

#define ITL_MSG_STX     0x7F
#define ITL_MSG_SEQ_0   0x00
#define ITL_MSG_SEQ_1   0x80

#define ITL_TIME_POLL     20     //30, 15,  10,  20(2011.4.13), 10, 20
#define ITL_TIME_OUT      100    //50, 100, 150, 1000, 100   
#define ITL_TIME_OUT_NUM  3      //1,3 
#define ITL_TIME_CMD_INTEVEL     100   //800-100
#define ITL_TIME_CMD_INTEVEL_1   20    //200-20
#define ITL_TIME_STACK    15     //3, 8(2011.4.26: wait more time for NV9), 15
#define ITL_TIME_REJECT   3
#define ITL_COM_RETRY     0      //0,1
#define ITL_COM_DELAY     1      //10,1
#define ITL_TIME_RESTART  5    //111227 MDBЭ��ITL����6min to 5s//6 minutes or more time



#define SET_TEMPERATURE     0 //֪ͨ�����¿����¶ȵ�������Ϣ
#define GET_TEMPERATURE     1 //֪ͨ�õ��¿����¶ȵ�������Ϣ
#define RETURN_TEMPERATURE  2 //֪ͨ�¿����¶��¶��������




//XMT�¿�;by gzz 20110105
#define XMT_CTR_DEVADDR     0x01//�豸��ַ
#define XMT_CTR_ADDR        (0x80+XMT_CTR_DEVADDR)//�豸ͷ
#define XMT_CTR_READTEMP    0x52//���������
#define XMT_CTR_SETTEMP     0x43//д�������
#define XMT_CTR_CMDSETTMP   0x00//������������:��д�¶�
#define XMT_CTR_CMDSETHY    0x05//������������:��д�¶ȿ���ƫ�ƾ���
#define XMT_CTR_GETTEMPOPT  1//���¶�
#define XMT_CTR_SETTEMPOPT  0//�趨�¶�
#define XMT_GET_TEMPTIME    300
#define XMT_TIME_GETTEMPERATURE 1200//����¶�




struct XMT_Mission
{ 
    uint8_t Addr;//�豸��ַ
    uint8_t cmd;//���������:52H��д����:43H
    uint8_t cmdSet;//������������
    uint8_t cmdPar[2];//�趨��ֵ
    uint8_t crcEnd;//crcУ��ֵ
    uint8_t recPV[2];
    uint8_t recSV[2];
	uint8_t recHY[2];
    uint16_t  recPVTemp;//������������
	uint16_t  recSVTemp;//�����趨���¶�
	uint16_t  recHYTemp;//�����趨���¶ȿ���ƫ�ƾ���
	uint16_t  temperaTimer;//�����趨���¶�
	uint8_t recBuf[30];	
	uint8_t rectemp;
	uint16_t GetTempeTime;//����¶�
	uint8_t IsValid;
	uint8_t msTimer;
};
//extern struct XMT_Mission xdata sysXMTMission;
extern struct XMT_Mission sysXMTMission;

extern unsigned char XMTMission_GetTemperature( void );
extern unsigned char XMTMission_SetTemperature( unsigned int temparater,unsigned char cmdsetflag );
extern unsigned char XMTMission_SetTemperatureS( unsigned int temparater );

#endif

