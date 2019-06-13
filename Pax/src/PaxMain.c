
#include "Global.h"

/********************** Internal macros declaration ************************/
#define TIMER_TEMPORARY		4
#define TIMERCNT_MAX		48000

/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
static int   MainSub(const ST_EVENT_MSG *pstEventMsg);
static int   CheckTermSatus(const ST_EVENT_MSG *pstEventMsg);
static void  FirstRunProc(const ST_EVENT_MSG *pstEventMsg);
static void  SetIdleTimer(void);
static uchar ChkIdleTimer(int iSeconds);
static void  ShowIdleMainUI(uchar *pbForceUpdate, uchar bShowGallery, int iGalleryImageNum);
static void  ShowGallery(int num);
static int   CheckInitTerminal(void);
static void  MainIdleProc(void);
/********************** Internal variables declaration *********************/

/********************** external reference declaration *********************/
extern int EventMain(ST_EVENT_MSG *pstEventMsg);

const APPINFO AppInfo =
{
	APP_NAME,
	EDCAPP_AID,
	EDC_VER_INTERN,
	"PAX TECHNOLOGY",
	"EDC TEMPLATE",
	"",
	0xFF,
	0xFF,
	0x01,
	""
};

/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

int EventMain(ST_EVENT_MSG *pstEventMsg)
{
	glEdcMsgPtr = pstEventMsg;	// save the event pointer 
	return MainSub(pstEventMsg);
}

int PaxMain(void)
{
	uchar			bUpdateUI, bGallery = FALSE;
	int				iRet;
	ST_EVENT_MSG	stEventMsg;
	int iGalleryImageNum = 0;
	uchar szGalleryImageNum[5] = {0};

    // Application initialization
	memset(&stEventMsg, 0, sizeof(stEventMsg));
	stEventMsg.MsgType = USER_MSG;
	
	iRet = EventMain(&stEventMsg);
	if (iRet == ERR_EXIT_APP)
	{
//	    CommOnHook(TRUE);
		return 0;
	}

    // Main loop
	bUpdateUI = TRUE;
	if(0 == GetEnv("PIC_NO", szGalleryImageNum))
	{
	    iGalleryImageNum = atoi(szGalleryImageNum);
	}
	else
	{
	    iGalleryImageNum = 0;
	}

	//RunApp(1); test

	while(1)
	{
		// Setup idle timer
		
		printf("EDC Dead Circle\n");
		if (bUpdateUI)
		{
			SetIdleTimer();
		}

		ShowIdleMainUI(&bUpdateUI, bGallery, iGalleryImageNum);	// bUpdateUI value may change

        // When magstripe card swiped
		if(MagSwiped() == 0)
		{
			memset(&stEventMsg, 0, sizeof(ST_EVENT_MSG));
			stEventMsg.MsgType = MAGCARD_MSG;
			stEventMsg.MagMsg.RetCode = MagRead(stEventMsg.MagMsg.track1,
												stEventMsg.MagMsg.track2, 
												stEventMsg.MagMsg.track3);
			
			iRet = EventMain(&stEventMsg);
			if (iRet == ERR_EXIT_APP)
			{
//			    CommOnHook(TRUE);
				return 0;
			}
			
			bUpdateUI = TRUE;
			bGallery = FALSE;
			CommOnHook(FALSE);
			
			continue;
		}

        // When chip card inserted
		if(ChkIfEmvEnable() && IccDetect(ICC_USER) == 0)
		{
			memset(&stEventMsg, 0, sizeof(ST_EVENT_MSG));
			stEventMsg.MsgType = ICCARD_MSG;
			
			iRet = EventMain(&stEventMsg);
			if (iRet == ERR_EXIT_APP)
			{
//			    CommOnHook(TRUE);
				return 0;
			}
			
			bUpdateUI = TRUE;
			bGallery = FALSE;
			CommOnHook(FALSE);
			
			continue;
		}

        // If any key is pressed
		if(0 == kbhit())
		{
			memset(&stEventMsg, 0, sizeof(ST_EVENT_MSG));
			stEventMsg.MsgType = KEYBOARD_MSG;
			stEventMsg.KeyValue = getkey();
			// Add by lirz v1.02.0000
			// If key enter,it repaint the PaxMain UI(mainly logo) again and again,do not do this
			if(KEYENTER == stEventMsg.KeyValue)
			{
			    //RunApp(1);  test
				continue;
			}
			
			iRet = EventMain(&stEventMsg);
			if (iRet == ERR_EXIT_APP)
			{
				SetCurrTitle(_T("EXIT?"));
//			    CommOnHook(TRUE);
				if(PasswordBank() == 0)
				{
					return 0;
				}
			}
			
			bUpdateUI = TRUE;
			bGallery = FALSE;
			CommOnHook(FALSE);
			
			continue;
		}

		// TODO:add your event-handlers here...
        if (ChkIdleTimer(glSysParam.stEdcInfo.ucIdleMinute * 60 /2))
        {
            //OsLog(LOG_ERROR, ":%d", glSysParam.stEdcInfo.ucIdleMinute/2);
            bGallery = TRUE;
            continue;
        }

		// One of idle-processing
		if (ChkIdleTimer(glSysParam.stEdcInfo.ucIdleMinute * 60))
		{
			MainIdleProc();
			bUpdateUI = TRUE;
			bGallery = FALSE;
			continue;
		}
	} // end of while( 1
	return 0;
}

void GuiTest()
{
	printf("GuiTest()\n");
	
	Gui_Init(0x10, 0x200);
	
	KeyBoardInit();
	
	ST_LCD_INFO lcdInfo;
	
	CLcdGetInfo(&lcdInfo);
	
	printf("lcdInfo.width = 0x%x\n", lcdInfo.width);
	printf("lcdInfo.height = %d\n", lcdInfo.height);
	printf("lcdInfo.ppl = %d\n", lcdInfo.ppl);
	printf("lcdInfo.ppc = %d\n", lcdInfo.ppc);
	printf("lcdInfo.fgColor = %d\n", lcdInfo.fgColor);
	printf("lcdInfo.bgColor = %d\n", lcdInfo.bgColor);
/*
	CLcdTextOut(80, 50, "PAX POS");
	
	CLcdSetFgColor(0x10);
	
	CLcdTextOut(80, 80, "PAX POS");
	
	CLcdSetFgColor(0x200);
	
	CLcdTextOut(80, 110, "PAX POS");
	
	CLcdSetFgColor(0x4000);
	
	CLcdTextOut(80, 140, "PAX POS");
*/
	GUI_TEXT_ATTR stTitleAttr = {
		GUI_FONT_LARGE,
		GUI_ALIGN_LEFT,
		GUI_FONT_STD,
	};
	
	GUI_TEXT_ATTR stContentAttr = 
	{
		GUI_FONT_SMALL,
		GUI_ALIGN_LEFT,
		GUI_FONT_STD,
	};
	
	CLcdDrawRect(100, 100, 120, 120, 0x4000);
	
	while(!kbhit())
	{
		printf("Wait For Key pressed\n");
		
		delay(1024 * 1024);
	}
	delay(1024);
	kbflush();
	delay(1024);
	
	
	Gui_DrawText("PAX", stContentAttr, 50, 50);
//	Gui_ShowMsgBox("TITLE", stTitleAttr, "OK", stContentAttr,  GUI_BUTTON_OK, 10, 0);
	
	while(!kbhit())
	{
		printf("Wait For Key pressed\n");
		
		delay(1024 * 1024);
	}
	
	kbflush();
}


// event processing
int MainSub(const ST_EVENT_MSG *pstEventMsg)
{
	int iRet;
	
	SystemInit();
	
	GuiTest();

	SetOffBase(NULL);

	CheckTermSatus(pstEventMsg);

	printf("Log1\n");
	
	iRet = ERR_NO_DISP;
	switch( pstEventMsg->MsgType )
	{
		case USER_MSG:
			ProcUserMsg();
			break;
		
		case EDCAPP_AUTOUPDATE:
			AutoDownParam();
			break;
		
		case EDCAPP_GETPARA:
			GetAutoDownParam();
			break;

		case EDC_ECR_MSG:
			iRet = ProcEcrMsg();
			break;
		
		case MAGCARD_MSG:
			iRet = ProcMagMsg();
			break;
		
		case ICCARD_MSG:
			iRet = ProcICCMsg();
			PromptRemoveICC();
			break;
		case KEYBOARD_MSG:
			iRet = ProcKeyMsg();
			break;

		case APPMSG_SYNC:
			break;

		default:
			break;
	}
	
	if(iRet != 0)
	{
		DispResult(iRet);
		Gui_ClearScr(); // Added by Kim_LinHB 2014-08-13 v1.01.0003 bug512
	}

	SetOffBase(NULL);

    kbflush();
	CheckInitTerminal();
	UnLockTerminal();

	return iRet; // Modified by Kim_LinHB 2014-08-13 v1.01.0003 bug 512
}

// read config parameters, check terminal status(e.g. if need to download parameters, if it's locked, etc.)
// and reset reversal flag etc
int CheckTermSatus(const ST_EVENT_MSG *pstEventMsg)
{
	FirstRunProc(pstEventMsg);
	LoadEdcLang();

	memcpy(&glSysParamBak, &glSysParam, sizeof(glSysParam));
	LoadSysDownEdcFile();	// load the files downloaded from Protims

	CheckInitTerminal();
	UnLockTerminal();

	RecoverTranLog();	// must called after system initialization
	InitTransInfo();

	return 0;
}

// 第一次运行时候处理(事实上每次main_sub都会运行)
// process for the first run
static char bFirstRun = 1; //added by Kim_LinHB 2014-6-7
void FirstRunProc(const ST_EVENT_MSG *pstEventMsg)
{
	uchar	szEngTime[16+1];
	uchar	ucNeedUpdateParam;

	// Added by Kim_LinHB 2014-7-2
	if(bFirstRun)
	{
		ST_FONT stFont[3];
		unsigned char sTermInfo[30];

		kbmute(0);
		GetTermInfo(sTermInfo);
		Gui_Init(_RGB_INT_(255,255,220), _RGB_INT_(0, 20,255));

		memcpy(stFont, gl_Font_Def, sizeof(gl_Font_Def));
		// Added by Kim_LinHB 2014-8-12 v1.01.0003 bug510
#ifdef _Sxxx_
		if ((sTermInfo[0] != _TERMINAL_S300_) && (sTermInfo[0] != _TERMINAL_S900_))
		{
			if(_TERMINAL_S800_ == sTermInfo[0]){
				stFont[1].Width = 12;
				stFont[1].Height = 24;
			}
			else{
				stFont[0].Width = 6;
				stFont[0].Height = 8;

				stFont[1].Width = 8;
				stFont[1].Height = 16;
			}
		}
#endif

		Gui_LoadFont(GUI_FONT_SMALL, &stFont[0],  NULL);
		Gui_LoadFont(GUI_FONT_NORMAL, &stFont[1], NULL);
		Gui_LoadFont(GUI_FONT_LARGE, &stFont[2], NULL );

		// Modified by Kim_LinHB 2014-7-2
		if( ExistSysFiles() )
		{
			if (ValidSysFiles())
			{
				LoadSysParam();
				LoadSysCtrlAll();

				ucNeedUpdateParam = FALSE;
				if (pstEventMsg->MsgType==USER_MSG)
				{
					if (UpdateTermInfo() || InitMultiAppInfo())
					{
						ucNeedUpdateParam = TRUE;
					}
				}
				if( glSysParam.stTxnCommCfg.pfUpdWaitUI!=DispWaitRspStatus )
				{
					glSysParam.stTxnCommCfg.pfUpdWaitUI = DispWaitRspStatus;
					glSysParam.stTMSCommCfg.pfUpdWaitUI = DispWaitRspStatus;
					ucNeedUpdateParam = TRUE;
				}

				if (ucNeedUpdateParam)
				{
					SaveSysParam();
				}
				bFirstRun = 0;
				return;
			}
			else
			{
				int iRet;
				Gui_ClearScr();
				iRet = Gui_ShowMsgBox(NULL, gl_stTitleAttr, _T("APP AND DATA\nINCONSIST.\nRESET CONFIG?"), gl_stCenterAttr, GUI_BUTTON_YandN, -1, NULL);
				if (iRet != GUI_OK)
				{
					SysHaltInfo("PLS REPLACE APP");
				}

				Gui_ClearScr();
				iRet = Gui_ShowMsgBox(NULL, gl_stTitleAttr, _T("WARNING\nDATA WILL BE CLEAR\nCONTINUE ?"), gl_stCenterAttr, GUI_BUTTON_YandN, -1, NULL);
				if (iRet != GUI_OK)
				{
					SysHaltInfo("PLS REPLACE APP");
				}

				RemoveSysFiles();
			}
		}

		GetEngTime(szEngTime);
		Gui_ClearScr();
		Gui_ShowMsgBox(szEngTime, gl_stTitleAttr, _T("PLEASE WAIT...\nSYSTEM INIT..."), gl_stCenterAttr, GUI_BUTTON_NONE, 0, NULL);

		LoadEdcDefault();	// set EDC default values
		InitTranLogFile();	// Init transaction log file
		NoDownloadInit();
		
		bFirstRun = 0;
	}

#ifdef ENABLE_EMV
	LoadEmvDefault();	// Init EMV kernel
#endif
}

// 设置空闲计时。设置一个比较长的倒计时，以用于不止一种的空闲事件处理
// set a idle timer with a long period of time, for processing several idle events
void SetIdleTimer(void)
{
	TimerSet(TIMER_TEMPORARY, TIMERCNT_MAX);
}

// 检查空闲计时，看是否已经流过了指定的分钟数
// check if the timer counted the specific time(uint:minute)
uchar ChkIdleTimer(int iSeconds)
{
	int	iCnt = TIMERCNT_MAX-TimerCheck(TIMER_TEMPORARY);
	
	PubASSERT(TIMERCNT_MAX > iSeconds*10);	//	ScrPrint(0,7,ASCII,"%d  ", iCnt/10);
	return (iCnt >= iSeconds*10);
}

// 显示空闲时用户界面
// show an idle UI
void ShowIdleMainUI(uchar *pbForceUpdate, uchar bShowGallery, int iGalleryImageNum)
{
	static	uchar	szLastTime[5+1] = {"00000"};
	uchar	szCurTime[16+1];

	GetEngTime(szCurTime);
	if( *pbForceUpdate || memcmp(szLastTime, &szCurTime[11], 4)!=0 )	// Reset magstripe reader every 10 minutes
	{
		MagClose();
		MagOpen();
		MagReset();
	}

	if(bShowGallery){
	    ShowGallery(iGalleryImageNum);
	}

	if( *pbForceUpdate || memcmp(szLastTime, &szCurTime[11], 5)!=0)
	{
		// refresh UI
		sprintf_string((char *)szLastTime, "%.5s", &szCurTime[11]); 

		// Gui_ClearScr(); // removed by Kim_LinHB 2014-08-13 v1.01.0003 bug512
		// Modified by Kim_LinHB 2014-8-11 v1.01.0003
        //Gui_ShowMsgBox(szCurTime, gl_stTitleAttr, NULL, gl_stCenterAttr, GUI_BUTTON_NONE, 0,NULL);
        Gui_UpdateTitle(szCurTime, gl_stTitleAttr);
		//Gui_DrawText(szCurTime, gl_stTitleAttr, 0, 5);
        if(*pbForceUpdate){
            if(!bShowGallery)
            {
                Gui_UpdateKey(KEYFN, "FUNC");
                Gui_UpdateKey(KEYMENU, "MENU");
                DispSwipeCard(TRUE);
			}
        }
		*pbForceUpdate = FALSE;
	}
#ifdef _WIN32
	DelayMs(100);
#endif
}

static void ShowGallery(int num)
{
    static int iCurrImages = 0;
    static unsigned long ulTimer_Late = 0;
    static unsigned long ulTimer_Curr = 0;

    if(num <= 0)
        return;

    ulTimer_Curr = GetTimerCount();
    if(ulTimer_Curr - ulTimer_Late >= 10000)
    {
        uchar szImageKey[8];
        uchar szImageValue[255] = {0};
        sprintf_string(szImageKey, "PIC_%d", iCurrImages);
        if(0 == GetEnv(szImageKey, szImageValue))
        {
            ulTimer_Late = ulTimer_Curr;
            Gui_ClearScr();
			Gui_DrawImage(szImageValue, 0,0);
        }

        ++iCurrImages;
        if(iCurrImages >= num)
            iCurrImages = 0;
    }

}

// Modified by Kim_LinHB 2014-7-8
int CheckInitTerminal(void)
{
	uchar	szCurTime[16+1], szLastTime[16+1];
	uchar	ucKey;
	uchar	szBuff[50];
	
	if( !(glSysParam.ucTermStatus & INIT_MODE) )
	{
		return 0;
	}
	
	TimerSet(0, 0);
	memset(szCurTime,  0, sizeof(szCurTime));
	memset(szLastTime, 0, sizeof(szLastTime));
	while( glSysParam.ucTermStatus & INIT_MODE )
	{
		if( TimerCheck(0)==0 )
		{
			TimerSet(0, 10);
			GetEngTime(szCurTime);
			if (strcmp(szCurTime, szLastTime)!=0)
			{
				Gui_ClearScr();
				sprintf_string(szBuff, "%s\n[%.14s]", _T("PLEASE INIT"), AppInfo.AppName);
				Gui_UpdateTitle(szCurTime, gl_stTitleAttr);
				Gui_DrawText(szBuff, gl_stCenterAttr, 0, 50);
				memcpy(szLastTime, szCurTime, sizeof(szLastTime));
			}
		}

		ucKey = PubWaitKey(10);
		if( ucKey==KEYF1 || ucKey==KEYFN )
		{
			InitTransInfo();
			FunctionInit();
			TimerSet(0, 0);
			memset(szLastTime, 0, sizeof(szLastTime));
		}
	}

	return 0;
}

void MainIdleProc(void)
{
#if !defined(_WIN32)
	// should not let POS go into sleep mode when running simulator
	int		iRet;

	if (ChkTerm(_TERMINAL_S90_))
	{
		if (glSysParam.stEdcInfo.ucIdleShutdown)
		{
			PowerOff();
		}
		else
		{
			// Modified by Kim_LinHB 2014-7-8
			Gui_ClearScr();
			Gui_ShowMsgBox(_T("POWER SAVING"), gl_stTitleAttr, _T("PRESS ANY KEY\nTO RECOVER"), gl_stCenterAttr, GUI_BUTTON_NONE, 0, NULL);

			do 
			{
				iRet = SysSleep("00");
			} while((iRet==-3) && (kbhit()!=0));
			// 很奇怪的现象：除非在上次SysSleep返回之后调用DelayMs(3000)，否则即使间隔1分钟，调用SysSleep仍会直接返回-3。
			// 因此我在这里加了判断，如果返回-3而且没有按键则继续重复SysSleep
			// 在外部已经保证了进入MainIdleProc的间隔>=1分钟
			// it needs to delay 3 seconds after return from SysSleep, otherwise SysSleep will return -3 even the period of calling SysSleep is over 1 min,
			// so here is a special processing, if return -3 from SysSleep and no key was pressed then continue calling SysSleep.

			DelayMs(100);
			kbflush();
			Gui_ClearScr(); // Added by Kim_LinHB 2014-08-13 v1.01.0003
		}
	}
#endif
}

// end of file

