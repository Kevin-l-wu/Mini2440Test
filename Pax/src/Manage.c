
#include "Global.h"

/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
static void ClearReversalSub(void);
static int  ViewTranSub(int iStartRecNo);

/********************** Internal variables declaration *********************/
/********************** external reference declaration *********************/

/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

void UnLockTerminal(void)
{
	if( ChkEdcOption(EDC_NOT_KEYBOARD_LOCKED) )
	{
		return;
	}

	while( !ChkEdcOption(EDC_NOT_KEYBOARD_LOCKED) )
	{
		SetCurrTitle(_T("TERMINAL  LOCKED")); // Added by Kim_LinHB 2014/9/16 v1.01.0009 bug493
		// Modified by Kim_LinHB 2014-6-8
		Gui_ClearScr();
		Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("MERCHANT PWD"), gl_stCenterAttr, GUI_BUTTON_NONE, -1, NULL);
		if( PasswordMerchant()==0 )
		{
			glSysParam.stEdcInfo.sOption[EDC_NOT_KEYBOARD_LOCKED>>8] |= (EDC_NOT_KEYBOARD_LOCKED & 0xFF);
			SaveEdcParam();
			PubBeepOk();
		}
	}
}

// Modified by Kim_LinHB 2014-6-8
int LockTerm(void)
{
	int iRet;
	SetCurrTitle(_T("LOCK TERM")); // Added by Kim_LinHB 2014/9/16 v1.01.0009 bug493
	Gui_ClearScr();
	iRet = Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("TERMINAL LOCK? "), gl_stCenterAttr, GUI_BUTTON_YandN, -1, NULL);

	if( GUI_OK == iRet)
	{
		glSysParam.stEdcInfo.sOption[EDC_NOT_KEYBOARD_LOCKED>>8] &= ~(EDC_NOT_KEYBOARD_LOCKED & 0xFF);
		SaveEdcParam();
		PubBeepOk();
		UnLockTerminal();
	}
	return 0;
}

// Modified by Kim_LinHB 2014-6-8
int ClearAllRecord(void)
{
	int iRet;
	SetCurrTitle(_T("CLEAR BATCH")); // Added by Kim_LinHB 2014/9/16 v1.01.0009 bug493
	Gui_ClearScr();
	iRet = Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("CONFIRM CLEAR"), gl_stCenterAttr, GUI_BUTTON_YandN, -1, NULL);

	if( iRet != GUI_OK)
	{
		return ERR_NO_DISP;
	}

	DispProcess();

	ClearRecord(ACQ_ALL);
	DispClearOk();
	return 0;
}

// Modified by Kim_LinHB 2014-6-8
int ClearConfig(void)
{
	int iRet;
	SetCurrTitle(_T("CLEAR CONFIG")); // Added by Kim_LinHB 2014/9/16 v1.01.0009 bug493
	Gui_ClearScr();
	iRet = Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("CONFIRM CLEAR"), gl_stCenterAttr, GUI_BUTTON_YandN, -1, NULL);

	if( iRet != GUI_OK)
	{
		return ERR_NO_DISP;
	}
	
	DispProcess();

	LoadEdcDefault();

#ifdef ENABLE_EMV
	LoadEmvDefault();
#endif
  
	DispClearOk();
	return 0;
}

// Modified by Kim_LinHB 2014-6-8
int ClearPassword(void)
{
	int iRet;
	SetCurrTitle(_T("CLEAR PASSWORD")); // Added by Kim_LinHB 2014/9/16 v1.01.0009 bug493
	Gui_ClearScr();
	iRet = Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("CONFIRM CLEAR"), gl_stCenterAttr, GUI_BUTTON_YandN, -1, NULL);

	if( iRet != GUI_OK)
	{
		return ERR_NO_DISP;
	}

	DispProcess();

	ResetPwdAll();
	SavePassword();
	DispClearOk();
	return 0;
}

// Modified by Kim_LinHB 2014-6-8
int ClearReversal(void)
{
	int iRet;
	SetCurrTitle(_T("CLEAR REVERSAL")); // Added by Kim_LinHB 2014/9/16 v1.01.0009 bug493
	Gui_ClearScr();
	iRet = Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("CONFIRM CLEAR"), gl_stCenterAttr, GUI_BUTTON_YandN, -1, NULL);

	if( iRet != GUI_OK)
	{
		return ERR_NO_DISP;
	}

	DispProcess();

	ClearReversalSub();
	DispClearOk();
	return 0;
}

void ClearReversalSub(void)
{
	int		iCnt;

	for(iCnt=0; iCnt<(int)glSysParam.ucAcqNum; iCnt++)
	{
		glSysCtrl.stRevInfo[iCnt].bNeedReversal = FALSE;
		if( glSysCtrl.sAcqStatus[iCnt]==S_PENDING )	// Ϊ��ɾ�������־???
		{
			glSysCtrl.sAcqStatus[iCnt] = S_USE;
		}
	}
	SaveSysCtrlNormal();

	// glSysCtrl.stField56
}

// ɾ���ض�acquirer�Ľ��׼�¼
// Delete transaction records belonging to specific acquirer
void ClearRecord(uchar ucAcqKey)
{
	int		iCnt;

	if( ucAcqKey==ACQ_ALL )
	{
	    DelFilesbyPrefix(GetCurSignPrefix(ACQ_ALL));
		glSysCtrl.uiLastRecNo = 0xFFFF;
		// ɾ��������־
		// Delete record
		for(iCnt=0; iCnt<MAX_TRANLOG; iCnt++)
		{
			glSysCtrl.sAcqKeyList[iCnt]    = INV_ACQ_KEY;
			glSysCtrl.sIssuerKeyList[iCnt] = INV_ISSUER_KEY;
		}

		// �ָ��յ���״̬�����������־
		// Reset status and reversal flag
		for(iCnt=0; iCnt<(int)glSysParam.ucAcqNum; iCnt++)
		{
			glSysCtrl.sAcqStatus[iCnt]         = S_USE;
			glSysCtrl.stRevInfo[iCnt].bNeedReversal = FALSE;
			glSysCtrl.uiLastRecNoList[iCnt]    = 0xFFFF;
			glSysCtrl.stField56[iCnt].uiLength = 0;	// erase bit 56
			if( !(glSysParam.stAcqList[iCnt].ulCurBatchNo>0 &&
				  glSysParam.stAcqList[iCnt].ulCurBatchNo<=999999L) )
			{
				glSysParam.stAcqList[iCnt].ulCurBatchNo = 1L;
			}
		}
		SaveSysCtrlAll();
	}
	else
	{
		if( glSysCtrl.uiLastRecNo<MAX_TRANLOG &&
			glSysCtrl.sAcqKeyList[glSysCtrl.uiLastRecNo]==ucAcqKey )
		{
			glSysCtrl.uiLastRecNo = 0xFFFF;
		}

		// ɾ��������־
		// delete all transaction records
		for(iCnt=0; iCnt<MAX_TRANLOG; iCnt++)
		{
			if( glSysCtrl.sAcqKeyList[iCnt]==ucAcqKey )
			{
				glSysCtrl.sAcqKeyList[iCnt]    = INV_ACQ_KEY;
				glSysCtrl.sIssuerKeyList[iCnt] = INV_ISSUER_KEY;
			}
		}

		// �ָ��յ���״̬�����������־
		// reset statuses of all acquirers, and clear reversal flags
		for(iCnt=0; iCnt<(int)glSysParam.ucAcqNum; iCnt++)
		{
			if( glSysParam.stAcqList[iCnt].ucKey==ucAcqKey )
			{
			    DelFilesbyPrefix(GetCurSignPrefix(ucAcqKey));
				glSysCtrl.sAcqStatus[iCnt] = S_USE;
				glSysCtrl.stRevInfo[iCnt].bNeedReversal = FALSE;
				glSysCtrl.uiLastRecNoList[iCnt] = 0xFFFF;
				if( glSysCtrl.stField56[iCnt].uiLength>0 )
				{
					glSysCtrl.stField56[iCnt].uiLength = 0;	// erase bit 56
					SaveField56();
				}

				// increase batch no
				glSysParam.stAcqList[iCnt].ulCurBatchNo = GetNewBatchNo(glSysParam.stAcqList[iCnt].ulCurBatchNo);

				break;
			}
		}
		SaveSysCtrlNormal();
	}

	SaveSysParam();
}

// ����ն����ݽ���
// Interface of "Clear". (FUNC99) 
// Modified by Kim_LinHB 2014-6-8
int DoClear(void)
{
	GUI_MENU	stClearMenu;
	GUI_MENUITEM stClearMenuItems[] =
	{
		{ "CLEAR CONFIG",	1,TRUE,  ClearConfig},
		{ "CLEAR BATCH",	2,TRUE,  ClearAllRecord},
		{ "CLEAR REVERSAL",	3,TRUE,  ClearReversal},
		{ "CLEAR PWD",	4,TRUE,  ClearPassword},
		{ "",	-1,FALSE, NULL},
	};

	SetCurrTitle(_T("CLEAR"));
	if( PasswordBank()!=0 )
	{
		return ERR_NO_DISP;
	}

	Gui_BindMenu(GetCurrTitle(), gl_stTitleAttr, gl_stLeftAttr, (GUI_MENUITEM *)stClearMenuItems, &stClearMenu);
	Gui_ClearScr();
	Gui_ShowMenuList(&stClearMenu, 0, USER_OPER_TIMEOUT, NULL);
	return 0;
}

// �鿴���׻���
// View total. (glTransTotal)
int ViewTotal(void)
{
	CalcTotal(ACQ_ALL);
	DispTransTotal(TRUE);
	return 0;
}

// �鿴���н��׼�¼
// View all transaction record
int ViewTranList(void)
{
	SetCurrTitle("TRANS REVIEW"); // Added by Kim_LinHB 2014/9/16 v1.01.0009 bug493
	if( GetTranLogNum(ACQ_ALL)==0 )
	{
		// Modified by Kim_LinHB 2014-6-8
		Gui_ClearScr();
		PubBeepErr();
		Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("EMPTY BATCH"), gl_stCenterAttr, GUI_BUTTON_CANCEL, 5, NULL);
		return ERR_NO_DISP;
	}

	// ������Խ���
	// view all transaction records
	ViewTranSub(-1);
	return 0;
}

// �鿴ָ�����׼�¼
// View specific record
// Modified by Kim_LinHB 2014-6-8
int ViewSpecList(void)
{
	int			iRet;
	TRAN_LOG	stLog;

	SetCurrTitle("TRANS REVIEW");

	while (1)
	{
		if( GetTranLogNum(ACQ_ALL)==0 )
		{
			Gui_ClearScr();
			PubBeepErr();
			Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("EMPTY BATCH"), gl_stCenterAttr, GUI_BUTTON_CANCEL, 5, NULL);
			return ERR_NO_DISP;
		}

		iRet = GetRecord(TS_ALL_LOG, &stLog);
		if( iRet!=0 )
		{
			return ERR_NO_DISP;
		}

		if (ViewTranSub((int)glProcInfo.uiRecNo)!=0)
		{
			break;
		}
	}
	return 0;
}

// ���׼�¼�������
// View transaction records
// ���أ�ERR_USERCANCEL--ȡ����ʱ�˳���0--������������ԭ���˳�
// return ERR_USERCANCEL--timeout or cancel
// Modified by Kim_LinHB 2014-6-8
int ViewTranSub(int iStartRecNo)
{
	int			iRecNo, iStep, iCnt, iActRecNo;
	TRAN_LOG	stLog;

	iRecNo = iStartRecNo;
	iStep  = iStartRecNo<0 ? 1 : 0;
	while( 1 )
	{
		iRecNo = iRecNo + iStep;
		if( iRecNo>=MAX_TRANLOG )
		{
			Gui_ClearScr();
			Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("END OF BATCH"), gl_stCenterAttr, GUI_BUTTON_NONE, 1, NULL);
			iRecNo = 0;
		}
		else if( iRecNo<0 )
		{
			Gui_ClearScr();
			Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("START OF BATCH"), gl_stCenterAttr, GUI_BUTTON_NONE, 1, NULL);
			iRecNo = MAX_TRANLOG-1;
		}
		if( glSysCtrl.sAcqKeyList[iRecNo]==INV_ACQ_KEY )
		{
			continue;
		}
		memset(&stLog, 0, sizeof(TRAN_LOG));
		LoadTranLog(&stLog, (ushort)iRecNo);
		for(iActRecNo=iCnt=0; iCnt<=iRecNo; iCnt++)
		{
			if( glSysCtrl.sAcqKeyList[iCnt]!=INV_ACQ_KEY )
			{
				iActRecNo++;
			}
		}

		{
			int		iRet;
			uchar	*pszTitle, szBuff[25], szTotalAmt[12+1], ucCnt = 0;
			GUI_PAGELINE stBuff[20];
			GUI_TEXT_ATTR stLeftAttr_Small = gl_stLeftAttr;
			GUI_PAGE	 stPage;

			TRAN_LOG	*pstLog = (TRAN_LOG *)&stLog;
			stLeftAttr_Small.eFontSize = GUI_FONT_SMALL;

			sprintf_string(stBuff[ucCnt].szLine, "%03d/%03d", iActRecNo, GetTranLogNum(ACQ_ALL));
			stBuff[ucCnt++].stLineAttr = gl_stLeftAttr;

			// Modified by Kim_LinHB 2014-8-8 v1.01.0002 bug508
			iRet = GetStateText(pstLog->uiStatus, szBuff);
			if(0 == iRet)
			{
				pszTitle = glTranConfig[pstLog->ucTranType].szLabel;
			}
			else if(1 == iRet)
			{
				pszTitle = glTranConfig[pstLog->ucOrgTranType].szLabel;
			}

			sprintf_string(stBuff[ucCnt].szLine, "Status:%s", szBuff);
			stBuff[ucCnt++].stLineAttr = gl_stLeftAttr;

			sprintf_string(stBuff[ucCnt].szLine, "TRACE:%06lu", pstLog->ulInvoiceNo);
			stBuff[ucCnt++].stLineAttr = gl_stLeftAttr;

			if( !ChkIfDispMaskPan2() )
			{
				strcpy(stBuff[ucCnt].szLine, pstLog->szPan);
			}
			else
			{
				MaskPan(pstLog->szPan, szBuff);
				strcpy(stBuff[ucCnt].szLine, szBuff);
			}
			stBuff[ucCnt++].stLineAttr = stLeftAttr_Small;


			PubAscAdd(pstLog->szAmount, pstLog->szTipAmount, 12, szTotalAmt);
			App_ConvAmountTran(szTotalAmt,	szBuff, GetTranAmountInfo(pstLog));
			strcpy(stBuff[ucCnt].szLine, szBuff);
			stBuff[ucCnt++].stLineAttr = stLeftAttr_Small;
		
			strcpy(stBuff[ucCnt].szLine, "APPROVAL CODE:");
			stBuff[ucCnt++].stLineAttr = gl_stLeftAttr;

			sprintf_string(stBuff[ucCnt].szLine, "%6.6s", pstLog->szAuthCode);
			stBuff[ucCnt++].stLineAttr = gl_stRightAttr;
			
			Conv2EngTime(pstLog->szDateTime, szBuff);
			sprintf_string(stBuff[ucCnt].szLine, "%s", szBuff);
			stBuff[ucCnt++].stLineAttr = stLeftAttr_Small;

			sprintf_string(stBuff[ucCnt].szLine, "RRN:%12.12s", pstLog->szRRN);
			stBuff[ucCnt++].stLineAttr = gl_stLeftAttr;

			Gui_CreateInfoPage(_T(pszTitle), gl_stTitleAttr, stBuff, ucCnt, &stPage);
			Gui_ClearScr();

			iRet = Gui_ShowInfoPage(&stPage, iStartRecNo < 0 ? TRUE : FALSE, USER_OPER_TIMEOUT);

			// ����ָ����¼,�����·�ҳ
			// if viewing a specific record, then return directly
			if (iStartRecNo>=0)
			{
				return 0;
			}

			if(GUI_OK_NEXT == iRet)
			{
				iStep = 1;
			}
			else if(GUI_OK_PREVIOUS == iRet)
			{
				iStep = -1;
			}
			else
			{
				return ERR_USERCANCEL;
			}
		}
	}
	return 0;
}

// Modified by Kim_LinHB 2014-6-8
int PrnLastTrans(void)
{
	int			iRet;

	SetCurrTitle(_T("REPRINT"));

	
	if( glSysCtrl.uiLastRecNo>=MAX_TRANLOG )
	{
		Gui_ClearScr();
		PubBeepErr();
		Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("EMPTY BATCH"), gl_stCenterAttr, GUI_BUTTON_CANCEL, 5, NULL);
		return ERR_NO_DISP;
	}

	InitTransInfo();
	iRet = LoadTranLog(&glProcInfo.stTranLog, glSysCtrl.uiLastRecNo);
	if( iRet!=0 )
	{
		return ERR_NO_DISP;
	}

	Gui_ClearScr();
	// Modified by Kim_LinHB 2014-8-11 v1.01.0003
	Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, NULL, gl_stCenterAttr, GUI_BUTTON_NONE, 0, NULL);

	FindAcq(glProcInfo.stTranLog.ucAcqKey);
	FindIssuer(glProcInfo.stTranLog.ucIssuerKey);
	PrintReceipt(PRN_REPRINT);
	return 0;
}

// Modified by Kim_LinHB 2014-6-8
int RePrnSpecTrans(void)
{
	int			iRet;

	SetCurrTitle(_T("REPRINT"));

	if( GetTranLogNum(ACQ_ALL)==0 )
	{
		Gui_ClearScr();
		PubBeepErr();
		Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("EMPTY BATCH"),gl_stCenterAttr, GUI_BUTTON_CANCEL, 5, NULL);
		return ERR_NO_DISP;
	}

	InitTransInfo();
	iRet = GetRecord(TS_ALL_LOG, &glProcInfo.stTranLog);
	if( iRet!=0 )
	{
		return ERR_NO_DISP;
	}

	Gui_ClearScr();
	// Modified by Kim_LinHB 2014-8-11 v1.01.0003
	Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, NULL, gl_stCenterAttr, GUI_BUTTON_NONE, 0, NULL);

	FindAcq(glProcInfo.stTranLog.ucAcqKey);
	FindIssuer(glProcInfo.stTranLog.ucIssuerKey);
	PrintReceipt(PRN_REPRINT);
	return 0;
}

// Modified by Kim_LinHB 2014-6-8
int PrnTotal(void)
{
	int		iRet;
	uchar	ucIndex;

	SetCurrTitle(_T("PRINT TOTAL"));

	Gui_ClearScr();
	iRet = SelectAcq(FALSE, GetCurrTitle(), &ucIndex);
	if( iRet!=0 )
	{
		return ERR_NO_DISP;
	}
	CalcTotal(glCurAcq.ucKey);

	Gui_ClearScr();
	if (ChkIfZeroTotal(&glTransTotal))
	{
		PubBeepErr();
		Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("EMPTY BATCH"),gl_stCenterAttr, GUI_BUTTON_CANCEL, 3, NULL);
		return ERR_NO_DISP;
	}

	PrnTotalAcq();
	return 0;
}

// Modified by Kim_LinHB 2014-6-8
int RePrnSettle(void)
{
	int		iRet;

	SetCurrTitle(_T("REPRINT SETTLE"));

	iRet = SelectAcq(FALSE, GetCurrTitle(), NULL);
	if( iRet!=0 )
	{
		return ERR_NO_DISP;
	}

	if( !glSysCtrl.stRePrnStlInfo.bValid[glCurAcq.ucIndex] )
	{
		PubBeepErr();
		Gui_ClearScr();
		Gui_ShowMsgBox(GetCurrTitle(), gl_stTitleAttr, _T("NO RECEIPT"),gl_stCenterAttr, GUI_BUTTON_CANCEL, 3, NULL);
		return ERR_NO_DISP;
	}

	sprintf_string((char *)glProcInfo.szSettleMsg, "%s", glSysCtrl.stRePrnStlInfo.szSettleMsg[glCurAcq.ucIndex]);
	memcpy(&glTransTotal, &glSysCtrl.stRePrnStlInfo.stAcqTotal[glCurAcq.ucIndex], sizeof(TOTAL_INFO));
	memcpy(glIssuerTotal, glSysCtrl.stRePrnStlInfo.stIssTotal[glCurAcq.ucIndex], sizeof(glIssuerTotal));
	glCurAcq.ulCurBatchNo = glSysCtrl.stRePrnStlInfo.ulBatchNo[glCurAcq.ucIndex];

	DispPrinting();
	PrintSettle(PRN_REPRINT);
	return 0;
}

#ifdef ENABLE_EMV
// Print EMV error log message
int PrintEmvErrLog(void)
{
	// Modified by Kim_LinHB 2014-6-8
	SetCurrTitle(_T("PRINT ERROR LOG"));
	if( PasswordBank()!=0 )
	{
		return ERR_NO_DISP;
	}

	DispProcess();

	PrintEmvErrLogSub();
	return 0;
}
#endif

// end of file

