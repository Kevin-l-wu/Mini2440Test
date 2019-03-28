#include "ClssApiMC.h"
#include "ClssApiAE.h"
#include "ClssApiWAVE.h"


//MC

int  Clss_CoreInit_MC(uchar ucDESupportFlag)
{
	
}
int  Clss_ReadVerInfo_MC(char *paucVer)
{
	
}
int  Clss_AddProprietaryData_MC(TLV_ELEMENT_MC *ptTLV_Element)
{
	
}
int  Clss_SetTLVDataList_MC(uchar *pucTLVDatas, uint unDataLen)
{
	
}
int  Clss_GetTLVDataList_MC(uchar *pucTagList, uchar ucTagListLen, uint unExpectDataLen, uchar *pucDataOut, uint *punActualDataOutLen)
{
	
}
int  Clss_SetTagPresent_MC(uchar *pucTag, uchar ucPresent)
{
	
}
int  Clss_SetParam_MC(uchar *paucTLVParam, uint unParamLen)
{
	
}

//CAPK
int  Clss_AddCAPK_MC_MChip(EMV_CAPK  *ptCAPKey)
{
	
}
void Clss_DelAllCAPK_MC_MChip(void)
{
	
}

// Revocation list
int  Clss_AddRevocList_MC_MChip(EMV_REVOCLIST *ptRevocList)
{
	
}
int  Clss_DelRevocList_MC_MChip(unsigned char ucIndex, unsigned char *pucRID)
{
	
}
void Clss_DelAllRevocList_MC_MChip(void)
{
	
}

 //Transaction processing
int Clss_SetFinalSelectData_MC(uchar *pucRspIn, int nRspLen)
{
	
}
int Clss_InitiateApp_MC(void)
{
	
}
int Clss_ReadData_MC(uchar *pucPathTypeOut)
{
	
}
// For Mag-Stripe only
int Clss_TransProc_MC_Mag(uchar *pucACTypeOut)
{
	
}
//end
// For MCHIP only
int Clss_TransProc_MC_MChip(uchar *pucACTypeOut)
{
	
}
int Clss_CleanTornLog_MC_MChip(uchar *pucCurrentDateTime, int nDateTimeLen, uchar ucCleanAllFlg)
{
	
}
int Clss_SetTornLog_MC_MChip(CLSS_TORN_LOG_RECORD *ptTornLog, int nTornNum)
{
	
}
int Clss_GetTornLog_MC_MChip(CLSS_TORN_LOG_RECORD *ptTornLog, int *pnTornNum, int *pnUpdatedFlg)
{
	
}
//end


int Clss_SetCBFun_SendTransDataOutput_MC(LCP_SENDTRANSDATAOUTPUT_MC cClssSendTransDataOutput)
{
	
}








//Parameters and Data Management Functions
int Clss_CoreInit_AE(void)
{
	
}
int Clss_ReadVerInfo_AE(char *paucVer)
{
	
}
int Clss_GetTLVData_AE(unsigned short usTag, unsigned char *pucDtOut, int *pnDtLen)
{
	
}
int Clss_SetTLVData_AE(unsigned short usTag, unsigned char *pucDtIn, int nDtLen)
{
	
}
int Clss_GetReaderParam_AE(Clss_ReaderParam_AE *ptParam)
{
	
}
int Clss_SetReaderParam_AE(Clss_ReaderParam_AE *ptParam)
{
	
}
int Clss_GetAddReaderParam_AE(Clss_AddReaderParam_AE *ptParam)
{
	
}
int Clss_SetAddReaderParam_AE(Clss_AddReaderParam_AE *ptParam)
{
	
}

int Clss_SetAEAidParam_AE(CLSS_AEAIDPARAM *ptParam)
{
	
}
int Clss_nGetTrackMapData_AE(unsigned char ucTrackFlg, unsigned char *pucDataOut, unsigned char *pucLenOut)
{
	
}
unsigned char Clss_GetOptimizeFlag_AE(void)
{
	
}

//Certification Authority (CA) Public Key  Management Functions
int Clss_AddCAPK_AE(EMV_CAPK  *ptCAPK)
{
	
}
void Clss_DelAllCAPK_AE(void)
{
	
}

//Terminal Issuer Public Key Certification Revocation List Management Functions
int  Clss_AddRevocList_AE(EMV_REVOCLIST *ptRevocList)
{
	
}
void Clss_DelAllRevocList_AE(void)
{
	
}

//Transaction Processing Functions
int Clss_SetFinalSelectData_AE(unsigned char *pucRspIn, int nRspLen)
{
	
}
int Clss_SetTransData_AE(Clss_TransParam *ptTransParam,Clss_PreProcInterInfo *ptInfo)
{
	
}
int Clss_Proctrans_AE(unsigned char *pucTransMode)
{
	
}
int Clss_ReadRecord_AE(unsigned char *pucOptimizeFlag)
{
	
}
int Clss_CardAuth_AE(void)
{
	
}
int Clss_StartTrans_AE(unsigned char ucSupFullOnline, unsigned char *pucAdviceFlag, unsigned char *pucOnlineFlag)
{
	
}
int Clss_CompleteTrans_AE(unsigned char ucOnlineResult,unsigned char ucOnlineMode,
						  ONLINE_PARAM *ptOnlineParam, unsigned char *pucAdviceFlag)
						  {
							  
						  }
int Clss_GetCVMType_AE(unsigned char *pucType)
{
	
}


int Clss_GetDebugInfo_AE(int nExpAssistInfoLen, unsigned char *paucAssistInfo, int *pnErrorCode)
{
	
}


//ENTRY

// 20090721
int  Clss_ReadVerInfo_Entry(char *paucVer)
{
	
}
int Clss_CoreInit_Entry(void)
{
	
}

//for monitor platform only
void Clss_GetICCStatus_Entry(unsigned char *SWA, unsigned char *SWB)
{
	
}
void Clss_SetDebug_Entry(int EnableFlag)
{
	
}

int Clss_SetPreProcInfo_Entry(Clss_PreProcInfo *ptPreProcInfoIn)
{
	
}
int Clss_PreTransProc_Entry(Clss_TransParam *pstTransParam)
{
	
}
int Clss_AppSlt_Entry(int Slot, int ReadLogFlag)
{
	
}
int Clss_FinalSelect_Entry(uchar *pucKernType, uchar *pucDtOut, int *pnDtLen)
{
	
}
int Clss_GetPreProcInterFlg_Entry(Clss_PreProcInterInfo *ptInfo)
{
	
}
//add for paypass 3.0.1 [1/22/2013 ZhouJie]
int Clss_GetErrorCode_Entry(int *pnErrorCode)
{
	
}
int Clss_SetMCVersion_Entry(uchar ucVer)
{
	
}
//add for PayWave
int Clss_SetExtendFunction_Entry(uchar *paucExFunc)
{
	
}

// 20090721 
void Clss_DelAllAidList_Entry(void)
{
	
}
int Clss_DelAidList_Entry(uchar  *pucAID, uchar  ucAidLen)
{
	
}
int Clss_AddAidList_Entry(uchar *pucAID, uchar ucAidLen, uchar ucSelFlg, uchar ucKernType)
{
	
}
int Clss_DelCurCandApp_Entry(void)
{
	
}

int Clss_GetFinalSelectData_Entry(uchar *paucDtOut, int *pnDtLen)
{
	
}

// 20090902
void Clss_DelAllPreProcInfo(void)
{
	
}
int Clss_DelPreProcInfo_Entry(uchar  *pucAID, uchar  ucAidLen)
{
	
}


//wanmin add on 2012.3.27
int clss_AppSelect_Entry_UnlockApp(Clss_TransParam *ptTransParam, ClssTmAidList *ptTermAid)
{
	
}







//WAVE


int  Clss_CoreInit_Wave(void)
{
	
}
int  Clss_ReadVerInfo_Wave(char *paucVer)
{
	
}
int  Clss_GetTLVData_Wave(unsigned short Tag, unsigned char *DtOut, int *nDtLen)
{
	
}
int  Clss_SetTLVData_Wave(unsigned short usTag, unsigned char *pucDtIn, int nDtLen)
{
	
}
 
int Clss_SetReaderParam_Wave(Clss_ReaderParam *ptParam)
{
	
}
int Clss_GetReaderParam_Wave(Clss_ReaderParam *ptParam)
{
	
}

void Clss_SetRdSchemeInfo_Wave(unsigned char ucNum, Clss_SchemeID_Info *pInfo)
{
	
}
void Clss_GetSchemeInfo_Wave(unsigned char *pNum, Clss_SchemeID_Info *pInfo)
{
	
}

int  Clss_SetEMVUnknowTLVData_Wave(unsigned short usTag, unsigned char *pDatIn, int nLen)
{
	
}

//CAPK
int  Clss_AddCAPK_Wave(EMV_CAPK  *capk )
{
	
}
int  Clss_GetCAPK_Wave(int Index, EMV_CAPK  *capk )
{
	
}
int  Clss_DelCAPK_Wave(unsigned char KeyID, unsigned char *RID)
{
	
}
void Clss_DelAllCAPK_Wave(void)
{
	
}

// Revocation list
int  Clss_AddRevocList_Wave(EMV_REVOCLIST *pRevocList)
{
	
}
int  Clss_DelRevocList_Wave(unsigned char ucIndex, unsigned char *pucRID)
{
	
}
void Clss_DelAllRevocList_Wave(void)
{
	
}
 
 //Transaction processing
int Clss_SetDRLParam_Wave(Clss_ProgramID tDRLParam)
{
	
}
int Clss_SetVisaAidParam_Wave(Clss_VisaAidParam *ptParam)
{
	
}
int Clss_SetTransData_Wave(Clss_TransParam *ptTransParam, Clss_PreProcInterInfo *ptInfo)
{
	
}
int Clss_SetFinalSelectData_Wave(unsigned char *paucDtIn, int nDtLen)
{
	
}
int Clss_Proctrans_Wave(unsigned char *pucTransPath, unsigned char *pucACType)
{
	
}

// only for qVSDC and wave2
int Clss_CardAuth_Wave(unsigned char *pucACType, unsigned char *pucDDAFailFlg)
{
	
}
void Clss_SetDDAOmitFlag_Wave(unsigned char ucOmitFlag)
{
	
}
int Clss_GetCvmType_Wave(unsigned char *pucType)
{
	
}
int Clss_ProcRestric_Wave(void)
{
	
}

//Issuer Update Processing
int Clss_IssuerAuth_Wave (unsigned char *pIAuthData, int nIAuthDataLen)
{
	
}
int Clss_IssScriptProc_Wave(unsigned char *pucScript, int nScriptLen)
{
	
}

int Clss_nGetTrack1MapData_Wave(unsigned char *pTrackData, int *pLen)
{
	
}
int Clss_nGetTrack2MapData_Wave(unsigned char *pTrackData, int *pLen)
{
	
}

unsigned char Clss_GetMSDType_Wave(unsigned char *ucMSDType)
{
	
}

//Debug interface
void Clss_SetDebug_Wave(int EnableFlag)
{
	
}
void Clss_GetICCStatus_Wave(unsigned char *SWA, unsigned char *SWB)
{
	
}

int Clss_GetDebugInfo_Wave(int nExpAssistInfoLen, unsigned char *paucAssistInfo, int *pnErrorCode)
{
	
}






//JCB



int Clss_CoreInit_JCB(void)
{
	
}
int Clss_ReadVerInfo_JCB(char *paucVer)
{
	
}

//TLV Data List
int  Clss_GetTLVDataList_JCB(unsigned char *pucTagList, unsigned char ucTagListLen, unsigned int unExpectDataLen, 
                            unsigned char *pucDataOut, unsigned int *punActualDataOutLen)
							{
								
							}
int  Clss_SetTLVDataList_JCB(unsigned char *pucTLVDatas, unsigned int unDataLen)
{
	
}

//CAPK
int  Clss_AddCAPK_JCB(EMV_CAPK  *ptCAPK )
{
	
}
void Clss_DelAllCAPK_JCB(void)
{
	
}

// Revocation list
int  Clss_AddRevocList_JCB(EMV_REVOCLIST *ptRevocList)
{
	
}
void Clss_DelAllRevocList_JCB(void)
{
	
}

//JCB Transaction Processing
int Clss_SetFinalSelectData_JCB(unsigned char *pucRspIn, int nRspLen)
{
	
}
int Clss_InitiateApp_JCB(unsigned char *pucPathTypeOut)
{
	
} 
int Clss_ReadData_JCB(void)
{
	
}
int Clss_TransProc_JCB(unsigned char ucExceptFileFlg)
{
	
}

int Clss_CardAuth_JCB(void)
{
	
}
int Clss_IssuerUpdateProc_JCB(unsigned char *paucScript, int *pnScriptLen)
{
	
}
//Get error code
int Clss_GetDebugInfo_JCB(int nExpAssistInfoLen, unsigned char *paucAssistInfo, int *pnErrorCode)
{
	
}







//DPAS

int Clss_CoreInit_DPAS(void)
{
	
}
int Clss_ReadVerInfo_DPAS(char *paucVer)
{
	
}

//TLV Data List
int  Clss_GetTLVDataList_DPAS(unsigned char *pucTagList, unsigned char ucTagListLen, unsigned int unExpectDataLen, 
                            unsigned char *pucDataOut, unsigned int *punActualDataOutLen)
							{
								
							}
int  Clss_SetTLVDataList_DPAS(unsigned char *pucTLVDatas, unsigned int unDataLen)
{
	
}

//CAPK
int  Clss_AddCAPK_DPAS(EMV_CAPK  *ptCAPK )
{
	
}
void Clss_DelAllCAPK_DPAS(void)
{
	
}

// Revocation list
int  Clss_AddRevocList_DPAS(EMV_REVOCLIST *ptRevocList)
{
	
}
void Clss_DelAllRevocList_DPAS(void)
{
	
}

//DPAS Transaction Processing
int Clss_SetFinalSelectData_DPAS(unsigned char *pucRspIn, int nRspLen)
{
	
}
int Clss_InitiateApp_DPAS(unsigned char *pucPathTypeOut)
{
	
}
int Clss_ReadData_DPAS(void)
{
	
}
int Clss_TransProc_DPAS(unsigned char ucExceptFileFlg)
{
	
}

int Clss_IssuerUpdateProc_DPAS(int nCommuStatus, unsigned char *paucScript, int *pnScriptLen)
{
	
}

int Clss_GetTrackMapData_DPAS(unsigned char ucTrackFlg, unsigned char *pTrackData, int *pLen)
{
	
}

//Get error code
int Clss_GetDebugInfo_DPAS(int nExpAssistInfoLen, unsigned char *paucAssistInfo, int *pnErrorCode)
{
	
}




uchar s_GetMatrixDot(uchar *str,uchar *MatrixDot,unsigned int *len, uchar flag)
{
	
}




