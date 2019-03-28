#include <stdio.h>
#include "SysCall.h"

#include "ModManager.h"
#include "../Protocol/inc/LcdProtocol.h"
#include "../Protocol/inc/ButtonProtocol.h"

unsigned int errno;

//Basic System

uchar SystemInit(void)
{
	print_string("SystemInit()\n");
}

void Beep(void)
{
	print_string("Beep()\n");
}

void Beef(uchar mode, ushort DlyTime)
{
	print_string("Beef()\n");
}

void BeepF(uchar mode, ushort DlyTime)
{
	print_string("BeepF()\n");
}

uchar SetTime(uchar *time)
{
	print_string("SetTime()\n");
}

void GetTime(uchar *time)
{
	print_string("GetTime()\n");
}

void DelayMs(ushort Ms)
{
	print_string("DelayMs()\n");
}

void TimerSet(uchar TimerNo, ushort Cnts)
{
	print_string("TimerSet()\n");
}

ushort TimerCheck(uchar TimerNo)
{
	print_string("TimerCheck()\n");
}

uint GetTimerCount(void)
{
	print_string("GetTimerCount()\n");
}

void ReadSN(uchar *SerialNo)
{
	print_string("ReadSN()\n");
}

int ReadCSN(uchar BufferLen, uchar *CSN)
{
	print_string("ReadCSN()\n");
}

void EXReadSN(uchar *SN)
{
	print_string("EXReadSN()\n");
}

uchar ReadVerInfo(uchar *VerInfo)
{
	print_string("ReadVerInfo()\n");
}

int GetTermInfo(uchar *out_info)
{
	print_string("GetTermInfo()\n");
}

int ReadFontLib(ulong Offset, uchar *FontData, int ReadLen)
{
	print_string("ReadFontLib()\n");
}

int EnumFont (ST_FONT *Fonts, int MaxFontNums)
{
	print_string("EnumFont()\n");
}

int Reboot()
{
	print_string("Reboot()\n");
}

void PciGetRandom(uchar *random)
{
	print_string("PciGetRandom()\n");
}

int SysSleep(uchar *DownCtrl)
{
	print_string("SysSleep()\n");
}

void SysIdle(void)
{
	print_string("SysIdle()\n");
}

uchar OnBase(void)
{
	print_string("OnBase()\n");
}

uchar BatteryCheck(void)
{
	print_string("BatteryCheck()\n");
}

void PowerOff(void)
{
	print_string("PowerOff()\n");
}

int GetTermInfoExt(uchar *InfoOut, int InfoOutLen)
{
	print_string("GetTermInfoExt()\n");
}

int EnumBaseFont (ST_FONT *Fonts, int MaxFontNums)
{
	print_string("EnumBaseFont()\n");
}

uchar GetEnv(char *name, uchar *value)
{
	print_string("GetEnv()\n");
}

uchar PutEnv(char *name, uchar *value)
{
	print_string("PutEnv()\n");
}

int SslSaveCerPrivkey(uchar *ServIp, ushort ServPort, ST_SSL_BUF CertChains[], 
	int CertChainsNum, ST_SSL_BUF *Crl, ST_SSL_BUF LocalCerts[], int LocalCertsNum, 
	uchar KeyIdx, ST_SSL_BUF *LocalPrivatekey, uchar FormatType)
{
	print_string("SslSaveCerPrivkey()\n");	
}

int SslDelCerPrivkey(unsigned char *ServIp, unsigned short ServPort)
{
	print_string("SslDelCerPrivkey()\n");
}

int SysConfig(uchar *ConfigInfoIn, int InfoInLen)
{
	print_string("SysConfig()\n");
}

void LedDisplay(unsigned char type, unsigned char *str)
{
	print_string("LedDisplay()\n");
}

int SetWakeupChannel (uint index, uchar *attr)
{
	print_string("SetWakeupChannel()\n");
}


//Operation

void des(uchar *input, uchar *output, uchar *deskey, int mode)
{
	print_string("des()\n");
}

void Hash(uchar* DataIn, uint DataInLen, uchar* DataOut)
{
	print_string("Hash()\n");
}

int RSARecover(uchar *pbyModule, uint dwModuleLen, uchar *pbyExp, uint dwExpLen,
	uchar *pbyDataIn, uchar *pbyDataOut)
{
	print_string("RSARecover()\n");
}

int RSAKeyPairGen(R_RSA_PUBLIC_KEY *pPublicKeyOut, R_RSA_PRIVATE_KEY *pPrivateKeyOut, 
	int iProtoKeyBit, int iPubEType)
{
	print_string("RSAKeyPairGen()\n");
}

int RSAKeyPairVerify(R_RSA_PUBLIC_KEY PublicKey, R_RSA_PRIVATE_KEY PrivateKey)
{
	print_string("RSAKeyPairVerify()\n");
}

int SHA( int Mode, const unsigned char *DataIn, int DataInLen, unsigned char *ShaOut)
{
	print_string("SHA()\n");
}

int AES(const unsigned char *Input, unsigned char *Output, const unsigned char *AesKey, 
	int KeyLen, int Mode)
{
	print_string("AES()\n");
}



//KeyBoard
ButtonModOps* gPaxKeyOps = NULL;

void KeyBoardInit()
{
	if(gPaxKeyOps == NULL)
	{
		gLocateProtocol(MOD_BUTTON, (void*)&gPaxKeyOps);
	}
	
	if(gPaxKeyOps != NULL)
	{
		gPaxKeyOps->ButtonInit();	
	}
}

void KeyBoardExit()
{
	if(gPaxKeyOps == NULL)
	{
		gLocateProtocol(MOD_BUTTON, (void*)&gPaxKeyOps);
	}
	
	if(gPaxKeyOps != NULL)
	{
		gPaxKeyOps->ButtonExit();	
	}
}


uchar kbhit(void)
{
	print_string("kbhit()\n");
	
	int ret = 0;
	
	if(gPaxKeyOps == NULL)
	{
		gLocateProtocol(MOD_BUTTON, (void*)&gPaxKeyOps);
	}
	
	if(gPaxKeyOps != NULL)
	{
		ret = gPaxKeyOps->ButtonHit();	
	}
	
	return ret;
}

void kbflush(void)
{
	print_string("kbflush()\n");
	
	if(gPaxKeyOps == NULL)
	{
		gLocateProtocol(MOD_BUTTON, (void*)&gPaxKeyOps);
	}
	
	if(gPaxKeyOps != NULL)
	{
		gPaxKeyOps->ButtonFlush();	
	}
}

uchar getkey(void)
{
	print_string("getkey()\n");
	uchar ret = 0;
	
	if(gPaxKeyOps == NULL)
	{
		gLocateProtocol(MOD_BUTTON, (void*)&gPaxKeyOps);
	}
	
	if(gPaxKeyOps != NULL)
	{
		ret = gPaxKeyOps->ButtonGet();	
	}
	
	return ret;
}

void kbmute(uchar flag)
{
	print_string("kbmute()\n");
}

uchar GetString(uchar *str, uchar mode, uchar minlen, uchar maxlen)
{
	print_string("GetString()\n");
}

uchar GetHzString(uchar *outstr, uchar max, ushort TimeOut)
{
	print_string("GetHzString()\n");
}

void kblight(uchar mode)
{
	print_string("kblight()\n");
}

void KbLock(uchar mode)
{
	print_string("KbLock()\n");
}

int KbCheck(int iCmd)
{
	print_string("KbCheck()\n");
}


//LCD

void ScrCls(void)
{
	print_string("ScrCls()\n");
}

void ScrClrLine(uchar startline, uchar endline)
{
	print_string("ScrClrLine()\n");
}

void ScrGray(uchar mode)
{
	print_string("ScrGray()\n");
}

void ScrBackLight(uchar mode)
{
	print_string("ScrBackLight()\n");
}

void ScrGotoxy(uchar x, uchar y)
{
	print_string("ScrGotoxy()\n");
}

int ScrSelectFont(ST_FONT *SingleCodeFont, ST_FONT *MultiCodeFont)
{
	print_string("ScrSelectFont()\n");
}

uchar ScrFontSet(uchar fontsize)
{
	print_string("ScrFontSet()\n");
}

uchar ScrAttrSet(uchar attr)
{
	print_string("ScrAttrSet()\n");
}

void Lcdprintf_string(uchar *fmt, ...)
{
	print_string("Lcdprintf_string()\n");
}

void ScrPrint(uchar col, uchar row, uchar mode, char*str,...)
{
	print_string("ScrPrint()\n");
}

void ScrPlot(uchar X, uchar Y, uchar Color)
{
	print_string("ScrPlot()\n");
}

void ScrDrLogo(uchar *logo)
{
	print_string("ScrDrLogo()\n");
}

void ScrDrLogoxy(int x, int y, uchar *logo)
{
	print_string("ScrDrLogoxy()\n");
}

uchar ScrRestore(uchar mode)
{
	print_string("ScrRestore()\n");
}

void ScrSetIcon(uchar icon_no, uchar mode)
{
	print_string("ScrSetIcon()\n");
}

void ScrGotoxyEx (int pixel_X, int pixel_Y)
{
	print_string("ScrGotoxyEx()\n");
}

void ScrGetxyEx(int *pixel_X, int *pixel_Y)
{
	print_string("ScrGetxyEx()\n");
}

void ScrDrawRect(int left, int top, int right, int bottom)
{
	print_string("ScrDrawRect()\n");
}

void ScrClrRect(int left, int top, int right, int bottom)
{
	print_string("ScrClrRect()\n");
}

void ScrSpaceSet(int CharSpace, int LineSpace)
{
	print_string("ScrSpaceSet()\n");
}

void ScrGetLcdSize(int *width, int *height)
{
	print_string("ScrGetLcdSize()\n");
}

void ScrTextOut(int pixel_X, int pixel_Y, unsigned char *txt)
{
	print_string("ScrTextOut()\n");
}

void ScrSetOutput(int device)
{
	print_string("ScrSetOutput()\n");
}

void ScrSetEcho(int mode)
{
	print_string("ScrSetEcho()\n");
}


static ST_LCD_INFO lcdInfo = {240, 320, 8, 16, 100, 600, {0}};
LcdModOps* gPaxLcdOps = NULL;

//Color Screen


int CLcdInit()
{
	int ret = -1;
	
	if(gPaxLcdOps == NULL)
	{
		gLocateProtocol(MOD_LCD, (void*)&gPaxLcdOps);
	}
	
	if(gPaxLcdOps != NULL)
	{
		gPaxLcdOps->LcdModInit();	
		ret = 0;
	}
	
	return ret;
}

int CLcdGetInfo (ST_LCD_INFO *pstLcdInfo)
{	
	pstLcdInfo->width = lcdInfo.width;
	pstLcdInfo->height = lcdInfo.height;
	pstLcdInfo->ppl = lcdInfo.ppl;
	pstLcdInfo->ppc = lcdInfo.ppc;
	pstLcdInfo->fgColor = lcdInfo.fgColor;
	pstLcdInfo->bgColor = lcdInfo.bgColor;
	
	return 0;
}

int CLcdSetFgColor(uint color)
{
	lcdInfo.fgColor = color;
}

int CLcdSetBgColor (uint color)
{
	lcdInfo.bgColor = color;
}

int CLcdBgDrawImg (int x, int y, const char *filename)
{
	
}

int CLcdBgDrawBox (uint left, uint top, uint right, uint bottom, uint color)
{
	int ret = -1;
	if(gPaxLcdOps == NULL)
	{
		gLocateProtocol(MOD_LCD, (void*)&gPaxLcdOps);
	}
	
	if(gPaxLcdOps != NULL)
	{
		ret = gPaxLcdOps->LcdDrawBox(left, top, right, bottom, color);
	}
	
	return ret;
}

int CLcdDrawPixel (uint x, uint y, uint color)
{
	int ret = -1;
	if(gPaxLcdOps == NULL)
	{
		gLocateProtocol(MOD_LCD, (void*)&gPaxLcdOps);
	}
	
	if(gPaxLcdOps != NULL)
	{
		ret = gPaxLcdOps->LcdDrawPixel(x, y, color);
	}
	
	return ret;
}

int CLcdGetPixel (uint x, uint y, uint *color){
	
	int ret = -1;
	if(gPaxLcdOps == NULL)
	{
		gLocateProtocol(MOD_LCD, (void*)&gPaxLcdOps);
	}
	
	if(gPaxLcdOps != NULL)
	{
		ret = gPaxLcdOps->LcdGetPixel(x, y, (unsigned short*)color);
	}
	
	return ret;
	
}

int CLcdDrawRect (uint left, uint top, uint right, uint bottom, uint color)
{
	int ret = -1;
	if(gPaxLcdOps == NULL)
	{
		gLocateProtocol(MOD_LCD, (void*)&gPaxLcdOps);
	}
	
	if(gPaxLcdOps != NULL)
	{
		ret = gPaxLcdOps->LcdDrawBox(left, top, right, bottom, color);
	}
	
	return ret;
}

int CLcdClrRect (uint left, uint top, uint right, uint bottom, uint mode)
{
	int ret = -1;
	if(gPaxLcdOps == NULL)
	{
		gLocateProtocol(MOD_LCD, (void*)&gPaxLcdOps);
	}
	
	if(gPaxLcdOps != NULL)
	{
		ret = gPaxLcdOps->LcdDrawBox(left, top, right, bottom, mode);
	}
	
	return ret;
}

int CLcdTextOut (uint x, uint y, char *fmt, ...)
{
	int ret = -1;
	
	if((x < lcdInfo.width) && (y < lcdInfo.height) && (fmt != NULL))
	{
		if(gPaxLcdOps == NULL)
		{
			gLocateProtocol(MOD_LCD, (void*)&gPaxLcdOps);
		}
		
		if(gPaxLcdOps != NULL)
		{
			gPaxLcdOps->LcdDisplayStringWithColor(x, y, fmt, (unsigned short)lcdInfo.fgColor);
			ret = 0;
		}
	}
	return ret;
}

int CLcdPrint (uint offset, uint line, uint mode, char *fmt, ...)
{
	
}

int CLcdClrLine (uint startline, uint endline)
{
	
}

int CLcdBgDrawGif (int x, int y, const char *filename)
{
	
}

int CLcdBgClrGif (uint gifno)
{
	
}


//Touch Screen

int TouchScreenOpen (unsigned int mode)
{
	
}

void TouchScreenClose (void)
{
	
}

void TouchScreenAttrSet (TS_ATTR_T *pstTsAttr)
{
	
}

void TouchScreenFlush (void)
{
	
}

int TouchScreenRead (TS_POINT_T *pstTsPoint, unsigned int num)
{
	
}


//MSR
void MagOpen(void)
{
	
}

void MagClose(void)
{
	
}

void MagReset(void)
{
	
}

uchar MagSwiped(void)
{
	
}

uchar MagRead(uchar *Track1, uchar *Track2, uchar *Track3)
{
	
}

uchar MagGetMagnePrint(uchar *pucMagnePrint)
{
	
}


//ICC Reader

uchar IccInit(uchar slot, uchar *ATR)
{
	
}

void IccClose(uchar slot)
{
	
}

void IccAutoResp(uchar slot, uchar autoresp)
{
	
}

uchar IccIsoCommand(uchar slot, APDU_SEND *ApduSend, APDU_RESP *ApduRecv)
{
	
}

uchar IccDetect(uchar slot)
{
	
}



//RF Reader

uchar PiccOpen(void)
{
	
}

uchar PiccSetup (uchar mode, PICC_PARA *picc_para)
{
	
}

uchar PiccDetect(uchar Mode, uchar *CardType, uchar *SerialInfo, uchar *CID, uchar *Other)
{
	
}

uchar PiccIsoCommand(uchar cid, APDU_SEND *ApduSend, APDU_RESP *ApduRecv)
{
	
}

uchar PiccRemove(uchar mode, uchar cid)
{
	
}

void PiccClose(void)
{
	
}

uchar M1Authority(uchar Type, uchar BlkNo, uchar *Pwd, uchar *SerialNo)
{
	
}

uchar M1ReadBlock(uchar BlkNo, uchar *BlkValue)
{
	
}

uchar M1WriteBlock(uchar BlkNo, uchar *BlkValue)
{
	
}

uchar M1Operate(uchar Type, uchar BlkNo, uchar *Value, uchar UpdateBlkNo)
{
	
}

void PiccLight(uchar ucLedIndex, uchar ucOnOff)
{
	
}

uchar PiccInitFelica(uchar ucRate, uchar ucPol)
{
	
}

uchar PiccCmdExchange(uint uiSendLen, uchar* paucInData, uint* puiRecLen, uchar* paucOutData)
{
	
}



//Printer

uchar PrnInit(void)
{
	
}

int PrnSelectFont(ST_FONT *SingleCodeFont, ST_FONT *MultiCodeFont)
{
	print_string("PrnSelectFont()\n");
}

void PrnFontSet(uchar Ascii, uchar CFont)
{
	
}

void PrnSpaceSet(uchar x, uchar y)
{
	
}

void PrnStep(short pixel)
{
	
}

uchar PrnStr(char *str, ...)
{
	
}

uchar PrnLogo(uchar *logo)
{
	
}

uchar PrnStart(void)
{
	
}

uchar PrnStatus(void)
{
	
}

void PrnLeftIndent(ushort Indent)
{
	
}

int PrnGetDotLine(void)
{
	
}

void PrnSetGray(int Level)
{
	
}

int PrnGetFontDot(int FontSize, uchar *str, uchar *OutDot)
{
	
}

void PrnDoubleWidth(int AscDoubleWidth, int LocalDoubleWidth)
{
	
}

void PrnDoubleHeight(int AscDoubleHeight, int LocalDoubleHeight)
{
	
}

void PrnAttrSet(int Reverse)
{
	
}

int PrnConfig(uchar mode, const char *ImageFile)
{
	
}

int PrnImage(const char *ImageFile, uchar ImageType)
{
	
}



//PED

int PedWriteKey(ST_KEY_INFO * KeyInfoIn, ST_KCV_INFO * KcvInfoIn)
{
	
}

int PedWriteTIK(uchar GroupIdx, uchar SrcKeyIdx, uchar KeyLen, uchar * KeyValueIn, 
	uchar * KsnIn, ST_KCV_INFO * KcvInfoIn)
{
	
}

int PedGetPinBlock(uchar KeyIdx, uchar *ExpPinLenIn, uchar * DataIn, uchar *PinBlockOut,
	uchar Mode, ulong TimeOutMs)
	{
	
}

int PedGetMac(uchar KeyIdx, uchar *DataIn, ushort DataInLen, uchar *MacOut, uchar Mode)
{
	
}

int PedCalcDES(uchar KeyIdx, uchar * DataIn, ushort DataInLen, uchar * DataOut, uchar Mode)
{
	
}

int PedGetPinDukpt(uchar GroupIdx, uchar *ExpPinLenIn, uchar * DataIn, uchar* KsnOut, 
	uchar * PinBlockOut, uchar Mode, ulong TimeoutMs)
	{
	
}

int PedGetMacDukpt(uchar GroupIdx, uchar *DataIn, ushort DataInLen, uchar *MacOut,
	uchar * KsnOut, uchar Mode)
	{
	
}

int PedReGenPinBlock (uchar UpdateFlag, ST_KEY_INFO *KeyInfoIn, ST_KCV_INFO *KcvInfoIn, 
	uchar *DataIn, uchar *PinBlockOut, uchar Mode)
	{
	
}

int PedVerifyPlainPin(uchar IccSlot, uchar *ExpPinLenIn, uchar *IccRespOut, uchar Mode,
	ulong TimeoutMs)
	{
	
}

int PedVerifyCipherPin(uchar IccSlot, uchar *ExpPinLenIn, RSA_PINKEY *RsaPinKeyIn, 
	uchar *IccRespOut, uchar Mode, ulong TimeoutMs)
	{
	
}

int PedGetKcv(uchar KeyType, uchar KeyIdx, ST_KCV_INFO *KcvInfoOut)
{
	
}

int PedWriteKeyVar(uchar KeyType, uchar SrcKeyIdx, uchar DstKeyIdx, uchar * XorVarIn, 
	ST_KCV_INFO * KcvInfoIn)
	{
	
}

int PedGetVer(uchar * VerInfoOut)
{
	
}

int PedErase()
{
	
}

int PedSetIntervaltime(ulong TPKIntervalTimeMs, ulong TAKIntervalTimeMs)
{
	
}

int PedSetKeyTag(uchar * KeyTagIn)
{
	
}

int PedSetFunctionKey(uchar ucKey)
{
	
}

int PedWriteRsaKey(uchar RSAKeyIndex, ST_RSA_KEY* pstRsakeyIn)
{
	
}

int PedRsaRecover (uchar RSAKeyIndex, uchar *pucDataIn, uchar * pucDataOut, uchar* pucKeyInfoOut)
{
	
}

int PedDukptDes (uchar GroupIdx, uchar KeyVarType, uchar *pucIV, ushort DataInLen, uchar *DataIn, 
	uchar * DataOut, uchar*KsnOut , uchar Mode)
	{
	
}

int PedGetDukptKSN(uchar GroupIdx, uchar * KsnOut){
	
}

int PedDukptIncreaseKsn (uchar GroupIdx)
{
	
}

int PedGetPinRsa(uchar RsaKeyIdx, uchar *ExpPinLenIn, uchar * DataIn, uchar *PinBlockOut, 
	uchar Mode, ulong TimeOutMs)
	{
	
}

int PedReadRsaKey(uchar RSAKeyIndex, ST_RSA_KEY* pstRsakeyOut)
{
	
}

int PedWriteAesKey(ST_AES_KEY_INFO * AesKeyInfoIn, ST_KCV_INFO * KcvInfoIn)
{
	
}

int PedCalcAes(unsigned char KeyIdx, unsigned char * InitVector, const unsigned char *DataIn, 
	unsigned short DataInLen, unsigned char *DataOut, unsigned char Mode)
	{
	
}

int PedGenSM2KeyPair(uchar *PvtKey, uchar *PubKey, ushort KeyLenBit)
{
	
}

int PedWriteSM2Key(uchar KeyIdx, uchar KeyType, uchar *KeyValue)
{
	
}

int PedSM2Sign(uchar PubKeyIdx, uchar PvtKeyIdx, uchar *Uid, ushort UidLen, uchar *Input, 
	uint InputLen, uchar *Signature)
	{
	
}

int PedSM2Verify(uchar PubKeyIdx, uchar *Uid, ushort UidLen, uchar *Input, uint InputLen, 
	const uchar *Signature)
	{
	
}

int PedSM2Recover (uchar KeyIdx, uchar *Input, ushort InputLen, uchar *Output, ushort *OutputLen, 
	uchar Mode)
	{
	
}

int PedSM3(uchar *Input, uint InputLen, uchar *Output, uchar Mode)
{
	
}

int PedSM4(uchar KeyIdx, uchar *Initvector, uchar *Input, ushort InputLen, uchar *Output, uchar Mode)
{
	
}

int PedGetMacSM (uchar KeyIdx, uchar *InitVector, uchar *Input, ushort InputLen, uchar *MacOut, uchar Mode)
{
	
}

int PedGetPinBlockSM4(uchar KeyIdx,uchar *ExpPinLenIn, uchar * DataIn, uchar *PinBlockOut, 
	uchar Mode, ulong TimeOutMs)
	{
	
}

int PedWriteKeyEncByRsa (const uchar *DataIn, ushort DataInLen, uchar DataInFormat, uchar PrvKeyIdx, 
	uchar DstKeyType, uchar DstKeyIdx, const uchar *KsnIn, ST_KCV_INFO * KcvInfoIn)
	{
	
}

int PedSetOfflinePinMode(uchar Mode, uchar TpkIdx, uchar *PinBlock, ushort PinBlockLen)
{
	
}



//Application management

int ReadAppInfo(uchar AppNo, APPINFO* ai)
{
	
}

int RunApp(uchar AppNo)
{
	
}

int DoEvent(uchar AppNo, ST_EVENT_MSG *msg)
{
	
}



//File Operation

int open(char *filename, uchar mode)
{
	print_string("open()\n");
}

int ex_open(char *filename, uchar mode, uchar* attr)
{
	print_string("ex_open()\n");
}

int read(int fid, uchar *dat, int len)
{
	print_string("read()\n");
}

int write(int fid, uchar *dat, int len)
{
	print_string("write()\n");
}

int close(int fid)
{
	print_string("close()\n");
}

int seek(int fid, long offset, uchar fromwhere)
{
	print_string("seek()\n");
}

long filesize(char *filename)
{
	print_string("filesize()\n");
}

int truncate(int fid, long len)
{
	print_string("truncate()\n");
}

int remove(const char *filename)
{
	print_string("remove()\n");
}

long freesize(void)
{
	print_string("freesize()\n");
}

int fexist(char *filename)
{
	print_string("fexist()\n");
}

int GetFileInfo(FILE_INFO* finfo)
{
	print_string("GetFileInfo()\n");
}

int FileToApp(uchar *FileName)
{
	print_string("FileToApp()\n");
}

int FileToParam (uchar *FileName, uchar *AppName, int iType)
{
	print_string("FileToParam()\n");
}

int FileToFont(uchar *FileName)
{
	print_string("FileToFont()\n");
}

int FileToMonitor(uchar *FileName)
{
	print_string("FileToMonitor()\n");
}

int FileToPuk(int PukType, int PukIdx, uchar *FileName)
{
	print_string("FileToPuk()\n");
}

int FileToDriver(uchar *FileName, uint Type)
{
	print_string("FileToDriver()\n");
}

int DelAppFile(uchar *AppName)
{
	print_string("DelAppFile()\n");
}

int GetLastError(void)
{
	print_string("GetLastError()\n");
}

void FsRecycle(int NeedSize)
{
	print_string("FsRecycle()\n");
}

long tell(int fid)
{
	print_string("tell()\n");
}



//FAT File System

uchar FsOpen(FS_W_STR *name, int attr)
{
	
}

int FsClose(int fd)
{
	
}

int FsDelete(FS_W_STR *name_in)
{
	
}
int FsGetInfo(int fd, FS_INODE_INFO *fs_inode)
{
	
}
int FsRename(FS_W_STR * name, FS_W_STR *new_name)
{
	
}
int FsDirRead(int fd, FS_INODE_INFO *fs_inode, int num)
{
	
}
int FsDirSeek(int fd, int num, int flag){
	
}
int FsFileRead(int fd, char *buf, int len)
{
	
}

int FsFileWrite(int fd, char *buf, int len)
{
	
}

int FsFileSeek(int fd, int offset, int flag)
{
	
}

int FsFileTell(int fd)
{
	
}

int FsFileTruncate(int fd, int size, int reserve_space)
{
	
}

int FsSetWorkDir(FS_W_STR *name)
{
	
}

int FsGetWorkDir(FS_W_STR *name)
{
	
}

int FsUdiskIn(void)
{
	
}

int FsGetDiskInfo(int disk_num, FS_DISK_INFO *disk_info)
{
	
}


//Communication Port

uchar PortOpen(uchar channel, uchar *Attr)
{
	
}

uchar PortClose(uchar channel)
{
	
}

uchar PortSend(uchar channel, uchar ch)
{
	
}

uchar PortRecv(uchar channel, uchar *ch, uint ms)
{
	
}

uchar PortReset(uchar channel)
{
	
}

uchar PortSends(uchar channel, uchar *str, ushort str_len)
{
	
}

uchar PortTxPoolCheck(uchar channel)
{
	
}

int PortPeep(uchar channel, uchar *buf, ushort len)
{
	
}

int PortRecvs(uchar channel, uchar * pszBuf, ushort usBufLen, ushort usTimeoutMs)
{
	
}

int SetHeartBeat(int SwOn, int MsgLen, const unsigned char *Msg, ushort interval100Ms, int COMn)
{
	
}



//MODEM


uchar ModemReset()
{
	
}

uchar ModemDial(COMM_PARA *MPara, uchar *TelNo, uchar mode)
{
	
}

uchar ModemCheck(void)
{
	
}

uchar ModemTxd(uchar *SendData, ushort len)
{
	
}

uchar ModemRxd(uchar *RecvData, ushort *len)
{
	
}

uchar ModemAsyncGet(uchar *ch)
{
	
}

uchar OnHook(void)
{
	
}

uchar HangOff(void)
{
	
}

ushort ModemExCommand(uchar *CmdStr, uchar *RespData, ushort *Dlen, ushort Timeout10ms)
{
	
}

int PPPLogin(char *name, char *passwd, long auth , int timeout)
{
	
}

void PPPLogout(void)
{
	
}

int PPPCheck(void)
{
	
}




//TCP/IP

int NetSocket(int domain, int type, int protocol)
{
	
}

int NetBind(int socket, struct net_sockaddr *addr, socklen_t addrlen)
{
	
}

int NetConnect(int socket, struct net_sockaddr *addr, socklen_t addrlen)
{
	
}

int NetListen(int socket, int backlog)
{
	
}

int NetAccept(int socket, struct net_sockaddr *addr, socklen_t *addrlen)
{
	
}

int NetSend(int socket, void *buf, int size, int flags)
{
	
}

int NetSendto(int socket, void *buf, int size, int flags, struct net_sockaddr *addr, socklen_t addrlen)
{
	
}

int NetRecv(int socket, void *buf, int size, int flags)
{
	
}

int NetRecvfrom(int socket, void *buf, int size, int flags, struct net_sockaddr *addr, socklen_t *addrlen)
{
	
}

int NetCloseSocket(int socket)
{
	
}

int Netioctl(int socket, int cmd, int arg)
{
	
}

int SockAddrSet(struct net_sockaddr *addr, char *ip_str, unsigned short port)
{
	
}

int SockAddrGet(struct net_sockaddr *addr, char *ip_str, unsigned short *port)
{
	
}

int DnsResolve(char *name, char *result, int len)
{
	
}

int NetPing(char *dst_ip_str, long timeout, int size)
{
	
}

int RouteSetDefault(int ifIndex)
{
	
}

int RouteGetDefault(void)
{
	
}

int NetDevGet(int Dev, char *HostIp, char *Mask, char *GateWay, char *Dns)
{
	
}



//Ethernet

int EthSet(char *host_ip, char *host_mask, char *gw_ip, char *dns_ip)
{
	
}

int EthGet(char *host_ip, char *host_mask, char *gw_ip, char *dns_ip, long *state)
{
	
}

int DhcpStart(void)
{
	
}

void DhcpStop(void)
{
	
}

int DhcpCheck(void)
{
	
}

int PppoeLogin(char *name, char *passwd, int timeout)
{
	
}

void PppoeLogout(void)
{
	
}

int PppoeCheck(void)
{
	
}

int NetAddStaticArp(char *ip_str, unsigned char mac[6])
{
	
}

void EthSetRateDuplexMode (int mode)
{
	
}

int EthGetFirstRouteMac (const char *dest_ip, unsigned char *mac, int len)
{
	
}

int EthMacGet(unsigned char mac[6])
{
	
}



//Wireless


int WlInit(const uchar *SimPinIn)
{
	print_string("WlInit()\n");
}

int WlGetSignal(uchar * SingnalLevelOut)
{
	print_string("WlGetSignal()\n");
}

int WlPppLogin(uchar *APNIn, uchar * UidIn, uchar * PwdIn, long Auth, int TimeOut, int AliveInterval)
{
	print_string("WlPppLogin()\n");
}

void WlPppLogout()
{
	print_string("WlPppLogout()\n");
}

int WlPppCheck(void)
{
	print_string("WlPppCheck()\n");
}

int WlOpenPort(void)
{
	print_string("WlOpenPort()\n");
}

int WlClosePort(void)
{
	print_string("WlClosePort()\n");
}

int WlSendCmd(const uchar * ATstrIn, uchar *RspOut, ushort Rsplen, ushort TimeOut, ushort Mode)
{
	print_string("WlSendCmd()\n");
}

void WlSwitchPower(uchar Onoff)
{
	print_string("WlSwitchPower()\n");
}

int WlSelSim (uchar simno)
{
	print_string("WlSelSim()\n");
}

int WlAutoStart(unsigned char * pin, unsigned char * APNIn, unsigned char * UidIn, unsigned char * PwdIn, long Auth, int TimeOut, int AliveInterval)
{
	print_string("WlAutoStart()\n");
}

int WlAutoCheck(void)
{
	print_string("WlAutoCheck()\n");
}

int WlPppLoginEx(const char *DialNum, const char *apn, char *Uid, char *Pwd, long Auth, int timeout, int AliveInterval)
{
	print_string("WlPppLoginEx()\n");
}

void WlTcpRetryNum(int value)
{
	print_string("WlTcpRetryNum()\n");
}

void WlSetTcpDetect(int value)
{
	print_string("WlSetTcpDetect()\n");
}

void WlSetDns(char *dns_ip)
{
	print_string("WlSetDns()\n");
}



//WIFI

int WifiOpen(void)
{
	print_string("WifiOpen()\n");
}

int WifiClose(void)
{
	print_string("WifiClose()\n");
}

int WifiScan(ST_WIFI_AP *outAps, int ApCount)
{
	print_string("WifiScan()\n");
}

int WifiConnect(ST_WIFI_AP *Ap, ST_WIFI_PARAM *WifiParam)
{
	print_string("WifiConnect()\n");
}

int WifiDisconnect(void)
{
	print_string("WifiDisconnect()\n");
}

int WifiCheck(ST_WIFI_AP *Ap)
{
	print_string("WifiCheck()\n");
}

int WifiCtrl(int iCmd, void *pArgIn, int iSizeIn , void* pArgOut, int iSizeOut)
{
	print_string("WifiCtrl()\n");
}

int WifiScanEx(ST_WIFI_AP_EX *outAps, unsigned int apCount )
{
	print_string("WifiScanEx()\n");
}



//Bluetooth

int BtOpen(void)
{
	print_string("BtOpen()\n");
}

int BtClose(void)
{
	print_string("BtClose()\n");
}

int BtGetConfig(ST_BT_CONFIG *pCfg)
{
	print_string("BtGetConfig()\n");
}

int BtSetConfig(ST_BT_CONFIG *pCfg)
{
	print_string("BtSetConfig()\n");
}

int BtScan(ST_BT_SLAVE *pSlave, unsigned int Cnt, unsigned int TimeOut)
{
	print_string("BtScan()\n");
}

int BtConnect(ST_BT_SLAVE *Slave)
{
	print_string("BtConnect()\n");
}

int BtDisconnect(void)
{
	print_string("BtDisconnect()\n");
}

int BtGetStatus(ST_BT_STATUS *pStatus)
{
	print_string("BtGetStatus()\n");
}

int BtCtrl(unsigned int iCmd, void *pArgIn, unsigned int iSizeIn, void* pArgOut, unsigned int iSizeOut)
{
	print_string("BtCtrl()\n");
}



//Remote Download

int RemoteLoadApp(const T_INCOMMPARA *ptCommPara)
{
	
}

void ProTimsEntry (void *param)
{
	
}

//G Sensor

void GetLeanAngle(int *piXLeanAngle, int *piYLeanAngle, int *piZLeanAngle)
{
	print_string("GetLeanAngle()\n");
}



//Fixed-line Telephone

int TelDial(char *TelNum)
{
	print_string("TelDial()\n");
}

void TelCheck(TEL_STATUS *tel_status)
{
	print_string("TelCheck()\n");
}

void TelOffHook(void)
{
	print_string("TelOffHook()\n");
}

void TelSetPara(TEL_PARA *para)
{
	print_string("TelSetPara()\n");
}



//Voice Function
int SoundPlay(char *param, char type)
{
	print_string("SoundPlay()\n");
}



//Barcode Module

int ScanOpen (void)
{
	print_string("ScanOpen()\n");
}

int ScanRead (uchar *buf, uint size)
{
	print_string("ScanRead()\n");
}

void ScanClose (void)
{
	print_string("ScanClose()\n");
}



//GPS Module
int GpsOpen (void)
{
	print_string("GpsOpen()\n");
}

void GpsClose (void)
{
	print_string("GpsClose()\n");
}

int GpsRead(GpsLocation *outGpsLoc)
{
	print_string("GpsRead()\n");
}








