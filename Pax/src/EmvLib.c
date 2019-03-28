#include "EmvLib.h"

EMV_PARAM		glEmvParam;

//密钥和应用管理函数定义
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

//密钥和应用管理函数定义
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

// 因无法由应用程序绕过证书列表问题而增加接口
int  EMVAddRevocList(EMV_REVOCLIST *pRevocList)
{
	
}
int  EMVDelRevocList(unsigned char ucIndex, unsigned char *pucRID)
{
	
}
void EMVDelAllRevocList(void)
{
	
}

//交易处理函数定义
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

//读日志时调用
//增加参数 ucFlg, 用于确定是否加入锁定的应用
int  EMVAppSelectForLog(int Slot, unsigned char ucFlg)
{
	
}
int  EMVAddIccTag(ELEMENT_ATTR tEleAttr[], int nAddNum)
{
	
}

//回调函数定义
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
int  cEMVSetParam(void) //因应用需求，增加返回值处理
// 标准非接PBOC回调，接触式可将此回调函数实现为空函数
//unsigned char  cEMVPiccIsoCommand(unsigned char cid, APDU_SEND* ApduSend, APDU_RESP* ApduRecv);
{
	
}
*/
//证件认证(PBOC2.0)
/*
int  cCertVerify(void)
{
	
}
*/
//读交易日志(PBOC2.0)
int ReadLogRecord(int RecordNo)
{
	
}
int GetLogItem(unsigned short Tag, unsigned char *TagData, int *TagLen)
{
	
}

// 获取参数标志,如打印签名标志
int EMVGetParamFlag(unsigned char ucParam, int *pnFlg)
{
	
}

// 读电子现金余额时调用
int EMVGetCardECBalance(unsigned long *plBalance)
{
	
}
int EMVSetTmECParam(EMV_TMECPARAM *tParam)
{
	
}

// 设置MCK配置中的参数，用于将MCK配置设置传给内核
int EMVSetMCKParam(EMV_MCKPARAM *pMCKParam){
	
}
int EMVGetMCKParam(EMV_MCKPARAM *pMCKParam)// 获取内核的MCK参数
{
	
}

// 获取EMV 库发布版本号及发布日期信息
int EMVReadVerInfo(char *paucVer)
{
	
}

//增加设置是否支持advice的标志
void EMVSetConfigFlag(int nConfigflag)
{
	
}

// 初始化EMV数据元值（内核设置默认值）
void EMVInitTLVData(void)
{
	
}

// 标准非接PBOC专用接口
// 设置非接PBOC最终选择以及GPO数据到内核中
int EMVSwitchClss(Clss_TransParam *ptTransParam,unsigned char *pucSelData, int nSelLen, uchar *pucGPOData, int nGPOLen)
{
	
}

// 设置大金额到内核
int EMVSetAmount(unsigned char * szAuthAmount, unsigned char * szCashBackAmount)
{
	
}


//ic卡类操作
uchar cEMVIccIsoCommand(uchar ucslot, APDU_SEND *tApduSend, APDU_RESP *tApduRecv)	//IC卡操作函数。该函数支持IC卡通用接口协议(T=0及T=1)。
{
	
}
//明文、密文PIN认证
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