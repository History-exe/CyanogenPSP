#include "umd.h"

int initUMD(void) 
{
    int UMD = sceUmdCheckMedium();
	if(UMD == 0) 
		return -1;
	UMD = sceUmdActivate(1, "disc0:");
	if(UMD < 0) 
		return -1;
	UMD = sceUmdWaitDriveStat(UMD_WAITFORINIT);
	return UMD;
}

int umdGetTitle(char *title) 
{
    SceUID fd = sceIoOpen("disc0:/UMD_DATA.BIN", PSP_O_RDONLY, 0777);
	if(fd < 0) 
		return -1;
	sceIoRead(fd, title, 10);
	title[10] = 0;
	sceIoClose(fd);
	return 1;
}