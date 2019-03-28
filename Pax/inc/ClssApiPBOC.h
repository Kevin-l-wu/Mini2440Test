/*****************************************************/
/* ClssApi_Pboc.h                                    */
/* Define the Application Program Interface          */
/* of qPBOC for all PAX Readers                      */
/* Created by Liuxl at July 30, 2009                 */
/*****************************************************/

#ifndef _CLSS_QPBOC_LIB_H
#define _CLSS_QPBOC_LIB_H

#include "CLCommon.h"


//��Կ��Ӧ�ù���������
int  Clss_CoreInit_Pboc(void);
int Clss_ReadVerInfo_Pboc(char *paucVer);
int  Clss_GetTLVData_Pboc(ushort Tag, uchar *DtOut, int *nDtLen);
int  Clss_SetTLVData_Pboc(ushort usTag, uchar *pucDtIn, int nDtLen);
 
//��Կ��Ӧ�ù��������� 
int  Clss_AddCAPK_Pboc(EMV_CAPK  *capk );
int  Clss_GetCAPK_Pboc(int Index, EMV_CAPK  *capk );
int  Clss_DelCAPK_Pboc(unsigned char KeyID, unsigned char *RID);
void Clss_DelAllCAPK_Pboc(void);

// added by liuxl 20070710 ���޷���Ӧ�ó����ƹ�֤���б���������ӽӿ�
int  Clss_AddRevocList_Pboc(EMV_REVOCLIST *pRevocList);
int  Clss_DelRevocList_Pboc(unsigned char ucIndex, unsigned char *pucRID);
void Clss_DelAllRevocList_Pboc(void);


// �����������
int Clss_SetReaderParam_Pboc(Clss_ReaderParam *ptParam);
int Clss_GetReaderParam_Pboc(Clss_ReaderParam *ptParam);
int Clss_SetReaderChannel_Pboc(uchar ucChannel);// ���÷ǽӶ�����ͨ�� [2/14/2011 zhoujie]

// �Զ�������������
int  Clss_SetEMVUnknowTLVData_Pboc(unsigned short usTag, unsigned char *pDatIn, int nLen);

//����(�ر�)EMV��CORE�ĵ���״̬
void Clss_SetDebug_Pboc(int EnableFlag);
void Clss_GetICCStatus_Pboc(unsigned char *SWA, unsigned char *SWB);

//���״���������
int Clss_SetPbocAidParam_Pboc(Clss_PbocAidParam *ptParam);
int Clss_SetTransData_Pboc(Clss_TransParam *ptTransParam, Clss_PreProcInterInfo *ptInfo);
int Clss_SetFinalSelectData_Pboc(uchar *paucDtIn, int nDtLen);
int Clss_Proctrans_Pboc(uchar *pucTransPath, uchar *pucACType);
int Clss_GetGPOData_Pboc(uchar *pucGPODt, int *nGPODtLen);
int Clss_ReSendLastCmd_Pboc(unsigned char *pucTransPath, unsigned char *pucACType);//  [1/6/2011 zhoujie]

// only for qVSDC and wave2
int Clss_CardAuth_Pboc(uchar *pucACType, uchar *pucDDAFailFlg);

// all schemeid
int Clss_GetCvmType_Pboc(uchar *pucType);
//��GET DATA�����ȡ��Ƭ����
int Clss_GetDataCmd_Pboc(ushort usTag, uchar *DtOut, int *nDtLen);
// MSD track data
int Clss_nGetTrack1MapData_Pboc(uchar *pTrackData, int *pLen); // for MSD and wave2
int Clss_nGetTrack2MapData_Pboc(uchar *pTrackData, int *pLen); // only for MSD

unsigned char cPiccIsoCommand_Pboc(uchar cid, APDU_SEND *ApduSend, APDU_RESP *ApduRecv);

// �������Ҫ����GPO�󣬶�Ӧ������֮ǰ�����쳣�ļ���飬���������ؽӿ� [9/8/2011 zhoujie]
int cClssCheckExceptionFile_Pboc(uchar *pucPAN, int nPANLen, uchar *pucPANSeq);

// ��ͨ����Ӧ�ó�ʼ���ӿ� [2012.3.27 ����]
int Clss_Proctrans_Pboc_UnlockApp(uchar *pucTransPath);

//SM2������֤
unsigned char cClssSM2Verify_Pboc(unsigned char *paucPubkeyIn,unsigned char *paucMsgIn,int nMsglenIn,unsigned char *paucSignIn, int nSignlenIn);

#endif
