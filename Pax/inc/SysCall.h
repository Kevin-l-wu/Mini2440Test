#ifndef _APPMANGEMENT_H_
#define _APPMANGEMENT_H_


#define MAX_RSA_MODULUS_LEN 128 
#define MAX_RSA_PRIME_LEN 128


#define RET_OK 0

//Basic System

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

#define SSL_OPER_OK 0
#define SSL_BEYOND_CERF_NUM -401
#define SSL_BEYOND_SERV_IP -402 //Too many servers.
#define SSL_FILE_OPER_FAIL -403
#define SSL_CER_NO_EXIST -404 //Certificate does not exist.
#define SSL_CER_NOSPACE -405 //Certificate space is not enough
#define SSL_NULL_ERR -406 //Null pointer.
#define SSL_BUF_EXCEEDED -407 //The pointed memory space is too large. 
#define SSL_CONV_FORMAT_ERR -408 //Decoding error.
#define SSL_IP_ERR -409 //IP analysis error.


typedef struct ssl_buf_s{ 
	void *ptr; /* Memory pointer*/ 
	int size; /* Space size*/ 
} ST_SSL_BUF;

typedef struct{ 
	int CharSet; /* Character setting */ 
	int Width; /* Width */ 
	int Height; /* Height */ 
	int Bold; /* Bold */ 
	int Italic; /* Italic */ 
} ST_FONT;



uchar SystemInit(void);

void Beep(void);

void Beef(uchar mode, ushort DlyTime);

void BeepF(uchar mode, ushort DlyTime);

uchar SetTime(uchar *time);

void GetTime(uchar *time);

void DelayMs(ushort Ms);

void TimerSet(uchar TimerNo, ushort Cnts);

ushort TimerCheck(uchar TimerNo);

uint GetTimerCount(void);

void ReadSN(uchar *SerialNo);

int ReadCSN(uchar BufferLen, uchar *CSN);

void EXReadSN(uchar *SN);

uchar ReadVerInfo(uchar *VerInfo);

int GetTermInfo(uchar *out_info);

int ReadFontLib(ulong Offset, uchar *FontData, int ReadLen);

int EnumFont (ST_FONT *Fonts, int MaxFontNums);

int Reboot();

void PciGetRandom(uchar *random);

int SysSleep(uchar *DownCtrl);

void SysIdle(void);

uchar OnBase(void);

uchar BatteryCheck(void);

void PowerOff(void);

int GetTermInfoExt(uchar *InfoOut, int InfoOutLen);

int EnumBaseFont (ST_FONT *Fonts, int MaxFontNums);

uchar GetEnv(char *name, uchar *value);

uchar PutEnv(char *name, uchar *value);

int SslSaveCerPrivkey(uchar *ServIp, ushort ServPort, ST_SSL_BUF CertChains[], int CertChainsNum, ST_SSL_BUF *Crl, ST_SSL_BUF LocalCerts[], int LocalCertsNum, uchar KeyIdx, ST_SSL_BUF *LocalPrivatekey, uchar FormatType);

int SslDelCerPrivkey(unsigned char *ServIp, unsigned short ServPort);

int SysConfig(uchar *ConfigInfoIn, int InfoInLen);

void LedDisplay(unsigned char type, unsigned char *str);

int SetWakeupChannel (uint index, uchar *attr);




//Operation

#define RSA_KEY_RET_ERR_BIT -1 //Parameter iProtoKeyBit error
#define RSA_KEY_RET_ERR_PUKE -2 //Parameter iPubEType error 
#define RSA_KEY_RET_ERR_DATA -3 //Generate data failed
#define RSA_KEY_RET_ERR_RANDOM -4 //Random data failed 
#define RSA_KEY_RET_ERR_ENCRY -5 //Encryption operation failed
#define RSA_KEY_RET_ERR_DECRY -6 //Decryption operation failed 
#define RSA_KEY_RET_ERR_VERIF -7 //Encryption and decryption authentication failed

#define SHA_TYPE_1 0 //SHA-1 
#define SHA_TYPE_224 1 //SHA-224
#define SHA_TYPE_256 2 //SHA-256 
#define SHA_TYPE_384 3 //SHA-384 
#define SHA_TYPE_512 4 //SHA-512

#define DECRYPT 0
#define ENCRYPT 1

typedef struct { 
	unsigned short int bits; /* length in bits of modulus */ 
	unsigned char modulus[MAX_RSA_MODULUS_LEN]; /* modulus */ 
	unsigned char exponent[MAX_RSA_MODULUS_LEN]; /* public exponent */ 
} R_RSA_PUBLIC_KEY;

typedef struct { 
	unsigned short int bits; /* length in bits of modulus */ 
	unsigned char modulus[MAX_RSA_MODULUS_LEN]; /* modulus */ 
	unsigned char publicExponent[MAX_RSA_MODULUS_LEN]; /* public exponent */ 
	unsigned char exponent[MAX_RSA_MODULUS_LEN]; /* private exponent */ 
	unsigned char prime[2][MAX_RSA_PRIME_LEN]; /* prime factors */ 
	unsigned char primeExponent[2][MAX_RSA_PRIME_LEN]; /* exponents for CRT */ 
	unsigned char coefficient[MAX_RSA_PRIME_LEN]; /* CRT coefficient */ 
} R_RSA_PRIVATE_KEY;

void des(uchar *input, uchar *output, uchar *deskey, int mode);

void Hash(uchar* DataIn, uint DataInLen, uchar* DataOut);

int RSARecover(uchar *pbyModule, uint dwModuleLen, uchar *pbyExp, uint dwExpLen, uchar *pbyDataIn, uchar *pbyDataOut);

int RSAKeyPairGen(R_RSA_PUBLIC_KEY *pPublicKeyOut, R_RSA_PRIVATE_KEY *pPrivateKeyOut, int iProtoKeyBit, int iPubEType);

int RSAKeyPairVerify(R_RSA_PUBLIC_KEY PublicKey, R_RSA_PRIVATE_KEY PrivateKey);

int SHA( int Mode, const unsigned char *DataIn, int DataInLen, unsigned char *ShaOut);

int AES(const unsigned char *Input, unsigned char *Output, const unsigned char *AesKey, int KeyLen, int Mode);

extern unsigned int errno;

//KeyBoard
#define NOKEY 0x00
#define KEY0 0x30
#define KEY1 0x31
#define KEY2 0x32
#define KEY3 0x33
#define KEY4 0x34
#define KEY5 0x35
#define KEY6 0x36
#define KEY7 0x37
#define KEY8 0x38
#define KEY9 0x39

#define KEYFN 0x15
#define KEYMENU 0x14
#define KEYALPHA 0x07
#define KEYUP 0x05
#define KEYDOWN 0x06
#define KEYCANCEL 0x1B
#define KEYCLEAR 0x08
#define KEYENTER 0x0D

#define KEYATM1 0x3B
#define KEYATM2 0x3C
#define KEYATM3 0x3D
#define KEYATM4 0x3E

#define KEYF1 0x01
#define KEYF2 0x02
#define KEYF3 0x03
#define KEYF5 0x09

#define FNKEY0 0x90
#define FNKEY1 0x91
#define FNKEY2 0x92
#define FNKEY3 0x93
#define FNKEY4 0x94
#define FNKEY5 0x95
#define FNKEY6 0x96
#define FNKEY7 0x97
#define FNKEY8 0x98
#define FNKEY9 0x99
#define FNKEYCLEAR 0x9A
#define FNKEYALPHA 0x9B
#define FNKEYUP 0x9C
#define FNKEYDOWN 0x9D
#define FNKEYMENU 0x9E
#define FNKEYENTER 0x9F
#define FNKEYCANCEL 0xA0
#define FNKEYATM1 0xA1

#define FNKEYATM2 0xA2
#define FNKEYATM3 0xA3
#define FNKEYATM4 0xA4
#define FNKEYF2 0xA5
#define FNKEYF3 0xA6

#define KEYLEFT 0x83

#define KEYRIGHT 0x89 
#define KEYSTAR 0x2A 
#define KEYNUM 0x23 

#define KEYFLASH 0x8B 
#define KEYRD 0x8C 
#define KEYHF 0x8D 
#define KEYPBOOK 0x85 
#define KEYPAY 0x86 
#define KEYPHONE 0x87 
#define KEYBACK 0x88 
#define KEYMSG 0x8A 
#define KEYDIALED 0x8E 
#define KEYSET 0x81 
/*Follows are key values of T620:
?add‘
KEYADD
0x2B
?subtract‘
KEYSUB
0x2D
?multiply‘
KEYMUL
0x26
?dot‘
KEYDOT
0x2E
?Function‘+ ?add‘
FNKEYADD
0xBB
?Function‘+ ?subtract‘
FNKEYSUB
0xBD
?Function‘+ ?multiply‘
FNKEYMUL
0xB6
?Function‘+ ?dot‘
FNKEYDOT
0xBE

*/

uchar kbhit(void);

void kbflush(void);

uchar getkey(void);

void kbmute(uchar flag);

uchar GetString(uchar *str, uchar mode, uchar minlen, uchar maxlen);

uchar GetHzString(uchar *outstr, uchar max, ushort TimeOut);

void kblight(uchar mode);

void KbLock(uchar mode);

int KbCheck(int iCmd);



//LCD

void ScrCls(void);

void ScrClrLine(uchar startline, uchar endline);

void ScrGray(uchar mode);

void ScrBackLight(uchar mode);

void ScrGotoxy(uchar x, uchar y);

int ScrSelectFont(ST_FONT *SingleCodeFont, ST_FONT *MultiCodeFont);

uchar ScrFontSet(uchar fontsize);

uchar ScrAttrSet(uchar attr);

void Lcdprintf_string(uchar *fmt, ...);

void ScrPrint(uchar col, uchar row, uchar mode, char*str,...);

void ScrPlot(uchar X, uchar Y, uchar Color);

void ScrDrLogo(uchar *logo);

void ScrDrLogoxy(int x, int y, uchar *logo);

uchar ScrRestore(uchar mode);

void ScrSetIcon(uchar icon_no, uchar mode);

void ScrGotoxyEx (int pixel_X, int pixel_Y);

void ScrGetxyEx(int *pixel_X, int *pixel_Y);

void ScrDrawRect(int left, int top, int right, int bottom);

void ScrClrRect(int left, int top, int right, int bottom);

void ScrSpaceSet(int CharSpace, int LineSpace);

void ScrGetLcdSize(int *width, int *height);

void ScrTextOut(int pixel_X, int pixel_Y, unsigned char *txt);

void ScrSetOutput(int device);

void ScrSetEcho(int mode);



//Color Screen

typedef struct { 
	unsigned int width; /* The width of the available area(pixel) */ 
	unsigned int height; /* The height of the available area(pixel)*/ 
	unsigned int ppl; /* The number of pixels in one character line */ 
	unsigned int ppc; /* The number of pixels in one character column */ 
	unsigned int fgColor; /* Foreground color */ 
	unsigned int bgColor; /* Background color */ 
	int reserved[8]; /* Reserved */ 
} ST_LCD_INFO;

int CLcdInit();

int CLcdGetInfo (ST_LCD_INFO *pstLcdInfo);

int CLcdSetFgColor(uint color);

int CLcdSetBgColor (uint color);

int CLcdBgDrawImg (int x, int y, const char *filename);

int CLcdBgDrawBox (uint left, uint top, uint right, uint bottom, uint color);

int CLcdDrawPixel (uint x, uint y, uint color);

int CLcdGetPixel (uint x, uint y, uint *color);

int CLcdDrawRect (uint left, uint top, uint right, uint bottom, uint color);

int CLcdClrRect (uint left, uint top, uint right, uint bottom, uint mode);

int CLcdTextOut (uint x, uint y, char *fmt, ...);

int CLcdPrint (uint offset, uint line, uint mode, char *fmt, ...);

int CLcdClrLine (uint startline, uint endline);

int CLcdBgDrawGif (int x, int y, const char *filename);

int CLcdBgClrGif (uint gifno);



//Touch Screen

typedef struct{ 
	unsigned char sampleRateModel;/* Sample rate mode: 0 – button-pressing sample rate; 1 – signature sample rate */ 
	unsigned char reserved[32];/* Reserved,it must be 0 */ 
} TS_ATTR_T;

typedef struct{ 
	int x; /* x coordinate value */ 
	int y; /* y coordinate value */ 
	int pressure; /* Pressure: 0: pressure-free; 100: has pressure; other values: reserved */ 
	int reserved[2];/* Reserved */ 
} TS_POINT_T;


int TouchScreenOpen (unsigned int mode);

void TouchScreenClose (void);

void TouchScreenAttrSet (TS_ATTR_T *pstTsAttr);

void TouchScreenFlush (void);

int TouchScreenRead (TS_POINT_T *pstTsPoint, unsigned int num);




//MSR

typedef struct{ 
	unsigned char RetCode; /* The result of application */ 
	unsigned char track1[256];/* Info buffer of track 1*/ 
	unsigned char track2[256]; /* Info buffer of track 2 */ 
	unsigned char track3[256]; /* Info buffer of track 3 */ 
} ST_MAGCARD;


void MagOpen(void);

void MagClose(void);

void MagReset(void);

uchar MagSwiped(void);

uchar MagRead(uchar *Track1, uchar *Track2, uchar *Track3);

uchar MagGetMagnePrint(uchar *pucMagnePrint);



//ICC Reader


typedef struct{ 
	unsigned char Command[4];  	/*CLA, INS, P1, P2 */
	unsigned short Lc; 			/* The data length of sending */
	unsigned char DataIn[512]; 	/* The data of sending to ICC */
	unsigned short Le;			/* The expected returned length */ 
} APDU_SEND;  

typedef struct{
	unsigned short LenOut; 
	unsigned char DataOut[512]; 
	unsigned char SWA;
	unsigned char SWB; 
} APDU_RESP;


uchar IccInit(uchar slot, uchar *ATR);

void IccClose(uchar slot); 

void IccAutoResp(uchar slot, uchar autoresp);

uchar IccIsoCommand(uchar slot, APDU_SEND *ApduSend, APDU_RESP *ApduRecv);

uchar IccDetect(uchar slot);


//RF Reader

#define PICC_LED_RED 0x01 
#define PICC_LED_GREEN 0x02 
#define PICC_LED_YELLOW 0x04 
#define PICC_LED_BLUE 0x08

typedef struct{ 
	uchar drv_ver[5]; /* The version information of Driver‘s, for example: ―1.01A‖ can only be read */ 
	uchar drv_date[12]; /* The date information of driver, such as:‖2006.08.25‖; Read only */ 
	uchar a_conduct_w; /* The output conductance of A card is write enable:1_allowed,Others—not allowed*/ 
	uchar a_conduct_val; /* The A card control variable of output conductance */ 
	uchar m_conduct_w; /* The output conductance of M1 card is write enable*/ 
	uchar m_conduct_val; /* The M1 card control variable of output conductance */ 
	uchar b_modulate_w; /* The modulation index of B card is written enable*/ 
	uchar b_modulate_val; /* The B card control variable of modulation index*/ 
	uchar card_buffer_w; /* Receiving buffer of card is written enable*/ 
	ushort card_buffer_val; /* Receiving buffer parameter of card (Unit: byte), valid: 1~256. when over 256,it will use 256;if it is 0,it will not be written in */ 
	uchar wait_retry_limit_w; /* Written enable for S(WTX) response to sending times: only applicable to P80 */ 
	ushort wait_retry_limit_val; /* The most repeat times of S(WTX) response, default is 3, only applicable to P80 */
	uchar card_type_check_w; /*Card type check is allow to write */ 
	uchar card_type_check_val; /* 0-Check card type, other-do not check the card type(default to check the card type ) */ 
	uchar card_RxThreshold_w; /*Receiver sensitivity of the B card is allow to write: 1--allowed, others--not allowed. The value is unreadable */ 
	uchar card_RxThreshold_val; /*Receiver sensitivity of the B card */ 
	uchar f_modulate_w; /* felica modulation depth allow to write */ 
	uchar f_modulate_val; /* felica modulation depth */ 
	uchar a_modulate_w; 
	uchar a_modulate_val; 
	uchar a_card_RxThreshold_w; 
	uchar a_card_RxThreshold_val; /* The modulation index of A card is written enable: 1--allowed, others--not allowed. */ /*The A card control variable of modulation index*/ /* Receiver sensitivity of the A card is allowed to write: 1--allowed, others--not allowed. */ /* Receiver sensitivity of the A card */ 
	uchar a_card_antenna_gain_w; 
	uchar a_card_antenna_gain_val; 
	uchar b_card_antenna_gain_w; 
	uchar b_card_antenna_gain_val; 
	uchar f_card_antenna_gain_w; 
	uchar f_card_antenna_gain_val;/*Antennagain of A card*/ /*Antennagain of B card*/ /*Antennagain of C card */
	uchar f_card_RxThreshold_w; 
	uchar f_card_RxThreshold_val; 
	uchar f_conduct_w; 
	uchar f_conduct_val; 
	uchar user_control_w; 
	uchar user_control_val; 
	uchar protocol_layer_fwt_set_w; 
	uint protocol_layer_fwt_set_val; 
	uchar picc_cmd_exchange_set_w; 
	uchar picc_cmd_exchange_set_val; 
	uchar reserved[60]; /* Receiver sensitivity of Felica*/ /*Electrical conductivity of Felica*/ /* Requirements for paypass certification, when press the specified key value, it will force to exit the transaction */ /*Settings of the protocol_layer timeouts */ /*PiccCmdExchange transceiving settings */ /*Reserved byte, for future expansion. All clear when writing */
} PICC_PARA;

uchar PiccOpen(void);

uchar PiccSetup (uchar mode, PICC_PARA *picc_para);

uchar PiccDetect(uchar Mode, uchar *CardType, uchar *SerialInfo, uchar *CID, uchar *Other);

uchar PiccIsoCommand(uchar cid, APDU_SEND *ApduSend, APDU_RESP *ApduRecv);

uchar PiccRemove(uchar mode, uchar cid);

void PiccClose(void);

uchar M1Authority(uchar Type, uchar BlkNo, uchar *Pwd, uchar *SerialNo);

uchar M1ReadBlock(uchar BlkNo, uchar *BlkValue);

uchar M1WriteBlock(uchar BlkNo, uchar *BlkValue);

uchar M1Operate(uchar Type, uchar BlkNo, uchar *Value, uchar UpdateBlkNo);

void PiccLight(uchar ucLedIndex, uchar ucOnOff);

uchar PiccInitFelica(uchar ucRate, uchar ucPol);

uchar PiccCmdExchange(uint uiSendLen, uchar* paucInData, uint* puiRecLen, uchar* paucOutData);




//Printer

#define PRN_OK 0x00
#define PRN_BUSY 0X01


uchar PrnInit(void);

int PrnSelectFont(ST_FONT *SingleCodeFont, ST_FONT *MultiCodeFont);

void PrnFontSet(uchar Ascii, uchar CFont);

void PrnSpaceSet(uchar x, uchar y);

void PrnStep(short pixel);

uchar PrnStr(char *str, ...);

uchar PrnLogo(uchar *logo);

uchar PrnStart(void);

uchar PrnStatus(void);

void PrnLeftIndent(ushort Indent);

int PrnGetDotLine(void);

void PrnSetGray(int Level);

int PrnGetFontDot(int FontSize, uchar *str, uchar *OutDot);

void PrnDoubleWidth(int AscDoubleWidth, int LocalDoubleWidth);

void PrnDoubleHeight(int AscDoubleHeight, int LocalDoubleHeight);

void PrnAttrSet(int Reverse);

int PrnConfig(uchar mode, const char *ImageFile);

int PrnImage(const char *ImageFile, uchar ImageType);



//PED


#define PED_RET_ERR_START -300 							//Start value of error code.
#define PED_RET_ERR_END -500 							//End value of error code. 
#define PED_RET_OK 0 									//Return values of PED are correct.
#define PED_RET_ERR_NO_KEY PED_RET_ERR_START-1 			//Key does not exist. 
#define PED_RET_ERR_KEYIDX_ERR PED_RET_ERR_START-2 		//Key index error, parameter index is not in the range.
#define PED_RET_ERR_DERIVE_ERR PED_RET_ERR_START-3 		//When key is written, the source key level is lower than the destination level. 
#define PED_RET_ERR_CHECK_KEY_FAIL PED_RET_ERR_START-4 	//Key verification failed.
#define PED_RET_ERR_NO_PIN_INPUT PED_RET_ERR_START-5 	//No PIN input 
#define PED_RET_ERR_INPUT_CANCEL PED_RET_ERR_START-6 	//Cancel to enter PIN.
#define PED_RET_ERR_WAIT_INTERVAL PED_RET_ERR_START-7 	//Calling function interval is less than minimum interval time. 
#define PED_RET_ERR_CHECK_MODE_ERR PED_RET_ERR_START-8 	//KCV mode error, do not support.
#define PED_RET_ERR_NO_RIGHT_USE PED_RET_ERR_START-9 	//Not allowed to use the key. When key label is not correct or source key type\
														is bigger than destination key type, PED will return this code. 
#define PED_RET_ERR_KEY_TYPE_ERR PED_RET_ERR_START-10 	//Key type error
#define PED_RET_ERR_EXPLEN_ERR PED_RET_ERR_START-11 	//Expected PIN length string error 
#define PED_RET_ERR_DSTKEY_IDX_ERR PED_RET_ERR_START-12 //Destination key index error
#define PED_RET_ERR_SRCKEY_IDX_ERR PED_RET_ERR_START-13 //Source key index


#define PED_RET_ERR_KEY_LEN_ERR PED_RET_ERR_START-14 	//Key length error
#define PED_RET_ERR_INPUT_TIMEOUT PED_RET_ERR_START-15 	//PIN input timeout 
#define PED_RET_ERR_NO_ICC PED_RET_ERR_START-16 		//IC card does not exist
#define PED_RET_ERR_ICC_NO_INIT PED_RET_ERR_START-17 	//IC card is not initialized 
#define PED_RET_ERR_GROUP_IDX_ERR PED_RET_ERR_START-18 	//DUKPT index error
#define PED_RET_ERR_PARAM_PTR_NULL PED_RET_ERR_START-19 //Pointer parameter error 
#define PED_RET_ERR_LOCKED PED_RET_ERR_START-20 		//PED locked
#define PED_RET_ERROR PED_RET_ERR_START-21				//PED general error 
#define PED_RET_ERR_NOMORE_BUF PED_RET_ERR_START-22 	//No free buffer
#define PED_RET_ERR_NEED_ADMIN PED_RET_ERR_START-23 	//Not administration 
#define PED_RET_ERR_DUKPT_OVERFLOW PED_RET_ERR_START-24 //DUKPT overflow
#define PED_RET_ERR_KCV_CHECK_FAIL PED_RET_ERR_START-25 //KCV check error 
#define PED_RET_ERR_SRCKEY_TYPE_ERR PED_RET_ERR_START-26 //Source key type error.
#define PED_RET_ERR_UNSPT_CMD PED_RET_ERR_START-27 		//Command not support 
#define PED_RET_ERR_COMM_ERR PED_RET_ERR_START-28 		//Communication error
#define PED_RET_ERR_NO_UAPUK PED_RET_ERR_START-29		//No user authentication public key 
#define PED_RET_ERR_ADMIN_ERR PED_RET_ERR_START-30 		//Administration error
#define PED_RET_ERR_DOWNLOAD_INACTIVE PED_RET_ERR_START-31 //PED download inactive 
#define PED_RET_ERR_KCV_ODD_CHECK_FAIL PED_RET_ERR_START-32 //KCV parity check fail






#define PED_TLK 0x01 //TLK
#define PED_TMK 0x02 //TMK 
#define PED_TPK 0x03 //PIN Key
#define PED_TAK 0x04 //MAC Key 
#define PED_TDK 0x05 //DES Key 
#define PED_TIK 0x07 //DUKPT Key 
#define PED_TAESK 0x20 //AES Key

typedef struct { 
	unsigned int modlen; // Module length of public key 
	unsigned char mod[256]; // Module of public key, significant byte ahead, filling 0 prefix 
	unsigned char exp[4]; // Exponent of public key, significant byte ahead, filling 0 prefix 
	unsigned char iccrandomlen; // Length of random data from IC 
	unsigned char iccrandom[8]; // Random data from ICC 
} RSA_PINKEY;

typedef struct { 
	int iModulusLen; // the length of modulus bits. 
	uchar aucModulus[512]; //if the length of modulus <512 byte, store from right, add 0x00 in left. 
	int iExponentLen; //Exponent length. 
	uchar aucExponent [512]; // when exponent <512 byte, add 0x00 in left. 
	uchar aucKeyInfo[128]; // key information will be customized by application. 
} ST_RSA_KEY;


typedef struct { 
	uchar ucSrcKeyType; /* Type of the source key to diversify this key, could be 
						PED_TLK,PED_TMK,PED_TPK,PED_TAK,PED_TDK, the level should not be less than ucDstKeyType*/ 
	uchar ucSrcKeyIdx; /* Index of the source key to diversify the key, start from 1.
						If the variable is 0,then the key will write in as plaintext */ 
	uchar ucDstKeyType; /* Type of the destination key, could be PED_TLK, PED_TMK, PED_TPK, PED_TAK, PED_TDK */ 
	uchar ucDstKeyIdx; /* Index of the destination key */ 
	int iDstKeyLen; /* Length of the destination key, could be 8,16,24 */ 
	uchar aucDstKeyValue[24]; /* The enciphered key value */ 
} ST_KEY_INFO;

typedef struct { 
	int iCheckMode; /* Check mode */ 
	uchar szCheckBuf[128]; /* The buffer of check data*/ 
} ST_KCV_INFO;

typedef struct { 
	uchar ucSrcKeyType; /* Type of the source key to diversify this key, could be PED_TLK,PED_TMK. */
	uchar ucSrcKeyIdx; /* Index of the source key to diversify the key, start from 1.
						If the variable is 0,then the key will be written in as plaintext */ 
	uchar ucDstKeyType; /* Type of the destination key, must be PED_TAESK */ 
	uchar ucDstKeyIdx; /* Index of the destination key */ 
	int iDstKeyLen; /* Length of the destination key, could be 16,24,32 */ 
	uchar aucDstKeyValue[32]; /*The cipher or plain text of key value*/ 
} ST_AES_KEY_INFO; 

int PedWriteKey(ST_KEY_INFO * KeyInfoIn, ST_KCV_INFO * KcvInfoIn);

int PedWriteTIK(uchar GroupIdx, uchar SrcKeyIdx, uchar KeyLen, uchar * KeyValueIn, 
	uchar * KsnIn, ST_KCV_INFO * KcvInfoIn);

int PedGetPinBlock(uchar KeyIdx, uchar *ExpPinLenIn, uchar * DataIn, uchar *PinBlockOut,
	uchar Mode, ulong TimeOutMs);

int PedGetMac(uchar KeyIdx, uchar *DataIn, ushort DataInLen, uchar *MacOut, uchar Mode);

int PedCalcDES(uchar KeyIdx, uchar * DataIn, ushort DataInLen, uchar * DataOut, uchar Mode);

int PedGetPinDukpt(uchar GroupIdx, uchar *ExpPinLenIn, uchar * DataIn, uchar* KsnOut, 
	uchar * PinBlockOut, uchar Mode, ulong TimeoutMs);

int PedGetMacDukpt(uchar GroupIdx, uchar *DataIn, ushort DataInLen, uchar *MacOut,
	uchar * KsnOut, uchar Mode);

int PedReGenPinBlock (uchar UpdateFlag, ST_KEY_INFO *KeyInfoIn, ST_KCV_INFO *KcvInfoIn, 
	uchar *DataIn, uchar *PinBlockOut, uchar Mode);

int PedVerifyPlainPin(uchar IccSlot, uchar *ExpPinLenIn, uchar *IccRespOut, uchar Mode,
	ulong TimeoutMs);

int PedVerifyCipherPin(uchar IccSlot, uchar *ExpPinLenIn, RSA_PINKEY *RsaPinKeyIn, 
	uchar *IccRespOut, uchar Mode, ulong TimeoutMs);

int PedGetKcv(uchar KeyType, uchar KeyIdx, ST_KCV_INFO *KcvInfoOut);

int PedWriteKeyVar(uchar KeyType, uchar SrcKeyIdx, uchar DstKeyIdx, uchar * XorVarIn, 
	ST_KCV_INFO * KcvInfoIn);

int PedGetVer(uchar * VerInfoOut);

int PedErase();

int PedSetIntervaltime(ulong TPKIntervalTimeMs, ulong TAKIntervalTimeMs);

int PedSetKeyTag(uchar * KeyTagIn);

int PedSetFunctionKey(uchar ucKey);

int PedWriteRsaKey(uchar RSAKeyIndex, ST_RSA_KEY* pstRsakeyIn);

int PedRsaRecover (uchar RSAKeyIndex, uchar *pucDataIn, uchar * pucDataOut, uchar* pucKeyInfoOut);

int PedDukptDes (uchar GroupIdx, uchar KeyVarType, uchar *pucIV, ushort DataInLen, uchar *DataIn, 
	uchar * DataOut, uchar*KsnOut , uchar Mode);

int PedGetDukptKSN(uchar GroupIdx, uchar * KsnOut);

int PedDukptIncreaseKsn (uchar GroupIdx);

int PedGetPinRsa(uchar RsaKeyIdx, uchar *ExpPinLenIn, uchar * DataIn, uchar *PinBlockOut, 
	uchar Mode, ulong TimeOutMs);

int PedReadRsaKey(uchar RSAKeyIndex, ST_RSA_KEY* pstRsakeyOut);

int PedWriteAesKey(ST_AES_KEY_INFO * AesKeyInfoIn, ST_KCV_INFO * KcvInfoIn);

int PedCalcAes(unsigned char KeyIdx, unsigned char * InitVector, const unsigned char *DataIn, 
	unsigned short DataInLen, unsigned char *DataOut, unsigned char Mode);

int PedGenSM2KeyPair(uchar *PvtKey, uchar *PubKey, ushort KeyLenBit);

int PedWriteSM2Key(uchar KeyIdx, uchar KeyType, uchar *KeyValue);

int PedSM2Sign(uchar PubKeyIdx, uchar PvtKeyIdx, uchar *Uid, ushort UidLen, uchar *Input, 
	uint InputLen, uchar *Signature);

int PedSM2Verify(uchar PubKeyIdx, uchar *Uid, ushort UidLen, uchar *Input, uint InputLen, 
	const uchar *Signature);

int PedSM2Recover (uchar KeyIdx, uchar *Input, ushort InputLen, uchar *Output, ushort *OutputLen, 
	uchar Mode);

int PedSM3(uchar *Input, uint InputLen, uchar *Output, uchar Mode);

int PedSM4(uchar KeyIdx, uchar *Initvector, uchar *Input, ushort InputLen, uchar *Output, uchar Mode);

int PedGetMacSM (uchar KeyIdx, uchar *InitVector, uchar *Input, ushort InputLen, uchar *MacOut, uchar Mode);

int PedGetPinBlockSM4(uchar KeyIdx,uchar *ExpPinLenIn, uchar * DataIn, uchar *PinBlockOut, 
	uchar Mode, ulong TimeOutMs);

int PedWriteKeyEncByRsa (const uchar *DataIn, ushort DataInLen, uchar DataInFormat, uchar PrvKeyIdx, 
	uchar DstKeyType, uchar DstKeyIdx, const uchar *KsnIn, ST_KCV_INFO * KcvInfoIn);

int PedSetOfflinePinMode(uchar Mode, uchar TpkIdx, uchar *PinBlock, ushort PinBlockLen);


//Application management

#define MAGCARD_MSG 0x01 //Magcard message
#define ICCARD_MSG 0x02 //IC card message 
#define KEYBOARD_MSG 0x03 //Keyboard message
#define USER_MSG 0x04 //User message

typedef struct { 
	unsigned char AppName[32]; /* Application name, unique */ 
	unsigned char AID[16]; /* Application ID */ 
	unsigned char AppVer[16]; /* Application version number */ 
	unsigned char AppProvider[32]; /* Application supplier */
	unsigned char Descript[64];  /* Application description */ 
	unsigned char DownloadTime[14]; /* Application download time */ 
	unsigned long MainAddress; /* The main function entry address main()*/ 
	unsigned long EventAddress; /* Event processing entry address event_main()*/ 
	unsigned char AppNum; /* Assigned application number */ 
	unsigned char RFU[73]; /* Reserved */ 
} APPINFO;

typedef struct { 
	int MsgType; /* event types (including magcard, key,IC card and custom events)*/ 
	ST_MAGCARD MagMsg; /* magcard data */ 
	unsigned char KeyValue; /* key value */ 
	unsigned char IccSlot; /* Contact IC card slot number */ 
	void *UserMsg; /*User-defined event information */ 
} ST_EVENT_MSG;

int ReadAppInfo(uchar AppNo, APPINFO* ai);

int RunApp(uchar AppNo);

int DoEvent(uchar AppNo, ST_EVENT_MSG *msg);


//File Operation


#define O_RDONLY    00000000  
#define O_WRONLY    00000001  
#define O_RDWR      00000002    
#define O_CREATE     00000100     


#define FILE_EXIST 1 
#define FILE_NOEXIST 2
#define MEM_OVERFLOW 3 
#define TOO_MANY_FILES 4
#define INVALID_HANDLE 5 
#define INVALID_MODE 6 
#define NO_FILESYS 7
#define FILE_NOT_OPENED 8 
#define FILE_OPENED 9
#define END_OVERFLOW 10
#define TOP_OVERFLOW 11
#define NO_PERMISSION 12 
#define FS_CORRUPT 13

typedef struct{ 
	unsigned char fid; /* 文件标识号 */ 
	unsigned char attr; /* 文件的属主 */ 
	unsigned char type; /* 文件类型 */ 
	char name[17]; /* 文件名 */ 
	unsigned long length; /* 文件长度 */ 
} FILE_INFO;

int open(char *filename, uchar mode);

int ex_open(char *filename, uchar mode, uchar* attr);

int read(int fid, uchar *dat, int len);

int write(int fid, uchar *dat, int len);

int close(int fid);

int seek(int fid, long offset, uchar fromwhere);

long filesize(char *filename);

int truncate(int fid, long len);

int remove(const char *filename);

long freesize(void);

int fexist(char *filename);

int GetFileInfo(FILE_INFO* finfo);

int FileToApp(uchar *FileName);

int FileToParam (uchar *FileName, uchar *AppName, int iType);

int FileToFont(uchar *FileName);

int FileToMonitor(uchar *FileName);

int FileToPuk(int PukType, int PukIdx, uchar *FileName);

int FileToDriver(uchar *FileName, uint Type);

int DelAppFile(uchar *AppName);

int GetLastError(void);

void FsRecycle(int NeedSize);

long tell(int fid);



//FAT File System

#define FS_ATTR_C 0x1 //Create if not exist
#define FS_ATTR_D 0x2 //inode is DIR 
#define FS_ATTR_E 0x4 //exclusive
#define FS_ATTR_R 0x8 //Read 
#define FS_ATTR_W 0x10 //Write
#define FS_ATTR_CRWD (FS_ATTR_C | FS_ATTR_R | FS_ATTR_W | FS_ATTR_D)
#define FS_ATTR_RW (FS_ATTR_R|FS_ATTR_W)
#define FS_ATTR_RWD (FS_ATTR_R|FS_ATTR_W|FS_ATTR_D)
#define LONG_NAME_MAX (200*2)
#define SHORT_NAME_MAX (11+1)
#define ATTR_RO 0x01 //read only
#define ATTR_HID 0x02 //hidden 
#define ATTR_SYS 0x04 //system file
#define ATTR_VOL 0x08 //volume ID 
#define ATTR_DIR 0x10 //directory
#define ATTR_ARC 0x20 //archive


typedef struct { 
	char *str; /* Pointer points to the storage space */ 
	int size; /* Storage space of string(unit: 1 byte) */
	int fmt; /*Encoding format, value is NAME_FMT_ASCII */ 
} FS_W_STR; /*File System Width STRing */

typedef struct fs_date_time_s{ 
	long seconds; 
	long minute; 
	long hour; 
	long day; 
	long month; 
	long year; 
} FS_DATE_TIME;

typedef struct { 
	FS_DATE_TIME wrtime; /* lastly write time */
	FS_DATE_TIME crtime; /* create time */ 
	int size; /* file size */ 
	int space; /* space size */ 
	int name_fmt; 
	int attr; /* Attributes*/ 
	int name_size; /* the valid length of name */ 
	char name[LONG_NAME_MAX+4]; 
} FS_INODE_INFO;

#define FS_VER_FAT16 0x0 
#define FS_VER_FAT32 0x1 
typedef struct { 
	int ver;/* version number :FS_VER_FAT16 or FS_VER_FAT32*/ 
	int free_space;/* Free space, unit: byte*/ 
	int used_space;/* Used space,unit byte */ 
} FS_DISK_INFO;


uchar FsOpen(FS_W_STR *name, int attr);
int FsClose(int fd);
int FsDelete(FS_W_STR *name_in);
int FsGetInfo(int fd, FS_INODE_INFO *fs_inode);
int FsRename(FS_W_STR * name, FS_W_STR *new_name);
int FsDirRead(int fd, FS_INODE_INFO *fs_inode, int num);
int FsDirSeek(int fd, int num, int flag);
int FsFileRead(int fd, char *buf, int len);
int FsFileWrite(int fd, char *buf, int len);
int FsFileSeek(int fd, int offset, int flag);
int FsFileTell(int fd);
int FsFileTruncate(int fd, int size, int reserve_space);
int FsSetWorkDir(FS_W_STR *name);
int FsGetWorkDir(FS_W_STR *name);
int FsUdiskIn(void);
int FsGetDiskInfo(int disk_num, FS_DISK_INFO *disk_info);






//Communication Port
#define COM1 0
#define COM2 1
#define COM_BT 7

uchar PortOpen(uchar channel, uchar *Attr);

uchar PortClose(uchar channel);

uchar PortSend(uchar channel, uchar ch);

uchar PortRecv(uchar channel, uchar *ch, uint ms);

uchar PortReset(uchar channel);

uchar PortSends(uchar channel, uchar *str, ushort str_len);

uchar PortTxPoolCheck(uchar channel);

int PortPeep(uchar channel, uchar *buf, ushort len);

int PortRecvs(uchar channel, uchar * pszBuf, ushort usBufLen, ushort usTimeoutMs);

int SetHeartBeat(int SwOn, int MsgLen, const unsigned char *Msg, ushort interval100Ms, int COMn);




//MODEM

typedef struct{ 
	unsigned char DP; /* Set tone/pulse dialing*/ 
	unsigned char CHDT; /* Check dialing tone*/ 
	unsigned char DT1; /* Waiting time from picking up phone to dial (Unit:100ms) */ 
	unsigned char DT2; /* Waiting time of ―,‖ (Unit:100ms) */ 
	unsigned char HT; /* Lasting time of one number in dual tone dialing (Unit:1ms) */ 
	unsigned char WT; /* Time interval between dialing two numbers in dual tone dialing (Unit:10ms) */ 
	unsigned char SSETUP; /* Communication bytes setting (including set up synchronization and asynchronous, baud rate, line, answer tone timeout etc.) */ 
	unsigned char DTIMES; /* Redial times, must >=1 */ 
	unsigned char TimeOut; /* Communication timeout (If there is no data exchange during this time, MODEM will hang up, No timeout when it is 0. Unit:10s) */ 
	unsigned char AsMode; /* Asynchronous communication (Only valid when asynchronous communication) */ 
} COMM_PARA;

uchar ModemReset();

uchar ModemDial(COMM_PARA *MPara, uchar *TelNo, uchar mode);

uchar ModemCheck(void);

uchar ModemTxd(uchar *SendData, ushort len);

uchar ModemRxd(uchar *RecvData, ushort *len);

uchar ModemAsyncGet(uchar *ch);

uchar OnHook(void);

uchar HangOff(void);

ushort ModemExCommand(uchar *CmdStr, uchar *RespData, ushort *Dlen, ushort Timeout10ms);

int PPPLogin(char *name, char *passwd, long auth , int timeout);

void PPPLogout(void);

int PPPCheck(void);



//TCP/IP

#define NET_AF_INET		2	/* Internet IP Protocol 	*/

enum NetSockType {
	NET_SOCK_DGRAM	= 1,
	NET_SOCK_STREAM	= 2,
	NET_SOCK_RAW	= 3,
	NET_SOCK_RDM	= 4,
	NET_SOCK_SEQPACKET	= 5,
	NET_SOCK_DCCP	= 6,
	NET_SOCK_PACKET	= 10,
};

enum NetCmd {
	CMD_TO_SET	= 1,
	CMD_EVENT_GET	= 2,
};

enum SockEvent{
	SOCK_EVENT_READ = 1,
	SOCK_EVENT_WRITE = 2,
	SOCK_EVENT_CONN = 3,
	SOCK_EVENT_NETACCEPT = 4,
	SOCK_EVENT_ERROR = 5,
};

#define RET_TCPCLOSED 1
#define RET_TCPOPENED 2
#define RET_TCPOPENING 3


#define NET_OK 0 //Normal, No errors 
#define NET_ERR_MEM -1 //Memory space is not enough.
#define NET_ERR_BUF -2 //Buffer error 
#define NET_ERR_ABRT -3 //Establish connection failed.
#define NET_ERR_RST -4 //Reset the connection by peer (receive the Reset from peer)
#define NET_ERR_CLSD -5 //Connection closed
#define NET_ERR_CONN -6 //Connection not successfully established 
#define NET_ERR_VAL -7 //Variable invalid
#define NET_ERR_ARG -8 //Parameter invalid 
#define NET_ERR_RTE -9 //Route invalid
#define NET_ERR_USE -10 //Address or port in use 
#define NET_ERR_IF -11 //Hardware error
#define NET_ERR_ISCONN -12 //Connected 
#define NET_ERR_TIMEOUT -13 //Timeout
#define NET_ERR_AGAIN -14 //Requested sources not exist, retrial required 
#define NET_ERR_EXIST -15 //Exist
#define NET_ERR_SYS -16 //System does not support 
#define NET_ERR_PASSWD -17 //Password wrong
#define NET_ERR_MODEM -18 //Modem dialing failed 
#define NET_ERR_LINKDOWN -19 //PPP link is breakdown; redial is required.
#define NET_ERR_LOGOUT -20 //Logout 
#define NET_ERR_PPP -21 //PPP disconnected
#define NET_ERR_STR -22 //String too long 
#define NET_ERR_DNS -23 //Resolving error of domain name.
#define NET_ERR_INIT -24 //Corresponding functional system are not initialized. 
#define NET_ERR_SERV -30 //Do not find PPPoE server.
#define NET_ERR_IRDA_SYN -51 //Restart the landline phone or place on different landline phones(S60, D210 only) 
#define NET_ERR_IRDA_COMM -54 //a failed communications between landline phone and mobile phone.

struct in_addr{
	char ipAddr[17];
};


struct sockaddr_in { 
	char sin_len; /* Length */ 
	char sin_family; /*Address cluster */ 
	short sin_port; /* Port number */ 
	struct in_addr sin_addr; /* Network address */ 
	char sin_zero[8]; /* Spare field */ 
};
	 
struct sockaddr{ 
	char sa_len; /* Length */ 
	char sa_family; /* Address cluster */ 
	char sa_data[14]; /* Protocol address */ 
};

struct net_sockaddr
{
	int* pAddr;
};

typedef unsigned int socklen_t;

int NetSocket(int domain, int type, int protocol);

int NetBind(int socket, struct net_sockaddr *addr, socklen_t addrlen);

int NetConnect(int socket, struct net_sockaddr *addr, socklen_t addrlen);

int NetListen(int socket, int backlog);

int NetAccept(int socket, struct net_sockaddr *addr, socklen_t *addrlen);

int NetSend(int socket, void *buf, int size, int flags);

int NetSendto(int socket, void *buf, int size, int flags, struct net_sockaddr *addr, socklen_t addrlen);

int NetRecv(int socket, void *buf, int size, int flags);

int NetRecvfrom(int socket, void *buf, int size, int flags, struct net_sockaddr *addr, socklen_t *addrlen);

int NetCloseSocket(int socket);

int Netioctl(int socket, int cmd, int arg);

int SockAddrSet(struct net_sockaddr *addr, char *ip_str, unsigned short port);

int SockAddrGet(struct net_sockaddr *addr, char *ip_str, unsigned short *port);

int DnsResolve(char *name, char *result, int len);

int NetPing(char *dst_ip_str, long timeout, int size);

int RouteSetDefault(int ifIndex);

int RouteGetDefault(void);

int NetDevGet(int Dev, char *HostIp, char *Mask, char *GateWay, char *Dns);



//Ethernet

int EthSet(char *host_ip, char *host_mask, char *gw_ip, char *dns_ip);

int EthGet(char *host_ip, char *host_mask, char *gw_ip, char *dns_ip, long *state);

int DhcpStart(void);

void DhcpStop(void);

int DhcpCheck(void);

int PppoeLogin(char *name, char *passwd, int timeout);

void PppoeLogout(void);

int PppoeCheck(void);

int NetAddStaticArp(char *ip_str, unsigned char mac[6]);

void EthSetRateDuplexMode (int mode);

int EthGetFirstRouteMac (const char *dest_ip, unsigned char *mac, int len);

int EthMacGet(unsigned char mac[6]);



//Wireless

#define WL_RET_ERR_PORTINUSE -201 	//Module port occupied
#define WL_RET_ERR_NORSP -202 		//Module no response 
#define WL_RET_ERR_RSPERR -203 		//Module response error
#define WL_RET_ERR_PORTNOOPEN -204 	//Module serial port not opened 
#define WL_RET_ERR_NEEDPIN -205 	//PIN Required
#define WL_RET_ERR_NEEDPUK -206 	//PUK required to decode PIN 
#define WL_RET_ERR_PARAMER -207 	//Parameter error
#define WL_RET_ERR_ERRPIN -208 		//PIN error 
#define WL_RET_ERR_NOSIM -209 		//SIM card not inserted
#define WL_RET_ERR_NOTYPE -210 		//Not supported type 
#define WL_RET_ERR_NOREG -211 		//Not register on the network.
#define WL_RET_ERR_INIT_ONCE -212 	//Module already initialized 
#define WL_RET_ERR_LINEOFF -214 	//Connection broken.
#define WL_RET_ERR_TIMEOUT -216 	//Timeout 
#define WL_RET_ERR_REGING -222 		//Registering on network.
#define WL_RET_ERR_PORTCLOSE -223 	//Close port error. 
#define WL_RET_ERR_MODEVER -225 	//Module version error.
#define WL_RET_ERR_DIALING -226 	//Dialing 
#define WL_RET_ERR_ONHOOK -227 		//Shutting down the module.
#define WL_RET_ERR_RST -228 		//Module reset. 
#define WL_RET_ERR_NOSIG -229 		//No signal.
#define WL_RET_ERR_POWEROFF -230 	//Module power off.

#define WL_RET_ERR_BUSY -231 		//Module busy.
#define WL_RET_ERR_OPENPORT -232 	//Open port error. 

#define WL_RET_ERR_PPP_BRK -299
#define WL_RET_ERR_OTHER -300 		//Other unknown error.

int WlInit(const uchar *SimPinIn);

int WlGetSignal(uchar * SingnalLevelOut);

int WlPppLogin(uchar *APNIn, uchar * UidIn, uchar * PwdIn, long Auth, int TimeOut, int AliveInterval);

void WlPppLogout();

int WlPppCheck(void);

int WlOpenPort(void);

int WlClosePort(void);

int WlSendCmd(const uchar * ATstrIn, uchar *RspOut, ushort Rsplen, ushort TimeOut, ushort Mode);

void WlSwitchPower(uchar Onoff);

int WlSelSim (uchar simno);

int WlAutoStart(unsigned char * pin, unsigned char * APNIn, unsigned char * UidIn, unsigned char * PwdIn, long Auth, int TimeOut, int AliveInterval);

int WlAutoCheck(void);

int WlPppLoginEx(const char *DialNum, const char *apn, char *Uid, char *Pwd, long Auth, int timeout, int AliveInterval);

void WlTcpRetryNum(int value);

void WlSetTcpDetect(int value);

void WlSetDns(char *dns_ip);


//WIFI

#define WIFI_RET_OK 0 					//No error.
#define WIFI_RET_ERROR_NODEV -1 		//Device error. 
#define WIFI_RET_ERROR_NORESPONSE -2 	//Has no response.
#define WIFI_RET_ERROR_NOTOPEN -3 		//WIFI is not open. 
#define WIFI_RET_ERROR_NOTCONN -4 		//Has not connected to AP
#define WIFI_RET_ERROR_NULL -5 			//Parameter is null. 
#define WIFI_RET_ERROR_PARAMERROR -6 	//Parameter error
#define WIFI_RET_ERROR_STATUSERROR -7 	//Status error
#define WIFI_RET_ERROR_AUTHERROR -9 	//Password authentication error
#define WIFI_RET_ERROR_NOAP -10 		//Exception occurs while scanning AP.
#define WIFI_RET_ERROR_IPCONF -11 		//Fail to set or get IP
#define IPLEN 4 						//IP length 
#define KEY_WEP_LEN_MAX 16 				//The maxlength of WEP key
#define KEY_WEP_LEN_64 5
#define KEY_WEP_LEN_128 13
#define KEY_WEP_LEN_152 16
#define KEY_WPA_MAXLEN 63 				//The maxlength of WPA
#define SSID_MAXLEN 32 					//The maxlength of SSID 
#define SCAN_AP_MAX 15 					//the maximum number of scanned AP
#define WLAN_SEC_UNSEC 0 				//Unencrypted 
#define WLAN_SEC_WEP 1 					//WEPencryption
#define WLAN_SEC_WPA_WPA2 2 			//WPA/WPA2 encryption 
#define WLAN_SEC_WPAPSK_WPA2PSK 3 		//WPA-PSK/WPA2-PSK encryption
#define WLAN_SEC_WPAPSK 4 				//WPA-PSK encryption
#define WLAN_SEC_WPA2PSK 5 				//WPA2-PSK encryption
#define WIFI_ROUTE_NUM 12 				//Route number of WIFI

typedef struct { 
	uchar Key[4][KEY_WEP_LEN_MAX]; 
	int KeyLen; 
	int Idx; 
} ST_WEP_KEY; 

typedef struct { 
	/* WEP password data */ /* WEP password length, 5 or 13 or 16*/ /* WEP key index[0..3] */ 
	int DhcpEnable; /* DHCP enable，0-close，1-open */ 
	uchar Ip[IPLEN]; /* Static IP*/
	uchar Mask[IPLEN]; /* Mask */ 
	uchar Gate[IPLEN]; /* Gateway */ 
	uchar Dns[IPLEN]; /* DNS */ 
	ST_WEP_KEY Wep; /* wep password*/ 
	uchar Wpa[KEY_WPA_MAXLEN]; /* WPApassword */ 
	uchar reserved[256]; /*Reserved*/ 
} ST_WIFI_PARAM; 

typedef struct{ 
	uchar Ssid[SSID_MAXLEN]; /* SSID of AP, for the length of AP name which is equal to the SSID_MAXLEN, SSID does not include ‘\0‘; for the length of AP name which is less than SSID_MAXLEN, SSID includes ?\0‘ */ 
	int SecMode; /* Security mode */ 
	int Rssi; /* Signal strength of AP*/ 
} ST_WIFI_AP; 

typedef struct{ 
	ST_WIFI_AP stBasicInfo; 
	unsigned char bssid[6]; 
	unsigned intbssType; /*0 represents unknow type; 1 represents Infrastrucure; 2 represents ad-hoc*/ 
	unsigned char reserved[128]; 
} ST_WIFI_AP_EX;

int WifiOpen(void);

int WifiClose(void);

int WifiScan(ST_WIFI_AP *outAps, int ApCount);

int WifiConnect(ST_WIFI_AP *Ap, ST_WIFI_PARAM *WifiParam);

int WifiDisconnect(void);

int WifiCheck(ST_WIFI_AP *Ap);

int WifiCtrl(int iCmd, void *pArgIn, int iSizeIn , void* pArgOut, int iSizeOut);

int WifiScanEx(ST_WIFI_AP_EX *outAps, unsigned int apCount );


//Bluetooth

#define BT_RET_OK 0 					//No error
#define BT_RET_ERROR_NODEV -1 			//Deviceinitialization failed(it is no not initialized) 
#define BT_RET_ERROR_PARAMERROR -2 	//Parameter error
#define BT_RET_ERROR_STACK -3 			//Stack error 
#define BT_RET_ERROR_TIMEOUT -4 		//timeout
#define BT_RET_ERROR_CONNECTED -5 		//Duplicating connections.

typedef struct { 
	uchar name[64]; /*Device name */ 
	uchar pin[16]; 	/*Device PIN */ 
	uchar mac[6]; 	/*DeviceMAC */ 
	uchar RFU[10]; 	/*Reserved bytes */ 
} ST_BT_CONFIG; 

typedef struct { 
	uchar name[64]; /* Device name */ 
	uchar mac[6]; 	/* DeviceMAC */ 
	uchar RFU[10]; 	/* Reserved bytes */ 
} ST_BT_SLAVE; 

typedef struct { 
	uchar status; /* connection status： 0 represents not connected, 1 represents connected 2 represents being connected*/ 
	uchar name[64]; /*The connected device name */ 
	uchar mac[6]; /* The connected deviceMAC */ 
} ST_BT_STATUS; 

typedef struct{ 
	unsigned char base_bt_exist; /* 0 represents it has no Bluetooth on the base; 1 represents it has Bluetooth */ 
	unsigned char base_bt_mac[6]; /* base bluetooth mac */
	unsigned char base_bt_pincode[16]; /* base bluetooth pincode */ 
	unsigned char base_bt_name[20]; /* base bluetooth name, it can take the first 20bytes at most. */ 
} ST_BT_BASE_INFO;


int BtOpen(void);

int BtClose(void);

int BtGetConfig(ST_BT_CONFIG *pCfg);

int BtSetConfig(ST_BT_CONFIG *pCfg);

int BtScan(ST_BT_SLAVE *pSlave, unsigned int Cnt, unsigned int TimeOut);

int BtConnect(ST_BT_SLAVE *Slave);

int BtDisconnect(void);

int BtGetStatus(ST_BT_STATUS *pStatus);

int BtCtrl(unsigned int iCmd, void *pArgIn, unsigned int iSizeIn, void* pArgOut, unsigned int iSizeOut);



//Remote Download

typedef struct { 
	unsigned char bCommMode; /*0:Serial, 1:Modem, 2:LAN, 3:GPRS, 4:CDMA, 5:PPP, 6:WIFI*/ 
	unsigned char *psAppName; /*Application name, blank means download all applications, parameter and data files */ 
	unsigned char bLoadType; /*Download type,bit0:application,bit1:parameter file,bit2:data file */ 
	unsigned char psProtocol; /*Protocol:0-protims;1-ftims;2-tims*/ 
	unsigned char bPedMode; /*0-External PED(eg.PP20) 1-Internal PED(P60-S1,P80 Only used for FTims, no need for ProTims and Tims */ 
	unsigned char bDMK_index; /*Master key index, no need for ProTims */ 
	unsigned char sTransCode[5]; /*Transaction code, only used for FTims, no need for ProTims and Tims */ 
								/* Value Description 
								"2001": Single application download transactions 
								"2011": Multi-application downloads transactions. 
								"1000": Single application of POS check downloads. 
								"1005": Single application POS check and download a within once communication process. 
								"1010": Multi-application POS query downloads. 
								"1015": Multi-application POS check and download a within once communication process. */ 
	unsigned char ucCallMode; /* Calling mode, Current not used in the domestic version of ProTims?FTims and Times 2005-09-26*/
								/*Used in the new oversea version of ProTims, mainly for Hong Kong P80 2005-10-17*/ 
								/*Applications call this interface to control. 
								bit0~bit3: 
									0: Operator requested; 
									1: Scheduled time; 
									2: Called by the remote server. Require monitor to receive the update message of server 
										(when interface return, monitor cannot be broken); 
									3: (v0.8.0) Called by the remote server. Request Monitor to process remote download */ 
									/* If bit4~bit7=0, The monitor of P70 need to use internal serial port to operate modem?*/
									/*bit4~bit7: 0:monitor do not need to establish communication links;1:monitor establish a connection by itself */ 
	unsigned char *psTermID; /*Terminal num. Current used in oversea version of PtoTims and the new domestic version of ProTims 2006-04-18*/ 
	union{ 
		struct{ 
			unsigned char *psPara; /*Serial port communication parameter, format as "57600,8,n,1" */ 
		} tSerial; /*Serial port communication parameter, valid when bCommMode=0 */ 
		struct{ 
			unsigned char *psTelNo; /*Tel. number, refer to ModemDial() */ 
			COMM_PARA *ptModPara; /*Modem communication parameter */ 
			unsigned char bTimeout; /*Delay after dial success[unit: second]*/
		} tModem; /*Modem communication parameter, valid when bCommMode=1 */ 
		struct{ 
			unsigned char *psLocal_IP_Addr; /*Local IP address, Must be Used in the TCP/IP mode */ 
			unsigned short wLocalPortNo; /*Local port number, not used now,IP80 module will use the default port number automatically*/ 
			unsigned char *psRemote_IP_Addr; /*Remote IP address. Must be used in the TCP/IP mode */ 
			unsigned short wRemotePortNo; /*Remote port number. Must be used in the TCP/IP mode */ 
			unsigned char *psSubnetMask; /*Subnet mask. Must be used in the TCP/IP mode 2006-04-21*/ 
			unsigned char *psGatewayAddr; /*Gateway. Must be used in the TCP/IP mode 2006-04-21*/ 
		} tLAN; /*TCP/IP communication parameter, valid when bCommMode=2 */ 
		struct { 
			unsigned char *psAPN; /*APN*/ 
			unsigned char *psUserName; /* User ID */ 
			unsigned char *psPasswd; /* User Password */ 
			unsigned char *psIP_Addr; /* IP address */ 
			unsigned short nPortNo; /* Prot No */ 
			unsigned char *psPIN_CODE; /*PIN value if PIN of SIM is activate */
			unsigned char *psAT_CMD; /* If there is AT command */ 
		} tGPRS; /*GPRS communication parameter, valid when bCommMode=3 */ 
		struct { 
			unsigned char *psTelNo; /* Phone num */ 
			unsigned char *psUserName; /* User name */ 
			unsigned char *psPasswd; /* User password */ 
			unsigned char *psIP_Addr; /* IP address */ 
			unsigned short nPortNo; /* Port num */ 
			unsigned char *psPIN_CODE; /*PIN value are needed if PIN of UIM is activate */ 
			unsigned char *psAT_CMD; /* If there is AT command */ 
		} tCDMA; /*CDMA communication parameter, valid when bCommMode=4 */ 
		struct { 
			unsigned char *psTelNo; /* Phone num */ 
			COMM_PARA *ptModPara; /*MODEM communication para */ 
			unsigned char *psUserName; /* User name */ 
			unsigned char *psPasswd; /* User password */ 
			unsigned char *psIP_Addr; /*IP address */ 
			unsigned short nPortNo; /* Port num */ 
		}tPPP; /*Based on the remote download communication parameter of modem Point to Point Protocol. The parameter will valid when bCommMode=5 */
		struct { /* The parameter is valid when WIFI parameter bCommMode=6 */ 
			unsigned char *Wifi_SSID; /*SSID*/ 
			unsigned char *psPasswd; /* User password */ 
			unsigned char *Local_IP; /* Local IP*/ 
			unsigned short Local_PortNo; 
			unsigned char *Remote_IP_Addr; /* Remote IP address*/ 
			unsigned short RemotePortNo; /* Remote port number */ 
			unsigned char *SubnetMask; /* Subnet mask */ 
			unsigned char *GatewayAddr; /* Gateway */ 
			unsigned char *Dns1; 
			unsigned char *Dns2; 
			unsigned char Encryption_Mode; /* 0 no encryption 
											1 WEP encryption; 
											2 WPA/WPA2encryption; 
											3 WPA-PSK/WPA2-PSK encryption 
											4 WPA-PSK encryption 
											5 WPA2-PSK encryption */ 
			unsigned char Encryption_Index; /*WEP Value range :1~4*/ 
			unsigned char DHCP_Flag; /*1: use DHCP*/ 
		} tWIFI; 
	} tUnion; 
} T_INCOMMPARA;

int RemoteLoadApp(const T_INCOMMPARA *ptCommPara);

void ProTimsEntry (void *param);


//G Sensor

void GetLeanAngle(int *piXLeanAngle, int *piYLeanAngle, int *piZLeanAngle);


//Fixed-line Telephone

#define TEL_RET_ERR_INVALID_PARAM -1 	//输入参数非法
#define TEL_RET_ERR_BUF_OVERFLOW -2 	//缓冲区溢出

typedef struct { 
	uchar HandlePicked; 	/* 1,Detect HandlePicked;0, Handle hang up */ 
	uchar RingFound; 		/* 1,Ring found;0,not found */ 
	uchar CidFound; 		/* 1,Caller ID information has been received, it can read the call;0,No call */ 
	uchar CidTimeStr[10]; 	/* month,day,hour,minute:30 35 32 36 30 39 34 39 indicate At 9:49 on May 26 */ 
	uchar CidTelNo[64]; 	/* Caller number Indication,valid values are {0123456789EPOpo} */ 
	uchar CidUserName [64]; /* User information, some calls do not have the information. */ 
} TEL_STATUS;

typedef struct { 
	uchar FskTxlevel; 		/*[0~7][default value:7] modem FSK, level value when sending data */ 
	uchar FskRxlevel; 		/*[0~7] [default value:7]modem FSK, level value when receiving data */ 
	uchar FskMarkTime; 		/* [1~30][units:10MS] [default value:2] The duration of MARK when MODEM fskcommunication */ 
	uchar CidFskGate; 		/* [0~7] [default value:7] the value ofthreshold level when receives FSK caller ID information*/ 
	uchar CidDtmfGate; 		/*[0~7] [default value:7] the value of threshold level when receives DTMF caller ID information */ 
	uchar DtmfOnTime; 		/*[6~255][units:10MS] [default value:9] The duration of sending every number element when DTMF dialing.*/
	uchar DtmfOffTime; 		/*[6~255][ units:10MS] [default value:9] The interval of sending elements of two numbers when DTMF dialing. */ 
	uchar PauseButtonTime; 	/*[0~255][ units:100MS[default value:5]] The paused time when press the pause button */ 
	uchar FirstRingMute; 	/*[ default value:0]>0,the times of mute;0,ring normally */ 
	uchar Reserved[51]; 	/* reserved */ 
} TEL_PARA;

int TelDial(char *TelNum);

void TelCheck(TEL_STATUS *tel_status);

void TelOffHook(void);

void TelSetPara(TEL_PARA *para);


//Voice Function

int SoundPlay(char *param, char type);


//Barcode Module

int ScanOpen (void);

int ScanRead (uchar *buf, uint size);

void ScanClose (void);


//GPS

#define GPS_RET_OK 0 					//Success
#define GPS_RET_NAVIGATING -1 			//Data is being read 
#define GPS_RET_ERROR_NODEV -2 			//Device error
#define GPS_RET_ERROR_NORESPONSE -3 	//GPS module has no response 
#define GPS_RET_ERROR_NOTOPEN -4 		//GPS module is not open
#define GPS_RET_ERROR_NULL -5 			//Parameter is null 
#define GPS_RET_ERROR_PARAMERROR -6 	//Parameter error
#define GPS_RET_ERROR_NAVIGATE_FAIL -7 	//Failed to get navigation data

typedef struct { 
	double latitude; 
	double longitude; /** latitude, unit:degree */ /** longitude, unit:degree*/
	double altitude; 
} GpsLocation;

int GpsOpen(void);

void GpsClose(void);

int GpsRead(GpsLocation *outGpsLoc);





#endif