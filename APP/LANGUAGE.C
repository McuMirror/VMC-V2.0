/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           LANGUAGE.C
** Last modified Date:  2013-03-06
** Last Version:        No
** Descriptions:        �˵�����                    
**------------------------------------------------------------------------------------------------------
** Created by:          gzz
** Created date:        2013-03-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#include "..\config.h"



/*************************************ά����������˵�************************************************/
/*
const char *     Menu_Channel[LANGUAGENO][5]=
{
	 {
	 "1.�������: ",
	 "2.����:     ",
	 "3.��Ʒ����: ",
	 "4.����״̬: ",
	 "5.��Ʒ���: "	 
	 },
	 {
	 "1.Column code:",
	 "2.Price:",
	 "3.Goods remain:",
	 "4.Column status:",
	 "5.Goods NO:"
	 },
	 //change by liya 2012-10-16
	 {
	 "1.����� �ܧ�ݧ�ߧܧ�: ",
	 "2.���֧ߧ�: ",
	 "3.�����ѧ�ܧ� �֧է�: ",
	 "4.����ѧ��� �ܧ�ݧ�ߧܧ�: ",
	 "5. ����ާ֧� �֧է�: "
	 },
	 //��������;by gzz 20121115
	 {
	 	"1 S\xf0\xfctun kodu:",
		"2.Fiyat:",
		"3.Kalan \xf0\xfcr\xf0\xfcn:",
		"4 S\xf0\xfctun  durumu:",
		"5.\xf0\xdcr\xf0\xfcn numaras\xf1\x31:"
	 }
};
*/










/*************************************���׽�����н���************************************************/
const char *     BUSINESS[MAXLANGUAGETYPE][10]=
{
	 {
	 "������Ʒ���:",
	 "�������",
	 "����",
	 "����:",
	 "����",
	 "ȱ��", 
	 "  ��ӭ����",
	 "����",
	  "�����",
	  "��������ֽ��"
	 },
	 {
	 "Input item code:",
	 "Invalid choice",
	 "Disabled",
	 "Price:",
	 "Error",
	 "Empty",
	 "  Welcome",
	 "Channel"
	 },
	 {
	 "���ӧ֧էڧ�� �ܧ�� ���٧ڧ�ڧ�:",
	 "���֧��ѧӧڧݧ�ߧ��� �ӧ��ҧ��",
	 "�����ܧݧ��֧ߧ�",
	 "Price:",
	 "����ڧҧܧ�",
	 "�����էѧ��",
	 "  Welcome",
	 "���ѧߧѧ�",
	 "Welcome",
	 "���ѧߧѧ�"
	 },
	 {
	 "URUN KODU GIRINIZ:",
	 "GECERSIZ SECIM",
	 "DISABLE",
	 "FIYAT:",
	 "HATALI",
	 "BITTI",
	 "HOSGELDINIZ",
	 "SUTUN",
	 "Welcome",
	 "���ѧߧѧ�"
	 },
	 {
	 "ENSERIR CODIGO:",
	 "ESCOLHA INVALIDA",
	 "SEM FUNCIONAMENTO",
	 "VALOR:",
	 "ERRO",
	 "VAZIO",
	 "  BENVINDO",
	 "BANDEJA",
	 "Welcome",
	 "���ѧߧѧ�"
	 }
};

/*************************************���׽�����Ͻ���************************************************/
const char  *BUSINESSERROR[MAXLANGUAGETYPE][23] =
{
   {
	  "��ͣ����",   
	  "������:	",
	  "��Ʒ������",
	  "����������",
	  "ֽ��������",
	  "Ӳ��������",
	  "�ұ���ȱ��",
	  "���л����޻�",
	  "���л�������",
	  "���������",
	  "����������",
	  "ϵͳ��������",
	  "LCD ����",
	  "���������쳣",
	  "����ȷ�Ϲ���",
	  "Flash����",
	  "ʱ�ӹ���",
	  "ȡ����������",
	  "������û��",
	  "����",
	  "ע��",
	  "�������޷�����,�����ѡ��",
	  "�����,��������ֽ��"
	  
   },
   {	
	  "Sorry, out of service",
	  "Err code: ",
	  "Channel err",
	  "Hopper is wrong",
	  "Bill err",
	  "Coin err",
	  "Hopper empty",
	  "Column empty",
	  "column err",
	  "column err",
	  "column err",
	  "system err",
	  "LCD err",
	  "price err",
	  "GOC err",
	  "Flash err",
	  "RTC err",
	  "FGP err",
	  "Door Open",
	  "Sorry",
	  "Caution",
	  "Unable to Give Change Now",
	  "Bill is disable"
   },
   //change by liya 2012-10-16
   {
	  "���� ��ѧҧ��ѧ֧�",
	  "����� ���ڧҧܧ�:",
	  "����ڧҧܧ� �ܧѧߧѧݧ�",
	  "������֧� ",
	  "���ڧݧ� ",
	  "����ߧ֧��",
	  "���������ӧڧ� �ӧѧݧ���",
	  "����էѧߧߧ���",
	  "�ߧ֧ڧ���ѧӧߧ����",
	  "�ߧ֧ڧ���ѧӧߧ����",
	  "�ߧ֧ڧ���ѧӧߧ����",
	  "��ѧ�ѧާ֧��� ��ڧ��֧ާ�",
	  "LCD",
	  "���֧ߧ� �ߧ֧��ѧӧڧݧ�ߧ�",
	  "GOC",
	  "Flash",
	  "RTC",
	  "FGP",
	  "�էӧ֧�� �ܧ�ߧ�֧ۧߧ֧��",
	  "�ڧ٧ӧڧߧڧ��",
	  "�ڧ٧ӧڧߧڧ��",
	  "�ڧ٧ӧڧߧڧ��",
	  "�ڧ٧ӧڧߧڧ��"
   },
   //��������;by gzz 20121115
   {
	  "SERVIS DISI",   
	  "HATA KODU",
	  "f0dcrf0fcn satf131ldf131",
	  "Def11fif15fiklif11fi hatasf131 tutun",
	  "Bill yetmezlif11fi",
	  "Coin hatasf131",
	  "Paralar eksik sikke bul",
	  "Tf0fcm gf0F6nderiler hif0e7bir mal road",
	  "Tf0fcm Kargo Karayolu fay",
	  "Kargo Karayolu kartf131 arf131zasf131",
	  "Kargo Karayolu kullanf131lamf131yor",
	  "Sistem parametreleri arf131zasf131",
	  "LCD arf131za",
	  "Fiyat anormal",
	  "GOC arf131za",
	  "Flash arxf1x31za",
	  "RTC arxf1x31za",
	  "FGP arxf1x31za",
	  "Kapxf1x31 kapalxf1x31 dexf1x1fil Pick",
	  "SERVIS DISI",
	  "\xf0\xfczg\xf0\xfcn\xf0\xfcm",
	  "\xf0\xfczg\xf0\xfcn\xf0\xfcm",
	  "P.USTU YOK SADECE BOZUK P"
   },
   {	
	  "DESLIGADO",
	  "CODIGO ERRADO: ",
	  "Channel err",
	  "Hopper is wrong",
	  "Bill err",
	  "Coin err",
	  "Hopper empty",
	  "Column empty",
	  "column err",
	  "column err",
	  "column err",
	  "system err",
	  "LCD err",
	  "price err",
	  "GOC err",
	  "Flash err",
	  "RTC err",
	  "FGP err",
	  "Door Open",
	  "Sorry",
	  "Sorry",
	  "Sorry",
	  "Sorry"
   }
};


/*************************************���׽��潻�׽���************************************************/
const char  *BUSINESSDEAL[MAXLANGUAGETYPE][20] =
{
   {	  
	  "���",
	  "���",
	  "������С���ֽ��",
	  "����,",
	  "�밴��ѡ��",
	  "����ѡ��",
	  "�����˱�",
	  "������Ʒ���:",
	  "�������",
	  "����",
	  "ȱ��",
	  "���ڳ���..",
	  "ѡ���ɹ�, ��ȡ��",
	  "��Ʒ������",
	  "��Ǯ����",
	  "���ڿۿ�,���Ժ�..",
	  "�ۿ�ʧ��",
	  "��ʱ:",
	  "��������Ӻ�:",
	  "��,��������Ӻ�:"
	  
   },
   {
   	  "Amount",	
	  "Balance",
	  "Pls insert a smaller bill",
	  "Insufficient fund,",
	  "Pls choose the product",
	  "Continue",
	  "Coin refund",
	  "Input Item code:",
	  "Invalid choice",
	  "Error",
	  "Empty",
	  "Dispensing..",
	  "Taking the goods",
	  "Sold out",
	  "Insufficient change",
	  "Debiting, please wait ... ",
	  "Debit failed",
	  "Time:",
	  "Input Cabinet No.:",
	  ",Input Box No.:"
	  
	  
   },
   //change by liya 2012-10-16
   {
   	  "���ѧݧѧߧ�",
	  "���ѧݧѧߧ�",
	  "�����ѧӧ��� ���ާ֧ߧ���",
	  "���ѧۧէ֧ߧ� �ߧ֧�����,",
	  "����اѧݧ�ۧ���, �ߧѧاާڧ�� �ܧ��ڧ��",
	  "����է�ݧاѧ��",
	  "CASHOUT"	 ,
	  "���ӧ֧էڧ�� �ܧ�� ���٧ڧ�ڧ�:",
	  "���֧��ѧӧڧݧ�ߧ��� �ӧ��ҧ��",
	  "����ڧҧܧ�",
	  "�����էѧ��",
	  "���ѧ�ӧѧ� �֧է�..",
	  "���ѧҧ֧�ڧ�� �֧է�",
	  "�����էѧߧ�",
	  "���٧ާ֧ߧڧ�� �ߧ֧�ӧѧ�ܧ�",
	  "�է֧ҧ֧��֧���..",
	  "Fail",
	  "Time:",
	  "Input Bin No.:",
	  ",Input Box No.:"
	   
   },
   //��������;by gzz 20121115
   {
   	  "TUTAR",
	  "BAKIYE",
	  "GECERLI KAGIT PARA GIRINIZ",
	  "YETERSIZ BAKIYE",
	  "LUTFEN URUN SECINIZ",
	  "DEVAM",
	  "PARA IADE",
	  "xf0xdcrxf0xfcn kodunu giriniz:",
	  "GECERSIZ SECIM",
	  "HATA",
	  "BOS",
	  "DAGITIM",
	  "URUNU ALINIZ",
	  "URUN YOK",
	  "BOZUK PARA YETERSIZ",
	  "borxf0xe7 olmak..",
	  "Fail",
	  "Time:",
	  "Input Bin No.:",
	  ",Input Box No.:"
   },
   {
   	  "VALOR",	
	  "SALDO",
	  "POR FAVOR ENSERIR MENOR NOTA",
	  "SALDO INSUFICIENTE,",
	  "ESCOLHA O PRODUTO",
	  "CONTINUE",
	  "TROCO MOEDA",
	  "ENSERIR CODIGO:",
	  "ESCOLHA INVALIDA",
	  "ERRO",
	  "VAZIO",
	  "PREPARANDO PARA SAIR",
	  "PEGAR PRODUTOS",
	  "SEM PRODUTO",
	  "TROCO  INSUFICIENTE",
	  "COBRANDO, OBRIGADO ... ",
	  "FALHA NA COBRAN\x83\A",
	  "TEMPO:",
	  "Input Bin No.:",
	  ",Input Box No.:" 
	  
   }
};

/*************************************���׽����������************************************************/
const char  *BUSINESSCHANGE[MAXLANGUAGETYPE][5] =
{
   {
      "��������",	  
	  "���µ�ͷ�����,Ƿ��:",
	  "��ȡ��Ǯ",
	  "Ʒζ��;���ղؼ���",
	  "   лл�ݹ�!"
   },
   {
      "Changing",
	  "IOU:",	  
	  "Please take the changes",
	  "Thank You",
	  " "
   },
   //change by liya	2012-10-16
   {
      "���ѧާ֧ߧ�:",
	  "�� �է�ݧ֧� ���ѧ�: ",	 
	  "���ѧҧ�� �ާ�ߧ֧�",
	  "����ѧ�ڧҧ�",
	  "   ����ѧ�ڧҧ�"
	  
   },
   //��������;by gzz 20121115
   {
      "BEKLEYINIZ",
	  "LUTFEN SERVISI ARAYIN: ",	 
	  "LUTFEN PARA USTUNU ALINIZ",
	  "TESEKKUR EDERIZ",
	  "TESEKKUR EDERIZ"
   },
   {
      "TROCANDO",
	  "IOU:",	  
	  "POR FAVOR PEGUE O TROCO",
	  "OBRIGADO",
	  "   OBRIGADO"
   }
};



