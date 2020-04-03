#ifndef __DISP_MANAGER_H
#define __DISP_MANAGER_H

typedef struct DispOpr {
	char *name;
	int iXres;
	int iYres;
	int iBpp;
	int (*DeviceInit)(void);
	int (*DeviceExit)(void);
	int (*ShowPixel)(int iPenX, int iPenY, unsigned int dwColor);
	int (*CleanScreen)(unsigned int dwBackColor);
	struct DispOpr *ptNext;
}T_DispOpr, *PT_DispOpr;

int RegisterDispOpr(PT_DispOpr ptDispOpr);
int DisplayInit(void);
int FBInit (void);
void ShowDispOpr(void);
PT_DispOpr GetDispOpr(char *pcName);


#endif //_DISP_MANAGER_H

