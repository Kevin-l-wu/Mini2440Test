#include "EmvLib.h"

EMV_PARAM		glEmvParam;

//��Կ��Ӧ�ù���������
int  EMVCoreInit(void)
{
	
}
void EMVGetParameter(EMV_PARAM *tParam)
{
	
}
void EMVSetParameter(EMV_PARAM *tParam)
{
	
}
int  EMVGetTLVData(unsigned short Tag, unsigned char *DataOut, int *OutLen)
{
	
}
int  EMVSetTLVData(unsigned short Tag, unsigned char *DataIn, int DataLen)
{
	
}
int  EMVGetScriptResult(unsigned char *Result, int *RetLen)
{
	
}

//��Կ��Ӧ�ù���������
int  EMVAddApp(EMV_APPLIST *App)
{
	
}
int  EMVGetApp(int Index, EMV_APPLIST *App)
{
	
}
int  EMVDelApp(unsigned char *AID, int AidLen)
{
	
}
int  EMVModFinalAppPara(EMV_APPLIST *ptEMV_APP)
{
	
}
int  EMVGetFinalAppPara(EMV_APPLIST *ptEMV_APP)
{
	
}
int  EMVGetLabelList(APPLABEL_LIST  *ptAppLabel, int *pnAppNum) //add by nt 20091218
{
	
}

int  EMVAddCAPK(EMV_CAPK  *capk )
{
	
}
int  EMVGetCAPK(int Index, EMV_CAPK  *capk )
{
	
}
int  EMVDelCAPK(unsigned char KeyID, unsigned char *RID)
{
	
}
int  EMVCheckCAPK(unsigned char *KeyID, unsigned char *RID)
{
	
}

// ���޷���Ӧ�ó����ƹ�֤���б���������ӽӿ�
int  EMVAddRevocList(EMV_REVOCLIST *pRevocList)
{
	
}
int  EMVDelRevocList(unsigned char ucIndex, unsigned char *pucRID)
{
	
}
void EMVDelAllRevocList(void)
{
	
}

//���״���������
int  EMVAppSelect(int Slot,unsigned long TransNo)
{
	
}
int  EMVReadAppData(void)
{
	
}
int  EMVCardAuth(void)
{
	
}
int  EMVStartTrans(unsigned long ulAuthAmt, unsigned long ulCashBackAmt, unsigned char *pACType) //for Visa Online
{
	
}
int  EMVCompleteTrans(int nCommuStatus, unsigned char *paucScript, int *pnScriptLen, unsigned char *pACType) //for Visa Online
{
	
}

//����־ʱ����
//���Ӳ��� ucFlg, ����ȷ���Ƿ����������Ӧ��
int  EMVAppSelectForLog(int Slot, unsigned char ucFlg)
{
	
}
int  EMVAddIccTag(ELEMENT_ATTR tEleAttr[], int nAddNum)
{
	
}

//�ص���������
/*
int  cEMVWaitAppSel(int TryCnt, EMV_APPLIST List[], int AppNum)
{
	
} 
int  cEMVInputAmount(unsigned long *AuthAmt, unsigned long *CashBackAmt)
{
	
}
int  cEMVGetHolderPwd(int TryFlag, int RemainCnt, unsigned char *pin)
{
	
}
int  cEMVReferProc(void)
{
	
}
int  cEMVUnknowTLVData(unsigned short Tag, unsigned char *dat, int len)
{
	
}
void cEMVAdviceProc(void)
{
	
}
void cEMVVerifyPINOK(void){
	
}

*/
/*
int  cEMVSetParam(void) //��Ӧ���������ӷ���ֵ����
// ��׼�ǽ�PBOC�ص����Ӵ�ʽ�ɽ��˻ص�����ʵ��Ϊ�պ���
//unsigned char  cEMVPiccIsoCommand(unsigned char cid, APDU_SEND* ApduSend, APDU_RESP* ApduRecv);
{
	
}
*/
//֤����֤(PBOC2.0)
/*
int  cCertVerify(void)
{
	
}
*/
//��������־(PBOC2.0)
int ReadLogRecord(int RecordNo)
{
	
}
int GetLogItem(unsigned short Tag, unsigned char *TagData, int *TagLen)
{
	
}

// ��ȡ������־,���ӡǩ����־
int EMVGetParamFlag(unsigned char ucParam, int *pnFlg)
{
	
}

// �������ֽ����ʱ����
int EMVGetCardECBalance(unsigned long *plBalance)
{
	
}
int EMVSetTmECParam(EMV_TMECPARAM *tParam)
{
	
}

// ����MCK�����еĲ��������ڽ�MCK�������ô����ں�
int EMVSetMCKParam(EMV_MCKPARAM *pMCKParam){
	
}
int EMVGetMCKParam(EMV_MCKPARAM *pMCKParam)// ��ȡ�ں˵�MCK����
{
	
}

// ��ȡEMV �ⷢ���汾�ż�����������Ϣ
int EMVReadVerInfo(char *paucVer)
{
	
}

//���������Ƿ�֧��advice�ı�־
void EMVSetConfigFlag(int nConfigflag)
{
	
}

// ��ʼ��EMV����Ԫֵ���ں�����Ĭ��ֵ��
void EMVInitTLVData(void)
{
	
}

// ��׼�ǽ�PBOCר�ýӿ�
// ���÷ǽ�PBOC����ѡ���Լ�GPO���ݵ��ں���
int EMVSwitchClss(Clss_TransParam *ptTransParam,unsigned char *pucSelData, int nSelLen, uchar *pucGPOData, int nGPOLen)
{
	
}

// ���ô���ں�
int EMVSetAmount(unsigned char * szAuthAmount, unsigned char * szCashBackAmount)
{
	
}


//ic�������
uchar cEMVIccIsoCommand(uchar ucslot, APDU_SEND *tApduSend, APDU_RESP *tApduRecv)	//IC�������������ú���֧��IC��ͨ�ýӿ�Э��(T=0��T=1)��
{
	
}
//���ġ�����PIN��֤
int cEMVPedVerifyPlainPin(uchar ucIccSlot, uchar *pucExpPinLenIn, uchar *ucIccRespOut, uchar ucMode,ulong ulTimeoutMs)
{
	
}
int cEMVPedVerifyCipherPin(uchar ucIccSlot, uchar *pExpPinLenIn, RSA_PINKEY *tRsaPinKeyIn, uchar *pucIccRespOut, uchar ucMode,ulong ulTimeoutMs)
{
	
}


int EMVSetPCIModeParam(unsigned char ucPciMode, unsigned char *pucExpectPinLen,unsigned long ulTimeoutMs)
{
	
}


int  EMVProcTrans(void)
{
	
}


void EMVGetICCStatus(unsigned char *SWA, unsigned char *SWB)
{
	
}

int EMVClearTransLog(void)
{
	
}