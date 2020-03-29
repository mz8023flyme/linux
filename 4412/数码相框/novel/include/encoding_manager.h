#ifndef _ENCODING_MANAGER_H
#define _ENCODING_MANAGER_H

#include <Fonts_manager.h>
#include <disp_manager.h>

typedef struct EncodingOpr {
	char *name;
	int iHeadLen;
	PT_FontOpr ptFontOprSupportedHead;
	int (*isSupport)(unsigned char *pucBufHead);
	int (*GetCodeFrmBuf)(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode);
	struct EncodingOpr *ptNext;
}T_EncodingOpr, *PT_EncodingOpr;

int EncodingInit(void);
int AsciiEncodingInit(void);
int AddFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr);
int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr);
int  Utf8EncodingInit(void);
int  Utf16leEncodingInit(void);
int  Utf16beEncodingInit(void);
void ShowEncodingOpr(void);
PT_EncodingOpr SelectEncodingOprForFile(unsigned char *pucFileBufHead);
int DelFontOprFrmEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr);


#endif /* _ENCODING_MANAGER_H */


