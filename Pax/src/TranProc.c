
#include "Global.h"

/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
static void PackInvoice(STISO8583 *pstSendPack, ulong ulInvoice);
static void ModifyProcessCode(void);
static void SetEntryMode(const TRAN_LOG *pstLog);
static void SetAmexEntryMode(const TRAN_LOG *pstLog);
static void SetStdEntryMode(const TRAN_LOG *pstLog);
static void SetCondCode(void);
static void SetInstAndAirTicketData(void);
static int  TransCashSub(void);
static int  TransSaleSub(void);
static int  TransAuthSub(uchar ucTranType);
static int  GetOfflineTrans(uchar ucTypeFlag);
static int  AdjustInput(void);
static int  TransSettleSub(void);
static int  TransUpLoad(void);

/********************** Internal variables declaration *********************/

/********************** external reference declaration *********************/

/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

void PackInvoice(STISO8583 *pstSendPack, ulong ulInvoice)
{
	memcpy(pstSendPack->sField62, "\x00\x06", 2);
	sprintf_string((char *)&(pstSendPack->sField62[2]), "%06lu", ulInvoice);
}

// 设置请求包的公共bits
// set bit content of ISO8583.
// Work as the "IsoInitSendMsg()" did in old style.
void SetCommReqField(void)
{
#ifdef ENABLE_EMV
	int	iLength;
#endif
	uchar	szTotalAmt[12+1];

	if( glProcInfo.stTranLog.ucTranType==LOAD_PARA )
	{
		memset(&glTMSSend, 0, sizeof(STTMS8583));
		sprintf_string((char *)glTMSSend.szMsgCode, "%.*s", LEN_MSG_CODE,
				glTranConfig[glProcInfo.stTranLog.ucTranType].szTxMsgID);
		sprintf_string((char *)glTMSSend.szProcCode,   "%.*s", LEN_PROC_CODE,
				glTranConfig[glProcInfo.stTranLog.ucTranType].szProcCode);
		sprintf_string((char *)glTMSSend.szNii,    "%.*s", LEN_NII,     glCurAcq.szNii);
		sprintf_string((char *)glTMSSend.szTermID, "%.*s", LEN_TERM_ID, glCurAcq.szTermID);
		sprintf_string((char *)glTMSSend.szSTAN, "%06lu", glSysCtrl.ulSTAN);
		return;
	}

	//ISO8583使用说明
	//Step3: 在这里设置打包时需要的数据，该域赋值则送，为空时则不送(默认的初始值全为空)
	//所有的报文都需要用到的域可在//set common bits下进行统一赋值
	//其它根据交易不同条件不同时可送可不送的分别处理
	//另注意sz型与s型开头成员变量,sz型直接赋所需要上送的值，s型需要在前两位赋值为该域的长度
	//例如:	// bit 62, ROC/SOC定义为  sField62
	//则先赋长度值memcpy(glSendPack.sField62, "\x00\x06", 2);
	//再赋数据值sprintf_string((char *)&glSendPack.sField62[2], "%06lu", glProcInfo.stTranLog.ulInvoiceNo);

	// Usage of ISO8583 module (For step 2, see in st8583.h)
	// Step 3: To set the data need in packing here.
	// If the glSendPack.xxxxx is filled with value, this bit will be sent, and vice versa.

	// Note that there're 2 kinds of member in glSendPack:
	//   glSendPack.szxxxxxx : this type means "string end with zero", the actual length can be determined by strlen()
	//   glSendPack.sxxxxxx  : the first 2 bytes contains length infomation, if length=0x01A0, should be "\x01\xA0"
	//     e.g. : bit62
	//             memcpy(glSendPack.sField62, "\x00\x06", 2);
	//             sprintf_string((char *)&glSendPack.sField62[2], "%06lu", glProcInfo.stTranLog.ulInvoiceNo);

	// set common bits
	memset(&glSendPack, 0, sizeof(STISO8583));
	sprintf_string((char *)glSendPack.szMsgCode,    "%.*s", LEN_MSG_CODE,
			glTranConfig[glProcInfo.stTranLog.ucTranType].szTxMsgID);

	sprintf_string((char *)glSendPack.szProcCode,   "%.*s", LEN_PROC_CODE,
			glTranConfig[glProcInfo.stTranLog.ucTranType].szProcCode);

	// modify bit 3, process code
	ModifyProcessCode();

	sprintf_string((char *)glSendPack.szNii,        "%.*s", LEN_NII,         glCurAcq.szNii);
	sprintf_string((char *)glSendPack.szTermID,     "%.*s", LEN_TERM_ID,     glCurAcq.szTermID);
	sprintf_string((char *)glSendPack.szMerchantID, "%.*s", LEN_MERCHANT_ID, glCurAcq.szMerchantID);

	sprintf_string((char *)glSendPack.szSTAN, "%06lu", glSysCtrl.ulSTAN);  //??
	glProcInfo.stTranLog.ulSTAN = glSysCtrl.ulSTAN;

	if( glProcInfo.stTranLog.ucTranType==SETTLEMENT || glProcInfo.stTranLog.ucTranType==UPLOAD ||
		glProcInfo.stTranLog.ucTranType==LOGON )
	{
		return;
	}

	// bit 4, transaction amount
	if( !ChkIfZeroAmt(glProcInfo.stTranLog.szTipAmount) )
	{
		PubAscAdd(glProcInfo.stTranLog.szAmount, glProcInfo.stTranLog.szTipAmount, 12, szTotalAmt);
		//PubAddHeadChars(szTotalAmt, 12, '0');		no need: already 12 digits
		sprintf_string((char *)glSendPack.szTranAmt,   "%.*s", LEN_TRAN_AMT,   szTotalAmt);
		sprintf_string((char *)glSendPack.szExtAmount, "%.*s", LEN_EXT_AMOUNT, glProcInfo.stTranLog.szTipAmount);
		if (ChkIfAmex())
		{
			if( glProcInfo.stTranLog.ucTranType==REFUND )
			{
				glSendPack.szExtAmount[0] = 0;
			}
			if( glProcInfo.stTranLog.ucTranType==VOID || (glProcInfo.stTranLog.uiStatus & TS_VOID) )
			{
				sprintf_string(glSendPack.szTranAmt, "%012lu", 0L);
				glSendPack.szExtAmount[0] = 0;
			}
		}
	}
	else
	{
		sprintf_string((char *)glSendPack.szTranAmt, "%.*s", LEN_TRAN_AMT, glProcInfo.stTranLog.szAmount);
		if( ChkIfAmex() )
		{
			if( (glProcInfo.stTranLog.ucTranType==VOID) || (glProcInfo.stTranLog.uiStatus & TS_VOID) )
			{
				sprintf_string(glSendPack.szTranAmt, "%012lu", 0L);
				glSendPack.szExtAmount[0] = 0;
			}
		}
	}

	if( (glProcInfo.stTranLog.ucTranType==OFFLINE_SEND) ||
		(glProcInfo.stTranLog.ucTranType==TC_SEND) )
	{
		sprintf_string((char *)glSendPack.szLocalTime, "%.6s",  &glProcInfo.stTranLog.szDateTime[8]);
		sprintf_string((char *)glSendPack.szLocalDate, "%.4s",  &glProcInfo.stTranLog.szDateTime[4]);
		sprintf_string((char *)glSendPack.szRRN,       "%.12s", glProcInfo.stTranLog.szRRN);
		sprintf_string((char *)glSendPack.szAuthCode,  "%.6s",  glProcInfo.stTranLog.szAuthCode);
	}
	else if( glProcInfo.stTranLog.ucTranType==VOID )
	{
		sprintf_string((char *)glSendPack.szLocalTime, "%.6s",  &glProcInfo.stTranLog.szDateTime[8]);
		sprintf_string((char *)glSendPack.szLocalDate, "%.4s",  &glProcInfo.stTranLog.szDateTime[4]);
		sprintf_string((char *)glSendPack.szRRN,       "%.12s", glProcInfo.stTranLog.szRRN);       // jiale 2006.12.12
		sprintf_string((char *)glSendPack.szAuthCode,  "%.6s",  glProcInfo.stTranLog.szAuthCode);	// jiale for void send 37.38field
	}
	else{
	    sprintf_string((char *)glSendPack.szLocalTime, "%.6s",  &glProcInfo.stTranLog.szDateTime[8]);
	    sprintf_string((char *)glSendPack.szLocalDate, "%.4s",  &glProcInfo.stTranLog.szDateTime[4]);
	}

	// PAN/track 1/2/3/expiry etc
	if( glProcInfo.stTranLog.uiEntryMode & MODE_CHIP_INPUT )
	{
		if( (glProcInfo.szTrack2[0]==0) ||
			(glProcInfo.stTranLog.ucTranType==OFFLINE_SEND) ||
			(glProcInfo.stTranLog.ucTranType==TC_SEND) )
		{
			sprintf_string((char *)glSendPack.szPan,     "%.*s", LEN_PAN,      glProcInfo.stTranLog.szPan);
			sprintf_string((char *)glSendPack.szExpDate, "%.*s", LEN_EXP_DATE, glProcInfo.stTranLog.szExpDate);
		}
		else
		{
		    //sprintf_string((char *)glSendPack.szExpDate, "%.*s", LEN_EXP_DATE, glProcInfo.stTranLog.szExpDate);
		    sprintf_string((char *)glSendPack.szTrack2,  "%.*s", LEN_TRACK2,   glProcInfo.szTrack2);
		}
		if( ChkIfCiti() || ChkIfDah() )
		{
			if( glProcInfo.stTranLog.ucTranType!=SETTLEMENT )
			{
				if( glProcInfo.stTranLog.bPanSeqOK )
				{
					sprintf_string((char *)glSendPack.szPanSeqNo, "%0*X", LEN_PAN_SEQ_NO, glProcInfo.stTranLog.ucPanSeqNo);
				}
			}
		}
	}
	else if( (glProcInfo.stTranLog.uiEntryMode & MODE_SWIPE_INPUT) ||
			 (glProcInfo.stTranLog.uiEntryMode & MODE_FALLBACK_SWIPE) )
	{
		if( glProcInfo.stTranLog.ucTranType==OFFLINE_SEND || glProcInfo.stTranLog.ucTranType==VOID )
		{
			sprintf_string((char *)glSendPack.szPan,     "%.*s", LEN_PAN,      glProcInfo.stTranLog.szPan);
			sprintf_string((char *)glSendPack.szExpDate, "%.*s", LEN_EXP_DATE, glProcInfo.stTranLog.szExpDate);
		}
		else
		{
			sprintf_string((char *)glSendPack.szTrack2, "%.*s", LEN_TRACK2, glProcInfo.szTrack2);
			if ( glProcInfo.stTranLog.uiEntryMode & MODE_FALLBACK_SWIPE )
			{
				sprintf_string((char *)glSendPack.szTrack3, "%.*s", LEN_TRACK3, glProcInfo.szTrack3);
			}
			if ( !ChkIfBoc() )
			{
				sprintf_string((char *)glSendPack.szTrack1, "%.*s", LEN_TRACK1, glProcInfo.szTrack1);
			}
		}
	}
	else if( (glProcInfo.stTranLog.uiEntryMode & MODE_MANUAL_INPUT) ||
			 (glProcInfo.stTranLog.uiEntryMode & MODE_FALLBACK_MANUAL) )
	{
		sprintf_string((char *)glSendPack.szPan,     "%.*s", LEN_PAN,      glProcInfo.stTranLog.szPan);
		sprintf_string((char *)glSendPack.szExpDate, "%.*s", LEN_EXP_DATE, glProcInfo.stTranLog.szExpDate);
	}

	SetEntryMode(&glProcInfo.stTranLog);		// bit 22, entry mode
	SetCondCode();		// bit 25, service condition code

	// bit 48 or 55, CVV2 or 4DBC
	if( ChkIfNeedSecurityCode() && !ChkIfAmex() )
	{
		memcpy(glSendPack.sField48, "\x00\x03", 2);
		sprintf_string((char *)&glSendPack.sField48[2], "%-3.3s", glProcInfo.szSecurityCode);
	}

	if( glProcInfo.stTranLog.uiEntryMode & MODE_PIN_INPUT )
	{
		PubLong2Char((ulong)LEN_PIN_DATA, 2, glSendPack.sPINData);
		memcpy(&glSendPack.sPINData[2], glProcInfo.sPinBlock, LEN_PIN_DATA);
	}

	SetInstAndAirTicketData();	// bit 48 and 63

	// bit 49

	// process bit 55,56
	if( glProcInfo.stTranLog.ucTranType==AUTH || glProcInfo.stTranLog.ucTranType==PREAUTH ||
		glProcInfo.stTranLog.ucTranType==SALE || glProcInfo.stTranLog.ucTranType==CASH    ||
		glProcInfo.stTranLog.ucTranType==INSTALMENT )
	{
		if( ChkIfAmex() && ChkIfNeedSecurityCode() && (glProcInfo.szSecurityCode[0]!=0) )
		{
			memcpy(glSendPack.sICCData, "\x00\x04", 2);
			sprintf_string((char *)&glSendPack.sICCData[2], "%-4.4s", glProcInfo.szSecurityCode);
		}
#ifdef ENABLE_EMV
		else if( (glProcInfo.stTranLog.uiEntryMode & MODE_FALLBACK_SWIPE) ||
			(glProcInfo.stTranLog.uiEntryMode & MODE_FALLBACK_MANUAL) )
		{
			SetDE55(DE55_SALE, &glSendPack.sICCData[2], &iLength);
			PubLong2Char((ulong)iLength, 2, glSendPack.sICCData);
			memcpy(glProcInfo.stTranLog.sIccData, &glSendPack.sICCData[2], iLength);
			glProcInfo.stTranLog.uiIccDataLen = (ushort)iLength;
		}
#endif
	}

#ifdef ENABLE_EMV
	if( glProcInfo.stTranLog.ucTranType==VOID && ChkIfFubon() )
	{
		PubLong2Char((ulong)glProcInfo.stTranLog.uiIccDataLen, 2, glSendPack.sICCData);
		memcpy(&glSendPack.sICCData[2], glProcInfo.stTranLog.sIccData, glProcInfo.stTranLog.uiIccDataLen);
		PubLong2Char((ulong)glProcInfo.stTranLog.uiField56Len, 2, glSendPack.sICCData2);
		memcpy(&glSendPack.sICCData2[2], glProcInfo.stTranLog.sField56, glProcInfo.stTranLog.uiField56Len);
	}
	if( glProcInfo.stTranLog.ucTranType==VOID && ChkIfHSBC() &&
		(glProcInfo.stTranLog.uiEntryMode & MODE_CHIP_INPUT) )
	{
		PubLong2Char((ulong)glProcInfo.stTranLog.uiField56Len, 2, glSendPack.sICCData2);
		memcpy(&glSendPack.sICCData2[2], glProcInfo.stTranLog.sField56, glProcInfo.stTranLog.uiField56Len);
	}
	if( ChkIfBea() || ChkIfScb() )
	{
		if( (glProcInfo.stTranLog.ucTranType==VOID) ||
			(glProcInfo.stTranLog.ucTranType==OFFLINE_SEND) ||
			(glProcInfo.stTranLog.ucTranType==TC_SEND) )
		{
			PubLong2Char((ulong)glProcInfo.stTranLog.uiIccDataLen, 2, glSendPack.sICCData);
			memcpy(&glSendPack.sICCData[2], glProcInfo.stTranLog.sIccData, glProcInfo.stTranLog.uiIccDataLen);
		}
	}
#endif

	// bit 62, ROC/SOC
	PackInvoice(&glSendPack, glProcInfo.stTranLog.ulInvoiceNo);
	// bit 63
	// ...

	if( ChkIfNeedMac() )
	{
		PubLong2Char((ulong)LEN_MAC, 2, glSendPack.sMac);
	}
}

// Bit 3 definition:
// AMEX 0200:
// For a sale transaction									00 40 0x
// For a refund transaction									20 40 0x
// For an on line void of on line sale						02 40 0x
// For an on line void of on line refund					22 40 0x
// For an on line void of off line sale seen by host		02 40 0x
// For an on line void of off line sale not seen by host	00 40 0x
//
// AMEX 0220:
// For a sale transaction (referred/sale comp or off line sale)
//													00 40 0x
// For an sale adjustment (i.e. add tip or void of sale):
//		When original sale not seen by host			00 40 0x
//		When original sale seen by host				02 40 0x
// For an off line refund transaction				20 40 0x
// For a void refund transaction:
//		When Trickle fed refund to host				22 40 0x
//		When Void off line refund not trickle fed	20 40 0x
void ModifyProcessCode(void)
{
	if( ChkIfAmex() )
	{
		glSendPack.szProcCode[2] = '4';
		if( glProcInfo.stTranLog.ucTranType==OFFLINE_SEND )
		{
			if( glProcInfo.stTranLog.ucOrgTranType==SALE     ||
				glProcInfo.stTranLog.ucOrgTranType==OFF_SALE ||
				glProcInfo.stTranLog.ucOrgTranType==SALE_COMP )
			{
				if( glProcInfo.stTranLog.uiStatus & (TS_ADJ|TS_VOID) )
				{
					if( glProcInfo.stTranLog.szRRN[0]!=0 )
					{
						glSendPack.szProcCode[1] = '2';
					}
				}
			}
			else if( glProcInfo.stTranLog.ucOrgTranType==REFUND )
			{
				glSendPack.szProcCode[0] = '2';
				if( glProcInfo.stTranLog.uiStatus & TS_VOID )
				{
					if( glProcInfo.stTranLog.ucTranType!=SETTLEMENT )
					{	// trickle feed
						glSendPack.szProcCode[1] = '2';
					}
				}
			}
		}
		else if( glProcInfo.stTranLog.ucTranType==VOID )
		{
			if( glProcInfo.stTranLog.ucOrgTranType==SALE )
			{
				glSendPack.szProcCode[1] = '2';
			}
			else if( glProcInfo.stTranLog.ucOrgTranType==REFUND )
			{
				glSendPack.szProcCode[0] = '2';
				glSendPack.szProcCode[1] = '2';
			}
			else if( glProcInfo.stTranLog.ucOrgTranType==OFF_SALE ||
					 glProcInfo.stTranLog.ucOrgTranType==SALE_COMP )
			{
				if( glProcInfo.stTranLog.szRRN[0]!=0 )
				{
					glSendPack.szProcCode[1] = '2';
				}
			}
		}
	}
	else
	{
		if( glProcInfo.stTranLog.ucTranType==OFFLINE_SEND )
		{
			if( glProcInfo.stTranLog.ucOrgTranType==SALE     ||
				glProcInfo.stTranLog.ucOrgTranType==OFF_SALE ||
				glProcInfo.stTranLog.ucOrgTranType==SALE_COMP )
			{
				if( glProcInfo.stTranLog.uiStatus & (TS_ADJ|TS_VOID) )
				{
					if( glProcInfo.stTranLog.szRRN[0]!=0 )
					{
						glSendPack.szProcCode[1] = '2';
					}
				}
			}
			else if( glProcInfo.stTranLog.ucOrgTranType==REFUND )
			{
				glSendPack.szProcCode[0] = '2';
				if( glProcInfo.stTranLog.uiStatus & TS_ADJ )
				{
					if( glProcInfo.stTranLog.szRRN[0]!=0 )
					{
						glSendPack.szProcCode[1] = '2';
					}
				}
			}
		}
		else if( glProcInfo.stTranLog.ucTranType==VOID )
		{
			if( glProcInfo.stTranLog.ucOrgTranType==SALE )
			{
				glSendPack.szProcCode[1] = '2';
			}
			else if( glProcInfo.stTranLog.ucOrgTranType==REFUND )
			{
				glSendPack.szProcCode[0] = '2';
				glSendPack.szProcCode[1] = '2';
			}
		}

        // Processing code 3rd digit
		// ...
	}

    PubStrUpper(glSendPack.szProcCode);
}

void SetEntryMode(const TRAN_LOG *pstLog)
{
	sprintf_string((char *)glSendPack.szEntryMode, "0000");

	if( ChkIfAmex() )
	{
		SetAmexEntryMode(pstLog);
	}
	else
	{
		SetStdEntryMode(pstLog);
	}
}

void SetAmexEntryMode(const TRAN_LOG *pstLog)
{
	glSendPack.szEntryMode[3] = '2';
	if (ChkIfPinReqdAllIssuer())
	{
		glSendPack.szEntryMode[3] = '1';	// pin capable
	}

#ifdef ENABLE_EMV
	if( ChkAcqOption(ACQ_EMV_FEATURE) )
	{
		EMVGetParameter(&glEmvParam);
		if (glEmvParam.Capability[1] & 0x40)
		{
			glSendPack.szEntryMode[3] = '1';	// pin capable
		}
		if (glEmvParam.Capability[1] & 0x90)
		{
			glSendPack.szEntryMode[3] = '3';	// offline pin capable
		}
	}
#endif

#ifdef ENABLE_EMV
	if( ChkAcqOption(ACQ_EMV_FEATURE) )
	{
		glSendPack.szEntryMode[1] = '5';
	}
#endif

	if( pstLog->uiEntryMode & MODE_SWIPE_INPUT )
	{
		if (glProcInfo.stTranLog.uiEntryMode & MODE_SECURITYCODE)
		{
			glSendPack.szEntryMode[2] = '6';
		}
		else
		{
			glSendPack.szEntryMode[2] = '2';
		}
	}
#ifdef ENABLE_EMV
	else if( pstLog->uiEntryMode & MODE_CHIP_INPUT )
	{
		glSendPack.szEntryMode[2] = '5';
	}
	else if( pstLog->uiEntryMode & MODE_FALLBACK_SWIPE )
	{
		glSendPack.szEntryMode[1] = '6';
		glSendPack.szEntryMode[2] = (glProcInfo.szSecurityCode[0]!=0) ? '6' : '2';
	}
	else if( pstLog->uiEntryMode & MODE_FALLBACK_MANUAL )
	{
		// ????;
	}
#endif
	else if( pstLog->uiEntryMode & MODE_MANUAL_INPUT )
	{
		glSendPack.szEntryMode[2] = (glProcInfo.szSecurityCode[0]!=0) ? '7' : '1';
	}
}

void SetStdEntryMode(const TRAN_LOG *pstLog)
{
	//------------------------------------------------------------------------------
	// Entry mode digit 1
#ifdef ENABLE_EMV
	if( ChkAcqOption(ACQ_EMV_FEATURE) )
	{
		if( ChkIfBoc() || ChkIfBea() )
		{
			glSendPack.szEntryMode[0] = '5';
		}
	}
#endif

	//------------------------------------------------------------------------------
	// Entry mode digit 2 and digit 3
	if( pstLog->uiEntryMode & MODE_MANUAL_INPUT )
	{
		memcpy(&glSendPack.szEntryMode[1], "01", 2);
	}
	else if( pstLog->uiEntryMode & MODE_SWIPE_INPUT )
	{
		memcpy(&glSendPack.szEntryMode[1], "02", 2);
	}
	else if( pstLog->uiEntryMode & MODE_CHIP_INPUT )
	{
		memcpy(&glSendPack.szEntryMode[1], "05", 2);
	}
	else if( pstLog->uiEntryMode & MODE_FALLBACK_SWIPE )
	{
		memcpy(&glSendPack.szEntryMode[1], "80", 2);

		// sort by banks (acquirer)
		if( ChkIfFubon() )
		{
			glSendPack.szEntryMode[2] = '1';
		}
		else if( ChkIfBoc())
		{
			if( pstLog->szPan[0]=='4' )
			{
				memcpy(&glSendPack.szEntryMode[1], "90", 2);
			}
			else if( pstLog->szPan[0]=='5' )
			{
				memcpy(&glSendPack.szEntryMode[1], "80", 2);
			}
			else if ( memcmp(pstLog->szPan, "35", 2)==0 )
			{
				memcpy(&glSendPack.szEntryMode[1], "97", 2);	// "971"
			}
		}
	}
	else if( pstLog->uiEntryMode & MODE_FALLBACK_MANUAL )
	{
	}

	//------------------------------------------------------------------------------
	// Entry mode digit 4
#ifdef ENABLE_EMV
	if( ChkAcqOption(ACQ_EMV_FEATURE) )
	{
		glSendPack.szEntryMode[3] = '1';    // default : support offline-PIN
	}
#endif
}

// set bit 25
void SetCondCode(void)
{
	if( ChkIfAmex() )
	{
		// condition code==06: Preauth, Auth, SaleComplete, sale below floor
		sprintf_string((char *)glProcInfo.stTranLog.szCondCode, "00");
		if( (glProcInfo.stTranLog.ucTranType==PREAUTH) || (glProcInfo.stTranLog.ucTranType==AUTH) )
		{
			sprintf_string((char *)glProcInfo.stTranLog.szCondCode, "06");
		}
		if( glProcInfo.stTranLog.ucTranType==OFFLINE_SEND )
		{
			if( !(glProcInfo.stTranLog.uiStatus & (TS_ADJ|TS_VOID)) &&
				 (glProcInfo.stTranLog.ucOrgTranType==SALE_COMP || glProcInfo.stTranLog.ucOrgTranType==SALE) )
			{
				sprintf_string((char *)glProcInfo.stTranLog.szCondCode, "06");
			}
		}
	}
	else
	{
		sprintf_string((char *)glProcInfo.stTranLog.szCondCode, "00");
		if( glProcInfo.stTranLog.ucTranType==PREAUTH )
		{
			sprintf_string((char *)glProcInfo.stTranLog.szCondCode, "06");
		}
		else if( glProcInfo.stTranLog.ucTranType==VOID || glProcInfo.stTranLog.ucTranType==OFFLINE_SEND )
		{
			if( glProcInfo.stTranLog.ucOrgTranType==SALE_COMP )
			{
				sprintf_string((char *)glProcInfo.stTranLog.szCondCode, "06");
			}
		}
	}

	sprintf_string((char *)glSendPack.szCondCode, "%.2s", glProcInfo.stTranLog.szCondCode);
}

// RFU for HK (bit 48, 63)
void SetInstAndAirTicketData(void)
{
	uchar	sBuff[32];

	if (ChkIfBea())
	{
		if (glProcInfo.stTranLog.ucInstalment!=0)
		{
			memcpy(glSendPack.sField63, "\x00\x02", 2);
			PubLong2Bcd((ulong)glProcInfo.stTranLog.ucInstalment, 1, sBuff);
			PubBcd2Asc0(sBuff, 1, glSendPack.sField63+2);
		}
	}
}

// 交易初始化:检查交易是否允许,显示交易标题
// initiate transaction, check allowance, display title. 
int TransInit(uchar ucTranType)
{
	glProcInfo.stTranLog.ucTranType = ucTranType;
	SetCurrTitle(_T(glTranConfig[glProcInfo.stTranLog.ucTranType].szLabel));

	if( !ChkIfTranAllow(ucTranType) )
	{
		return ERR_NO_DISP;
	}

	return 0;
}

// Modified by Kim_LinHB 2014-8-8 v1.01.0002 bug506
int TransCapture(void)
{
	int	iRet;

	if( glProcInfo.stTranLog.ucTranType==CASH )
	{
		TransInit(glProcInfo.stTranLog.ucTranType);
		iRet = TransCashSub();
	}
	else if ( glProcInfo.stTranLog.ucTranType==SALE ||
		glProcInfo.stTranLog.ucTranType==INSTALMENT )
	{
		TransInit(glProcInfo.stTranLog.ucTranType);
		iRet = TransSaleSub();
	}
	else
	{
		if( ChkEdcOption(EDC_AUTH_PREAUTH) )
		{
			TransInit(AUTH);
			iRet = TransAuthSub(AUTH);
		}
		else
		{
			TransInit(PREAUTH);
			iRet = TransAuthSub(PREAUTH);
		}
	}

	return iRet;
}

// Modified by Kim_LinHB 2014-7-11
int InstallmentMenu(void)
{
	int		iRet, iMenuNo;
	GUI_MENU stInstMenu;
	GUI_MENUITEM stInstMenuItem[] =
	{
		{ _T_NOOP("INSTAL SALE "), 0, TRUE,  NULL},
		{ _T_NOOP("INSTAL VOID "), 1, TRUE,  NULL},
		{ "", -1, FALSE,  NULL},
	};

	Gui_BindMenu(GetCurrTitle(), gl_stCenterAttr, gl_stLeftAttr, (GUI_MENUITEM *)stInstMenuItem, &stInstMenu);
	Gui_ClearScr();
	iMenuNo = 0;
	iRet = Gui_ShowMenuList(&stInstMenu, GUI_MENU_DIRECT_RETURN, USER_OPER_TIMEOUT, &iMenuNo);

	if(GUI_OK == iRet)
	{
		switch( iMenuNo )
		{
		case 0:
			iRet = TransSale(TRUE);
			break;

		case 1:
			iRet = TransVoid();
			break;

		default:
			return ERR_NO_DISP;
		}
	}
	else
	{
		return ERR_NO_DISP;
	}

	CommOnHook(FALSE);
	return iRet;
}

int TransCash(void)
{
	int		iRet;

	iRet = TransInit(CASH);
	if( iRet!=0 )
	{
		return iRet;
	}


	iRet = GetCard(CARD_INSERTED|CARD_SWIPED|CARD_KEYIN);
	if( iRet!=0 )
	{
		return iRet;
	}

	return TransSaleSub();
}

int TransCashSub(void)
{
	int		iRet;

	if( !ChkIfTranAllow(glProcInfo.stTranLog.ucTranType) )
	{
		return ERR_NO_DISP;
	}
	if( !ChkSettle() )
	{
		return ERR_NO_DISP;
	}

	iRet = GetAmount();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetDescriptor();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetAddlPrompt();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetPIN(FALSE);
	if( iRet!=0 )
	{
		return iRet;
	}

	SetCommReqField();
	iRet = TranProcess();
	if( iRet!=ERR_NEED_FALLBACK )
	{
		return iRet;
	}

	// continue fallback process
	glProcInfo.bIsFallBack = TRUE;
	glProcInfo.stTranLog.uiEntryMode &= 0xF0;

	iRet = GetCard(FALLBACK_SWIPE|CARD_SWIPED);
	if( iRet!=0 )
	{
		return iRet;
	}

	SetCommReqField();
	return TranProcess();
}

// 普通消费、分期消费
// sale or installment
int TransSale(uchar ucInstallment)
{
	int		iRet;
	uchar	ucEntryMode;

	iRet = TransInit((uchar)(ucInstallment ? INSTALMENT : SALE));
	if( iRet!=0 )
	{
		return iRet;
	}

	ucEntryMode = CARD_SWIPED|CARD_KEYIN;
	if (!ucInstallment)
	{
		ucEntryMode |= CARD_INSERTED;
	}

	iRet = GetCard(ucEntryMode);
	if( iRet!=0 )
	{
		return iRet;
	}

	return TransSaleSub();
}

// 负责SALE和INSTALLMENT
// for sale and installment
int TransSaleSub(void)
{
	int		iRet;

	if( !ChkIfTranAllow(glProcInfo.stTranLog.ucTranType) )
	{
		return ERR_NO_DISP;
	}

	// instalment时，仅当选择plan之后才最终确认ACQ，因此现在不需要检查settle状态
	// when doing installment, ACQ will be confirmed after selecting plan, so don't need to check if need to settle now.
	if (glProcInfo.stTranLog.ucTranType!=INSTALMENT)
	{
		if( !ChkSettle() )
		{
			return ERR_NO_DISP;
		}
	}

	iRet = GetAmount();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetInstalPlan();
	if( iRet!=0 )
	{
		return iRet;
	}

	// instalment时，仅当选择plan之后才最终确认ACQ
	// when doing installment, ACQ is confirmed after selecting plan
	if (glProcInfo.stTranLog.ucTranType==INSTALMENT)
	{
		if( !ChkSettle() )
		{
			return ERR_NO_DISP;
		}
	}

	iRet = GetDescriptor();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetAddlPrompt();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetPIN(FALSE);
	if( iRet!=0 )
	{
		return iRet;
	}

	if( ChkIfBelowMagFloor() && !glProcInfo.bExpiryError && !ChkIfIccTran(glProcInfo.stTranLog.uiEntryMode) )
	{
//		sprintf_string((char *)glProcInfo.stTranLog.szCondCode, "06");
		sprintf_string((char *)glProcInfo.stTranLog.szAuthCode, "%02ld", glSysCtrl.ulSTAN % 100);
		glProcInfo.stTranLog.uiStatus |= TS_CHANGE_APPV|TS_FLOOR_LIMIT;
		return FinishOffLine();
	}

	SetCommReqField();
	iRet = TranProcess();
	if( iRet!=ERR_NEED_FALLBACK )
	{
		return iRet;
	}

	// continue fallback process
	glProcInfo.bIsFallBack = TRUE;
	glProcInfo.stTranLog.uiEntryMode &= 0xF0;

	iRet = GetCard(FALLBACK_SWIPE|CARD_SWIPED);
	if( iRet!=0 )
	{
		return iRet;
	}

	SetCommReqField();
	return TranProcess();
}

// 授权/预授权交易
// authorization / pre-authorization
int TransAuth(uchar ucTranType)
{
	int		iRet;

	PubASSERT(ucTranType==AUTH || ucTranType==PREAUTH);

	iRet = TransInit(ucTranType);
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetCard(CARD_INSERTED|CARD_SWIPED|CARD_KEYIN);
	if( iRet!=0 )
	{
		return iRet;
	}

	return TransAuthSub(ucTranType);
}

int TransAuthSub(uchar ucTranType)
{
	int		iRet;

	if( !ChkIfTranAllow(ucTranType) )
	{
		return ERR_NO_DISP;
	}
	if( !ChkSettle() )
	{
		return ERR_NO_DISP;
	}

	if( ChkIssuerOption(ISSUER_NO_PREAUTH) )
	{
		DispBlockFunc();
		return ERR_NO_DISP;
	}

	iRet = GetAmount();
	if( iRet!=0 )
	{
		return iRet;
	}

	if( !ChkIfAmex() )
	{
		iRet = GetDescriptor();
		if( iRet!=0 )
		{
			return iRet;
		}

		iRet = GetAddlPrompt();
		if( iRet!=0 )
		{
			return iRet;
		}
	}

	iRet = GetPIN(FALSE);
	if( iRet!=0 )
	{
		return iRet;
	}

	SetCommReqField();
	iRet = TranProcess();
	if( iRet!=ERR_NEED_FALLBACK )
	{
		return iRet;
	}

	// continue fallback process
	glProcInfo.bIsFallBack = TRUE;
	glProcInfo.stTranLog.uiEntryMode &= 0xF0;

	iRet = GetCard(FALLBACK_SWIPE|CARD_SWIPED);
	if( iRet!=0 )
	{
		return iRet;
	}

	SetCommReqField();
	return TranProcess();
}

int FinishOffLine(void)
{
	uchar	ucTranAct;

	SetOffBase(OffBaseDisplay);

	DispProcess();

	if( !(glProcInfo.stTranLog.uiEntryMode & MODE_CHIP_INPUT) &&
		(glProcInfo.stTranLog.ucTranType!=SALE_COMP) )
	{
		sprintf_string((char *)glProcInfo.stTranLog.szRspCode, "00");
	}
	glProcInfo.stTranLog.ulInvoiceNo = glSysCtrl.ulInvoiceNo;
	ucTranAct = glTranConfig[glProcInfo.stTranLog.ucTranType].ucTranAct;

	if (ucTranAct & ACT_INC_TRACE)
	{
		GetNewTraceNo();
	}

	DoE_Signature();

	if( ucTranAct & WRT_RECORD )
	{
		glProcInfo.stTranLog.uiStatus |= TS_NOSEND;
		SaveTranLog(&glProcInfo.stTranLog);
	}

	if( ucTranAct & PRN_RECEIPT )	// print slip
	{
		CommOnHook(FALSE);
		GetNewInvoiceNo();
		PrintReceipt(PRN_NORMAL);
	}

	DispResult(0);
	// PubWaitKey(glSysParam.stEdcInfo.ucAcceptTimeout); // Hidden by Kim_LinHB 2014/9/11 v1.01.0008 bug523

	return 0;
}

int TranReversal(void)
{
#ifdef ENABLE_EMV
	int	iLength;
#endif
	int	iRet;
	SYS_PROC_INFO	stProcInfoBak;

	if( glProcInfo.stTranLog.ucTranType==LOAD_PARA ||
		glProcInfo.stTranLog.ucTranType==ECHO_TEST ||
		glProcInfo.stTranLog.ucTranType==LOAD_CARD_BIN )
	{
		return 0;
	}

	if( !glSysCtrl.stRevInfo[glCurAcq.ucIndex].bNeedReversal )
	{
		return 0;
	}

	// backup current process information
	memcpy(&glProcInfo.stSendPack, &glSendPack, sizeof(STISO8583));
	memcpy(&stProcInfoBak, &glProcInfo, sizeof(SYS_PROC_INFO));
	// Modified by Kim_LinHB 2014-8-8 v1.01.0002 bug506
	//glProcInfo.stTranLog.ucTranType = REVERSAL;
	TransInit(REVERSAL);

	memcpy(&glSendPack, &glSysCtrl.stRevInfo[glCurAcq.ucIndex].stRevPack, sizeof(STISO8583));
	sprintf_string((char *)glSendPack.szMsgCode, "0400");
	if( ChkIfBoc() )  // Boc erase F55
	{
		memset(glSendPack.sICCData, 0, 2);
	}
	if( ChkIfAmex() )
	{
		memset(glSendPack.sICCData, 0, 2);
		memset(glSendPack.szLocalDate, 0, sizeof(glSendPack.szLocalDate));
		memset(glSendPack.szLocalTime, 0, sizeof(glSendPack.szLocalTime));
		memset(glSendPack.szRRN,       0, sizeof(glSendPack.szRRN));
		memset(glSendPack.szAuthCode,  0, sizeof(glSendPack.szAuthCode));
	}
	memset(glSendPack.sPINData, 0, sizeof(glSendPack.sPINData));	// erase PIN block

#ifdef ENABLE_EMV
	if( (glSysCtrl.stRevInfo[glCurAcq.ucIndex].uiEntryMode & MODE_CHIP_INPUT) &&
		ChkIfAcqNeedDE56() )
	{
		iLength = glSysCtrl.stField56[glCurAcq.ucIndex].uiLength;
		if( iLength>0 )
		{
			memcpy(&glSendPack.sICCData2[2], glSysCtrl.stField56[glCurAcq.ucIndex].sData, iLength);
		}
		else
		{
			SetStdEmptyDE56(&glSendPack.sICCData2[2], &iLength);
		}
		PubLong2Char((ulong)iLength, 2, glSendPack.sICCData2);
	}
	if( ChkIfDah() || ChkIfCiti() )
	{
		memset(glSendPack.sICCData2, 0, 2);
	}
	if( (glSysCtrl.stRevInfo[glCurAcq.ucIndex].uiEntryMode & MODE_FALLBACK_SWIPE) ||
		(glSysCtrl.stRevInfo[glCurAcq.ucIndex].uiEntryMode & MODE_FALLBACK_MANUAL) )
	{
		if (ChkIfBoc())
		{
			memset(glSendPack.sICCData2, 0, 2);
		}
	}
#endif

	if( ChkIfBoc() )
	{
		memset(glSendPack.szLocalDate, 0, sizeof(glSendPack.szLocalDate));
		memset(glSendPack.szLocalTime, 0, sizeof(glSendPack.szLocalTime));
		memset(glSendPack.szRRN, 0, sizeof(glSendPack.szRRN));
		memset(glSendPack.szAuthCode, 0, sizeof(glSendPack.szAuthCode));
	}

	while( 1 )
	{
		iRet = SendRecvPacket();
		if( iRet!=0 )
		{
			break;
		}
		if( memcmp(glRecvPack.szRspCode, "00", 2)==0 )
		{
			break;
		}
		if( ChkIfAmex() && (memcmp(glRecvPack.szRspCode, "08", 2)==0 || memcmp(glRecvPack.szRspCode, "88", 2)==0) )
		{
			break;
		}

		sprintf_string((char *)glProcInfo.stTranLog.szRspCode, "%.2s", glRecvPack.szRspCode);
		DispResult(ERR_HOST_REJ);
// 		iRet = ERR_NO_DISP;
		iRet = ERR_TRAN_FAIL;
		break;
	}
	if( iRet==0 )
	{	// clear reversal flag
		SaveRevInfo(FALSE);
	}

	if (iRet==0)
	{
		// increase invoice for coming AMEX transaction
		if (ChkIfAmex())
		{
			if (glTranConfig[stProcInfoBak.stTranLog.ucTranType].ucTranAct & PRN_RECEIPT)
			{
				stProcInfoBak.stTranLog.ulInvoiceNo = GetNewInvoiceNo();
				PackInvoice(&stProcInfoBak.stSendPack, stProcInfoBak.stTranLog.ulInvoiceNo);
			}
		}
	}

	// restore process information
	memcpy(&glProcInfo, &stProcInfoBak, sizeof(SYS_PROC_INFO));
	memcpy(&glSendPack, &glProcInfo.stSendPack, sizeof(STISO8583));

	TransInit(glProcInfo.stTranLog.ucTranType); // Added by Kim_LinHB 2014-8-8 v1.01.0002

	return iRet;
}

int GetOfflineTrans(uchar ucTypeFlag)
{
	int		iRet;
	ushort	uiCnt;

	for(uiCnt=0; uiCnt<MAX_TRANLOG; uiCnt++)
	{
		if( glSysCtrl.sAcqKeyList[uiCnt]!=glCurAcq.ucKey )
		{
			continue;
		}

		memset(&glProcInfo.stTranLog, 0, sizeof(TRAN_LOG));
		iRet = LoadTranLog(&glProcInfo.stTranLog, uiCnt);
		if( iRet!=0 )
		{
			return FALSE;
		}

		if ( (ucTypeFlag & OFFSEND_TC) &&
			 (glProcInfo.stTranLog.uiStatus & TS_NEED_TC) )
		{
			glProcInfo.uiRecNo = uiCnt;
			return TRUE;
		}
		else if ( (ucTypeFlag & OFFSEND_TRAN) &&
			 (glProcInfo.stTranLog.uiStatus & TS_NOSEND) )
		{
			glProcInfo.uiRecNo = uiCnt;
			return TRUE;
		}
	}

	return FALSE;
}

int OfflineSend(uchar ucTypeFlag)
{
#ifdef ENABLE_EMV
	int iLength;
#endif
	int	iRet;
	SYS_PROC_INFO	stProcInfoBak;

	if( glProcInfo.stTranLog.ucTranType!=SETTLEMENT )
	{	// the time to load 400 txn log is about 1-2 seconds
		if( ChkAcqOption(ACQ_DISABLE_TRICK_FEED) )
		{
			return 0;
		}
	}

	memcpy(&glProcInfo.stSendPack, &glSendPack, sizeof(STISO8583));
	memcpy(&stProcInfoBak, &glProcInfo,  sizeof(SYS_PROC_INFO));
	while( 1 )
	{
		InitTransInfo();
		if( !GetOfflineTrans(ucTypeFlag) )
		{
			iRet = 0;
			break;
		}

		if (glProcInfo.stTranLog.uiStatus & TS_NEED_TC)
		{
			glProcInfo.stTranLog.ucOrgTranType = glProcInfo.stTranLog.ucTranType;
			glProcInfo.stTranLog.ucTranType    = TC_SEND;

			SetCommReqField();

			if (0)
			{
				sprintf_string((char *)glSendPack.szMsgCode, "0220");
			}

			// bit39
			//sprintf_string((char *)glSendPack.szRspCode, "%.2s", glProcInfo.stTranLog.szRspCode);

#ifdef ENABLE_EMV
			// bit55
			//SetDE55(DE55_TC, glSendPack.sICCData+2, &iLength);
            SetTCDE55(&glProcInfo.stTranLog, glSendPack.sICCData+2, &iLength);
			PubLong2Char((ulong)iLength, 2, glSendPack.sICCData);
#endif

			iRet = SendRecvPacket();
			if( iRet!=0 )
			{
				break;
			}

			// update txn status
			glProcInfo.stTranLog.uiStatus &= ~TS_NEED_TC;

			// Modified by Kim_LinHB 2014-8-8 v1.01.0002 bug506
			// glProcInfo.stTranLog.ucTranType = glProcInfo.stTranLog.ucOrgTranType;
			TransInit(glProcInfo.stTranLog.ucOrgTranType);

			UpdateTranLog(&glProcInfo.stTranLog, glProcInfo.uiRecNo);

			if( stProcInfoBak.stTranLog.ucTranType!=SETTLEMENT )
			{
				break;
			}

			continue;
		} 

		glProcInfo.stTranLog.ucOrgTranType = glProcInfo.stTranLog.ucTranType;
		glProcInfo.stTranLog.ucTranType    = OFFLINE_SEND;

		SetCommReqField();

		// bit 4
		if( glProcInfo.stTranLog.uiStatus & TS_VOID )
		{
			glSendPack.szTranAmt[0] = 0;
		}

		// bit 37, 39
		if( !ChkIfAmex() )
		{
			if( glProcInfo.stTranLog.uiEntryMode & MODE_CHIP_INPUT )
			{
				sprintf_string((char *)glSendPack.szRspCode, "%.2s", glProcInfo.stTranLog.szRspCode);
				if( glProcInfo.stTranLog.szRRN[0]!=0 )	// seen by host
				{
					glSendPack.szRspCode[0] = 0;
				}
			}
		}
		if( ChkAcqOption(ACQ_DBS_FEATURE) )
		{	// 香港星展银行,sale_complete上送时, VISA/MASTER需要上送 BIT37 BIT39
			if( glProcInfo.stTranLog.ucOrgTranType==SALE_COMP &&
				glProcInfo.stTranLog.szRRN[0]==0 )
			{
				sprintf_string((char *)glSendPack.szRRN, "%.12s", &glProcInfo.stTranLog.szRRN[1]);
			}
		}

		if( ChkIfAmex() && (glProcInfo.stTranLog.uiStatus & TS_FLOOR_LIMIT) )
		{
			if( !(glProcInfo.stTranLog.uiStatus & (TS_ADJ|TS_VOID)) )
			{
				sprintf_string((char *)glSendPack.szAddlRsp, "03");
			}
		}

		// bit 55
		if( glProcInfo.stTranLog.uiEntryMode & MODE_CHIP_INPUT )
		{
			if( ChkIfAmex() )
			{
				if( !(glProcInfo.stTranLog.uiStatus & TS_ADJ) &&
					((glProcInfo.stTranLog.ucOrgTranType==OFF_SALE) ||
					 (glProcInfo.stTranLog.ucOrgTranType==SALE_COMP) ||
					 (glProcInfo.stTranLog.ucOrgTranType==SALE && (glProcInfo.stTranLog.uiStatus & TS_NOSEND))) )
				{
					PubLong2Char((ulong)glProcInfo.stTranLog.uiIccDataLen, 2, glSendPack.sICCData);
					memcpy(&glSendPack.sICCData[2], glProcInfo.stTranLog.sIccData, glProcInfo.stTranLog.uiIccDataLen);
				}
			}
			else
			{	// only send ICC sale below floor
				if( glProcInfo.stTranLog.szRRN[0]==0 )
				{
					PubLong2Char((ulong)glProcInfo.stTranLog.uiIccDataLen, 2, glSendPack.sICCData);
					memcpy(&glSendPack.sICCData[2], glProcInfo.stTranLog.sIccData, glProcInfo.stTranLog.uiIccDataLen);
				}
			}
		}

		// bit 60
		if( !ChkIfAmex() && (glProcInfo.stTranLog.uiStatus & (TS_ADJ|TS_VOID)) )
		{
			if( glProcInfo.stTranLog.uiStatus & TS_VOID )
			{
				sprintf_string((char *)glSendPack.szField60, "%.12s", glProcInfo.stTranLog.szAmount);
			}
			else
			{
				sprintf_string((char *)glSendPack.szField60, "%.12s", glProcInfo.stTranLog.szOrgAmount);
			}
		}

		iRet = SendRecvPacket();
		if( iRet!=0 )
		{
			break;
		}

		// update txn status
		glProcInfo.stTranLog.uiStatus &= ~TS_NOSEND;
		glProcInfo.stTranLog.uiStatus |= TS_OFFLINE_SEND;

		// Modified by Kim_LinHB 2014-8-8 v1.01.0002 bug506
		//glProcInfo.stTranLog.ucTranType = glProcInfo.stTranLog.ucOrgTranType;
		TransInit(glProcInfo.stTranLog.ucOrgTranType);

		UpdateTranLog(&glProcInfo.stTranLog, glProcInfo.uiRecNo);

		if( stProcInfoBak.stTranLog.ucTranType!=SETTLEMENT )
		{	// is trickle feed, only need send one txn
			break;
		}
	}
	memcpy(&glProcInfo, &stProcInfoBak, sizeof(SYS_PROC_INFO));
	memcpy(&glSendPack, &glProcInfo.stSendPack, sizeof(STISO8583));

	return iRet;
}

// transaction complete for voice referral
int TranSaleComplete(void)
{
#ifdef ENABLE_EMV
	int		iLength;
#endif
	int		iRet;
	unsigned char szBuff[100];

	if( !ChkIssuerOption(ISSUER_EN_VOICE_REFERRAL) )
	{
		return ERR_TRAN_FAIL;
	}

	if( (glProcInfo.stTranLog.ucTranType!=PREAUTH) &&
		(glProcInfo.stTranLog.ucTranType!=AUTH) &&
		(glProcInfo.stTranLog.ucTranType!=SALE) &&
		(glProcInfo.stTranLog.ucTranType!=INSTALMENT) &&
		(glProcInfo.stTranLog.ucTranType!=CASH) )
	{
		return ERR_TRAN_FAIL;
	}

	if( glProcInfo.stTranLog.ucTranType>=SALE )
	{
		TransInit(SALE_COMP);
	}

	CommOnHook(FALSE);
	// Modified by Kim_LinHB 2014-7-11	
	sprintf_string(szBuff, "CALL:%-11.11s", glCurIssuer.szRefTelNo);
	SetCurrTitle(szBuff);
	Gui_ClearScr();
	
	if( ChkEdcOption(EDC_REFERRAL_DIAL) )
	{
		Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("PLS CALL BANK"), gl_stCenterAttr, GUI_BUTTON_NONE, 0, NULL);
		iRet = ReferralDial(glCurIssuer.szRefTelNo);
		if( iRet!=0 )
		{
			return iRet;
		}
	}
	else
	{
		if(GUI_OK != Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("PLS CALL BANK"), gl_stCenterAttr, GUI_BUTTON_YandN, USER_OPER_TIMEOUT, NULL))
		{   
			Gui_ClearScr();
			Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("TRANS CANCELED"), gl_stCenterAttr, GUI_BUTTON_OK, 3, NULL);
			return ERR_NO_DISP;
		}
	}

	iRet = GetPreAuthCode();
	if( iRet!=0 )
	{
		Gui_ClearScr();
		Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("TRANS CANCELED"), gl_stCenterAttr, GUI_BUTTON_OK, 3, NULL);
		return ERR_NO_DISP;
	}

	// 香港星展银行,sale_complete上送时, VISA/MASTER需要上送 BIT37 BIT39
	// For DBS in HongKong.
	memmove(&glProcInfo.stTranLog.szRRN[1], glProcInfo.stTranLog.szRRN, 12);
	glProcInfo.stTranLog.szRRN[0] = 0;
//	sprintf_string((char *)glProcInfo.stTranLog.szCondCode, "06");

#ifdef ENABLE_EMV
	if( (glProcInfo.stTranLog.uiEntryMode & MODE_CHIP_INPUT) &&
		ChkIfAcqNeedDE56() )
	{
		SetDE56(glProcInfo.stTranLog.sField56, &iLength);
		glProcInfo.stTranLog.uiField56Len = (ushort)iLength;
	}
#endif

	return FinishOffLine();
}

// 退货
//refund
int TransRefund(void)
{
	int		iRet;

	iRet = TransInit(REFUND);
	if( iRet!=0 )
	{
		return iRet;
	}

	if( !ChkEdcOption(EDC_NOT_REFUND_PWD) )
	{
		if( PasswordRefund()!=0 )
		{
			return ERR_NO_DISP;
		}
	}

	iRet = GetCard(CARD_SWIPED|CARD_KEYIN);
	if( iRet!=0 )
	{
		return iRet;
	}

	if( !ChkSettle() )
	{
		return ERR_NO_DISP;
	}

	if( ChkIssuerOption(ISSUER_NO_REFUND) )
	{
		DispBlockFunc();
		return ERR_NO_DISP;
	}

	iRet = GetAmount();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetDescriptor();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetAddlPrompt();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetPIN(FALSE);
	if( iRet!=0 )
	{
		return iRet;
	}

	if( ChkAcqOption(ACQ_ONLINE_REFUND) )
	{
		SetCommReqField();
		return TranProcess();
	}
	else
	{
		return FinishOffLine();
	}
}

// 离线
// offline sale
int TransOffSale(void)
{
	int		iRet;

	iRet = TransInit(OFF_SALE);
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetCard(SKIP_CHECK_ICC|CARD_SWIPED|CARD_KEYIN);
	if( iRet!=0 )
	{
		return iRet;
	}

	if( !ChkSettle() )
	{
		return ERR_NO_DISP;
	}

	if( !ChkIssuerOption(ISSUER_EN_OFFLINE) )
	{
		DispBlockFunc();
		return ERR_NO_DISP;
	}

	iRet = GetAmount();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetDescriptor();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetAddlPrompt();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetPreAuthCode();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetPIN(FALSE);
	if( iRet!=0 )
	{
		return iRet;
	}

	return FinishOffLine();
}

int TransVoid(void)
{
	int			iRet;
	uchar		ucTranAct, bOnlineFlag, szTempAmt[12+1];
	uchar		szDispBuff[50];
	GUI_TEXT_ATTR stCenter = gl_stCenterAttr;

	iRet = TransInit(VOID);
	if( iRet!=0 )
	{
		return iRet;
	}

	if( !ChkEdcOption(EDC_NOT_VOID_PWD) )
	{
		if( PasswordVoid()!=0 )
		{
			return ERR_NO_DISP;
		}
	}

	while( 1 )
	{
		iRet = GetRecord(TS_OK|TS_NOSEND|TS_ADJ, &glProcInfo.stTranLog);
		if( iRet!=0 )
		{
			return iRet;
		}

		ucTranAct = glTranConfig[glProcInfo.stTranLog.ucTranType].ucTranAct;
		if( ucTranAct & VOID_ALLOW )
		{
			glProcInfo.stTranLog.ucOrgTranType = glProcInfo.stTranLog.ucTranType;
			glProcInfo.stTranLog.ucTranType    = VOID;
			break;
		}

		Gui_ClearScr();
		PubBeepErr();
		Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("NOT ALLOW VOID"), gl_stCenterAttr, GUI_BUTTON_CANCEL, 3, NULL);
	}
	FindAcq(glProcInfo.stTranLog.ucAcqKey);
	FindIssuer(glProcInfo.stTranLog.ucIssuerKey);

	if( ChkIssuerOption(ISSUER_NO_VOID) )
	{
		DispBlockFunc();
		return ERR_NO_DISP;
	}

	if( !ChkSettle() )
	{
		return ERR_NO_DISP;
	}

	if( ChkAcqOption(ACQ_ONLINE_VOID) )
	{
		DispWait();
		PreDial();
	}

	PubAscAdd(glProcInfo.stTranLog.szAmount, glProcInfo.stTranLog.szTipAmount, 12, szTempAmt);
	GetDispAmount(szTempAmt, szDispBuff);	// show total amount
	strcat(szDispBuff, "\n");
	strcat(szDispBuff, _T("VOID ?"));
#ifndef _Dxxx_
	stCenter.eFontSize = GUI_FONT_SMALL;  // Added by Kim_LinHB 2014-08-26 v1.01.0005 bug509
#endif

	Gui_ClearScr();
	if(GUI_OK != Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, szDispBuff, stCenter, GUI_BUTTON_YandN, USER_OPER_TIMEOUT, NULL))
	{
		return ERR_USERCANCEL;
	}

 	bOnlineFlag = TRUE;
  	if( glProcInfo.stTranLog.uiStatus & TS_NOSEND )
 	{	// offsale/sale below/sale comp.../adjust
 		if( glProcInfo.stTranLog.uiStatus & TS_ADJ )
 		{	// 该调整还未上送,作废调整后的金额
			PubAscSub(glProcInfo.stTranLog.szOrgAmount, glProcInfo.stTranLog.szAmount, 12, szTempAmt);
			//PubAddHeadChars(szTempAmt, 12, '0');		no need: already 12 digits
			sprintf_string((char *)glProcInfo.stTranLog.szTipAmount, "%.12s", szTempAmt);
 		}
		if( glProcInfo.stTranLog.szRRN[0]==0 )
		{
			bOnlineFlag = FALSE;
		}
 	}

 	if( !ChkAcqOption(ACQ_ONLINE_VOID) )
	{
		bOnlineFlag = FALSE;
	}

	if( bOnlineFlag )
	{
		SetCommReqField();
		return TranProcess();
	}
	else	// offline void
	{
		glProcInfo.stTranLog.uiStatus |= (TS_VOID|TS_NOSEND);
		if( glProcInfo.stTranLog.szRRN[0]==0 )
		{	// not seen by host(orginal txn is offsale/sale below/salecomp/ ...)
			glProcInfo.stTranLog.uiStatus &= ~(TS_NOSEND);
		}
		GetDateTime(glProcInfo.stTranLog.szDateTime);
		DoE_Signature();
		UpdateTranLog(&glProcInfo.stTranLog, glProcInfo.uiRecNo);
		PrintReceipt(PRN_NORMAL);
		DispResult(0);

		return 0;
	}
}

// Modified by Kim_LinHB 2014-7-11
int TransOther(void)
{
	int		iRet, iMenuNo;
	uchar	ucTranType;
	GUI_MENU stTranMenu;
	GUI_MENUITEM stDefTranMenuItem[] =
	{
#ifdef ENABLE_CONTLESS
		{ _T_NOOP("CLSS SALE"), 0,TRUE,  NULL},
#else
		{ _T_NOOP("CLSS SALE"), 0,FALSE,  NULL},
#endif
		{ _T_NOOP("VOID"), 1,TRUE,  NULL},
		{ _T_NOOP("OFFLINE"), 2,TRUE,  NULL},
		{ _T_NOOP("REFUND"), 3,TRUE,  NULL},
		{ _T_NOOP("ADJUST"), 4,TRUE,  NULL},
		{ _T_NOOP("SETTLE"), 5,TRUE,  NULL},
		{ "", 6,TRUE,  NULL},	// reserved for auth/preauth
		{ _T_NOOP("INSTALLMENT"), 7,TRUE,  NULL},
		{ "", -1,FALSE,  NULL},
	};
	GUI_MENUITEM stTranMenuItem[20];
	static	uchar	szPreAuthTitle[] = _T_NOOP("PREAUTH");
	static	uchar	szAuthTitle[]    = _T_NOOP("AUTH");
	int iMenuItemNum = 0;
	int i;
	
	if( ChkEdcOption(EDC_AUTH_PREAUTH) )
	{
		ucTranType = AUTH;
		sprintf_string((char *)stDefTranMenuItem[6].szText, "%s", szAuthTitle);
	}
	else
	{
		ucTranType = PREAUTH;
		sprintf_string((char *)stDefTranMenuItem[6].szText, "%s", szPreAuthTitle);
	}

    for(i = 0; i < sizeof(stDefTranMenuItem)/sizeof(GUI_MENUITEM); ++i)
    {
        if (7 == i && ((glSysParam.ucPlanNum==0) || !ChkEdcOption(EDC_ENABLE_INSTALMENT)))
            continue;
        if(stDefTranMenuItem[i].bVisible)
        {
            memcpy(&stTranMenuItem[iMenuItemNum], &stDefTranMenuItem[i], sizeof(GUI_MENUITEM));
            sprintf_string(stTranMenuItem[iMenuItemNum].szText, "%s", stDefTranMenuItem[i].szText);
            ++iMenuItemNum;
        }
    }
    stTranMenuItem[iMenuItemNum].szText[0] = 0;

	Gui_BindMenu("TRANSACTION", gl_stCenterAttr, gl_stLeftAttr, (GUI_MENUITEM *)stTranMenuItem, &stTranMenu);
	
	Gui_ClearScr();
	iMenuNo = 0;
	iRet = Gui_ShowMenuList(&stTranMenu, GUI_MENU_DIRECT_RETURN, USER_OPER_TIMEOUT, &iMenuNo);
	if(GUI_OK == iRet)
	{
		switch( iMenuNo )
		{
#ifdef ENABLE_CONTLESS
		case 0:
//		    PiccLightOpen();
//		    Gui_RegCallback(GUI_CALLBACK_LISTEN_EVENT, ClssVirtualLight);
			iRet = TransClssSale();
//			Gui_RegCallback(GUI_CALLBACK_LISTEN_EVENT, NULL);
//			PiccLightClose();
			break;
#endif
		case 1:
			iRet = TransVoid();
			break;

		case 2:
			iRet = TransOffSale();
			break;

		case 3:
			iRet = TransRefund();
			break;

		case 4:
			TransAdjust();
			iRet = 0;
			break;

		case 5:
			iRet = TransSettle();
			break;

		case 6:
			iRet = TransAuth(ucTranType);
			break;

		case 7:
			iRet = InstallmentMenu();

		default:
			return ERR_NO_DISP;
		}

		CommOnHook(FALSE);
		Gui_ClearScr(); // Added by Kim_LinHB 9/9/2014 v1.01.0007 bug515

		return iRet;
	}
	return ERR_NO_DISP;
}

// 调整交易输入
// adjust transaction amount
// Modified by Kim_LinHB 2014-7-11
int AdjustInput(void)
{
	int		iRet;

	SetCurrTitle(_T("ADJUST"));

	while( 1 )
	{
		InitTransInfo();
		iRet = GetRecord(TS_NOSEND|TS_ADJ, &glProcInfo.stTranLog);
		if( iRet!=0 )
		{
			return iRet;
		}

		if( ChkIfZeroAmt(glProcInfo.stTranLog.szAmount) &&
			ChkIfZeroAmt(glProcInfo.stTranLog.szTipAmount) )
		{
			Gui_ClearScr();
			PubBeepErr();
			Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("DO NOT ADJUST"), gl_stCenterAttr, GUI_BUTTON_CANCEL, 3, NULL);
			continue;
		}

		if( glProcInfo.stTranLog.ucTranType==REFUND && ChkIfAmex() )
		{
			Gui_ClearScr();
			PubBeepErr();
			Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("DO NOT ADJUST"), gl_stCenterAttr, GUI_BUTTON_CANCEL, 3, NULL);
			continue;
		}

		FindAcq(glProcInfo.stTranLog.ucAcqKey);
		FindIssuer(glProcInfo.stTranLog.ucIssuerKey);
		if( !ChkIssuerOption(ISSUER_EN_ADJUST) )
		{
			DispBlockFunc();
			continue;
		}
		if( ChkSettle() )
		{
			break;
		}
		return ERR_NO_DISP;
	}

	return 0;
}

// 调整
// adjustment transaction
int TransAdjust( void )
{
	int		iRet;
	uchar	szTotalAmt[12+1], szTemp[12+1];
	uchar	szCurTime[14+1];
	GUI_INPUTBOX_ATTR stInputAttr;

	SetCurrTitle(_T("ADJUST"));
	if( !ChkEdcOption(EDC_NOT_ADJUST_PWD) )
	{
		if( PasswordAdjust()!=0 )
		{
			return ERR_NO_DISP;
		}
	}

	if( GetTranLogNum(ACQ_ALL)==0 )
	{
		Gui_ClearScr();
		PubBeepErr();
		Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("EMPTY BATCH"), gl_stCenterAttr, GUI_BUTTON_CANCEL, 5, NULL);
		Gui_ClearScr(); 
		return ERR_NO_DISP;
	}

	memset(&stInputAttr, 0, sizeof(stInputAttr));

	stInputAttr.eType = GUI_INPUT_NUM;
	stInputAttr.nMinLen = 1;
	stInputAttr.nMaxLen = 8;
	stInputAttr.bEchoMode = 1;

	while( 1 )
	{
		iRet = AdjustInput();
		if( iRet!=0 )
		{
		    Gui_RegCallback(GUI_CALLBACK_UPDATE_TEXT, NULL);
			return iRet;
		}
		Gui_RegCallback(GUI_CALLBACK_UPDATE_TEXT, DisplayInputAmount);
		while( 2 )
		{
			strcpy(szTotalAmt, glProcInfo.stTranLog.szAmount);
			Gui_ClearScr();
			iRet = Gui_ShowInputBox(GetCurrTitle(), gl_stTitleAttr, _T("NEW TOTAL"), gl_stLeftAttr, 
				szTotalAmt, gl_stRightAttr, &stInputAttr, USER_OPER_TIMEOUT);
			
			if( iRet!=GUI_OK )
			{
			    Gui_RegCallback(GUI_CALLBACK_UPDATE_TEXT, NULL);
				return iRet;
			}

			// Use transaction currency to do conversion
			//PubAddHeadChars(szTotalAmt, 12, '0');
			AmtConvToBit4Format(szTotalAmt, glProcInfo.stTranLog.stTranCurrency.ucIgnoreDigit);
			if( !ValidAdjustAmount(glProcInfo.stTranLog.szAmount, szTotalAmt) )
			{
				continue;
			}
			if (!ValidBigAmount(glProcInfo.stTranLog.szAmount))
			{
				continue;
			}
			if( !ConfirmAmount(NULL, szTotalAmt) )
			{
				break;
			}

			if( !(glProcInfo.stTranLog.uiStatus & TS_NOSEND) )
			{
				PubAscAdd(glProcInfo.stTranLog.szAmount, glProcInfo.stTranLog.szTipAmount, 12, szTemp);
				//PubAddHeadChars(szTemp, 12, '0');  no need: already 12 digits
				sprintf_string((char *)glProcInfo.stTranLog.szOrgAmount, "%.12s", szTemp);
				glProcInfo.stTranLog.uiStatus |= (TS_ADJ|TS_NOSEND);
			}

			PubAscSub(szTotalAmt, glProcInfo.stTranLog.szAmount, 12, szTemp);
			//PubAddHeadChars(szTemp, 12, '0');		no need: already 12 digits
			sprintf_string((char *)glProcInfo.stTranLog.szTipAmount, "%.12s", szTemp);
			GetDateTime(szCurTime);
			sprintf_string((char *)glProcInfo.stTranLog.szDateTime, "%.14s", szCurTime);

			iRet = UpdateTranLog(&glProcInfo.stTranLog, glProcInfo.uiRecNo);
			if( iRet!=0 )
			{
			    Gui_RegCallback(GUI_CALLBACK_UPDATE_TEXT, NULL);
				return iRet;
			}

//			PrintReceipt(PRN_NORMAL);	// no receipt for adjust
			glSysCtrl.uiLastRecNo = glProcInfo.uiRecNo;		// use reprint to print adjust
			SaveSysCtrlBase();

			DispAccepted();
			break;	// continue to adjust
		}	// end of while( 2
		Gui_RegCallback(GUI_CALLBACK_UPDATE_TEXT, NULL);
	}	// end of while( 1
	return RET_OK;
}

// Echo test
int TransEchoTest(void)
{
	int		iRet;
	uchar	ucAcqIndex, szTitle[16+1];

	TransInit(ECHO_TEST);

	sprintf_string((char *)szTitle, "%s", glSysParam.ucAcqNum>8 ? "SELECT ACQ:" : "SELECT ACQUIRER");
	iRet = SelectAcq(FALSE, szTitle, &ucAcqIndex);
	if( iRet!=0 )
	{
		return ERR_NO_DISP;
	}

	SetCommReqField();
	iRet = SendRecvPacket();
	DispResult(iRet);
	return 0;
}

// 结算
// settlement
int TransSettle(void)
{
	int		iRet;
	uchar	ucAcqIndex, szTitle[16+1];
	uchar	ucForceSettle;

	TransInit(SETTLEMENT);

	sprintf_string((char *)szTitle, "%s", glSysParam.ucAcqNum>8 ? "SELECT ACQ:" : "SELECT ACQUIRER");
	iRet = SelectAcq(TRUE, szTitle, &ucAcqIndex);
	if( iRet!=0 )
	{
		return ERR_NO_DISP;
	}

	DispProcess();
	if( ucAcqIndex==MAX_ACQ )
	{
		CalcTotal(ACQ_ALL);
	}
	else
	{
		CalcTotal(glCurAcq.ucKey);
	}

	iRet = DispTransTotal(FALSE);
	if( iRet!=0 )
	{
		return ERR_NO_DISP;
	}

	ucForceSettle = FALSE;
	if( ChkIfZeroTotal(&glTransTotal) )
	{
		unsigned char szBuff[100];
		sprintf_string(szBuff, "%s\n%s", _T("EMPTY BATCH"), _T("SETTLE ANYWAY ?"));
		Gui_ClearScr();
		if(GUI_OK != Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, szBuff, gl_stCenterAttr, GUI_BUTTON_YandN, USER_OPER_TIMEOUT, NULL))
		{
			return ERR_NO_DISP;
		}
		ucForceSettle = TRUE;
	}

	if( !ChkEdcOption(EDC_NOT_SETTLE_PWD) )
	{
		if( PasswordSettle()!=0 )
		{
			return ERR_NO_DISP;
		}
	}

	if( ucAcqIndex!=MAX_ACQ )
	{
		return TransSettleSub();
	}

	for(ucAcqIndex=0; ucAcqIndex<glSysParam.ucAcqNum; ucAcqIndex++)
	{
		SetCurAcq(ucAcqIndex);
		memcpy(&glTransTotal, &glAcqTotal[ucAcqIndex], sizeof(TOTAL_INFO));

		if (ChkIfZeroTotal(&glTransTotal) && !ucForceSettle)
		{
			continue;
		}

		iRet = TransSettleSub();
		if( iRet!=0 )
		{
			if( iRet!=ERR_NO_DISP )
			{
				return iRet;
			}
		}
		// last error ?
	}

	Gui_ClearScr(); // add by lrz v1.02.0000

	return 0;
}

// 结算当前收单行(glCurAcq)
// Settle current acquirer.
int TransSettleSub(void)
{
#ifdef ENABLE_EMV
	int		iLength;
#endif
	int		iRet, bFirstSettle;
	uchar	szBuff[200];

	// don't need to check zero total. it has been check outside.

	TransInit(SETTLEMENT);

	sprintf_string((char*)glProcInfo.szSettleMsg, "CLOSED");
	if( glCurAcq.ucPhoneTimeOut<150 )	// ???? How about Tcp
	{	// 结算时，timeout加长
		// increase the timeout when settle
		glCurAcq.ucPhoneTimeOut += glCurAcq.ucPhoneTimeOut/2;
	}
	else
	{
		glCurAcq.ucPhoneTimeOut = 250;
	}

	iRet = TranReversal();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = OfflineSend(OFFSEND_TC | OFFSEND_TRAN);
	if( iRet!=0 )
	{
		return iRet;
	}

	bFirstSettle = TRUE;
	while( 1 )
	{
		TransInit(SETTLEMENT);

		SetCurrTitle(glCurAcq.szName);
		SetCommReqField();
		sprintf_string((char *)glSendPack.szProcCode, "%s", bFirstSettle ? "920000" : "960000");
		if( ChkIfAmex() )
		{
			memcpy((char *)glSendPack.sField62, "\x00\x06", 2);
			sprintf_string((char *)glSendPack.sField62+2, "%06lu", glSysCtrl.ulInvoiceNo);
		}
		//PubAddHeadChars(glTransTotal.szSaleAmt,   12, '0');  no need: already 12 digits
		//PubAddHeadChars(glTransTotal.szRefundAmt, 12, '0');  no need: already 12 digits
		sprintf_string((char *)&glSendPack.sField63[2], "%03d%12.12s%03d%12.12s",
						glTransTotal.uiSaleCnt,   glTransTotal.szSaleAmt,
						glTransTotal.uiRefundCnt, glTransTotal.szRefundAmt);
		memset(&glSendPack.sField63[30+2], '0', 60); //should be 30+2
		PubLong2Char(90L, 2, glSendPack.sField63);

#ifdef ENABLE_EMV
		if( bFirstSettle && !ChkIfAmex() && ChkIfEmvEnable() && ChkIfAcqNeedDE56() )
		{	
			// 送F56
			// send Field56
			iLength = glSysCtrl.stField56[glCurAcq.ucIndex].uiLength;
			if( iLength>0 )
			{
				memcpy(&glSendPack.sICCData2[2], glSysCtrl.stField56[glCurAcq.ucIndex].sData, iLength);
			}
			else
			{
				SetStdEmptyDE56(&glSendPack.sICCData2[2], &iLength);
			}
			PubLong2Char((ulong)iLength, 2, glSendPack.sICCData2);
		}
#endif

		if( ChkIfAmex() )
		{
			// when AMEX settlement, bit63 len = 36 byte
			PubLong2Char(36L, 2, glSendPack.sField63);
			glSendPack.sField63[2+36] = 0;
		}
		sprintf_string((char *)glSendPack.szField60, "%06lu", glCurAcq.ulCurBatchNo);

		iRet = SendRecvPacket();
		if( iRet!=0 )
		{
			return iRet;
		}
		if( !bFirstSettle )
		{
			if( memcmp(glRecvPack.szRspCode, "95", 2)==0 )
			{
				memcpy(glRecvPack.szRspCode, "77", 2);
			}
			break;
		}

		if( memcmp(glRecvPack.szRspCode, "95", 2)!=0 )
		{
			break;
		}
		if( glSysCtrl.stField56[glCurAcq.ucIndex].uiLength>0 )
		{
			glSysCtrl.stField56[glCurAcq.ucIndex].uiLength = 0;	// erase bit 56
			SaveField56();
		}

		// display error information
		DispResult(ERR_HOST_REJ);

		sprintf_string((char*)glProcInfo.szSettleMsg, "CLOSED(95)");
		iRet = TransUpLoad();
		if( iRet!=0 )
		{
			return iRet;
		}
		bFirstSettle = FALSE;
	}
	if( memcmp(glRecvPack.szRspCode, "77", 2)==0 )
	{
		sprintf_string((char*)glProcInfo.szSettleMsg, "CLOSED(77 RECONCILE ERROR)");
	}

	if( memcmp(glRecvPack.szRspCode, "00", 2)==0 ||
		(!ChkIfAmex() && memcmp(glRecvPack.szRspCode, "77", 2)==0) )
	{
		glSysCtrl.sAcqStatus[glCurAcq.ucIndex] = S_CLR_LOG;
		SaveSysCtrlBase();
		CommOnHook(FALSE);
		ClearRecord(glCurAcq.ucKey);
		DelFilesbyPrefix(GetCurSignPrefix(glCurAcq.ucKey));
		GetNewInvoiceNo();

		DispPrinting();
		iRet = PrintSettle(PRN_NORMAL);
		if( iRet!=0 )
		{
			// Modified by Kim_LinHB 2014-7-11
			Gui_ClearScr();
			Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("PLEASE REPRINT"), gl_stCenterAttr, GUI_BUTTON_OK, 3, NULL);
			iRet = ERR_NO_DISP;
		}

		// Modified by Kim_LinHB 2014-7-11
		if( memcmp(glRecvPack.szRspCode, "77", 2)==0 )
		{
			strcpy(szBuff, _T("RECONCILE ERROR"));
		}
		else if( glTransTotal.uiSaleCnt==0 && glTransTotal.uiRefundCnt==0 )
		{
			strcpy(szBuff, _T("NO TXN TOTAL"));
		}
		else
		{
			sprintf_string((char *)szBuff, "BATCH %06ld\nCLOSED", glCurAcq.ulCurBatchNo);
		}
		Gui_ClearScr();
		PubBeepOk();
		Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, szBuff, gl_stCenterAttr, GUI_BUTTON_OK, 5, NULL);
		Gui_ClearScr();
		return iRet;
	}

	Gui_ClearScr();
	return 0;
}

// 批上送
// batch upload
// Modified by Kim_LinHB 2014-7-11
int TransUpLoad(void)
{
	int			iRet;
	ushort		uiIndex, uiTranNum, uiCnt;
	uchar		szTotalAmt[12+1], szTitle[20];
	TRAN_LOG	stLog;

	TransInit(UPLOAD);

	uiTranNum = GetTranLogNum(glCurAcq.ucKey);
	for(uiCnt=uiIndex=0; uiIndex<MAX_TRANLOG; uiIndex++)
	{
		if( glSysCtrl.sAcqKeyList[uiIndex]!=glCurAcq.ucKey )
		{
			continue;
		}
		uiCnt++;
		
		sprintf_string(szTitle, "%03u/%03u", uiCnt, uiTranNum);
		SetCurrTitle(szTitle);
	
		memset(&stLog, 0, sizeof(TRAN_LOG));
		iRet = LoadTranLog(&stLog, uiIndex);
		if( iRet!=0 )
		{
			return iRet;
		}
		if( stLog.ucTranType==PREAUTH || (stLog.uiStatus & TS_NOT_UPLOAD) )
		{
			continue;
		}
		if( (stLog.uiStatus & TS_VOID) && !ChkIfAmex() )
		{	// AMEX 需要upload void，其它acquirer不用
			continue;
		}

		SetCommReqField();
		sprintf_string((char *)glSendPack.szPan, "%.19s", stLog.szPan);
		sprintf_string((char *)glSendPack.szProcCode, "%.*s", LEN_PROC_CODE,
				glTranConfig[stLog.ucTranType].szProcCode);
		glSendPack.szProcCode[LEN_PROC_CODE-1] = '1';
		if (ChkIfAmex())
		{
			glSendPack.szProcCode[1] = '0';
			glSendPack.szProcCode[2] = '4';
		}

		if (ChkIfAmex() && (stLog.uiStatus & TS_VOID))
		{
			sprintf_string(glSendPack.szTranAmt, "%012lu", 0L);
			glSendPack.szExtAmount[0] = 0;
		}
		else
		{
			PubAscAdd(stLog.szAmount, stLog.szTipAmount, 12, szTotalAmt);
			//PubAddHeadChars(szTotalAmt, 12, '0');  no need: already 12 digits
			sprintf_string((char *)glSendPack.szTranAmt, "%.12s", szTotalAmt);
			if( !ChkIfZeroAmt(stLog.szTipAmount) )
			{
				sprintf_string((char *)glSendPack.szExtAmount, "%.12s", stLog.szTipAmount);
			}
		}

		sprintf_string((char *)glSendPack.szLocalTime, "%.6s", &stLog.szDateTime[8]);
		sprintf_string((char *)glSendPack.szLocalDate, "%.4s", &stLog.szDateTime[4]);
		sprintf_string((char *)glSendPack.szExpDate,   "%.4s", stLog.szExpDate);	// [1/11/2007 Tommy]
		SetEntryMode(&stLog);
		sprintf_string((char *)glSendPack.szCondCode, "%.2s",  stLog.szCondCode);
		sprintf_string((char *)glSendPack.szRRN,      "%.12s", stLog.szRRN);
		sprintf_string((char *)glSendPack.szAuthCode, "%.6s",  stLog.szAuthCode);

		if( !ChkIfAmex() )
		{
			sprintf_string((char *)glSendPack.szRspCode, "%.2s", stLog.szRspCode);
			if( stLog.uiEntryMode & MODE_CHIP_INPUT )
			{
				PubLong2Char((ulong)stLog.uiIccDataLen, 2, glSendPack.sICCData);
				memcpy(&glSendPack.sICCData[2], stLog.sIccData, stLog.uiIccDataLen);
			}
		}

		memcpy(glSendPack.sField62, "\x00\x06", 2);
		sprintf_string((char *)&glSendPack.sField62[2], "%06ld", stLog.ulInvoiceNo);
		memcpy(glSendPack.szField60, glTranConfig[stLog.ucTranType].szTxMsgID, 4);
		if( stLog.uiStatus & TS_OFFLINE_SEND )
		{
			memcpy(glSendPack.szField60, "0220", 4);
			if( ChkAcqOption(ACQ_DBS_FEATURE) )
			{
				if( stLog.ucTranType!=OFF_SALE && stLog.ucTranType==SALE_COMP &&
					!(stLog.uiStatus & TS_FLOOR_LIMIT) )
				{
					memcpy(glSendPack.szField60, "0200", 4);
				}
			}
		}
		else if( stLog.uiStatus & TS_VOID )
		{
			memcpy(glSendPack.szField60, "0200", 4);
		}
		sprintf_string((char *)&glSendPack.szField60[4], "%06ld%12s", stLog.ulSTAN, "");

		iRet = SendRecvPacket();
		if( iRet!=0 )
		{
			return iRet;
		}
		DispResult(0);
	}

	return 0;
}


// end of file

