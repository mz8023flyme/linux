#ifndef __DRAW_H
#define __DRAW_H

#include "Fonts_manager.h"


typedef struct PageDesc {
	int iPage;
	unsigned char *pucLcdFirstPosAtFile;
	unsigned char *pucLcdNextPageFirstPosAtFile;
	struct PageDesc *ptPrePage;
	struct PageDesc *ptNextPage;
} T_PageDesc, *PT_PageDesc;



int SetTextDetail(char *pcHZKFile, char *pcFileFreetype, unsigned int dwFontSize);
int SelectAndInitDisplay(char *pcName);
int OpenTextFile(char *pcFileName);
int ShowNextPage(void);
int ShowOnePage(unsigned char *pucTextFileMemCurPos);
int RelocateFontPos(PT_FontBitMap ptFontBitMap);
int ShowOneFont(PT_FontBitMap ptFontBitMap);
int ShowPrePage(void);



#endif //__DRAW_H

