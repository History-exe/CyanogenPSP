#include <pspkernel.h>  
#include <pspdebug.h> 
#include <pspctrl.h>
#include <psppower.h>
#include <stdio.h>
#include <pspwlan.h>
#include <oslib/oslib.h>

#include <kubridge.h>

#define MAX_FZIP_DISPLAY		10
#define FZIP_DISPLAY_X			10
#define FZIP_DISPLAY_Y			60 
#define FZIP_CURR_DISPLAY_Y     58

#define Version "flash0:/vsh/etc/version.txt"
#define PWD "system/settings/backupPassword.txt"
#define selector_xDistance 0 //The distance between the X axis of the two selections
#define selector_yDistance 20 //The distance between the Y axis of the two selections

int scePowerGetBatteryRemainCapacity(void);

OSL_IMAGE *recoverybg, *Selector;

OSL_FONT *roboto;

void ShowPage5();
void ShowPage4();
void backupPassword();
void ShowPage3();
void ShowPage2();
void ShowPage1();
void ShowVersionTxt();
int ConfigurationMenu();
int ShowAdvancedCnfMenu();
int ShowCnfMenu();
int ShowSystemMenu();
int ShowBatteryMenu();
void unloadRecoveryMenuAssets();
int mainRecoveryMenu();