
#include "Global.h"

/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
/********************** Internal variables declaration *********************/

/********************** external reference declaration *********************/

/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

// When the first time called by manager, the msg type must be USER_MSG
// Modified by Kim_LinHB 2014-6-8
int ProcUserMsg(void)
{
	int		iRet;
	uchar	szEngTime[16+1];
	uchar	ucAcqIndex;

	// no need to init module if running as minor sub-app
	if( glSysParam.stTxnCommCfg.ucCommType == CT_GPRS  ||
		glSysParam.stTxnCommCfg.ucCommType == CT_CDMA  ||
		glSysParam.stTxnCommCfg.ucCommType == CT_WCDMA  || // added by  Gillian 2015/11/23
		glSysParam.stTxnCommCfg.ucCommType == CT_BLTH  || // Added by Kim_LinHB 2014-08-18 v1.01.0004
		glSysParam.stTxnCommCfg.ucCommType == CT_TCPIP ||
		glSysParam.stTxnCommCfg.ucCommType == CT_WIFI)
	{
		GetEngTime(szEngTime);

		Gui_ClearScr();
		Gui_ShowMsgBox(szEngTime, gl_stTitleAttr, _T("INIT COMM..."), gl_stCenterAttr, GUI_BUTTON_NONE, 0, NULL);
		
		iRet = CommInitModule(&glSysParam.stTxnCommCfg);
		if (iRet != 0)
		{
			unsigned char szBuff[200];
			CommOnHook(TRUE);
			strcpy(szBuff, _T("INIT FAIL"));
			strcat(szBuff, "\n");
			if (glSysParam.stTxnCommCfg.ucCommType==CT_GPRS  ||
				glSysParam.stTxnCommCfg.ucCommType==CT_CDMA  ||
				glSysParam.stTxnCommCfg.ucCommType==CT_WCDMA)
			{
				strcat(szBuff, _T("PLS CHECK SIM OR\nHARDWARE/SIGNAL."));
			}
			else if (glSysParam.stTxnCommCfg.ucCommType==CT_TCPIP)
			{
				strcat(szBuff, _T("PLS CHECK CABLE\nOR CONFIG."));
			}
			else if (glSysParam.stTxnCommCfg.ucCommType==CT_WIFI)
			{
				strcat(szBuff, _T("PLS CHECK SIGNAL\nOR CONFIG."));
			}
			Gui_ClearScr();
			Gui_ShowMsgBox(szEngTime, gl_stTitleAttr, szBuff, gl_stCenterAttr, GUI_BUTTON_NONE, -1, NULL);
		}
	}
	// Use a new method to communicates with app-manager

	// erase transaction log of last settlement
	for(ucAcqIndex = 0; ucAcqIndex < glSysParam.ucAcqNum; ucAcqIndex++)
	{
		if(glSysCtrl.sAcqStatus[ucAcqIndex] == S_CLR_LOG)
		{
			ClearRecord(glSysParam.stAcqList[ucAcqIndex].ucKey);
		}
	}

	return 0;
}

// Auto download parameter, new Protims protocol
int AutoDownParam(void)
{
	return 0;
}


// get auto download parameter and return it to app manager
int GetAutoDownParam(void)
{
	return 0;
}

// Process ECR message
int ProcEcrMsg(void)
{
	return 0;
}

// Process magcard swipe event
int ProcMagMsg(void)
{
	int		iRet;

	// Modified by Kim_LinHB 2014-8-8 v1.01.0002 bug506
	//glProcInfo.stTranLog.ucTranType = SALE_OR_AUTH;
	TransInit(SALE_OR_AUTH);

	iRet = SwipeCardProc(TRUE);
	if( iRet!=0 )
	{
		return ERR_NO_DISP;
	}

	iRet = ValidCard();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = TransCapture();
	if( iRet!=0 )
	{
		CommOnHook(FALSE);
		return iRet;
	}

	return 0;
}

// Process IC card insertion event
int ProcICCMsg(void)
{
	int		iRet;

	// Modified by Kim_LinHB 2014-8-8 v1.01.0002 bug506
	// glProcInfo.stTranLog.ucTranType = SALE_OR_AUTH;
	TransInit(SALE_OR_AUTH);

	iRet = GetCard(SKIP_DETECT_ICC|CARD_INSERTED);
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = TransCapture();
	if( iRet!=0 )
	{
		CommOnHook(FALSE);
		return iRet;
	}

	return 0;
}

// Process FN key pressed event
int ProcFuncKeyMsg(void)
{
	FunctionExe(FALSE, glEdcMsgPtr->KeyValue);
	CommOnHook(FALSE);

	return 0;
}

// Process common key pressed event
int ProcKeyMsg(void)
{
	int		iRet, iFuncNo;

	// if UserMsg points to a string "CARDNO=XXXXXX..."
	if ((glEdcMsgPtr->UserMsg!=NULL) &&
		(memcmp((char *)(glEdcMsgPtr->UserMsg), "CARDNO=", 7)==0))
	{
		// Modified by Kim_LinHB 2014-8-8 v1.01.0002 bug506
		// glProcInfo.stTranLog.ucTranType = SALE_OR_AUTH;
		TransInit(SALE_OR_AUTH);
		iRet = GetManualPanFromMsg((char *)(glEdcMsgPtr->UserMsg)+7);	// iRet determines the flow is continue or not
		if (iRet!=0)
		{
			return iRet;
		}

		iRet = TransCapture();
		CommOnHook(FALSE);
		
		return ERR_NO_DISP;
	}

#if !defined(APP_MANAGER_VER) && !defined(_WIN32)
	if (glEdcMsgPtr->KeyValue == KEYCANCEL)
	{
		if (ChkIfAllowExit())
		{
			return ERR_EXIT_APP;
		}
	}
#endif

	if( glEdcMsgPtr->KeyValue>=KEY0 && glEdcMsgPtr->KeyValue<=KEY9 )
	{
		// Modified by Kim_LinHB 2014-8-8 v1.01.0002 bug506
		//glProcInfo.stTranLog.ucTranType = SALE_OR_AUTH;
		TransInit(SALE_OR_AUTH);

		iRet = ManualInputPan(glEdcMsgPtr->KeyValue);
		if( iRet!=0 )
		{
			return iRet;
		}
		iRet = TransCapture();
		if( iRet!=0 )
		{
			CommOnHook(FALSE);
			return iRet;
		}
		Gui_ClearScr(); // Added by Kim_LinHB 2014-08-14 v1.01.0003 bug512
	}

	iRet = ERR_NO_DISP;
	switch( glEdcMsgPtr->KeyValue )
	{
		case KEYF1:
		case KEYFN:
			iFuncNo = FunctionInput();
			if( iFuncNo>=0 )
			{
				glEdcMsgPtr->KeyValue = (uchar)iFuncNo;
				ProcFuncKeyMsg();
			}
			break;
	 
		case KEYUP:
			FunctionMenu();
			break;
			
		case KEYDOWN:
			 if(ChkTerm(_TERMINAL_SP30_)){
				iRet = TransOther();
				PromptRemoveICC();
			 }
			else
				FunctionMenu();
			break;
		case KEYF2:
		case KEYMENU:
			iRet = TransOther();
			PromptRemoveICC();
			break;
		case KEYALPHA:
			if(ChkTerm(_TERMINAL_S300_)) //added temporarily
			{
				iRet = TransOther();
				PromptRemoveICC();
			}
			break;

	#ifdef _Sxx_
		case KEYATM1:
			iRet = TransVoid();
			break;

		case KEYATM2:
			TransAdjust();
			break;

		case KEYATM3:
			iRet = TransSettle();
			break;

		case KEYATM4:
			PrnLastTrans();
			break;
	#endif
	#ifdef ENABLE_CONTLESS
		case KEYENTER:
			iRet = TransClssSale();
			break;
	#endif
		default:
			break;
	}
	
	if(iRet != 0)
	{
		CommOnHook(FALSE);
		return iRet;
	}

	return 0;
}

// end of file

