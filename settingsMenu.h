#include <pspkernel.h>
#include <pspctrl.h>
#include <pspumd.h>

//PSP Net Stuff
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspwlan.h>

//OSLib
#include <oslib/oslib.h>

//File Management
#include <pspiofilemgr.h>
#include <pspiofilemgr_kernel.h>
#include <pspiofilemgr_dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h> 
#include <stdlib.h> 

#define configFile "system/build.prop"
#define Address "www.google.com"
#define MAX 8

#define stateOff 0
#define stateNoClock 1
#define stateOverClock 2
#define stateUnderClock 3

#define MAX_SETTINGS_DISPLAY		4 // max amount of files displayed on-screen.
#define SETTINGS_DISPLAY_X			20 // X value of where the filebrowser is displayed.
#define SETTINGS_DISPLAY_Y			82 // Y value of the filebrowser is displayed.
#define SETTINGS_CURR_DISPLAY_Y     62 
#define SELECT_ICON_DISPLAY_Y       80 

#define YEAR ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 \
+ (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))

#define MONTH (__DATE__ [2] == 'n' ? 0 \
: __DATE__ [2] == 'b' ? 1 \
: __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 2 : 3) \
: __DATE__ [2] == 'y' ? 4 \
: __DATE__ [2] == 'n' ? 5 \
: __DATE__ [2] == 'l' ? 6 \
: __DATE__ [2] == 'g' ? 7 \
: __DATE__ [2] == 'p' ? 8 \
: __DATE__ [2] == 't' ? 9 \
: __DATE__ [2] == 'v' ? 10 : 11)

#define DAY ((__DATE__ [4] == ' ' ? 0 : __DATE__ [4] - '0') * 10 \
+ (__DATE__ [5] - '0'))

#define DATE_AS_INT (((YEAR - 2000) * 12 + MONTH) * 31 + DAY)

//declaration
OSL_IMAGE *settingsbg, *cursor, *usbdebug, *aboutbg, *offswitch, *onswitch, *displaybg, *performancebg, *wifibg, *developerbg, *about, *highlight, 
		  *developeroptions, *themes, *wifi, *processorbg, *cpuset, *check, *updatesbg, *performance, *recoverybg, *security, *securitybg, *music,
		  *select, *deselect;

//definition of our sounds
OSL_SOUND *tone;

struct fw_version
{
	unsigned int major;
	unsigned int minor;
};

char rgbValuesPath[250], backgroundDataPath[500], settingsFontColorPath[200];

struct settingsFontColor 
{
   int	r;
   int	g;
   int	b;
};

char settingsBgPath[250], displayBgPath[250], aboutBgPath[250], developerBgPath[250], performanceBgPath[250], performance2BgPath[250], 
	 securityBgPath[250], updatesBgPath[250], wifiBgPath[250];
	 
char highlightPath[250], offSwitchPath[250], onSwitchPath[250], selectPath[250];

char aboutPath[250], developerPath[250], performancePath[250], securityPath[250], themesPath[250], wifiPath[250];

char keyBoardBgPath[250], cursorPath[250];

char apolloBgPath[250], apolloSelectorPath[250], nowplayingBgPath[250];

char fmBgPath[250], fmSelectorPath[250], diriconPath[250];

char galleryBgPath[250], gallerySelectorPath[250];

char gameBgPath[250], gameSelectorPath[250];

char quickSettingsBgPath[250], controlsPath[250], navbarPath[250], navbar2Path[250], backdropPath[250], navbarHighlightPath[250], navbarHighlight2Path[250], volumeControlPath[250];

typedef struct fw_version fw_version;

int connectAPCallback(int state);
int connectToAP(int config);
char backgroundPath[500], nickname[25], pspname[25], fontPath[100], themeDirPath[250];
int widgetActivator, eDesktopActivator, bootAnimActivator, gBootActivator, processorState, hrTime, language, usbStat, isPlaying, batteryM, experimentalF;
int r, g, b;
float fontSize;
int passProtect;
int DARK;
int updateReady;
void onlineUpdater();
void flashUpdate();
void switchStatus(int n);
int getCpuClock();
int getBusClock();
void pspGetModel(int x, int y);
void aboutMenu();
void creditsMenu();
void updatesMenu();
void performanceMenu();
void setCpuBoot();
void processorMenu();
void ramMenu();
void storageMenu();
void batteryMenu();
void setPowerManagement();
void displayMenu();
void displayThemes();
void displayFontMenu();
void displayFontSizeMenu();
void settingsDisplay();
void settingsUp();
void settingsDown();
void settingsUpx5();
void settingsDownx5();
void changeFont();
int changeLanguage();
void replaceAsset(char tempData[], char path[], char imgPath[], char redirectPath[]);
void themesLoad();
void themesReload();
void iconPackLoad();
void iconPackReload();
void settingsControls(int n);
char * settingsBrowse(const char * path, int n);
void displaySubThemes(char * browseDirectory, int n);
void displayTime();
void displayMiscellaneous();
void securityMenu();
void wifiMenu();
void developerMenu();
void settingsHighlight();
void settingsDeleteResources();
void settingsMenu();