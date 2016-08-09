#include <oslib/oslib.h>

#include <pspnet.h>	//PSP Net Stuff
#include <pspnet_inet.h>

//PSP USB
#include <pspusb.h>
#include <pspusbstor.h>
#include <pspmodulemgr.h>
#include <kubridge.h>
#include "pspusbdevice.h"

#include <sys/unistd.h>
#include <stdbool.h>

#define Address "www.google.com"

#define musicPath "ms0:/MUSIC"

char tempMessage[20];
char tempPin[5];
char tempData[250];
char buffer[100];

bool connectionInitialized;

enum colors {
    RED =	0xFF0000FF,
    GREEN =	0xFF00FF00,
    BLUE =	0xFFFF0000,
    WHITE =	0xFFFFFFFF,
    LITEGRAY = 0xFFBFBFBF,
    GRAY =  0xFF7F7F7F,
    DARKGRAY = 0xFF3F3F3F,
    BLACK = 0xFF000000,
};

void LowMemExit();
void debugDisplay();
int getCpuClock();
int getBusClock();
void pspGetModel(int x, int y);
int setFileDefaultsInt(char *path, int value, int var);
float setFileDefaultsFloat(char *path, float value, float var);
char * setFileDefaultsChar(char path[], char data[], char var[]);
void removeUpdateZip();
void installRequiredFiles();
void openOSK(char * message, char * initialMessage, int textLimit, int lang);
int isEmpty(FILE *file);
void fadeOut(OSL_IMAGE* bg,int x, int y);
void fadeIn(OSL_IMAGE* bg, int x, int y);
int disableUsb(void);
int enableUsb();
int enableUsbEx(u32 device);
void networkInit();
bool networkInitializeConnection();
bool networkShutDownConnection();
int initNetDialog();
bool networkGetFile(const char *url, const char *filepath);
int connectAPCallback(int state);
int connectToAP(int config);
void onlineUpdater();
void flashUpdate();
int isUSBCableActivated();
int isUSBCableConnected();
char getPSPNickname();