#ifndef TFTP_H_
#define TFTP_H_

typedef struct _TFTP_PACKAGE
{
	u16 opcode;
	u16 blocknum;
	u8 data[0];
} TFTP_PACKAGE;

void TftpSendRequest();

void TftpSetLoadAddr(unsigned char* address);
void TftpSetFileName(unsigned char* fileName);

#endif