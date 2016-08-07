#include <oslib/oslib.h>

#include <pspnet_resolver.h>
#include <psphttp.h>

//definition of our Images
OSL_IMAGE *background, *cursor, *ic_allapps, *ic_allapps_pressed, *ic_launcher_apollo, *ic_launcher_settings, *ic_launcher_messenger, *ic_launcher_browser, 
		  *pointer, *pointer1, *notif, *batt100, *batt80, *batt60, *batt40, *batt20, *batt10, *batt0, *battcharge, *transbackground, *notif2, *debug, 
		  *brightness, *control, *navbar2,  *welcome, *quickSettings, *playing, *navbar, *wDay, *wNight, *layerA, *layerB, *layerC, *layerD, *volumeBar,
		  *volumeControl, *navbarHighlight, *navbarHighlight2;
		  
//definition of our sounds
OSL_SOUND *camera_click, *KeypressStandard, *Lock, *LowBattery, *Unlock, *WirelessChargingStarted;

//definition of our font
OSL_FONT *Roboto;

char timeAndBatteryFontColorPath[200];

struct timeAndBatteryStatusFontColor 
{
   int	r;
   int	g;
   int	b;
};

//kernel function imports
int getBrightness(void);
void setBrightness(int brightness);
int displayEnable(void);
int displayDisable(void);

int imposeGetVolume();
int imposeSetVolume();
int imposeGetBrightness();
int imposeSetBrightness(int value);
int imposeGetBacklightOffTime();
int imposeSetBacklightOffTime(int value);

void set_volume(int vol);
void increase_volume(int n);
void decrease_volume(int n);

void internet();
void controls();
void battery(int batx, int baty, int n);
void volumeController();
void appDrawerIcon();
void navbarButtons(int n);
void androidQuickSettings();
void dayNightCycleWidget();
void homeUnloadAssets();
void home();