
#ifndef _EMV_COMMON_H
#define _EMV_COMMON_H

typedef struct  
{
    unsigned long  ulAmntAuth;     // ��Ȩ���(unsigned long), ��Ϊ����, ��ý�������ulAmntOther�Ľ��
    unsigned long  ulAmntOther;    // �������(unsigned long) 
    unsigned long  ulTransNo;      // �������м�����(4 BYTE)
	unsigned char  ucTransType;    // ��������'9C', 0-����/���� 1-�ֽ�/����
	unsigned char  aucTransDate[4]; // �������� YYMMDD
	unsigned char  aucTransTime[4]; // ����ʱ�� HHMMSS
}Clss_TransParam;

typedef struct{
    int MaxLen;
    unsigned short Tag;
    unsigned short Attr;
	unsigned short usTemplate[2];// ������ģ�棬û����Ϊ0
	unsigned char ucSource;// ����ȡֵEMV_SRC_TM��EMV_SRC_ICC��EMV_SRC_ISS
}ELEMENT_ATTR;


typedef struct  
{
	unsigned char	ucAidLen;				//AID Length
	unsigned char	ucAid[17];			//Current aid
	unsigned char	ucScriptLen;				//issuer script data len
	unsigned char	ucScriptData[300];			//issuer script data	
}iss_scrstrc;

#endif
